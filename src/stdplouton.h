#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "token.h"


float add(float a, float b) {
    return a + b;
}
float subtract(float a, float b) {

    return a - b;
}
float multiply(float a, float b) {
    return a * b;
}
float divide(float a, float b) {
    return a / b;
}

float assert_eq (float a, float b) {

    if (a != b ){
        exit(1);

    }
    return 0;
}
void print_token(Token t) {
    switch (t.tag) {
    case NUMBER:
        break;
    case FUNCTION:
        printf(" - function #%d\n ", (int)t.id);
        break;
    case START:
        printf(" - start \n ");
        break;
    case END:
        printf(" - end \n ");
        break;
    case SCOPE:
        printf(" - scope \n ");
        break;
    default:
        printf(" - unknown tag \n");

        break;
    }
}



float print_cmd(TokenTree *tokens, TokenTree *scope) {
    for (int k = 1; k < tokens->len; k++) {
        switch (tokens->list[k].tag) {
        case SCOPE:
            printf("%0.2f ",
                   add(0, TokenTree_run(tokens->list[k].val.scope,
                                        scope)));
            break;
        case NUMBER:
            printf("%0.2f ", tokens->list[k].val.number);
        case STRING: 
            printf("%s ", tokens->list[k].val.string.ptr);
            break;
        default:
            print_token(tokens->list[k]);
            break;
        }
    }
    printf("\n");

    return 0;

}

typedef float (*operation_func)(float, float);
operation_func get_operation(Fn f) {
    switch (f) {
    case DIV:
        return divide;
    case MUL:
        return multiply;
    case SUB:
        return subtract;
    case ASSERT_EQ:
        return assert_eq;
    default:
        return add;
    }
}

