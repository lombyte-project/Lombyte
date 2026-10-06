#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"

#include "rnc/gameplay/entities/moby.h"

struct Player {
    u8 pad0[0x80];
    Vec4 pos;
    u8 pad90[0x8];
    f32 unk98;
    u8 pad9C[0x1F4];
    Vec4 unk290;
    u8 pad2A0[0x5C];
    struct Moby *unk2FC;
    u8 pad300[0x1D84];
    s32 unk2084;
    u8 pad2088[0x1FC];
    s32 unk2284;
};

struct CamColl {
    Vec4 pos;
    f32 vel;
    u8 pad14[0xC];
    Vec4 dir;
    Vec4 unk30;
    Vec4 unk40;
    f32 dir_vel[4];
    Vec4 unk60;
    Vec4 unk70;
    Vec4 unk80;
    Vec4 unk90;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 hist[5];
    u8 padC0[0x14];
    struct Moby *unkD4;
    f32 unkD8;
    f32 unkDC;
};

extern struct Player D_0013F350;
extern struct CamColl D_001870D0;

extern f32 cam_interp_values(f32 *vel, f32 from, f32 to, f32 stiffness, f32 damping,
                             f32 max) __asm__("FUN_001ebd78");
extern float AbsoluteFloat(float input) __asm__("func_001F99C0");
extern void FUN_001f9a28(void *out, void *a, void *b);
extern void FUN_001f9a68(void *out, void *a, f32 s);
extern f32 FUN_001f9ab0(void *a, void *b);
extern f32 FUN_001f9af0(void *a);
extern void FUN_001f9bf8(void *out, void *a, f32 len);

void FUN_001ed470(void) {
    Vec4 dir;
    Vec4 proj;
    struct CamColl *cam;
    f32 d;
    s32 i;
    struct Moby *m;

    cam = &D_001870D0;
    FUN_001f9bf8(&dir, &D_0013F350.unk290, -1.0f);
    qcopy(&cam->unk40, &cam->unk30);
    qcopy(&cam->unk30, &dir);
    if (FUN_001f9ab0(&cam->dir, &dir) < -0.98f) {
        dir.f[0] += 0.2f;
        dir.f[1] += 0.2f;
        dir.f[2] += 0.2f;
    }
    cam->dir.f[0] =
        cam_interp_values(&cam->dir_vel[0], cam->dir.f[0], dir.f[0], 0.015f, 0.2f, 0.0f);
    cam->dir.f[1] =
        cam_interp_values(&cam->dir_vel[1], cam->dir.f[1], dir.f[1], 0.015f, 0.2f, 0.0f);
    cam->dir.f[2] =
        cam_interp_values(&cam->dir_vel[2], cam->dir.f[2], dir.f[2], 0.015f, 0.2f, 0.0f);
    FUN_001f9bf8(&cam->dir, &cam->dir, 1.0f);

    FUN_001f9a28(&cam->unk70, &D_0013F350.pos, &cam->unk60);
    cam->unkA0 = FUN_001f9af0(&cam->unk70);
    d = FUN_001f9ab0(&cam->unk70, &dir);
    cam->unkA8 = d;
    FUN_001f9bf8(&proj, &dir, d);
    qcopy(&cam->unk90, &proj);
    FUN_001f9a28(&cam->unk80, &cam->unk70, &proj);
    cam->unkA4 = FUN_001f9af0(&cam->unk80);
    FUN_001f9a68(&cam->unk80, &cam->unk80, 1.0f / cam->unkA4);
    qcopy(&cam->unk60, &D_0013F350.pos);

    if (D_0013F350.unk2284 != 0x50 || D_0013F350.unk2084 == 0x11) {
        cam->pos.f[0] = D_0013F350.pos.f[0];
        cam->pos.f[1] = D_0013F350.pos.f[1];
        cam->pos.f[2] =
            cam_interp_values(&cam->vel, cam->pos.f[2], D_0013F350.pos.f[2], 0.0075f, 0.175f, 0.0f);
        cam->pos.f[3] = D_0013F350.pos.f[2];
    } else {
        cam->pos.f[0] = D_0013F350.pos.f[0];
        cam->pos.f[1] = D_0013F350.pos.f[1];
    }

    for (i = 0; i < 4; i++) {
        cam->hist[i] = cam->hist[i + 1];
    }
    cam->hist[i] = D_0013F350.unk98;

    m = D_0013F350.unk2FC;
    if (m != NULL && m->oclass != 0x4BA && m->oclass != 0x336) {
        if (m == cam->unkD4) {
            cam->unkDC = m->pos.z - cam->unkD8;
            if (AbsoluteFloat(cam->unkDC) < 0.001f) {
                cam->unkDC = 0.0f;
            }
            cam->unkD8 = cam->unkD4->pos.z;
        } else {
            cam->unkD4 = m;
            cam->unkDC = 0.0f;
            cam->unkD8 = m->pos.z;
        }
    } else {
        cam->unkDC = 0.0f;
        cam->unkD4 = NULL;
        cam->unkD8 = D_0013F350.pos.f[2];
    }
}

extern __typeof__(FUN_001ed470) func_001ED470 __attribute__((alias("FUN_001ed470")));
