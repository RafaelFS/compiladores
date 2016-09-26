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
	char currentCharacter = getNextToken(pointerToSourceFile);
	while(currentCharacter != EOF){
		printf("%c\n", currentCharacter);
		currentCharacter = getNextToken(pointerToSourceFile);
	}
}
