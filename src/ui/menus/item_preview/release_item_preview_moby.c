#include "types.h"
struct ItemPreviewLink {
    u8 pad_0[0x44];
    s32 moby;
};

extern s32 delete_moby() __asm__("FUN_00225530");
s32 release_item_preview_moby(struct ItemPreviewLink *preview) __asm__("FUN_0021eaf0");

s32 release_item_preview_moby(struct ItemPreviewLink *preview) {
    preview->moby = delete_moby(preview->moby);
    return 0;
}
