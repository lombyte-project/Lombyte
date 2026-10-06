extern int NextBit(void *bitstream, int bit_count) __asm__("_nextBit");
extern void FlushBuffer(void *bitstream, int byte_count) __asm__("_flushBuf");

void ExtraInfo(void *bitstream) __asm__("_extrainfo");

void ExtraInfo(void *bitstream) {
    while (NextBit(bitstream, 1) != 0) {
        FlushBuffer(bitstream, 8);
    }
}
