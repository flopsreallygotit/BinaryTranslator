#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "tree.hpp"
#include "config.hpp"
#include "replacements.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Recursive descent:

node_t *getGrammar      (FUNC_ARGUMENTS);

node_t *getStatement    (FUNC_ARGUMENTS);

node_t *getFunction     (FUNC_ARGUMENTS);

node_t *getHeader       (FUNC_ARGUMENTS);

node_t *getParameter    (FUNC_ARGUMENTS);

node_t *getCondition    (FUNC_ARGUMENTS);

node_t *getCycle        (FUNC_ARGUMENTS);

node_t *getReturn       (FUNC_ARGUMENTS);

node_t* getCall         (FUNC_ARGUMENTS);

node_t* getInOutput     (FUNC_ARGUMENTS);

node_t* getVariable     (FUNC_ARGUMENTS);

node_t* getExpression   (FUNC_ARGUMENTS);

node_t* getAddSub       (FUNC_ARGUMENTS);

node_t* getMulDiv       (FUNC_ARGUMENTS);

node_t* getPower        (FUNC_ARGUMENTS);

node_t* getBrackets     (FUNC_ARGUMENTS);

node_t* getUnary        (FUNC_ARGUMENTS);

node_t *getNumberOrName (FUNC_ARGUMENTS);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
