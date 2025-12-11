#include "token.h"
#include <stdlib.h>
#include <string.h>

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
