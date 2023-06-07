#include "translation.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (const int argc, const char *argv[])
{
    // Filename

    const char *filename = checkFirstArgvAndGetIt(argc, argv);

    CHECKERROR(filename != NULL &&
               "Filename is NULL", 
               -1);

    // Tokenizing input

    token_t **tokenArray = tokenArrayConstructor(filename);

    CHECKERROR(tokenArray != NULL &&
               "Token array is NULL",
               -1);

    #ifdef DEBUG
    tokenArrayDump(tokenArray);
    #endif

    // Making tree from tokens

    tree_t tree = {};
    treeConstructor(&tree);
    size_t tokenIndex = 0;

    tree.root = getGrammar(tokenArray, &tokenIndex, false);
    getSubtreeParents(tree.root, NULL);

    #ifdef DEBUG

    FILE *output = fopen("BackEndOutput.htm", "w");
    treeDump(&tree, "Tree after parse.", output);
    fclose(output);
    
    #endif

    // Making binary code & variables tables & function array out of tree

    program_t program = {};
    programConstructor(&program, tree.root);
    parseStatement(tree.root, &program);
    treeDestructor(&tree);

    // Deallocating memory

    tokenArrayDestructor(tokenArray);
    programDestructor(&program);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
