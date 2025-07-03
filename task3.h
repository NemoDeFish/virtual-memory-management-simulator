#ifndef TASK3_H
#define TASK3_H

#include <stdio.h>

#include "linked_list.h"

// Similar to Task 2, implement the page table and allocate frames to physical
// memory. However in Task 3, all frames might be allocated when a page fault
// occurs. In such a case, evict a page from physical memory by following the
// FIFO page replacement algorithm.
void task3(FILE* fp);

// Frees up a frame according to the FIFO algorithm.
unsigned int task3_free_frame(Node** fifo_queue_head, Node** fifo_queue_tail);

// Allocates the referenced page to the free frame.
unsigned int task3_allocate_page(unsigned int fframe, unsigned int page_table[],
                                 unsigned int physical_mem[]);

#endif