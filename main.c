/*
* main.c
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

#include <stdio.h>
#include <string.h>
#include "sintatico.h"

#define FILENAME_MAX_LENGTH 200
#define FILENAME_ARG_INDEX 1

void copyFileNameFromInputArguments(char *argv[], char filename[]);
FILE* openAndGetPointerToSourceFile(char filename[]);

int main(int argc, char *argv[]){
	char filename[FILENAME_MAX_LENGTH];
	copyFileNameFromInputArguments(argv, filename);

	FILE *pointerToSourceFile = openAndGetPointerToSourceFile(filename);
	if (pointerToSourceFile) {
		compile(pointerToSourceFile);
	}
	return 0;
}

void copyFileNameFromInputArguments(char *argv[], char filename[]){
	strncpy(filename, argv[FILENAME_ARG_INDEX], FILENAME_MAX_LENGTH);
  filename[FILENAME_MAX_LENGTH - 1] = '\0';
}

FILE* openAndGetPointerToSourceFile(char filename[]) {
	char *mode = "r";
	FILE *pointerToSourceFile = fopen(filename, mode);
	if (pointerToSourceFile == NULL) {
		printf("%s\n", "Erro ao ler arquivo fonte" );
	}
	return pointerToSourceFile;
}
