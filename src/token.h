#pragma once

// Forward declaration
typedef struct TokenV TokenV;

typedef enum {
    NUMBER,
    FUNCTION,
    START,
    END,
} TokenTag;

typedef enum {
    ADD,
    SUB,
    SCOPE,
    SETX,
    SETY,
} Fn;

typedef union {
    float number;
    char *string;
    int active;
    TokenV *scope;  // Now TokenV is known
    Fn fn;
} TokenPayload;

typedef struct {
    TokenTag tag;
    TokenPayload val;
} Token;

struct TokenV {  // Define the struct here
    Token *list;
    int len;
    int cap;
};

// Function declarations
TokenV *create_TokenV(int cap); 
int push_TokenV(TokenV *u, Token x); 
void free_TokenV(TokenV *vec);
