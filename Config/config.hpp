#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stddef.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static const int Poizon = 0xAB0BA;

enum SIZES
{
    MaxTokenCount     = 8192,
    MaxVariableNumber = 8192,
    MaxFunctionsCount = 64, 
    MaxTableSize      = 10,

    PageSize = 4096,
    QWord    = 8, 
    RamSize  = 256,

    PrintfSize = 0xA8,
    ScanfSize  = 0xAC,
    MemorySize = 0x100,

    PrintfBuffer = 20,
    ScanfBuffer  = 20,
    ItoaBuffer   = 20,

    EntryPoint = 0x400078, 
};

static const char registerName[4][4] = {"RAX", "RCX", "RDX", "RDX"};

enum REG_MASKS
{
    RAX_MASK = 0x0,
    RBX_MASK = 0x3,
    RCX_MASK = 0x1,
    RDX_MASK = 0x2,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum PUSH_TYPE
{
    PUSH_IMM         = 0,
    PUSH_REG         = 1,
    PUSH_IMM_MEM     = 2,
    PUSH_REG_MEM     = 3,
    PUSH_IMM_REG_MEM = 4,
};

enum POP_TYPE 
{
    POP_REG         = 0,
    POP_REG_MEM     = 1,
    POP_IMM_MEM     = 2,
    POP_IMM_REG_MEM = 3,
};

enum JUMP_TYPE
{
    JMP_JMP  = 0, 
    JMP_JE   = 1,
    JMP_JNE  = 2,
    JMP_JG   = 3,
    JMP_JGE  = 4,
    JMP_JL   = 5, 
    JMP_JLE  = 6,
    JMP_CALL = 7,
};

enum OPERATIONS
{
    OP_NULL = -1,

    OP_ADD = 0,
    OP_SUB = 1,

    OP_MUL = 3,
    OP_DIV = 4,

    OP_LOG = 6,
    OP_LN  = 7,

    OP_SIN = 9,
    OP_COS = 10,

    OP_POW = 12,

    OP_IS_BT = 14,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Options that language support

enum OPTIONS
{
    // Unknown type
    UNKNOWN = 0,

    // Standart operations
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    POW = 5,

    // Trigonometry
    SIN = 6,
    COS = 7,

    // Square root moment
    SQRT = 8,

    // Equation symbols
    AND   = 9,
    OR    = 10,
    EQ    = 11,
    IS_EE = 12,
    IS_GE = 13,
    IS_BE = 14,
    IS_GT = 15,
    IS_BT = 16,
    IS_NE = 17,
    
    // Special nodes
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

    // Input / Output
    IN  = 28,
    OUT = 29,

    // Tokenizer extra types
    OPEN_BR         = 30,
    CLOSE_BR        = 31,
    TERMINATION_SYM = 32,
    SEMI_COL        = 33,

    VOID = 34,
    TYPE = 35,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Types of data

enum TYPES
{
    OPTION     = 0,
    VALUE      = 1,
    NAME       = 2,
    SHORT_NAME = 3,
    VARIABLE   = 4,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Union for node data

typedef union data_t
{
    OPTIONS option;
    char   *name;
    double  value;
} data_t;

typedef struct variable_t
{
    char *name;

    size_t position;
} variable_t;

typedef struct table_t
{
    size_t size;
    size_t registerPosition;

    variable_t *variableArray;
} table_t;

typedef struct function_t
{
    char *name;
    size_t paramNumber;

    OPTIONS returnValue; // TODO maybe replace with bool

    size_t functionStart;
} function_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Stack element type

typedef table_t * elem_t;

/// @brief Stack struct

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef struct stack_t
{
    #ifdef STRUCTCANARY

    canary_t leftCanary;

    #endif

    elem_t *data;

    size_t  capacity;
    size_t  size;

    #ifdef HASH

    size_t  currentSum;

    #endif

    #ifdef BIRTHINFO

    size_t      birthLine;
    const char *birthFile;

    #endif

    #ifdef STRUCTCANARY

    canary_t rightCanary;

    #endif
}stack_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Token is one meaningful code unit

typedef struct token_t
{
    TYPES  type;
    data_t data;
} token_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
