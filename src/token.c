#include "token.h"
#include <stdlib.h>
#include <string.h>

TokenTree *TokenTree_new(int cap) {
    TokenTree *vec = malloc(sizeof(TokenTree));
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

int TokenTree_push(TokenTree *u, Token x) {
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

void TokenTree_free(TokenTree *vec) {
    for (int i = 0; i < vec->len; i++) {
        Token t = vec->list[i];

        if (t.tag == SCOPE) {
            TokenTree_free(t.val.scope);
        }
    }
    if (vec != NULL) {
        free(vec->list);
        free(vec);
    }
}
