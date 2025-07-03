#ifndef TASK2_H
#define TASK2_H

#include <stdio.h>

// Implement system's page table and translate logical addresses to physical
// addresses for each logical address in the input file using file pointer.
void task2(FILE* fp);

// Use page number to identify relevant page table entry
unsigned int task2_entry(unsigned int* page_table, unsigned int pnumber);

// Extract the rightmost present bit from the page table entry
unsigned int task2_present_bit(unsigned int entry);

// Allocate page to free frame in increasing frame number.
unsigned int task2_allocate_frame(unsigned int* physical_mem);

// Update corresponding page table entry by shifting the frame number to the
// left by 1 and append the present bit at the end.
unsigned int task2_update_entry(unsigned int fnumber);

// Extract the leftmost frame number bits from the page table entry starting
// after the present bit.
unsigned int task2_fnumber(unsigned int entry);

// Use the information in the page table entry and the offset to
// construct the corresponding physical address.
unsigned int task2_paddress(unsigned int fnumber, unsigned int poffset);

// Prints an execution transcript for task 2.
void task2_print(unsigned int pnumber, unsigned int pfault,
                 unsigned int fnumber, unsigned int paddress);

#endif