
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

float run_tokens(TokenV *tokens) {
    float acc = 0;
    if (tokens->list[0].val.fn == ADD) {
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
    }
    return acc;
}

TokenV *run_str(char s[]) {
    TokenV *tokens = create_TokenV(10);
    char *rest = s;
    char *token = strtok(s, " "); // Remove the duplicate call
    while (token != NULL) {
        rest = token + strlen(token) + 1;
        char *end_p;
        float result = strtof(token, &end_p);

        // Check if conversion was successful (end_p != token means some digits
        // were parsed)
        if (end_p != token) {
            Token x;
            x.tag = NUMBER;
            x.val.number = result;
            push_TokenV(tokens, x);
        } else {
            // It's not a number, could be a FUNCTION token
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
            } else if (strncmp(token, ";", 1) == 0) {
                x.tag = NUMBER;
                x.val.number = last_acc;
                push_TokenV(tokens, x);
            }
        }

        token = strtok(NULL, " ");
    }
    float acc = run_tokens(tokens);

    printf("->%f\n", acc);
    free_TokenV(tokens);
    last_acc = acc;
    return tokens;
}
