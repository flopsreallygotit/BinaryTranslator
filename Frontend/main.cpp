#include <assert.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "tokenizer.hpp"
#include "universalUtils.hpp"
#include "recursiveDescent.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (const int argc, const char *argv[])
{
    const char *filename = checkFirstArgvAndGetIt(argc, argv);

    assert(filename != NULL);

    token_t **tokenArray = tokenArrayConstructor(filename);

    assert(tokenArray != NULL);

    #ifdef DEBUG
    tokenArrayDump(tokenArray);
    #endif

    tree_t tree = {};
    treeConstructor(&tree);

    size_t token_idx = 0;
    tree.root = getGrammar(tokenArray, &token_idx);

    assert(tree.root != NULL);

    #ifdef DEBUG

    FILE *output = fopen("FrontEndOutput.htm", "w");
    treeDump(&tree, "Tree after parse.", output);
    fclose(output);

    #endif

    outputTree(&tree, filename);

    tokenArrayDestructor(tokenArray);
    treeDestructor(&tree);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
