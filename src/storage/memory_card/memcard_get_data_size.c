#include "types.h"

/* Return the aligned byte span of a linked DMA packet chain. */
s32 GetDmaPacketSpanBytes(const u32 *packet) __asm__("GetDmaPacketSpanBytes");

s32 GetDmaPacketSpanBytes(const u32 *packet) {
    s32 span_bytes = 8;

    if (packet[0] != 0) {
        s32 alignment_mask = -4;

        do {
            span_bytes += 8;
            span_bytes += (s32)packet[1];
            packet += 4;
            span_bytes = (span_bytes + 3) & alignment_mask;
        } while (packet[0] != 0);
    }

    return span_bytes + 8;
}

/* Recovered original symbol name. */
extern __typeof__(GetDmaPacketSpanBytes) memcard_GetDataSize
    __attribute__((alias("GetDmaPacketSpanBytes")));
