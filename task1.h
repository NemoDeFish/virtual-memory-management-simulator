#ifndef TASK1_H
#define TASK1_H

#include <stdio.h>

// Extracts the page number and offset for each logical address in the input
// file using file pointer.
void task1(FILE* fp);

// Extracts the offset (lowest 12 bits) from a logical address.
unsigned int task1_poffset(unsigned int laddress);

// Extracts the page number (next 10 bits after offset) from a logical address.
unsigned int task1_pnumber(unsigned int laddress);

// Prints an execution transcript for task 1.
void task1_print(unsigned int laddress, unsigned int pnumber,
                 unsigned int poffset);

#endif