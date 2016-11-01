/*
* sintatico.h
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/
#include "lexico.h"

#define TOKEN_ARR_SIZE 4096

typedef struct _tokenArr {
    Token tokens[TOKEN_ARR_SIZE];
    int size;
    int pos;
} TokenArr;

void compile(FILE *pointerToSourceFile);
void readFromFile(InputStr *str, FILE *pointerToSourceFile);
int getTokens(TokenArr *tokenArr, InputStr *str);
int checkSyntax(TokenArr *tokenArr);
void todo(char str[100]);

int autProgram(TokenArr *tokenArr);
int autTypes(TokenArr *tokenArr);
int autFunctions(TokenArr *tokenArr);
int autMain(TokenArr *tokenArr);
int autGlobal(TokenArr *tokenArr);
int autTypeDef(TokenArr *tokenArr);
int autName(TokenArr *tokenArr);
int autCustomType(TokenArr *tokenArr);
int autType(TokenArr *tokenArr);
int autStruct(TokenArr *tokenArr);
int autDecl(TokenArr *tokenArr);
int autFuncDecl(TokenArr *tokenArr);
int autFuncImpl(TokenArr *tokenArr);
int autFuncCmdList(TokenArr *tokenArr);
int autCmd(TokenArr *tokenArr);
int autExpression(TokenArr *tokenArr);
int autAttr(TokenArr *tokenArr);
int autIfCmd(TokenArr *tokenArr);
int autItrCmd(TokenArr *tokenArr);
int autInCmd(TokenArr *tokenArr);
int autOutCmd(TokenArr *tokenArr);
int autIntNum(TokenArr *tokenArr);
