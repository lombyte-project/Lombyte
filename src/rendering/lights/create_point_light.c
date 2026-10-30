#include "types.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/rendering/shrub_instance.h"
#include "rnc/rendering/tie_instance.h"

typedef struct {
    f32 v[4];
} __attribute__((aligned(16))) Vec;

typedef struct {
    s16  tie_start;
    s16  tie_count;
    s16  shrub_start;
    s16  shrub_count;
    s16  tfrag_start;
    s16  tfrag_count;
    s16 *instance_indices;
    u8   pad[0x20];
} LightSlot;

typedef struct {
    u8  pad0[0x10];
    Vec sphere;
} LightDef;

typedef struct {
    u8  pad[0x36];
    u16 lights;
    u8  pad38[8];
} Obj40;

extern LightDef  point_lights[] __asm__("D_0019C1C0");
extern LightSlot point_light_data[] __asm__("D_0019C3C0");
extern Obj40    *D_00160E8C __attribute__((sda));
extern s32       D_00160E90;

extern s32  FUN_001f9bb0(void *, void *);
extern void scale_vector(void *, void *, f32) __asm__("FUN_001f9a80");

void create_point_light(s32 i) __asm__("FUN_00201ba8");

void create_point_light(s32 i) {
    LightSlot     *slot;
    LightDef      *def;
    s16           *instance_index_cursor;
    s16           *end;
    Vec            sphere;
    TieInstance   *tie;
    ShrubInstance *shrub;
    Obj40         *object_record_cursor;
    s32            tie_index;
    s32            tfrag_index;
    s32            shrub_index;
    u32            v;

    slot = &point_light_data[i];
    instance_index_cursor = slot->instance_indices;
    end = instance_index_cursor + 0x200;
    def = &point_lights[i];
    qcopy(&sphere, &def->sphere);
    sphere.v[3] += 8.0f;
    if (instance_index_cursor < end) {
        slot->tie_start = 0;
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
        slot->tie_count = instance_index_cursor - slot->instance_indices;
        if (instance_index_cursor < end) {
            slot->tfrag_start = slot->tie_count;
            scale_vector(&sphere, &sphere, 1024.0f);
            object_record_cursor = D_00160E8C;
            for (tfrag_index = 0; tfrag_index < D_00160E90; tfrag_index++, object_record_cursor++) {
                if (FUN_001f9bb0(&sphere, object_record_cursor)) {
                    *instance_index_cursor++ = tfrag_index;
                    v = object_record_cursor->lights;
                    if (v == 0xFFFF) {
                        object_record_cursor->lights = i | 0xFFF0;
                    } else if ((v & 0xFFF0) == 0xFFF0) {
                        object_record_cursor->lights = (v & 0xFF0F) | (i << 4);
                    } else if ((v & 0xFF00) == 0xFF00) {
                        object_record_cursor->lights = (v & 0xF0FF) | (i << 8);
                    } else if ((v & 0xF000) == 0xF000) {
                        object_record_cursor->lights = (v & 0x0FFF) | (i << 12);
                    } else {
                        instance_index_cursor--;
                    }
                    if (instance_index_cursor >= end) {
                        break;
                    }
                }
            }
            slot->tfrag_count =
                (instance_index_cursor - slot->instance_indices) - slot->tfrag_start;
            if (instance_index_cursor < end) {
                slot->shrub_start = instance_index_cursor - slot->instance_indices;
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
                slot->shrub_count =
                    (instance_index_cursor - slot->instance_indices) - slot->shrub_start;
            }
        }
    }
}

extern __typeof__(create_point_light) func_00201BA8 __attribute__((alias("FUN_00201ba8")));
