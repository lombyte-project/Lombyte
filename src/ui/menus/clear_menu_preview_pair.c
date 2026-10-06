#include "types.h"
struct MenuPreviewPair {
    u8 pad_0[0x44];
    s32 first_object;
    s32 second_object;
};

extern s32 delete_moby() __asm__("FUN_00225530");
s32 clear_menu_preview_pair(struct MenuPreviewPair *preview) __asm__("FUN_0021df58");

s32 clear_menu_preview_pair(struct MenuPreviewPair *preview) {
    s32 first_result;

    first_result = delete_moby(preview->first_object);
    preview->first_object = first_result;
    /* Retail passes the first deletion result to the second call. */
    preview->second_object = delete_moby(first_result);
    return 0;
}
