#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fast_decompress/FUN_0020b618.s", FUN_0020b618);
#else
#include "types.h"

/* Provisional reconstruction: this C follows the retail decompression logic,
 * but functional equivalence is not fully proven. The current C has not been
 * dynamically validated against the master archive; DMA hardware behavior and
 * malformed-input exception handling also remain unverified. The normal build
 * uses the assembly oracle, so a passing full ELF check does not validate this
 * C fallback's behavior. */

/* Channel 9 transfers alternating 0x2000-byte compressed blocks to EE scratchpad. */
#define SPR_DMA_CHCR (*(volatile u32 *)0x1000d400)
#define SPR_DMA_MADR (*(volatile u32 *)0x1000d410)
#define SPR_DMA_QWC  (*(volatile u32 *)0x1000d420)
#define SPR_DMA_SADR (*(volatile u32 *)0x1000d480)

#define START_INPUT_DMA() do { \
    SPR_DMA_SADR = scratch_offset; \
    SPR_DMA_QWC = 0x200; \
    scratch_offset ^= 0x2000; \
    SPR_DMA_MADR = (u32)dma_source; \
    SPR_DMA_CHCR = 0x100; \
} while (0)
#define WAIT_INPUT_DMA() do { \
    while (SPR_DMA_CHCR & 0x100) {} \
} while (0)

extern s32 RaiseKernelTrap(void);
s32 fast_decompress(u8 *source, u8 *destination) __asm__("FUN_0020b618");
s32 fast_decompress(u8 *source, u8 *destination)
{
    u8 *input;
    u8 *input_end;
    u8 *output;
    u8 *copy;
    u8 *dma_source;
    u8 *scratch;
    u8 *output_start;
    u32 compressed_size;
    u32 scratch_offset;
    u32 command;
    s32 count;
    u32 distance;
    u32 trailing;

    if (((u16)(source[0] | (source[1] << 8)) ^ 0x4157) != (source[2] ^ 0x44)) {
        RaiseKernelTrap();
        return RaiseKernelTrap();
    }
    compressed_size = (u32)source[3] | ((u32)source[4] << 8) |
                      ((u32)source[5] << 16) | ((u32)source[6] << 24);
    output_start = destination;
    output = destination;
    dma_source = source + 0x10;
    scratch_offset = 0;
    scratch = (u8 *)0x70000000;
    START_INPUT_DMA();
    dma_source += 0x2000;
    WAIT_INPUT_DMA();
    START_INPUT_DMA();
    dma_source += 0x2000;
    compressed_size -= 0x10;
    input = scratch;
    input_end = (u8 *)0x80000000;
    if (compressed_size < 0x2000) {
        input_end = input + compressed_size;
    }
    count = input[0] - 0x11;
    if (count <= 0) goto read_literal;
    input++;
    do {
        *output++ = *input++;
    } while (--count != 0);
    goto read_command;

read_literal:
    if (input >= input_end) goto done;
    command = *input++;
    if (command >= 0x10) goto decode_command;
    count = command;
    if (count == 0) count = *input++ + 0xf;
    output[0] = input[0];
    output[1] = input[1];
    output[2] = input[2];
    output += 3;
    input += 3;
    do {
        *output++ = *input++;
    } while (--count != 0);

read_command:
    if (input >= input_end) goto done;
    command = *input++;
decode_command:
    if (command >= 0x40) {
        distance = 1 + ((command >> 2) & 7) + ((u32)*input++ << 3);
        count = (command >> 5) - 1;
        copy = output - distance;
        goto copy_match;
    }
    if (command >= 0x20) {
        count = command & 0x1f;
        if (count == 0) count = *input++ + 0x1f;
        distance = 1 + (input[0] >> 2) + ((u32)input[1] << 6);
        input += 2;
        copy = output - distance;
        goto copy_match;
    }
    if (command < 0x10) {
        RaiseKernelTrap();
        /* If the exception resumes, the next retail instruction decodes a short match. */
        distance = 1 + ((command >> 2) & 7) + ((u32)*input++ << 3);
        count = (command >> 5) - 1;
        copy = output - distance;
        goto copy_match;
    }
    count = command & 7;
    if (count == 0) count = *input++ + 7;
    distance = ((command & 8) << 11) + (input[0] >> 2) + ((u32)input[1] << 6);
    input += 2;
    if (distance == 0) {
        if (count == 1) goto copy_trailing;
        WAIT_INPUT_DMA();
        scratch = (u8 *)0x70000000;
        START_INPUT_DMA();
        dma_source += 0x2000;
        input = scratch + scratch_offset;
        compressed_size -= 0x2000;
        input_end = (u8 *)0x80000000;
        if (compressed_size < 0x2000) input_end = input + compressed_size;
        goto read_literal;
    }
    copy = output - distance - 0x4000;
copy_match:
    output[0] = copy[0];
    output[1] = copy[1];
    output += 2;
    copy += 2;
    do {
        *output++ = *copy++;
    } while (--count > 0);
copy_trailing:
    trailing = input[-2] & 3;
    if (trailing != 0) {
        output[0] = input[0];
        output[1] = input[1];
        output[2] = input[2];
        output += trailing;
        input += trailing;
        goto read_command;
    }
    goto read_literal;
done:
    return output - output_start;
}
#endif /* NON_MATCHING */
