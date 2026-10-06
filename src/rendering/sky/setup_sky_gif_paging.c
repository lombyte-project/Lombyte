#include "types.h"

#include "rnc/rendering/dma_tag.h"
struct SkyVisibilityEntry {
    u64 flags;
    u8 pad8[8];
};
struct SkyVisibilityList {
    u8 pad0[0xC];
    s16 count;
    u8 padE[2];
    struct SkyVisibilityEntry *points;
};
extern struct DmaTag *D_00160F00;
extern struct DmaTag *D_00160470;
extern u8 D_00160450[];
extern struct SkyVisibilityList *D_0016045C;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015F458;
extern void func_001F21B8(void *, s32);

void setup_sky_gif_paging(void) __asm__("FUN_0022b4c8");

void setup_sky_gif_paging(void) {
    struct DmaTag *tag;
    struct SkyVisibilityList *visibility_list;
    s32 entry_index;

    tag = D_00160F00;
    D_00160470 = tag;
    tag = tag + 1;
    D_00160F00 = tag;
    func_001F21B8(D_00160450, 1);
    visibility_list = D_0016045C;
    D_0015EE74 = D_0015EE78;
    D_0015F458 = 0;
    for (entry_index = 0; entry_index < visibility_list->count; entry_index++) {
        visibility_list->points[entry_index].flags = 0;
    }
}

extern __typeof__(setup_sky_gif_paging) func_0022B4C8 __attribute__((alias("FUN_0022b4c8")));
