#pragma once

// Forward declaration
typedef struct TokenTree TokenTree;

typedef enum {
    NUMBER,
    FUNCTION,
    STRING,
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
    PRINT,
    CON,
    USER_FUNCTION,

} Fn;

typedef struct {

    char *ptr;
    int len;

} Slice;

typedef union {
    float number;
    Slice string;
    int active;
    TokenTree *scope;  
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
