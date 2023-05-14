#ifndef CONFIG_H
#define CONFIG_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stddef.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum SIZES
{
    MaxTokenCount     = 10000,
    MaxVariableNumber = 5000,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Options that language support.
enum OPTIONS
{
    // Unknown type.
    UNKNOWN = 0,

    // Standart operations.
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    POW = 5,

    // Trigonometry.
    SIN = 6,
    COS = 7,

    // Square root moment.
    SQRT = 8,

    // Boolean algebra.
    IS_EE = 11,
    IS_GE = 12,
    IS_BE = 13,
    IS_GT = 14,
    IS_BT = 15,
    IS_NE = 16,
    
    // Special nodes.
    ST    = 18,
    IF    = 19,
    ELSE  = 20,
    NIL   = 21,
    VAR   = 22,
    WHILE = 23,
    FUNC  = 24,
    RET   = 25,
    CALL  = 26,
    PARAM = 27,

    // Input / Output.
    IN  = 28,
    OUT = 29,

    // Tokenizer extra types.
    OPEN_BR         = 30,
    CLOSE_BR        = 31,
    TERMINATION_SYM = 32,
    SEMI_COL        = 33,

    // I don'r really know yet what they're meaning, but they're in standart.
    VOID = 34,
    TYPE = 35,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Type of data.
enum TYPES
{
    OPTION     = 0,
    VALUE      = 1,
    NAME       = 2,
    SHORT_NAME = 3,
    VARIABLE   = 4,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Union for node data.
typedef union data_t
{
    OPTIONS option;
    char   *name;
    double  value;
} data_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Struct of variable.
typedef struct variable_t
{
    char *function;
    char *name;

    size_t ramIP;
} variable_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Token is one meaningful code unit.
typedef struct token_t
{
    TYPES type;
    data_t data;
} token_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
