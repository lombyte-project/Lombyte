/* Level 12 moby class hoven_turret (class 1267): the manned turret, its camera, target timer, HUD and destruction fade. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/input/pad_globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/rendering/packets/emit_rgba_draw_packet.h"

typedef struct Moby Moby;

extern Moby *D_L12_0015FFD8;
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003046C8), where it is exact; names translated to the US level program. */

extern void snap_scripted_camera_to_pose(void *) __asm__("FUN_L08_00313ba8");

extern char hoven_turret_camera_local_offset __asm__("D_L12_00161EF0") __attribute__((sda));
void        update_hoven_turret_camera(struct Moby *moby) __asm__("FUN_L12_003032e8");

void update_hoven_turret_camera(struct Moby *moby) {
    float position_and_rotation[8];
    char *data = (char *)moby->pvars;
    snap_scripted_camera_to_pose(moby);
    transform_vector_by_basis(
        position_and_rotation, &hoven_turret_camera_local_offset, &moby->rot_row0);
    fast_vec_add(position_and_rotation, position_and_rotation, &moby->pos);
    set_camera_position(position_and_rotation);
    position_and_rotation[4] = 0;
    position_and_rotation[5] = -*(float *)(data + 0x74);
    position_and_rotation[6] = fast_add_rotations(1.5707964f, moby->rot.z);
    set_camera_rotation(position_and_rotation + 4);
}

typedef struct {
    u8  pad0[0x40];
    s32 targets[10];
    u8  pad68[0x20];
    s32 timer;
} Data_303370;

/* Raises the timer while one of the ten targets is in the view cone, otherwise counts it down. */
void update_hoven_turret_target_alignment_timer(void *moby) __asm__("FUN_L12_00303370");

void update_hoven_turret_target_alignment_timer(void *moby) {
    Data_303370 *data = (Data_303370 *)((struct Moby *)moby)->pvars;
    int          found = 0;
    int          i;
    Vec4         position;

    for (i = 0; i < 10; i++) {
        if (data->targets[i] != -1) {
            Moby *target = &D_L12_0015FFD8[data->targets[i]];
            float yaw_limit;
            float pitch_limit;
            float yaw;
            float pitch;

            qcopy(&position, &target->pos);
            yaw_limit = 0.12217305f;
            pitch_limit = yaw_limit;
            if (target->oclass == 0x146)
                position.f[2] += 1.5f;
            if (target->oclass == 0x4FE) {
                yaw_limit = 0.034906585f;
                pitch_limit = 0.05235988f;
                position.f[2] += 1.5f;
            }
            yaw = fast_arc_tan(position.f[0] - l12_camera.pos.f[0],
                               position.f[1] - l12_camera.pos.f[1]);
            pitch = fast_arc_tan(vector_distance_xy(&l12_camera.pos, &position),
                                 position.f[2] - l12_camera.pos.f[2]);
            if (fast_difference_between_rotations(yaw, l12_camera.yaw) < yaw_limit &&
                fast_difference_between_rotations(pitch, -l12_camera.pitch) < pitch_limit)
                found = 1;
        }
    }
    if (!found) {
        fast_dec_timer(&data->timer);
    } else {
        data->timer += 2;
        if (scale_game_frames(30) < data->timer)
            data->timer = scale_game_frames(30);
    }
}

#include "sda.h"

/* Update of moby class 1267 (Hoven turret): waits for the hero, is manned and aimed with the stick, fires, and burns out when destroyed. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00304920), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef float Vec_304920[4] __attribute__((aligned(16)));

typedef struct {
    char          pad0;
    unsigned char b1;
    char          pad2[0xE];
    char          f10[0x60];
    short         f70;
    short         f72;
    float         f74;
    float         f78;
    float         f7C;
    int           f80;
    int           f84;
    int           f88;
    float         f8C;
    int           f90;
    int           f94;
    int           f98;
    int           f9C;
    float         fA0;
    float         fA4;
    int           fA8;
    short         fAC;
    short         fAE;
} Data_304920;

typedef struct {
    char         pad0[0x6C];
    struct Moby *f6C;
} Snd;

typedef struct {
    char  pad0[0x2C];
    float f2C;
} Hit_304920;

extern char *FUN_L00_0025a420(void *, int, int);
extern char *FUN_L00_0026dd20(void *, void *, int, int, float);
extern float D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;

#include "rnc/overlay/l12/mobys/hoven_turret_data/hoven_turret_update.h"

extern float              randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float              approach_value(float *, float, float);
extern float              D_L12_0015F580_macro[] __asm__("D_L12_0015F580") MACRO_ADDR;
extern int                D_L12_0015F5C4_macro __asm__("D_L12_0015F5C4") MACRO_ADDR;
extern int D_L12_001619A0 MACRO_ADDR;
extern int                D_L12_00161F00 __attribute__((sda));
extern int                D_L12_00161F04 __attribute__((sda));
extern int D_L12_00161F20 MACRO_ADDR;
extern int D_L12_00161F24 MACRO_ADDR;
extern int                random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int                hero_set_state(int, int) __asm__("FUN_L12_002400d0");
extern void               FUN_L00_001fff28(void *, int, float);
extern void               hero_respawn(void) __asm__("FUN_L00_00211250");
extern void               set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void               moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void               jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void               spawn_explosion(void *,
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
                                          int,
                                          float,
                                          float,
                                          int,
                                          int,
                                          int,
                                          int) __asm__("FUN_L00_0025e450");
extern void               FUN_L00_00262cf0(void *, void *, float, float, float);
extern void               start_scene(int) __asm__("FUN_L00_00298840");
extern void               queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void  spawn_turret_rocket(void *, void *, void *, int) __asm__("FUN_L08_002e7eb0");
extern void  update_hoven_turret_target_alignment_timer(void *) __asm__("FUN_L12_00303370");
extern void  draw_hoven_turret_hud() __asm__("FUN_L12_00304218");
extern void  draw_hoven_turret_destruction_fade() __asm__("FUN_L12_00304d98");
extern void  allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void  attach_manipulator(void *, int, void *);
extern void  detach_manipulator(void *, void *);
extern void  memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void  set_moby_animation(void *, s32, s32) __asm__("func_00212ED8");
extern char  D_0013E533[];
extern int   invert52_macro[] __asm__("D_0015EDDC") MACRO_ADDR;
extern Moby *D_L12_0015FFD8_gp __asm__("D_L12_0015FFD8") MACRO_ADDR;
extern int   func_0020BFC8_304920(int, int) __asm__("FUN_0020b178");

extern f32 frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
void       hoven_turret_update(struct Moby *moby) __asm__("FUN_L12_00303540");

void hoven_turret_update(struct Moby *moby) {
    enum {
        HOVEN_TURRET_STATE_INITIALIZE = 0,
        HOVEN_TURRET_STATE_WAIT_FOR_BOARDING = 3,
        HOVEN_TURRET_STATE_MANNED = 4,
        HOVEN_TURRET_STATE_START_SCENE = 5,
        HOVEN_TURRET_STATE_INACTIVE = 6,
        HOVEN_TURRET_STATE_DESTRUCTION_EFFECTS = 7,
        HOVEN_TURRET_STATE_SAVE_DATA = 8
    };
    Data_304920 *data = *(Data_304920 **)&moby->pvars;

    ((Snd *)(D_0013E533 + 0x1D))->f6C = moby;
    switch (moby->state) {
    case HOVEN_TURRET_STATE_INITIALIZE:
        D_L12_00161F00 = 0;
        D_L12_00161F04 = 0;
        if (visited52[moby->mission + (current_level_index << 4)] == 0xFF) {
            moby->state = 6;
        } else {
            attach_manipulator(moby, 0, data);
            moby->state = 3;
            data->f84 = 0xC8;
            moby->rot.y = 0.0f;
            data->f70 = scale_game_frames(0x1E);
            data->f72 = 0;
            data->f78 = 0.0f;
            data->f7C = 0.0f;
            data->fA0 = moby->pos.z;
            data->f98 = -1;
            moby->rot.z = 3.1415927f;
            ((Snd *)(D_0013E533 + 0x1D))->f6C = moby;
        }
        break;
    case HOVEN_TURRET_STATE_WAIT_FOR_BOARDING: {
        {
            Vec_304920 out;

            moby_point_position(moby, 3, out);
        }
        if (vector_distance_xy(((float *)&moby->pos),
                               ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f) < 7.0f ||
            ((Hero *)(D_0013E533 + 0xE1D))->ground_moby == moby) {
            moby->pos.z = moby->pos.z + ((data->fA0 - 1.2f) - moby->pos.z) * 0.11f;
            data->fA4 =
                fast_add_rotations(
                    data->fA4,
                    fast_add_rotations(
                        fast_subtract_rotations(
                            fast_arc_tan(
                                ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f[0] - moby->pos.x,
                                ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f[1] - moby->pos.y),
                            moby->rot.z),
                        1.5707964f) *
                        0.01f) *
                0.9f;
            moby->rot.z = fast_add_rotations(moby->rot.z, data->fA4);
        } else {
            if (moby->pos.z < data->fA0) {
                moby->pos.z = moby->pos.z + D_0015ED6C_gp;
            } else {
                moby->pos.z = data->fA0;
            }
            if (((Hero *)(D_0013E533 + 0xE1D))->ground_moby != moby && data->f98 != -1) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
            }
        }
        if (vector_distance_xy(((float *)&moby->pos),
                               ((float *)&((Hero *)(D_0013E533 + 0xE1D))->moby->pos)) < 1.3f) {
            if (scale_game_frames(0xF) < ((Hero *)(D_0013E533 + 0xE1D))->grounded_frames) {
                jet_mount(((float *)&moby->pos), ((float *)&moby->rot), 1, 0, 0);
                if (moby->prev_seq != 5) {
                    set_moby_animation(moby, 5, 0);
                }
                hero_set_state(0x32, 1);
                update_hoven_turret_camera((char *)moby);
                data->f8C = 200.0f;
                moby->hit_slot = 0xFF;
                D_L12_00161F20++;
                moby->state = 4;
            }
        }
        break;
    }
    case HOVEN_TURRET_STATE_MANNED:
        D_L12_00161F00 = 1;
        ((Hero *)(D_0013E533 + 0xE1D))->unk20A5 = 1;
        ((Hero *)(D_0013E533 + 0xE1D))->collision_off_frames = scale_game_frames(0x78);
        ((Hero *)(D_0013E533 + 0xE1D))->invulnerable_frames = scale_game_frames(0x78);
        if (D_L12_0015F5C4_macro != 2 && D_L12_001619A0 == 0) {
            add_draw_callback(draw_hoven_turret_hud, moby);
        }
        update_hoven_turret_target_alignment_timer(moby);
        if (fast_dec_timer(&data->f90)) {
            Hit_304920 *hit = (Hit_304920 *)FUN_L00_0025a420(moby, 0x10000, 0);
            if (hit != 0) {
                data->f90 = scale_game_frames(4);
                data->f8C = data->f8C - hit->f2C;
            }
            moby->hit_slot = 0xFF;
        }
        if (D_L12_00161F24 == 0) {
            if (((Hero *)(D_0013E533 + 0xE1D))->ship_flags & 1) {
                if (data->f8C > 100.0f) {
                    D_L12_00161F20--;
                }
                ((Hero *)(D_0013E533 + 0xE1D))->ship_flags = 0;
                hero_respawn();
            }
            if (data->f8C <= 0.0f) {
                D_L12_00161F04 = 1;
                data->fAC = scale_game_frames(0x78);
                moby->state = 7;
                break;
            }
        }
        if (data->f94 != -1 &&
            (signed char)((struct Moby *)((data->f94 << 8) + (*(int *)&D_L12_0015FFD8_gp)))->state <
                0) {
            if (D_L12_0015F5C4_macro == 2) {
                if (data->b1 != 0) {
                    detach_manipulator(moby, data);
                }
                if (moby->prev_seq != 0) {
                    set_moby_animation(moby, 0, 0);
                }
                moby->pos.z = data->fA0;
                moby->rot.z = 3.1415927f;
            } else {
                jet_clear_persistent(moby->mission);
                queue_scene_movie_with_game_language(0xD);
                unlock_planet(0xD);
                {
                    Vec_304920 cpos;
                    Vec_304920 crot;

                    *(u128 *)&cpos = 0;
                    *(u128 *)&crot = 0;
                    cpos[0] = 328.4f;
                    cpos[1] = 338.7f;
                    cpos[2] = 60.98f;
                    crot[2] = -1.7f;

                    D_L12_00161F00 = 0;
                    set_race_camera(cpos, crot, 0, 1);
                }
                moby->state = 5;
                break;
            }
        }
        if (D_L12_001619A0 != 0) {
            break;
        }
        {
            float joy[2];
            float y;
            float target_yaw_step;
            float target_pitch_step;

            joy[0] = ((Hero *)(D_0013E533 + 0xE1D))->stick_x;
            y = ((Hero *)(D_0013E533 + 0xE1D))->stick_y;
            if (invert52_macro[0] == 0) {
                y = -y;
            }
            joy[1] = y;
            target_yaw_step = hoven_turret_yaw_input_rate * 0.017453292f * D_0015ED6C_gp * -joy[0];
            if ((data->f7C >= 0.0f && data->f7C < target_yaw_step) ||
                (data->f7C <= 0.0f && target_yaw_step < data->f7C)) {
                approach_value(&data->f7C,
                               target_yaw_step,
                               hoven_turret_yaw_acceleration_rate * 0.017453292f *
                                   frame_time_sq_macro);
            } else {
                approach_value(&data->f7C,
                               target_yaw_step,
                               hoven_turret_yaw_braking_rate * 0.017453292f * frame_time_sq_macro);
            }
            moby->rot.z = fast_add_rotations(moby->rot.z, data->f7C);
            target_pitch_step =
                -(hoven_turret_pitch_input_rate * 0.017453292f * D_0015ED6C_gp) * -joy[1];
            if ((data->f78 >= 0.0f && data->f78 < target_pitch_step) ||
                (data->f78 <= 0.0f && target_pitch_step < data->f78)) {
                approach_value(&data->f78,
                               target_pitch_step,
                               hoven_turret_pitch_acceleration_rate * 0.017453292f *
                                   frame_time_sq_macro);
            } else {
                approach_value(&data->f78,
                               target_pitch_step,
                               hoven_turret_pitch_braking_rate * 0.017453292f *
                                   frame_time_sq_macro);
            }
            data->f74 = fast_add_rotations(data->f74, data->f78);
            if (data->f74 > hoven_turret_pitch_max) {
                data->f74 = hoven_turret_pitch_max;
            }
            if (data->f74 < hoven_turret_pitch_min) {
                data->f74 = hoven_turret_pitch_min;
            }
            FUN_L00_001fff28(data->f10, 1, -data->f74);
            if (moby->anim_flags & 2) {
                blend_moby_animation(moby, 5, 0, 1);
            }
            fast_dec_timer_s16(&data->f70);
            if ((pad_held[0] & 0xA0) && data->f70 == 0) {
                Vec_304920 firing_offset;
                Vec_304920 muz;
                float      yaw;
                int        i;

                blend_moby_animation(moby, data->f80 == 1 ? 4 : 3, 0, 1);
                yaw = fast_add_rotations(moby->rot.z, 1.5707964f);
                build_spherical_offset(
                    firing_offset, hoven_turret_firing_offset_rate * D_0015ED6C_gp, yaw, data->f74);
                if (data->f80 == 1) {
                    data->f80 = 0;
                    moby_point_position(moby, 2, muz);
                } else {
                    moby_point_position(moby, 1, muz);
                    data->f80 = 1;
                }
                fast_vec_add(muz, muz, firing_offset);
                fast_vec_add(muz, muz, firing_offset);
                fast_vec_add(muz, muz, firing_offset);
                spawn_turret_rocket(moby, muz, firing_offset, 0);
                allocate_voice_for_target_entry(0, 0, (int)moby);
                l12_camera.shake_amount = 0.03f;
                l12_camera.shake_frames = scale_game_frames(0xA);
                l12_camera.shake2_amount = 0.03f;
                l12_camera.shake2_frames = scale_game_frames(7);
                data->f70 = scale_game_frames(0x14);
                for (i = 0; i < 5; i++) {
                    Vec_304920 randomized_firing_direction;
                    Vec_304920 rnd;

                    *(u128 *)rnd = 0;
                    rnd[0] = random_float_between(-1.0f, 1.0f);
                    rnd[1] = random_float_between(-1.0f, 1.0f);
                    rnd[2] = random_float_between(-1.0f, 1.0f);
                    *(u128 *)randomized_firing_direction = *(u128 *)rnd;
                    fast_vec_normalize(randomized_firing_direction,
                                       randomized_firing_direction,
                                       fast_vec_length(firing_offset) *
                                           (frame_scale_macro * (0.65f - 1.0f) + 1.0f));
                    fast_vec_add(
                        randomized_firing_direction, firing_offset, randomized_firing_direction);
                    fast_vec_normalize(
                        randomized_firing_direction,
                        randomized_firing_direction,
                        random_float_between(D_0015ED6C_gp * 3.0f, D_0015ED6C_gp * 6.0f));
                    FUN_L00_0026dd20(
                        muz,
                        randomized_firing_direction,
                        0x5F2F4F6F,
                        random_int_between(scale_game_frames(5), scale_game_frames(0xA)),
                        50000.0f);
                }
                fast_vec_normalize(firing_offset, firing_offset, D_0015ED6C_gp * 0.5f);
                FUN_L00_0026dd20(muz,
                                 firing_offset,
                                 0x2F4F7F7F,
                                 random_int_between(scale_game_frames(4), scale_game_frames(7)),
                                 500000.0f);
            }
            if (D_L12_001619A0 == 0) {
                update_hoven_turret_camera((char *)moby);
            }
        }
        break;
    case HOVEN_TURRET_STATE_DESTRUCTION_EFFECTS:
        add_draw_callback(draw_hoven_turret_destruction_fade, moby);
        if (fast_dec_timer_s16(&data->fAE)) {
            Vec_304920 explosion_position;
            float      randomized_view_offset_distance;
            float      randomized_view_yaw;
            float      randomized_view_pitch;
            float      t;

            randomized_view_offset_distance =
                random_float_between(hoven_turret_destruction_view_offset_distance_min,
                                     hoven_turret_destruction_view_offset_distance_max);
            randomized_view_yaw = fast_add_rotations(l12_camera.yaw, randf_sym(0.0f, 0.5235988f));
            randomized_view_pitch =
                fast_add_rotations(l12_camera.pitch, randf_sym(0.0f, 0.5235988f));
            qcopy(explosion_position, &l12_camera.pos);
            FUN_L00_00262cf0(explosion_position,
                             explosion_position,
                             randomized_view_offset_distance,
                             randomized_view_yaw,
                             randomized_view_pitch);
            t = random_float_between(0.5f, 1.3f);
            spawn_explosion(moby,
                            D_L12_0015F580_macro,
                            explosion_position,
                            0.0f,
                            0.0f,
                            7,
                            0xF,
                            0x19,
                            t,
                            t * 0.57f,
                            10.0f,
                            3,
                            0.7f,
                            0.0f,
                            0,
                            0,
                            -1,
                            0);
            data->fAE = scale_game_frames(random_int_between(5, 0xD));
        }
        if (fast_dec_timer_s16(&data->fAC)) {
            hero_respawn();
        }
        break;
    case HOVEN_TURRET_STATE_START_SCENE:
        if (D_L12_0015F5C4_macro != 2) {
            start_scene(1);
            moby->state = 8;
        }
        break;
    case HOVEN_TURRET_STATE_SAVE_DATA:
        if (D_L12_0015F5C4_macro != 2) {
            func_0020BFC8_304920(0, -1);
            moby->state = 6;
        }
        break;
    case HOVEN_TURRET_STATE_INACTIVE:
        if (moby->prev_seq != 0) {
            set_moby_animation(moby, 0, 0);
        }
        break;
    }
}
/* Draws the HUD gauge: a four-part frame, a 32-segment dial that fades with the data's charge, and two rotating needles. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003055F8), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x74];
    float f74;
    char  pad78[0x10];
    int   f88;
    float f8C;
} Data;

extern short hoven_turret_hud_gauge_center_x __asm__("D_L12_00161E14") __attribute__((sda));
extern short hoven_turret_hud_gauge_center_y __asm__("D_L12_00161E18") __attribute__((sda));
extern short hoven_turret_hud_frame_half_width __asm__("D_L12_00161E1C") __attribute__((sda));
extern short hoven_turret_hud_frame_half_height __asm__("D_L12_00161E20") __attribute__((sda));
extern short hoven_turret_hud_health_dial_radius_x __asm__("D_L12_00161E24") __attribute__((sda));
extern short hoven_turret_hud_health_dial_radius_y __asm__("D_L12_00161E28") __attribute__((sda));

#include "rnc/overlay/l12/mobys/hoven_turret_data/draw_hoven_turret_hud_2.h"

extern short hoven_turret_hud_orientation_indicator_radius_x __asm__("D_L12_00161E44")
    __attribute__((sda));
extern short hoven_turret_hud_orientation_indicator_radius_y __asm__("D_L12_00161E48")
    __attribute__((sda));

#include "rnc/overlay/l12/mobys/hoven_turret_data/draw_hoven_turret_hud.h"

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void        FUN_L02_0020bc88(long *m, int *b, int *a, long c, int flag);
extern long func_001F4868_3055F8(int) __asm__("FUN_001f44b8");
extern void func_L02_0020BF88_3055F8(void *, void *, void *, long, int) __asm__("FUN_L02_0020bc88");

void draw_hoven_turret_hud(Moby *moby) __asm__("FUN_L12_00304218");

void draw_hoven_turret_hud(Moby *moby) {
    int   uv[4];
    long  xy[4];
    int   colors[4];
    Data *data = *(Data **)&moby->pvars;
    long  texture;
    int   filled;
    int   color;
    int   dim_color;
    int   i;
    int   j;
    float angle;

    vu1_add_g_sregister(0x42, 0x8000000044L);
    uv[0] = hoven_turret_hud_frame_vertex_0_uv;
    uv[1] = hoven_turret_hud_frame_vertex_1_uv;
    uv[2] = hoven_turret_hud_frame_vertex_2_uv;
    uv[3] = hoven_turret_hud_frame_vertex_3_uv;
    colors[3] = hoven_turret_hud_frame_color;
    colors[2] = hoven_turret_hud_frame_color;
    colors[1] = hoven_turret_hud_frame_color;
    colors[0] = hoven_turret_hud_frame_color;
    texture = func_001F4868_3055F8(hoven_turret_hud_frame_texture_index_offset + 0x28);
    for (i = 0; i < 4; i++) {
        float sign_x;
        float sign_y;
        if (i & 1) {
            sign_x = -1.0f;
        } else {
            sign_x = 1.0f;
        }
        if (i >= 2) {
            sign_y = -1.0f;
        } else {
            sign_y = 1.0f;
        }
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] +=
            (u32)((truncate_float_to_s32((*(float *)&hoven_turret_hud_gauge_center_y +
                                          *(float *)&hoven_turret_hud_frame_half_height * sign_y) *
                                         16.0f)
                   << 16) -
                  (int)0x80000000);
        xy[0] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x +
                                        *(float *)&hoven_turret_hud_frame_half_width * sign_x)
                  << 4) +
                 0x8000;
        xy[1] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[1] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x +
                                        *(float *)&hoven_turret_hud_frame_half_width * sign_x)
                  << 4) +
                 0x8000;
        xy[2] +=
            (u32)((truncate_float_to_s32((*(float *)&hoven_turret_hud_gauge_center_y +
                                          *(float *)&hoven_turret_hud_frame_half_height * sign_y) *
                                         16.0f)
                   << 16) -
                  (int)0x80000000);
        xy[2] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        xy[3] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[3] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        func_L02_0020BF88_3055F8(xy, uv, colors, texture, 1);
    }
    texture = func_001F4868_3055F8(6);
    uv[0] = hoven_turret_hud_health_dial_vertex_0_uv;
    uv[1] = hoven_turret_hud_health_dial_vertex_1_uv;
    uv[2] = hoven_turret_hud_health_dial_vertex_2_uv;
    uv[3] = hoven_turret_hud_health_dial_vertex_3_uv;
    if (data->f8C > 100.0f) {
        color = fast_tween_color(0x6000FFFF, 0x6000FF00, (data->f8C - 100.0f) / 100.0f);
    } else {
        color = fast_tween_color(0x600000FF, 0x6000FFFF, data->f8C / 100.0f);
    }
    filled = truncate_float_to_s32((1.0f - data->f8C / 200.0f) * 32.0f);
    dim_color = (color & 0xFFFFFF) + 0x20000000;
    for (j = 0; j < 32; j++) {
        float start_angle, end_angle;
        int   x0, y0, x1, y1;
        if (j >= filled) {
            colors[3] = color;
            colors[2] = color;
            colors[1] = color;
            colors[0] = color;
        } else {
            colors[3] = dim_color;
            colors[2] = dim_color;
            colors[1] = dim_color;
            colors[0] = dim_color;
        }
        start_angle = fast_normalize_angle((float)j * 0.19634955f);
        end_angle = fast_add_rotations(start_angle, 0.19634955f);
        x0 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_x *
                                   fast_sin(start_angle));
        y0 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_y *
                                   -fast_cos(start_angle));
        x1 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_x *
                                   fast_sin(end_angle));
        y1 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_y *
                                   -fast_cos(end_angle));
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (u32)((truncate_float_to_s32(
                            (*(float *)&hoven_turret_hud_gauge_center_y + (float)y1) * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[0] +=
            (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x + (float)x1) << 4) +
            0x8000;
        xy[1] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[1] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        xy[2] += (u32)((truncate_float_to_s32(
                            (*(float *)&hoven_turret_hud_gauge_center_y + (float)y0) * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[2] +=
            (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x + (float)x0) << 4) +
            0x8000;
        xy[3] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[3] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        func_L02_0020BF88_3055F8(xy, uv, colors, texture, 1);
    }
    if (scale_game_frames(0xF) < data->f88) {
        color = fast_tween_color(0x5000FFFF,
                                 0x500000FF,
                                 convert_integer_to_float(data->f88 - scale_game_frames(0xF)) /
                                     (float)scale_game_frames(0xF));
    } else {
        color =
            fast_tween_color(0x5000FF00,
                             0x5000FFFF,
                             convert_integer_to_float(data->f88) / (float)scale_game_frames(0xF));
    }
    uv[0] = hoven_turret_hud_orientation_indicator_vertex_0_uv;
    uv[1] = hoven_turret_hud_orientation_indicator_vertex_1_uv;
    uv[2] = hoven_turret_hud_orientation_indicator_vertex_2_uv;
    uv[3] = hoven_turret_hud_orientation_indicator_vertex_3_uv;
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, 0.0f));
    xy[0] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[0] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, 1.5707964f));
    xy[1] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[1] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, -1.5707964f));
    xy[2] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[2] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, 3.1415927f));
    xy[3] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[3] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    func_L02_0020BF88_3055F8(
        xy, uv, colors, get_effect_texture(hoven_turret_hud_yaw_indicator_texture_index), 1);
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, 0.0f));
    xy[0] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[0] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, 1.5707964f));
    xy[1] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[1] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, -1.5707964f));
    xy[2] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[2] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, 3.1415927f));
    xy[3] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[3] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    func_L02_0020BF88_3055F8(
        xy, uv, colors, get_effect_texture(hoven_turret_hud_pitch_indicator_texture_index), 1);
}

/* Scale and play the vendor cue while its counter is below the limit. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00306178), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0xAC];
    short value;
} Level12VendorCounterData;

typedef struct {
    char                      pad0[0x78];
    Level12VendorCounterData *data;
} Level12VendorCounterMoby;

void draw_hoven_turret_destruction_fade(Level12VendorCounterMoby *moby) __asm__("FUN_L12_00304d98");

void draw_hoven_turret_destruction_fade(Level12VendorCounterMoby *moby) {
    Level12VendorCounterData *data = moby->data;
    int                       count = scale_game_frames(0x50);
    int                       value = data->value;
    if (value < count) {
        emit_rgba_draw_packet(0xAA, 0, 0, (count - value) * 0x50 / count);
    }
}
