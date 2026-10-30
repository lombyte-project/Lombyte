/* deci2Putchar: buffered deci2 character output. */

#include "types.h"

extern s32 deci2_buffered_byte_count[] __asm__("D_0012FBFC");
extern s8  deci2_output_buffer[] __asm__("D_00154D00");
extern s32 Kputs(s8 *text);

void deci2Putchar(s32 c) __asm__("deci2Putchar");

void deci2Putchar(s32 output_byte) {
    s32 index = deci2_buffered_byte_count[0];

    if (index >= 126) {
        deci2_buffered_byte_count[0] = 0;
        deci2_output_buffer[127] = 0;
        Kputs(deci2_output_buffer);
        index = deci2_buffered_byte_count[0];
    }
    if (output_byte == '\n') {
        deci2_output_buffer[index] = (s8)output_byte;
        deci2_output_buffer[index + 1] = 0;
        deci2_buffered_byte_count[0] = 0;
        Kputs(deci2_output_buffer);
    } else {
        deci2_output_buffer[index] = (s8)output_byte;
        deci2_buffered_byte_count[0] = index + 1;
    }
}
