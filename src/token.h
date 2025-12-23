#pragma once

// Forward declaration
typedef struct TokenTree TokenTree;

typedef enum {
    NUMBER,
    FUNCTION,
    START,
    END,
    SCOPE,
    DEFINITION,
} TokenTag;

typedef enum {
    ADD,
    SUB,
    ASSERT_EQ,
    DIV,
    MUL,
    GET,
    CON,
    USER_FUNCTION,

} Fn;

typedef union {
    float number;
    char *string;
    int active;
    TokenTree *scope;  // Now TokenTree is known
    Fn fn;
} TokenPayload;

typedef struct {
    TokenTag tag;
    TokenPayload val;
    unsigned long id;
} Token;

struct TokenTree {  // Define the struct here
    Token *list;
    int len;
    int cap;
};

// Function declarations
TokenTree *TokenTree_new(int cap); 

int TokenTree_push(TokenTree *u, Token x); 
void TokenTree_free(TokenTree *vec);
