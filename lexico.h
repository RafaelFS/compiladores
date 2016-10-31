/*
* lexico.h
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

#include "token.h"

// STATES
#define STATE_S      0
#define STATE_CS_2   2
#define STATE_ID_2 102
#define STATE_CT_2 202
#define STATE_CT_3 203
#define STATE_CT_4 204
#define STATE_CT_5 205
#define STATE_CT_6 206
#define STATE_CT_7 207
#define STATE_CT_8 208
#define STATE_ST_2 302
#define STATE_ST_3 303
#define STATE_ST_4 304
#define STATE_OP_2 402
#define STATE_OP_3 403
#define STATE_OP_4 404
#define STATE_OP_5 405
#define STATE_OP_6 406
#define STATE_PT_2 502

// SIZE_CONSTANTS
#define MAX_KEYWORD_SIZE 64
#define KEYWORD_ARR_SIZE 19
#define MAX_INPUT_SIZE 16384

typedef struct _inputStr {
    char value[MAX_INPUT_SIZE];
    int pos;
} InputStr;

int isLetter(char c);
int isDigit(char c);
int isPrintableAscii(char c);
int isKeyword(char string[MAX_KEYWORD_SIZE]);

char advance(InputStr *str);

void incrementToken(Token *token, char currentChar);

int getNextToken(Token *token, InputStr *str);
