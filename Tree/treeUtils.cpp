#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *nodeConstructor (TYPES type, data_t data, node *parent)
{                                          
    node *Node = (node *) calloc (1, sizeof(node));

    CHECKERROR(Node != NULL && 
               "Can't allocate memory for node.", 
               NULL); 

    Node->data  = data;
    Node->type  = type;

    Node->parent = parent;

    Node->left  = NULL;                                               
    Node->right = NULL;

    return Node;
}

node *createUnionNode (TYPES  type, data_t data, node *parent, 
                       node *left, node *right)
{
    node *Node = nodeConstructor(type, data, parent);

    Node->left  = left;
    Node->right = right;

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *treeConstructorFunction (const char *filename, const int line)
{
    tree *Tree = (tree *) calloc (1, sizeof(tree));

    CHECKERROR(Tree != NULL && 
               "Can't allocate memory for tree.",
               NULL);

    Tree->size = 0;

    // Reader will made root itself.

    #ifdef BIRTHINFO

    Tree->birthFile = filename;
    Tree->birthLine = line;

    #endif

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void nodeDestructor (node *Node)
{
    if (Node == NULL)
        return;

    if (Node->left  != NULL)
        nodeDestructor(Node->left);
    
    if (Node->right != NULL)
        nodeDestructor(Node->right);

    Node->left  = NULL;
    Node->right = NULL;

    free(Node);
    Node = NULL;

    return;
}

ISERROR treeDestructor (tree *Tree)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to destroy nullpointer.", 
               NULLPOINTER);

    Tree->size = 0;

    nodeDestructor(Tree->root);

    free(Tree);
    Tree = NULL;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void copySubtree (node **destNode, node *srcNode, node *parNode)
{
    *destNode = nodeConstructor(srcNode->type, srcNode->data, parNode);

    if (srcNode->left != NULL)
        copySubtree(&((*destNode)->left),  srcNode->left,  srcNode->left);

    if (srcNode->right != NULL)
        copySubtree(&((*destNode)->right), srcNode->right, srcNode->right);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void countSubtreeSize (node *Node, size_t *Size)
{
    if (Node->left)
        countSubtreeSize(Node->left,  Size);

    if (Node->right)
        countSubtreeSize(Node->right, Size);

    (*Size)++;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define PUT_QUOTE if (putQuotes) putc('"', file)
#define OUTPUT PUT_QUOTE; fprintf(file, "%s", Node->data.name); PUT_QUOTE

static void dataOutput (const node *Node, 
                        bool putQuotes, FILE *file)
{
    #define DEFINE_OPTION(Option, ...)    \
        if (Node->data.option == Option)  \
            fprintf(file, "%s", #Option);
        
    switch (Node->type)
    {
        case VALUE:
            fprintf(file, "%lg", Node->data.value);
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
            #include "options.h"
            break;
        
        default:
            PUTERROR("Unknown type of Node.");
            return;
    }

    #undef DEFINE_OPTION
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursivePreorderPrintTree (const node *Node, FILE *file)
{
    putc('{', file);

    dataOutput(Node, true, file);

    if (Node->type == NAME)
        fprintf(file, "{NIL}{VOID}");

    if (Node->type == SHORT_NAME)
        fprintf(file, "{NIL}{NIL}");

    if (Node->type == OPTION)
    {
        if (Node->left  != NULL)
            recursivePreorderPrintTree(Node->left,  file);
        else
            fprintf(file, "{NIL}");

        if (Node->right != NULL)
            recursivePreorderPrintTree(Node->right, file);
        else
            fprintf(file, "{NIL}");
    }

    putc('}', file);

    return;
}

ISERROR preorderPrintTree (const tree *Tree, FILE *file)
{
    CHECKERROR(Tree != NULL &&
               "Can't print nullpointer.",
               NULLPOINTER);

    recursivePreorderPrintTree(Tree->root, file);

    putc('\n', file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void outputTree (const tree *Tree, const char *filename)
{
    char *newFilename = stralloccat(filename, ".tree");

    FILE *file = fopen(newFilename, "w");

    free(newFilename);
    newFilename = NULL;

    preorderPrintTree(Tree, file);

    fclose(file);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define PRINTNODE(Node, file)                                       \
    do                                                              \
    {                                                               \
        fprintf(file, "    Node   pointer: " "%p\n", Node);         \
        fprintf(file, "    Parent pointer: " "%p\n", Node->parent); \
        fprintf(file, "    Left   pointer: " "%p\n", Node->left);   \
        fprintf(file, "    Right  pointer: " "%p\n", Node->right);  \
        fprintf(file, "    Element: ");                             \
        dataOutput(Node, true, file);                               \
        fprintf(file, "\n\n");                                      \
    }                                                               \
    while (0)

static void recursivePrintTree (const node *Node, FILE *file)
{
    PRINTNODE(Node, file);

    if (Node->left  != NULL)
        recursivePrintTree(Node->left,  file);

    if (Node->right != NULL)
        recursivePrintTree(Node->right, file);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR simpleTreeDumpFunction (const tree *Tree, const char *treename, const char *filename, 
                                       const int   line, const char *function, FILE *file)
{
    CHECKERROR(Tree     != NULL && 
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
            HTMLBOLD "Tree %s[%p] " HTMLBOLDRESET, 
            filename, line, function, treename, Tree);

    fprintf(file, "\n{\n    Size: %lu\n", Tree->size);

    putc('\n', file);

    if (Tree->root)
        recursivePrintTree(Tree->root, file);

    else
        fprintf(file, "    Tree hasn't root.\n");

    fprintf(file, "}\n\n");

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void outputNode (node *Node, FILE *tmp)
{
    const char *color = NULL;

    if (Node->type == OPTION)
        color = globalOptionFillColor;
    
    else if (Node->type == VALUE)
        color = globalValueFillColor;

    else
        color = globalNameFillColor;

    fprintf(tmp, "    node%p [shape=box3d, style=\"filled\" fillcolor=\"%s\"," 
            "fontname=\"Arial\", label = ",                                    
            Node, color);
                           
    dataOutput(Node, true, tmp);

    fprintf(tmp, "]\n"); 

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Only address of current node or it's parent allow us to distinguish them.
// So I use address for this.

#define DEFINENODE(Parent, Node, tmp)                             \
    do                                                            \
    {                                                             \
        if (Node != NULL)                                         \
        {                                                         \
            outputNode(Node, tmp);                                \
            fprintf(tmp, "    node%p -> node%p\n", Parent, Node); \
        }                                                         \
    }                                                             \
    while (0)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursiveFileFiller (const node *Node, FILE *tmp)
{
    DEFINENODE(Node, Node->left,  tmp);
    DEFINENODE(Node, Node->right, tmp);

    if (Node->left  != NULL)
        recursiveFileFiller(Node->left,  tmp);

    if (Node->right != NULL)
        recursiveFileFiller(Node->right, tmp);
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void fillTemporaryFile (const tree *Tree, FILE *tmp)
{

    fprintf(tmp, "digraph BST\n{\n");

    outputNode(Tree->root, tmp);
    
    recursiveFileFiller(Tree->root, tmp);

    putc('}', tmp);  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Html output need sources of images.
// => We need to save all dump images.
// => We need a number of current image:

/// @brief Number of tree dump.
static int DumpNumber = 0;

ISERROR treeDumpFunction (const tree *Tree,     const char *message,
                          const char *treename, const char *filename, 
                          const int   line,     const char *function, 
                          FILE *file)
{
    CHECKERROR(Tree     != NULL && 
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

    ISERROR treeError = simpleTreeDumpFunction(Tree, treename, filename,
                                               line, function, file);
    fprintf(file, "</p>\n");

    systemf("mkdir -p ./TreeDumpSources/");

    FILE *tmp = fopen("./TreeDumpSources/tmp.txt", "w");

    CHECKERROR(tmp != NULL &&
               "Can't open temporary file for graph dump.",
               ALLOCATIONERROR);

    fillTemporaryFile(Tree, tmp);
    fclose(tmp);

    systemf("dot ./TreeDumpSources/tmp.txt -T png -o ./TreeDumpSources/graph%d.png", DumpNumber);

    fprintf(file, "<img src = ./TreeDumpSources/graph%d.png>\n", DumpNumber);
    DumpNumber++;

    systemf("rm -rf ./TreeDumpSources/tmp.txt");

    return treeError;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
