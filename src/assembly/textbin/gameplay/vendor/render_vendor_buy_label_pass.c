#include "types.h"
#include "asm.h"
#include "rnc/globals.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/vendor/render_vendor_buy_label_pass/FUN_00238f08.s", FUN_00238f08);
#else
/* Near miss, 14 rows: the argument setup order at the first call and one extra %hi for D_001DFFB0. */
/* Ported from rac1-decomp (src/game/vendor.c: func_0023A220). */
#include "types.h"
#include "sda.h"

typedef struct { int type; int kind; int pad[3]; } VSlotA;
typedef struct {
    char pad0[0x40];
    int alt;
    char pad1[0x14];
    int idx;
    int sel;
    char pad2[0x70];
    VSlotA slots[1];
} VendorA;
typedef struct {
    int first;
    int second;
    unsigned short price;
    unsigned short priceAlt;
    char pad1[2];
    unsigned short max;
    char pad2[8];
} WeaponInfoA;
extern char D_001E63C0[];
extern int D_0013D428[];
extern int D_0015ED98 MACRO_ADDR;
extern char D_001DFFB0[];
extern unsigned char D_0013D4E3[];
extern void func_001FB8F0(int, int, int, int, int, int, int);
extern void *memset(void *, int, unsigned int) __asm__("func_001153FC");
extern void *func_001FDD10(int);
extern void func_001F75F0(void *, long, void *, int);

void FUN_00238f08(char *unused, int x, int y) {
    char sp[0x18];
    VendorA *v;
    int m;
    int price;
    int sz = 0x18;

    func_001FB8F0(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    memset(sp, 0, 0x18);
    *(short *)(sp + 0x2) = y;
    *(short *)(sp + 0x6) = x;
    *(short *)(sp + 0x8) = x >> 1;
    *(short *)(sp + 0xA) = (y >> 1) - 7;
    *(short *)(sp + 0x10) = 0x10;
    *(short *)(sp + 0x12) = 1;
    if (((VendorA *)D_001E63C0)->sel != 0) {
        if (((VendorA *)D_001E63C0)->slots[((VendorA *)D_001E63C0)->idx].kind == 1 &&
            D_0013D428[((VendorA *)D_001E63C0)->slots[((VendorA *)D_001E63C0)->idx].type] >=
                ((WeaponInfoA *)(D_001DFFB0 + ((VendorA *)D_001E63C0)->slots[((VendorA *)D_001E63C0)->idx].type * sz))->max) {
            m = 0x5233;
        } else {
            v = (VendorA *)D_001E63C0;
            if (v->slots[v->idx].kind == 1) {
                if (v->alt) {
                    price = ((WeaponInfoA *)(D_001DFFB0 + v->slots[v->idx].type * 0x18))->priceAlt;
                } else {
                    price = ((WeaponInfoA *)(D_001DFFB0 + v->slots[v->idx].type * 0x18))->price;
                }
                m = 0x5233;
                if (D_0015ED98 >= price)
                    m = 0x5234;
            } else {
                int cost;
                if (D_0013D4E3[0] != 0) {
                    cost = ((struct { WeaponInfoA a[1]; } *)D_001DFFB0)->a[v->slots[v->idx].type].second;
                } else {
                    cost = ((WeaponInfoA *)(D_001DFFB0 + v->slots[v->idx].type * 0x18))->first;
                }
                m = 0x5233;
                if (D_0015ED98 >= cost)
                    m = 0x524E;
            }
        }
    } else {
        m = 0x5234;
    }
    if (m != 0) {
        *(unsigned short *)(sp + 0x12) |= 4;
        func_001F75F0(sp, 0x80F0F0F0L, func_001FDD10(0x5234), -1);
        *(unsigned short *)(sp + 0x12) ^= 4;
        *(short *)(sp + 0xA) = (y - *(short *)(sp + 0xE)) >> 1;
        func_001F75F0(sp, 0x80F0F0F0L, func_001FDD10(0x5234), -1);
    }
}
#endif /* NON_MATCHING */
