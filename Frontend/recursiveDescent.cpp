#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "tokenizer.hpp"
#include "universalUtils.hpp"
#include "recursiveDescent.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Recursive descent:

// Better check replacements.h before reading code after this.

// Parents of all nodes are null because we don't need it in reader.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getGrammar (FUNC_ARGUMENTS)
{
    node_t *node = getStatement(ARGUMENTS);

    if (CURRENT_OPTION != TERMINATION_SYM)
    {
        nodeDestructor(node);
        
        return NULL;
    }

    return node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getStatement (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == TERMINATION_SYM)
        return NULL;

    if (CURRENT_OPTION == SEMI_COL)
    {
        NEXT_TOKEN; 

        return getStatement(ARGUMENTS);
    }

    if (CURRENT_OPTION == CLOSE_BR)
    {
        NEXT_TOKEN;

        return NULL;
    }

    node_t *node = NEW_NODE(ST);

    CHECK_ALL_OK;

    node->left  = getFunction(ARGUMENTS);
    node->right = getStatement(ARGUMENTS);

    return node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getFunction (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == FUNC)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        node->left  = getHeader(ARGUMENTS);
        node->right = getStatement(ARGUMENTS);
    
        return node;
    }

    else
        return getCondition(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getHeader (FUNC_ARGUMENTS)
{
    DUMP;

    node_t *node = getNumberOrName(ARGUMENTS);

    CHECK_ALL_OK;

    node->type = NAME;
    node->left = getParameter(ARGUMENTS);

    return node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getParameter (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == OPEN_BR) 
        NEXT_TOKEN;

    if (CURRENT_OPTION == CLOSE_BR)
    {
        NEXT_TOKEN;

        return NULL;
    }

    node_t *node = NEW_NODE(PARAM);

    CHECK_ALL_OK;

    node->left  = getVariable(ARGUMENTS);
    node->right = getParameter(ARGUMENTS);

    return node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getCondition (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == IF)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        node->left = getExpression(ARGUMENTS);

        node->right = NEW_NODE(ELSE);

        CHECK_ALL_OK;

        node->right->left = getStatement(ARGUMENTS);

        if (CURRENT_OPTION != ELSE)
            return NULL;

        NEXT_TOKEN;

        node->right->right = getStatement(ARGUMENTS);
        
        return node;
    }

    else
        return getCycle(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getCycle (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == WHILE)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        node->left  = getExpression(ARGUMENTS);
        node->right = getStatement(ARGUMENTS);
        
        return node;
    }

    else
        return getReturn(ARGUMENTS);
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getReturn (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == RET)
    {
        NEXT_TOKEN;

        return NEW_NODE(RET);
    }

    else 
        return getCall(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getCall (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == CALL)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        node->left = getNumberOrName(ARGUMENTS);
        node->left->type = SHORT_NAME;

        return node;
    }

    else 
        return getInOutput(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getInOutput (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == IN ||
        CURRENT_OPTION == OUT)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        node->left = getParameter(ARGUMENTS);

        return node;
    }

    else
        return getVariable(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getVariable (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == VAR)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        node->left  = getVariable(ARGUMENTS);
        node->right = getExpression(ARGUMENTS);

        return node;
    }

    else
        return getUnary(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getExpression (FUNC_ARGUMENTS)
{
    DUMP;

    node_t *leftNode = getAddSub(ARGUMENTS);

    if (CURRENT_OPTION == IS_EE ||
        CURRENT_OPTION == IS_GE ||
        CURRENT_OPTION == IS_BE ||
        CURRENT_OPTION == IS_GT ||
        CURRENT_OPTION == IS_BT ||
        CURRENT_OPTION == IS_NE)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;
        
        NEXT_TOKEN;

        node->left = leftNode;

        node->right = getAddSub(ARGUMENTS);
    
        return node;
    }

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getAddSub (FUNC_ARGUMENTS)
{
    DUMP;

    node_t *leftNode = getMulDiv(ARGUMENTS);

    OPTIONS Option = CURRENT_OPTION;

    while (Option == ADD || 
           Option == SUB)
    {
        NEXT_TOKEN;

        node_t *rightNode = getMulDiv(ARGUMENTS);

        leftNode = UNION_NODE(Option);
    }   

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getMulDiv (FUNC_ARGUMENTS)
{
    DUMP;

    node_t *leftNode = getPower(ARGUMENTS);

    OPTIONS Option = CURRENT_OPTION;

    while (Option == MUL || 
           Option == DIV)
    {
        NEXT_TOKEN;

        node_t *rightNode = getPower(ARGUMENTS);

        leftNode = UNION_NODE(Option);
    }   

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getPower (FUNC_ARGUMENTS)
{
    DUMP;

    node_t *leftNode = getBrackets(ARGUMENTS);

    OPTIONS Option = CURRENT_OPTION;

    while (Option == POW)
    {
        NEXT_TOKEN; 

        node_t *rightNode = getBrackets(ARGUMENTS);

        leftNode = UNION_NODE(Option);
    }   

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getBrackets (FUNC_ARGUMENTS)
{ 
    DUMP;

    if (CURRENT_OPTION == OPEN_BR)
    {
        NEXT_TOKEN;

        node_t* node = getExpression(ARGUMENTS);

        CHECK_ALL_OK;

        if (CURRENT_OPTION == CLOSE_BR)
        {
            NEXT_TOKEN;

            return node;
        }

        else 
            return NULL;
    }

    else
        return getUnary(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getUnary (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == SIN || // Ne pravoslavno TODO it Vinokur don't like it :-(((
        CURRENT_OPTION == COS ||
        CURRENT_OPTION == SQRT)
    {
        node_t *node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        node->left = getBrackets(ARGUMENTS);

        return node;
    }

    else 
        return getNumberOrName(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getNumberOrName (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_TOKEN->type == VALUE)
    {
        node_t *node = NEW_CURRENT_VALUE_NODE

        CHECK_ALL_OK;

        NEXT_TOKEN;

        return node;
    }

    else if (CURRENT_TOKEN->type == NAME)
    {
        node_t *node = NEW_CURRENT_VARIABLE_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        return node;
    }

    else 
        return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
