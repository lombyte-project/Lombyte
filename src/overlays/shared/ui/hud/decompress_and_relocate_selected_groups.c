/* HUD: decompresses and relocates the selected groups. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/game/overlay_game_state.h"
#include "asm.h"
#include "sda.h"

typedef struct {
    char  pad[0x50];
    int   sz[8];
    char  pad2[0x20];
    void *p[8];
} T_234fd0;

typedef struct {
    char      pad[0x18];
    T_234fd0 *t;
} G_234fd0;

extern G_234fd0           D_L00_0017E4D8_lui __asm__("D_L00_0017E4D8") NOT_SDA;
void                      stash_receive_data(void *, void *, int, int, int) __asm__("FUN_00232f20");
void                      fast_decompress(void *, int) __asm__("FUN_L00_0024d8d0");
void                      link_hud_bank(int, int) __asm__("FUN_001fefc0");
extern int D_L00_0017E510 NOT_SDA;
extern int D_L00_0017E518 NOT_SDA;
extern int D_L00_0017E51C NOT_SDA;
extern int D_L00_0017E520 NOT_SDA;
void decompress_and_relocate_selected_groups(int mode) __asm__("FUN_L00_00234fd0");

void decompress_and_relocate_selected_groups(int mode) {
    G_234fd0 *decompression_slot_state = &D_L00_0017E4D8_lui;
    int       decompression_destination = D_L00_001B9CF0.unk104;
    void     *compressed_data_buffer = (void *)D_L00_001B9CF0.unk10;
    {
        int slot_1_size;
        if ((slot_1_size = decompression_slot_state->t->sz[1])) {
            stash_receive_data(compressed_data_buffer,
                               decompression_slot_state->t->p[1],
                               0,
                               D_L00_0017E510 / 16,
                               0);
            fast_decompress(compressed_data_buffer, decompression_destination);
            link_hud_bank(0, decompression_destination);
            decompression_destination += slot_1_size;
        }
    }
    {
        int slot_3_size;
        if ((slot_3_size = decompression_slot_state->t->sz[3])) {
            stash_receive_data(compressed_data_buffer,
                               decompression_slot_state->t->p[3],
                               0,
                               D_L00_0017E518 / 16,
                               0);
            fast_decompress(compressed_data_buffer, decompression_destination);
            link_hud_bank(2, decompression_destination);
            decompression_destination += slot_3_size;
        }
    }
    if (mode == 0 || mode == 2) {
        {
            int slot_4_size;
            if ((slot_4_size = decompression_slot_state->t->sz[4])) {
                stash_receive_data(compressed_data_buffer,
                                   decompression_slot_state->t->p[4],
                                   0,
                                   D_L00_0017E51C / 16,
                                   0);
                fast_decompress(compressed_data_buffer, decompression_destination);
                link_hud_bank(3, decompression_destination);
                decompression_destination += slot_4_size;
            }
        }
    }
    if ((unsigned)(mode - 1) < 2) {
        G_234fd0 *group_4_decompression_state = &D_L00_0017E4D8_lui;
        if (group_4_decompression_state->t->sz[5]) {
            stash_receive_data(compressed_data_buffer,
                               group_4_decompression_state->t->p[5],
                               0,
                               D_L00_0017E520 / 16,
                               0);
            fast_decompress(compressed_data_buffer, decompression_destination);
            link_hud_bank(4, decompression_destination);
        }
    }
}

typedef struct {
    int pad[5];
    int a[8];
    int b[16];
    int c[8];
} T_235310;

typedef struct {
    int   v;
    short s;
    short pad;
} E_235310;

typedef struct {
    char      pad[0x18];
    T_235310 *t;
    char      pad2[8];
    E_235310 *a;
    E_235310 *b;
} G_235310;

extern G_235310 D_L00_0017E4D8 NOT_SDA;

void apply_group_entry_adjustment(int group_index) __asm__("FUN_L00_00235310");

void apply_group_entry_adjustment(int group_index) {
    int subtraction_amount = D_L00_0017E4D8.t->c[group_index];
    int i, j, s, end;
    if (group_index == 0) {
        for (j = 0; j < D_L00_0017E4D8.t->b[0]; j++) {
            D_L00_0017E4D8.a[j].s = 0;
        }
    }
    if (group_index)
        s = D_L00_0017E4D8.t->a[group_index - 1];
    else
        s = 0;
    end = D_L00_0017E4D8.t->a[group_index];
    for (i = s; i < end; i++) {
        D_L00_0017E4D8.b[i].v -= subtraction_amount;
        D_L00_0017E4D8.b[i].v |= 0x80000000;
    }
    if (group_index)
        s = D_L00_0017E4D8.t->b[group_index - 1];
    else
        s = 0;
    end = D_L00_0017E4D8.t->b[group_index];
    for (i = s; i < end; i++) {
        D_L00_0017E4D8.a[i].v -= subtraction_amount;
        D_L00_0017E4D8.a[i].v |= 0x80000000;
    }
    D_L00_0017E4D8.t->c[group_index] = 0;
}

extern volatile int l00_inactive_moby_array_start __asm__("D_L00_0015FA70");
extern volatile int l00_inactive_loaded_moby_end __asm__("D_L00_0015FA74");
extern volatile int l00_inactive_moby_pool_last_slot __asm__("D_L00_0015FA78");
extern volatile int l00_inactive_moby_pool_end __asm__("D_L00_0015FA7C");

#include "rnc/overlay/level_labels.h"

extern volatile int D_L00_0015FFE0;
extern volatile int l00_active_moby_pool_end __asm__("D_L00_0015FFE8");
extern volatile int l00_active_moby_context_selector __asm__("D_L00_0015F728") __attribute__((sda));

void select_moby_context(int state) __asm__("FUN_L00_00235668");

void select_moby_context(int state) {
    int oldState = l00_active_moby_context_selector;
    if (state != oldState) {
        int old70, old74, old78, old7C, oldDC, oldE0, oldE8;
        oldDC = (int)moby_table;
        old70 = l00_inactive_moby_array_start;
        l00_inactive_moby_array_start = oldDC;
        oldDC = D_L00_0015FFDC;
        old74 = l00_inactive_loaded_moby_end;
        l00_inactive_loaded_moby_end = oldDC;
        oldDC = D_L00_0015FFE0;
        old78 = l00_inactive_moby_pool_last_slot;
        oldE8 = l00_active_moby_pool_end;
        old7C = l00_inactive_moby_pool_end;
        l00_active_moby_context_selector = oldState ^ 1;
        *(u8 *volatile *)&moby_table = (u8 *)old70;
        D_L00_0015FFDC = old74;
        D_L00_0015FFE0 = old78;
        l00_inactive_moby_pool_last_slot = oldDC;
        l00_active_moby_pool_end = old7C;
        l00_inactive_moby_pool_end = oldE8;
    }
}

s32 D_L02_0017EB18[1] __attribute__((section(".data"))) = {0};

s32 D_L02_0017EB1C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_0017E618[1] __attribute__((section(".data"))) = {0};

s32 D_L03_0017E61C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_0017EC98[1] __attribute__((section(".data"))) = {0};

s32 D_L06_0017EC9C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_0017E598[1] __attribute__((section(".data"))) = {0};

s32 D_L07_0017E59C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_0017ED18[1] __attribute__((section(".data"))) = {0};

s32 D_L08_0017ED1C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_0017EC18[1] __attribute__((section(".data"))) = {0};

s32 D_L14_0017EC1C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_0017EB18[1] __attribute__((section(".data"))) = {0};

s32 D_L15_0017EB1C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_0017EE18[1] __attribute__((section(".data"))) = {0};

s32 D_L17_0017EE1C[1] __attribute__((section(".data"))) = {0};

G_234fd0 D_L01_0017E958 = {0};

int D_L01_0017E990 NOT_SDA = 0;
int D_L01_0017E998 NOT_SDA = 0;
int D_L01_0017E99C NOT_SDA = 0;
int D_L01_0017E9A0 NOT_SDA = 0;
int                D_L01_0015FA70 = 0;
int                D_L01_0015FA74 = 0;
int                D_L01_0015FA78 = 0;
int                D_L01_0015FA7C = 0;
int                D_L01_0015FFE8 = 0;
int                D_L01_0015F728 = 0;
G_234fd0           D_L02_0017EAD8 = {0};

int D_L02_0017EB10 NOT_SDA = 0;
int D_L02_0017EB20 NOT_SDA = 0;
int                D_L02_0015FA70 = 0;
int                D_L02_0015FA74 = 0;
int                D_L02_0015FA78 = 0;
int                D_L02_0015FA7C = 0;
int                D_L02_0015FFE8 = 0;
int                D_L02_0015F728 = 0;
G_234fd0           D_L03_0017E5D8 = {0};

int D_L03_0017E610 NOT_SDA = 0;
int D_L03_0017E620 NOT_SDA = 0;
int                D_L03_0015FA70 = 0;
int                D_L03_0015FA74 = 0;
int                D_L03_0015FA78 = 0;
int                D_L03_0015FA7C = 0;
int                D_L03_0015FFE8 = 0;
int                D_L03_0015F728 = 0;
G_234fd0           D_L04_0017E658 = {0};

int D_L04_0017E690 NOT_SDA = 0;
int D_L04_0017E698 NOT_SDA = 0;
int D_L04_0017E69C NOT_SDA = 0;
int D_L04_0017E6A0 NOT_SDA = 0;
int                D_L04_0015FA70 = 0;
int                D_L04_0015FA74 = 0;
int                D_L04_0015FA78 = 0;
int                D_L04_0015FA7C = 0;
int                D_L04_0015FFE8 = 0;
int                D_L04_0015F728 = 0;
G_234fd0           D_L05_0017E958 = {0};

int D_L05_0017E990 NOT_SDA = 0;
int D_L05_0017E998 NOT_SDA = 0;
int D_L05_0017E99C NOT_SDA = 0;
int D_L05_0017E9A0 NOT_SDA = 0;
int                D_L05_0015FA80 = 0;
int                D_L05_0015FA84 = 0;
int                D_L05_0015FA88 = 0;
int                D_L05_0015FA8C = 0;
int                D_L05_0015FFE8 = 0;
int                D_L05_0015F738 = 0;
int D_L06_0017EC90 NOT_SDA = 0;
int D_L06_0017ECA0 NOT_SDA = 0;
int                D_L06_0015FA70 = 0;
int                D_L06_0015FA74 = 0;
int                D_L06_0015FA78 = 0;
int                D_L06_0015FA7C = 0;
int                D_L06_0015F728 = 0;
G_234fd0           D_L07_0017E558 = {0};

int D_L07_0017E590 NOT_SDA = 0;
int D_L07_0017E5A0 NOT_SDA = 0;
int                D_L07_0015FA70 = 0;
int                D_L07_0015FA74 = 0;
int                D_L07_0015FA78 = 0;
int                D_L07_0015FA7C = 0;
int                D_L07_0015F728 = 0;
G_234fd0           D_L08_0017ECD8 = {0};

int D_L08_0017ED10 NOT_SDA = 0;
int D_L08_0017ED20 NOT_SDA = 0;
int                D_L08_0015FA70 = 0;
int                D_L08_0015FA74 = 0;
int                D_L08_0015FA78 = 0;
int                D_L08_0015FA7C = 0;
int                D_L08_0015FFE8 = 0;
int                D_L08_0015F728 = 0;
G_234fd0           D_L09_0017E658 = {0};

int D_L09_0017E690 NOT_SDA = 0;
int D_L09_0017E698 NOT_SDA = 0;
int D_L09_0017E69C NOT_SDA = 0;
int D_L09_0017E6A0 NOT_SDA = 0;
int                D_L09_0015FA70 = 0;
int                D_L09_0015FA74 = 0;
int                D_L09_0015FA78 = 0;
int                D_L09_0015FA7C = 0;
int                D_L09_0015FFE8 = 0;
int                D_L09_0015F728 = 0;
int D_L10_0017E990 NOT_SDA = 0;
int D_L10_0017E998 NOT_SDA = 0;
int D_L10_0017E99C NOT_SDA = 0;
int D_L10_0017E9A0 NOT_SDA = 0;
int                D_L10_0015FA70 = 0;
int                D_L10_0015FA74 = 0;
int                D_L10_0015FA78 = 0;
int                D_L10_0015FA7C = 0;
int                D_L10_0015FFE8 = 0;
int                D_L10_0015F728 = 0;
G_234fd0           D_L11_0017EED8 = {0};

int D_L11_0017EF10 NOT_SDA = 0;
int D_L11_0017EF18 NOT_SDA = 0;
int D_L11_0017EF1C NOT_SDA = 0;
int D_L11_0017EF20 NOT_SDA = 0;
int                D_L11_0015FA70 = 0;
int                D_L11_0015FA74 = 0;
int                D_L11_0015FA78 = 0;
int                D_L11_0015FA7C = 0;
int                D_L11_0015FFE8 = 0;
int                D_L11_0015F728 = 0;
G_234fd0           D_L12_0017E958 = {0};

int D_L12_0017E990 NOT_SDA = 0;
int D_L12_0017E998 NOT_SDA = 0;
int D_L12_0017E99C NOT_SDA = 0;
int D_L12_0017E9A0 NOT_SDA = 0;
int                D_L12_0015FA70 = 0;
int                D_L12_0015FA74 = 0;
int                D_L12_0015FA78 = 0;
int                D_L12_0015FA7C = 0;
int                D_L12_0015FFE8 = 0;
int                D_L12_0015F728 = 0;
G_234fd0           D_L13_0017E7D8 = {0};

int D_L13_0017E810 NOT_SDA = 0;
int D_L13_0017E818 NOT_SDA = 0;
int D_L13_0017E81C NOT_SDA = 0;
int D_L13_0017E820 NOT_SDA = 0;
int                D_L13_0015FA70 = 0;
int                D_L13_0015FA74 = 0;
int                D_L13_0015FA78 = 0;
int                D_L13_0015FA7C = 0;
int                D_L13_0015FFE8 = 0;
int                D_L13_0015F728 = 0;
G_234fd0           D_L14_0017EBD8 = {0};

int D_L14_0017EC10 NOT_SDA = 0;
int D_L14_0017EC20 NOT_SDA = 0;
int                D_L14_0015FA70 = 0;
int                D_L14_0015FA74 = 0;
int                D_L14_0015FA78 = 0;
int                D_L14_0015FA7C = 0;
int                D_L14_0015FFE8 = 0;
int                D_L14_0015F728 = 0;
G_234fd0           D_L15_0017EAD8 = {0};

int D_L15_0017EB10 NOT_SDA = 0;
int D_L15_0017EB20 NOT_SDA = 0;
int                D_L15_0015FA70 = 0;
int                D_L15_0015FA74 = 0;
int                D_L15_0015FA78 = 0;
int                D_L15_0015FA7C = 0;
int                D_L15_0015FFE8 = 0;
int                D_L15_0015F728 = 0;
G_234fd0           D_L16_0017E958 = {0};

int D_L16_0017E990 NOT_SDA = 0;
int D_L16_0017E998 NOT_SDA = 0;
int D_L16_0017E99C NOT_SDA = 0;
int D_L16_0017E9A0 NOT_SDA = 0;
int                D_L16_0015FA80 = 0;
int                D_L16_0015FA84 = 0;
int                D_L16_0015FA88 = 0;
int                D_L16_0015FA8C = 0;
int                D_L16_0015FFE8 = 0;
int                D_L16_0015F738 = 0;
G_234fd0           D_L17_0017EDD8 = {0};

int D_L17_0017EE10 NOT_SDA = 0;
int D_L17_0017EE20 NOT_SDA = 0;
int                D_L17_0015FA70 = 0;
int                D_L17_0015FA74 = 0;
int                D_L17_0015FA78 = 0;
int                D_L17_0015FA7C = 0;
int                D_L17_0015FFE8 = 0;
int                D_L17_0015F728 = 0;
G_234fd0           D_L18_0017EED8 = {0};

int D_L18_0017EF10 NOT_SDA = 0;
int D_L18_0017EF18 NOT_SDA = 0;
int D_L18_0017EF1C NOT_SDA = 0;
int D_L18_0017EF20 NOT_SDA = 0;
int                D_L18_0015FA70 = 0;
int                D_L18_0015FA74 = 0;
int                D_L18_0015FA78 = 0;
int                D_L18_0015FA7C = 0;
int                D_L18_0015F728 = 0;

int D_L00_0017E518 NOT_SDA = 0;
int D_L00_0017E51C NOT_SDA = 0;
int D_L00_0017E520 NOT_SDA = 0;
