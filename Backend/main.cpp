#include "translation.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (const int argc, const char *argv[])
{
    const char *filename = checkFirstArgvAndGetIt(argc, argv);

    CHECKERROR(filename != NULL &&
               "Filename is NULL", 
               -1);

    token_t **tokenArray = tokenArrayConstructor(filename);

    CHECKERROR(tokenArray != NULL &&
               "Token array is NULL",
               -1);

    #ifdef DEBUG
    tokenArrayDump(tokenArray);
    #endif

    variable_t **variableArray = variableArrayConstructor(MaxVariableNumber);

    size_t tokenIndex    = 0;
    size_t variableIndex = 0;

    tree_t tree = {};
    treeConstructor(&tree);

    tree.root = getGrammar(tokenArray,    &tokenIndex, 
                           variableArray, &variableIndex, 
                           MaxVariableNumber, false, NULL);

    #ifdef DEBUG
    FILE *output = fopen("BackEndOutput.htm", "w");
    treeDump(&tree, "Tree after parse.", output);
    fclose(output);
    #endif

    variableArrayDestructor(variableArray, MaxVariableNumber);
    tokenArrayDestructor(tokenArray);
    treeDestructor(&tree);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
