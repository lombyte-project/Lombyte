/* Level start: initializes the level runtime state and the level item inventory. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/audio/streaming/register_audio_stream_callback.h"
#include "rnc/rendering/view/init_view_context.h"
#include "rnc/rendering/vu1/vu1_init_chain.h"
#include "rnc/runtime/memory/init_mem_slots.h"
#include "rnc/rendering/vu1/dmac_vif1_enable.h"

extern u8   D_L00_00197BC0_lui[] __asm__("D_L00_00197BC0") NOT_SDA;
extern char D_L00_001C5B00_lui[] __asm__("D_L00_001C5B00") NOT_SDA;
extern char D_L00_001BC1B0_lui[] __asm__("D_L00_001BC1B0") NOT_SDA;
void        select_level_vu_chain(void) __asm__("FUN_002335a0");
void        fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
void        register_part_types(void) __asm__("FUN_L00_00268900");
void        init_level_item_inventory(void) __asm__("FUN_L00_0023d198");
extern char D_L00_0019A980[] NOT_SDA;
extern char D_L00_0019A280[] NOT_SDA;
extern char D_L00_001C7700[] NOT_SDA;
extern char D_L00_001C6F00[] NOT_SDA;
extern char D_L00_001BD3B0[] NOT_SDA;
extern char D_L00_001BCDB0[] NOT_SDA;
extern char D_L00_00173EC0[] NOT_SDA;
void        init_level_runtime_state(void) __asm__("FUN_L00_0023d028");

void init_level_runtime_state(void) {
    int texture_allocation_base;
    register_audio_stream_callback();
    select_level_vu_chain();
    init_mem_slots();
    texture_allocation_base = gs_texture_allocation_base;
    gs_texture_allocation_cursor = texture_allocation_base;
    D_0015EE78_gp = texture_allocation_base;
    fill_transfer_words(D_L00_00173EC0, 0x87654321, 0x10);
    fill_transfer_words(D_L00_00197BC0_lui, -1, 0x800);
    fill_transfer_words(D_L00_0019A980, -1, 0xE00);
    fill_transfer_words(D_L00_0019A280, 0, 0xE0);
    fill_transfer_words(D_L00_001C5B00_lui, -1, 0xC00);
    fill_transfer_words(D_L00_001C7700, -1, 0x800);
    fill_transfer_words(D_L00_001C6F00, 0, 0x80);
    fill_transfer_words(D_L00_001BC1B0_lui, -1, 0x400);
    fill_transfer_words(D_L00_001BD3B0, -1, 0x400);
    fill_transfer_words(D_L00_001BCDB0, 0, 0x40);
    init_view_context();
    update_view_context();
    vu1_init_chain();
    register_part_types();
    dmac_vif1_enable();
    init_level_item_inventory();
    if (current_level_index != 0) {
        unlock_planet(current_level_index);
    }
    D_0015EE24 += 0xB4;
}

extern int D_L00_001C3E98[];
extern u8  D_0015EDD0[] __attribute__((sda));
extern int D_00141660_23d028[] __asm__("D_00141660");
void       append_level_item_if_absent(int i) __asm__("FUN_L00_0023d3d8");

void init_level_item_inventory(void) {
    int           j;
    int           item;
    unsigned char entry;
    if (current_level_index >= 19) {
        int i;
        for (i = 0; i < 37; i++) {
            item_available[i] = 1;
            items_acquired[i] = 1;
        }
        {
            int entry0 = 0x4A;
            int entry1 = 0x50;
            int entry2 = 0x4F;
            int entry3 = 0x54;
            int entry4 = 0x51;
            int entry5 = 0x4E;
            int entry6 = 0x4B;
            int entry7 = 0x52;
            int entry8 = 0x4D;
            int entry9 = 0x59;
            int entry10 = 0x58;
            int entry11 = 0x53;
            pause_iop_sprite_lists[0] = 15;
            D_0015EDD0[0] = entry0;
            D_0015EDD0[1] = entry1;
            D_0015EDD0[2] = entry2;
            D_0015EDD0[3] = entry3;
            D_0015EDD0[4] = entry4;
            D_0015EDD0[5] = entry5;
            D_0015EDD0[6] = entry6;
            D_0015EDD0[7] = entry7;
            D_0015EDD0[8] = entry8;
            D_0015EDD0[9] = entry9;
            D_0015EDD0[10] = entry10;
            D_0015EDD0[11] = entry11;
            pause_iop_sprite_lists[1] = 12;
            pause_iop_sprite_lists[2] = 13;
            pause_iop_sprite_lists[3] = 11;
            pause_iop_sprite_lists[4] = 17;
            pause_iop_sprite_lists[5] = 10;
            pause_iop_sprite_lists[6] = 16;
            pause_iop_sprite_lists[7] = 19;
        }
    } else {
        int i;
        if (item_available[10] == 0) {
            grant_inventory_item(10, 1);
            D_0015EDD0[0] = 0x4A;
            D_00141660_23d028[0] = 10;
        }
        for (i = 0; i < 12; i++) {
            entry = D_0015EDD0[i];
            if (entry != 0xFF) {
                item = entry & 0x3F;
                if (item != 10) {
                    for (j = 0; j < 19 && D_L00_001C3E98[j] != item; j++)
                        ;
                    if (j == 19 || item == 0)
                        D_0015EDD0[i] = 0xFF;
                }
            }
        }
        append_level_item_if_absent(current_level_index);
    }
}

/* Appends D_L00_001C3E98[i] to the 0xFF-terminated D_0015EDD0 list unless already present. */
void append_level_item_if_absent(int i) {
    int item_id = D_L00_001C3E98[i];
    int j;
    if (item_id == 0)
        return;
    for (j = 0; (D_0015EDD0[j] & 0x3F) != item_id && D_0015EDD0[j] != 0xFF; j++)
        ;
    if (D_0015EDD0[j] == 0xFF)
        D_0015EDD0[j] = item_id;
}

char D_L01_0019AE00 NOT_SDA = 0;
char D_L01_0019A700 NOT_SDA = 0;
char D_L01_001C7B80 NOT_SDA = 0;
char D_L01_001C7380 NOT_SDA = 0;
char D_L01_001BD830 NOT_SDA = 0;
char D_L01_001BD230 NOT_SDA = 0;
char D_L02_0019AF80 NOT_SDA = 0;
char D_L02_0019A880 NOT_SDA = 0;
char D_L02_001C7D00 NOT_SDA = 0;
char D_L02_001C7500 NOT_SDA = 0;
char D_L02_001BD9B0 NOT_SDA = 0;
char D_L02_001BD3B0 NOT_SDA = 0;
char D_L02_001744C0 NOT_SDA = 0;
char D_L03_0019AA80 NOT_SDA = 0;
char D_L03_0019A380 NOT_SDA = 0;
char D_L03_001C7800 NOT_SDA = 0;
char D_L03_001C7000 NOT_SDA = 0;
char D_L03_001BD4B0 NOT_SDA = 0;
char D_L03_001BCEB0 NOT_SDA = 0;
char D_L03_00173FC0 NOT_SDA = 0;
char D_L04_0019AB00 NOT_SDA = 0;
char D_L04_0019A400 NOT_SDA = 0;
char D_L04_001C7880 NOT_SDA = 0;
char D_L04_001C7080 NOT_SDA = 0;
char D_L04_001BD530 NOT_SDA = 0;
char D_L04_001BCF30 NOT_SDA = 0;
char D_L05_0019AE00 NOT_SDA = 0;
char D_L05_0019A700 NOT_SDA = 0;
char D_L05_001C7B80 NOT_SDA = 0;
char D_L05_001C7380 NOT_SDA = 0;
char D_L05_001BD830 NOT_SDA = 0;
char D_L05_001BD230 NOT_SDA = 0;
char D_L05_001742C0 NOT_SDA = 0;
char D_L06_0019B100 NOT_SDA = 0;
char D_L06_0019AA00 NOT_SDA = 0;
char D_L06_001C7E80 NOT_SDA = 0;
char D_L06_001C7680 NOT_SDA = 0;
char D_L06_001BDB30 NOT_SDA = 0;
char D_L06_001BD530 NOT_SDA = 0;
char D_L06_00174640 NOT_SDA = 0;
char D_L07_0019AA00 NOT_SDA = 0;
char D_L07_0019A300 NOT_SDA = 0;
char D_L07_001C7780 NOT_SDA = 0;
char D_L07_001C6F80 NOT_SDA = 0;
char D_L07_001BD430 NOT_SDA = 0;
char D_L07_001BCE30 NOT_SDA = 0;
char D_L07_00173F40 NOT_SDA = 0;
char D_L08_0019B180 NOT_SDA = 0;
char D_L08_0019AA80 NOT_SDA = 0;
char D_L08_001C7F00 NOT_SDA = 0;
char D_L08_001C7700 NOT_SDA = 0;
char D_L08_001BDBB0 NOT_SDA = 0;
char D_L08_001BD5B0 NOT_SDA = 0;
char D_L08_001746C0 NOT_SDA = 0;
char D_L09_0019AB00 NOT_SDA = 0;
char D_L09_0019A400 NOT_SDA = 0;
char D_L09_001C7880 NOT_SDA = 0;
char D_L09_001C7080 NOT_SDA = 0;
char D_L09_001BD530 NOT_SDA = 0;
char D_L09_001BCF30 NOT_SDA = 0;
char D_L09_00174040 NOT_SDA = 0;
char D_L10_0019AE00 NOT_SDA = 0;
char D_L10_0019A700 NOT_SDA = 0;
char D_L10_001C7B80 NOT_SDA = 0;
char D_L10_001C7380 NOT_SDA = 0;
char D_L10_001BD830 NOT_SDA = 0;
char D_L10_001BD230 NOT_SDA = 0;
char D_L10_00174340 NOT_SDA = 0;
char D_L11_0019B380 NOT_SDA = 0;
char D_L11_0019AC80 NOT_SDA = 0;
char D_L11_001C8100 NOT_SDA = 0;
char D_L11_001C7900 NOT_SDA = 0;
char D_L11_001BDDB0 NOT_SDA = 0;
char D_L11_001BD7B0 NOT_SDA = 0;
char D_L11_001748C0 NOT_SDA = 0;
char D_L12_0019AE00 NOT_SDA = 0;
char D_L12_0019A700 NOT_SDA = 0;
char D_L12_001C7B80 NOT_SDA = 0;
char D_L12_001C7380 NOT_SDA = 0;
char D_L12_001BD830 NOT_SDA = 0;
char D_L12_001BD230 NOT_SDA = 0;
char D_L12_00174340 NOT_SDA = 0;
char D_L13_0019AC80 NOT_SDA = 0;
char D_L13_0019A580 NOT_SDA = 0;
char D_L13_001C7A00 NOT_SDA = 0;
char D_L13_001C7200 NOT_SDA = 0;
char D_L13_001BD6B0 NOT_SDA = 0;
char D_L13_001BD0B0 NOT_SDA = 0;
char D_L13_001741C0 NOT_SDA = 0;
char D_L14_0019B080 NOT_SDA = 0;
char D_L14_0019A980 NOT_SDA = 0;
char D_L14_001C7E00 NOT_SDA = 0;
char D_L14_001C7600 NOT_SDA = 0;
char D_L14_001BDAB0 NOT_SDA = 0;
char D_L14_001BD4B0 NOT_SDA = 0;
char D_L14_001745C0 NOT_SDA = 0;
char D_L15_0019AF80 NOT_SDA = 0;
char D_L15_0019A880 NOT_SDA = 0;
char D_L15_001C7D00 NOT_SDA = 0;
char D_L15_001C7500 NOT_SDA = 0;
char D_L15_001BD9B0 NOT_SDA = 0;
char D_L15_001BD3B0 NOT_SDA = 0;
char D_L16_0019AE00 NOT_SDA = 0;
char D_L16_0019A700 NOT_SDA = 0;
char D_L16_001C7B80 NOT_SDA = 0;
char D_L16_001C7380 NOT_SDA = 0;
char D_L16_001BD830 NOT_SDA = 0;
char D_L16_001BD230 NOT_SDA = 0;
char D_L17_0019B280 NOT_SDA = 0;
char D_L17_0019AB80 NOT_SDA = 0;
char D_L17_001C8000 NOT_SDA = 0;
char D_L17_001C7800 NOT_SDA = 0;
char D_L17_001BDCB0 NOT_SDA = 0;
char D_L17_001BD6B0 NOT_SDA = 0;
char D_L17_001747C0 NOT_SDA = 0;
char D_L18_0019B380 NOT_SDA = 0;
char D_L18_0019AC80 NOT_SDA = 0;
char D_L18_001C8100 NOT_SDA = 0;
char D_L18_001C7900 NOT_SDA = 0;
char D_L18_001BDDB0 NOT_SDA = 0;
char D_L18_001BD7B0 NOT_SDA = 0;
char D_L18_001748C0 NOT_SDA = 0;
