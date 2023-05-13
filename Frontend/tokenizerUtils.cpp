#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "config.h"
#include "textUtils.hpp"
#include "tokenizerUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Skipping useless symbols.

static void skipSpaces (char **string)
{
    while (isspace(**string) && (**string != '\0'))
        (*string)++;

    return;
}

static void skipComments (char **string)
{
    if (**string == '<')
        while ((**string != '>') && (**string != '\0'))
            (*string)++;

    if (**string == '>')
        (*string)++;

    return;
}

static void skipUselessSymbols (char **string)
{
    skipSpaces(string);
    skipComments(string);
    skipSpaces(string);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Working with tokens.

static token *tokenConstructor (TYPES type, data_t data)
{
    token *Token = (token *) calloc (1, sizeof(token));

    Token->type = type;
    Token->data = data;

    return Token;
}

static void tokenDestructor (token *Token)
{ 
    free(Token);
    Token = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functions for string scanning.

static double scanNumber (char **string)
{
    double number = NAN;
    int    length = 0;

    sscanf(*string, "%lg%n", &number, &length);

    *string += length;

    return number;
}

static char *scanString (char **string)
{
    char *buffer = NULL;
    int   length = 0;

    sscanf(*string, "%ms%n", &buffer, &length);

    *string += length;

    return buffer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Checks that string contains option.

static OPTIONS getOption (const char *string)
{
    OPTIONS option = UNKNOWN;

    printf("String is '%s'\n", string);
    size_t  length = strlen(string);

    #define DEFINE_OPTION(option, name, ...)                                  \
        if (length == strlen(name) && strncasecmp(name, string, length) == 0) \
            return option;

    #include "options.h"

    #undef DEFINE_OPTION

    return option;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Destructor for token array.

void tokenArrayDestructor (token **tokenArray)
{
    size_t token_idx = 0;

    while (tokenArray[token_idx]->data.option != TERMINATION_SYM)
    {
        if (tokenArray[token_idx]->type == NAME)
        {
            free(tokenArray[token_idx]->data.name);
            tokenArray[token_idx]->data.name = NULL;
        }

        tokenDestructor(tokenArray[token_idx]);

        token_idx++;
    }

    tokenDestructor(tokenArray[token_idx]);

    free(tokenArray);
    tokenArray = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor for token array.

token **tokenArrayConstructor (const char *filename)
{
    text_t text = {};
    textConstructor(&text, filename, false);

    token **tokenArray = (token **) calloc (MaxTokenCount, sizeof(token *));
    size_t tokenNumber = 0;

    char  *bufferPointer = text.buffer;
    while (*bufferPointer != '\0')
    {
        skipUselessSymbols(&bufferPointer);

        if (*bufferPointer == '\0')
            break;

        if (isdigit(*bufferPointer))
        {
            double number = scanNumber(&bufferPointer);

            if (!isfinite(number))
            {
                PUTERROR("Can't scan number.");

                goto DESTRUCTOR;
            }

            tokenArray[tokenNumber] = tokenConstructor(VALUE, {.value = number});

            tokenNumber++;
        }
        
        else
        {
            fprintf(stderr, "Buffer pointer %p | %c | ", bufferPointer, *bufferPointer);
            fprintf(stderr, "%d;\n", *bufferPointer);
            char *string = scanString(&bufferPointer);
            OPTIONS option = getOption(string);

            if (option != UNKNOWN)
            {
                tokenArray[tokenNumber] = tokenConstructor(OPTION, {.option = option});

                free(string);
                string = NULL;
            }

            else
                tokenArray[tokenNumber] = tokenConstructor(NAME,   {.name = string});

            tokenNumber++;
        }
    }

    tokenArray[tokenNumber] = tokenConstructor(OPTION, {.option = TERMINATION_SYM});

    textDestructor(&text);

    return tokenArray;

    DESTRUCTOR:
        textDestructor(&text);
        
        for (size_t token_idx = 0; token_idx < tokenNumber; token_idx++)
        {
            free(tokenArray[token_idx]);
            tokenArray[token_idx] = NULL;
        }

        free(tokenArray);
        tokenArray = NULL;

        return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Dump for token array.

void tokenDump (token *Token)
{
    switch (Token->type)
    {
        case OPTION:
            printf("OPTION: %d\n",  Token->data.option);
            break;

        case VALUE:
            printf("VALUE:  %lg\n", Token->data.value);
            break;

        case SHORT_NAME:
        case VARIABLE: // Can't be initialized in tokenizer. Just for warning remove.
        case NAME:
            printf("NAME:   %s\n",  Token->data.name);
            break;

        default:
            PUTERROR("Unknown type of node.");
            break;
    }
}

void tokenArrayDump (token **tokenArray)
{
    size_t token_idx = 0;

    printf(BOLD MAGENTA "TOKEN ARRAY DUMP:\n" BOLD YELLOW);
    while (tokenArray[token_idx]->data.option != TERMINATION_SYM)
    {
        tokenDump(tokenArray[token_idx]);

        token_idx++;
    }

    printf("TERMINATION SYMBOL: %d\n\n" RESET, tokenArray[token_idx]->data.option);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
