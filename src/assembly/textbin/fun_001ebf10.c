#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001ebf10/FUN_001ebf10.s", FUN_001ebf10);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

typedef struct Cam {
    u128 m0;
    u128 m1;
    u128 m2;
    Vec4 pos;
    u8 pad40[0x24];
    f32 tgt[3];
    void *buf;
    u8 pad74[4];
    f32 speed;
    u8 pad7C;
    u8 unk7D;
    s16 mode;
    u8 pad80[4];
    s16 idx;
    u8 pad86[8];
    s16 unk8E;
} Cam;

typedef struct {
    u8 pad0[0x1D];
    u8 kind;
} CamInfo;

typedef struct {
    u8 pad0[0x1C];
    CamInfo *info;
} CamSlot;

struct CamState {
    u8 pad0[0x140];
    u128 unk140;
    u8 pad150[0x30];
    Cam *cur;
    Cam *prev;
    u8 pad188[0xE8];
    s16 unk270;
    u8 pad272;
    u8 unk273;
    u8 pad274[0x14];
    f32 unk288;
    u8 pad28C[8];
    f32 unk294;
    u8 pad298[0x5C];
    s32 unk2F4;
    u8 pad2F8[0xA0];
    s32 unk398;
};

extern struct CamState D_00186F40;
extern CamSlot *D_0015EF90;
extern s32 D_0015ED84;
extern u8 D_00189650[];
extern s32 D_0018C32C;

extern void func_001EBC90(void);
extern void func_001EBEC8(Cam *cam);
extern void func_001F98D0(void *dst, void *src, s32 size);
extern s32 func_001FA6D0(f32 speed);

void FUN_001ebf10(Cam *cam) {

    Cam *old;
    CamInfo *info;
    f32 speed;
    f32 *t;
    Vec4 *src;
    s32 kind = 0;
    s32 mode;

    info = D_0015EF90[cam->idx].info;
    old = D_00186F40.cur;
    mode = old->mode;
    if (info != 0) {
        kind = info->kind;
    }
    if (mode == 4) {
        cam->unk8E = 1;
        src = &cam->pos;
    } else if (mode == 2 || kind == 1 || kind == 5) {
        if (kind == 1) {
            speed = cam->speed;
            D_00186F40.unk273 = 0;
            if (speed > 0.0f) {
                D_00186F40.unk288 = speed;
                D_00186F40.unk294 = speed;
            } else {
                D_00186F40.unk294 = 0.018f;
                D_00186F40.unk288 = 0.018f;
            }
        } else if (kind == 5) {
            speed = cam->speed;
            D_00186F40.unk273 = 2;
            if (speed > 0.0f) {
                D_00186F40.unk2F4 = func_001FA6D0(speed);
            } else {
                D_00186F40.unk2F4 = 40;
            }
        }
        if (D_00186F40.unk270 == 0) {
            D_00186F40.unk270 = 1;
        } else {
            D_00186F40.unk270 = 2;
        }
        src = &cam->pos;
    } else if (mode == 3 || mode == 5 || kind == 3 || kind == 6) {
        qcopy(&cam->pos, &old->pos);
        qcopy(&cam->m0, &old->m0);
        qcopy(&cam->m1, &old->m1);
        qcopy(&cam->m2, &old->m2);
        cam->unk7D = 2;
        src = &cam->pos;
        if (old->mode == 5 || kind == 6) {
            speed = cam->speed;
            D_00186F40.unk273 = 0;
            if (speed > 0.0f) {
                D_00186F40.unk288 = speed;
                D_00186F40.unk294 = speed;
            } else {
                D_00186F40.unk294 = 0.018f;
                D_00186F40.unk288 = 0.018f;
                if (D_0015ED84 == 1) {
                    D_00186F40.unk294 = 0.01f;
                    D_00186F40.unk288 = 0.01f;
                }
            }
            if (D_00186F40.unk270 == 0) {
                D_00186F40.unk270 = 1;
            } else {
                D_00186F40.unk270 = 2;
            }
        }
    } else {
        cam->unk8E = 1;
        src = &cam->pos;
    }

    old->mode = 0;
    old->unk7D = 0;
    old->unk8E = 0;
    D_00186F40.prev = old;
    func_001F98D0(D_00189650, D_00189650 - 0x280, 0x280);
    D_00186F40.prev->buf = D_00189650;
    D_00186F40.cur = cam;
    cam->buf = D_00189650 - 0x280;
    D_00186F40.unk398 = 0;
    func_001EBEC8(cam);
    func_001EBC90();
    if (D_0018C32C == 0) {
        qcopy(&D_00186F40.unk140, src);
    }
    t = cam->tgt;
    t[0] = cam->pos.x;
    t[1] = src->y;
    t[2] = src->z;
}
#endif /* NON_MATCHING */
