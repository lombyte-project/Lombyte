#include "types.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/rendering/shrub_instance.h"
#include "rnc/rendering/tie_instance.h"

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
    u8 pad[0x36];
    u16 lights;
    u8 pad38[8];
} Obj40;

extern LightDef point_lights[] __asm__("D_0019C1C0");
extern LightSlot point_light_data[] __asm__("D_0019C3C0");
extern Obj40 *D_00160E8C __attribute__((sda));
extern s32 D_00160E90;

extern s32 FUN_001f9bb0(void *, void *);
extern void scale_vector(void *, void *, f32) __asm__("FUN_001f9a80");

void create_point_light(s32 i) __asm__("FUN_00201ba8");

void create_point_light(s32 i) {
    LightSlot *slot;
    LightDef *def;
    s16 *instance_index_cursor;
    s16 *end;
    Vec sphere;
    TieInstance *tie;
    ShrubInstance *shrub;
    Obj40 *m;
    s32 tie_index;
    s32 n2;
    s32 shrub_index;
    u32 v;

    slot = &point_light_data[i];
    instance_index_cursor = slot->buf;
    end = instance_index_cursor + 0x200;
    def = &point_lights[i];
    qcopy(&sphere, &def->sphere);
    sphere.v[3] += 8.0f;
    if (instance_index_cursor < end) {
        slot->n0 = 0;
        tie_index = 0;
        for (tie = ties; tie != ties_end; tie++, tie_index++) {
            if (FUN_001f9bb0(&sphere, tie)) {
                *instance_index_cursor++ = tie_index;
                v = tie->lights;
                if (v == 0xFFFF) {
                    tie->lights = i | 0xFFF0;
                } else if ((v & 0xFFF0) == 0xFFF0) {
                    tie->lights = (v & 0xFF0F) | (i << 4);
                } else if ((v & 0xFF00) == 0xFF00) {
                    tie->lights = (v & 0xF0FF) | (i << 8);
                } else if ((v & 0xF000) == 0xF000) {
                    tie->lights = (v & 0x0FFF) | (i << 12);
                } else {
                    instance_index_cursor--;
                }
                if (instance_index_cursor >= end) {
                    break;
                }
            }
        }
        slot->c0 = instance_index_cursor - slot->buf;
        if (instance_index_cursor < end) {
            slot->n1 = slot->c0;
            scale_vector(&sphere, &sphere, 1024.0f);
            m = D_00160E8C;
            for (n2 = 0; n2 < D_00160E90; n2++, m++) {
                if (FUN_001f9bb0(&sphere, m)) {
                    *instance_index_cursor++ = n2;
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
                        instance_index_cursor--;
                    }
                    if (instance_index_cursor >= end) {
                        break;
                    }
                }
            }
            slot->c1 = (instance_index_cursor - slot->buf) - slot->n1;
            if (instance_index_cursor < end) {
                slot->n2 = instance_index_cursor - slot->buf;
                shrub_index = 0;
                for (shrub = shrubs; shrub != shrubs_end; shrub++, shrub_index++) {
                    if (FUN_001f9bb0(&sphere, shrub)) {
                        *instance_index_cursor++ = shrub_index;
                        v = shrub->lights;
                        if (v == 0xFFFF) {
                            shrub->lights = i | 0xFFF0;
                        } else if ((v & 0xFFF0) == 0xFFF0) {
                            shrub->lights = (v & 0xFF0F) | (i << 4);
                        } else if ((v & 0xFF00) == 0xFF00) {
                            shrub->lights = (v & 0xF0FF) | (i << 8);
                        } else if ((v & 0xF000) == 0xF000) {
                            shrub->lights = (v & 0x0FFF) | (i << 12);
                        } else {
                            instance_index_cursor--;
                        }
                        if (instance_index_cursor >= end) {
                            break;
                        }
                    }
                }
                slot->c2 = (instance_index_cursor - slot->buf) - slot->n2;
            }
        }
    }
}

extern __typeof__(create_point_light) func_00201BA8 __attribute__((alias("FUN_00201ba8")));
