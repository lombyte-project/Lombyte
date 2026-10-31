/* Level 18 class 0x565: a linked moby group that moves to a target transform and falls, and the boss helpers that drive it. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

extern s32  random_integer_below(s32) __asm__("FUN_00213260");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern short         *D_L18_001AC240[];
extern unsigned char *D_L18_0015FFD8;
extern float          random_float_between(float, float) __asm__("FUN_002132a8");

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F2AE0), where it is exact; names translated to the US level program. */

typedef struct {
    int  idx;
    char pad[0x2C];
} VEnt;

typedef struct {
    int   pad0;
    int   i4;
    int   i8;
    char  padC[4];
    char  v10[0x10];
    char  v20[0x10];
    char  pad30[0x20];
    VEnt  ent[15];
    char  pad320[0x10];
    float f330;
    float f334;
    int   cnt;
    char  pad33C[4];
    float f340;
    float f344;
    int   f348;
    int   f34C;
    int   f350;
} VData;

extern char *D_L18_001600EC;

#include "rnc/overlay/l18/mobys/falling_moby_group_data/update_linked_moby_group_transform_and_fall_2.h"

extern float D_L18_001677D8 NOT_SDA;
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float fast_add_rotations(float, float);
extern float fast_cos(float);
extern float fast_sin(float);
extern int   random_integer_below(int);
extern int   scale_game_frames(int);
extern float D_L18_0015F580_macro[] __asm__("D_L18_0015F580") MACRO_ADDR;

#include "rnc/overlay/l18/mobys/falling_moby_group_data/falling_moby_group_update.h"

extern void spawn_debris_chunk(
    void *, void *, unsigned int, int, float, float, float, float, int) __asm__("FUN_L01_002f8530");
extern void
capture_linked_moby_relative_transforms(char *, void *, int) __asm__("FUN_L12_0027b268");
extern void remove_child_carrier_and_child(void *) __asm__("FUN_L18_002fa8e0");

void falling_moby_group_update(struct Moby *moby) __asm__("FUN_L18_002f16f0");

/* Moby class 1381 */
void falling_moby_group_update(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_APPROACH_TARGET_TRANSFORM = 2,
        MOBY_FALL_AND_REMOVE = 4
    };
    VData *moby_vars = (VData *)moby->pvars;
    if (moby_vars) {
        float initial_acceleration_step = group_transform_initial_acceleration_gp * frame_time_sq;
        float initial_braking_step = group_transform_initial_braking_gp * frame_time_sq;
        float initial_maximum_speed = group_transform_initial_maximum_speed_gp * frame_time;
        float later_acceleration_step = group_transform_later_acceleration_gp * frame_time_sq;
        float later_braking_step = group_transform_later_braking_gp * frame_time_sq;
        float later_maximum_speed = group_transform_later_maximum_speed_gp * frame_time;
        switch (moby->state) {
        case MOBY_INITIALIZE: {
            short **tp;
            short  *t;
            int     j;
            qcopy(moby_vars->v10, &moby->pos);
            qcopy(moby_vars->v20, &moby->rot);
            tp = D_L18_001AC240 + moby->group;
            j = 0;
            t = *tp;
            if (t == 0)
                return;
            while (j < 16) {
                int idx = (unsigned short)*t & 0x7FFF;
                if (*(short *)((idx << 8) + (int)D_L18_0015FFD8 + 0xA6) == 0x630) {
                    moby_vars->ent[j].idx = idx;
                    j++;
                }
                if (*t++ < 0)
                    break;
            }
            moby_vars->f348 = 0;
            moby_vars->f34C = scale_game_frames(30);
            moby_vars->cnt = j;
            capture_linked_moby_relative_transforms(moby, (char *)moby_vars + 0x30, j);
            moby->state = 1;
            break;
        }
        case MOBY_APPROACH_TARGET_TRANSFORM: {
            int            hero_on_group_or_class_764_moby;
            int            i;
            unsigned char *hero_ground_moby;
            if (group_transform_advance_enabled_gp != 0) {
                if (moby_vars->f330 < 1.5f) {
                    advance_accelerated_scalar(1.5f,
                                               initial_acceleration_step,
                                               initial_braking_step,
                                               initial_maximum_speed,
                                               &moby_vars->f330,
                                               &moby_vars->f334);
                } else {
                    advance_accelerated_scalar(2.0f,
                                               later_acceleration_step,
                                               later_braking_step,
                                               later_maximum_speed,
                                               &moby_vars->f330,
                                               &moby_vars->f334);
                }
            }
            if (moby_vars->f330 > group_transform_debris_start_progress_threshold_gp &&
                moby_vars->f344 == 0.0f) {
                moby_vars->f344 = *(int *)&group_transform_debris_initial_random_bound_gp;
            }
            if (moby_vars->f330 > group_transform_fall_progress_threshold_gp &&
                moby_vars->i8 != 0) {
                moby->state = 4;
            }
            hero_on_group_or_class_764_moby = 0;
            for (i = 0; i < moby_vars->cnt; i++) {
                unsigned char *group_moby = D_L18_0015FFD8 + (moby_vars->ent[i].idx << 8);
                if (hero.ground_moby == group_moby) {
                    moby_vars->f348 = 1;
                    hero_on_group_or_class_764_moby = 1;
                    break;
                }
            }
            hero_ground_moby = hero.ground_moby;
            if (hero_ground_moby != 0) {
                short s = MOBY(hero_ground_moby)->oclass;
                if (s == 0x764)
                    hero_on_group_or_class_764_moby = 1;
            }
            if (moby_vars->f348 != 0 && hero_on_group_or_class_764_moby == 0 &&
                hero.air_frames.s == 0) {
                if (fast_dec_timer(&moby_vars->f34C))
                    moby->state = 4;
            }
            break;
        }
        case MOBY_FALL_AND_REMOVE: {
            float t = frame_time_sq * 5.0f;
            moby->pos.z += moby_vars->f340;
            moby_vars->f340 = moby_vars->f340 - t;
            if (moby->pos.z < 20.0f) {
                int i;
                for (i = 0; i < moby_vars->cnt; i++) {
                    remove_child_carrier_and_child(D_L18_0015FFD8 + (moby_vars->ent[i].idx << 8));
                }
                mark_moby_for_removal(moby);
                return;
            }
            break;
        }
        }
        if ((unsigned char)(moby->state - 1) < 2) {
            lerp_vector(&moby->pos,
                        moby_vars->v10,
                        D_L18_001600EC + (moby_vars->i4 << 7) + 0x30,
                        moby_vars->f330 * 0.5f);
            lerp_vector(&moby->rot,
                        moby_vars->v20,
                        D_L18_001600EC + (moby_vars->i4 << 7) + 0x70,
                        moby_vars->f330 * 0.5f);
        }
        apply_linked_moby_relative_transforms(moby, (char *)moby_vars + 0x30, moby_vars->cnt);
        if (moby_vars->f344 != 0.0f) {
            if (moby->state != 4) {
                float debris_spawn_position[4];
                float s, z, debris_spawn_angle;
                int   debris_spawn_frames;
                moby_vars->f344 = moby_vars->f344 + 0.25f;
                if (random_integer_below(truncate_float_to_s32(moby_vars->f344)) == 0) {
                    s = random_float_between(-1.0471976f, 1.0471976f);
                    debris_spawn_angle = fast_add_rotations(s, D_L18_001677D8);
                    z = random_float_between(0.0f, 20.0f);
                    debris_spawn_position[0] = fast_cos(debris_spawn_angle) * z;
                    debris_spawn_position[1] = fast_sin(debris_spawn_angle) * z;
                    debris_spawn_position[2] = 15.0f;
                    fast_vec_add(debris_spawn_position, debris_spawn_position, &hero.motion.pos);
                    debris_spawn_frames = scale_game_frames(240);
                    spawn_debris_chunk(debris_spawn_position,
                                       D_L18_0015F580_macro,
                                       0x2B8,
                                       debris_spawn_frames,
                                       group_transform_debris_scale_multiplier_gp,
                                       2.0f,
                                       1.0f,
                                       0.75f,
                                       0);
                }
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3038), where it is exact; names translated to the US level program. */

extern void mark_block_entries_within_transform_xy_bounds(int, int) __asm__("FUN_L18_002f1d08");
extern void set_group_child_carrier_byte_bc(int, int) __asm__("FUN_L18_002fa888");

#include "rnc/overlay/l18/mobys/falling_moby_group.h"

void activate_drek_boss_class_0565_moby(struct Moby *moby) __asm__("FUN_L18_002f1c38");

void activate_drek_boss_class_0565_moby(struct Moby *moby) {
    char *state = (char *)moby->pvars;
    if (moby->oclass == 0x565 && moby->state == 1) {
        if (*(int *)(state + 0xC) != -1) {
            mark_block_entries_within_transform_xy_bounds(0x802, *(int *)(state + 0xC));
            mark_block_entries_within_transform_xy_bounds(0x908, *(int *)(state + 0xC));
            mark_block_entries_within_transform_xy_bounds(0x954, *(int *)(state + 0xC));
        }
        moby->state = 2;
        set_group_child_carrier_byte_bc(moby->group, 1);
    }
}
/* Reset this vendor moby's counters before its per-frame update. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F30C8), where it is exact; names translated to the US level program. */

typedef struct {
    char               pad0[0x20];
    unsigned char      state;
    char               pad21[0x57];
    struct L18VD_1cc8 *data;
    char               pad7C[0x2A];
    short              class_id;
} L18VM_1cc8;

typedef struct L18VD_1cc8 {
    char pad0[0x30];
    char update_data[0x300];
    int  field330;
    char pad334[4];
    int  field338;
    char pad33C[8];
    int  field344;
} L18VD_1cc8;

void reset_drek_boss_falling_moby_group(L18VM_1cc8 *moby) __asm__("FUN_L18_002f1cc8");

void reset_drek_boss_falling_moby_group(L18VM_1cc8 *moby) {
    L18VD_1cc8 *data = moby->data;
    if (moby->class_id == 0x565) {
        moby->state = 1;
        data->field330 = 0;
        data->field344 = 0;
    }
    apply_linked_moby_relative_transforms(moby, data->update_data, data->field338);
}
/* For each entry of block D_L18_001C6480[D_L18_001C6780[A]]: runs func_001F9BF0 /
   func_001F9EC0 against the records at D_L18_001600EC + B * 0x80 (+0x30, +0x40)
   for the entry's vector and, when both of the result's first two components are
   within 1.0, clears the entry's +0x0C and sets its +0x14 to 1.0. The index is
   copied (k = i) for the two stores and the loop tests `n > i`, as decomp-permuter
   found for retail's register use. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3108), where it is exact; names translated to the US level program. */

typedef struct {
    int   pad[3];
    int   w0c;
    int   pad2;
    float w14;
    int   pad3[2];
} Ent;

typedef struct {
    char  pad[0x26];
    short n;
    Ent  *e;
} Blk;

extern char         *D_L18_001C6200[];
extern unsigned char l18_block_indices_by_lookup_index[] __asm__("D_L18_001C6500");

void mark_block_entries_within_transform_xy_bounds(int block_lookup_index,

                                                   int transform_record_index) {
    int i = 0;
    int k;
    if (((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])->n > i)
        do {
            float entry_offset[4];
            fast_vec_sub(
                entry_offset,
                ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])->e +
                    i,
                D_L18_001600EC + (transform_record_index << 7) + 0x30);
            transform_vector_by_basis(
                entry_offset, entry_offset, D_L18_001600EC + (transform_record_index << 7) + 0x40);
            k = i;
            if (absolute_float(entry_offset[0]) < 1.0f && absolute_float(entry_offset[1]) < 1.0f) {
                ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])
                    ->e[k]
                    .w0c = 0;
                ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])
                    ->e[k]
                    .w14 = 1.0f;
            }
        } while (++i <
                 ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])->n);
}
