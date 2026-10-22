#include "types.h"

void relocate_asset_entry_pointers(u8 *a0, s32 idx, s32 on) __asm__("FUN_002032e0");

void relocate_asset_entry_pointers(u8 *table_container, s32 animation_index, s32 on) {
    u8 *base = table_container;
    u8 *selected_table_base;
    u32 optional_data_offset;

    base += animation_index * 4;
    selected_table_base = *(u8 **)(base + 72);
    optional_data_offset = *(u32 *)(selected_table_base + 20);
    if (optional_data_offset != 0) {
        *(u32 *)(selected_table_base + 20) = (u32)(selected_table_base + optional_data_offset);
    }
    if (*(u8 *)(selected_table_base + 16) != 0) {
        s32 i = 0;
        u32 *relocation_slot = (u32 *)(selected_table_base + 28);
        do {
            *relocation_slot = (u32)(selected_table_base + *relocation_slot);
            i++;
            relocation_slot++;
        } while (i < *(u8 *)(selected_table_base + 16));
    }
}
