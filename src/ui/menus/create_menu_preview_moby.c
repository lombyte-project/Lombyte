#include "types.h"
struct PreviewMobyResource {
    u8 pad0[6];
    u8 class_flags;
};
struct PreviewMoby {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[3];
    struct PreviewMobyResource *resource;
    u8 pad28[8];
    u8 culling_radius;
    u8 force_visible;
    u16 control;
    u8 pad34[0x3F];
    u8 state_73;
};
extern u8 moby_class_slots[] __asm__("D_001B3AC0");
extern struct PreviewMoby *create_moby(s32) __asm__("func_0020C4F8");
extern void refresh_moby_spatial_bounds(struct PreviewMoby *) __asm__("func_0020DEF8");
extern void PackRenderCommandFields(struct PreviewMoby *, s32, s32, s32, s32);

struct PreviewMoby *create_menu_preview_moby(s32 oclass) __asm__("FUN_00225490");

struct PreviewMoby *create_menu_preview_moby(s32 oclass) {
    struct PreviewMoby *moby;
    struct PreviewMoby *result;
    u8 unset = 0xFF;
    if (moby_class_slots[oclass] == unset)
        return 0;
    {
        moby = create_moby(oclass);
        if (moby != 0) {
            moby->control = unset;
            moby->culling_radius = unset;
            moby->state = 0;
            moby->force_visible = 1;
            refresh_moby_spatial_bounds(moby);
            PackRenderCommandFields(moby, 0x202020, 0xE, 0xE, 0);
            if (moby->resource->class_flags != 0) {
                moby->state_73 = 0x18;
            }
        }
        result = moby;
    }
    return result;
}

extern struct PreviewMoby *func_00225490(s32 oclass) __attribute__((alias("FUN_00225490")));
