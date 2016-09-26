/*
* lexico.c
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

#include <stdio.h>
#include "lexico.h"


int hasValidNumberOfReadCharacters(int numberOfReadCharacters);

char getNextToken(FILE *pointerToSourceFile) {
	char currentCharacter;
	int numberOfReadCharacters = fscanf(pointerToSourceFile, "%c", &currentCharacter);

	if (hasValidNumberOfReadCharacters(numberOfReadCharacters)) {
		return currentCharacter;
	} else {
		return EOF;
	}
}

int hasValidNumberOfReadCharacters(int numberOfReadCharacters) {
	return (numberOfReadCharacters > 0);
}
