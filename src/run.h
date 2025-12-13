#pragma once
#include "token.h"

void setter_fn(float acc, Fn tag);

TokenV *run_str(char s[]);
float run_tokens(TokenV *tokens);

