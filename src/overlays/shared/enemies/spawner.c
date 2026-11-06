/* Shared moby class spawner: lobs spawner_egg bubbles towards target regions. */

#include "types.h"
#include "rnc/mobys/moby_state.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/shared/enemies/spawner_egg.h"
#include "rnc/overlay/shared/math/random.h"

enum {
    SPAWNER_STATE_INITIALIZE = 0,
    SPAWNER_STATE_SPAWN = 1,
    SPAWNER_STATE_EXPLODE = 2
};

typedef struct SpawnerVars {
    u8    pad0[0x20];
    f32   unk20;
    u8    pad24[0x3c];
    u8    unk60[0x7];
    s8    unk67;
    u8    pad68[0x8];
    u8    unk70[0x10];
    f32   target_search_radius;
    s32   excluded_moby;
    s32   region_path_index;
    s16   spawn_interval_frames;
    s16   spawn_timer;
    s8    max_spawn_count;
    s8    spawn_count;
    u8    pick_region_nearest_hero;
    s8    egg_grows_held_moby;
    s32   cleared_moby_group;
    u8    pad98[0x18];
    char *slots[4];
} SpawnerVars;

extern int FUN_L00_0025a478_c(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L00_0025d458_c(void *, void *) __asm__("FUN_L00_0025d458");
extern void FUN_L00_0025d538_c(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00257470_c(void *, int, int) __asm__("FUN_L00_00257470");
extern int
activate_effect_l08(void *, void *, float, void *, int, void *, int) __asm__("FUN_L00_0025ff38");
extern void add_random_offset_to_vector_xyz(float *, float) __asm__("FUN_L00_0025f8e0");
extern float
FUN_L00_0025abf0_30ab80(void *, void *, void *, float, float) __asm__("FUN_L00_0025abf0");
extern void  FUN_L00_0025e450_30ab80(void *,
                                     void *,
                                     void *,
                                     float,
                                     float,
                                     int,
                                     int,
                                     int,
                                     float,
                                     float,
                                     float,
                                     float,
                                     int,
                                     float,
                                     int,
                                     int,
                                     int,
                                     int) __asm__("FUN_L00_0025e450");
extern void *FUN_L00_00263fd8_30ab80(char *,
                                     int,
                                     void *,
                                     void *,
                                     int,
                                     int,
                                     float,
                                     float *,
                                     float *,
                                     float *) __asm__("FUN_L00_00263fd8");

extern char *spawner_target_region_records __asm__("D_L09_001600EC") __attribute__((sda));
extern float D_L09_0015F580[] MACRO_ADDR;
extern f32   frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern f32   frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

void process_spawner_hit_record(struct Moby *moby, char *spawner_vars) __asm__("FUN_L09_0030a9e8");

void process_spawner_hit_record(struct Moby *moby, char *spawner_vars) {
    char *moby_record;
    int   record_processing_status;
    float remaining_value;

    moby_record = find_moby_hit(moby, 0xC30000, 0);
    FUN_L00_0025a478_c(
        moby, moby_record, spawner_vars + 0x20, 0, &record_processing_status, 0, 0, 4);
    if (record_processing_status >= 2) {
        remaining_value = *(float *)(spawner_vars + 0x20) - *(float *)(moby_record + 0x2C);
        *(float *)(spawner_vars + 0x20) = remaining_value;
        if (0.0f < remaining_value) {
            spawner_vars[0x67] = 0x5A;
            FUN_L00_0025d458_c(moby, spawner_vars + 0x60);
        } else {
            FUN_L00_00257470_c(moby, 0, -1);
            moby->state = SPAWNER_STATE_EXPLODE;
            spawner_vars[0x67] = 0x78;
            FUN_L00_0025d458_c(moby, spawner_vars + 0x60);
        }
    }
    moby->hit_slot = 0xFF;
    FUN_L00_0025d538_c(moby, spawner_vars + 0x60);
}

/* Counts the live level-0x75E mobys among the 20 slots at +0xB0 and returns the last slot not counted. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_0030BEE8), where it is exact; names translated to the US level program. */
int refresh_spawner_population_and_find_free_slot(int   unused,
                                                  char *spawner_vars) __asm__("FUN_L09_0030aae0");

int refresh_spawner_population_and_find_free_slot(int unused, char *spawner_vars) {
    int    last = -1;
    int    i;
    char **slot;
    spawner_vars[0x91] = count_active_moby_group_members(*(int *)(spawner_vars + 0x94), -1);
    for (i = 0, slot = (char **)(spawner_vars + 0xB0); i < 20; i++, slot++) {
        char *slot_moby = *slot;
        if (slot_moby != 0 && MOBY(slot_moby)->oclass == 0x75E &&
            (unsigned char)MOBY(slot_moby)->state != MOBY_STATE_REMOVAL_MARKER &&
            (unsigned char)MOBY(slot_moby)->state != 0xFD) {
            spawner_vars[0x91]++;
        } else {
            last = i;
        }
    }
    return last;
}

/* Spawner update (moby class 0x75D): state 0 clears the slot list, state 1 picks a target and lobs a
 * 0x75E moby towards it, state 2 explodes. Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c:
 * func_L09_0030BF88), where it is exact; this build has no 0.85 limit check before a spawn. */
void spawner_update(char *moby) __asm__("FUN_L09_0030ab80");

/* Moby class 1885 */
void spawner_update(char *moby) {
    SpawnerVars *spawner_vars = (SpawnerVars *)MOBY(moby)->pvars;
    float        target[4];
    float        launch_velocity[4];
    float        spawn_position[4];
    float        offset[4];

    process_spawner_hit_record(moby, spawner_vars);
    switch ((unsigned char)MOBY(moby)->state) {
    case SPAWNER_STATE_INITIALIZE: {
        int moby_to_remove = 0;
        while (get_first_moby_in_group(&moby_to_remove, spawner_vars->cleared_moby_group, 0, 0) ==
               0) {
            mark_moby_for_removal((void *)moby_to_remove);
        }
        MOBY(moby)->state = SPAWNER_STATE_SPAWN;
        return;
    }
    case SPAWNER_STATE_SPAWN: {
        process_spawner_hit_record(moby, spawner_vars);
        if (!fast_dec_timer_s16(&spawner_vars->spawn_timer))
            return;
        if (random_integer_below(7) == 0) {
            char *excluded = 0;
            int   excluded_count = 0;
            char *region = 0;
            int   region_count = 0;
            if (spawner_vars->excluded_moby != -1) {
                excluded = &spawner_vars->excluded_moby;
                excluded_count = 1;
            }
            if (spawner_vars->region_path_index != -1) {
                char *q = ((char *)l09_paths[spawner_vars->region_path_index]);
                region_count = *(int *)q;
                region = q + 0x10;
            }
            activate_effect_l08(moby,
                                (char *)spawner_vars + 0x100,
                                spawner_vars->target_search_radius,
                                excluded,
                                excluded_count,
                                region,
                                region_count);
        } else {
            char *target_moby = *(char **)((char *)spawner_vars + 0x140);
            if (target_moby != 0 &&
                (unsigned char)MOBY(target_moby)->state != MOBY_STATE_REMOVAL_MARKER &&
                (unsigned char)MOBY(target_moby)->state != 0xFD) {
                qcopy((char *)spawner_vars + 0x100, &MOBY(target_moby)->pos);
            } else {
                *(int *)((char *)spawner_vars + 0x140) = 0;
                *(int *)((char *)spawner_vars + 0x144) = 2;
            }
        }
        if (*(int *)((char *)spawner_vars + 0x144) == 2)
            return;
        {
            int   last = refresh_spawner_population_and_find_free_slot((int)moby, spawner_vars);
            char *sel;
            if ((unsigned char)spawner_vars->spawn_count >=
                (unsigned char)spawner_vars->max_spawn_count)
                return;
            if (last == -1)
                return;
            sel = 0;
            if (spawner_vars->pick_region_nearest_hero != 0) {
                int   i;
                float best = 100000.0f;
                for (i = 0; i < 4; i++) {
                    int idx = *(int *)((char *)spawner_vars + 0xA0 + i * 4);
                    if (idx != -1) {
                        char *q = spawner_target_region_records + (idx << 7);
                        float candidate_distance = fast_vec_dist(D_0013E533 + 0xE9D, q + 0x30);
                        if (candidate_distance < best) {
                            sel = q;
                            best = candidate_distance;
                        }
                    }
                }
                if (sel == 0) {
                    fast_vec_sub(target, (char *)spawner_vars + 0x100, &MOBY(moby)->pos);
                    fast_vec_scale(target, target, 0.5f);
                    if (3.0f < spawner_vars->target_search_radius)
                        clamp_vector_length(target, target, 3.0f);
                    if (fast_vec_length(target) < 3.0f)
                        fast_vec_normalize(target, target, 3.0f);
                    fast_vec_add(target, target, &MOBY(moby)->pos);
                } else {
                    qcopy(target, sel + 0x30);
                }
            } else {
                int start = random_int_between(0, 3);
                int i;
                for (i = 0; i < 4; i++) {
                    int k = (i + start) % 4;
                    int idx = *(int *)((char *)spawner_vars + (k << 2) + 0xA0);
                    if (idx != -1) {
                        sel = spawner_target_region_records + (idx << 7);
                        break;
                    }
                }
                if (sel == 0) {
                    float target_offset_scale = spawner_vars->target_search_radius;
                    if (target_offset_scale < 3.0f)
                        target_offset_scale = 3.0f;
                    generate_random_scaled_vector2(target, target_offset_scale);
                    fast_vec_add(target, target, &MOBY(moby)->pos);
                } else {
                    qcopy(target, sel + 0x30);
                }
            }
            if (sel != 0) {
                fast_vec_scale(launch_velocity, sel, random_float_between(-1.0f, 1.0f));
                fast_vec_add(target, target, launch_velocity);
                fast_vec_scale(launch_velocity, sel + 0x10, random_float_between(-1.0f, 1.0f));
                fast_vec_add(target, target, launch_velocity);
            }
            {
                float ground = probe_ground_height(target, 0, 0.5f);
                if (ground != 0.0f)
                    target[2] = ground;
            }
            fast_vec_sub(launch_velocity, target, &MOBY(moby)->pos);
            {
                float target_distance = fast_vec_length(launch_velocity);
                float speed = target_distance * (frame_time_macro * 0.6f);
                fast_vec_normalize(launch_velocity, launch_velocity, speed);
                moby_point_position(moby, 0, spawn_position);
                fast_vec_normalize(offset, launch_velocity, 0.05f);
                add_random_offset_to_vector_xyz(offset, 0.025f);
                fast_vec_add(spawn_position, spawn_position, offset);
                launch_velocity[2] = FUN_L00_0025abf0_30ab80(
                    spawn_position, target, 0, speed, -(frame_time_sq_macro * 9.8f));
            }
            spawner_vars->slots[last] =
                spawn_spawner_egg((int)moby,
                                  spawn_position,
                                  launch_velocity,
                                  (unsigned char)spawner_vars->egg_grows_held_moby,
                                  (unsigned char)last);
            spawner_vars->spawn_timer = scale_game_frames(spawner_vars->spawn_interval_frames);
            spawner_vars->spawn_count = (char)(spawner_vars->spawn_count + 1);
        }
        return;
    }
    case SPAWNER_STATE_EXPLODE:
        FUN_L00_0025e450_30ab80(moby,
                                &spawner_vars->unk70,
                                0,
                                0.0f,
                                0.0f,
                                30,
                                8,
                                20,
                                8.0f,
                                4.0f,
                                18.0f,
                                1.0f,
                                1,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
        FUN_L00_00263fd8_30ab80(moby,
                                0x6C5,
                                &MOBY(moby)->pos,
                                moby + 0x40,
                                0,
                                0,
                                0.0f,
                                D_L09_0015F580,
                                D_L09_0015F580,
                                D_L09_0015F580);
        FUN_L00_00263fd8_30ab80(moby,
                                0x6C6,
                                &MOBY(moby)->pos,
                                moby + 0x40,
                                0,
                                0,
                                0.0f,
                                D_L09_0015F580,
                                D_L09_0015F580,
                                D_L09_0015F580);
        FUN_L00_00263fd8_30ab80(moby,
                                0x6C7,
                                &MOBY(moby)->pos,
                                moby + 0x40,
                                0,
                                0,
                                0.0f,
                                D_L09_0015F580,
                                D_L09_0015F580,
                                D_L09_0015F580);
        mark_moby_for_removal(moby);
        return;
    default:
        return;
    }
}
