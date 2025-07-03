#include "bit_utils.h"
#include "constants.h"
#include "file_io.h"
#include "linked_list.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

// Initialize all TLB entries to INVALID.
void initialize_tlb(unsigned int* tlb);

// Return index of TLB entry that maps to page number.
unsigned int find_tlb_index(unsigned int* tlb, unsigned int pnumber);

// Update the TLB according to a LRU policy.
void update_tlb(unsigned int* tlb, unsigned int pnumber, unsigned int fnumber,
                Node* recent_pages_tail, unsigned int* tsize);

// Flush TLB entry corresponding to evicted page if necessary.
void flush_tlb(unsigned int* tlb, unsigned int epage, unsigned int* tsize);

// Incorporate TLB into address translation process.
void task4(FILE* fp) {
    Node *fifo_queue_head = NULL, *fifo_queue_tail = NULL,
         *recent_pages_head = NULL, *recent_pages_tail = NULL;
    unsigned int physical_mem[NUM_FRAMES] = {0};
    unsigned int page_table[NUM_PAGES] = {0};
    unsigned int tlb[NUM_ENTRIES];
    unsigned int laddress, pfault, fnumber, paddress, pnumber, poffset, entry,
        present_bit, epage, tlbhit, tlb_index, tsize = 0;

    initialize_tlb(tlb);

    // Read each logical address from input files
    while ((laddress = read_line(fp)) != INVALID) {
        // Extract page number and offset from the logical address
        pnumber = task1_pnumber(laddress);
        poffset = task1_poffset(laddress);
        task1_print(laddress, pnumber, poffset);

        // On every memory reference, find the page that was referenced in the
        // list and move it to the head. If the page has not yet been used, add
        // page to most recently used pages queue
        if (find_move_head(&recent_pages_head, &recent_pages_tail, pnumber) ==
            INVALID) {
            add_head(&recent_pages_head, &recent_pages_tail, pnumber);
        }

        // Use the page number to look for a valid entry in the TLB
        tlb_index = find_tlb_index(tlb, pnumber);

        // If a valid entry was found in the TLB
        if (tlb_index != INVALID) {
            // TLB hit
            tlbhit = 1;

            // Extract frame number from TLB entry
            fnumber = extract_bits(tlb[tlb_index], 0, OFFSET_BITS);

            // Use the information in the page table entry and the offset to
            // construct the corresponding physical address
            paddress = task2_paddress(fnumber, poffset);

            printf("tlb-hit=%u,page-number=%u,frame=%u,physical-address=%u\n",
                   tlbhit, pnumber, fnumber, paddress);
        } else {  // If a valid entry was not found in the TLB
            // TLB miss
            tlbhit = 0;

            printf(
                "tlb-hit=%u,page-number=%u,frame=none,physical-address=none\n",
                tlbhit, pnumber);

            // Use page number to identify relevant page table entry
            entry = task2_entry(page_table, pnumber);
            present_bit = task2_present_bit(entry);
            fnumber = task2_fnumber(entry);
            pfault = 0;

            // In case of page fault
            if (present_bit == 0) {
                pfault = 1;

                // If there are free frames in physical memory, allocate page to
                // free frame in increasing frame number
                fnumber = task2_allocate_frame(physical_mem);

                // If there are no free frames in physical memory
                if (fnumber == INVALID) {
                    // Free up a frame according to the FIFO algorithm
                    fnumber =
                        task3_free_frame(&fifo_queue_head, &fifo_queue_tail);
                    // Allocate the referenced page to it
                    epage =
                        task3_allocate_page(fnumber, page_table, physical_mem);

                    // Flush TLB entry corresponding to evicted page if
                    // necessary
                    flush_tlb(tlb, epage, &tsize);
                }

                // Update FIFO queue
                add_tail(&fifo_queue_head, &fifo_queue_tail, fnumber);

                // Update corresponding page table entry
                page_table[pnumber] = task2_update_entry(fnumber);
            }

            // Update the TLB according to a LRU policy
            update_tlb(tlb, pnumber, fnumber, recent_pages_tail, &tsize);

            // Use the information in the page table entry and the offset to
            // construct the corresponding physical address
            paddress = task2_paddress(fnumber, poffset);

            // Prints an execution transcript for task 2.
            task2_print(pnumber, pfault, fnumber, paddress);
        }
    }

    freeList(recent_pages_head);
    freeList(fifo_queue_head);
}

// Initialize all TLB entries to INVALID.
void initialize_tlb(unsigned int* tlb) {
    for (int i = 0; i < NUM_ENTRIES; i++) {
        tlb[i] = INVALID;
    }
}

// Return index of TLB entry that maps to page number.
unsigned int find_tlb_index(unsigned int* tlb, unsigned int pnumber) {
    for (int i = 0; i < NUM_ENTRIES; i++) {
        // Extract page number from TLB entry
        if (extract_bits(tlb[i], OFFSET_BITS, PN_BITS) == pnumber) {
            return i;
        }
    }

    // If page number is not in the TLB
    return INVALID;
}

// Add a new entry to the TLB in the first available INVALID slot.
void add_tlb_entry(unsigned int* tlb, unsigned int pnumber,
                   unsigned int fnumber) {
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (tlb[i] == INVALID) {
            // Join the page number and the fnumber into a single entry
            tlb[i] = encode_bits(pnumber, fnumber, OFFSET_BITS);
            return;
        }
    }
}

// Update the TLB according to a LRU policy.
void update_tlb(unsigned int* tlb, unsigned int pnumber, unsigned int fnumber,
                Node* recent_pages_tail, unsigned int* tsize) {
    // If the TLB is not full
    if (*tsize < NUM_ENTRIES) {
        // Add TLB entry
        add_tlb_entry(tlb, pnumber, fnumber);
        (*tsize)++;
        printf("tlb-remove=none,tlb-add=%u\n", pnumber);
    } else {  // If the TLB is full
        // Find least-recently-used page in the TLB
        Node* current = recent_pages_tail;
        while (current != NULL) {
            int eindex = find_tlb_index(tlb, current->page);
            if (eindex != INVALID) {
                // Replace evicted entry with an entry for the page that was
                // just looked up
                tlb[eindex] = encode_bits(pnumber, fnumber, OFFSET_BITS);
                printf("tlb-remove=%u,tlb-add=%u\n", current->page, pnumber);
                return;
            }
            current = current->prev;
        }
    }
}

// Flush TLB entry corresponding to evicted page if necessary.
void flush_tlb(unsigned int* tlb, unsigned int epage, unsigned int* tsize) {
    // Check if evicted page is inside the TLB
    int eindex = find_tlb_index(tlb, epage);

    // If evicted page is inside TLB
    if (eindex != INVALID) {
        // Flush the TLB entry corresponding to the evicted page
        tlb[eindex] = INVALID;
        (*tsize)--;
        printf("tlb-flush=%u,tlb-size=%u\n", epage, *tsize);
    }
}