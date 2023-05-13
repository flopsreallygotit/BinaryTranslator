#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeUtils.h"
#include "replacements.h"
#include "tokenizerUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Parses file and creates tree depending on it.
/// @param filename Name of file with programm.
tree *parseTokens (token **tokenArray);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Recursive descent:

node *getGrammar      (FUNC_ARGUMENTS);

node *getStatement    (FUNC_ARGUMENTS);

node *getFunction     (FUNC_ARGUMENTS);

node *getHeader       (FUNC_ARGUMENTS);

node *getParameter    (FUNC_ARGUMENTS);

node *getCondition    (FUNC_ARGUMENTS);

node *getCycle        (FUNC_ARGUMENTS);

node *getReturn       (FUNC_ARGUMENTS);

node* getCall         (FUNC_ARGUMENTS);

node* getInOutput     (FUNC_ARGUMENTS);

node* getVariable     (FUNC_ARGUMENTS);

node* getExpression   (FUNC_ARGUMENTS);

node* getAddSub       (FUNC_ARGUMENTS);

node* getMulDiv       (FUNC_ARGUMENTS);

node* getPower        (FUNC_ARGUMENTS);

node* getBrackets     (FUNC_ARGUMENTS);

node* getUnary        (FUNC_ARGUMENTS);

node *getNumberOrName (FUNC_ARGUMENTS);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
