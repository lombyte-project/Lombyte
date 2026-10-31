/* Shared moby class hydrodisplacer: the dock where the hero uses the hydrodisplacer, with its activation camera. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/input/pad_globals.h"

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct PadState PadState;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2E0E50;

typedef struct {
    int   i0;
    int   i4;
    int   i8;
    int   iC;
    float f10;
    float f14;
} Data;

typedef struct {
    char       p0[0x30];
    Vec_2E0E50 v30;
    char       p40[0x30];
    Vec_2E0E50 v70;
} Node;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2e01b0;

extern int   hero_set_state() __asm__("FUN_L00_002223f8");
void         emit_hydrodisplacer_effect(char *m) __asm__("FUN_L00_002e01b0");
void         end_hydrodisplacer_interaction(char *a) __asm__("FUN_L00_002e0340");
extern int   func_001F9850_2E0E50(int) __asm__("FUN_001f96f8");
extern int   pad_pressed_within(int, int, int) __asm__("FUN_L00_00266d60");
extern int   func_L00_002E1790_2E0E50(Moby *) __asm__("FUN_L00_002e02e0");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   func_001FA898_2E0E50(float) __asm__("FUN_001fa6d0");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void  func_001F9C08_2E0E50(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float func_001FA790_2E0E50(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA748_2E0E50(float, float) __asm__("FUN_001fa580");
extern int   func_001F9908_2E0E50(void *) __asm__("FUN_001f9740");
int          FUN_001f9740_2e01b0(void *) __asm__("FUN_001f9740");
void         spawn_debris_chunk(V_2e01b0 *, V_2e01b0 *, int, int) __asm__("FUN_L00_0026f548");
int          FUN_001f96f8_2e01b0(int) __asm__("FUN_001f96f8");

extern int   D_L00_0015F5C4_macro __asm__("D_L00_0015F5C4") MACRO_ADDR;
extern int   hydrodisplacer_camera_active_macro __asm__("D_L00_0015F404") MACRO_ADDR;
extern char *moby_table_macro __asm__("D_L00_0015FFD8") MACRO_ADDR;
extern char *D_L00_001600EC_macro __asm__("D_L00_001600EC") MACRO_ADDR;
extern float fade_level_macro __asm__("D_L00_0015F3FC") MACRO_ADDR;
extern float hydrodisplacer_tank_fill_time_macro __asm__("D_L00_00161BB4") MACRO_ADDR;
extern float frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern float frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern int   hydrodisplacer_camera_active __asm__("D_L00_0015F404");
extern short D_L00_0015F3FC;

float D_L01_00162040 = 1e+02f;
float D_L02_00162030 = 1e+02f;
float D_L03_00161C6C = 1e+02f;
float D_L04_00161D5C = 1e+02f;
float D_L05_00161FAC = 1e+02f;
float D_L06_001622BC = 1e+02f;
float D_L07_00161CB4 = 1e+02f;
float D_L08_001622F8 = 1e+02f;
float D_L09_00161D9C = 1e+02f;
float D_L10_00161F08 = 1e+02f;
float D_L11_00162314 = 1e+02f;
float D_L12_00161DC4 = 1e+02f;
float D_L13_00161E24 = 1e+02f;
float D_L14_00162128 = 1e+02f;
float D_L15_00161FB4 = 1e+02f;
float D_L16_00161D5C = 1e+02f;
float D_L17_00161F94 = 1e+02f;
float D_L18_001621F4 = 1e+02f;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E0E50), where it is exact; names translated to the US level program. */
void hydrodisplacer_update(Moby *moby) __asm__("FUN_L00_002df9a0");

/* Moby class 1251 */
void hydrodisplacer_update(Moby *moby) {
    enum {
        HYDRODISPLACER_INITIALIZE = 0,
        HYDRODISPLACER_WAIT_FOR_ANIMATION_FLAG = 1,
        HYDRODISPLACER_WAIT_FOR_ACTIVATION = 2,
        HYDRODISPLACER_WAIT_FOR_HERO_FLAG = 3,
        HYDRODISPLACER_CAMERA_TRANSITION = 4,
        HYDRODISPLACER_WAIT_FOR_TIMER = 5
    };
    enum {
        HYDRODISPLACER_CAMERA_PREPARE_INTERPOLATION = 0,
        HYDRODISPLACER_CAMERA_INTERPOLATE = 1,
        HYDRODISPLACER_CAMERA_RETURN_TO_ACTIVATION = 2
    };
    Vec_2E0E50 camera_pos, camera_rot;
    Data      *hydrodisplacer_interaction_state = (void *)moby->pvars;

    if (hero.items[0].state == 3) {
        end_hydrodisplacer_interaction(moby);
        return;
    }
    if (hero.hydro_tank_full != 0) {
        if (D_L00_0015F5C4_macro == 0 && !((unsigned int)(hero.state.current - 0x38) < 2)) {
            emit_hydrodisplacer_effect(moby);
        }
        if (hero.hydro_tank_full != 0) {
            float accel = frame_scale_sq_macro * 0.02f;
            float max_speed = frame_scale_sq_macro * 0.3f;

            hero.item_eases[0].attach_joint = 1;
            hero.item_eases[0].scale = 2.3f;
            hero.item_eases[0].accel = accel;
            hero.item_eases[0].max_speed = max_speed;
            if (!((unsigned int)(hero.state.current - 0x38) < 2)) {
                float offset_y = 1024.0f / moby->scale * -0.2f;

                hero.item_eases[1].accel = accel;
                hero.item_eases[1].max_speed = max_speed;
                hero.item_eases[2].max_speed = max_speed;
                hero.item_eases[1].attach_joint = 2;
                hero.item_eases[2].attach_joint = 3;
                hero.item_eases[2].accel = accel;
                hero.item_eases[1].pos_target.f[1] = offset_y;
                hero.item_eases[2].pos_target.f[1] = offset_y;
            }
        }
    }
    switch (moby->state) {
    case HYDRODISPLACER_INITIALIZE:
        allocate_voice_for_target_entry(0, 0, moby);
        moby->state = 1;
        break;
    case HYDRODISPLACER_WAIT_FOR_ANIMATION_FLAG:
        if (moby->anim_flags & 2) {
            moby->state = 2;
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_ACTIVATION:
        if (pad_pressed_within(hero.items[0].button_mask, func_001F9850_2E0E50(8), 0) == 0) {
            break;
        }
        if ((pad_held[0] & hero.items[0].button_mask) == 0) {
            break;
        }
        if (hero.ground_moby == 0 || hero.air_frames.s != 0 || hero.ground_moby->oclass != 0x155) {
            break;
        }
        if (func_L00_002E1790_2E0E50(moby) == 0 || (unsigned int)hero.state.control_mode >= 2) {
            break;
        }
        moby->state = 3;
        hero_set_state(0x38, 0);
        if (hero.hydro_tank_full != 0) {
            set_hero_animation(0x83, 0, func_001F9850_2E0E50(6));
        } else {
            set_hero_animation(0x46, 0, func_001F9850_2E0E50(6));
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_HERO_FLAG: {
        int  *platform;
        char *linked;

        if (hero.ground_moby == 0 || hero.ground_moby->oclass != 0x155) {
            end_hydrodisplacer_interaction(moby);
            return;
        }
        if (!(hero.anim_flags & 2)) {
            break;
        }
        platform = (void *)hero.ground_moby->pvars;
        linked = moby_table_macro + (platform[0] << 8);
        hero_set_state(0x39, 0);
        if (hero.hydro_tank_full != 0) {
            set_hero_animation(0x84, 0, func_001F9850_2E0E50(6));
        } else {
            set_hero_animation(0x47, 0, func_001F9850_2E0E50(6));
        }
        if (hero.hydro_tank_full == 0) {
            allocate_voice_for_target_entry(0x14, 0, hero.moby);
        } else {
            allocate_voice_for_target_entry(0x15, 0, hero.moby);
        }
        if (platform[1] != -1 && platform[2] != -1) {
            moby->command = 0;
            moby->state = 4;
            hydrodisplacer_interaction_state->f10 = 0.0f;
            jet_mount(g_debug_camera.position, g_debug_camera.angles, 1, 0, 0);
            if (hero.hydro_tank_full != 0) {
                hydrodisplacer_interaction_state->i8 = platform[1];
                hydrodisplacer_interaction_state->iC = platform[2];
            } else {
                hydrodisplacer_interaction_state->iC = platform[1];
                hydrodisplacer_interaction_state->i8 = platform[2];
            }
            break;
        }
        moby->state = 5;
        hydrodisplacer_interaction_state->i0 =
            func_001FA898_2E0E50(scale_time(hydrodisplacer_tank_fill_time_macro));
        if (hero.hydro_tank_full == 0) {
            ((Moby *)linked)->command = 8;
            hero.hydro_tank_full = 1;
        } else {
            ((Moby *)linked)->command = 4;
            hero.hydro_tank_full = 0;
        }
        break;
    }
    case HYDRODISPLACER_CAMERA_TRANSITION:
        if (hero.ground_moby == 0 || hero.ground_moby->oclass != 0x155) {
            end_hydrodisplacer_interaction(moby);
            return;
        }
        switch (moby->command) {
        case HYDRODISPLACER_CAMERA_PREPARE_INTERPOLATION: {
            char *linked;

            approach_value(&fade_level_macro, 1.0f, 1.0f / func_001F9850_2E0E50(15));
            if (!(1.0f <= fade_level_macro)) {
                break;
            }
            linked = moby_table_macro + (*(int *)hero.ground_moby->pvars << 8);
            moby->command = 1;
            set_camera_position(D_L00_001600EC_macro + (hydrodisplacer_interaction_state->i8 << 7) +
                                0x30);
            set_camera_rotation(D_L00_001600EC_macro + (hydrodisplacer_interaction_state->i8 << 7) +
                                0x70);
            hydrodisplacer_camera_active_macro = 1;
            if (hero.hydro_tank_full == 0) {
                ((Moby *)linked)->command = 8;
                hero.hydro_tank_full = 1;
            } else {
                ((Moby *)linked)->command = 4;
                hero.hydro_tank_full = 0;
            }
            break;
        }
        case HYDRODISPLACER_CAMERA_INTERPOLATE:
            if (hydrodisplacer_interaction_state->f10 == 0.0f && 0.0f < fade_level_macro) {
                approach_value(&fade_level_macro, 0.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            if (hydrodisplacer_interaction_state->f10 < 1.0f) {
                advance_accelerated_scalar(&hydrodisplacer_interaction_state->f10,
                                           &hydrodisplacer_interaction_state->f14,
                                           1.0f,
                                           frame_time_sq_macro * 0.666f,
                                           frame_time_sq_macro * 0.666f,
                                           frame_time_macro * 0.5f);
                func_001F9C08_2E0E50(
                    &camera_pos,
                    D_L00_001600EC_macro + (hydrodisplacer_interaction_state->i8 << 7) + 0x30,
                    D_L00_001600EC_macro + (hydrodisplacer_interaction_state->iC << 7) + 0x30,
                    hydrodisplacer_interaction_state->f10);
                clear_u64_value(&camera_rot);
                camera_rot.y =
                    func_001FA790_2E0E50(
                        ((Node *)D_L00_001600EC_macro)[hydrodisplacer_interaction_state->iC].v70.y,
                        ((Node *)D_L00_001600EC_macro)[hydrodisplacer_interaction_state->i8]
                            .v70.y) *
                    hydrodisplacer_interaction_state->f10;
                camera_rot.y = func_001FA748_2E0E50(
                    ((Node *)D_L00_001600EC_macro)[hydrodisplacer_interaction_state->i8].v70.y,
                    camera_rot.y);
                camera_rot.z =
                    func_001FA790_2E0E50(
                        ((Node *)D_L00_001600EC_macro)[hydrodisplacer_interaction_state->iC].v70.z,
                        ((Node *)D_L00_001600EC_macro)[hydrodisplacer_interaction_state->i8]
                            .v70.z) *
                    hydrodisplacer_interaction_state->f10;
                camera_rot.z = func_001FA748_2E0E50(
                    ((Node *)D_L00_001600EC_macro)[hydrodisplacer_interaction_state->i8].v70.z,
                    camera_rot.z);
                set_camera_position(&camera_pos);
                set_camera_rotation(&camera_rot);
                return;
            }
            if (fade_level_macro < 1.0f) {
                approach_value(&fade_level_macro, 1.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            moby->command = 2;
            hydrodisplacer_camera_active_macro = 0;
            jet_end_mount(3);
            return;
        case HYDRODISPLACER_CAMERA_RETURN_TO_ACTIVATION:
            if (0.0f < fade_level_macro) {
                approach_value(&fade_level_macro, 0.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            moby->state = 2;
            hero_set_state(0x3A, 0);
            if (hero.hydro_tank_full != 0) {
                set_hero_animation(0x48, 0, func_001F9850_2E0E50(6));
            } else {
                set_hero_animation(0x85, 0, func_001F9850_2E0E50(6));
            }
            return;
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_TIMER:
        if (hero.ground_moby != 0 && hero.ground_moby->oclass == 0x155 &&
            func_001F9908_2E0E50(hydrodisplacer_interaction_state) == 0) {
            break;
        }
        end_hydrodisplacer_interaction(moby);
        moby->state = 2;
        break;
    }
}

void emit_hydrodisplacer_effect(char *moby) {
    V_2e01b0     effect_position, effect_motion_vector;
    char        *moby_data = ((char *)MOBY(moby)->pvars);
    struct Hero *player;
    int          countdown_lower_bound;
    float        normalization_scale;
    if (FUN_001f9740_2e01b0(moby_data + 4) == 0)
        return;
    moby_point_position(moby, 0, &effect_position);
    normalization_scale = -random_float_between(frame_time, frame_time * 3.0f);
    fast_vec_normalize(&effect_motion_vector, moby + 0xD0, normalization_scale);
    player = &hero;
    effect_motion_vector.x += player->motion.displacement.f[0] * 0.8f;
    effect_motion_vector.y += player->motion.displacement.f[1] * 0.8f;
    effect_motion_vector.z += player->motion.displacement.f[2] +
                              random_float_between(frame_time * -0.5f, frame_time + frame_time);
    spawn_debris_chunk(&effect_position, &effect_motion_vector, 2, random_int_between(0x5A, 0x78));
    countdown_lower_bound = FUN_001f96f8_2e01b0(5);
    *(int *)(moby_data + 4) = random_int_between(countdown_lower_bound, FUN_001f96f8_2e01b0(15));
}

/* Tests flag 2 (or 1 when &hero.hydro_tank_full is set) at 0xBC of the moby selected by +0x2FC. */
s32 test_hydrodisplacer_activation_flag(void) __asm__("FUN_L00_002e02e0");

s32 test_hydrodisplacer_activation_flag(void) {
    struct Hero *activation_hero;
    u8          *linked_moby;
    s32          ground_moby_address;
    s32          idx;
    u8           flags;
    activation_hero = &hero;
    linked_moby = moby_table;
    ground_moby_address = ((s32)activation_hero->ground_moby);
    flags = (u8)activation_hero->hydro_tank_full;
    linked_moby += *(s32 *)*(s32 *)((u8 *)ground_moby_address + 0x78) << 8;
    if (!flags) {
        if (linked_moby[0xBC] & 2)
            return 1;
    } else {
        if (linked_moby[0xBC] & 1)
            return 1;
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E17F0), where it is exact; names translated to the US level program. */

/* Moby state 4: clears two globals, calls 002eac18(3); control mode 0x13: calls 002223f8(0, 1). */
void end_hydrodisplacer_interaction(char *moby) {
    if (MOBY(moby)->state == 4) {
        hydrodisplacer_camera_active = 0;
        *(int *)&D_L00_0015F3FC = 0;
        jet_end_mount(3);
    }
    if (hero.state.control_mode == 0x13) {
        hero_set_state(HERO_STATE_IDLE, 1);
    }
}

float hydrodisplacer_tank_fill_time __asm__("D_L00_00161BB4") = 1e+02f;
