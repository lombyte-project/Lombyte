#include "types.h"
#include "rnc/math/asm_helpers.h"

extern void fill_transfer_words(void *, s32, s32) __asm__("FUN_001f97e8");
void        decode_compressed_occlusion_map(u8 *destination,
                                            u8 *control_stream,
                                            u8 *span_stream) __asm__("FUN_00207c28");

/* Decode a 32 KiB bit map in 1 KiB output blocks. Each span pair skips bytes
   then writes a run of bits in scratchpad. A zero control count toggles the bit
   again until a nonzero count appears. Pack eight expanded bits per byte and
   carry any expansion past 0x70002000 into the next block. */
void decode_compressed_occlusion_map(u8 *destination, u8 *control_stream, u8 *span_stream) {
    u8          *destination_end;
    volatile u8 *expanded_cursor;
    u8          *next_start;
    u8          *bit_read;
    volatile u8 *packed_write;
    u8          *carry_read;
    u8          *carry_write;
    u8           bit_value;
    int          run_length;
    u32          packed;
    u32          control_remaining;
    u8          *scratchpad_end;

    destination_end = destination + 0x8000;
    scratchpad_end = (u8 *)0x70002000;
    control_remaining = *control_stream >> 1;
    control_stream++;
    fill_transfer_words((void *)0x70000000, 0, 0x2400);
    bit_value = 1;
    expanded_cursor = (u8 *)0x70000000;

    for (;;) {
        next_start = destination + 0x400;
        do {
            expanded_cursor += *span_stream++;
            run_length = *span_stream++;
            while (run_length != 0) {
                run_length--;
                if (control_remaining == 0) {
                    do {
                        control_remaining = *control_stream++;
                        bit_value = !bit_value;
                    } while (control_remaining == 0);
                }
                *expanded_cursor++ = bit_value;
                control_remaining--;
            }
        } while (expanded_cursor < scratchpad_end);

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

        fast_mem_copy(destination, (void *)0x70000000, 0x400);
        destination = next_start;
        if (destination == destination_end) {
            return;
        }

        next_start = (u8 *)expanded_cursor - 0x2000;
        fill_transfer_words((void *)0x70000000, 0, 0x2000);
        carry_read = (u8 *)0x70002000;
        carry_write = (u8 *)0x70000000;
        if (expanded_cursor > scratchpad_end) {
            do {
                *carry_write++ = *carry_read++;
            } while (carry_read < expanded_cursor);
        }
        fill_transfer_words((void *)0x70002000, 0, 0x400);
        expanded_cursor = next_start;
    }
}

extern __typeof__(decode_compressed_occlusion_map) func_00207C28
    __attribute__((alias("FUN_00207c28")));
