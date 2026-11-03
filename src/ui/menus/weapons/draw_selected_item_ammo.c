#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

#include "rnc/game/item_state.h"
#include "rnc/rendering/texture.h"
#include "rnc/sdk/libc/stdio.h"
#include "rnc/ui/text/text_region.h"

extern char D_001602A0[];
extern char ammo_count_decimal_format[] __asm__("D_001602B8");
extern char ammo_count_grouped_decimal_format[] __asm__("D_001602C0");
extern char D_001602D0[];

struct ImageEntry {
    u8  pad0[8];
    u16 unk8;
    u8  padA[4];
    u16 unkE;
    u8  pad10[8];
};

extern struct ImageEntry D_001DFFB0[];
extern void              setup_gif_paging(s32) __asm__("func_001F4280");
extern void              do_gif_paging() __asm__("func_001F4398");
extern void font_print_window_regular(void *, u64, void *, s32) __asm__("func_001F7580");
extern u8  *get_help_message_text(s32) __asm__("func_001FDD10");
extern void vu1_add_g_sregister(s32, s32) __asm__("func_00233980");
s32         draw_selected_item_ammo(s32 image_descriptor_input) __asm__("FUN_0021f158");

s32 draw_selected_item_ammo(s32 image_descriptor_input) {
    u8                 ammo_text[0x50];
    u8                 descriptor[0x20];
    s16                cell_id;
    s32                selected_item_id;
    s32                ammo;
    s32                entry_field;
    struct ImageEntry *selected_item_record;
    struct MenuScreen *grid;
    u8                *end_ptr;

    grid = menu_system.current->focus;
    cell_id = grid->data.grid.cells[grid->data.grid.selected_cell].id;
    selected_item_id = cell_id;
    if (*(selected_item_id + item_available) == 0) {
        return 0;
    }
    ammo = weapon_ammo_counts[selected_item_id];
    selected_item_record = &D_001DFFB0[selected_item_id];
    entry_field = (s32)selected_item_record->unkE;
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0xB);
    if (selected_item_record->unk8 == 0) {
        sprintf(ammo_text, get_help_message_text(0x4F52));
    } else {
        if (ammo < 0x3E8) {
            end_ptr = ammo_text + sprintf(ammo_text, ammo_count_decimal_format, ammo);
        } else {
            end_ptr =
                ammo_text +
                sprintf(ammo_text, ammo_count_grouped_decimal_format, ammo / 1000, ammo % 1000);
        }
        if ((s32)entry_field < 0x3E8) {
            sprintf(end_ptr, D_001602A0, (s32)entry_field);
        } else {
            sprintf(end_ptr, D_001602D0, (s32)entry_field / 1000, (s32)entry_field % 1000);
        }
    }
    setup_gif_paging(0);
    pack_image_descriptor((s32 *)descriptor, image_descriptor_input);
    ((struct TextRegion *)descriptor)->line_advance = 0x10;
    ((struct TextRegion *)descriptor)->flags =
        TEXT_REGION_CENTER_HORIZONTALLY | TEXT_REGION_CENTER_VERTICALLY;
    font_print_window_regular((s32 *)descriptor, (((u64)0x80FF << 0x10) | 0xA888), ammo_text, -1);
    do_gif_paging();
    return 2;
}

/* Small data reached with lui/addiu: the ammo count formats. */
char ammo_count_decimal_format[] __asm__("D_001602B8") __attribute__((section(".sdata"))) = "%d/";
char ammo_count_grouped_decimal_format[] __asm__("D_001602C0")
    __attribute__((section(".sdata"))) = "%d,%03d/";
