#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stddef.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "config.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Colors of node_t filling in dump

static const char globalOptionFillColor[]   = "#9542F5";
static const char globalVariableFillColor[] = "#BE11D1";
static const char globalValueFillColor[]    = "#348BAD";
static const char globalNameFillColor[]     = "#D44724";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Node struct

typedef struct node_t
{
    node_t *parent;
    
    node_t *left;

    TYPES  type;
    data_t data;

    node_t *right;
} node_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Tree struct

typedef struct tree_t
{
    node_t  *root;
    size_t size;

    #ifdef BIRTHINFO

    const char *birthFile;
    int         birthLine;

    #endif
} tree_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
