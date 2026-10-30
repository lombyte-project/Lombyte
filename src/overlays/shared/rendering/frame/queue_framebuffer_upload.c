/* Frame: queues a framebuffer upload. */
#include "types.h"
#include "asm.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/rendering/vu1/vu1_tex_flush.h"

/* Ported from rac1-decomp (src/game/draw.c: func_001F7680), where it is exact; names translated to the US level program. */

extern unsigned char *D_L00_001611C0;
extern int            frame_size[] __asm__("D_0013E504");

#define PK ((unsigned char *)(*(int **)&D_L00_001611C0))

void queue_framebuffer_upload(unsigned int source) __asm__("FUN_L00_001fc558");

void queue_framebuffer_upload(unsigned int source) {
    int            remaining = frame_size[0];
    int            destination = draw_buffer_address >> 8;
    unsigned long  count;
    unsigned long  transfer;
    unsigned char *packet;
    unsigned long *words;

    do {
        count = (long)remaining;
        if (count > 0x80)
            count = 0x80;
        *(unsigned int *)(PK + 0) = 0x10000006;
        remaining -= 0x80;
        *(unsigned int *)(PK + 4) = 0;
        *(unsigned int *)(PK + 8) = 0;
        *(unsigned int *)(PK + 12) = 0x50000006;
        packet = PK;
        (*(int **)&D_L00_001611C0) = (int *)(packet + 0x10);
        transfer = (unsigned long)count << 7;
        words = (unsigned long *)(packet + 0x10);
        words[0] = 0x4000000000000001UL;
        words[1] = 0x0eeeeeee;
        words[2] = ((unsigned long)(unsigned int)destination << 32) | 0x8000000000000UL;
        words[3] = 0x50;
        words[4] = 0;
        words[5] = 0x51;
        words[6] = ((unsigned long)count << 32) | 0x200;
        words[7] = 0x52;
        words[8] = 0;
        words[9] = 0x53;
        words[10] = transfer | 0x0800000000008000UL;
        words[11] = 0;
        (*(int **)&D_L00_001611C0) = (int *)(packet + 0x70);
        *(unsigned int *)(packet + 0x70) = (unsigned int)(long)(int)(transfer | 0x30000000);
        *(unsigned int *)(PK + 4) = source;
        source += (int)(count * 0x800);
        destination += (int)(count * 8);
        *(unsigned int *)(PK + 8) = 0;
        *(unsigned int *)(PK + 12) = (unsigned int)transfer | 0x50000000;
        (*(int **)&D_L00_001611C0) = (int *)(PK + 0x10);
    } while (remaining > 0);
    vu1_tex_flush();
}
