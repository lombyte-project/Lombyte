#include "types.h"
#include "sda.h"

typedef struct {
    u8 pad_0[0x20];
    s32 w;
    u8 pad_24[0x1C];
    s32 sel;
} SaveMenu;

typedef struct {
    s32 id;
    s32 bolts;
    s32 count;
    s32 time;
    u8 pad_10[5];
    u8 b15;
    u8 b16;
    u8 b17;
    u8 pad_18[4];
} SaveSlot;

typedef struct {
    u8 pad_0[8];
    s32 state;
    u8 pad_C[0x14];
    SaveSlot slots[5];
    u8 pad_AC[0x28];
    s32 xD4;
    s32 pad_D8;
    s32 xDC;
} SaveInfo;

extern SaveInfo D_0013D290;
extern s32 D_0015ED80 MACRO_ADDR;
extern s32 D_001601B0 __attribute__((sda));
extern char D_001602A0[];
extern char D_001602D0[];
extern char D_00160300[];
extern char D_00160310[];
extern char D_00160320[];

extern void func_001F4280(s32);
extern void func_001F4398(void);
extern void func_001F65B0(s32, s32, u64, char *, s32);
extern void func_001F6B88(s32, s32, u64, char *, s32);
extern char *func_001FDD10(s32);
extern s32 FUN_001ff960(s32, s32);
extern void func_001FFC30(s32, s32, s32, s32, s32, s32);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern s32 sprintf(char *, const char *, ...);

s32 fun_002239e0(SaveMenu *m) __asm__("FUN_002239e0");

s32 fun_002239e0(SaveMenu *m) {
    char buf[0x50];
    SaveSlot *slot;
    s32 color;
    s32 w;
    s32 x0;
    s32 ya;
    s32 yb;
    s32 y0;
    s32 y1;
    s32 i;
    s32 y;
    s32 t;
    s32 h;
    s32 n;
    s32 m2;

    y = 4;
    func_001F4280(0);
    for (i = 0; i < 5; i++) {
        w = m->w;
        ya = y - 4;
        yb = y + 0x34;
        x0 = w - 3;
        y0 = y - 1;
        y1 = y + 0x31;
        if (D_0013D290.xD4 < 3 && D_0013D290.xDC < 0 && m->sel == i) {
            func_00200E08(0, ya, w, yb, 0x8020FFFF, 0);
            func_00200E08(3, y0, x0, y1, D_001601B0, 0);
        }
        func_00200E08(3, y0, x0, y1, 0x80303030, 0);
        slot = &D_0013D290.slots[i];
        if (D_0013D290.xD4 >= 3 || D_0013D290.xDC >= 0 || D_0013D290.state != 2) {
            y += 0x30;
        } else {
            if (slot->id == -1) {
                y += 0x10;
                func_001F6B88(m->w / 2, y, 0x80FFA888, func_001FDD10(0x5217), -1);
                y += 0x20;
            } else {
                t = slot->time;
                h = t / 216000;
                m2 = t / 3600 - h * 60;
                color = m->sel == i ? (s32)0x8020FFFF : (s32)0x80FFA888;
                if (h > 99) {
                    h = 99;
                }
                sprintf(buf, D_00160300, h, m2);
                func_001FFC30(FUN_001ff960(0xE99E, 3), 4, y, 0x10, 0x10, 0x80);
                func_001F65B0(0x16, y, color, buf, -1);
                if (slot->count != 0) {
                    func_001FFC30(FUN_001ff960(0xE99E, 4), 0x4E, y, 0x10, 0x10, 0x80);
                    sprintf(buf, D_001602A0, slot->count < 100 ? slot->count : 99);
                    func_001F65B0(0x60, y, color, buf, -1);
                }
                n = slot->bolts;
                if (n > 9999999) {
                    n = 9999999;
                }
                y += 0x10;
                if (n < 1000) {
                    sprintf(buf, D_001602A0, n);
                } else if (n < 1000000) {
                    sprintf(buf, D_001602D0, n / 1000, n % 1000);
                } else {
                    sprintf(buf, D_00160310, n / 1000000, n % 1000000 / 1000, n % 1000);
                }
                func_001FFC30(FUN_001ff960(0x754F, 0xF), 4, y, 0x10, 0x10, 0x80);
                func_001F65B0(0x16, y, color, buf, -1);
                y += 0x10;
                sprintf(buf, D_00160320, slot->b16, slot->b15, slot->b17);
                func_001FFC30(FUN_001ff960(0xE99E, 2), 4, y, 0x10, 0x10, 0x80);
                func_001F65B0(0x16, y, color, buf, -1);
                y += 0x10;
            }
        }
        if (D_0015ED80 != 0) {
            y += 0x24;
        } else {
            y += 0x1B;
        }
    }
    func_001F4398();
    return 2;
}

extern __typeof__(fun_002239e0) func_002239E0 __attribute__((alias("FUN_002239e0")));
