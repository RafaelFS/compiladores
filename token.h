/*
* token.h
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

// TOKEN TYPES
#define TYPE_IDENTIFIER 0
#define TYPE_CONSTANT   1
#define TYPE_STRING     2
#define TYPE_OPERATOR   3
#define TYPE_PUNCTUATOR 4
#define TYPE_KEYWORD    5

#define MAX_TOKEN_SIZE 64

typedef struct _token {
    int type;
    int size;
    char value[MAX_TOKEN_SIZE];
} Token;

void printTokenType(int type);