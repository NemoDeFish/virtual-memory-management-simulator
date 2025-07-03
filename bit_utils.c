// Extract length bits from num, starting at bit start.
unsigned int extract_bits(unsigned int num, int start, int length) {
    // Create a bit mask with length of 1s by first shifting an unsigned integer
    // 1 to the right by length bits, then subtracting 1 from it
    unsigned int mask = (1U << length) - 1;
    // Shift the number to the right so that the desired bits are at the LSB,
    // then use the AND operation with the mask to keep the lowest length bits
    // and clear the remaining bits
    return (num >> start) & mask;
}

// Encode two integers by combining the left integer with the right integer into
// a single integer.
unsigned int encode_bits(unsigned int left, unsigned int right,
                         unsigned int right_bits) {
    // Create a bit mask with length of 1s by first shifting an unsigned integer
    // 1 to the right by length bits, then subtracting 1 from it
    unsigned int mask = (1U << right_bits) - 1;
    // Shift the left integer by right_bits, mask the right integer to keep only
    // the right_bits, then combine them using the OR operation into a single
    // integer
    return (left << right_bits) | (right & mask);
}