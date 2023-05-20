#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "textUtils.hpp"
#include "tokenizer.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Skipping useless symbols.

static void skipSpaces (char **string)
{
    while (isspace(**string) && (**string != '\0'))
        (*string)++;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Working with tokens.

static token_t *tokenConstructor (TYPES type, data_t data)
{
    token_t *token = (token_t *) calloc (1, sizeof(token_t));

    token->type = type;
    token->data = data;

    return token;
}

static void tokenDestructor (token_t *token)
{ 
    free(token);
    token = NULL;

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

static char *scanString (char **string, const char *specificator)
{
    char *buffer = NULL;
    int   length = 0;

    sscanf(*string, specificator, &buffer, &length);

    *string += length;

    return buffer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Checks that string contains option.

static OPTIONS getOption (const char *string)
{
    OPTIONS option = UNKNOWN;
    size_t  length = strlen(string);

    #define DEFINE_OPTION(Option, ...)                                              \
        if (length == strlen(#Option) && strncasecmp(#Option, string, length) == 0) \
            return Option;

    #include "options.hpp"

    #undef DEFINE_OPTION

    return option;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Destructor for token_t array.

void tokenArrayDestructor (token_t **tokenArray)
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

// Constructor for token_t array.

token_t **tokenArrayConstructor (const char *filename)
{
    text_t text = {};
    textConstructor(&text, filename, false);

    token_t **tokenArray = (token_t **) calloc (MaxTokenCount, sizeof(token_t *));
    size_t tokenNumber = 0;

    char  *bufferPointer = text.buffer;
    while (*bufferPointer != '\n' && *bufferPointer != '\0')
    {
        skipSpaces(&bufferPointer);

        if (*bufferPointer == '{' ||
            *bufferPointer == '}')
            bufferPointer++;
        
        else if (*bufferPointer == '"')
        {
            bufferPointer++;

            char *name = scanString(&bufferPointer, "%m[^\"]%n");
            tokenArray[tokenNumber] = tokenConstructor(NAME, {.name = name});

            bufferPointer++;
            tokenNumber++;
        }

        else if (isdigit(*bufferPointer))
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
        
        else if (isalpha(*bufferPointer))
        {
            char *string = scanString(&bufferPointer, "%m[^{}]%n");

            OPTIONS option = getOption(string);

            if (option != UNKNOWN)
            {
                tokenArray[tokenNumber] = tokenConstructor(OPTION, {.option = option});

                free(string);
                string = NULL;
            }

            else
                PUTERROR("Unknown option.");

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

// Dump for token_t array.

void tokenDump (token_t *token)
{
    switch (token->type)
    {
        case OPTION:
            printf("OPTION: %d\n",  token->data.option);
            break;

        case VALUE:
            printf("VALUE:  %lg\n", token->data.value);
            break;

        case SHORT_NAME:
        case VARIABLE: // Can't be initialized in tokenizer. Just for warning remove.
        case NAME:
            printf("NAME:   %s\n",  token->data.name);
            break;

        default:
            PUTERROR("Unknown type of node.");
            break;
    }
}

void tokenArrayDump (token_t **tokenArray)
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
