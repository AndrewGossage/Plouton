#pragma once
#include "token.h"


int global_functions_init(); 


void global_functions_deinit(); 

extern TokenTree global_functions;
void setter_fn(float acc, Fn tag);
TokenTree *TokenTree_parse(char s[]);
float TokenTree_run(TokenTree *tokens);

