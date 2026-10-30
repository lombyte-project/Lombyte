#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/ui/map/map_icon.h"

struct Weapon {
    s32 name;
    u8  pad4[0x14];
};

extern struct Weapon D_001DFFB0[];
extern char          menu_item_decimal_format[] __asm__("D_0015FDA0");
extern char          menu_item_placeholder_error_text[] __asm__("D_0015FDA8");
extern u8           *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32           sprintf(char *, const char *, ...);
void                 format_menu_item_text(s32 idx, u8 *dst) __asm__("FUN_00208280");

/* Builds the label of map icon idx (update_map_icons): its help text with
   "%b" replaced by the name of item label_item. */

void format_menu_item_text(s32 map_icon_index, u8 *output_text_cursor) {
    u8  replacement_text[0x10];
    u8 *template_cursor;
    u8 *replacement_cursor;

    template_cursor =
        get_help_message_text(level_map_selection.icons[map_icon_index].label_text_id);
    replacement_cursor = replacement_text;
    if (template_cursor == 0) {
        return;
    }
    while (*template_cursor != '\0' && *template_cursor != '%') {
        *output_text_cursor++ = *template_cursor++;
    }
    if (*template_cursor == '\0') {
        *output_text_cursor = *template_cursor;
        return;
    }
    template_cursor++;
    if (*template_cursor == 'b') {
        sprintf((char *)replacement_text,
                menu_item_decimal_format,
                D_001DFFB0[level_map_selection.icons[map_icon_index].label_item].name);
    } else {
        sprintf((char *)replacement_text, menu_item_placeholder_error_text);
    }
    template_cursor++;
    while (*replacement_cursor != '\0') {
        *output_text_cursor++ = *replacement_cursor++;
    }
    while (*template_cursor != '\0') {
        *output_text_cursor++ = *template_cursor++;
    }
    *output_text_cursor = '\0';
}

extern __typeof__(format_menu_item_text) func_00208280 __attribute__((alias("FUN_00208280")));
