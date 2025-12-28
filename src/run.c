#include "run.h"
#include "parse.h"
#include "stdplouton.h"
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
int check_debug() {
    char *val = getenv("DEBUG_TREE");

    if (val != NULL && val[0] != '\0') {
        return 1;
    }
    return 0;
}

float TokenTree_run_con(TokenTree *tokens, TokenTree *scope) {

    printf("val: 1\n");

    Token i = scope->list[1];
    printf("val: 2\n");

    Token x = scope->list[2];
    printf("val: 3\n");

    Token y = scope->list[3];
    printf("val: about to run\n");
    float val = TokenTree_run(i.val.scope, scope);
    printf("val: %0.2f\n", val);
    if (val != 0) {
        return TokenTree_run(x.val.scope, scope);
    }
    return TokenTree_run(y.val.scope, scope);
}

float TokenTree_run(TokenTree *tokens, TokenTree *scope) {
    float acc = 0;

    if (tokens->len == 0) {
        return 0;
    }
    Token first = tokens->list[0];
    operation_func op;
    for (int i = 0; i < tokens->len; i++) {
        Token t = tokens->list[i];
        if (first.tag == FUNCTION) {
            if (first.val.fn == CON) {
                int k = 1;
                float v =
                    add(0, TokenTree_run(tokens->list[k].val.scope, scope));
                if (v == 0) {
                    return TokenTree_run(tokens->list[3].val.scope, scope);
                } else {
                    return TokenTree_run(tokens->list[2].val.scope, scope);
                }
            } else if (first.val.fn == PRINT) {
                return print_cmd(tokens, scope);

            } else {
                op = get_operation(first.val.fn);
            }
        } else {
            op = add;
        }

        if (t.tag == FUNCTION && t.val.fn == GET) {
            t = scope->list[t.id + 1];
        }

        if (i == 1) {
            // Handle first operand - could be NUMBER, SCOPE, GET, or user
            // function

            // Now evaluate whatever t is
            if (t.tag == NUMBER) {
                acc = t.val.number;
            } else if (t.tag == SCOPE) {

                acc = TokenTree_run(t.val.scope, scope);
            }
            // ... other cases
            continue;
        }
        if (t.tag == FUNCTION && t.val.fn == GET) {
            if (scope) {
                for (int k = 0; k < scope->len; k++) {
                }
            }
            t = scope->list[t.id];
        }

        else if (t.tag == SCOPE) {
            acc = op(acc, TokenTree_run(t.val.scope, scope));
        } else if (t.tag == NUMBER) {
            acc = op(acc, t.val.number);
        } else if (t.tag == FUNCTION && t.id != 0) {
            Token *fns = global_functions.list;
            for (int k = 0; k < tokens->len; k++) {
                if (tokens->list[k].tag == NUMBER) {
                }
            }
            for (int j = 0; j < global_functions.len; j++) {
                Token x = fns[j];
                TokenTree *new_scope = TokenTree_new(tokens->len);

                if (x.id == t.id) {
                    for (int kk = 1; kk < tokens->len; kk++) {
                        Token y = tokens->list[kk];

                        if (y.tag == SCOPE || y.tag == FUNCTION) {

                            // Evaluate the scope argument in caller's
                            // context
                            Token evaluated;
                            evaluated.tag = NUMBER;
                            evaluated.val.number =
                                TokenTree_run(y.val.scope, scope);

                            new_scope->list[kk] = evaluated;
                        } else {
                            new_scope->list[kk] = y;
                        }
                    }
                    new_scope->len = tokens->len;
                    return op(acc, TokenTree_run(x.val.scope, new_scope));
                    break;
                }
            }
        }
    }

    return acc;
}
