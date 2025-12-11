#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NUMBER,
    FUNCTION,
    START,
    END,
} TokenTag;

typedef enum {
    ADD,
    SUB,
    SETX,
    SETY,

} Fn;

typedef union {
    float number;
    char *string;
    int active;
    Fn fn;
} TokenPayload;

typedef struct {
    TokenTag tag;
    TokenPayload val;
} Token;

typedef struct {
    Token *list;
    int len;
    int cap;
} TokenV;

TokenV *create_TokenV(int cap) {
    TokenV *vec = malloc(sizeof(TokenV));
    if (vec == NULL) {
        return NULL;
    }
    vec->list = malloc(cap * sizeof(Token));
    if (vec->list == NULL) {
        free(vec);
        return NULL;
    }
    vec->len = 0;
    vec->cap = cap;
    return vec;
}

int push_TokenV(TokenV *u, Token x) {
    if (u->len >= u->cap) {
        // todo actually handle resize
        return 1;
    }
    const int i = u->len; // Use current len as index
    u->list[i].tag = x.tag;
    u->list[i].val = x.val;
    u->len += 1; // Increment after using as index
    return 0;
}

void free_TokenV(TokenV *vec) {
    if (vec != NULL) {
        free(vec->list);
        free(vec);
    }
}
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

int run_str(char s[]) {
    TokenV *tokens = create_TokenV(10);

    char *token = strtok(s, " "); // Remove the duplicate call
    while (token != NULL) {
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
            }
        }

        token = strtok(NULL, " ");
    }

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

    printf("->%f\n", acc);

    free_TokenV(tokens);
    last_acc = acc;
    return acc;
}

int main() {
    char s[100];
    printf("Enter a command\n");
    while (1) {
        printf("\n:");
        fgets(s, sizeof(s), stdin);
        if (strncmp(s, "quit", 4) == 0) {
            return 0;
        }
        run_str(s);
    }
}
