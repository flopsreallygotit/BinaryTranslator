#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <assert.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "tree.hpp"
#include "config.hpp"
#include "tokenizer.hpp"
#include "replacements.hpp"
#include "universalUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef struct binary_t {
    size_t size;
    size_t dataSize;

    char *data;
} binary_t;

typedef struct program_t
{
    node_t     *node;
    stack_t    *stack;

    function_t *functionsArray;
    size_t functionNumber;

    binary_t *code;
    int ramIP;

    bool main;
} program_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Make language standart tree out of tokens

node_t *getGrammar(LONG_FUNC_ARGUMENTS);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Creates binary code struct 
 
binary_t *binaryConstructor ();

/// @brief Deletes binary code struct

void binaryDestructor (binary_t *code);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void programConstructor (program_t *program, node_t *node);
void programDestructor  (program_t *program);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Create variables table

table_t *tableConstructor (stack_t *stack);

/// @brief Delete variables table

void tableDestructor(table_t *table);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void parseCall      (node_t *node, program_t *program);
void parseStatement (node_t *node, program_t *program);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
