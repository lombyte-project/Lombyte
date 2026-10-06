#include "types.h"
#include "sda.h"
#include "qcopy.h"

typedef struct {
    f32 v[4];
} __attribute__((aligned(16))) Vec;

typedef struct {
    s16 n0;
    s16 c0;
    s16 n2;
    s16 c2;
    s16 n1;
    s16 c1;
    s16 *buf;
    u8 pad[0x20];
} LightSlot;

typedef struct {
    u8 pad0[0x10];
    Vec sphere;
} LightDef;

typedef struct {
    u8 pad[0x1E];
    u16 lights;
} Obj20;

typedef struct {
    u8 pad[0x36];
    u16 lights;
    u8 pad38[8];
} Obj40;

extern LightDef D_0019C1C0[];
extern LightSlot D_0019C3C0[];
extern Obj20 *D_00160F50;
extern Obj20 *D_00160F54;
extern Obj40 *D_00160E8C __attribute__((sda));
extern s32 D_00160E90;
extern Obj20 *D_001603D4;
extern Obj20 *D_001603D8;

extern s32 FUN_001f9bb0(void *, void *);
extern void FUN_001f9a80(void *, void *, f32);

void create_point_light(s32 i) __asm__("FUN_00201ba8");

void create_point_light(s32 i) {
    LightSlot *slot;
    LightDef *def;
    s16 *p;
    s16 *end;
    Vec sphere;
    Obj20 *o;
    Obj40 *m;
    s32 n;
    s32 n2;
    s32 n3;
    u32 v;

    slot = &D_0019C3C0[i];
    p = slot->buf;
    end = p + 0x200;
    def = &D_0019C1C0[i];
    qcopy(&sphere, &def->sphere);
    sphere.v[3] += 8.0f;
    if (p < end) {
        slot->n0 = 0;
        n = 0;
        for (o = D_00160F50; o != D_00160F54; o++, n++) {
            if (FUN_001f9bb0(&sphere, o)) {
                *p++ = n;
                v = o->lights;
                if (v == 0xFFFF) {
                    o->lights = i | 0xFFF0;
                } else if ((v & 0xFFF0) == 0xFFF0) {
                    o->lights = (v & 0xFF0F) | (i << 4);
                } else if ((v & 0xFF00) == 0xFF00) {
                    o->lights = (v & 0xF0FF) | (i << 8);
                } else if ((v & 0xF000) == 0xF000) {
                    o->lights = (v & 0x0FFF) | (i << 12);
                } else {
                    p--;
                }
                if (p >= end) {
                    break;
                }
            }
        }
        slot->c0 = p - slot->buf;
        if (p < end) {
            slot->n1 = slot->c0;
            FUN_001f9a80(&sphere, &sphere, 1024.0f);
            m = D_00160E8C;
            for (n2 = 0; n2 < D_00160E90; n2++, m++) {
                if (FUN_001f9bb0(&sphere, m)) {
                    *p++ = n2;
                    v = m->lights;
                    if (v == 0xFFFF) {
                        m->lights = i | 0xFFF0;
                    } else if ((v & 0xFFF0) == 0xFFF0) {
                        m->lights = (v & 0xFF0F) | (i << 4);
                    } else if ((v & 0xFF00) == 0xFF00) {
                        m->lights = (v & 0xF0FF) | (i << 8);
                    } else if ((v & 0xF000) == 0xF000) {
                        m->lights = (v & 0x0FFF) | (i << 12);
                    } else {
                        p--;
                    }
                    if (p >= end) {
                        break;
                    }
                }
            }
            slot->c1 = (p - slot->buf) - slot->n1;
            if (p < end) {
                slot->n2 = p - slot->buf;
                n3 = 0;
                for (o = D_001603D4; o != D_001603D8; o++, n3++) {
                    if (FUN_001f9bb0(&sphere, o)) {
                        *p++ = n3;
                        v = o->lights;
                        if (v == 0xFFFF) {
                            o->lights = i | 0xFFF0;
                        } else if ((v & 0xFFF0) == 0xFFF0) {
                            o->lights = (v & 0xFF0F) | (i << 4);
                        } else if ((v & 0xFF00) == 0xFF00) {
                            o->lights = (v & 0xF0FF) | (i << 8);
                        } else if ((v & 0xF000) == 0xF000) {
                            o->lights = (v & 0x0FFF) | (i << 12);
                        } else {
                            p--;
                        }
                        if (p >= end) {
                            break;
                        }
                    }
                }
                slot->c2 = (p - slot->buf) - slot->n2;
            }
        }
    }
}

extern __typeof__(create_point_light) func_00201BA8 __attribute__((alias("FUN_00201ba8")));
