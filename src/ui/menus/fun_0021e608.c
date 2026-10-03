#include "types.h"
struct PreviewMenuGame {
    u8 pad_0[0x40];
    struct PreviewItemSelection * selection;
};

struct ItemPreviewPair {
    u8 pad_0[0x44];
    s32 primary_moby;
    s32 secondary_moby;
};

struct PreviewItemSelection {
    u8 pad_0[0x3C];
    s32 index;
    u8 pad_40[0x8];
    s32 table_address;
};

extern u8 available_preview_items[] __asm__("D_0013D4C0");
extern struct PreviewMenuGame *preview_menu_games[] __asm__("D_001D5BF4");
extern s32 draw_moby_list() __asm__("func_0020D330");
s32 draw_available_item_preview_mobys(struct ItemPreviewPair *preview) __asm__("FUN_0021e608");

s32 draw_available_item_preview_mobys(struct ItemPreviewPair *preview) {
    s32 primary_moby;
    s32 secondary_moby;
    struct PreviewItemSelection *selection;

    selection = preview_menu_games[0]->selection;
    if (available_preview_items[*(s16 *)((u8 *)((selection->index * 0xA)
                                              + selection->table_address) + 0x6)] == 0) {
        return 0;
    }
    primary_moby = preview->primary_moby;
    if (primary_moby != 0) {
        draw_moby_list(primary_moby, 1);
    }
    secondary_moby = preview->secondary_moby;
    if (secondary_moby != 0) {
        draw_moby_list(secondary_moby, 1);
    }
    return 8;
}

/* draw_moby_list reads only the Moby address and selection arguments. */
