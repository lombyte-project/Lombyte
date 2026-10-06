#include "types.h"
#include "sda.h"

typedef struct {
    s32 unk0;
    u16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u16 unk10;
} MenuItem;

typedef struct {
    u8 pad0[0x4];
    u16 unk4;
    u8 pad6[0x2];
    u8 unk8;
    u8 pad9[0x2D];
    s16 unk36;
    s32 unk38;
    u8 *unk3C;
} Menu;

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    u16 unk14;
    u8 pad16[0x2];
} Weapon;

typedef struct {
    char *s[6];
} Suffixes;

extern s32 D_00160070 __attribute__((sda));
extern s32 D_0015F60C;
extern s32 D_0015F60C_far __asm__("D_0015F60C") MACRO_ADDR;
extern s32 D_001993C0[];
extern Suffixes D_001E8680;
extern char D_00160088[];
extern char D_00160090[];
extern s32 D_0015ED88;
extern Weapon D_001DFFB0[];
extern s32 D_0015ED98;
extern u8 D_0013D4C0[];
extern u8 D_0013D4E8[];
extern u8 D_0013D388[];

extern void func_001E9458(s32);
extern s32 remove_hud_item(s32) __asm__("FUN_001ff480");
extern void func_001E9460(s32);
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern char *strcpy(char *, const char *);
extern char *FindSubstring(char *, char *);
extern s32 sprintf(char *, const char *, ...);
extern void copy_text_to_shared_buffer(char *) __asm__("func_001FF658");
extern s32 compute_clamped_count_difference(void) __asm__("FUN_00215248");

void func_00216C48(void *arg0, Menu *menu, s32 arg2);

#define SCRATCH ((char *)0x70000000)

void FUN_00216c48(void *arg0, Menu *menu, s32 arg2) {
    char buf[0x100];
    Suffixes sfx;
    char num[0x20];
    MenuItem *e;
    char *p;
    s32 ok;
    s32 next;
    u16 flags;
    s16 n;
    s16 kind;
    s32 t;

    e = (MenuItem *)(menu->unk3C + menu->unk36 * 0x1C);
    if (menu->unk3C == 0) {
        return;
    }
    if (arg2 != 0) {
        menu->unk4 = menu->unk36;
        menu->unk38 = D_0015F60C;
        n = e->unk6;
        if (menu->unk36 != n) {
            flags = e->unk10;
            menu->unk36 = e->unk6;
            e = (MenuItem *)(menu->unk3C + n * 0x1C);
            menu->unk8 = *(u8 *)&e->unk10 & 1;
            if (flags & 4) {
                D_001993C0[2] = (s32)arg0;
                D_001993C0[3] = (s32)menu;
                if (e->unk4 & 0x4000) {
                    func_001E9458((s16)(e->unk4 ^ 0x4000));
                } else {
                    remove_hud_item(D_00160070);
                    D_00160070 = -1;
                    func_001E9460((s16)e->unk4);
                }
            }
        }
    }
    if (menu->unk36 == -1) {
        return;
    }
    D_001993C0[5] = e->unk0;
    kind = e->unk8;
    if (kind == 6) {
        sfx = D_001E8680;
        strcpy(SCRATCH, get_help_message_text(e->unk0));
        p = FindSubstring(SCRATCH, D_00160088);
        if (p != 0) {
            p[1] = 's';
        }
        t = D_001DFFB0[e->unkA].unk14;
        sprintf(num, D_00160090, t / 1000, sfx.s[D_0015ED88 % 6], t % 1000);
        sprintf(buf, SCRATCH, num);
    } else {
        sprintf(buf, get_help_message_text(e->unk0));
    }
    if (D_001993C0[5] != 0) {
        copy_text_to_shared_buffer(buf);
    } else if (D_00160070 != -1) {
        remove_hud_item(D_00160070);
        D_00160070 = -1;
    }
    if (e->unk8 == 0) {
        return;
    }
    ok = 0;
    switch (e->unk8) {
    case 1:
        ok = !(D_0015ED98 < D_001DFFB0[e->unkA].unk0);
        break;
    case 2:
        ok = D_0013D4C0[e->unkA] != 0;
        break;
    case 3:
        ok = 0;
        if (D_0013D4C0[e->unkA] != 0) {
            ok = D_0013D4E8[e->unkA] == 0;
        }
        break;
    case 4:
        ok = D_0013D388[e->unkA] != 0;
        break;
    case 5:
        ok = !(compute_clamped_count_difference() < e->unkA);
        break;
    case 6:
        ok = D_0015ED98 >= D_001DFFB0[e->unkA].unk14 && compute_clamped_count_difference() >= 4;
        break;
    default:
        ok = 0;
        break;
    }
    if (ok) {
        next = e->unkC;
    } else {
        next = e->unkE;
    }
    if (next != menu->unk36) {
        menu->unk36 = next;
        menu->unk38 = D_0015F60C_far;
        e = (MenuItem *)(menu->unk3C + next * 0x1C);
        menu->unk8 = *(u8 *)&e->unk10 & 1;
        func_00216C48(arg0, menu, 0);
    }
}

extern __typeof__(FUN_00216c48) func_00216C48 __attribute__((alias("FUN_00216c48")));
