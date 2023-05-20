#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "config.hpp"
#include "universalUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Makes token_t array.
/// @param tokenArray Token array.
token_t **tokenArrayConstructor (const char *filename);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Prints token_t
/// @param Token Pointer to token_t.
void tokenDump (token_t *Token);

/// @brief Prints token_t array.
/// @param tokenArray Array of tokens.
void tokenArrayDump (token_t **tokenArray);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Deletes token_t array.
/// @param tokenArray Token array.
void tokenArrayDestructor (token_t **tokenArray);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
