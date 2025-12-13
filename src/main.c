#include "run.h"
#include "token.h"
#include <stdio.h>
#include <string.h>

int main() {
    char s[100];

    if (global_functions_init() != 0) {
        printf("Was not able to init global scope\n");
        return 1;
    }

    printf("Enter a command\n");

    while (1) {
        printf("\n:");
        fgets(s, sizeof(s), stdin);
        if (strncmp(s, "quit", 4) == 0) {
            global_functions_deinit();
            return 0;
        }
        TokenTree *t = TokenTree_parse(s);
        if (t) {
            TokenTree_run(t);
            TokenTree_free(t);
        }
    }
}
