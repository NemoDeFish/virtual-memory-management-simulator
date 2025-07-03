#include "constants.h"
#include "file_io.h"
#include "task1.h"
#include "bit_utils.h"

// Extracts the page number and offset for each logical address in the input
// file using file pointer.
void task1(FILE* fp) {
    unsigned int laddress, pnumber, poffset;
    
    // Read each logical address from input file
    while ((laddress = read_line(fp)) != INVALID) {
        // Extracts the page number and offset for each logical address
        pnumber = task1_pnumber(laddress);
        poffset = task1_poffset(laddress);
        // Prints an execution transcript for task 1
        task1_print(laddress, pnumber, poffset);
    }
}

// Extracts the offset (lowest 12 bits) from a logical address.
unsigned int task1_poffset(unsigned int laddress) {
    return extract_bits(laddress, 0, OFFSET_BITS);
}

// Extracts the page number (next 10 bits after offset) from a logical address.
unsigned int task1_pnumber(unsigned int laddress) {
    return extract_bits(laddress, OFFSET_BITS, PN_BITS);
}

// Prints an execution transcript for task 1.
void task1_print(unsigned int laddress, unsigned int pnumber,
                 unsigned int poffset) {
    printf("logical-address=%u,page-number=%u,offset=%u\n", laddress, pnumber,
           poffset);
}