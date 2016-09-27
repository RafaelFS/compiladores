#include <stdio.h>

#define TYPE_IDENTIFIER 0
#define TYPE_CONSTANT   1
#define TYPE_STRING     2
#define TYPE_OPERATOR   3
#define TYPE_PUNCTUATOR 4

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

const char LETTERS[52] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
const char DIGITS[10]  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const char ASCII[95]   = {' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'};

typedef struct _token {
    int type;
    int size;
    char value[100];
} Token;

typedef struct _inputStr {
    char value[200];
    int pos;
} InputStr;

int isLetter(char c);
int isDigit(char c);
int isAscii(char c);

char advance(InputStr *str);

void incrementToken(Token *token, char currentChar);

int lex(Token *ptoken, InputStr str);

//////////
// MAIN //
//////////

int main() {

    InputStr str;

    str.pos = 0;

    printf("Please type a segment of code: ");
    scanf("%s", str.value);

    Token t;

    printf("%d \n", lex(&t, str));

    printf("%s (%d)\n", t.value, t.type);

    return 0;
}

///////////////
// FUNCTIONS //
///////////////

int isLetter(char c) {
    int i;

    for (i = 0; i < 52; i++) {
        if (c == LETTERS[i]) {
            return 1;
        }
    }
    return 0;
}

int isDigit(char c) {
    int i;
    for (i = 0; i < 10; i++) {
        if (c == DIGITS[i]) {
            return 1;
        }
    }
    return 0;
}

int isAscii(char c) {
    int i;

    for (i = 0; i < 95; i++) {
        if (c == ASCII[i]) {
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
    printf("%s\n", ((*token).value));
}

int lex(Token *token, InputStr str) {
    int currentState = STATE_S;

    char currentChar;
    currentChar = advance(&str);

    (*token).size = 0;

    while (1) {
        switch(currentState) {
            // Initial state
            case STATE_S:

                while (currentChar == ' ' || currentChar == '\t' || currentChar == '\r' || currentChar == '\n') {
                    currentChar = advance(&str);
                }

                switch(currentChar) {
                    case '#':
                        currentChar = advance(&str);
                        currentState = STATE_CS_2;
                    break;

                    case '+':
                    case '-':
                    case '/':
                    case '%':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentChar = advance(&str);
                        currentState = STATE_OP_2;
                    break;

                    case '>':
                    case '<':
                    case '=':
                    case '!':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentChar = advance(&str);
                        currentState = STATE_OP_3;
                    break;

                    case '*':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentChar = advance(&str);
                        currentState = STATE_OP_4;
                    break;

                    case '|':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentChar = advance(&str);
                        currentState = STATE_OP_5;
                    break;

                    case '&':
                        (*token).type = TYPE_OPERATOR;
                        incrementToken(token, currentChar);
                        currentChar = advance(&str);
                        currentState = STATE_OP_6;
                    break;

                    case '\"':
                        (*token).type = TYPE_STRING;
                        incrementToken(token, currentChar);
                        currentChar = advance(&str);
                        currentState = STATE_ST_2;
                    break;

                    case '\'':
                        (*token).type = TYPE_CONSTANT;
                        incrementToken(token, currentChar);
                        currentChar = advance(&str);
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
                        currentChar = advance(&str);
                        currentState = STATE_PT_2;
                    break;

                    default:
                    printf("%c", currentChar);
                        if (isLetter(currentChar)) {
                            printf("Letra");
                            (*token).type = TYPE_IDENTIFIER;
                            incrementToken(token, currentChar);
                            currentChar = advance(&str);
                            currentState = STATE_ID_2;
                        } else if (isDigit(currentChar)) {
                            (*token).type = TYPE_CONSTANT;
                            incrementToken(token, currentChar);
                            currentChar = advance(&str);
                            currentState = STATE_CT_2;
                        } else {
                            return 0;
                        }
                }
            break;

            // Comments & Spacing
            case STATE_CS_2:
                while (currentChar != '\r' && currentChar != '\n') {
                    currentChar = advance(&str);
                }
                currentState = STATE_S;
            break;

            // Identifier
            case STATE_ID_2:
                while (isLetter(currentChar) == 1 || isDigit(currentChar) == 1) {
                    incrementToken(token, currentChar);
                    printf("%s %c\n", "É identificador char atual: ", currentChar);
                    currentChar = advance(&str);
                }
                // check keyword
                return 1;
            break;

            // Constant
            case STATE_CT_2:
                while (isDigit(currentChar)) {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                }
                if (currentChar == '.') {
                    currentChar = advance(&str);
                    currentState = STATE_CT_3;
                } else {
                    return 1;
                }
            break;

            case STATE_CT_3:
                if (isDigit(currentChar)) {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_CT_4;
                } else {
                    // if no digit was found after the '.', remove it from the token and return it, since it is a valid number
                    (*token).size--;
                    return 1;
                }
            break;

            case STATE_CT_4:
                while (isDigit(currentChar)) {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                }
                return 1;
            break;

            case STATE_CT_5:
                if (currentChar != '\'' && currentChar != '\\' && isAscii(currentChar)) {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_CT_6;
                } else if (currentChar == '\\') {
                    currentChar = advance(&str);
                    currentState = STATE_CT_8;
                } else {
                    return 0;
                }
            break;

            case STATE_CT_6:
                if (currentChar == '\'') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_CT_7;
                } else {
                    return 0;
                }
            break;

            case STATE_CT_7:
                return 1;
            break;

            case STATE_CT_8:
                if (currentChar == '\\' || currentChar == '"' || currentChar == '\'' || currentChar == 'n' || currentChar == 't' || currentChar == 'r') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_CT_6;
                } else {
                    return 0;
                }
            break;

            // String
            case STATE_ST_2:
                while (currentChar != '"' && currentChar != '\\' && isAscii(currentChar)) {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                }
                if (currentChar == '"') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_ST_3;
                } else if (currentChar == '\\') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_ST_4;
                } else {
                    return 0;
                }
            break;

            case STATE_ST_3:
                return 1;
            break;

            case STATE_ST_4:
                if (currentChar == '\\' || currentChar == '"' || currentChar == '\'' || currentChar == 'n' || currentChar == 't' || currentChar == 'r') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_ST_2;
                } else {
                    return 0;
                }
            break;

            // Operator
            case STATE_OP_2:
                return 1;
            break;

            case STATE_OP_3:
                if (currentChar == '=') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_OP_2;
                } else {
                    return 1;
                }
            break;

            case STATE_OP_4:
                if (currentChar == '*') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_OP_2;
                } else {
                    return 1;
                }
            break;

            case STATE_OP_5:
                if (currentChar == '|') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_OP_2;
                } else {
                    return 0;
                }
            break;

            case STATE_OP_6:
                if (currentChar == '&') {
                    incrementToken(token, currentChar);
                    currentChar = advance(&str);
                    currentState = STATE_OP_2;
                } else {
                    return 0;
                }
            break;

            // Punctuator
            case STATE_PT_2:
                return 1;
            break;

            default:
                return 0;
        }

    }
}
