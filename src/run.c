#include "run.h"
#include "parse.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_LIMIT 10

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
void global_functions_deinit() { free(global_functions.list); }

void print_token(Token t) {
    switch (t.tag) {
    case NUMBER:
        break;
    case FUNCTION:
        printf(" - function #%d\n ", (int)t.id);
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
            acc -= TokenTree_run(t.val.scope, NULL);
        } else if (t.tag == NUMBER) {
            acc -= t.val.number;
        }
    }
    return acc;
}

float add(float a, float b) {
    printf("add(%f, %f) = %f\n", a, b, a + b);
    return a + b;
}
float subtract(float a, float b) {
    printf("subtract(%f, %f) = %f\n", a, b, a - b);
    return a - b;
}
float multiply(float a, float b) {
    printf("multiply(%f, %f) = %f\n", a, b, a * b);
    return a * b;
}
float divide(float a, float b) {
    printf("divide(%f, %f) = %f\n", a, b, a / b);
    return a / b;
}

typedef float (*operation_func)(float, float);
operation_func get_operation(Fn f) {
    switch (f) {
    case DIV:
        return divide;
    case MUL:
        return multiply;
    case SUB:
        return subtract;
    default:
        return add;
    }
}
float TokenTree_run(TokenTree *tokens, TokenTree *scope) {
    float acc = 0;
    if (tokens->len == 0) {
        return 0;
    }
    if (check_debug()) {
        for (int i = 0; i < tokens->len; i++) {
            print_token(tokens->list[i]);
        }
    }
    Token first = tokens->list[0];
    operation_func op;
    if (first.tag == FUNCTION) {
        op = get_operation(first.val.fn);
    } else {
        op = add;
    }
    for (int i = 0; i < tokens->len; i++) {
        Token t = tokens->list[i];
        if (i == 1) {
            // Handle first operand - could be NUMBER, SCOPE, GET, or user
            // function
            if (t.tag == FUNCTION && t.val.fn == GET) {
                t = scope->list[t.id + 1];
            }

            // Now evaluate whatever t is
            if (t.tag == NUMBER) {
                acc = t.val.number;
            } else if (t.tag == SCOPE) {
                acc = TokenTree_run(t.val.scope, t.val.scope);
            }
            // ... other cases
            continue;
        }
        if (t.tag == FUNCTION && t.val.fn == GET) {
            if (scope) {
                for (int k = 0; k < scope->len; k++) {
                    printf("  scope[%d]: tag=%d\n", k, scope->list[k].tag);
                }
            }
            t = scope->list[t.id];
        }
        if (t.tag == SCOPE) {
            acc = op(acc, TokenTree_run(t.val.scope, t.val.scope));
        } else if (t.tag == NUMBER) {
            acc = op(acc, t.val.number);
        } else if (t.tag == FUNCTION && t.id != 0) {
            Token *fns = global_functions.list;
            printf("Calling user function, tokens->len=%d\n", tokens->len);
            for (int k = 0; k < tokens->len; k++) {
                printf("  tokens[%d]: tag=%d\n", k, tokens->list[k].tag);
                if (tokens->list[k].tag == NUMBER) {
                    printf("    value=%f\n", tokens->list[k].val.number);
                }
            }
            for (int j = 0; j < global_functions.len; j++) {
                Token x = fns[j];
                if (x.id == t.id) {
                    return op(acc, TokenTree_run(x.val.scope, tokens));
                    break;
                }
            }
        }
    }
    printf("->%0.2f\n", acc);

    return acc;
}
