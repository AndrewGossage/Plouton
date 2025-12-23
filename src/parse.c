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

char *find_str_end(char *s) {
    char *end = s;
    int depth = 1;
    while (end[0] != '\n') {
        if (depth <= 0) {
            return end;
        }
        if (end[0] == '`') {
            depth--;
        }
        end++;
    }
    return end;
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
    TokenTree *tokens = TokenTree_new(100);
    char *rest = s;
    const int l = strlen(s);
    for (int i = 0; i < l; i++) {
        if (s[i] == '\n' || s[i] == '\t') {
            s[i] = ' ';
        }
    }
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

            token = strtok(end, " \n");
            continue;
        } else if (token[0] == '`') {
            Token x;
            char *end = find_str_end(rest);
            char *subs = substring(rest, end - 1);

            size_t len = strlen(token + 1) + 1 + strlen(subs) + 1;
            char *string = malloc(len);
            sprintf(string, "%s %s", token + 1, subs);

            free(subs);

            x.tag = STRING;
            x.val.string.ptr = string;
            x.val.string.len = strlen(string);
            TokenTree_push(tokens, x);
            token = strtok(end, " \n");
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
                token = strtok(NULL, " \n");
                const unsigned long id = hash((unsigned char *)token);
                token = strtok(NULL, " \n");
                Token x;
                rest = token + strlen(token) + 1;
                char *end = find_end(rest);
                char *subs = substring(rest, end);
                x.tag = DEFINITION;
                x.val.scope = TokenTree_parse(subs);
                x.id = id;
                TokenTree_push(&global_functions, x);
                free(subs);
                token = strtok(end, " \n");
                continue;
            } else if (strncmp(token, "?", 1) == 0) {
                x.val.fn = CON;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "print", 5) == 0) {
                x.val.fn = PRINT;
                TokenTree_push(tokens, x);

            } else if (token[0] == '!') {
                char *sub_token = token + 1;
                x.tag = FUNCTION;
                x.val.fn = USER_FUNCTION;

                x.id = hash((unsigned char *)sub_token);

                TokenTree_push(tokens, x);
            } else if (token[0] == '$') {
                char *sub_token = token + 1;
                x.tag = FUNCTION;
                x.val.fn = GET;
                x.id = strtol(sub_token, NULL, 10);

                TokenTree_push(tokens, x);
            } else if (strcmp(token, "A_EQ") == 0) {
                x.val.fn = ASSERT_EQ;
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
            } else if (strncmp(token, "<", 1) == 0) {
                x.val.fn = LT;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, ">", 1) == 0) {
                x.val.fn = GT;
                TokenTree_push(tokens, x);
            } else if (strncmp(token, "=", 1) == 0) {
                x.val.fn = EQ;
                TokenTree_push(tokens, x);
            }
        }
        token = strtok(NULL, " \n");
    }
    return tokens;
}
