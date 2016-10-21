#include <stdio.h>
#include <string.h>

// TOKEN TYPES
#define TYPE_IDENTIFIER 0
#define TYPE_CONSTANT   1
#define TYPE_STRING     2
#define TYPE_OPERATOR   3
#define TYPE_PUNCTUATOR 4
#define TYPE_KEYWORD    5

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
#define MAX_TOKEN_SIZE 64
#define TOKEN_ARR_SIZE 256
#define MAX_INPUT_SIZE 1024


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

typedef struct _token {
    int type;
    int size;
    char value[MAX_TOKEN_SIZE];
} Token;

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

int lex(Token *token, InputStr *str);

//////////
// MAIN //
//////////

int main() {

    InputStr str;

    str.pos = 0;

    printf("Please type a segment of code: ");
    fgets(str.value, MAX_INPUT_SIZE, stdin);

    Token tokens[TOKEN_ARR_SIZE];
    int tokensFound = 0;

    while(1) {
        Token t = tokens[tokensFound];

        int success = lex(&t, &str);

        if (success) {
            tokensFound++;
            printf("%s <size = %d, type = %d>\n", t.value, t.size, t.type);
        } else {
            if(str.value[str.pos] == '\0'){
                printf("%s\n", "Finished extracting tokens");
            } else {
                printf("Invalid character'%c' at position %d \n", str.value[str.pos], str.pos);
            }
            break;
        }
    }

    return 0;
}

///////////////
// FUNCTIONS //
///////////////

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
    int i;

    for (i = 0; i < KEYWORD_ARR_SIZE; i++) {
        if (strcmp(string, KEYWORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char advance(InputStr *str) {
    char c = (*str).value[(*str).pos];
    (*str).pos++;
    return c;
}

void incrementToken(Token *token, char currentChar) {
    (*token).value[(*token).size] = currentChar;
    (*token).size++;
}

int lex(Token *token, InputStr *str) {
    int currentState = STATE_S;

    char currentChar;

    (*token).size = 0;
    (*token).value[0] = '\0';

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
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_2;
                    break;

                    case '>':
                    case '<':
                    case '=':
                    case '!':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_3;
                    break;

                    case '|':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_5;
                    break;

                    case '&':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_OP_6;
                    break;

                    case '\"':
                        (*token).type = TYPE_STRING;
                        incrementToken(token, currentChar);
                        currentState = STATE_ST_2;
                    break;

                    case '\'':
                        (*token).type = TYPE_CONSTANT;
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
                        (*token).type = TYPE_PUNCTUATOR;
                        incrementToken(token, currentChar);
                        currentState = STATE_PT_2;
                    break;

                    default:
                        if (isLetter(currentChar)) {
                            (*token).type = TYPE_IDENTIFIER;
                            incrementToken(token, currentChar);
                            currentState = STATE_ID_2;
                        } else if (isDigit(currentChar)) {
                            (*token).type = TYPE_CONSTANT;
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
                    if (isKeyword((*token).value)) {
                        (*token).type = TYPE_KEYWORD;
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
                    (*str).pos--;
                    (*token).size--;
                    (*token).value[(*token).size] = '\0';
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

    (*str).pos--;
    return searchStatus;
}
