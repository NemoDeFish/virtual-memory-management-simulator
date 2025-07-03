#include "bit_utils.h"
#include "constants.h"
#include "file_io.h"
#include "linked_list.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

// Similar to Task 2, implement the page table and allocate frames to physical
// memory. However in Task 3, all frames might be allocated when a page fault
// occurs. In such a case, evict a page from physical memory by following the
// FIFO page replacement algorithm.
void task3(FILE* fp) {
    Node *fifo_queue_head = NULL, *fifo_queue_tail = NULL;
    unsigned int physical_mem[NUM_FRAMES] = {0};
    unsigned int page_table[NUM_PAGES] = {0};
    unsigned int laddress, pfault, fnumber, paddress, pnumber, poffset, entry,
        present_bit;

    // Read each logical address from input files
    while ((laddress = read_line(fp)) != INVALID) {
        // Extract page number and offset from the logical address
        pnumber = task1_pnumber(laddress);
        poffset = task1_poffset(laddress);
        task1_print(laddress, pnumber, poffset);

        // Use page number to identify relevant page table entry
        entry = task2_entry(page_table, pnumber);
        present_bit = task2_present_bit(entry);

        // In case of page fault
        if (present_bit == 0) {
            pfault = 1;

            // If there are free frames, allocate page to free frame in
            // increasing frame number
            fnumber = task2_allocate_frame(physical_mem);

            // If there are no free frames
            if (fnumber == INVALID) {
                fnumber = task3_free_frame(&fifo_queue_head, &fifo_queue_tail);
                task3_allocate_page(fnumber, page_table, physical_mem);
            }

            // Update FIFO queue
            add_tail(&fifo_queue_head, &fifo_queue_tail, fnumber);

            // Update corresponding page table entry
            page_table[pnumber] = task2_update_entry(fnumber);
        } else {
            // In case where page is in physical memory
            pfault = 0;

            // Use page number to identify relevant page table entry
            fnumber = task2_fnumber(entry);
        }

        // Use the information in the page table entry and the offset to
        // construct the corresponding physical address
        paddress = task2_paddress(fnumber, poffset);

        // Prints an execution transcript for task 2.
        task2_print(pnumber, pfault, fnumber, paddress);
    }

    freeList(fifo_queue_head);
}

// Frees up a frame according to the FIFO algorithm.
unsigned int task3_free_frame(Node** fifo_queue_head, Node** fifo_queue_tail) {
    // Free up frame according to FIFO algorithm
    return remove_head(fifo_queue_head, fifo_queue_tail);
}

// Find the page to evict corresponding to freed frame from page table.
unsigned int task3_epage(unsigned int page_table[], unsigned int fframe) {
    for (int i = 0; i < NUM_PAGES; i++) {
        // Since the page table was initialized to 0, we need to check the
        // present bit to ensure that the frame is in memory before we check the
        // actual frame number
        if (task2_present_bit(page_table[i]) == PRESENT_BIT &&
            task2_fnumber(page_table[i]) == fframe) {
            return i;
        }
    }

    return INVALID;
}

// Allocates the referenced page to the free frame.
unsigned int task3_allocate_page(unsigned int fframe, unsigned int page_table[],
                                 unsigned int physical_mem[]) {
    // Evict page corresponding to freed frame from page table
    unsigned int epage = task3_epage(page_table, fframe);
    page_table[epage] = 0;
    printf("evicted-page=%u,freed-frame=%u\n", epage, fframe);

    // Allocate the referenced page to the free frame
    task2_allocate_frame(physical_mem);
    return epage;
}