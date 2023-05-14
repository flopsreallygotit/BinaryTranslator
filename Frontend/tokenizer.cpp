#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "textUtils.hpp"
#include "tokenizer.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Checks that string contains option.

static OPTIONS getOption (const char *string)
{
    OPTIONS option = UNKNOWN;

    #ifdef DEBUG
    printf("String is '%s'\n", string);
    #endif
    
    size_t  length = strlen(string);

    #define DEFINE_OPTION(option, name, ...)                                  \
        if (length == strlen(name) && strncasecmp(name, string, length) == 0) \
            return option;

    #include "options.hpp"

    #undef DEFINE_OPTION

    return option;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Destructor for token_t array.

void tokenArrayDestructor (token_t **tokenArray)
{
    size_t tokenIndex = 0;

    while (tokenArray[tokenIndex]->data.option != TERMINATION_SYM)
    {
        if (tokenArray[tokenIndex]->type == NAME)
        {
            free(tokenArray[tokenIndex]->data.name);
            tokenArray[tokenIndex]->data.name = NULL;
        }

        tokenDestructor(tokenArray[tokenIndex]);

        tokenIndex++;
    }

    tokenDestructor(tokenArray[tokenIndex]);

    free(tokenArray);
    tokenArray = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor for token_t array.

token_t **tokenArrayConstructor (const char *filename)
{
    text_t text = {};
    textConstructor(&text, filename, false);

    assert(text.buffer != NULL);

    token_t **tokenArray = (token_t **) calloc (MaxTokenCount, sizeof(token_t *));
    size_t tokenNumber = 0;

    char *bufferPointer = text.buffer;
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
            #ifdef DEBUG
            fprintf(stderr, "Buffer pointer %p | %c | ", bufferPointer, *bufferPointer);
            fprintf(stderr, "%d;\n", *bufferPointer);
            #endif

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
        
        for (size_t tokenIndex = 0; tokenIndex < tokenNumber; tokenIndex++)
        {
            free(tokenArray[tokenIndex]);
            tokenArray[tokenIndex] = NULL;
        }

        free(tokenArray);
        tokenArray = NULL;

        return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
    size_t tokenIndex = 0;

    printf(BOLD MAGENTA "TOKEN ARRAY DUMP:\n" BOLD YELLOW);
    while (tokenArray[tokenIndex]->data.option != TERMINATION_SYM)
    {
        tokenDump(tokenArray[tokenIndex]);

        tokenIndex++;
    }

    printf("TERMINATION SYMBOL: %d\n\n" RESET, tokenArray[tokenIndex]->data.option);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
