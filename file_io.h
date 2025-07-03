#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>

// Opens a file to read and returns a file pointer.
FILE* open_file(char* filename);

// Closes a file.
void close_file(FILE* fp);

// Reads a logical address from the file.
int read_line(FILE* fp);

#endif