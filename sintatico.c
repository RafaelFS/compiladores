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
	Token currentToken = getNextToken(pointerToSourceFile);
	while(currentToken.class != EndOfFile){
		printf("%c\n", *currentToken.value);
		currentToken = getNextToken(pointerToSourceFile);
	}
}
