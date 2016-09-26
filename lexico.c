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

Token getNextToken(FILE *pointerToSourceFile) {
	char currentCharacter;
	Token currentToken;

	int numberOfReadCharacters = fscanf(pointerToSourceFile, "%c", &currentCharacter);

	if (hasValidNumberOfReadCharacters(numberOfReadCharacters)) {
		currentToken.class = Unknown;
	} else {
		currentCharacter = EOF;
		currentToken.class = EndOfFile;
	}

	currentToken.value = &currentCharacter;

	return currentToken;
}

int hasValidNumberOfReadCharacters(int numberOfReadCharacters) {
	return (numberOfReadCharacters > 0);
}
