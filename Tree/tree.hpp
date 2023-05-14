#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeConfig.hpp"
#include "universalUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Constructs node_t
/// @param type Type of data
/// @param data Content of node_t
/// @param parent Parent of node_t

node_t *nodeConstructor (TYPES type, data_t data, node_t *parent);

/// @brief Create node_t that becomes root of left subtree and right subtree
/// @param type Type of data
/// @param data Content of node_t
/// @param parent Parent of node_t
/// @param left Pointer to left subtree
/// @param right Pointer to right subtree

node_t *createUnionNode (TYPES type, data_t data, node_t *parent, 
                       node_t *left, node_t *right);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define treeConstructor(treePointer) \
    treeConstructorFunction(treePointer, __FILE__, __LINE__)

/// @brief Constructor for tree_t
/// @param filename Name of file, where Constructor was called
/// @param line Line, where Constructor was called

void treeConstructorFunction (tree_t *tree, const char *filename, const int line);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Node destructor
/// @param Node Pointer to node_t

void nodeDestructor (node_t *Node);

/// @brief Tree destructor
/// @param Tree Pointer to tree_t

ISERROR treeDestructor (tree_t *Tree);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Copy subtree to destination node_t
/// @param destNode Destination node_t
/// @param srcNode Current node_t of source subtree
/// @param parNode Current parent 

void copySubtree (node_t **destNode, node_t *srcNode, node_t *parNode);

/// @brief Count size of subtree
/// @param Node Subtree root
/// @param Size Pointer to size variable

void countSubtreeSize (node_t *Node, size_t *Size);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Print tree_t in pre-order
/// @param Tree Poiter to tree_t
/// @param file Pointer to file

ISERROR preorderPrintTree (const tree_t *Tree, FILE *file = stdout);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Output tree_t in pre-order to file filename.tree_t
/// @param Tree Pointer to tree_t
/// @param filename Name of file

void outputTree (const tree_t *Tree, const char *filename);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Dump tree_t in htm file (Shell for dumpFunction())
/// @param Tree Pointer to tree_t
/// @param file Pointer to file

#define treeDump(Tree, message, file) \
    treeDumpFunction(Tree, message, #Tree, __FILE__, __LINE__, __PRETTY_FUNCTION__, file)

ISERROR treeDumpFunction (const tree_t *Tree,     const char *message,
                          const char *treename, const char *filename, 
                          const int   line,     const char *function, 
                          FILE *file);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
