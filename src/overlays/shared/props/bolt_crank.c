/* Shared moby class bolt_crank. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/l01/hero/hero_set_state.h"
#include "rnc/overlay/shared/mobys/record_moby_context_bytes.h"
#include "qcopy.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/options.h"
#include "rnc/overlay/cuboid.h"

/* Bolt-crank Moby.state, including the callback-only state. */
enum BoltCrankState {
    BOLT_CRANK_INIT = 0x0,
    BOLT_CRANK_IDLE = 0x1,
    BOLT_CRANK_CALLBACK_ONLY = 0x2,
    BOLT_CRANK_TURNING = 0x3,
    BOLT_CRANK_LOWERING = 0x4,
    BOLT_CRANK_DONE = 0x5
};

typedef struct {
    f32 progress; /* 0x00: turned / (turns * 2 pi), 0..1 */
    u8  pad_04[0x10];
    s32 idle_frames;       /* 0x14 */
    f32 turned;            /* 0x18 */
    f32 speed;             /* 0x1C */
    s32 camera_point;      /* 0x20 */
    f32 fall_speed;        /* 0x24 */
    f32 camera_params[2];  /* 0x28 */
    f32 turns;             /* 0x30 */
    s32 exit_camera_point; /* 0x34: -1 none */
    s32 reusable;          /* 0x38: back to idle when done */
    f32 rest_height;       /* 0x3C */
} BoltCrankVars;

extern char D_L01_00167240[];
float       absolute_float(float input) __asm__("func_001F99C0");

extern int           D_0015ED84;
extern unsigned char menu_item_enabled_1_lui __asm__("D_0013D395") NOT_SDA;

extern char  D_L01_001BAAA0[];
extern f32   D_L01_00167258_lui __asm__("D_L01_00167258") NOT_SDA;
extern char *D_L01_00167280_lui __asm__("D_L01_00167280") NOT_SDA;
f32          bolt_crank_radial_correction_step_gp __asm__("D_L01_001618A0") = 0;
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern void  divide_vector_xyz(void *, void *, f32) __asm__("FUN_L00_001ff378");
extern void  vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
extern f32   step_angle_toward_target(f32 *, f32, int, f32) __asm__("FUN_L00_0025bc00");
extern int   pad_pressed_within(int, int, int *) __asm__("FUN_L00_00266d60");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  jet_end_mount(int) __asm__("FUN_L00_002eac18");
extern void  set_camera_5_track_acceleration_yz(f32, f32) __asm__("FUN_L01_00316e88");

/* On level 1, when a state-5 moby of type 0x118 has sub-id 0x34 or 0x35, sets the matching flag byte (D_0013D394 / D_0013D395). */

void set_novalis_bolt_crank_completion_flags(char *moby) __asm__("FUN_L01_002e0bd8");

void set_novalis_bolt_crank_completion_flags(char *moby) {
    int level = current_level_index;
    if (level != 1)
        return;
    if (((short)MOBY(moby)->save_id) == 0x34 && MOBY(moby)->oclass == 0x118 &&
        (unsigned char)moby[0x20] == 5) {
        menu_item_enabled_0 = level;
        level = D_0015ED84;
    }
    if (level == 1 && ((short)MOBY(moby)->save_id) == 0x35 && MOBY(moby)->oclass == 0x118 &&
        (unsigned char)moby[0x20] == 5) {
        menu_item_enabled_1_lui = level;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E1FE0). */

/* Turned by the hero (state 0x3B) until progress reaches 1, then lowered and saved as done. */

void bolt_crank_update(struct Moby *moby) __asm__("FUN_L01_002e0c68");

/* Moby class 280 */
void bolt_crank_update(struct Moby *moby) {
    BoltCrankVars *crank = (BoltCrankVars *)moby->pvars;
    float          matrix[16];
    float          stick[4], step[4], facing[4];
    Vec4           rotation, target;
    float          offset[4], spring_pos;
    float          distance, yaw_diff, angle, old_angle, height_diff, max_speed, height, ground;
    int            save_id, area_clear;
    struct Moby   *clearance_candidate_moby;

    if (crank == 0) {
        moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 6.2831855f);
        return;
    }

    if (hero.state.current != HERO_STATE_CRANK) {
        if (moby->state == BOLT_CRANK_TURNING) {
            moby->state = BOLT_CRANK_IDLE;
            if (D_L01_00167280_lui == 0) {
                return;
            }
            jet_end_mount(4);
            return;
        }
    }
    switch (moby->state) {
    case BOLT_CRANK_INIT:
        crank->rest_height = moby->pos.z;
        moby->state = BOLT_CRANK_IDLE;
        goto done;
    case BOLT_CRANK_IDLE: {
        save_id = moby->save_id;
        if (l01_save_state.cleared[(short)save_id] != 0 ||
            ((D_0014C190_257218[current_level_index][(short)save_id >> 5] >> (save_id & 0x1F)) &
             1)) {
            moby->pos.z = moby->pos.z - 0.5f;
            moby->state = BOLT_CRANK_DONE;
            crank->progress = 1.0f;
            goto done;
        }
        distance = vector_distance_xy(&moby->pos, &hero.motion.pos);
        if (distance < 1.75f) {
            if (hero.vehicle_mode == 0) {
                angle = fast_arc_tan(moby->pos.x - hero.motion.pos.f[0],
                                     moby->pos.y - hero.motion.pos.f[1]);
                yaw_diff = fast_difference_between_rotations(hero.motion.rot.f[2], angle);
                if (yaw_diff < 1.5707964f) {
                    height_diff = absolute_float((hero.motion.pos.f[2] - 0.3f) - moby->pos.z);
                    if (height_diff < 0.2f && hero.items[0].item_id == 8 &&
                        hero.items[0].moby != 0) {
                        int pressed = pad_pressed_within(0x80, scale_game_frames(7), 0);
                        if (pressed != 0 &&
                            (unsigned int)scale_game_frames(0x1E) <
                                (unsigned int)crank->idle_frames &&
                            moby->pos.z == crank->rest_height) {
                            if (crank->progress < 1.0f) {
                                area_clear = 1;
                                crank->idle_frames = 0;
                                bolt_crank_radial_correction_step_gp = 0.0f;
                                for (clearance_candidate_moby = (struct Moby *)D_L01_0015FFE4;
                                     clearance_candidate_moby != 0;
                                     clearance_candidate_moby = clearance_candidate_moby->next) {
                                    if (clearance_candidate_moby->flags & 0x1000) {
                                        if (vector_distance_xy(&moby->pos,
                                                               &clearance_candidate_moby->pos) <
                                            3.25f) {
                                            area_clear = 0;
                                            break;
                                        }
                                    }
                                }
                                if (area_clear == 0) {
                                    goto done;
                                }
                                l01_hero_set_state(HERO_STATE_CRANK, 1);
                                jet_mount(D_L01_00167240,
                                          D_L01_00167240 + 0x10,
                                          3,
                                          scale_game_frames(0xB4),
                                          0);
                                set_camera_rotation(&l01_cuboids[crank->camera_point].rot);
                                set_camera_position(&l01_cuboids[crank->camera_point].pos);
                                set_camera_5_track_acceleration_yz(crank->camera_params[0],
                                                                   crank->camera_params[1]);
                                moby->state = BOLT_CRANK_TURNING;
                                crank->fall_speed = 0.0f;
                                crank->turned = crank->progress * (crank->turns * 6.2831855f);
                                goto done;
                            }
                        }
                    }
                }
            }
        }
        if (crank->progress < 1.0f) {
            crank->idle_frames = crank->idle_frames + 1;
            approach_value(&crank->progress, 0.0f, 1.0f / (float)scale_game_frames(0x3C));
            if (0.0f < crank->progress) {
                moby->rot.z = fast_subtract_rotations(
                    moby->rot.z, crank->turns * 6.2831855f / (float)scale_game_frames(0x3C));
            }
            moby->pos.z = moby->pos.z + frame_time * 0.5f;
            if (crank->rest_height <= moby->pos.z) {
                moby->pos.z = crank->rest_height;
            }
        } else {
            if (crank->reusable == 0) {
                goto done;
            }
            if (crank->exit_camera_point != -1 &&
                visited52[moby->mission + (current_level_index << 4)] != 0xFF) {
                record_moby_context_bytes(crank, 4, moby, 2, D_L01_001BAAA0);
                jet_clear_persistent(moby->mission);
                if (crank->exit_camera_point != -1) {
                    LevelCuboid *point = &l01_cuboids[crank->exit_camera_point];
                    jet_reset_camera(&point->pos, &point->rot);
                }
            }
            moby->pos.z = moby->pos.z - frame_time * 0.5f;
            {
                float lowest = crank->rest_height - 0.5f;
                if (moby->pos.z <= lowest) {
                    moby->pos.z = lowest;
                }
            }
        }
        goto done;
    }
    case BOLT_CRANK_CALLBACK_ONLY:
        goto done;
    case BOLT_CRANK_TURNING:
        target.q = 0;
        target.f[2] = D_L01_00167258_lui;
        rotation.q = target.q;
        old_angle =
            fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
        if ((pad_pressed_within(0x80, scale_game_frames(7), 0) != 0 &&
             scale_game_frames(0x3C) < hero.state_timer) ||
            crank->progress == 1.0f) {
            l01_hero_set_state(0, 1);
            if (crank->reusable != 0) {
                moby->state = BOLT_CRANK_IDLE;
            } else if (crank->progress == 1.0f) {
                record_moby_context_bytes(crank, 4, moby, 2, D_L01_001BAAA0);
                moby->state = BOLT_CRANK_LOWERING;
                D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |=
                    1 << (moby->save_id & 0x1F);
                moby_killed[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                allocate_voice_for_target_entry(0, 0, moby);
                if (crank->exit_camera_point != -1 &&
                    visited52[moby->mission + (current_level_index << 4)] != 0xFF) {
                    record_moby_context_bytes(crank, 4, moby, 2, D_L01_001BAAA0);
                    jet_clear_persistent(moby->mission);
                    if (crank->exit_camera_point != -1) {
                        LevelCuboid *point = &l01_cuboids[crank->exit_camera_point];
                        jet_reset_camera(&point->pos, &point->rot);
                    }
                }
            }
            jet_end_mount(4);
            return;
        }
        stick[1] = -hero.stick_x;
        stick[0] = -hero.stick_y;
        distance = vector_length_xy(stick);
        if (1.0f < distance) {
            divide_vector_xyz(stick, stick, distance);
        }
        euler_rotation_matrix(matrix, &rotation);
        transform_vector(stick, stick, matrix);
        facing[0] = fast_cos(hero.moby->rot.z);
        facing[1] = fast_sin(hero.moby->rot.z);
        facing[2] = 0.0f;
        distance = fast_vec_dot(facing, stick);
        {
            float max_speed_change = frame_time_sq * 5.5f;
            float change = distance - crank->speed;
            if (max_speed_change < change)
                change = max_speed_change;
            if (change < -max_speed_change)
                change = -max_speed_change;
            max_speed = frame_time * 3.7f;
            crank->speed = crank->speed + change;
        }
        if (max_speed < crank->speed)
            crank->speed = max_speed;
        if (crank->speed < 0.0f)
            crank->speed = 0.0f;
        if (absolute_float(crank->speed) < frame_time_sq * 5.5f * 0.25f)
            crank->speed = 0.0f;
        fast_vec_normalize(step, facing, crank->speed);

        if (hero.anim_flags & 2) {
            if (hero.moby->prev_seq == 0x44) {
                set_hero_animation(0x3F, 0, (float)scale_game_frames(7));
            }
        }
        if (hero.moby->prev_seq != 0x44) {
            if (hero.anim_changing == 0) {
                if (hero.moby->seq == 0x40 && absolute_float(crank->speed) == 0.0f) {
                    set_hero_animation(0x3F, 0, (float)scale_game_frames(0xE));
                } else {
                    if (hero.moby->seq == 0x3F) {
                        if (0.0f < crank->speed) {
                            set_hero_animation(0x40, 0, (float)scale_game_frames(0xE));
                        }
                    } else if (hero.moby->seq == 0x40 && frame_time * 3.7f * 0.6f < crank->speed) {
                        set_hero_animation(0x41, 0, (float)scale_game_frames(0xE));
                    } else if (hero.moby->seq == 0x41 && crank->speed < frame_time * 3.7f * 0.4f) {
                        set_hero_animation(0x40, 0, (float)scale_game_frames(0x10));
                    }
                }
            }
        }

        {
            struct Hero *player = &hero;
            height = player->motion.pos.f[2];
            fast_vec_add(&player->motion.pos, &player->motion.pos, step);
            angle = fast_arc_tan(player->motion.pos.f[0] - moby->pos.x,
                                 player->motion.pos.f[1] - moby->pos.y);
            step[0] = fast_cos(angle) * 1.05f;
            step[1] = fast_sin(angle) * 1.05f;
            step[2] = 0.0f;
            fast_vec_add(&target, &moby->pos, step);
            fast_vec_sub(offset, &target, &player->motion.pos);
            offset[2] = 0.0f;
            distance = vector_length_xy(offset);
            spring_pos = -distance;
            vendor_spring(&spring_pos,
                          &bolt_crank_radial_correction_step_gp,
                          0.0f,
                          frame_scale_sq * 0.015f,
                          frame_scale_sq * 0.3f,
                          frame_time * 7.0f);
            fast_vec_normalize(offset, offset, bolt_crank_radial_correction_step_gp);
            fast_vec_add(&player->motion.pos, &player->motion.pos, offset);
            player->motion.pos.f[2] = height;
            if (opt_mirror_anim) {
                player->motion.move_yaw = fast_add_rotations(-1.5707964f, angle);
            } else {
                player->motion.move_yaw = fast_add_rotations(1.5707964f, angle);
            }
        }
        step_angle_toward_target(
            &hero.motion.rot.f[2], hero.motion.move_yaw, 0, frame_time * 6.2831855f);
        crank->fall_speed = crank->fall_speed + frame_time_sq * 9.8f;
        hero.motion.pos.f[2] = hero.motion.pos.f[2] - (crank->fall_speed - 2.0f);
        ground = probe_ground_height(&hero.motion.pos, 0, 0.5f);
        hero.motion.pos.f[2] = hero.motion.pos.f[2] - 2.0f;
        if (hero.motion.pos.f[2] < ground) {
            hero.motion.pos.f[2] = ground;
            crank->fall_speed = 0.0f;
        }
        if (hero.moby->prev_seq != 0x44) {
            float limit = 1.0471976f;
            float handle_yaw = fast_add_rotations(0.2617994f, angle);
            step_angle_toward_target(&moby->rot.z, handle_yaw, 0, frame_time * 3.1415927f);
            if (limit < fast_subtract_rotations(moby->rot.z, handle_yaw)) {
                moby->rot.z = fast_subtract_rotations(moby->rot.z, limit);
            } else if (fast_subtract_rotations(moby->rot.z, handle_yaw) < -1.0471976f) {
                moby->rot.z = fast_add_rotations(moby->rot.z, limit);
            }
        }
        yaw_diff = fast_difference_between_rotations(angle, old_angle);
        crank->turned = crank->turned + yaw_diff;
        crank->progress = crank->turned / (crank->turns * 6.2831855f);
        if (1.0f < crank->progress)
            crank->progress = 1.0f;
        if (crank->progress < 0.0f)
            crank->progress = 0.0f;
        fast_mem_zero16(&hero.motion.velocity, 0x90);
        goto done;
    case BOLT_CRANK_LOWERING:
        moby->pos.z = moby->pos.z - frame_time * 0.5f;
        {
            float lowest = crank->rest_height - 0.5f;
            if (moby->pos.z <= lowest) {
                moby->pos.z = lowest;
                moby->state = BOLT_CRANK_DONE;
                crank->progress = 1.0f;
            }
        }
        goto done;
    case BOLT_CRANK_DONE:
        crank->progress = 1.0f;
        goto done;
    }
done:
    set_novalis_bolt_crank_completion_flags((char *)moby);
}

char D_L04_001BA7A0 NOT_SDA = 0;
f32                 D_L04_00161840 = 0;
char D_L08_001BAE20 NOT_SDA = 0;
f32                 D_L08_00161930 = 0;
