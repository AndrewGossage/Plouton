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

