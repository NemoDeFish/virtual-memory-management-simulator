#ifndef BIT_UTILS_H
#define BIT_UTILS_H

// Extract length bits from num, starting at bit start.
unsigned int extract_bits(unsigned int address, int start, int length);

// Encode two integers by combining the left integer with the right integer into
// a single integer.
unsigned int encode_bits(unsigned int left, unsigned int right,
                         unsigned int right_bits);

#endif