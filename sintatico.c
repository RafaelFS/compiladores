/*
* sintatico.c
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/
#include <stdio.h>
#include "sintatico.h"
#include "lexico.h"

void compile(FILE *pointerToSourceFile) {

    InputStr str;

    str.pos = 0;

    char c;

    do {
        c = fgetc(pointerToSourceFile);
        str.value[str.pos] = c;
        str.pos++;
    } while (c != EOF);

    str.pos = 0;

    Token tokens[TOKEN_ARR_SIZE];
    int tokensFound = 0;

    while(1) {
        Token t = tokens[tokensFound];

        int success = getNextToken(&t, &str);

        if (success) {
            tokensFound++;
            printTokenType(t.type);
            printf("\t %s \n", t.value);
        } else {
            if(str.value[str.pos] == EOF){
                printf("Finished extracting %d tokens.\n", tokensFound);
            } else {
                printf("Invalid character'%c' at position %d.\n", str.value[str.pos], str.pos);
            }
            break;
        }
    }
}
