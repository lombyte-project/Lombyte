#include "types.h"

struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct Point { u64 flags; u8 pad8[8]; };
struct PointList { u8 pad0[0xC]; s16 count; u8 padE[2]; struct Point *points; };
extern struct DmaTag *D_00160F00;
extern struct DmaTag *D_00160470;
extern u8 D_00160450[];
extern struct PointList *D_0016045C;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015F458;
extern void func_001F21B8(void *, s32);

void setup_sky_gif_paging(void) __asm__("FUN_0022b4c8");

void setup_sky_gif_paging(void) {
    struct DmaTag *tag;
    struct PointList *list;
    s32 i;

    tag = D_00160F00;
    D_00160470 = tag;
    tag = tag + 1;
    D_00160F00 = tag;
    func_001F21B8(D_00160450, 1);
    list = D_0016045C;
    D_0015EE74 = D_0015EE78;
    D_0015F458 = 0;
    for (i = 0; i < list->count; i++) {
        list->points[i].flags = 0;
    }
}

extern __typeof__(setup_sky_gif_paging) func_0022B4C8 __attribute__((alias("FUN_0022b4c8")));
