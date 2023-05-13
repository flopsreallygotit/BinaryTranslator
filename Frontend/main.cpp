#include "recursiveDescent.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (const int argc, const char *argv[])
{
    const char *filename = checkFirstArgvAndGetIt(argc, argv);

    CHECKERROR(filename != NULL &&
               "Filename is NULL", 
               -1);

    token **tokenArray = tokenArrayConstructor(filename);

    CHECKERROR(tokenArray != NULL &&
               "Token array is NULL",
               -1);

    #ifdef DEBUG
    tokenArrayDump(tokenArray);
    #endif

    tree *Tree = parseTokens(tokenArray);

    CHECKERROR(Tree != NULL &&
               "Tree is NULL.",
               ERROR);

    #ifdef DEBUG

    FILE *output = fopen("FrontEndOutput.htm", "w");
    treeDump(Tree, "Tree after parse.", output);
    fclose(output);

    #endif

    outputTree(Tree, filename);

    tokenArrayDestructor(tokenArray);
    treeDestructor(Tree);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
