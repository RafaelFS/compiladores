/*
* token.c
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

#include <stdio.h>
#include "token.h"

void printTokenType(int type) {
    switch (type) {
        case TYPE_IDENTIFIER:
            printf("<Identifier>");
            break;
        case TYPE_CONSTANT:
            printf("<Constant>  ");
            break;
        case TYPE_STRING:
            printf("<String>    ");
            break;
        case TYPE_OPERATOR:
            printf("<Operator>  ");
            break;
        case TYPE_PUNCTUATOR:
            printf("<Punctuator>");
            break;
        case TYPE_KEYWORD:
            printf("<Keyword>   ");
            break;
    }
}