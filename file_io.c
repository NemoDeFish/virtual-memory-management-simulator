#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

// Opens a file to read and returns a file pointer.
FILE* open_file(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}

// Closes a file.
void close_file(FILE* fp) {
    if (fp != NULL) {
        fclose(fp);
    }
}

// Reads a logical address from the file.
int read_line(FILE* fp) {
    unsigned int laddress;

    // If file pointer still not at EOF
    if (fscanf(fp, "%d", &laddress) != EOF) {
        return laddress;
    }

    // If file pointer reached EOF
    return INVALID;
}