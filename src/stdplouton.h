#pragma once
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

typedef float (*operation_func)(float, float);
operation_func get_operation(Fn f) {
    switch (f) {
    case DIV:
        return divide;
    case MUL:
        return multiply;
    case SUB:
        return subtract;
    default:
        return add;
    }
}

