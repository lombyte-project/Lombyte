/* Menus: finds or assigns a bolt source slot. */
#include "types.h"
#include "rnc/game/bolt_sources.h"
#include "asm.h"

extern int  D_0013CB04[];
extern char D_L00_001B9CF8[];

#include "sda.h"

typedef struct {
    unsigned short a;
    unsigned short b;
    int            c;
    unsigned short d;
} E_27f448;

typedef struct {
    unsigned short a;
    unsigned short pad;
    unsigned short b;
    unsigned short pad2[3];
} T_27f448;

typedef struct {
    char pad[0x224];
    int  x224;
} S_27f448;

extern S_27f448 D_L00_001841F0_lui __asm__("D_L00_001841F0") NOT_SDA;

typedef struct {
    char pad0[0x30];
    int  x30;
    char pad34[0xC];
    int  x40;
} P_27f448;

extern E_27f448 D_L00_001BA3B8[] NOT_SDA;
extern T_27f448 D_L00_001C1E40[] NOT_SDA;
extern int     *selection_values __asm__("D_L00_001602A0") __attribute__((sda));
extern s32      D_0013D510_lui[] __asm__("D_0013D510") NOT_SDA;

int initialize_selection_entries_and_index(P_27f448 *selection_state) __asm__("FUN_L00_0027f448");

int initialize_selection_entries_and_index(P_27f448 *selection_state) {
    int i;
    for (i = 0; i < 20 && D_0013D510_lui[i]; i++) {
        D_L00_001BA3B8[i].b = 1;
        D_L00_001BA3B8[i].c = 0;
        D_L00_001BA3B8[i].a = D_L00_001C1E40[D_0013D510_lui[i]].a;
        D_L00_001BA3B8[i].d = D_L00_001C1E40[D_0013D510_lui[i]].b;
    }
    D_L00_001BA3B8[i].a = 0;
    selection_state->x40 = 0;
    selection_state->x30 |= 0x8000;
    for (i = 0; selection_values[i]; i++) {
        if (D_L00_001841F0_lui.x224 == selection_values[i]) {
            selection_state->x40 = i;
            break;
        }
    }
    return 0;
}

INCLUDE_ASM("config/us/overlays/asm/FUN_L00_0027f700.s", FUN_L00_0027f700);
/* Ported from rac1-decomp (src/overlays/shared/pause_00277208.c: func_L00_00286068), where it is exact; names translated to the US level program. */

int find_or_assign_bolt_source_slot(int lvl, int uid) __asm__("FUN_L00_00284d90");

int find_or_assign_bolt_source_slot(int lvl, int uid) {
    int k = -1;

    if (uid >= 0) {
        uid++;
        k = 63;
        while (k >= 0 && bolt_source_stats[lvl][k].uid != uid &&
               bolt_source_stats[lvl][k].uid != 0) {
            k--;
        }
        if (k >= 0) {
            bolt_source_stats[lvl][k].uid = uid;
        }
    }
    return k;
}

E_27f448 D_L01_001BA838 = {0};

T_27f448 D_L01_001C22C0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L02_001BA9B8 = {0};

T_27f448 D_L02_001C2440 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L03_001BA4B8 = {0};

T_27f448 D_L03_001C1F40 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L04_001BA538 = {0};

T_27f448 D_L04_001C1FC0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L05_001BA838 = {0};

T_27f448 D_L05_001C22C0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L06_001BAB38 = {0};

T_27f448 D_L06_001C25C0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L07_001BA438 = {0};

T_27f448 D_L07_001C1EC0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L08_001BABB8 = {0};

T_27f448 D_L08_001C2640 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L09_001BA538 = {0};

T_27f448 D_L09_001C1FC0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L10_001BA838 = {0};

T_27f448 D_L10_001C22C0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L11_001BADB8 = {0};

T_27f448 D_L11_001C2840 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L12_001BA838 = {0};

T_27f448 D_L12_001C22C0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L13_001BA6B8 = {0};

T_27f448 D_L13_001C2140 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L14_001BAAB8 = {0};

T_27f448 D_L14_001C2540 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L15_001BA9B8 = {0};

T_27f448 D_L15_001C2440 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L16_001BA838 = {0};

T_27f448 D_L16_001C22C0 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L17_001BACB8 = {0};

T_27f448 D_L17_001C2740 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};

E_27f448 D_L18_001BADB8 = {0};

T_27f448 D_L18_001C2840 = {
    0x4eb9, 0, 0x4ede, {0, 0, 0}
};
