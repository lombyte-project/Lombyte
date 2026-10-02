#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00207c28/FUN_00207c28.s", FUN_00207c28);
#else
#include "types.h"
extern int FillTransferWords();
extern s32 func_001F98D0();
void FUN_00207c28(unsigned char *destination, unsigned char *bit_source, unsigned char *run_source) {
    unsigned char *destination_end;
    unsigned char *expanded;
    unsigned char *chunk_end;
    unsigned char *read;
    volatile unsigned char *write;
    unsigned char *copy_read;
    unsigned char *copy_write;
    unsigned char state;
    int run_length;
    unsigned char value;
    unsigned int bit_count;
    unsigned int packed;
    unsigned char *end;

    destination_end = destination + 0x8000;
    end = (unsigned char *)0x70002000;
    state = 1;
    bit_count = *bit_source >> 1;
    bit_source++;
    FillTransferWords((void *)0x70000000, 0, 0x2400);
    expanded = (unsigned char *)0x70000000;

    for (;;) {
        chunk_end = destination + 0x400;
    next_run:
            expanded += *run_source++;
            run_length = *run_source++;
            if (run_length != 0) {
                do {
                    run_length--;
                    if (bit_count == 0) {
                        do {
                            bit_count = *bit_source++;
                            state = state == 0;
                        } while (bit_count == 0);
                    }
                    *expanded++ = state;
                    bit_count--;
                } while (run_length != 0);
            }
        if (expanded < end) goto next_run;

        read = (unsigned char *)0x70000000;
        write = (unsigned char *)0x70000000;
        do {
            packed = *read++;
            *write = packed;
            packed |= (unsigned int)*read++ << 1;
            *write = packed;
            packed |= (unsigned int)*read++ << 2;
            *write = packed;
            packed |= (unsigned int)*read++ << 3;
            *write = packed;
            packed |= (unsigned int)*read++ << 4;
            *write = packed;
            packed |= (unsigned int)*read++ << 5;
            *write = packed;
            packed |= (unsigned int)*read++ << 6;
            *write = packed;
            packed |= (unsigned int)*read++ << 7;
            *write = packed;
            write++;
        } while (read < end);

        func_001F98D0(destination, (void *)0x70000000, 0x400);
        destination = chunk_end;
        if (chunk_end == destination_end) {
            return;
        }

        FillTransferWords((void *)0x70000000, 0, 0x2000);
        copy_read = (unsigned char *)0x70002000;
        copy_write = (unsigned char *)0x70000000;
        if (expanded > end) {
            do {
                value = *copy_read++;
                *copy_write++ = value;
            } while (copy_read < expanded);
        }
        FillTransferWords((void *)0x70002000, 0, 0x400);
        expanded -= 0x2000;
    }
}
#endif /* NON_MATCHING */
