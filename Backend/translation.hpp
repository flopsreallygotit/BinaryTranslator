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

/// @brief Create array of variables
/// @param capacity Capacity of array 

variable_t **variableArrayConstructor (size_t capacity);

/// @brief Delete array of variables
/// @param variableArray Array of variables
/// @param capacity Capacity of array
void variableArrayDestructor (variable_t **variableArray, size_t capacity);

/// @brief Parses language standart tree

node_t *getGrammar (LONG_FUNC_ARGUMENTS);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
