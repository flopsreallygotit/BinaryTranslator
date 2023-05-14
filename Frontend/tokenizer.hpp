#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "config.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Makes token_t array.
/// @param tokenArray token array.
token_t **tokenArrayConstructor (const char *filename);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Prints token_t
/// @param token Pointer to token_t.
void tokenDump (token_t *token);

/// @brief Prints token_t array.
/// @param tokenArray Array of tokens.
void tokenArrayDump (token_t **tokenArray);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Deletes token_t array.
/// @param tokenArray token array.
void tokenArrayDestructor (token_t **tokenArray);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
