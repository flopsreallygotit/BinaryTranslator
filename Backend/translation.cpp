#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/mman.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "opcodes.hpp"
#include "stackUtils.hpp"
#include "translation.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getGrammar (LONG_FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_TOKEN->type == NAME)
    {
        if (isFunction == true)
        {
            char *name = CURRENT_NAME;
            
            NEXT_TOKEN;

            node_t *node  = nodeConstructor(NAME, {.name = name}, NULL);

            CHECK_ALL_OK;

            node->left  = getGrammar(ARGUMENTS, false);
            node->right = getGrammar(ARGUMENTS, false);

            return node;
        }

        else
        {
            node_t *node = NEW_CURRENT_VARIABLE_NODE;

            CHECK_ALL_OK;

            NEXT_TOKEN;

            return node;
        }
    }

    else if (CURRENT_TOKEN->type == VALUE)
    {
        node_t *node = NEW_CURRENT_VALUE_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        return node;
    }

    else if (CURRENT_OPTION == NIL  ||
             CURRENT_OPTION == VOID || // TODO
             CURRENT_OPTION == TYPE)
    {
        NEXT_TOKEN;

        return NULL;
    }

    else if (CURRENT_OPTION == TERMINATION_SYM)
        return NULL;

    else
    {
        OPTIONS Option = CURRENT_OPTION;

        node_t *leftNode  = NULL;
        node_t *rightNode = NULL;
        
        NEXT_TOKEN;

        if (Option == FUNC || Option == CALL)
            leftNode = getGrammar(ARGUMENTS, true);  

        else
            leftNode = getGrammar(ARGUMENTS, false);        

        rightNode = getGrammar(ARGUMENTS, false);

        node_t *node = NEW_NODE(Option);

        CHECK_ALL_OK;

        node->left  = leftNode;
        node->right = rightNode;
        
        return node;  
    }
}

// void putVoidsAndMakeCorrectRets (node_t *node)
// {
//     if (node->type == OPTION && 
//         node->data.option == FUNC)
//         node->left->right = 

//     if (node->left != NULL)
//         putVoidsAndMakeCorrectRets(node->left);

//     if (node->right != NULL)
//         putVoidsAndMakeCorrectRets(node->right);

//     return;
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

binary_t *binaryConstructor ()
{
    binary_t *code = (binary_t *) calloc (1, sizeof(binary_t));

    code->data = (char *) calloc (PageSize, PageSize);
    memset(code->data, 0xC3, PageSize);

    code->size = PageSize;
    code->dataSize = 0;

    return code;
}

void binaryDestructor (binary_t *code)
{
    if (code == NULL)
        return;

    free(code->data);
    free(code);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void codeAppend (binary_t *code, const char *data, size_t dataSize)
{
    if (code == NULL || 
        data == NULL ||
        dataSize == 0)
        return;

    if (code->size - code->dataSize < dataSize)
    {
        while (code->size - code->dataSize < dataSize)
            code->size *= 2;

        code->data = (char *) realloc (code->data, code->size * sizeof(char));        
    }

    memcpy(code->data + code->dataSize, data, dataSize);

    code->dataSize += dataSize;

    return;
}

static void codeCopy (binary_t *code, size_t destination, void *argument, size_t size)
{
    if (code == NULL)
        return;

    if (destination == code->dataSize)
    {
        if (code->size - code->dataSize < size)
        {
            while (code->size - code->dataSize < size)
                code->size *= 2;

            code->data = (char *) realloc (code->data, code->size);
        }

        code->dataSize += size;
    }

    memcpy(code->data + destination, argument, size);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void programConstructor (program_t *program, node_t *node)
{
    if (program == NULL)
        return;

    program->node = node;

    program->functionsArray = (function_t *) calloc (MaxFunctionsCount, sizeof(function_t));
    program->functionNumber = 0;

    program->code = binaryConstructor();

    program->stack = stackConstructor(MaxTableSize);
    
    return;
}

void programDestructor (program_t *program)
{
    if (program == NULL)
        return;

    for (size_t tableIndex = 0; tableIndex < program->stack->size; tableIndex++)
		tableDestructor(program->stack->data[tableIndex]);
	
	stackDestructor(program->stack);	
    binaryDestructor(program->code);
    free(program->functionsArray);

    *program = {};

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

table_t *tableConstructor (stack_t *stack)
{
    if (stack == NULL)
        return NULL;

    table_t *table = (table_t *) calloc (1, sizeof(table_t));

    if (stack->size == 0)
        table->registerPosition = 0;

    else
    {
        table_t *currentTable = stack->data[stack->size - 1];

        table->registerPosition = currentTable->registerPosition + currentTable->size;
    }

    table->variableArray = (variable_t *) calloc (MaxVariableNumber, sizeof(variable_t));
    stackPush(stack, table);

    return table;
}

void tableDestructor (table_t *table)
{
    if (table == NULL)
        return;

    free(table->variableArray);
    free(table);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Lord please forgive me for this code

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void makeRet (binary_t *code)
{
    codeAppend(code, "\xc3", 1);

    printf(BOLD YELLOW "RET\n" RESET);

    return;
}

static void makePop (binary_t *code, POP_TYPE type, 
                     int immValue, char registerValue)
{
    #define DEFINE_POP(type, size, binCode, action)                             \
        case POP_##type:                                                        \
        {                                                                       \
            printf(BOLD YELLOW "POP_"#type "\n\tIMM: %d\n\tREG: %s\n" RESET,    \
                   immValue, registerName[registerValue]);                      \
            codeAppend(code, binCode, size);                                    \
            action;                                                             \
            break;                                                              \
        }

    switch (type)
    {
        #include "pops.hpp"

		default: 
            break;
    }
    
    #undef DEFINE_POP

    return;
}

static void makePush (binary_t *code, PUSH_TYPE type, 
                      int immValue, char registerValue)
{
    #define DEFINE_PUSH(type, size, binCode, action)                            \
        case PUSH_##type:                                                       \
        {                                                                       \
            printf(BOLD YELLOW "PUSH_"#type"\n\tIMM: %d\n\tREG: %s\n" RESET,    \
                   immValue, registerName[registerValue]);                      \
            codeAppend(code, binCode, size);                                    \
            action;                                                             \
            break;                                                              \
        }

    switch(type)
    {
        #include "pushes.hpp"

		default: 
            break;
    }
   
    #undef DEFINE_PUSH

    return;
}

static void makeOperation (binary_t *code, OPTIONS type)
{
    #define DEFINE_OPERATION(type, size, binCode)   \
        case type:                                  \
        {                                           \
            printf(BOLD YELLOW #type "\n" RESET);   \
            codeAppend(code, binCode, size);        \
            break;                                  \
        }

    switch (type)
	{
        #include "operations.hpp"

		default: 
            break;
    }

    #undef DEFINE_OPERATION

    return;
}

static void makeJump (binary_t *code, JUMP_TYPE type, int shift)
{
    #define DEFINE_JMP(type, size, binCode)                         \
        case JMP_##type:                                            \
        {                                                           \
            codeAppend(code, binCode, size);                        \
            printf(BOLD YELLOW "JMP_"#type" 0x%x " RESET, shift);   \
            break;                                                  \
        }

    switch(type)
    {
        #include "jumps.hpp"

        default:  
            break;
    }

    #undef DEFINE_JMP
    
    codeCopy(code, code->dataSize, &shift, sizeof(int));

    return;
}

static void remakeJump (binary_t *code, int argument)
{
    int shift = code->dataSize - argument - 4;

    codeCopy(code, argument, &shift, sizeof(int));

    printf(BOLD YELLOW "REMAKE JUMP ARG in 0x%x on 0x%x\n" RESET, argument, shift);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static int getVariablePosition (node_t *node, stack_t *stack)
{
	size_t stackIndex = stack->size - 1;
	table_t *currentTable = stack->data[stackIndex];
	size_t lastRegister = currentTable->registerPosition;

	while (stackIndex >= 0)
	{
		for (size_t varIndex = 0; varIndex < currentTable->size; varIndex++)
			if (strcasecmp(node->data.name, (currentTable->variableArray[varIndex]).name) == 0)
				return (int) (currentTable->variableArray[varIndex]).position + 
                       (int) currentTable->registerPosition - 
                       (int) lastRegister;
		
		if (stackIndex == 0)
			break;
		
		stackIndex--;
		currentTable = stack->data[stackIndex];
	}

	return Poizon;
}

static void countExpression (node_t *node, program_t *program)
{
	if (node->left)  
        countExpression(node->left,  program);
	if (node->right) 
        countExpression(node->right, program);
	
	if (node->type == VALUE)
        makePush(program->code, (PUSH_TYPE) PUSH_IMM, node->data.value, 0);
	
	else if (node->type == VARIABLE)
	{
		int variablePosition = getVariablePosition(node, program->stack);

        makePush(program->code, (PUSH_TYPE) PUSH_IMM_REG_MEM, QWord * variablePosition, RAX_MASK);
	}

	else if (node->data.option == CALL)
	{
		parseCall(node, program);

        makePush(program->code, (PUSH_TYPE) PUSH_REG, 0, RCX_MASK);
	}

	if (node->type == OPTION)
        makeOperation(program->code, node->data.option);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Cringe vol. 2

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void parseRet (node_t *node, program_t *program)
{
	// TODO check

	bool isInLocal = false;
	node_t *currentNode = node;

	while (currentNode->data.option != FUNC)
	{
		if (currentNode->data.option == IF      ||
			currentNode->data.option == ELSE    ||
			currentNode->data.option == WHILE)
			isInLocal = true;
		
		currentNode = currentNode->parent;
	}
    
    if (program->main == false)
	    countExpression(node->left, program);
    else 
        codeAppend(program->code, SET_EXIT, SET_EXIT_SIZE);

	table_t *currentTable = program->stack->data[program->stack->size - 2];

	if (isInLocal == true)
	{
        makePop (program->code, (POP_TYPE)  POP_REG,  0, RCX_MASK);
        makePush(program->code, (PUSH_TYPE) PUSH_REG, 0, RAX_MASK); 
        makePush(program->code, (PUSH_TYPE) PUSH_IMM, QWord * currentTable->size, 0);
        makeOperation(program->code, SUB);
        makePop (program->code, (POP_TYPE)  POP_REG,  0, RAX_MASK);
	}
    
    makeRet(program->code);

	return;
}

static void parseWhile (node_t *node, program_t *program)
{
    // TODO check
	
    size_t whileAddress = program->code->dataSize;
	countExpression(node->left, program);

    makePush(program->code, (PUSH_TYPE) PUSH_IMM, 0, 0);

    size_t whileNoAddress = program->code->dataSize + 7;
    makeJump(program->code, (JUMP_TYPE) JMP_JE, 0);

	parseStatement(node->right, program);
	
	table_t *currentTable = program->stack->data[program->stack->size - 1];
	tableDestructor(currentTable);

    elem_t element = NULL;
	stackPop(program->stack, &element);			
	
	currentTable = program->stack->data[program->stack->size - 1];

    makePush(program->code, (PUSH_TYPE) PUSH_IMM, currentTable->registerPosition, 0);
    makePop (program->code, (POP_TYPE)  POP_REG,  0, RAX_MASK);
 
    makeJump(program->code, (JUMP_TYPE) JMP_JMP, program->code->dataSize - whileAddress);
    remakeJump(program->code, whileNoAddress);
	
	return;
}

static void parseIf (node_t *node, program_t *program)
{
    // TODO check
	
	countExpression(node->left, program);

    makePush(program->code, (PUSH_TYPE) PUSH_IMM, 0, 0);
	
	if (node->right->data.option != ELSE)
	{
        size_t jmpAddress = program->code->dataSize + 7;
        makeJump(program->code, (JUMP_TYPE) JMP_JE, 0);

		parseStatement(node->right, program);

		table_t* currentTable = program->stack->data[program->stack->size - 1];
		tableDestructor(currentTable);

        elem_t element = NULL;
		stackPop(program->stack, &element);			
		
		currentTable = program->stack->data[program->stack->size - 1];

        makePush(program->code, (PUSH_TYPE) PUSH_IMM, currentTable->registerPosition, 0);
        makePop (program->code, (POP_TYPE)  POP_REG,  0, RAX_MASK);

        remakeJump(program->code, jmpAddress);       
	}

	else
	{
        size_t elseAddress = program->code->dataSize + 7;
        makeJump(program->code, (JUMP_TYPE) JMP_JE, 0);
	
		parseStatement(node->right->left, program);

		table_t *currentTable = program->stack->data[program->stack->size - 1];
		tableDestructor(currentTable);

        elem_t element = NULL;
		stackPop(program->stack, &element);			
		
		currentTable = program->stack->data[program->stack->size - 1];

        makePush(program->code, (PUSH_TYPE) PUSH_IMM, currentTable->registerPosition, 0);
        makePop (program->code, (POP_TYPE)  POP_REG, 0, RAX_MASK);
        
        size_t ifAddress = program->code->dataSize + 7; 
        makeJump(program->code, (JUMP_TYPE) JMP_JMP, 0);

        remakeJump(program->code, elseAddress);

		parseStatement(node->right->right, program);
		
		currentTable = program->stack->data[program->stack->size - 1];
		tableDestructor(currentTable);

		stackPop(program->stack, &element);			
		
		currentTable = program->stack->data[program->stack->size - 1];

        makePush(program->code, (PUSH_TYPE) PUSH_IMM, currentTable->registerPosition, 0);
        makePop (program->code, (POP_TYPE)  POP_REG, 0, RAX_MASK);
        
	    remakeJump(program->code, ifAddress - 6);	
	}
	
	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void parseEq (node_t *node, program_t *program)
{
    // TODO checks
	
	int variablePosition = getVariablePosition(node->left, program->stack);
	countExpression(node->right, program);
	
    makePop(program->code, (POP_TYPE) POP_IMM_REG_MEM, QWord * variablePosition, RAX_MASK);
	
	return;
}

static void parseVar (node_t *node, program_t *program)
{
    // TODO checks
	
	variable_t newVariable = {}; 
	table_t *currentTable = program->stack->data[program->stack->size - 1];

	newVariable.name = node->left->data.name;
	newVariable.position = currentTable->size;
		
	currentTable->variableArray[currentTable->size] = newVariable;
	currentTable->size++;
		
	countExpression(node->right, program);
    
    makePop(program->code, (POP_TYPE) POP_IMM_REG_MEM, QWord * newVariable.position, RAX_MASK);

	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void parseScanf (node_t *node, program_t *program)
{
	node_t *currentNode = node;

	while (currentNode != NULL)
	{
		// if (checkVariable(node->left, program->stack) == false)
		// {
		// 	node->left // TODO
		// }
		
		int variablePosition = getVariablePosition(node->left, program->stack);

        printf(BOLD YELLOW "INPUT\n" RESET);

        codeAppend(program->code, PUSH_RDI,       PUSH_RDI_SIZE);
        codeAppend(program->code, LEA_RDI_RSP,    LEA_RDI_RSP_SIZE);
        codeAppend(program->code, PUSH_REGS,      PUSH_REGS_SIZE);
        codeAppend(program->code, MOV_RBP_RSP,    MOV_RBP_RSP_SIZE);
        codeAppend(program->code, AND_RSP_FF,     AND_RSP_FF_SIZE);
        codeAppend(program->code, MOVABS_R10_IMM, MOVABS_R10_IMM_SIZE); 

        uint64_t address = EntryPoint + PageSize + MemorySize;

        codeCopy(program->code, program->code->dataSize, &address, 8);

        codeAppend(program->code, CALL_R10,    CALL_R10_SIZE);
        codeAppend(program->code, MOV_RSP_RBP, MOV_RSP_RBP_SIZE);
        codeAppend(program->code, POP_REGS,    POP_REGS_SIZE);
         
        makePop(program->code, (POP_TYPE) POP_IMM_REG_MEM, variablePosition, RAX_MASK);

		currentNode = currentNode->right;
	}
	
	return;
}

static void parsePrintf (node_t *node, program_t *program) 
{
	node_t *currentNode = node;

	while (currentNode != NULL)
	{
		// if (checkVariable(node->left, program->stack) == false)
		// {
		// 	(node->left); // TODO process it
		// }
  
        printf(BOLD YELLOW "OUTPUT\n" RESET);

		int varPos = getVariablePosition(node->left, program->stack);

        makePush(program->code, (PUSH_TYPE) PUSH_IMM_REG_MEM, QWord * varPos, RAX_MASK);

        codeAppend(program->code, MOV_RBP_RSP,    MOV_RBP_RSP_SIZE);
        codeAppend(program->code, PUSH_REGS,      PUSH_REGS_SIZE);
        codeAppend(program->code, AND_RSP_FF,     AND_RSP_FF_SIZE);
        codeAppend(program->code, MOVABS_R10_IMM, MOVABS_R10_IMM_SIZE);

        uint64_t address = EntryPoint + PageSize + MemorySize + ScanfSize + ScanfBuffer;  
        codeCopy(program->code, program->code->dataSize, &address, 8);

        codeAppend(program->code, CALL_R10,    CALL_R10_SIZE);
        codeAppend(program->code, POP_REGS,    POP_REGS_SIZE);
        codeAppend(program->code, MOV_RSP_RBP, MOV_RSP_RBP_SIZE);
        codeAppend(program->code, POP_RDI,     POP_RDI_SIZE);

		currentNode = currentNode->right;
	}
	
	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static size_t countParams(node_t *node)
{
	node_t *currentNode = node;
	size_t  paramNumber = 0;

	while (currentNode != NULL)
	{
		paramNumber++;

		currentNode = currentNode->right;
	}

	return paramNumber;
}

void parseCall (node_t *node, program_t *program)
{
	bool   isDefFunc = 0;
	size_t funcParam = 0;
	size_t callParam = 0;
	
    size_t functionIndex = 0;
	for (functionIndex = 0; functionIndex < program->functionNumber; functionIndex++)
		if (strcasecmp((program->functionsArray[functionIndex]).name, node->left->data.name) == 0)
		{
			isDefFunc = true; 
			funcParam = (program->functionsArray[functionIndex]).paramNumber;

			break;	
		}
	
	if (isDefFunc == false)
	{
		printf("Unknown function: %s;\n", node->left->data.name);

        abort();
	}

	
	if (node->left->left)
		callParam = countParams(node->left->left);
	
	table_t *currentTable = program->stack->data[program->stack->size - 1];

    makePush(program->code, (PUSH_TYPE) PUSH_REG, 0, RAX_MASK); 
    makePush(program->code, (PUSH_TYPE) PUSH_IMM, QWord * currentTable->size, 0);
    makeOperation(program->code, ADD);
    makePop(program->code,  (POP_TYPE)  POP_REG,  0, RAX_MASK);

	for (int index = (int) callParam - 1; index >= 0; index--)
        makePop(program->code, (POP_TYPE) POP_IMM_REG_MEM, QWord * index, RAX_MASK);
    
    makeJump(program->code, (JUMP_TYPE) JMP_CALL, 
             -5 - program->code->dataSize + 
             program->functionsArray[functionIndex].functionStart);

    makePush(program->code, (PUSH_TYPE) PUSH_REG, 0, RAX_MASK); 
    makePush(program->code, (PUSH_TYPE) PUSH_IMM, QWord * currentTable->size, 0);
    makeOperation(program->code, SUB);
    makePop(program->code,  (POP_TYPE)  POP_REG,  0, RAX_MASK);

	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static size_t parseParams (node_t *node, table_t *table)
{
	node_t *currentNode = node;
	size_t paramNumber = 0;

	while (currentNode != NULL)
	{
		paramNumber++;

		variable_t parameter = {};
        
        parameter.name = currentNode->left->data.name;
        parameter.position = table->size;
        
		table->variableArray[table->size] = parameter;

		table->size++;
		currentNode = currentNode->right;
	}

	return paramNumber;
}

static void parseFunc (node_t *node, program_t *program)
{
	for (size_t index = 0; index < program->functionNumber; index++)
		if (strcasecmp((program->functionsArray[index]).name, node->left->data.name) == 0)
			printf("Function %s is already defined", node->left->data.name);
		
    if (strcasecmp("main", node->left->data.name) == 0) 
    {
        remakeJump(program->code, 41);
        program->main = true;
    }

	(program->functionsArray[program->functionNumber]).name = node->left->data.name;

	if (node->left != NULL        &&
        node->left->right != NULL &&
        strcasecmp(node->left->right->data.name, "TYPE") == 0)
		(program->functionsArray[program->functionNumber]).returnValue = TYPE;

	else
		(program->functionsArray[program->functionNumber]).returnValue = VOID;
	
	table_t *newTable = tableConstructor(program->stack);
	
	if (node->left != NULL && node->left->left != NULL)
		(program->functionsArray[program->functionNumber]).paramNumber = parseParams(node->left->left, newTable);
	
    (program->functionsArray[program->functionNumber]).functionStart = program->code->dataSize;
	program->functionNumber++;

	parseStatement(node->right, program);

	tableDestructor(newTable);

	stackPop(program->stack, NULL);			

    program->main = false;
	
	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void parseStatement (node_t *node, program_t *program)
{
    if (node    == NULL ||
        program == NULL)
        return;

    table_t *oldTable = NULL;

    if (program->stack->size != 0)
        oldTable = program->stack->data[program->stack->size - 1];

    if (program->stack->size != 0 &&
        (node->parent != NULL && node->parent->data.option != FUNC))
    {
        makePush(program->code, (PUSH_TYPE) PUSH_REG, 0, RAX_MASK); 
        makePush(program->code, (PUSH_TYPE) PUSH_IMM, (int) QWord * oldTable->size, 0);
        makeOperation(program->code, ADD);
        makePop(program->code,  (POP_TYPE)  POP_REG,  0, RAX_MASK);
    }

    table_t *newTable = NULL;
	
	if (node->parent == NULL || 
        (node->parent != NULL         && 
         node->parent->type == OPTION && 
         node->parent->data.option != FUNC)
       )
	    newTable = tableConstructor(program->stack);
	
    bool isReturn = true;
	bool firstFunction = false;	
	node_t *currentNode = node;

	while (currentNode != NULL)
	{
		if (currentNode->left->data.option == FUNC && firstFunction == false) 
		{
            codeAppend(program->code, SET_RAM, SET_RAM_SIZE);
            
            uint64_t address = EntryPoint + PageSize; 
            codeCopy(program->code, program->code->dataSize, &(address), QWord);
            
            uint64_t scanfBuffer = EntryPoint + PageSize + MemorySize + ScanfSize; 
            
            codeAppend(program->code, SET_SCANF_BUF, SET_SCANF_BUF_SIZE);
            codeCopy(program->code, program->code->dataSize, &(scanfBuffer), QWord);

            uint64_t printfBuffer = EntryPoint + PageSize    + MemorySize + 
                                    ScanfSize  + ScanfBuffer + PrintfSize; 
            
            codeAppend(program->code, SET_PRINTF_BUF, SET_PRINTF_BUF_SIZE);
            codeCopy(program->code, program->code->dataSize, &(printfBuffer), QWord);

            uint64_t itoaBuffer = printfBuffer + PrintfBuffer;
            codeAppend(program->code, SET_ITOA_BUF, SET_ITOA_BUF_SIZE);
            codeCopy(program->code, program->code->dataSize, &(itoaBuffer), QWord);

            makeJump(program->code, (JUMP_TYPE) JMP_JMP, 0);

			firstFunction = true;
		}

		switch(currentNode->left->data.option)
		{
			case VAR:    
                printf(BOLD RED "PARSING VAR:\n" RESET);
                parseVar(currentNode->left, program); 
                break;

			case EQ:
                printf(BOLD RED "PARSING EQ:\n" RESET);
                parseEq(currentNode->left, program); 
                break;

			case FUNC:	  
                printf(BOLD RED "PARSING FUNC:\n" RESET);
                parseFunc(currentNode->left, program); 
                break;

			case OUT: 
                printf(BOLD RED "PARSING OUT:\n" RESET);
                parsePrintf(currentNode->left->left, program); 
                break;

			case IN:  
                printf(BOLD RED "PARSING IN:\n" RESET);
                parseScanf(currentNode->left->left, program); 
                break;

			case CALL:   
                printf(BOLD RED "PARSING CALL:\n" RESET);
                parseCall(currentNode->left, program); 
                break;
            
			case IF:     
            case ELSE:
                printf(BOLD RED "PARSING IF & ELSE:\n" RESET);
                parseIf(currentNode->left, program); 
                break;

			case WHILE:  
                printf(BOLD RED "PARSING WHILE:\n" RESET);
                parseWhile(currentNode->left, program); 
                break;

			case RET: 
                printf(BOLD RED "PARSING RET:\n" RESET);
                isReturn = true; 
                parseRet(currentNode->left, program); 
                break;

			default: 
                printf(BOLD RED "\n\n\nABOBA\n\n\n" RESET);
                abort();
		}
		
		currentNode = currentNode->right;
	}

	if (isReturn == false && program->stack->size != 0 && 
        (node->parent && node->parent->data.option != FUNC))
	{
        makePush(program->code, (PUSH_TYPE) PUSH_REG, 0, RAX_MASK); 
        makePush(program->code, (PUSH_TYPE) PUSH_IMM, QWord * oldTable->size, 0);
        makeOperation(program->code, SUB);
        makePop(program->code,  (POP_TYPE)  POP_REG,  0, RAX_MASK);
	}

    return;
}

void codeRun (binary_t *code)
{
    if (code == NULL)
        return;

    int result = mprotect(code->data, code->dataSize, PROT_EXEC | PROT_WRITE | PROT_READ);
    
    void (* JIT) (void) = (void (*) (void)) (code->data);

    asm
    (
        "push %rax\n"    
        "push %rsi\n"    
        "push %rdi\n"    
        "push %rbp\n"
    );

    JIT();

    asm
    (
        "pop %rbp\n\t"   
        "pop %rdi\n\t"   
        "pop %rsi\n\t"   
        "pop %rax\n\t"
    );
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
