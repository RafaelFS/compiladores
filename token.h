/*
* token.h
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

#include <string.h>

typedef enum Class { Digit, Unknown, EndOfFile} Class;

typedef struct Token {
   Class class;
   char *value;
} Token;
