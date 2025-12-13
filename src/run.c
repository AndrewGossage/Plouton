
#include "run.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_LIMIT 10

float last_acc = 0;
float X = 1;
float Y = 2;
void setter_fn(float acc, Fn tag) {
    if (tag == SETX) {
        X = acc;
    } else if (tag == SETY) {
        Y = acc;
    }
}

TokenTree global_functions;
int global_functions_init() {
    global_functions.cap = F_LIMIT;
    global_functions.len = 0;
    global_functions.list = malloc(F_LIMIT * sizeof(Token));
    if (!global_functions.list) {
        fprintf(stderr, "allocation failed\n");
        return 1;
    }
    return 0;
}

// djb2 by dan bernstein
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void global_functions_deinit() { free(global_functions.list); }

void print_token(Token t) {
    switch (t.tag) {
    case NUMBER:
        printf(" - number \n ");
        break;
    case FUNCTION:
        printf(" - function \n ");
        break;
    case START:
        printf(" - start \n ");
        break;
    case END:
        printf(" - end \n ");
        break;
    case SCOPE:
        printf(" - scope \n ");
        break;
    default:
        printf(" - unknown tag \n");

        break;
    }
}
int check_debug() {
    char *val = getenv("DEBUG_TREE");

    if (val != NULL && val[0] != '\0') {
        return 1;
    }
    return 0;
}

float run_sub(TokenTree *tokens) {
    float acc = tokens->list[1].val.number;
    for (int i = 2; i < tokens->len; i++) {
        Token t = tokens->list[i];
        if (t.tag == SCOPE) {
            acc -= TokenTree_run(t.val.scope);
        } else if (t.tag == NUMBER) {
            acc -= t.val.number;
        } else if (t.tag == FUNCTION) {
            setter_fn(acc, tokens->list[i].val.fn);
        }
    }
    return acc;
}

float run_add(TokenTree *tokens) {
    float acc = tokens->list[1].val.number;
    for (int i = 2; i < tokens->len; i++) {
        Token t = tokens->list[i];
        if (t.tag == SCOPE) {
            acc += TokenTree_run(t.val.scope);
        } else if (t.tag == NUMBER) {
            acc += t.val.number;
        } else if (t.tag == FUNCTION) {
            setter_fn(acc, tokens->list[i].val.fn);
        }
    }
    return acc;
}

float TokenTree_run(TokenTree *tokens) {
    float acc = 0;
    if (tokens->len == 0) {
        return 0;
    }

    if (check_debug()) {

        if (tokens->list[0].tag != FUNCTION) {
            for (int i = 0; i < tokens->len; i++) {
                print_token(tokens->list[i]);
            }
        }
    }
    Token first = tokens->list[0];
    if ((first.tag == FUNCTION && tokens->list[0].val.fn == ADD) ||
        first.tag == SCOPE) {
        for (int i = 0; i < tokens->len; i++) {
            Token t = tokens->list[i];

            if (t.tag == SCOPE) {
                acc += TokenTree_run(t.val.scope);
            } else if (t.tag == NUMBER) {
                acc += t.val.number;
            } else if (t.tag == FUNCTION) {
                setter_fn(acc, tokens->list[i].val.fn);
            }
        }
    } else if (first.tag == FUNCTION && tokens->list[0].val.fn == SUB) {
        acc = run_sub(tokens);
    }
    printf("->%0.2f\n", acc);
    return acc;
}

char *substring(const char *start, const char *end) {
    // Calculate the length
    size_t len = end - start;

    // Allocate memory for the substring (+1 for null terminator)
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy the characters
    memcpy(result, start, len);

    // Add null terminator
    result[len] = '\0';

    return result;
}

char *find_end(char *s) {
    char *end = s;
    int depth = 1;
    while (end[0] != '\n') {
        if (depth <= 0) {
            return end;
        }
        if (end[0] == ')') {
            depth--;
        } else if (end[0] == '(') {
            depth++;
        }
        end++;
    }
    return end;
}
TokenTree *TokenTree_parse(char s[]) {
    TokenTree *tokens = TokenTree_new(10);
    char *rest = s;
    char *token = strtok(s, " ");

    while (token != NULL) {
        rest = token + strlen(token) + 1;
        char *end_p;
        float result = strtof(token, &end_p);

        if (end_p != token) {
            // It's a number
            Token x;
            x.tag = NUMBER;
            x.val.number = result;
            TokenTree_push(tokens, x);
        } else if (strncmp(token, "(", 1) == 0) {
            Token x;

            char *end = find_end(rest);
            char *subs = substring(rest, end);

            x.tag = SCOPE;

            x.val.scope = TokenTree_parse(subs);
            TokenTree_push(tokens, x);

            free(subs);

            strtok(end, " ");

        } else {
            // Handle other tokens (FUNCTION, variables, etc.)
            Token x;
            x.id = 0;
            x.tag = FUNCTION;
            if (strncmp(token, "fn", 2) == 0) {
                token = strtok(NULL, " ");
                printf("hash: %d\n", (int)hash((unsigned char *)token));
            } else if (strncmp(token, "add", 3) == 0) {
                x.val.fn = ADD;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "sub", 3) == 0) {
                x.val.fn = SUB;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "|x", 2) == 0) {
                x.val.fn = SETX;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "|y", 2) == 0) {
                x.val.fn = SETY;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "$x", 2) == 0) {
                x.tag = NUMBER;
                x.val.number = X;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "$y", 2) == 0) {
                x.tag = NUMBER;
                x.val.number = Y;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "$", 1) == 0) {
                x.tag = NUMBER;
                x.val.number = last_acc;
                TokenTree_push(tokens, x);
            }
        }
        token = strtok(NULL, " ");
    }
    return tokens;
}
