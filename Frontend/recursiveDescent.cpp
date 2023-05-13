#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "universalUtils.hpp"
#include "recursiveDescent.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Parsing tree from input file:

tree *parseTokens (token **tokenArray)
{
    tree *Tree = treeConstructor;

    CHECKERROR(Tree != NULL &&
               "Can't create tree.",
               NULL);

    size_t token_idx = 0;

    Tree->root = getGrammar(tokenArray, &token_idx);

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Recursive descent:

// Better check replacements.h before reading code after this.

// Parents of all nodes are null because we don't need it in reader.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getGrammar (FUNC_ARGUMENTS)
{
    node *Node = getStatement(ARGUMENTS);

    if (CURRENT_OPTION != TERMINATION_SYM)
    {
        nodeDestructor(Node);
        
        return NULL;
    }

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getStatement (FUNC_ARGUMENTS)
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

    node *Node = NEW_NODE(ST);

    CHECK_ALL_OK;

    Node->left  = getFunction(ARGUMENTS);
    Node->right = getStatement(ARGUMENTS);

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getFunction (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == FUNC)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        Node->left  = getHeader(ARGUMENTS);
        Node->right = getStatement(ARGUMENTS);
    
        return Node;
    }

    else
        return getCondition(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getHeader (FUNC_ARGUMENTS)
{
    DUMP;

    node *Node = getNumberOrName(ARGUMENTS);

    CHECK_ALL_OK;

    Node->type = NAME;
    Node->left = getParameter(ARGUMENTS);

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getParameter (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == OPEN_BR) 
        NEXT_TOKEN;

    if (CURRENT_OPTION == CLOSE_BR)
    {
        NEXT_TOKEN;

        return NULL;
    }

    node *Node = NEW_NODE(PARAM);

    CHECK_ALL_OK;

    Node->left  = getVariable(ARGUMENTS);
    Node->right = getParameter(ARGUMENTS);

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getCondition (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == IF)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        Node->left = getExpression(ARGUMENTS);

        Node->right = NEW_NODE(ELSE);

        CHECK_ALL_OK;

        Node->right->left = getStatement(ARGUMENTS);

        if (CURRENT_OPTION != ELSE)
            return NULL;

        NEXT_TOKEN;

        Node->right->right = getStatement(ARGUMENTS);
        
        return Node;
    }

    else
        return getCycle(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getCycle (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == WHILE)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        Node->left  = getExpression(ARGUMENTS);
        Node->right = getStatement(ARGUMENTS);
        
        return Node;
    }

    else
        return getReturn(ARGUMENTS);
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getReturn (FUNC_ARGUMENTS)
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

node *getCall (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == CALL)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        Node->left = getNumberOrName(ARGUMENTS);
        Node->left->type = SHORT_NAME;

        return Node;
    }

    else 
        return getInOutput(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getInOutput (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == IN ||
        CURRENT_OPTION == OUT)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        Node->left = getParameter(ARGUMENTS);

        return Node;
    }

    else
        return getVariable(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getVariable (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == VAR)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        Node->left  = getVariable(ARGUMENTS);
        Node->right = getExpression(ARGUMENTS);

        return Node;
    }

    else
        return getUnary(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getExpression (FUNC_ARGUMENTS)
{
    DUMP;

    node *leftNode = getAddSub(ARGUMENTS);

    if (CURRENT_OPTION == IS_EE ||
        CURRENT_OPTION == IS_GE ||
        CURRENT_OPTION == IS_BE ||
        CURRENT_OPTION == IS_GT ||
        CURRENT_OPTION == IS_BT ||
        CURRENT_OPTION == IS_NE)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;
        
        NEXT_TOKEN;

        Node->left = leftNode;

        Node->right = getAddSub(ARGUMENTS);
    
        return Node;
    }

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getAddSub (FUNC_ARGUMENTS)
{
    DUMP;

    node *leftNode = getMulDiv(ARGUMENTS);

    OPTIONS Option = CURRENT_OPTION;

    while (Option == ADD || 
           Option == SUB)
    {
        NEXT_TOKEN;

        node *rightNode = getMulDiv(ARGUMENTS);

        leftNode = UNION_NODE(Option);
    }   

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getMulDiv (FUNC_ARGUMENTS)
{
    DUMP;

    node *leftNode = getPower(ARGUMENTS);

    OPTIONS Option = CURRENT_OPTION;

    while (Option == MUL || 
           Option == DIV)
    {
        NEXT_TOKEN;

        node *rightNode = getPower(ARGUMENTS);

        leftNode = UNION_NODE(Option);
    }   

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getPower (FUNC_ARGUMENTS)
{
    DUMP;

    node *leftNode = getBrackets(ARGUMENTS);

    OPTIONS Option = CURRENT_OPTION;

    while (Option == POW)
    {
        NEXT_TOKEN; 

        node *rightNode = getBrackets(ARGUMENTS);

        leftNode = UNION_NODE(Option);
    }   

    return leftNode;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getBrackets (FUNC_ARGUMENTS)
{ 
    DUMP;

    if (CURRENT_OPTION == OPEN_BR)
    {
        NEXT_TOKEN;

        node* Node = getExpression(ARGUMENTS);

        CHECK_ALL_OK;

        if (CURRENT_OPTION == CLOSE_BR)
        {
            NEXT_TOKEN;

            return Node;
        }

        else 
            return NULL;
    }

    else
        return getUnary(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getUnary (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_OPTION == SIN || // Ne pravoslavno TODO it Vinokur don't like it :-(((
        CURRENT_OPTION == COS ||
        CURRENT_OPTION == SQRT)
    {
        node *Node = NEW_CURRENT_OPTION_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        Node->left = getBrackets(ARGUMENTS);

        return Node;
    }

    else 
        return getNumberOrName(ARGUMENTS);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *getNumberOrName (FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_TOKEN->type == VALUE)
    {
        node *Node = NEW_CURRENT_VALUE_NODE

        CHECK_ALL_OK;

        NEXT_TOKEN;

        return Node;
    }

    else if (CURRENT_TOKEN->type == NAME)
    {
        node *Node = NEW_CURRENT_VARIABLE_NODE;

        CHECK_ALL_OK;

        NEXT_TOKEN;

        return Node;
    }

    else 
        return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
