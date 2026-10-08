#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/ui/map/map_icon.h"
struct Weapon {
    s32 name;
    u8 pad4[0x14];
};
extern struct Weapon D_001DFFB0[];
extern char D_0015FDA0[];
extern char D_0015FDA8[];
extern u8 *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 sprintf(char *, const char *, ...);
void format_menu_item_text(s32 idx, u8 *dst) __asm__("FUN_00208280");

/* Builds the label of map icon idx (update_map_icons): its help text with
   "%b" replaced by the name of item label_item. */

void format_menu_item_text(s32 idx, u8 *dst) {
    u8 buf[0x10];
    u8 *src;
    u8 *p;

    src = get_help_message_text(D_001A00F0.icons[idx].label_text_id);
    p = buf;
    if (src == 0) {
        return;
    }
    while (*src != '\0' && *src != '%') {
        *dst++ = *src++;
    }
    if (*src == '\0') {
        *dst = *src;
        return;
    }
    src++;
    if (*src == 'b') {
        sprintf((char *)buf, D_0015FDA0, D_001DFFB0[D_001A00F0.icons[idx].label_item].name);
    } else {
        sprintf((char *)buf, D_0015FDA8);
    }
    src++;
    while (*p != '\0') {
        *dst++ = *p++;
    }
    while (*src != '\0') {
        *dst++ = *src++;
    }
    *dst = '\0';
}

extern __typeof__(format_menu_item_text) func_00208280 __attribute__((alias("FUN_00208280")));
