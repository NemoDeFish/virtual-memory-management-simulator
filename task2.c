#include "constants.h"
#include "file_io.h"
#include "task1.h"
#include "task2.h"
#include "bit_utils.h"

// Implement system's page table and translate logical addresses to physical
// addresses for each logical address in the input file using file pointer.
void task2(FILE* fp) {
    unsigned int physical_mem[NUM_FRAMES] = {0};
    unsigned int page_table[NUM_PAGES] = {0};
    unsigned int laddress, pfault, fnumber, paddress, pnumber, poffset, entry,
        present_bit;

    // Read each logical address from input file
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

            // Allocate page to free frame in increasing frame number
            fnumber = task2_allocate_frame(physical_mem);

            // For this task, we do not check if the page was successfully
            // allocated to a free frame because we assume that the number of
            // pages referenced by the running process does not exceed the
            // number of frames in the system. This means that there will always
            // be free frames.

            // Update corresponding page table entry
            page_table[pnumber] = task2_update_entry(fnumber);
        } else {
            // In case where page is in physical memory
            pfault = 0;

            fnumber = task2_fnumber(entry);
        }

        // Use the information in the page table entry and the offset to
        // construct the corresponding physical address
        paddress = task2_paddress(fnumber, poffset);

        // Prints an execution transcript for task 2.
        task2_print(pnumber, pfault, fnumber, paddress);
    }
}

// Use page number to identify relevant page table entry.
unsigned int task2_entry(unsigned int* page_table, unsigned int pnumber) {
    return page_table[pnumber];
}

// Extract the rightmost present bit from the page table entry.
unsigned int task2_present_bit(unsigned int entry) {
    return extract_bits(entry, 0, PRESENT_BIT);
}

// Allocate page to free frame in increasing frame number.
unsigned int task2_allocate_frame(unsigned int* physical_mem) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        // Check if frame is free
        if (physical_mem[i] == 0) {
            // Mark frame as used
            physical_mem[i] = 1;
            return i;
        }
    }

    return INVALID;
}

// Update corresponding page table entry by shifting the frame number to the
// left by 1 and append the present bit at the end.
unsigned int task2_update_entry(unsigned int fnumber) {
    return encode_bits(fnumber, 1, PRESENT_BIT);
}

// Extract the leftmost frame number bits from the page table entry starting
// after the present bit.
unsigned int task2_fnumber(unsigned int entry) {
    return extract_bits(entry, PRESENT_BIT, FRAME_BITS);
}

// Use the information in the page table entry and the offset to
// construct the corresponding physical address.
unsigned int task2_paddress(unsigned int fnumber, unsigned int poffset) {
    return fnumber * FRAME_SIZE + poffset;
}

// Prints an execution transcript for task 2.
void task2_print(unsigned int pnumber, unsigned int pfault,
                 unsigned int fnumber, unsigned int paddress) {
    printf("page-number=%u,page-fault=%u,frame-number=%u,physical-address=%u\n",
           pnumber, pfault, fnumber, paddress);
}