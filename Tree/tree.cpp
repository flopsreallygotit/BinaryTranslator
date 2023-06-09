#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "tree.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node_t *nodeConstructor (TYPES type, data_t data, node_t *parent)
{                                          
    node_t *node = (node_t *) calloc (1, sizeof(node_t));

    CHECKERROR(node != NULL && 
               "Can't allocate memory for node_t.", 
               NULL); 

    node->data  = data;
    node->type  = type;

    node->parent = parent;

    node->left  = NULL;                                               
    node->right = NULL;

    return node;
}

node_t *createUnionNode (TYPES  type, data_t data, node_t *parent, 
                       node_t *left, node_t *right)
{
    node_t *node = nodeConstructor(type, data, parent);

    node->left  = left;
    node->right = right;

    return node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void treeConstructorFunction (tree_t *tree, const char *filename, const int line)
{
    tree->size = 0;

    // Reader will make root itself.

    #ifdef BIRTHINFO

    tree->birthFile = filename;
    tree->birthLine = line;

    #endif

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void nodeDestructor (node_t *node)
{
    if (node == NULL)
        return;

    if (node->left  != NULL)
        nodeDestructor(node->left);
    
    if (node->right != NULL)
        nodeDestructor(node->right);

    node->left  = NULL;
    node->right = NULL;

    free(node);
    node = NULL;

    return;
}

ISERROR treeDestructor (tree_t *tree)
{
    CHECKERROR(tree != NULL && 
               "You are trying to destroy nullpointer.", 
               NULLPOINTER);

    tree->size = 0;

    nodeDestructor(tree->root);

    *tree = 
    {
        .root = NULL, 
        .size = 0
    };

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void getSubtreeParents (node_t *node, node_t *parentNode)
{
    if (node == NULL)
        return;

    node->parent = parentNode;

    if (node->left  != NULL)
        getSubtreeParents(node->left,  node);

    if (node->right != NULL)
        getSubtreeParents(node->right, node);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void copySubtree (node_t **destNode, node_t *srcNode, node_t *parNode)
{
    *destNode = nodeConstructor(srcNode->type, srcNode->data, parNode);

    if (srcNode->left != NULL)
        copySubtree(&((*destNode)->left),  srcNode->left,  srcNode->left);

    if (srcNode->right != NULL)
        copySubtree(&((*destNode)->right), srcNode->right, srcNode->right);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void countSubtreeSize (node_t *node, size_t *Size)
{
    if (node->left)
        countSubtreeSize(node->left,  Size);

    if (node->right)
        countSubtreeSize(node->right, Size);

    (*Size)++;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define PUT_QUOTE if (putQuotes) putc('"', file)
#define OUTPUT PUT_QUOTE; fprintf(file, "%s", node->data.name); PUT_QUOTE

static void dataOutput (const node_t *node, 
                        bool putQuotes, FILE *file)
{
    #define DEFINE_OPTION(Option, ...)    \
        if (node->data.option == Option)  \
            fprintf(file, "%s", #Option);
        
    switch (node->type)
    {
        case VALUE:
            fprintf(file, "%lg", node->data.value);
            break;

        case NAME:
            OUTPUT;
            break;

        case SHORT_NAME:
            OUTPUT;
            break;

        case VARIABLE:
            OUTPUT;
            break;

        case OPTION:
            #include "options.hpp"
            break;
        
        default:
            PUTERROR("Unknown type of node.");
            return;
    }

    #undef DEFINE_OPTION
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursivePreorderPrintTree (const node_t *node, FILE *file)
{
    putc('{', file);

    dataOutput(node, true, file);

    if (node->type == NAME)
    {
        if (node->left != NULL)
            recursivePreorderPrintTree(node->left, file);
        else
            fprintf(file, "{NIL}");

        fprintf(file, "{VOID}");
    }

    if (node->type == SHORT_NAME)
        fprintf(file, "{NIL}{NIL}");

    if (node->type == OPTION)
    {
        if (node->left  != NULL)
            recursivePreorderPrintTree(node->left,  file);
        else
            fprintf(file, "{NIL}");

        if (node->right != NULL)
            recursivePreorderPrintTree(node->right, file);
        else
            fprintf(file, "{NIL}");
    }

    putc('}', file);

    return;
}

ISERROR preorderPrintTree (const tree_t *tree, FILE *file)
{
    CHECKERROR(tree != NULL &&
               "Can't print nullpointer.",
               NULLPOINTER);

    recursivePreorderPrintTree(tree->root, file);

    putc('\n', file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void outputTree (const tree_t *tree, const char *filename)
{
    char *newFilename = stralloccat(filename, ".tree");

    FILE *file = fopen(newFilename, "w");

    free(newFilename);

    preorderPrintTree(tree, file);

    fclose(file);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define PRINTNODE(node, file)                                       \
    do                                                              \
    {                                                               \
        fprintf(file, "    node   pointer: " "%p\n", node);         \
        fprintf(file, "    Parent pointer: " "%p\n", node->parent); \
        fprintf(file, "    Left   pointer: " "%p\n", node->left);   \
        fprintf(file, "    Right  pointer: " "%p\n", node->right);  \
        fprintf(file, "    Element: ");                             \
        dataOutput(node, true, file);                               \
        fprintf(file, "\n\n");                                      \
    }                                                               \
    while (0)

static void recursivePrintTree (const node_t *node, FILE *file)
{
    PRINTNODE(node, file);

    if (node->left  != NULL)
        recursivePrintTree(node->left,  file);

    if (node->right != NULL)
        recursivePrintTree(node->right, file);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR simpleTreeDumpFunction (const tree_t *tree, const char *treename, const char *filename, 
                                       const int   line, const char *function, FILE *file)
{
    CHECKERROR(tree     != NULL && 
               "Can't dump nullpointer.", 
               NULLPOINTER);

    CHECKERROR(treename != NULL &&
               "Can't dump with nullpointer to treename.",
               NULLPOINTER);

    CHECKERROR(filename != NULL &&
               "Can't dump with nullpointer to filename.",
               NULLPOINTER);

    CHECKERROR(function != NULL &&
               "Can't dump with nullpointer to function.", 
               NULLPOINTER);

    CHECKERROR(file     != NULL &&
               "Can't dump with nullpointer to file.", 
               NULLPOINTER);

    fprintf(file, HTMLBLUE "\nTree dump at %s:%d in %s\n" HTMLRESET 
            HTMLBOLD "tree %s[%p] " HTMLBOLDRESET, 
            filename, line, function, treename, tree);

    fprintf(file, "\n{\n    Size: %lu\n", tree->size);

    putc('\n', file);

    if (tree->root)
        recursivePrintTree(tree->root, file);

    else
        fprintf(file, "    tree hasn't root.\n");

    fprintf(file, "}\n\n");

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void outputNode (node_t *node, FILE *tmp)
{
    const char *color = NULL;

    if (node->type == OPTION)
        color = globalOptionFillColor;
    
    else if (node->type == VALUE)
        color = globalValueFillColor;

    else
        color = globalNameFillColor;

    fprintf(tmp, "    node_t%p [shape=box3d, style=\"filled\" fillcolor=\"%s\"," 
            "fontname=\"Arial\", label = ",                                    
            node, color);
                           
    dataOutput(node, true, tmp);

    fprintf(tmp, "]\n"); 

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Only address of current node_t or it's parent allow us to distinguish them.
// So I use address for this.

#define DEFINENODE(Parent, node, tmp)                             \
    do                                                            \
    {                                                             \
        if (node != NULL)                                         \
        {                                                         \
            outputNode(node, tmp);                                \
            fprintf(tmp, "    node_t%p -> node_t%p\n", Parent, node); \
        }                                                         \
    }                                                             \
    while (0)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursiveFileFiller (const node_t *node, FILE *tmp)
{
    DEFINENODE(node, node->left,  tmp);
    DEFINENODE(node, node->right, tmp);

    if (node->left  != NULL)
        recursiveFileFiller(node->left,  tmp);

    if (node->right != NULL)
        recursiveFileFiller(node->right, tmp);
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void fillTemporaryFile (const tree_t *tree, FILE *tmp)
{

    fprintf(tmp, "digraph BST\n{\n");

    outputNode(tree->root, tmp);
    
    recursiveFileFiller(tree->root, tmp);

    putc('}', tmp);  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Html output need sources of images.
// => We need to save all dump images.
// => We need a number of current image:

/// @brief Number of tree_t dump.
static int DumpNumber = 0;

ISERROR treeDumpFunction (const tree_t *tree,     const char *message,
                          const char *treename, const char *filename, 
                          const int   line,     const char *function, 
                          FILE *file)
{
    CHECKERROR(tree     != NULL && 
               "Can't dump nullpointer.", 
               NULLPOINTER);

    CHECKERROR(message  != NULL && 
               "Can't dump with nullpointer to message.", 
               NULLPOINTER);

    CHECKERROR(treename != NULL &&
               "Can't dump with nullpointer to treename.",
               NULLPOINTER);

    CHECKERROR(filename != NULL &&
               "Can't dump with nullpointer to filename.",
               NULLPOINTER);

    CHECKERROR(function != NULL &&
               "Can't dump with nullpointer to function.", 
               NULLPOINTER);

    CHECKERROR(file     != NULL &&
               "Can't dump with nullpointer to file.", 
               NULLPOINTER);

    fprintf(file, 
            "\n<hr>\n<pre>\n"
            "<h2>%s</h2>\n"
            HTMLVERDANA, 
            message);

    ISERROR treeError = simpleTreeDumpFunction(tree, treename, filename,
                                               line, function, file);
    fprintf(file, "</p>\n");

    systemf("mkdir -p ./TreeDumpSources/");

    FILE *tmp = fopen("./TreeDumpSources/tmp.txt", "w");

    CHECKERROR(tmp != NULL &&
               "Can't open temporary file for graph dump.",
               ALLOCATIONERROR);

    fillTemporaryFile(tree, tmp);
    fclose(tmp);

    systemf("dot ./TreeDumpSources/tmp.txt -T png -o ./TreeDumpSources/graph%d.png", DumpNumber);

    fprintf(file, "<img src = ./TreeDumpSources/graph%d.png>\n", DumpNumber);
    DumpNumber++;

    systemf("rm -rf ./TreeDumpSources/tmp.txt");

    return treeError;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
