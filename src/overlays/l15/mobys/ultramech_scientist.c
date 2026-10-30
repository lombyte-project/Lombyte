/* Level 15 ultramech scientist moby class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/game/queue_planet_unlock_message.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/level_state.h"

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern int D_L15_0015F5C4;

extern void start_scene(int) __asm__("FUN_L00_00298840");

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");

extern float fast_add_rotations(float, float);

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

extern char *D_L15_001600EC;

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

extern void unlock_planet(int) __asm__("FUN_L00_002607d0");

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

extern char *D_L15_0015FFE4 MACRO_ADDR;

extern char D_0013E533[];

extern f32 frame_scale_sq_gp __asm__("D_0015ED64") MACRO_ADDR;

extern s32 current_level_index_gp __asm__("D_0015ED84") MACRO_ADDR;

extern int update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");

extern int initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");

extern int count_active_moby_group_members(int, int) __asm__("FUN_L01_0026e008");

extern unsigned char big_head_cheat_enabled_macro[] __asm__("D_0015EDB0") MACRO_ADDR;

extern void FUN_L00_002502a0(int);

extern void FUN_L00_00284e50(void *, void *);

extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");

extern void apply_npc_big_head_cheat(void *) __asm__("FUN_L12_002e6b60");

extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");

extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");

void ultramech_scientist_update(struct Moby *moby) __asm__("FUN_L15_002ec760");

void ultramech_scientist_update(struct Moby *moby) {
    enum {
        ULTRAMECH_SCIENTIST_INITIALIZE = 0,
        ULTRAMECH_SCIENTIST_WAIT_FOR_DIALOGUE = 1,
        ULTRAMECH_SCIENTIST_RESUME_AFTER_DIALOGUE = 2,
        ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_CONDITIONS = 3,
        ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_DELAY = 4,
        ULTRAMECH_SCIENTIST_START_FIRST_SCENE = 5,
        ULTRAMECH_SCIENTIST_WAIT_FOR_FIRST_SCENE = 6,
        ULTRAMECH_SCIENTIST_START_SECOND_SCENE_AND_SAVE = 7,
        ULTRAMECH_SCIENTIST_SAVE_AND_DISABLE = 8
    };
    char        *data;
    char        *hero_pos;
    struct Moby *other;
    int          control_mode;

    data = (char *)moby->pvars;
    apply_npc_big_head_cheat(moby);
    if (*(int *)(data + 0x14C) == -1) {
        return;
    }
    {
        char *player = D_0013E533 + 0xE1D;
        if (*(unsigned char *)(player + 0x20A4) != 2) {
            int mobys = (int)l15_moby_array;
            *(int *)((*(int *)(data + 0x14C) << 8) + mobys + 0x94) = 0;
            *(unsigned short *)((*(int *)(data + 0x14C) << 8) + mobys + 0x34) |= 0x41;
        }
    }
    if (moby->state == 9) {
        moby->update_dist = 0;
        moby->collision = 0;
        moby->flags |= 0x41;
        return;
    }
    if (D_L15_0015F5C4 == 2) {
        moby->collision = 0;
        moby->flags |= 0x41;
    } else {
        moby->collision = moby->pclass->collision;
        moby->flags &= 0xFFBE;
        initialize_moby_grounding(moby);
    }

    switch (moby->state) {
    case ULTRAMECH_SCIENTIST_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        *(float *)(data + 0xC) = 20.0f;
        initialize_npc_dialogue(moby, data);
        if (visited52[*(int *)(data + 0x280) + (current_level_index_gp << 4)] != 0xFF) {
            break;
        }
        if (level_available[16] != 0) {
            moby->state = 9;
            break;
        }
        moby->state = 3;
        other = (struct Moby *)D_L15_0015FFE4;
        if (other == 0) {
            break;
        }
        {
            int dead_state = 0xFE;
            int gone_state = 0xFD;
            for (; other != 0; other = other->next) {
                if (other->mission != *(int *)(data + 0x280)) {
                    continue;
                }
                if (other == moby) {
                    continue;
                }
                if (other == 0) {
                    continue;
                }
                if (other->state == dead_state) {
                    continue;
                }
                if (other->state == gone_state) {
                    continue;
                }
                mark_moby_for_removal(other);
            }
        }
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_DIALOGUE:
        hero_pos = D_0013E533 + 0xE9D;
        if (is_point_inside_clip_volume(hero_pos, *(int *)(data + 0x27C)) == 0) {
            break;
        }
        control_mode = *(int *)(hero_pos + 0x200C);
        if ((unsigned int)control_mode >= 2 && control_mode != 9) {
            break;
        }
        if (update_npc_dialogue(moby, data) == 0) {
            break;
        }
        FUN_L00_002502a0(*(int *)(data + 0x280));
        moby->anim_speed = 0.0f;
        moby->state = 2;
        other = (struct Moby *)D_L15_0015FFE4;
        if (other == 0) {
            break;
        }
        {
            int dead_state = 0xFE;
            int gone_state = 0xFD;
            for (; other != 0; other = other->next) {
                if (other->mission != *(int *)(data + 0x280)) {
                    continue;
                }
                if (other == moby) {
                    continue;
                }
                if (other == 0) {
                    continue;
                }
                if (other->state == dead_state) {
                    continue;
                }
                if (other->state == gone_state) {
                    continue;
                }
                mark_moby_for_removal(other);
            }
        }
        break;
    case ULTRAMECH_SCIENTIST_RESUME_AFTER_DIALOGUE:
        if (D_L15_0015F5C4 == 2) {
            break;
        }
        moby->anim_speed = 1.0f;
        if (*(short *)(data + 0x4) == 0) {
            if (*(int *)(data + 0x150) != -1) {
                char *paths = D_L15_001600EC;
                FUN_L00_00284e50(paths + (*(int *)(data + 0x150) << 7) + 0x30,
                                 paths + (*(int *)(data + 0x150) << 7) + 0x70);
            }
            moby->state = 3;
        } else {
            moby->state = 9;
        }
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_CONDITIONS: {
        char *player = D_0013E533 + 0xE1D;
        if (*(unsigned char *)(player + 0x20A4) != 2) {
            break;
        }
    }
        if (*(int *)(data + 0x158) != -1) {
            char *spawned = (char *)l15_moby_array + (*(int *)(data + 0x158) << 8);
            if (spawned != 0) {
                int state = *(unsigned char *)(spawned + 0x20);
                if (state != 0xFE) {
                    if (state != 0xFD) {
                        mark_moby_for_removal(spawned);
                        *(int *)(data + 0x158) = -1;
                    }
                }
            }
        }
        if (count_active_moby_group_members(*(int *)(data + 0x144), 0x40) != 0) {
            break;
        }
        if (count_active_moby_group_members(*(int *)(data + 0x148), 0x40) != 0) {
            break;
        }
        moby->state = 4;
        *(int *)(data + 0x15C) = scale_game_frames(120);
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_DELAY:
        if (fast_dec_timer(data + 0x15C) == 0) {
            break;
        }
        restore_saved_hero_moby_and_clear_vehicle_mode();
        set_moby_animation((char *)l15_moby_array + (*(int *)(data + 0x14C) << 8), 0, 0);
        *(int *)(data + 0x140) = 1;
        *(short *)(data + 0x36) = 1;
        *(unsigned char *)(data + 0x8) = 1;
        unlock_planet(0x10);
        FUN_L00_002502a0(*(int *)(data + 0x284));
        request_music_track(0, 5);
        if (*(int *)(data + 0x154) != -1) {
            char *paths = D_L15_001600EC;
            char *paths_again;
            set_race_camera(paths + (*(int *)(data + 0x154) << 7) + 0x30,
                            paths + (*(int *)(data + 0x154) << 7) + 0x70,
                            0,
                            1);
            paths_again = D_L15_001600EC;
            FUN_L00_00284e50(paths_again + (*(int *)(data + 0x154) << 7) + 0x30,
                             paths_again + (*(int *)(data + 0x154) << 7) + 0x70);
        }
        moby->state = 5;
        break;
    case ULTRAMECH_SCIENTIST_START_FIRST_SCENE:
        start_scene(1);
        moby->state = 6;
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_FIRST_SCENE:
        if (D_L15_0015F5C4 == 2) {
            break;
        }
        queue_scene_movie_with_game_language(0x10);
        moby->state = 7;
        break;
    case ULTRAMECH_SCIENTIST_START_SECOND_SCENE_AND_SAVE:
        if (D_L15_0015F5C4 != 2) {
            queue_planet_unlock_message(0x10);
            start_scene(2);
            moby->state = 8;
        }
        memcard_save_data(0, -1);
        moby->state = 9;
        break;
    case ULTRAMECH_SCIENTIST_SAVE_AND_DISABLE:
        memcard_save_data(0, -1);
        moby->state = 9;
        break;
    default:
        break;
    }

    {
        char *player;
        float target[4];
        float eye[4];
        float eye_to_target_vector[4];
        float value;
        float pitch;
        float yaw;
        float deg_to_rad;
        float heading;
        float ease_rate;
        float ease_limit;
        /* Fakematch: an always-true flag around the aiming block keeps retail's register allocation. */
        int aim = 0;
        ease_rate = 0.02f;
        ease_limit = 0.3f;
        aim = 1;
        value = vector_distance_xy(&moby->pos, D_0013E533 + 0xE9D);
        if (value < 8.0f) {
            player = D_0013E533 + 0xE1D;
            value = fast_arc_tan(*(float *)(player + 0xD0) - moby->pos.x,
                                 *(float *)(player + 0xD4) - moby->pos.y);
            value = fast_difference_between_rotations(moby->rot.z, value);
            if (value < 1.5707964f) {
                value = fast_vec_length(player + 0x100);
                if (0.01f < value) {
                    *(int *)(data + 0x274) = scale_game_frames(120);
                } else {
                    fast_dec_timer(data + 0x274);
                }
                goto wander;
            }
        }
        if (*(int *)(data + 0x274) != 0) {
            *(int *)(data + 0x274) = 0;
            qcopy(data + 0x260, D_0013E533 + 0xE1D + 0xD0);
        }
    wander:
        if (fast_dec_timer(data + 0x278) == 0) {
            goto aim_head;
        }
        deg_to_rad = 0.017453292f;
        value = random_float_between(180.0f, 300.0f);
        value = multiply_global_factor_ed64(value);
        *(int *)(data + 0x278) = truncate_float_to_s32(value);
        value = random_float_between(-90.0f, 90.0f);
        heading = fast_add_rotations(moby->rot.z, value * deg_to_rad);
        value = random_float_between(0.0f, 30.0f);
        build_spherical_offset(data + 0x260, 6.0f, heading, value * deg_to_rad);
        fast_vec_add(data + 0x260, data + 0x260, &moby->pos);
    aim_head:
        if (*(int *)(data + 0x274) != 0) {
            qcopy(target, D_0013E533 + 0xE1D + 0xD0);
            ease_rate = 0.04f;
            ease_limit = 0.3f;
        } else {
            qcopy(target, data + 0x260);
        }
        if (aim != 0) {
            qcopy(eye, &moby->pos);
            eye[2] = eye[2] + 1.0f;
            fast_vec_sub(eye_to_target_vector, target, eye);
            value = fast_arc_tan(eye_to_target_vector[0], eye_to_target_vector[1]);
            yaw = fast_subtract_rotations(value, moby->rot.z);
            value = vector_length_xy(eye_to_target_vector);
            value = fast_arc_tan(value, eye_to_target_vector[2]);
            pitch = -value;
            if (yaw > 1.5707964f) {
                yaw = 1.5707964f;
            } else if (yaw < -1.5707964f) {
                yaw = -1.5707964f;
            }
            if (pitch > 0.5235988f) {
                pitch = 0.5235988f;
            } else if (pitch < -0.5235988f) {
                pitch = -0.5235988f;
            }
            *(float *)(data + 0x1C8) = yaw * 0.5f;
            *(float *)(data + 0x1C4) = pitch * 1.25f;
            *(float *)(data + 0x248) = yaw * 0.5f;
        }
        if (big_head_cheat_enabled_macro[0] != 0) {
            *(float *)(data + 0x1D0) = 2.75f;
        }
        update_joint_animation(
            moby, data + 0x160, 1, ease_rate * frame_scale_sq_gp, ease_limit * frame_scale_sq_gp);
        update_joint_animation(
            moby, data + 0x1E0, 0, ease_rate * frame_scale_sq_gp, ease_limit * frame_scale_sq_gp);
    }
}
