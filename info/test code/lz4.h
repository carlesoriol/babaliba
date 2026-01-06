#include <stdint.h>
#include <string.h>

/**
 * lz4_decompress
 * @src: Compressed input data
 * @src_len: Length of compressed data
 * @dst: Buffer to store decompressed data
 * @dst_capacity: Maximum size of dst buffer
 * * Returns: Number of bytes written to dst, or -1 on error.
 */
int lz4_decompress(const uint8_t* src, int src_len, uint8_t* dst, int dst_capacity) {
    const uint8_t* ip = src;
    const uint8_t* const iend = src + src_len;
    uint8_t* op = dst;
    uint8_t* const oend = dst + dst_capacity;

    while (ip < iend) {
        // 1. Read the Token
        uint8_t token = *ip++;
        int literal_len = token >> 4;

        // 2. Handle Literal Length Extension
        if (literal_len == 15) {
            uint8_t s;
            do {
                s = *ip++;
                literal_len += s;
            } while (s == 255);
        }

        // 3. Copy Literals
        if (op + literal_len > oend) return -1; // Overflow
        memcpy(op, ip, literal_len);
        ip += literal_len;
        op += literal_len;

        if (ip >= iend) break; // End of block

        // 4. Read Match Offset (2 bytes, Little Endian)
        uint16_t offset = ip[0] | (ip[1] << 8);
        ip += 2;

        // 5. Handle Match Length Extension
        int match_len = (token & 0x0F) + 4; // Minimum match is 4 bytes
        if (match_len == 19) { // 15 + 4
            uint8_t s;
            do {
                s = *ip++;
                match_len += s;
            } while (s == 255);
        }

        // 6. Copy Match from dictionary (back-reference)
        uint8_t* match_ptr = op - offset;
        if (match_ptr < dst || op + match_len > oend) return -1; // Security check

        // LZ4 allows overlapping matches, so we copy byte-by-byte
        for (int i = 0; i < match_len; i++) {
            *op++ = *match_ptr++;
        }
    }

    return (int)(op - dst);
}