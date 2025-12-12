
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
float run_tokens(TokenV *tokens) {
    float acc = 0;
    if (tokens->list[0].tag != FUNCTION) {
        for (int i = 0; i < tokens->len; i++) {
            print_token(tokens->list[i]);
        }
    } else if (tokens->list[0].val.fn == ADD) {
        for (int i = 0; i < tokens->len; i++) {
            if (tokens->list[i].tag == NUMBER) {
                acc += tokens->list[i].val.number;
            } else if (tokens->list[i].tag == FUNCTION) {
                setter_fn(acc, tokens->list[i].val.fn);
            }
        }
    } else if (tokens->list[0].val.fn == SUB) {
        acc = tokens->list[1].val.number;
        for (int i = 2; i < tokens->len; i++) {
            if (tokens->list[i].tag == NUMBER) {
                acc -= tokens->list[i].val.number;
            } else if (tokens->list[i].tag == FUNCTION) {
                setter_fn(acc, tokens->list[i].val.fn);
            }
        }
    } else {
    }

    printf("->%0.2f", acc);
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
    while (end[0] != ')' && end[0] != '\n') {
        end++;
    }
    return end;
}
TokenV *run_str(char s[]) {
    TokenV *tokens = create_TokenV(10);
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
            push_TokenV(tokens, x);
        } else if (strncmp(token, "(", 1) == 0) {
            Token x;
            char *end = find_end(rest);
            char *subs = substring(rest, end);
            x.tag = SCOPE;
            x.val.scope = run_str(subs);
        } else {
            // Handle other tokens (FUNCTION, variables, etc.)
            Token x;
            x.tag = FUNCTION;
            if (strncmp(token, "add", 3) == 0) {
                x.val.fn = ADD;
                push_TokenV(tokens, x);
            } else if (strncmp(token, "sub", 3) == 0) {
                x.val.fn = SUB;
                push_TokenV(tokens, x);
            } else if (strncmp(token, "|x", 2) == 0) {
                x.val.fn = SETX;
                push_TokenV(tokens, x);
            } else if (strncmp(token, "|y", 2) == 0) {
                x.val.fn = SETY;
                push_TokenV(tokens, x);
            } else if (strncmp(token, "$x", 2) == 0) {
                x.tag = NUMBER;
                x.val.number = X;
                push_TokenV(tokens, x);
            } else if (strncmp(token, "$y", 2) == 0) {
                x.tag = NUMBER;
                x.val.number = Y;
                push_TokenV(tokens, x);
            } else if (strncmp(token, "$", 1) == 0) {
                x.tag = NUMBER;
                x.val.number = last_acc;
                push_TokenV(tokens, x);
            }
        }
        token = strtok(NULL, " ");
    }
    run_tokens(tokens);
    return tokens;
}
