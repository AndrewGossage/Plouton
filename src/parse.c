#include "run.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
float last_acc = 0;
float X = 1;
float Y = 2;

// djb2 by dan bernstein
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
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
    char *token = strtok(s, " \n");

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

            token = strtok(end, " ");
            continue;

        } else {
            // Handle other tokens (FUNCTION, variables, etc.)
            Token x;
            x.id = 0;
            x.tag = FUNCTION;
            if (strncmp(token, "/*", 2) == 0) {
                return tokens;
            }
            if (strncmp(token, "fn", 2) == 0) {
                token = strtok(NULL, " ");
                const unsigned long id = hash((unsigned char *)token);
                token = strtok(NULL, " ");
                Token x;
                rest = token + strlen(token) + 1;
                char *end = find_end(rest);
                char *subs = substring(rest, end);
                x.tag = DEFINITION;
                x.val.scope = TokenTree_parse(subs);
                x.id = id;
                TokenTree_push(&global_functions, x);
                free(subs);
                token = strtok(end, " ");
                continue;
            } else if (token[0] == '!') {
                char *sub_token = token + 1;
                x.tag = FUNCTION;
                x.val.fn = USER_FUNCTION;
                printf("TOKEN::%s:: REST::%s::", token, rest);

                x.id = hash((unsigned char *)sub_token);

                TokenTree_push(tokens, x);
            } else if (token[0] == '$') {
                char *sub_token = token + 1;
                x.tag = FUNCTION;
                x.val.fn = GET;
                x.id = strtol(sub_token, NULL, 10);

                TokenTree_push(tokens, x);

            } else if (strncmp(token, "+", 1) == 0) {
                x.val.fn = ADD;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "-", 1) == 0) {
                x.val.fn = SUB;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "/", 1) == 0) {
                x.val.fn = DIV;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "*", 1) == 0) {
                x.val.fn = MUL;
                TokenTree_push(tokens, x);
            }
        }
        token = strtok(NULL, " ");
    }
    return tokens;
}
