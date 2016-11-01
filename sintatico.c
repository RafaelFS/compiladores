/*
* sintatico.c
*
* PCS2056 - Linguagens e Compiladores
* Pedro Napolitano Sant’Ana 	8041817
* Rafael Freitas da Silva 	6480585
*
*/
#include <stdio.h>
#include <string.h>
#include "sintatico.h"

void compile(FILE *pointerToSourceFile) {

    InputStr str;
    TokenArr tokenArr;

    readFromFile(&str, pointerToSourceFile);

    // Syntax Analysis
    if (getTokens(&tokenArr, &str) == 1 && checkSyntax(&tokenArr) == 1) {
        printf("Lex and Syn OK\n");
    } else {
        printf("Lex or Syn failed\n");
    }
}

void readFromFile(InputStr *str, FILE *pointerToSourceFile) {
    char c;

    str->pos = 0;

    do {
        c = fgetc(pointerToSourceFile);
        str->value[str->pos] = c;
        str->pos++;
    } while (c != EOF);
}

int getTokens(TokenArr *tokenArr, InputStr *str) {
    tokenArr->size = 0;

    str->pos = 0;

    while (1) {
        Token *t = (&tokenArr->tokens[tokenArr->size]);

        int success = getNextToken(t, str);

        if (success) {
            tokenArr->size++;
            printTokenType(t->type);
            printf("\t %s \n", t->value);
        } else if(str->value[str->pos] == EOF){
            printf("Finished extracting %d tokens.\n", tokenArr->size);
            return 1;
        } else {
            printf("Invalid character'%c' at position %d.\n", str->value[str->pos], str->pos);
            return 0;
        }
    }
}

int checkSyntax(TokenArr *tokenArr) {
    tokenArr->pos = 0;
    return autProgram(tokenArr);
}

void todo(char str[100]) {
    printf("Matched automata [%s]\n", str);
}

int isInteger(char constant[MAX_TOKEN_SIZE]){
    char *point = strstr(constant, ".");
    if (point) {
        return 0;
    } else {
        return 1;
    }
}

int autProgram(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autFunctions(tokenArr)) {
                    state = 3;
                } else if (autMain(tokenArr)) {
                    state = 4;
                } else if (autTypes(tokenArr)) {
                    state = 1;
                } else if (autGlobal(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autFunctions(tokenArr)) {
                    state = 3;
                } else if (autMain(tokenArr)) {
                    state = 4;
                } else if (autGlobal(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (autFunctions(tokenArr)) {
                    state = 3;
                } else if (autMain(tokenArr)) {
                    state = 4;
                } else if (autTypes(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (autMain(tokenArr)) {
                    state = 4;
                } else if (autTypes(tokenArr)) {
                    state = 1;
                } else if (autGlobal(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                todo("Program");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autTypes(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "types") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else if (autTypeDef(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                todo("Types");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autFunctions(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "functions") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (autFuncDecl(tokenArr)) {
                    state = 2;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "implementation") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                if (autFuncImpl(tokenArr)) {
                    state = 5;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autMain(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "main") == 0) {
                    state = 1;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    state = 2;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (autCmd(tokenArr)) {
                    state = 2;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    state = 3;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                todo("Main");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autGlobal(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "global") == 0) {
                    state = 1;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    state = 2;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (autDecl(tokenArr)) {
                    state = 2;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    state = 3;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                todo("Global");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autTypeDef(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autCustomType(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autName(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                todo("TypeDef");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autName(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_IDENTIFIER) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("Name");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autCustomType(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autType(tokenArr) == 1 || autName(tokenArr) == 1) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("CustomType");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autType(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && (strcmp(t.value, "int") == 0 || strcmp(t.value, "float") == 0 || strcmp(t.value, "bool") == 0 || strcmp(t.value, "char") == 0)) {
                    tokenArr->pos++;
                    state = 1;
                } else if (autStruct(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("Type");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autStruct(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "struct") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else if (autTypeDef(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                todo("Struct");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autDecl(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autCustomType(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autAttr(tokenArr)) {
                    state = 3;
                }
                else if (autName(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "[") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
              todo("Decl");
              syntaxMatch = 1;
            break;

            case 4:
                if (autIntNum(tokenArr)) {
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "]") == 0) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 6:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autFuncDecl(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autCustomType(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autName(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;


            case 3:
                if (autTypeDef(tokenArr)) {
                    state = 3;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                todo("FuncDecl");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
  }

  if (syntaxMatch == 0) {
      tokenArr->pos = startingToken;
  }

  return syntaxMatch;
}

int autFuncImpl(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autCustomType(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autName(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;


            case 3:
                if (autTypeDef(tokenArr)) {
                    state = 3;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                if (autFuncCmdList(tokenArr)) {
                    state = 5;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 6;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 6:
                todo("FuncImpl");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autFuncCmdList(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autCmd(tokenArr)) {
                    state = 1;
                } else if (t.type == TYPE_KEYWORD && strcmp(t.value, "return") == 0) {
                    state = 2;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autCmd(tokenArr)) {
                    state = 1;
                } else if(t.type == TYPE_KEYWORD && strcmp(t.value, "return") == 0) {
                    state = 2;
                    tokenArr->pos++;
                } else {
                  todo("FuncCmdList");
                  syntaxMatch = 1;
                }
            break;

            case 2:
                if (autExpression(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    state = 1;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autCmd(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autDecl(tokenArr) || autAttr(tokenArr) || autIfCmd(tokenArr) || autItrCmd(tokenArr) || autInCmd(tokenArr) || autOutCmd(tokenArr)) {
                     state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("Cmd");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autExpression(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if(autAriExpr(tokenArr)){
                    state = 1;
                } else if(autBoolExpr(tokenArr)){
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("Expression");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
  }

  int autAttr(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if(autVar(tokenArr)){
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if(t.type == TYPE_PUNCTUATOR && strcmp(t.value, "=") == 0){
                    state = 2;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if(autExpression(tokenArr)){
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if(t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0){
                    state = 4;
                    tokenArr->pos++;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                todo("Attr");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autIfCmd(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "if") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (autBoolExpr(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 6;
                } else if (autCmd(tokenArr)) {
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 6:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "else") == 0) {
                    tokenArr->pos++;
                    state = 7;
                } else if (t.type == TYPE_KEYWORD && strcmp(t.value, "elsif") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 1;
                }
            break;

            case 7:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 8;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 8:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 9;
                } else if (autCmd(tokenArr)) {
                    state = 8;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 9:
                todo("IfCmd");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autItrCmd(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "while") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (autBoolExpr(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "{") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "}") == 0) {
                    tokenArr->pos++;
                    state = 6;
                } else if (autCmd(tokenArr)) {
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 6:
                todo("ItrCmd");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autInCmd(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "read") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (autVar(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                todo("InCmd");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autOutCmd(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_KEYWORD && strcmp(t.value, "print") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_STRING) {
                    tokenArr->pos++;
                    state = 3;
                } else if (autExpression(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                todo("OutCmd");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autIntNum(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_CONSTANT && isInteger(t.value)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("IntNum");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autAriExpr(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autAriTermA(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_OPERATOR && (strcmp(t.value, "+") == 0 || strcmp(t.value, "-") == 0)) {
                    tokenArr->pos++;
                    state = 0;
                } else {
                    todo("AriExpr");
                    syntaxMatch = 1;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autAriTermA(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autAriTermB(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_OPERATOR && (strcmp(t.value, "%%") == 0 || strcmp(t.value, "/") == 0 || strcmp(t.value, "*") == 0)) {
                    tokenArr->pos++;
                    state = 0;
                } else {
                    todo("AriTermA");
                    syntaxMatch = 1;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autAriTermB(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_OPERATOR && strcmp(t.value, "-") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else if (autAriFactor(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autAriFactor(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                todo("AriTermB");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;;
}

int autAriFactor(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else if (t.type == TYPE_CONSTANT) {
                    tokenArr->pos++;
                    state = 1;
                } else if (autVar(tokenArr) == 1 || autFuncCall(tokenArr) == 1) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("AriFactor");
                syntaxMatch = 1;
            break;

            case 2:
                if (autAriExpr(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autBoolExpr(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autBoolTermA(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_OPERATOR && strcmp(t.value, "||") == 0) {
                    tokenArr->pos++;
                    state = 0;
                } else {
                    todo("BoolExpr");
                    syntaxMatch = 1;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autBoolTermA(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autBoolTermB(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_OPERATOR && strcmp(t.value, "&&") == 0) {
                    tokenArr->pos++;
                    state = 0;
                } else {
                    todo("BoolTermA");
                    syntaxMatch = 1;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autBoolTermB(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_OPERATOR && strcmp(t.value, "!") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else if (autBoolTermC(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autBoolTermC(tokenArr)) {
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                todo("BoolTermB");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autBoolTermC(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autBoolFactor(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (autBoolComp(tokenArr)) {
                    state = 0;
                } else {
                    todo("BoolTermC");
                    syntaxMatch = 1;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autBoolFactor(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else if (t.type == TYPE_KEYWORD && (strcmp(t.value, "true") == 0 || strcmp(t.value, "false") == 0)) {
                    tokenArr->pos++;
                    state = 1;
                } else if (autFuncCall(tokenArr) == 1 || autVar(tokenArr) == 1) {
                    state = 1;
                } else if (autAriExpr(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("BoolFactor");
                syntaxMatch = 1;
            break;

            case 2:
                if (autBoolExpr(tokenArr)) {
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (autAriComp(tokenArr)) {
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (autAriExpr(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autAriComp(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_OPERATOR && (strcmp(t.value, ">") == 0 || strcmp(t.value, "<") == 0 || strcmp(t.value, ">=") == 0 || strcmp(t.value, "<=") == 0)) {
                    tokenArr->pos++;
                    state = 1;
                } else if (autBoolComp(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("AriComp");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autBoolComp(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (t.type == TYPE_OPERATOR && (strcmp(t.value, "==") == 0 || strcmp(t.value, "!=") == 0)) {
                    tokenArr->pos++;
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                todo("BoolComp");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autFuncCall(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autName(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "(") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 2:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else if (autParamList(tokenArr)) {
                    state = 3;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ")") == 0) {
                    tokenArr->pos++;
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ";") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                todo("FuncCall");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autParamList(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            case 0:
                if (autExpression(tokenArr)) {
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ",") == 0) {
                    tokenArr->pos++;
                    state = 0;
                } else {
                    todo("ParamList");
                    syntaxMatch = 1;
                }
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}

int autVar(TokenArr *tokenArr) {
    int state = 0;
    int syntaxMatch = -1;
    int startingToken = tokenArr->pos;
    Token t;
    while (syntaxMatch == -1) {
        t = tokenArr->tokens[tokenArr->pos];
        switch (state) {
            printf("%s\n", t.value );
            case 0:
                if(autName(tokenArr)){
                    state = 1;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 1:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "[") == 0) {
                    tokenArr->pos++;
                    state = 2;
                } else if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, ".") == 0) {
                    tokenArr->pos++;
                    state = 3;
                } else {
                    todo("Var");
                    syntaxMatch = 1;
                }
            break;

            case 2:
                if (autAriExpr(tokenArr)) {
                    state = 4;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 3:
                if (autName(tokenArr)) {
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 4:
                if (t.type == TYPE_PUNCTUATOR && strcmp(t.value, "]") == 0) {
                    tokenArr->pos++;
                    state = 5;
                } else {
                    syntaxMatch = 0;
                }
            break;

            case 5:
                todo("Var");
                syntaxMatch = 1;
            break;

            default:
                syntaxMatch = 0;
        }

        if (tokenArr->pos == tokenArr->size) {
            syntaxMatch = 0;
        }
    }

    if (syntaxMatch == 0) {
        tokenArr->pos = startingToken;
    }

    return syntaxMatch;
}
