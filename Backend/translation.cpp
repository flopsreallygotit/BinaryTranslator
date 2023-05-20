#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "translation.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static variable_t *variableConstructor (char *function, char *name, size_t ramIP)
{
    variable_t *variable = (variable_t *) calloc (1, sizeof(variable_t));

    variable->function = function;
    variable->name     = name;
    variable->ramIP    = ramIP;

    return variable;
}

static void variableDestructor (variable_t *variable)
{
    if (variable == NULL)
        return;

    variable->function = NULL;
    variable->name     = NULL;
    variable->ramIP    = 0;

    free(variable);
    variable = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

variable_t **variableArrayConstructor (size_t capacity)
{
    return (variable_t **) calloc (capacity, sizeof(variable_t *));
}

void variableArrayDestructor (variable_t **variableArray, size_t capacity)
{
    if (variableArray == NULL)
        return;

    for (size_t variableIndex = 0; variableIndex < capacity; variableIndex++)
        variableDestructor(variableArray[variableIndex]);

    free(variableArray);
    variableArray = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *getGrammar (LONG_FUNC_ARGUMENTS)
{
    DUMP;

    if (CURRENT_TOKEN->type == NAME)
    {
        if (isFunction)
        {
            function = CURRENT_NAME;
            NEXT_TOKEN;

            node_t *node  = nodeConstructor(NAME, {.name = function}, NULL);

            CHECK_ALL_OK;

            node->left  = getGrammar(LONG_ARGUMENTS, false, function);
            node->right = getGrammar(LONG_ARGUMENTS, false, function);

            return node;
        }

        else
        {
            CURRENT_VARIABLE = NEW_VARIABLE;
            NEXT_VARIABLE;

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
             CURRENT_OPTION == VOID ||
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

        node_t* leftNode  = NULL;
        node_t* rightNode = NULL;
        
        NEXT_TOKEN;

        if (Option == FUNC || Option == CALL)
            leftNode = getGrammar(LONG_ARGUMENTS, true,  function);  

        else
            leftNode = getGrammar(LONG_ARGUMENTS, false, function);        

        rightNode = getGrammar(LONG_ARGUMENTS, false, function);

        node_t *node  = NEW_NODE(Option);

        CHECK_ALL_OK;

        node->left  = leftNode;
        node->right = rightNode;
        
        return node;  
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
