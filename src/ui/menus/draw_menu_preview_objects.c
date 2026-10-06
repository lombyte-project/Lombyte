#include "types.h"
struct Moby { u8 pad0[0x50]; u64 tag; u8 pad58[0x4E]; s16 oclass; };
struct MenuPreviewObject { u8 pad0[0x50]; u64 tag; };
struct MenuPreviewObjects { u8 pad0[0x44]; struct MenuPreviewObject *items[24]; u8 held[24]; };
extern s32 D_001D5020[];
extern void draw_moby_list(void *, s32) __asm__("func_0020D330");
s32 draw_menu_preview_objects(struct MenuPreviewObjects *preview) __asm__("FUN_0021ddf8");

s32 draw_menu_preview_objects(struct MenuPreviewObjects *preview) {
    s32 i;
    u8 *first_object;

    for (i = 0; i < 24; i++) {
        if (preview->items[i] == 0) {
            continue;
        }
        if (preview->held[i] != 0) {
            continue;
        }
        if (D_001D5020[i] == 0) {
            continue;
        }
        if (i == 7 && ((struct Moby *)preview->items[7])->oclass == 0x4A) {
            first_object = (u8 *)preview->items[0];
            if (first_object[0x52] != first_object[0x53]) {
                continue;
            }
        }
        switch (i) {
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 10:
        case 11:
        case 12:
            if ((preview->items[0]->tag & 0xFFFF0000) == 0x99990000
                && (u8)preview->items[0]->tag >= 0x4D && (u8)preview->items[0]->tag < 0x92) {
                continue;
            }
            break;
        }
        draw_moby_list(preview->items[i], 1);
    }
    return 4;
}

extern __typeof__(draw_menu_preview_objects) func_0021DDF8 __attribute__((alias("FUN_0021ddf8")));
