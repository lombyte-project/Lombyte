/* Level 18 thruster_pack_lock_green and its list helpers. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

#include "qcopy.h"

#include "sda.h"

extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D79F0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x10];
    int   i10;
    char  pad1[0x10];
    float f24;
} L18Cls_q;

typedef struct {
    char  pad0[0x10];
    float f10;
    char *p14;
    char *p18;
    char *p1C;
    float f20;
    float f24;
    float f28;
    float f2C;
    int   i30;
    float f34;
    int   i38;
} L18E_q;

extern char *D_L18_00174858_lui __asm__("D_L18_00174858") NOT_SDA;
extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern int  D_L18_0015F5C4_d __asm__("D_L18_0015F5C4");
extern int  D_L18_0016D290_lui __asm__("D_L18_0016D290") NOT_SDA;
extern void FUN_001f9bf8_q(void *, void *, float) __asm__("FUN_001f9bf8");
extern void update_entity(void *) __asm__("FUN_L00_00250df8");
f32         probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

extern float D_L18_00161998 __attribute__((sda));

#include "rnc/overlay/l18/mobys/thruster_pack_lock_green_globals.h"

extern float thruster_pack_lock_green_active_height_offset_gp __asm__("D_L18_0016199C")
    __attribute__((sda));
extern float thruster_pack_lock_green_rest_height_offset_gp __asm__("D_L18_001619A0")
    __attribute__((sda));
void thruster_pack_lock_green_update(char *moby) __asm__("FUN_L18_002d6600");

void thruster_pack_lock_green_update(char *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_UPDATE_LOCK_COMPONENTS = 1
    };
    L18E_q *moby_vars = ((L18E_q *)MOBY(moby)->pvars);
    MOBY(moby)->scale = (((L18Cls_q *)MOBY(moby)->pclass))->f24 * D_L18_00161998;
    switch (((unsigned char *)moby)[0x20]) {
    case STATE_INITIALIZE: {
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + 1.0f;
        MOBY(moby)->collision = 0;
        MOBY(moby)->pos.z = probe_ground_height(moby + 0x10, 0, 0.5f);
        qcopy(moby_vars, moby + 0x10);
        if (D_L18_00174858_lui != 0) {
            moby_vars->i38 = 1;
        }
        {
            char *spawned_moby = create_moby(0x6EB);
            spawned_moby[0x30] = 0x40;
            MOBY(spawned_moby)->draw_dist = 0x56;
            spawned_moby[0x31] = 1;
            MOBY(spawned_moby)->draw_dist = 0x56;
            MOBY(spawned_moby)->scale =
                (((L18Cls_q *)MOBY(spawned_moby)->pclass))->f24 * D_L18_00161998;
            qcopy(spawned_moby + 0x10, moby_vars);
            qcopy(spawned_moby + 0x40, moby + 0x40);
            update_entity(spawned_moby);
            moby_vars->p1C = spawned_moby;
        }
        moby_vars->p14 = create_moby(0x6F0);
        moby_vars->p14[0x30] = 0x40;
        *(short *)(moby_vars->p14 + 0x32) = 0x56;
        moby_vars->p14[0x31] = 1;
        *(float *)(moby_vars->p14 + 0x2C) =
            (*(L18Cls_q **)(moby_vars->p14 + 0x24))->f24 * D_L18_00161998;
        {
            char *q = moby_vars->p14;
            qcopy(q + 0x40, moby + 0x40);
            update_entity(q);
        }
        moby_vars->p18 = create_moby(0x6F0);
        moby_vars->p18[0x30] = 0x40;
        *(short *)(moby_vars->p18 + 0x32) = 0x40;
        moby_vars->p18[0x31] = 1;
        *(float *)(moby_vars->p18 + 0x2C) =
            (*(L18Cls_q **)(moby_vars->p18 + 0x24))->f24 * D_L18_00161998;
        {
            char *q = moby_vars->p18;
            qcopy(q + 0x40, moby + 0x40);
            *(float *)(moby_vars->p18 + 0x48) =
                fast_add_rotations(*(float *)(q + 0x48), 3.1415901f);
            update_entity(moby_vars->p18);
        }
        ((unsigned char *)moby)[0x20] = 1;
        {
            float base_height = MOBY(moby)->pos.z;
            float u = thruster_pack_lock_green_rest_height_offset_gp;
            moby_vars->f10 = base_height;
            moby_vars->f34 = u;
        }
        MOBY(moby)->collision = (((L18Cls_q *)MOBY(moby)->pclass))->i10;
        break;
    }
    case STATE_UPDATE_LOCK_COMPONENTS: {
        float v[4];
        if (D_L18_0015F5C4_d == 2) {
            if (moby_vars->i38 != 0) {
                if (D_L18_0016D290_lui == 3) {
                    if (moby_vars->p14 != 0)
                        mark_moby_for_removal(moby_vars->p14);
                    if (moby_vars->p18 != 0)
                        mark_moby_for_removal(moby_vars->p18);
                    if (moby_vars->p1C != 0)
                        mark_moby_for_removal(moby_vars->p1C);
                    mark_moby_for_removal(moby);
                }
            }
        }
        if (moby_vars->f24 != 0.0f) {
            if ((moby_vars->i30 == 0 && fast_dec_timer((int *)&moby_vars->f2C) == 2) ||
                (D_L18_0015F5C4_d == 2 && D_L18_0016D290_lui >= 4)) {
                allocate_voice_for_target_entry(0, 0, (int)moby);
                moby_vars->f24 = 0;
            }
            {
                struct Hero *hero_ptr = &hero;
                if (hero_ptr->air_frames.s == 0 && hero_ptr->state.current == 0x22) {
                    allocate_voice_for_target_entry(0, 0, (int)moby);
                    moby_vars->f24 = 0;
                }
            }
        }
        if (moby_vars->f24 != 0.0f) {
            if (moby_vars->f20 != moby_vars->f24) {
                advance_accelerated_scalar(&moby_vars->f20,
                                           &moby_vars->f28,
                                           moby_vars->f24,
                                           frame_time_sq * 4.0f,
                                           frame_time_sq * 4.0f,
                                           frame_time * 4.0f);
            } else {
                float k = frame_time_sq;
                advance_accelerated_scalar(&moby_vars->f34,
                                           &moby_vars->f28,
                                           thruster_pack_lock_green_active_height_offset_gp,
                                           k * 16.0f,
                                           k * 4.0f,
                                           frame_time * 8.0f);
            }
        } else if (moby_vars->f34 != thruster_pack_lock_green_rest_height_offset_gp) {
            float k = frame_time_sq;
            advance_accelerated_scalar(&moby_vars->f34,
                                       &moby_vars->f28,
                                       thruster_pack_lock_green_rest_height_offset_gp,
                                       k * 16.0f,
                                       k * 4.0f,
                                       frame_time * 8.0f);
        } else {
            advance_accelerated_scalar(&moby_vars->f20,
                                       &moby_vars->f28,
                                       moby_vars->f24,
                                       frame_time_sq * 4.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time * 4.0f);
        }
        if (moby_vars->p14 != 0) {
            FUN_001f9bf8_q(moby_vars->p14 + 0x10, moby_vars->p14 + 0xD0, -moby_vars->f20);
            FUN_001f9bf8_q(v, moby_vars->p14 + 0xE0, -moby_vars->f20 * 0.2f);
            fast_vec_add(moby_vars->p14 + 0x10, moby_vars->p14 + 0x10, v);
            fast_vec_add(moby_vars->p14 + 0x10, moby_vars->p14 + 0x10, moby_vars);
            update_entity(moby_vars->p14);
        }
        if (moby_vars->p18 != 0) {
            FUN_001f9bf8_q(moby_vars->p18 + 0x10, moby_vars->p18 + 0xD0, -moby_vars->f20);
            FUN_001f9bf8_q(v, moby_vars->p18 + 0xE0, -moby_vars->f20 * 0.2f);
            fast_vec_add(moby_vars->p18 + 0x10, moby_vars->p18 + 0x10, v);
            fast_vec_add(moby_vars->p18 + 0x10, moby_vars->p18 + 0x10, moby_vars);
            update_entity(moby_vars->p18);
        }
        MOBY(moby)->pos.z = moby_vars->f10 + moby_vars->f34;
    }
    }
}

extern short D_L18_0015FFD8_d __asm__("D_L18_0015FFD8");
extern int   D_L18_0015FFD8; /* no foreign declaration */

typedef struct {
    char  pad0[0x10];
    float x;
    float y;
} L18Pos;

extern int *D_L18_001AC240[];

void *
find_class_247_moby_with_largest_xy_angle_in_list(float *reference_position,
                                                  float *comparison_position,
                                                  int moby_list_index) __asm__("FUN_L18_002d6b58");

void *find_class_247_moby_with_largest_xy_angle_in_list(float *reference_position,
                                                        float *comparison_position,

                                                        int moby_list_index) {
    short *moby_list_entry = (short *)D_L18_001AC240[moby_list_index];
    char  *best = 0;
    float  bestd = 0.0f;
    if (moby_list_entry == 0)
        return 0;
    while (1) {
        int moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
        if (*(short *)(moby_byte_offset + (int)*(char **)&D_L18_0015FFD8_d + 0xA6) == 0x247) {
            float comparison_to_reference_angle =
                fast_arc_tan(reference_position[0] - comparison_position[0],
                             reference_position[1] - comparison_position[1]);
            L18Pos *candidate_moby =
                (L18Pos *)(moby_byte_offset + (int)*(char **)&D_L18_0015FFD8_d);
            float d = fast_difference_between_rotations(
                comparison_to_reference_angle,
                fast_arc_tan(reference_position[0] - candidate_moby->x,
                             reference_position[1] - candidate_moby->y));
            if (bestd < d) {
                bestd = d;
                best = (char *)D_L18_0015FFD8 + moby_byte_offset;
            }
        }
        if (*moby_list_entry++ < 0)
            return best;
    }
}
/* Of the mobys of class 0x247 in list idx, the one nearest to pos (within
   255 units), or 0. The second spelling of the table base keeps the
   address out of the loop's $gp-relative reload in the bc1f delay slot. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D8070), where it is exact; names translated to the US level program. */

extern int *D_L18_001AC240_c[] __asm__("D_L18_001AC240");

char *find_nearest_class_247_moby_in_list(void *reference_position,
                                          int   moby_list_index) __asm__("FUN_L18_002d6c80");

char *find_nearest_class_247_moby_in_list(void *reference_position, int moby_list_index) {
    short *moby_list_entry = (short *)D_L18_001AC240_c[moby_list_index];
    char  *best = 0;
    float  bestd = 255.0f;
    if (moby_list_entry == 0)
        return 0;
    while (1) {
        int   moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
        char *candidate_moby = (char *)(moby_byte_offset + (int)D_L18_0015FFD8);
        if (MOBY(candidate_moby)->oclass == 0x247) {
            float candidate_distance = fast_vec_dist(reference_position, candidate_moby + 0x10);
            if (candidate_distance < bestd) {
                bestd = candidate_distance;
                best = D_L18_0015FFD8 + moby_byte_offset;
            }
        }
        if (*moby_list_entry++ < 0)
            return best;
    }
}

extern int D_L18_001AC240_u[] __asm__("D_L18_001AC240");
/* 0x002d6d50, 108 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Sets the data fields 0x30 and 0x24 of each listed moby whose class is 0x247.
void set_thruster_pack_lock_green_list_fields(int moby_list_index) __asm__("FUN_L18_002d6d50");

void set_thruster_pack_lock_green_list_fields(int moby_list_index) {
    short *moby_list_entry = (short *)D_L18_001AC240_u[moby_list_index];
    if (moby_list_entry != 0) {
        char *base = *(char **)&D_L18_0015FFD8_d;
        do {
            int   moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
            char *moby = (char *)moby_byte_offset + (int)base;
            if (MOBY(moby)->oclass == 0x247) {
                char *data = ((char *)MOBY(moby)->pvars);
                *(int *)(data + 0x30) = 1;
                *(float *)(data + 0x24) = 1.5f;
            }
        } while (*moby_list_entry++ >= 0);
    }
}

/* Initialize the vendor action mode and scale. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D81B0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x24];
    float scale;
    char  pad28[4];
    int   mode;
} Level18VendorActionData;

typedef struct {
    char                     pad0[0x78];
    Level18VendorActionData *data;
} Level18VendorActionMoby;

/* Switches the data to mode 5 at scale 1.5. */
void moby_init_action_mode(Level18VendorActionMoby *moby) __asm__("FUN_L18_002d6dc0");

void moby_init_action_mode(Level18VendorActionMoby *moby) {
    Level18VendorActionData *data = moby->data;
    data->mode = 5;
    data->scale = 1.5f;
}
