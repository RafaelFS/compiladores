/*
* main.c
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

#include <stdio.h>
#include "sintatico.h"

int main(int argc, char *argv[]){
	printf("%s\n", "Testando main");
	compile();
	return 0;
}
