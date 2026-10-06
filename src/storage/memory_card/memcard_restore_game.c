#include "types.h"
extern u8 D_001A04C0[];
extern u8 D_001A07C0[];
extern char D_001E81D0[];
extern s32 GetDmaPacketSpanBytes(void *);
extern void DebugPrint(char *, ...);
extern void memcard_restore_data(u8 *, s32, void *) __asm__("func_0020AF20");
void memcard_restore_game(u8 *p) __asm__("FUN_00209298");

void memcard_restore_game(u8 *p) {
    s32 size0 = GetDmaPacketSpanBytes(D_001A04C0);
    s32 size1 = GetDmaPacketSpanBytes(D_001A07C0);
    s32 len0 = ((s32 *)p)[0];
    s32 i;

    if (len0 != size0 || ((s32 *)p)[1] != size1) {
        DebugPrint(D_001E81D0);
        return;
    }
    p += 8;
    memcard_restore_data(p, 0, D_001A04C0);
    p += len0;
    for (i = 0; i < 20; i++) {
        memcard_restore_data(p, i, D_001A07C0);
        p += size1;
    }
}

extern __typeof__(memcard_restore_game) func_00209298
    __attribute__((alias("FUN_00209298")));
