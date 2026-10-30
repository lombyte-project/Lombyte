#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

#include "rnc/game/item_state.h"

extern char D_001602A0[];
extern u8   ammo_count_decimal_format[] __asm__("D_001602B8");
extern u8   ammo_count_grouped_decimal_format[] __asm__("D_001602C0");
extern char D_001602D0[];

struct ImageEntry {
    u8  pad0[8];
    u16 unk8;
    u8  padA[4];
    u16 unkE;
    u8  pad10[8];
};

extern struct ImageEntry D_001DFFB0[];
extern void              PackImageDescriptor(s32 *, s32);
extern void              setup_gif_paging(s32) __asm__("func_001F4280");
extern void              do_gif_paging() __asm__("func_001F4398");
extern void font_print_window_regular(void *, u64, void *, s32) __asm__("func_001F7580");
extern u8  *get_help_message_text(s32) __asm__("func_001FDD10");
extern void vu1_add_g_sregister(s32, s32) __asm__("func_00233980");
extern s32  sprintf();
s32         draw_selected_item_ammo(s32 image_descriptor_input) __asm__("FUN_0021f158");

s32 draw_selected_item_ammo(s32 image_descriptor_input) {
    u8                 sp_slot[0x50];
    u8                 descriptor[0x20];
    s16                cell_id;
    s32                selected_item_id;
    s32                ammo;
    s32                entry_field;
    struct ImageEntry *entry;
    struct MenuScreen *grid;
    u8                *end_ptr;

    grid = menu_system.current->focus;
    cell_id = grid->data.grid.cells[grid->data.grid.selected_cell].id;
    selected_item_id = cell_id;
    if (*(selected_item_id + item_available) == 0) {
        return 0;
    }
    ammo = weapon_ammo_counts[selected_item_id];
    entry = &D_001DFFB0[selected_item_id];
    entry_field = (s32)entry->unkE;
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0xB);
    if (entry->unk8 == 0) {
        sprintf(sp_slot, get_help_message_text(0x4F52));
    } else {
        if (ammo < 0x3E8) {
            end_ptr = sp_slot + sprintf(sp_slot, ammo_count_decimal_format, ammo);
        } else {
            end_ptr = sp_slot +
                      sprintf(sp_slot, ammo_count_grouped_decimal_format, ammo / 1000, ammo % 1000);
        }
        if ((s32)entry_field < 0x3E8) {
            sprintf(end_ptr, D_001602A0, (s32)entry_field);
        } else {
            sprintf(end_ptr, D_001602D0, (s32)entry_field / 1000, (s32)entry_field % 1000);
        }
    }
    setup_gif_paging(0);
    PackImageDescriptor((s32 *)descriptor, image_descriptor_input);
    *(s16 *)(descriptor + 0x10) = 0x10;
    *(s16 *)(descriptor + 0x12) = 3;
    font_print_window_regular((s32 *)descriptor, (((u64)0x80FF << 0x10) | 0xA888), sp_slot, -1);
    do_gif_paging();
    return 2;
}
