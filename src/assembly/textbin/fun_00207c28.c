#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00207c28/FUN_00207c28.s", FUN_00207c28);
#else
#include "types.h"

extern void FillTransferWords(void *, s32, s32) __asm__("func_001F97E8");
extern void copy_blocks_16_forward(void *, void *, s32) __asm__("func_001F98D0");
void decode_compressed_occlusion_map(u8 *destination, u8 *control_stream, u8 *span_stream) __asm__("FUN_00207c28");

/* Produces 32 KiB in 1 KiB blocks, carrying expanded bits across scratchpad
   blocks. Control zero runs toggle repeatedly; span zero runs only skip. */
void decode_compressed_occlusion_map(u8 *destination, u8 *control_stream, u8 *span_stream) {
    u8 *destination_end;
    u8 *expanded_cursor;
    u8 *chunk_end;
    u8 *bit_read;
    volatile u8 *packed_write;
    u8 *carry_read;
    u8 *carry_write;
    u8 bit_value;
    int run_length;
    u8 value;
    u32 packed;
    u32 control_remaining;
    u8 *scratchpad_end;

    destination_end = destination + 0x8000;
    scratchpad_end = (u8 *)0x70002000;
    bit_value = 1;
    control_remaining = *control_stream >> 1;
    control_stream++;
    FillTransferWords((void *)0x70000000, 0, 0x2400);
    expanded_cursor = (u8 *)0x70000000;

    for (;;) {
        chunk_end = destination + 0x400;
    next_run:
            expanded_cursor += *span_stream++;
            run_length = *span_stream++;
            if (run_length != 0) {
                do {
                    run_length--;
                    if (control_remaining == 0) {
                        do {
                            control_remaining = *control_stream++;
                            bit_value = bit_value == 0;
                        } while (control_remaining == 0);
                    }
                    *expanded_cursor++ = bit_value;
                    control_remaining--;
                } while (run_length != 0);
            }
        if (expanded_cursor < scratchpad_end) goto next_run;

        bit_read = (u8 *)0x70000000;
        packed_write = (u8 *)0x70000000;
        do {
            packed = *bit_read++;
            *packed_write = packed;
            packed |= (u32)*bit_read++ << 1;
            *packed_write = packed;
            packed |= (u32)*bit_read++ << 2;
            *packed_write = packed;
            packed |= (u32)*bit_read++ << 3;
            *packed_write = packed;
            packed |= (u32)*bit_read++ << 4;
            *packed_write = packed;
            packed |= (u32)*bit_read++ << 5;
            *packed_write = packed;
            packed |= (u32)*bit_read++ << 6;
            *packed_write = packed;
            packed |= (u32)*bit_read++ << 7;
            *packed_write = packed;
            packed_write++;
        } while (bit_read < scratchpad_end);

        copy_blocks_16_forward(destination, (void *)0x70000000, 0x400);
        destination = chunk_end;
        if (chunk_end == destination_end) {
            return;
        }

        FillTransferWords((void *)0x70000000, 0, 0x2000);
        carry_read = (u8 *)0x70002000;
        carry_write = (u8 *)0x70000000;
        if (expanded_cursor > scratchpad_end) {
            do {
                value = *carry_read++;
                *carry_write++ = value;
            } while (carry_read < expanded_cursor);
        }
        FillTransferWords((void *)0x70002000, 0, 0x400);
        expanded_cursor -= 0x2000;
    }
}

extern __typeof__(decode_compressed_occlusion_map) func_00207C28 __attribute__((alias("FUN_00207c28")));

#endif /* NON_MATCHING */
