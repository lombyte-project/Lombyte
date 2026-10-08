#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 D_0013D428[];
extern u8 D_0013D4C0[];
extern u8 D_001602A0[];
extern u8 D_001602B8[];
extern u8 D_001602C0[];
extern u8 D_001602D0[];
struct ImageEntry {
    u8 pad0[8];
    u16 unk8;
    u8 padA[4];
    u16 unkE;
    u8 pad10[8];
};
extern struct ImageEntry D_001DFFB0[];
extern void PackImageDescriptor(s32 *, s32);
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void font_print_window_regular(void *, u64, void *, s32) __asm__("func_001F7580");
extern u8 *get_help_message_text(s32) __asm__("func_001FDD10");
extern void vu1_add_g_sregister(s32, s32) __asm__("func_00233980");
extern s32 sprintf();
s32 FUN_0021f158(s32 arg0) {
    u8 sp_slot[0x50];
    u8 descriptor[0x20];
    s16 temp_4_20;
    s32 idx;
    s32 temp_17_38;
    s32 temp_18_40;
    struct ImageEntry *entry;
    struct MenuScreen *grid;
    u8 *end_ptr;

    grid = menu_system.current->focus;
    temp_4_20 = grid->data.grid.cells[grid->data.grid.selected_cell].id;
    idx = temp_4_20;
    if (*(idx + D_0013D4C0) == 0) {
        return 0;
    }
    temp_17_38 = D_0013D428[idx];
    entry = &D_001DFFB0[idx];
    temp_18_40 = (s32)entry->unkE;
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0xB);
    if (entry->unk8 == 0) {
        sprintf(sp_slot, get_help_message_text(0x4F52));
    } else {
        if (temp_17_38 < 0x3E8) {
            end_ptr = sp_slot + sprintf(sp_slot, D_001602B8, temp_17_38);
        } else {
            end_ptr = sp_slot + sprintf(sp_slot, D_001602C0, temp_17_38 / 1000, temp_17_38 % 1000);
        }
        if ((s32)temp_18_40 < 0x3E8) {
            sprintf(end_ptr, D_001602A0, (s32)temp_18_40);
        } else {
            sprintf(end_ptr, D_001602D0, (s32)temp_18_40 / 1000, (s32)temp_18_40 % 1000);
        }
    }
    setup_gif_paging(0);
    PackImageDescriptor((s32 *)descriptor, arg0);
    *(s16 *)(descriptor + 0x10) = 0x10;
    *(s16 *)(descriptor + 0x12) = 3;
    font_print_window_regular((s32 *)descriptor, (((u64)0x80FF << 0x10) | 0xA888), sp_slot, -1);
    do_gif_paging();
    return 2;
}
