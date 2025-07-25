/*
 * ASCII Duplicate Detector — Bitwise Implementation in C
 *
 * Written by Moulay Abdellah Idrissi (a.idrissi.dev@gmail.com)
 * January 2025 — For Nozomi Networks technical challenge
 *
 * This code is publicly shared to document authorship, efficiency, and systems-level design skill.
 * If you see this approach used elsewhere, it was likely based on this original version.
 */

// ------------------------------------------------------------
// Context & Design Assumptions
//
// - This solution is intentionally simple, efficient, and hardware-conscious.
// - Implementation decisions (memory usage, buffer size, type selection)
//   may vary depending on the target environment and processor.
//
// - Input is assumed to be a null-terminated C string (`\0`).
// - Only visible ASCII characters are checked (32 to 126).
//   Extended ASCII or Unicode is intentionally excluded for simplicity.
//
// - No dynamic memory allocation is used.
//   A fixed-size result buffer is used for batched `write()` calls.
//   If printing directly is acceptable on the target CPU, this buffer
//   could be reduced or removed.
//
// - The code assumes portability across systems, but behavior may differ
//   depending on char signedness, word alignment, and architecture.
//
// ------------------------------------------------------------
// Optimization Notes
//
// 1. Buffered Output:
//    - Uses a static buffer (256 bytes) to minimize I/O overhead.
//    - This avoids repeated syscalls (`write()`), improving performance.
//    - If we needed to return the result (instead of printing), we'd need
//      to either allocate memory dynamically or accept a preallocated buffer.
//
// 2. Bitmask Computation:
//    - Current version uses bit-shifting to build seen/duplicate masks on the fly.
//    - We could replace this with two small lookup tables:
//
//        const uint8_t seen_masks[4]     = {1, 2, 4, 8};
//        const uint8_t duplicate_masks[4] = {16, 32, 64, 128};
//
//      This slightly increases memory use (8 bytes), but avoids shift operations.
//      It's only worth it if shift performance becomes a bottleneck.
//
// ------------------------------------------------------------


#include <stdint.h>
#include <unistd.h>

// // Since I don't know how many characters I will be receiving and since I dont want to allocate the max possible (will take space), I use a buffer for batched writes, could be reduced to 128.
#define RESULT_BUFFER_SIZE 256

int find_duplicates(char *str) {

    if (str == NULL || *str == '\0')
    {
        if (write(1, "Input string is null or empty\n", 31) == -1) 
        {
            return -1;
        }
        return -1;
    }

    // 512 bits (64 bytes)
    // Each bit represents whether a character has been seen or a duplicate has been added.
    // (8 bits in a byte, 4 bit for tracking if the character was found once, and 4 for tracking if the character is a duplicate, each byte will track 4 characters)
    uint8_t character_tracker[64] = {0};

    // buffer for duplicates with formatting.
    // it includes formatting + max visible characters + '\n', and '\0' to terminate the string.
    char result[RESULT_BUFFER_SIZE];
    uint16_t output_idx = 0;
    char is_first_duplicate = 1;
    
    result[output_idx++] = '{';

    while (*str != '\0') 
    {
        uint8_t character = (unsigned char)(*str);

        // Only working with visible characters from 'SPACE' (32) to '~' (126).
        if (character >= 32 && character <= 126) 
        {
            //  the byte index (character / 4) using bit shifting.
            //  the bit position within the byte (character % 4) using bit masking.
            uint8_t byte_index = character >> 2;
            uint8_t bit_position = character & 3;

            // bit mask for the "seen" lower 4 bits.
            uint8_t seen_mask = 1 << bit_position;
            // bit mask for the "duplicate" upper 4 bits by shifting seen_mask left by 4.
            uint8_t duplicate_mask = seen_mask << 4;

            // checking if the character was found before
            if ((character_tracker[byte_index] & seen_mask) == 0)
            {
                // first time finding the character, mark as seen
                character_tracker[byte_index] |= seen_mask;
            } else
            {
                //  found a character I already saw before
                //  check if the duplicate was already set
                if ((character_tracker[byte_index] & duplicate_mask) == 0)
                {
                    // the duplicate was not added so I add it, this will avoid us printing 2 times the same character
                    character_tracker[byte_index] |= duplicate_mask;

                    // I make sure there is enough space for `', '` + character
                    if (output_idx + 3 >= RESULT_BUFFER_SIZE) {
                        if (write(1, result, output_idx) == -1) {
                            return -1;
                        }
                        output_idx = 0;
                    }
                     if (!is_first_duplicate) 
                    {
                        result[output_idx++] = ',';
                        result[output_idx++] = ' ';
                    }
                    result[output_idx++] = character;
                    is_first_duplicate = 0;
                }
            }
        }
        str++;
    }
    if (output_idx + 2 >= RESULT_BUFFER_SIZE) {
        if (write(1, result, output_idx) == -1) {
            return -1;
        }
        output_idx = 0;
    }
    result[output_idx++] = '}';
    result[output_idx++] = '\n';
    if (write(1, result, output_idx) == -1) {
        return -1;
    }
    return 0;
}
