#ifndef REPLACEMENTS_H
#define REPLACEMENTS_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// For functions:

#ifndef FUNC_ARGUMENTS
#define FUNC_ARGUMENTS token_t **tokenArray, size_t *tokenIndex
#endif

#ifndef ARGUMENTS
#define ARGUMENTS tokenArray, tokenIndex
#endif

#ifndef LONG_FUNC_ARGUMENTS
#define LONG_FUNC_ARGUMENTS FUNC_ARGUMENTS,                                     \
                            variable_t **variableArray, size_t *variableIndex,  \
                            size_t capacity, bool isFunction, char *function
#endif

#ifndef LONG_ARGUMENTS
#define LONG_ARGUMENTS ARGUMENTS,                       \
                       variableArray, variableIndex,    \
                       capacity
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Token features:

#ifndef CURRENT_TOKEN
#define CURRENT_TOKEN tokenArray[*tokenIndex]
#endif

#ifndef NEXT_TOKEN
#define NEXT_TOKEN (*tokenIndex)++
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Current values depending on token_t type:

#ifndef CURRENT_OPTION
#define CURRENT_OPTION CURRENT_TOKEN->data.option
#endif

#ifndef CURRENT_VALUE
#define CURRENT_VALUE CURRENT_TOKEN->data.value
#endif

#ifndef CURRENT_NAME
#define CURRENT_NAME CURRENT_TOKEN->data.name
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// New node depending on token_t type:

#ifndef NEW_CURRENT_OPTION_NODE
#define NEW_CURRENT_OPTION_NODE nodeConstructor(OPTION, {.option = CURRENT_OPTION}, NULL);
#endif

#ifndef NEW_CURRENT_VALUE_NODE
#define NEW_CURRENT_VALUE_NODE nodeConstructor(VALUE, {.value = CURRENT_VALUE}, NULL);
#endif

#ifndef NEW_CURRENT_VARIABLE_NODE
#define NEW_CURRENT_VARIABLE_NODE nodeConstructor(VARIABLE, {.name = CURRENT_NAME}, NULL);
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// New option nodes:

#ifndef NEW_NODE
#define NEW_NODE(Option)   nodeConstructor(OPTION, {.option = Option}, NULL);
#endif

#ifndef UNION_NODE
#define UNION_NODE(Option) createUnionNode(OPTION, {.option = Option}, NULL, leftNode, rightNode)
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Return if node is null:

#ifndef CHECK_ALL_OK
#define CHECK_ALL_OK if (node == NULL) return NULL;
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef CURRENT_VARIABLE
#define CURRENT_VARIABLE variableArray[*variableIndex]
#endif

#ifndef NEXT_VARIABLE
#define NEXT_VARIABLE (*variableIndex)++;
#endif

#ifndef NEW_VARIABLE
#define NEW_VARIABLE variableConstructor(function, CURRENT_NAME, *variableIndex + 1);
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef LEFT_OPERATION
#define LEFT_OPERATION  node->left,  prevOption, variableArray, capacity, file
#endif

#ifndef RIGHT_OPERATION
#define RIGHT_OPERATION node->right, prevOption, variableArray, capacity, file
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// DEBUG:
// Dump info about current token_t:

#ifdef DEBUG

#define DUMP                                \
    PUTMESSAGE("INFO");                     \
    printf(BOLD YELLOW);                    \
    printf("TOKEN:  %lu\n", *tokenIndex);   \
    tokenDump(CURRENT_TOKEN);               \
    printf("\n" RESET);

#else

#define DUMP

#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif