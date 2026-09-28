#include "types.h"
struct RenderResource {
    u8 pad0[6];
    u8 flags;
};
struct RenderEntry {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[3];
    struct RenderResource *resource;
    u8 pad28[8];
    u8 mode;
    u8 enabled;
    u16 index;
    u8 pad34[0x3F];
    u8 size;
};
extern u8 D_001B3AC0[];
extern struct RenderEntry *func_0020C4F8(s32);
extern void func_0020DEF8(struct RenderEntry *);
extern void PackRenderCommandFields(struct RenderEntry *, s32, s32, s32, s32);

struct RenderEntry *FUN_00225490(s32 index) {
    struct RenderEntry *entry;
    struct RenderEntry *result;
    u8 unset = 0xFF;
    if (D_001B3AC0[index] == unset) return 0;
    {
        entry = func_0020C4F8(index);
        if (entry != 0) {
            entry->index = unset;
            entry->mode = unset;
            entry->state = 0;
            entry->enabled = 1;
            func_0020DEF8(entry);
            PackRenderCommandFields(entry, 0x202020, 0xE, 0xE, 0);
            if (entry->resource->flags != 0) {
                entry->size = 0x18;
            }
        }
        result = entry;
    }
    return result;
}

extern struct RenderEntry *func_00225490(s32 index) __attribute__((alias("FUN_00225490")));
