#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_io.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

int main(int argc, char* argv[]) {
    char* filename = NULL;
    char* task = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[i + 1];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            task = argv[i + 1];
        }
    }

    if (filename == NULL || task == NULL) {
        fprintf(stderr, "Usage: ./translate -f <filename> -t <task>\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp = open_file(filename);

    if (strcmp(task, "task1") == 0) {
        task1(fp);
    } else if (strcmp(task, "task2") == 0) {
        task2(fp);
    } else if (strcmp(task, "task3") == 0) {
        task3(fp);
    } else if (strcmp(task, "task4") == 0) {
        task4(fp);
    } else {
        fprintf(stderr, "Invalid task\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    
    return 0;
}