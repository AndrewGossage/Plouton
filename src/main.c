#include "run.h"
#include <stdio.h>
#include <string.h>

int main() {
    char s[100];
    printf("Enter a command\n");
    while (1) {
        printf("\n:");
        fgets(s, sizeof(s), stdin);
        if (strncmp(s, "quit", 4) == 0) {
            return 0;
        }
        run_str(s);
    }
}
