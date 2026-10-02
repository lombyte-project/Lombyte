#include "types.h"

/* Draws the pause menu's ring of 8 icons and the quick-select overlay.
   arg0 is the pause-state object: unk20/unk24 give the viewport
   width/height (used for the ring's center and radius), unk50 the
   selected slot (drawn with a pulsing highlight box), and slots[i]
   indexes an icon-info table (D_001863D0, 0x4C bytes/entry) and a byte
   flags table (D_0013E520) when nonzero. */

struct IconEnt {
    u8 pad[0x38];
    u16 id;
    u8 pad3A[0x12];
};

struct PauseState {
    u8 pad00[0x20];
    s32 w;
    s32 h;
    u8 pad28[8];
    s32 slots[8];
    s32 sel;
};

extern s32 D_0015F438;
extern s32 D_001601B0 __attribute__((sda));
extern u8 D_0013E520[];
extern u8 D_001602D8[];
extern u8 D_001602E0[];
extern u8 D_001863D0[];
extern s32 SubtractIntegerWithClamp(s32);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 func_001FA610(f32);
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern void func_001F6530(s32, s32, u64, s32, s32);
extern s32 func_001FF960(s32, s32);
extern void func_001FFC30(s32, s32, s32, s32, s32, s32);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);

s32 draw_pause_ring(struct PauseState *arg0) __asm__("FUN_00220b20");

s32 draw_pause_ring(struct PauseState *arg0) {
    s32 i;
    s32 flag;
    f32 cx, cy, radius;
    struct IconEnt *iconTab;
    s32 v1, v0;

    func_001F4280(0);
    v1 = arg0->w;
    iconTab = D_001863D0;
    v0 = arg0->h;
    flag = v1 < v0;
    /* v1 doubles as `selected`: retail's movz reuses the same register
       that already holds v1 (loaded straight into it), so the C must
       overwrite v1 itself rather than assign a fresh local -- that
       fresh local costs an extra `move` before the conditional one. */
    cx = (f32)v1 * 0.5f;
    cy = (f32)v0 * 0.5f;
    if (flag == 0) {
        v1 = v0;
    }
    radius = (f32)v1 * 0.5f - 40.0f;
    i = 0;
    do {
        f32 t, x, y;

        t = func_001FA610((f32)i * 0.7853982f + -1.5707964f);
        x = cx + fast_cos(t) * radius;
        y = cy + fast_sin(t) * radius;
        if (i == arg0->sel) {
            u64 color = ((SubtractIntegerWithClamp((D_0015F438 & 0x3F) - 0x20) + 0x40)
                         * 0x10202) | 0x80000000;

            func_00200E08((s32)x - 0x13, (s32)y - 0x13, (s32)x + 0x13, (s32)y + 0x13,
                          color, 0);
            func_00200E08((s32)x - 0x12, (s32)y - 0x12, (s32)x + 0x12, (s32)y + 0x12,
                          D_001601B0, 0);
        }
        if (arg0->slots[i] == 0) {
            func_00200E08((s32)x - 0xF, (s32)y - 0xF, (s32)x + 0xF, (s32)y + 0xF,
                          0x40404040L, 0);
        } else {
            s32 idx = arg0->slots[i];
            s32 id = func_001FF960(iconTab[idx].id, D_0013E520[idx] ? 4 : 0);

            func_001FFC30(id, (s32)x - 0x11, (s32)y - 0x11, 0x20, 0x20, 0x80);
        }
        i += 1;
    } while (i < 8);
    func_001FFC30(func_001FF960(0xE99E, 0), 8, 0x27, 0x20, -0x20, 0x80);
    func_001FFC30(func_001FF960(0xE99E, 0), arg0->w - 0xA, 0x27, -0x20, -0x20, 0x80);
    func_001F6530(0x28, 0xF, 0x80FFA888L, D_001602D8, -1);
    func_001F6530(arg0->w - 0x3C, 0xF, 0x80FFA888L, D_001602E0, -1);
    func_001F4398();
    return 2;
}

extern __typeof__(draw_pause_ring) func_00220B20 __attribute__((alias("FUN_00220b20")));
