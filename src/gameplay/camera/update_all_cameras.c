#include "types.h"
typedef struct { f32 x; f32 y; f32 z; } Vec3;
struct Camera { u8 pad0[0x30]; Vec3 pos; u8 pad3C[0x28]; Vec3 prev_pos; u8 pad70[0x1C]; s16 mode; u8 pad8E[0x12]; };
struct CamMode { u8 pad0[0xC]; void (*update)(struct Camera *); u8 pad10[4]; };
extern struct Camera *D_001870C0[];
extern struct Camera D_00187410[];
extern s32 D_00189B50[];
extern struct CamMode D_001E8C00[];
extern void func_001EC3D8(struct Camera *);
extern s32 func_001EC210(struct Camera *, struct Camera *);
extern void func_001EBF10(struct Camera *);
extern void func_001EBE68(struct Camera *);
extern void func_001EBCF0(void);
s32 update_all_cameras(void) __asm__("FUN_001ec420");

s32 update_all_cameras(void) {
    struct Camera *best;
    s32 changed;
    s32 i;
    void (*update)(struct Camera *);
    Vec3 *src;
    Vec3 *dst;

    best = D_001870C0[0];
    changed = 0;
    func_001EC3D8(best);
    for (i = 0; i < 48; i++) {
        if (D_00189B50[i] != 0 && &D_00187410[i] != best && func_001EC210(&D_00187410[i], best) != 0) {
            best = &D_00187410[i];
            changed = 1;
        }
    }
    if (changed) {
        func_001EBF10(best);
    }
    update = D_001E8C00[best->mode].update;
    func_001EBE68(best);
    if (update != 0) {
        update(best);
    }
    src = &best->pos;
    dst = &best->prev_pos;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    func_001EBCF0();
    return -1;
}

extern __typeof__(update_all_cameras) func_001EC420 __attribute__((alias("FUN_001ec420")));
