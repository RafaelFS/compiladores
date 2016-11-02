/*
* lexico.c
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/

#include <stdio.h>
#include <string.h>
#include "lexico.h"

int isLetter(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    } else {
        return 0;
    }
}

int isDigit(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    } else {
        return 0;
    }
}

int isPrintableAscii(char c) {
    if (c >= ' ' && c <= '~') {
        return 1;
    } else {
        return 0;
    }
}

int isKeyword(char string[MAX_KEYWORD_SIZE]) {

    const char KEYWORDS[KEYWORD_ARR_SIZE][MAX_KEYWORD_SIZE] = {
        {'t', 'y', 'p', 'e', 's'},
        {'i', 'n', 't'},
        {'f', 'l', 'o', 'a', 't'},
        {'b', 'o', 'o', 'l'},
        {'c', 'h', 'a', 'r'},
        {'s', 't', 'r', 'u', 'c', 't'},
        {'g', 'l', 'o', 'b', 'a', 'l'},
        {'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', 's'},
        {'i', 'm', 'p', 'l', 'e', 'm', 'e', 'n', 't', 'a', 't', 'i', 'o', 'n'},
        {'m', 'a', 'i', 'n'},
        {'t', 'r', 'u', 'e'},
        {'f', 'a', 'l', 's', 'e'},
        {'i', 'f'},
        {'e', 'l', 's', 'i', 'f'},
        {'e', 'l', 's', 'e'},
        {'w', 'h', 'i', 'l', 'e'},
        {'p', 'r', 'i', 'n', 't'},
        {'r', 'e', 'a', 'd'},
        {'r', 'e', 't', 'u', 'r', 'n'}
    };

    int i;

    for (i = 0; i < KEYWORD_ARR_SIZE; i++) {
        if (strcmp(string, KEYWORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char advance(InputStr *str) {
    char c = str->value[str->pos];
    str->pos++;
    return c;
}

void incrementToken(Token *token, char currentChar) {
    token->value[token->size] = currentChar;
    token->size++;
}

int getNextToken(Token *token, InputStr *str) {
    int currentState = STATE_S;

    char currentChar;

    token->size = 0;
    token->value[0] = '\0';

    int searchStatus = -1;

    while (searchStatus != 0 && searchStatus != 1) {
        currentChar = advance(str);

        switch(currentState) {
            // Initial state
            case STATE_S:
                switch(currentChar) {
                    case ' ':
                    case '\t':
                    case '\n':
                    case '\r':
                        currentState = STATE_S;
                    break;

                    case '#':
                        currentState = STATE_CS_2;
                    break;

                    case '+':
                    case '-':
                    case '/':
                    case '%':
                    case '*':
                        token->type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_2;
                    break;

                    case '>':
                    case '<':
                    case '=':
                    case '!':
                        token->type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_3;
                    break;

                    case '|':
                        token->type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_5;
                    break;

                    case '&':
                        token->type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_6;
                    break;

                    case '\"':
                        token->type = TYPE_STRING;
                        incrementToken(token, currentChar);
                        currentState = STATE_ST_2;
                    break;

                    case '\'':
                        token->type = TYPE_CONSTANT;
                        incrementToken(token, currentChar);
                        currentState = STATE_CT_5;
                    break;

                    case '(':
                    case ')':
                    case '{':
                    case '}':
                    case '[':
                    case ']':
                    case ';':
                    case ',':
                    case '.':
                        token->type = TYPE_PUNCTUATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_PT_2;
                    break;

                    default:
                        if (isLetter(currentChar)) {
                            token->type = TYPE_IDENTIFIER;
                            incrementToken(token, currentChar);
                            currentState = STATE_ID_2;
                        } else if (isDigit(currentChar)) {
                            token->type = TYPE_CONSTANT;
                            incrementToken(token, currentChar);
                            currentState = STATE_CT_2;
                        } else {
                            searchStatus = 0;
                        }
                }
            break;

            // Comments & Spacing
            case STATE_CS_2:
                if (currentChar != '\r' && currentChar != '\n') {
                    currentState = STATE_CS_2;
                } else {
                    currentState = STATE_S;
                }
            break;

            // Identifier
            case STATE_ID_2:
                if (isLetter(currentChar) == 1 || isDigit(currentChar) == 1) {
                    incrementToken(token, currentChar);
                    currentState = STATE_ID_2;
                } else {
                    if (isKeyword(token->value)) {
                        token->type = TYPE_KEYWORD;
                    }
                    searchStatus = 1;
                }
            break;

            // Constant
            case STATE_CT_2:
                if (isDigit(currentChar)) {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_2;
                } else if (currentChar == '.') {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_3;
                } else {
                    searchStatus = 1;
                }
            break;

            case STATE_CT_3:
                if (isDigit(currentChar)) {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_4;
                } else {
                    // if no digit was found after the '.', remove it from the token and return it, since it is a valid number
                    str->pos--;
                    token->size--;
                    token->value[token->size] = '\0';
                    searchStatus = 1;
                }
            break;

            case STATE_CT_4:
                if (isDigit(currentChar)) {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_4;
                } else {
                    searchStatus = 1;
                }
            break;

            case STATE_CT_5:
                if (currentChar != '\'' && currentChar != '\\' && isPrintableAscii(currentChar)) {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_6;
                } else if (currentChar == '\\') {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_8;
                } else {
                    searchStatus = 0;
                }
            break;

            case STATE_CT_6:
                if (currentChar == '\'') {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_7;
                } else {
                    searchStatus = 0;
                }
            break;

            case STATE_CT_7:
                searchStatus = 1;
            break;

            case STATE_CT_8:
                if (currentChar == '\\' || currentChar == '"' || currentChar == '\'' || currentChar == 'n' || currentChar == 't' || currentChar == 'r') {
                    incrementToken(token, currentChar);
                    currentState = STATE_CT_6;
                } else {
                    searchStatus = 0;
                }
            break;

            // String
            case STATE_ST_2:
                if (currentChar != '"' && currentChar != '\\' && isPrintableAscii(currentChar)) {
                    incrementToken(token, currentChar);
                    currentState = STATE_ST_2;
                } else if (currentChar == '"') {
                    incrementToken(token, currentChar);
                    currentState = STATE_ST_3;
                } else if (currentChar == '\\') {
                    incrementToken(token, currentChar);
                    currentState = STATE_ST_4;
                } else {
                    searchStatus = 0;
                }
            break;

            case STATE_ST_3:
                searchStatus = 1;
            break;

            case STATE_ST_4:
                if (currentChar == '\\' || currentChar == '"' || currentChar == '\'' || currentChar == 'n' || currentChar == 't' || currentChar == 'r') {
                    incrementToken(token, currentChar);
                    currentState = STATE_ST_2;
                } else {
                    searchStatus = 0;
                }
            break;

            // Operator
            case STATE_OP_2:
                searchStatus = 1;
            break;

            case STATE_OP_3:
                if (currentChar == '=') {
                    incrementToken(token, currentChar);
                    currentState = STATE_OP_2;
                } else {
                    searchStatus = 1;
                }
            break;

            case STATE_OP_5:
                if (currentChar == '|') {
                    incrementToken(token, currentChar);
                    currentState = STATE_OP_2;
                } else {
                    searchStatus = 0;
                }
            break;

            case STATE_OP_6:
                if (currentChar == '&') {
                    incrementToken(token, currentChar);
                    currentState = STATE_OP_2;
                } else {
                    searchStatus = 0;
                }
            break;

            // Punctuator
            case STATE_PT_2:
                searchStatus = 1;
            break;

            default:
                searchStatus = 0;
        }
    }

    str->pos--;
    return searchStatus;
}
