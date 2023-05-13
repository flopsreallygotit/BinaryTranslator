#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeConfig.h"
#include "universalUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Constructs node
/// @param type Type of data
/// @param data Content of node
/// @param parent Parent of node

node *nodeConstructor (TYPES type, data_t data, node *parent);

/// @brief Create node that becomes root of left subtree and right subtree
/// @param type Type of data
/// @param data Content of node
/// @param parent Parent of node
/// @param left Pointer to left subtree
/// @param right Pointer to right subtree

node *createUnionNode (TYPES type, data_t data, node *parent, 
                       node *left, node *right);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define treeConstructor \
    treeConstructorFunction(__FILE__, __LINE__)

/// @brief Constructor for tree
/// @param filename Name of file, where Constructor was called
/// @param line Line, where Constructor was called

tree *treeConstructorFunction (const char *filename, const int line);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Node destructor
/// @param Node Pointer to node

void nodeDestructor (node *Node);

/// @brief Tree destructor
/// @param Tree Pointer to tree

ISERROR treeDestructor (tree *Tree);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Copy subtree to destination node
/// @param destNode Destination node
/// @param srcNode Current node of source subtree
/// @param parNode Current parent 

void copySubtree (node **destNode, node *srcNode, node *parNode);

/// @brief Count size of subtree
/// @param Node Subtree root
/// @param Size Pointer to size variable

void countSubtreeSize (node *Node, size_t *Size);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Print tree in pre-order
/// @param Tree Poiter to tree
/// @param file Pointer to file

ISERROR preorderPrintTree (const tree *Tree, FILE *file = stdout);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Output tree in pre-order to file filename.tree
/// @param Tree Pointer to tree
/// @param filename Name of file

void outputTree (const tree *Tree, const char *filename);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Dump tree in htm file. (Shell for dumpFunction())
/// @param Tree Pointer to tree
/// @param file Pointer to file

#define treeDump(Tree, message, file) \
    treeDumpFunction(Tree, message, #Tree, __FILE__, __LINE__, __PRETTY_FUNCTION__, file)

ISERROR treeDumpFunction (const tree *Tree,     const char *message,
                          const char *treename, const char *filename, 
                          const int   line,     const char *function, 
                          FILE *file);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
