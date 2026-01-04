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

float eq (float a, float b) {
    if (a == b ){
        return 1;

    }
    return 0;
}

float lt (float a, float b) {
    if (a < b ){
        return 1;

    }
    return 0;
}

float gt (float a, float b) {
    return !lt(a,b);
}





float assert_eq (float a, float b) {
    printf("ASSERT = %0.2f, %0.2f \n", a, b);

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
        Token t = tokens->list[k];
        if (t.tag == FUNCTION && t.val.fn == GET) {

            t = scope->list[t.id + 1];
        }

        switch (t.tag) {
        case SCOPE:
            printf("%0.2f ",
                   add(0, TokenTree_run(t.val.scope,
                                        scope)));
            break;
        case NUMBER:
            printf("%0.2f ", t.val.number);
            break;
        case STRING: 
            printf("%s ", t.val.string.ptr);
            break;
        default:
            print_token(t);
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
    case EQ:
        return eq;
    case LT:
        return lt;
    case GT:
        return gt;
    default:
        return add;
    }
}

