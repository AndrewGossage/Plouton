#include "parse.h"
#include "run.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_file(char *filename) {
    FILE *fp;
    char *buffer;
    long file_size;
    size_t bytes_read;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(fp);
        return 1;
    }

    file_size = ftell(fp);
    if (file_size == -1) {
        perror("ftell");
        fclose(fp);
        return 1;
    }

    rewind(fp);

    buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        perror("malloc");
        fclose(fp);
        return 1;
    }

    bytes_read = fread(buffer, 1, file_size, fp);
    if (bytes_read != (size_t)file_size) {
        perror("fread");
        free(buffer);
        fclose(fp);
        return 1;
    }

    buffer[file_size] = '\0';

    /* do something with buffer */
    TokenTree *t = TokenTree_parse(buffer);
    if (t) {
        float res = TokenTree_run(t, NULL);
        printf("->%0.2f\n", res);

        TokenTree_free(t);
    } else {
        printf("Could not parse file\n");
    }

    free(buffer);
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    char s[100];

    if (global_functions_init() != 0) {
        printf("Was not able to init global scope\n");
        return 1;
    }
    if (argc > 1) {
        printf("Running from file \n");
        return run_file(argv[1]);
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
            float res = TokenTree_run(t, NULL);
            printf("->%0.2f\n", res);
            TokenTree_free(t);
        }
    }
}
