/* Level 13 Qwark's starfighter moby class: voice lines, joint attachments and effects, path steering, damage, death, weapons, attack commands and tractor beam. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/music/music_stream_state.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/overlay/shared/camera/path_flythrough_camera.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/l13/mobys/mine.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"
#include "qzero.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/fun_00213928.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/l13/mobys/qwarks_starfighter_shield.h"

extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");

extern float D_0015ED6C;

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern char D_00140940[];

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern int FUN_L00_00257b90(int, int);

extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");

extern void FUN_L00_00257470(struct Moby *, int, int);

extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern char *D_L13_001B07B0[];

extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);

float absolute_float(float input) __asm__("func_001F99C0");

void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

extern char *D_L13_0015FFD8;

extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");

extern char D_0013E550[];

extern s32 random_integer_below(s32) __asm__("FUN_00213260");

extern int rand_q(void) __asm__("FUN_001160d8");

extern int random_integer_below(int) __asm__("FUN_00213260");

extern unsigned char l13_starfighter_voice_line_ids[] __asm__("D_L13_001D3558");

int queue_starfighter_voice_line(void          *unused,
                                 unsigned char *moby_data,
                                 int            base,
                                 int            line_count) __asm__("FUN_L13_002e86f8");

int queue_starfighter_voice_line(void *unused, unsigned char *moby_data, int base, int line_count) {
    int j = rand_q() % line_count;
    int id = l13_starfighter_voice_line_ids[base + j];
    int i;
    int lt;
    if (base < 0x1D) {
        if (base < 0xC) {
            if (moby_data[0x148] != 0)
                return 0;
            if (*(short *)(moby_data + 0x144) > 0)
                return 0;
        } else {
            if (*(short *)(moby_data + 0x146) > 0)
                return 0;
            if (moby_data[0x148] != 0 && base >= 0x17 && base < 0x1B)
                return 0;
        }
    }
    lt = base < 0x1D;
    if (music_stream_state.secondary.handle != 0 ||
        music_stream_state.queued_secondary_track != -1) {
        if (lt)
            return 0;
    }
    if (base < 0x1D) {
        for (i = 0; i < line_count; i++) {
            if ((*(int *)(moby_data + 0x13C) >> id) & 1) {
                if (base < 0xC && random_integer_below(4) == 0)
                    *(int *)(moby_data + 0x13C) &= ~(1 << id);
                j++;
                j %= line_count;
                id = l13_starfighter_voice_line_ids[base + j];
            } else {
                music_stream_state.queued_secondary_track = id + 50000;
                moby_data[0x13A] = 1;
                *(int *)(moby_data + 0x13C) |= 1 << id;
                return 1;
            }
        }
        if (base < 0xC)
            *(int *)(moby_data + 0x13C) &= ~(1 << id);
    } else {
        music_stream_state.queued_secondary_track = id + 50000;
        moby_data[0x13A] = 1;
        return 1;
    }
    return 0;
}

extern int D_L13_001D3478[] __asm__("D_L13_001D3478");

typedef struct {
    char pad[0xC4];
    int  on[1];
} Vd_v;

extern int joint_effect_gate_slot_indices[] __asm__("D_L13_001D3508");

extern int gated_effect_joint_indices[] __asm__("D_L13_001D3520");

void emit_gated_moby_joint_effects(struct Moby *source_moby,
                                   char        *emission_gate_data) __asm__("FUN_L13_002e8920");

void emit_gated_moby_joint_effects(struct Moby *source_moby, char *emission_gate_data) {
    OvlQuad zv;
    float   bone_position[4];
    int     i;
    for (i = 0; i < 6; i++) {
        if (((Vd_v *)emission_gate_data)->on[joint_effect_gate_slot_indices[i]] != 0) {
            float scale_ratio;
            int   effect_frame_count, k;
            zv = 0;
            scale_ratio = source_moby->scale / (source_moby->pclass->scale * 4.0f);
            moby_point_position(source_moby, gated_effect_joint_indices[i], bone_position);
            if (frame_count & 1) {
                effect_frame_count = scale_game_frames(random_int_between(0xF, 0x14));
                spawn_effect(bone_position,
                             &zv,
                             0xCF0000FF,
                             0xCF,
                             effect_frame_count,
                             truncate_float_to_s32(scale_ratio * 350.0f),
                             -100,
                             1);
            } else if (source_moby->command == 0 ||
                       D_L13_001D3478[source_moby->command - 1] !=
                           joint_effect_gate_slot_indices[i] ||
                       random_integer_below(9) == 0) {
                effect_frame_count = scale_game_frames(random_int_between(0xF, 0x16));
                spawn_effect(bone_position,
                             &zv,
                             0x6000FFFF,
                             0x80,
                             effect_frame_count,
                             truncate_float_to_s32(scale_ratio * 600.0f),
                             -100,
                             1);
            }
            k = truncate_float_to_s32(random_float_between(100.0f, 250.0f) * scale_ratio);
            effect_frame_count = scale_game_frames(random_int_between(8, 0xC));
            spawn_effect(bone_position, &zv, 0xEFFF7F4F, 0xFF0000, effect_frame_count, k, -k, 1);
        }
    }
}

extern int randomized_effect_joint_indices[] __asm__("D_L13_001D3538");

void emit_randomized_moby_joint_effects(struct Moby *moby) __asm__("FUN_L13_002e8b58");

void emit_randomized_moby_joint_effects(struct Moby *moby) {
    OvlQuad zv;
    float   effect_position[4];
    int     i;
    for (i = 0; randomized_effect_joint_indices[i] != -1; i++) {
        float    randomized_scale_ratio;
        int      effect_frame_count, k;
        OvlQuad *z = &zv;
        zv = 0;
        randomized_scale_ratio = moby->scale / (moby->pclass->scale * 4.0f);
        randomized_scale_ratio = randomized_scale_ratio * random_float_between(0.8f, 1.1f);
        moby_point_position(moby, randomized_effect_joint_indices[i], effect_position);
        add_random_offset_to_vector_xyz(effect_position, randomized_scale_ratio * 1.4f);
        if (frame_count & 1) {
            effect_frame_count = scale_game_frames(FUN_L00_00257b90(0x14, 0x3C));
            k = truncate_float_to_s32(random_float_between(450.0f, 550.0f) *
                                      randomized_scale_ratio);
            spawn_effect(effect_position, z, 0xDF000FFF, 0xCF, effect_frame_count, k, -100, 1);
        } else {
            if (moby->command != 0) {
                if (random_integer_below(0xC) != 0)
                    continue;
            }
            effect_frame_count = scale_game_frames(FUN_L00_00257b90(0x14, 0x32));
            k = truncate_float_to_s32(random_float_between(600.0f, 800.0f) *
                                      randomized_scale_ratio);
            spawn_effect(effect_position, z, 0x8000FFFF, 0x80, effect_frame_count, k, -100, 1);
        }
    }
}

extern int attachment_joint_indices_by_command[] __asm__("D_L13_001D34A8");

extern char *spawn_moby_joint_attachment(char *, int, int) __asm__("FUN_L13_002ecd10");

extern void FUN_0020c828(char *);

void update_moby_joint_attachments(struct Moby *parent_moby,
                                   char        *attachment_data) __asm__("FUN_L13_002e8d28");

void update_moby_joint_attachments(struct Moby *parent_moby, char *attachment_data) {
    int   i;
    int   attachment_joint_index;
    char *attached_moby;
    char *pose_follower_moby;
    for (i = 0; i < 10; i++) {
        attached_moby = ((char **)(attachment_data + 0xC4))[i];
        if (attached_moby != 0) {
            moby_point_position(parent_moby, i, attached_moby + 0x10);
            if (MOBY(attached_moby)->pos.x < 22.0f)
                MOBY(attached_moby)->pos.x = 22.0f;
            if (MOBY(attached_moby)->pos.x > 1001.0f)
                MOBY(attached_moby)->pos.x = 1001.0f;
            if (MOBY(attached_moby)->pos.y < 22.0f)
                MOBY(attached_moby)->pos.y = 22.0f;
            if (MOBY(attached_moby)->pos.y > 1001.0f)
                MOBY(attached_moby)->pos.y = 1001.0f;
            if (MOBY(attached_moby)->pos.z < 22.0f)
                MOBY(attached_moby)->pos.z = 22.0f;
            if (MOBY(attached_moby)->pos.z > 1001.0f)
                MOBY(attached_moby)->pos.z = 1001.0f;
            qcopy(attached_moby + 0x40, &parent_moby->rot);
            *(OvlQuad *)&MOBY(attached_moby)->rot_row0 = *(OvlQuad *)&parent_moby->rot_row0;
            *(OvlQuad *)&MOBY(attached_moby)->rot_row1 = *(OvlQuad *)&parent_moby->rot_row1;
            *(OvlQuad *)&MOBY(attached_moby)->rot_row2 = *(OvlQuad *)&parent_moby->rot_row2;
        }
    }
    attachment_joint_index = attachment_joint_indices_by_command[parent_moby->command];
    if (parent_moby->state == 5)
        attachment_joint_index = -1;
    if (attachment_joint_index != -1) {
        if (*(char **)(attachment_data + 0xF0) == 0 ||
            *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) == 0xFE ||
            *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) == 0xFD)
            *(char **)(attachment_data + 0xF0) =
                spawn_moby_joint_attachment(parent_moby, attachment_joint_index, 0x191);
        attached_moby = *(char **)(attachment_data + 0xF0);
        if (attached_moby != 0 && MOBY(attached_moby)->state != 0xFE &&
            MOBY(attached_moby)->state != 0xFD) {
            moby_point_position(parent_moby, attachment_joint_index, attached_moby + 0x10);
            if (MOBY(attached_moby)->pos.x < 22.0f)
                MOBY(attached_moby)->pos.x = 22.0f;
            if (MOBY(attached_moby)->pos.x > 1001.0f)
                MOBY(attached_moby)->pos.x = 1001.0f;
            if (MOBY(attached_moby)->pos.y < 22.0f)
                MOBY(attached_moby)->pos.y = 22.0f;
            if (MOBY(attached_moby)->pos.y > 1001.0f)
                MOBY(attached_moby)->pos.y = 1001.0f;
            if (MOBY(attached_moby)->pos.z < 22.0f)
                MOBY(attached_moby)->pos.z = 22.0f;
            if (MOBY(attached_moby)->pos.z > 1001.0f)
                MOBY(attached_moby)->pos.z = 1001.0f;
            qcopy(attached_moby + 0x40, &parent_moby->rot);
            *(OvlQuad *)&MOBY(attached_moby)->rot_row0 = *(OvlQuad *)&parent_moby->rot_row0;
            *(OvlQuad *)&MOBY(attached_moby)->rot_row1 = *(OvlQuad *)&parent_moby->rot_row1;
            *(OvlQuad *)&MOBY(attached_moby)->rot_row2 = *(OvlQuad *)&parent_moby->rot_row2;
        }
    } else {
        if (*(char **)(attachment_data + 0xF0) != 0 &&
            *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) != 0xFE &&
            *(unsigned char *)(*(char **)(attachment_data + 0xF0) + 0x20) != 0xFD) {
            FUN_0020c828(*(char **)(attachment_data + 0xF0));
            *(char **)(attachment_data + 0xF0) = 0;
        }
    }
    pose_follower_moby = *(char **)(attachment_data + 0x110);
    if (pose_follower_moby != 0) {
        qcopy(pose_follower_moby + 0x10, &parent_moby->pos);
        qcopy(pose_follower_moby + 0x40, &parent_moby->rot);
        *(OvlQuad *)&MOBY(pose_follower_moby)->rot_row0 = *(OvlQuad *)&parent_moby->rot_row0;
        *(OvlQuad *)&MOBY(pose_follower_moby)->rot_row1 = *(OvlQuad *)&parent_moby->rot_row1;
        *(OvlQuad *)&MOBY(pose_follower_moby)->rot_row2 = *(OvlQuad *)&parent_moby->rot_row2;
    }
}

void update_moby_spring_steering_and_banking(struct Moby *moby,
                                             char        *moby_data,
                                             float        target_yaw,
                                             float        target_pitch) __asm__("FUN_L13_002e9018");

void update_moby_spring_steering_and_banking(struct Moby *moby,
                                             char        *moby_data,
                                             float        target_yaw,

                                             float target_pitch) {
    float absolute_roll;
    FUN_L00_00258278(moby,
                     (float *)(moby_data + 0xA8),
                     target_yaw,
                     frame_time_sq * 3.1415927f,
                     frame_time_sq * 6.2831855f,
                     frame_time * 6.2831855f);
    moby->rot.y = spring_turn((float *)(moby_data + 0xA4),
                              moby->rot.y,
                              target_pitch,
                              frame_time_sq * 3.1415927f,
                              frame_time_sq * 6.2831855f,
                              frame_time * 6.2831855f);
    absolute_roll = absolute_float(moby->rot.x);
    moby->rot.x +=
        -*(float *)(moby_data + 0xA8) * ((1.5707964f - absolute_roll) / 1.5707964f) * 0.5f;
    if (moby->rot.x > 1.5707964f)
        moby->rot.x = 1.5707964f;
    if (moby->rot.x < -1.5707964f)
        moby->rot.x = -1.5707964f;
    moby->rot.x *= 0.985f;
    normalize_angle(moby->rot.x);
}

extern void FUN_L13_002e9018_c(void *, void *, float, float) __asm__("FUN_L13_002e9018");

int advance_moby_along_selected_path(struct Moby *moby,
                                     char        *moby_data,
                                     int          path_slot_index,
                                     int          aim) __asm__("FUN_L13_002e9160");

int advance_moby_along_selected_path(struct Moby *moby,
                                     char        *moby_data,
                                     int          path_slot_index,

                                     int aim) {
    char *path = D_L13_001B07B0[*(int *)(&moby_data[path_slot_index << 2] + 0xB0)];
    float distance_from_previous_node, seg, segment_distance_shortfall, steering_blend_factor, yaw,
        pitch;
    int   wrapped = 0;
    float target_node_steering_vector[4];
    distance_from_previous_node =
        fast_vec_dist(path + (*(short *)(moby_data + 0x9E) * 16 + 0x10), &moby->pos);
    seg = fast_vec_dist(path + (*(short *)(moby_data + 0x9E) * 16 + 0x10),
                        path + (*(short *)(moby_data + 0x9C) * 16 + 0x10));
    segment_distance_shortfall = seg - distance_from_previous_node;
    if (0.0f <= segment_distance_shortfall &&
        segment_distance_shortfall < *(float *)(moby_data + 0xAC)) {
        *(short *)(moby_data + 0x9E) = *(unsigned short *)(moby_data + 0x9C);
        *(short *)(moby_data + 0x9C) =
            *(unsigned short *)(moby_data + 0x9C) + ((signed char *)moby_data)[0x10C];
        if (*(short *)(moby_data + 0x9C) >= *(int *)path) {
            *(short *)(moby_data + 0x9C) = 0;
            wrapped = 1;
        }
        if (*(short *)(moby_data + 0x9C) < 0) {
            wrapped = 1;
            *(short *)(moby_data + 0x9C) = *(unsigned short *)path - 1;
        }
        steering_blend_factor = 0.0f;
    } else {
        steering_blend_factor = distance_from_previous_node / seg;
    }
    if (steering_blend_factor < 0.05f)
        steering_blend_factor = 0.05f;
    if (1.0f < steering_blend_factor)
        steering_blend_factor = 1.0f;
    fast_vec_scale(moby_data + 0x70, moby_data + 0x70, 1.0f - steering_blend_factor);
    fast_vec_sub(
        target_node_steering_vector, path + (*(short *)(moby_data + 0x9C) * 16 + 0x10), &moby->pos);
    fast_vec_normalize(
        target_node_steering_vector, target_node_steering_vector, *(float *)(moby_data + 0xAC));
    fast_vec_scale(target_node_steering_vector, target_node_steering_vector, steering_blend_factor);
    fast_vec_add(moby_data + 0x70, moby_data + 0x70, target_node_steering_vector);
    fast_vec_normalize(moby_data + 0x70, moby_data + 0x70, *(float *)(moby_data + 0xAC));
    fast_vec_add(&moby->pos, &moby->pos, moby_data + 0x70);
    yaw = fast_arc_tan(target_node_steering_vector[0], target_node_steering_vector[1]);
    pitch = -fast_arc_tan(vector_length_xy(target_node_steering_vector),
                          target_node_steering_vector[2]);
    if (moby->state == 5) {
        struct Hero *hero_state = &hero;
        yaw = fast_arc_tan(hero_state->motion.pos.f[0] - moby->pos.x,
                           hero_state->motion.pos.f[1] - moby->pos.y);
        pitch = -fast_arc_tan(vector_distance_xy(&moby->pos, &hero_state->motion.pos),
                              hero_state->motion.pos.f[2] - moby->pos.z);
    }
    if (aim != 0)
        FUN_L13_002e9018_c(moby, moby_data, yaw, pitch);
    return wrapped;
}

extern int FUN_L13_002e9160_c(char *, char *, int, int) __asm__("FUN_L13_002e9160");

int update_moby_path_following_away_from_hero(struct Moby *moby,
                                              char        *path_following_data,
                                              int          path_slot_index,
                                              int          aim) __asm__("FUN_L13_002e9408");

int update_moby_path_following_away_from_hero(struct Moby *moby,
                                              char        *path_following_data,
                                              int          path_slot_index,

                                              int aim) {
    char *path = walker_paths[*(int *)(&path_following_data[path_slot_index << 2] + 0xB0)];
    int   result_flags = 0;
    float ahead[4];
    float behind[4];
    float ahead_distance_to_hero, behind_distance_to_hero;
    int   next_node_index;
    int   k;
    int   desired_path_direction;
    if (path != 0) {
        next_node_index = *(short *)(path_following_data + 0x9C) + 1;
        if (next_node_index >= *(int *)path) {
            next_node_index = 0;
        }
        qcopy(ahead, path + next_node_index * 16 + 0x10);
        ahead_distance_to_hero = fast_vec_dist(ahead, &hero.motion.pos);
        k = *(short *)(path_following_data + 0x9C) - 1;
        if (k < 0) {
            k = *(int *)path - 1;
        }
        qcopy(behind, path + k * 16 + 0x10);
        behind_distance_to_hero = fast_vec_dist(behind, &hero.motion.pos);
        if (ahead_distance_to_hero < behind_distance_to_hero) {
            desired_path_direction = -1;
        } else if (behind_distance_to_hero < ahead_distance_to_hero) {
            desired_path_direction = 1;
        } else {
            desired_path_direction = path_following_data[0x10C] == 1 ? 1 : -1;
        }
        if (desired_path_direction != (signed char)path_following_data[0x10C]) {
            float k = frame_scale;
            *(float *)(path_following_data + 0xAC) *= k * -0.100000024f + 1.0f;
            if (*(float *)(path_following_data + 0xAC) < k * 0.15f) {
                short cur = *(short *)(path_following_data + 0x9C);
                path_following_data[0x10C] = desired_path_direction;
                *(short *)(path_following_data + 0x9C) =
                    *(unsigned short *)(path_following_data + 0x9E);
                *(short *)(path_following_data + 0x9E) = cur;
            }
        } else {
            float distance_to_hero = fast_vec_dist(&moby->pos, &hero.motion.pos);
            float target_speed_multiplier = 1.0f;
            float sp;
            if (moby->command == 4) {
                target_speed_multiplier = 3.0f;
            }
            sp = *(float *)(path_following_data + 0xAC);
            sp += (target_speed_multiplier * 30.0f / distance_to_hero - sp) * (frame_scale * 0.12f);
            *(float *)(path_following_data + 0xAC) = sp;
            if (sp < 0.0f) {
                *(float *)(path_following_data + 0xAC) = 0.0f;
            }
        }
        result_flags = FUN_L13_002e9160_c(moby, path_following_data, path_slot_index, aim);
    }
    if (0.0f < *(float *)(path + (*(short *)(path_following_data + 0x9C) << 4) + 0x1C)) {
        result_flags |= 2;
    }
    return result_flags;
}

extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");

void detach_moby_joint_attachment(void *unused,
                                  char *parent_data,
                                  int   child_slot_index) __asm__("FUN_L13_002e9680");

void detach_moby_joint_attachment(void *unused, char *parent_data, int child_slot_index) {
    char  *base = parent_data + 0xC4;
    int    child_slot_byte_offset = child_slot_index * 4;
    char **slot = (char **)(base + child_slot_byte_offset);
    char  *moby = *slot;
    if (moby != 0) {
        char  *data = ((char *)MOBY(moby)->pvars);
        float *child_launch_vector = (float *)(data + 0x10);
        float  random_half_range, random_small_angle_step, random_second_angle_step,
            random_third_angle_step;
        cart_burst(unused, moby + 0x10, -1, 2.0f, 13.0f);
        fast_vec_scale(child_launch_vector, parent_data + 0x70, 0.95f);
        add_random_offset_to_vector_xyz(child_launch_vector, frame_time * 30.0f);
        moby[0x20] = 3;
        fast_vec_add(moby + 0x10, moby + 0x10, child_launch_vector);
        random_half_range = frame_time;
        random_half_range = random_half_range * 0.34906585f;
        random_small_angle_step = random_float_between(-random_half_range, random_half_range);
        random_half_range = frame_time;
        *(float *)(data + 0x30) = random_small_angle_step;
        random_half_range = random_half_range * 3.1415927f;
        random_second_angle_step = random_float_between(-random_half_range, random_half_range);
        random_half_range = frame_time;
        *(float *)(data + 0x34) = random_second_angle_step;
        random_half_range = random_half_range * 3.1415927f;
        random_third_angle_step = random_float_between(-random_half_range, random_half_range);
        *(float *)(data + 0x38) = random_third_angle_step;
        MOBY(moby)->spawn_frame = 0x40404000000000L;
        MOBY(moby)->hit_slot = 0xFF;
        *slot = 0;
    }
}

void begin_starfighter_death_sequence(struct Moby *moby,
                                      char        *vars,
                                      int         *result) __asm__("FUN_L13_002e97e0");

void begin_starfighter_death_sequence(struct Moby *moby, char *vars, int *result) {
    int          path_ids = (int)vars + 0xB0;
    struct Moby *mobys;
    float        dx;
    float        dy;
    float        dz;
    float        horizontal;
    float        scale;
    char        *velocity;

    *result = 0;
    moby->flags &= 0xEFFF;
    FUN_L00_00257470(moby, 0, -1);
    vars[0x67] = 0x78;
    trooper_anim_update(moby, (short *)(vars + 0x60));
    vars[0x13B] = 4;
    scale = D_0015ED6C * 10.0f;
    velocity = (char *)l13_cuboids + (*(int *)(vars + 0x130) << 7);
    *(short *)(vars + 0x9C) = 0;
    fast_vec_normalize(vars + 0x70, velocity, scale);
    {
        int   index = *(short *)(vars + 0x9C);
        int  *path_id = (int *)path_ids + (unsigned char)vars[0x13B];
        char *path = D_L13_001B07B0[*path_id];
        char *point;
        char *next;

        qcopy(&moby->pos, (char *)((unsigned int)(index * 16 + 0x10) + (unsigned int)path));
        point = (char *)((int)path + index * 16);
        next = path + ++index * 16;
        dx = *(float *)(next + 0x10) - *(float *)(point + 0x10);
        dy = *(float *)(next + 0x14) - *(float *)(point + 0x14);
        moby->rot.z = fast_arc_tan(dx, dy);
    }
    {
        int   index = *(short *)(vars + 0x9C);
        int  *path_id = (int *)path_ids + (unsigned char)vars[0x13B];
        char *path = D_L13_001B07B0[*path_id];
        int   point = index * 16 + 0x10;
        int   next = index * 16 + 0x20;

        horizontal = vector_distance_xy(path + point, path + next);
    }
    {
        int   index = *(short *)(vars + 0x9C);
        char *path;
        char *point;
        char *next;

        path_ids += (unsigned char)vars[0x13B] * 4;
        path = D_L13_001B07B0[*(int *)path_ids];
        next = path + (index + 1) * 16;
        point = path + index * 16;
        dz = *(float *)(next + 0x18) - *(float *)(point + 0x18);
        moby->rot.y = -fast_arc_tan(horizontal, dz);
    }
    vars[0x10C] = 1;
    mobys = (struct Moby *)D_L13_0015FFD8;
    mobys[*(int *)(vars + 0x134)].flags |= 2;
    mobys[*(int *)(vars + 0x134)].flags |= 1;
    mobys[*(int *)(vars + 0x134)].visible = 0;
    activate_descriptor(*(int *)(vars + 0x12C));
    moby->state = 7;
    *(float *)(vars + 0xAC) = D_0015ED6C * 80.0f;
    *(int *)(vars + 0xF8) = scale_game_frames(600);
    queue_starfighter_voice_line(moby, (unsigned char *)vars, 0x22, 1);
    *(short *)(vars + 0x138) = 0;
    vars[0x148] = 0;
    if (*(int *)(vars + 0x104) != -1) {
        set_animation_parameter(*(int *)(vars + 0x104), 0);
        *(int *)(vars + 0x104) = -1;
    }
    *(int *)(vars + 0x13C) = -1;
}

typedef struct Moby_2EAE80 Moby_2EAE80;

typedef struct {
    char pad0[0x88];
    int  f88;
    int  f8C;
    char pad90[0x5C];
    int  fEC;
    char padF0[0x28];
    int  f118;
} Vars_2EAE80;

struct Moby_2EAE80 {
    char          pad0[0x20];
    unsigned char state;
    char          pad21[0x57];
    Vars_2EAE80  *vars;
    char          pad7C[0x28];
    unsigned char bA4;
    char          padA5;
    short         oclass;
    char          padA8[0x14];
    unsigned char bBC;
};

typedef struct {
    float f0;
    short h4;
    short h6;
} Sub;

typedef struct {
    char           pad0[0x20];
    Sub            sub;
    char           pad28[0x38];
    char           q60[7];
    unsigned char  b67;
    char           pad68[0x5C];
    Moby_2EAE80   *list[11];
    Moby_2EAE80   *first;
    char           padF4[0xC];
    int            f100;
    char           pad104[0x18];
    int            i11C;
    char           pad120[0x18];
    unsigned short h138;
    char           pad13A[0xE];
    unsigned char  b148;
} Data_2EAE80;

typedef struct {
    char           pad0[0x20];
    Moby_2EAE80   *target;
    char           pad24[6];
    unsigned short h2A;
    float          f2C;
} Hit_2EAE80;

typedef struct {
    char          pad0[0x74];
    unsigned char b74;
    char          pad75[0x13];
    Moby_2EAE80  *f88;
} Voice;

typedef struct {
    char         pad0[0x15F0];
    Moby_2EAE80 *f15F0;
} Hero_2EAE80;

extern void begin_starfighter_death_sequence();

extern Hit_2EAE80 *func_L00_0025B478_2EAE80(void *, int, int) __asm__("FUN_L00_0025a420");

extern void func_L13_002EAC18_2EAE80(void *, void *, void *, void *) __asm__("FUN_L13_002e97e0");

extern void func_L00_0025E4B0_2EAE80(void *, void *) __asm__("FUN_L00_0025d458");

extern void func_L13_002EAAB8_2EAE80(void *, void *, int) __asm__("FUN_L13_002e9680");

extern int func_L13_002E9B30_2EAE80(void *, void *, int, int) __asm__("FUN_L13_002e86f8");

void process_moby_attachment_damage(Moby_2EAE80 *moby,
                                    Data_2EAE80 *moby_data) __asm__("FUN_L13_002e9a48");

void process_moby_attachment_damage(Moby_2EAE80 *moby, Data_2EAE80 *moby_data) {
    enum {
        DAMAGE_STAGE_SKIP_LINE_QUEUE = 0,
        DAMAGE_STAGE_QUEUE_RANDOM_LINE = 1,
        DAMAGE_STAGE_START_TIMER_AND_QUEUE_LINE = 2,
        DAMAGE_STAGE_QUEUE_SINGLE_LINE = 3,
        DAMAGE_STAGE_SKIP_LINE_QUEUE_ALTERNATE = 4,
        DAMAGE_STAGE_QUEUE_ALTERNATE_SINGLE_LINE = 5,
        DAMAGE_STAGE_QUEUE_LATE_SINGLE_LINE = 6,
        DAMAGE_STAGE_SKIP_LINE_QUEUE_SECOND_ALTERNATE = 7
    };
    enum {
        CHILD_HIT_HALF_DAMAGE = 0x127,
        CHILD_HIT_FIXED_DAMAGE = 0x3F1
    };
    int         hit;
    int         rf;
    int         mode;
    int         i;
    int         k;
    int         j;
    float       lo;
    float       hi;
    float       k6;
    float       step;
    Hit_2EAE80 *r;
    Sub        *dd;

    if (moby->state < 6) {
        dd = &moby_data->sub;
        mode = 0;
        rf = 0;
        k6 = 6.0f;
        step = (float)dd->h4 / k6;
        hi = (k6 - convert_integer_to_float(moby->bBC)) * step;
        if (moby->bBC >= 5) {
            hi = (k6 - convert_integer_to_float(moby->bBC - 1)) * step;
        }
        lo = hi + step * 0.3f;
        if (moby_data->first != 0) {
            r = func_L00_0025B478_2EAE80(moby_data->first, 0x230000, 0);
            fun_00213928(moby, r, dd, 0, &hit, 0, 0, 4);
            if (r != 0 && r->target != 0 &&
                (r->target->oclass == 0x52 || r->target->oclass == 0x53 ||
                 r->target->oclass == 0xD4)) {
                moby_data->first->bA4 = 0xFF;
            } else if (hit >= 2) {
                if (0.0f < r->f2C && r->h2A == 0x3F1) {
                    r->f2C = 0.3f;
                }
                if (r != 0 && moby->bBC == 7) {
                    r->f2C = r->f2C * 1.5f;
                }
                if (dd->f0 <= r->f2C) {
                    func_L13_002EAC18_2EAE80(moby, moby_data, dd, r);
                    mode = 3;
                    moby_data->first->bA4 = 0xFF;
                } else {
                    dd->f0 = dd->f0 - r->f2C;
                    moby_data->b67 = 0xFA;
                    mode = 1;
                    moby_data->sub.h6 = scale_game_frames(0);
                    func_L00_0025E4B0_2EAE80(moby, moby_data->q60);
                    moby_data->first->bA4 = 0xFF;
                }
                if (moby->state == 7) {
                    moby->bBC++;
                    rf = 1;
                    {
                        Vars_2EAE80 *v = (*(Moby_2EAE80 **)D_00140940)->vars;

                        v->f88 = 0;
                        v->fEC = 0;
                        v->f8C = 0;
                        v->f118 = 0;
                    }
                } else if (dd->f0 < hi) {
                    int tv = D_L13_001D3478[moby->bBC - 1];

                    if (tv != -1) {
                        allocate_voice_for_target_entry(7, 0, moby);
                        func_L13_002EAAB8_2EAE80(moby, moby_data, tv);
                    }
                    if (moby->state == 5) {
                        moby->state = 4;
                    }
                    moby->bBC++;
                    if (moby_data->i11C != -1) {
                        if (((Voice *)(D_0013E550 + moby_data->i11C * 0x70))->f88 == moby &&
                            ((Voice *)(D_0013E550 + moby_data->i11C * 0x70))->b74 != 0) {
                            release_voice_slot(moby_data->i11C);
                        }
                        moby_data->i11C = -1;
                    }
                    rf = 1;
                    {
                        Vars_2EAE80 *v = (*(Moby_2EAE80 **)D_00140940)->vars;

                        v->f88 = 0;
                        v->fEC = 0;
                        v->f8C = 0;
                        v->f118 = 0;
                    }
                    moby_data->h138 = 0;
                    moby_data->b148 = 0;
                    mode = 2;
                    if (moby->bBC == 7) {
                        for (j = moby->bBC - 1; j < 10; j++) {
                            func_L13_002EAAB8_2EAE80(moby, moby_data, D_L13_001D3478[j]);
                        }
                        allocate_voice_for_target_entry(7, 0, moby);
                    }
                } else if (dd->f0 < lo) {
                    moby_data->b148 = 1;
                }
            }
        }
        for (i = 0; i < 11 && mode == 0; i++) {
            if (moby_data->list[i] == 0) {
                continue;
            }
            r = func_L00_0025B478_2EAE80(moby_data->list[i], 0x230000, 0);
            fun_00213928(moby, r, dd, 0, &hit, 0, 0, 4);
            if (r != 0 && r->target != 0 &&
                (r->target->oclass == 0x52 || r->target->oclass == 0x53 ||
                 r->target->oclass == 0xD4)) {
                moby_data->list[i]->bA4 = 0xFF;
                continue;
            }
            if (hit < 2) {
                continue;
            }
            if (0.0f < r->f2C) {
                switch (r->h2A) {
                case CHILD_HIT_FIXED_DAMAGE:
                    r->f2C = 0.1f;
                    break;
                case CHILD_HIT_HALF_DAMAGE:
                    r->f2C = r->f2C * 0.5f;
                    break;
                }
            }
            if (r != 0 && moby->bBC == 7) {
                r->f2C = r->f2C * 1.5f;
            }
            if (dd->f0 <= r->f2C) {
                func_L13_002EAC18_2EAE80(moby, moby_data, dd, r);
                mode = 3;
                moby_data->list[i]->bA4 = 0xFF;
            } else {
                dd->f0 = dd->f0 - r->f2C;
                moby_data->b67 = 0xFA;
                mode = 1;
                moby_data->sub.h6 = scale_game_frames(0);
                func_L00_0025E4B0_2EAE80(moby, moby_data->q60);
                moby_data->list[i]->bA4 = 0xFF;
            }
            if (moby->state == 7) {
                moby->bBC++;
                rf = 1;
                {
                    Vars_2EAE80 *v = ((Hero_2EAE80 *)&hero)->f15F0->vars;

                    v->f88 = 0;
                    v->fEC = 0;
                    v->f8C = 0;
                    v->f118 = 0;
                }
            } else if (dd->f0 < hi) {
                int tv = D_L13_001D3478[moby->bBC - 1];

                if (tv != -1) {
                    func_L13_002EAAB8_2EAE80(moby, moby_data, tv);
                }
                allocate_voice_for_target_entry(7, 0, moby);
                if (moby->state == 5) {
                    moby->state = 4;
                }
                moby->bBC++;
                if (moby_data->i11C != -1) {
                    if (((Voice *)(D_0013E550 + moby_data->i11C * 0x70))->f88 == moby &&
                        ((Voice *)(D_0013E550 + moby_data->i11C * 0x70))->b74 != 0) {
                        release_voice_slot(moby_data->i11C);
                    }
                    moby_data->i11C = -1;
                }
                rf = 1;
                {
                    Vars_2EAE80 *v = ((Hero_2EAE80 *)&hero)->f15F0->vars;

                    v->f88 = 0;
                    v->fEC = 0;
                    v->f8C = 0;
                    v->f118 = 0;
                }
                moby_data->h138 = 0;
                moby_data->b148 = 0;
                mode = 2;
                if (moby->bBC == 7) {
                    for (k = moby->bBC - 1; k < 10; k++) {
                        func_L13_002EAAB8_2EAE80(moby, moby_data, D_L13_001D3478[k]);
                    }
                    allocate_voice_for_target_entry(7, 0, moby);
                }
            } else if (dd->f0 < lo) {
                moby_data->b148 = 1;
            }
        }
        if (mode > 0) {
            if (rf != 0) {
                switch (moby->bBC - 1) {
                case DAMAGE_STAGE_SKIP_LINE_QUEUE:
                    break;
                case DAMAGE_STAGE_QUEUE_RANDOM_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0xC, 4);
                    break;
                case DAMAGE_STAGE_START_TIMER_AND_QUEUE_LINE:
                    moby_data->f100 = scale_game_frames(0x78);
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x10, 1);
                    break;
                case DAMAGE_STAGE_QUEUE_SINGLE_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x11, 1);
                    break;
                case DAMAGE_STAGE_SKIP_LINE_QUEUE_ALTERNATE:
                    break;
                case DAMAGE_STAGE_QUEUE_ALTERNATE_SINGLE_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x13, 1);
                    break;
                case DAMAGE_STAGE_QUEUE_LATE_SINGLE_LINE:
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x16, 1);
                    break;
                case DAMAGE_STAGE_SKIP_LINE_QUEUE_SECOND_ALTERNATE:
                    break;
                }
            } else if (moby->bBC == 7 && moby_data->b148 != 0) {
                if (!(moby_data->h138 & 1) &&
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x14, 2) != 0) {
                    moby_data->h138 |= 1;
                }
                if (!(moby_data->h138 & 2) &&
                    func_L13_002E9B30_2EAE80(moby, moby_data, 0x14, 2) != 0) {
                    moby_data->h138 |= 2;
                }
            }
        }
    }
    trooper_anim_finish(moby, moby_data->q60);
}

void predict_hero_ship_projectile_lead_position(char *predicted_target_position,
                                                char *projectile_origin_position,
                                                float projectile_speed) __asm__("FUN_L13_002ea330");

void predict_hero_ship_projectile_lead_position(char *predicted_target_position,
                                                char *projectile_origin_position,

                                                float projectile_speed) {
    char *base = D_0013E533 + 0xE1D;
    char *vehicle_data = *(char **)(*(char **)(base + 0x15F0) + 0x78);
    float y = fast_add_rotations(*(float *)(*(char **)(base + 0x15F0) + 0x48),
                                 *(float *)(vehicle_data + 0x90));
    float x = fast_add_rotations(*(float *)(*(char **)(base + 0x15F0) + 0x44),
                                 *(float *)(vehicle_data + 0x94));
    float projectile_origin_to_vehicle_distance =
        fast_vec_dist(projectile_origin_position, *(char **)(base + 0x15F0) + 0x10);
    float k = *(float *)(vehicle_data + 0x64);
    build_spherical_offset(predicted_target_position,
                           projectile_origin_to_vehicle_distance / (k + projectile_speed) * k,
                           y,
                           -x);
    fast_vec_add(
        predicted_target_position, predicted_target_position, *(char **)(base + 0x15F0) + 0x10);
}

extern void
predict_hero_ship_projectile_lead_position(char *a, char *b, float f) __asm__("FUN_L13_002ea330");

float calculate_hero_aim_angles(float *pos,
                                float *pitch,
                                float *yaw,
                                float  lead) __asm__("FUN_L13_002ea400");

float calculate_hero_aim_angles(float *pos, float *pitch, float *yaw, float lead) {
    struct Hero *base = &hero;
    char        *hero_ship_moby = ((char *)base->ship_moby);
    float        target_position[4];
    float        y;
    float        pitch_angle;
    if (hero_ship_moby != 0 && MOBY(hero_ship_moby)->oclass == 0x45) {
        if (lead > 0.0f) {
            predict_hero_ship_projectile_lead_position((char *)target_position, (char *)pos, lead);
        } else {
            qcopy(target_position, hero_ship_moby + 0x10);
        }
        y = fast_arc_tan(target_position[0] - pos[0], target_position[1] - pos[1]);
        pitch_angle =
            -fast_arc_tan(vector_distance_xy(pos, target_position), target_position[2] - pos[2]);
    } else {
        struct Hero *hero_state = &hero;
        y = fast_arc_tan(hero_state->motion.pos.f[0] - pos[0],
                         hero_state->motion.pos.f[1] - pos[1]);
        pitch_angle = -fast_arc_tan(vector_distance_xy(pos, &hero_state->motion.pos),
                                    hero_state->motion.pos.f[2] - pos[2]);
    }
    if (yaw != 0)
        *yaw = y;
    if (pitch != 0)
        *pitch = pitch_angle;
    return y;
}

extern int FUN_001160d8_e5(void) __asm__("FUN_001160d8");

extern void FUN_L00_0024f7c8_e5(char *, int, float *) __asm__("FUN_L00_0024f7c8");

extern unsigned char *
FUN_L13_002c13b0_e5(char *, char *, char *, char *, float, int, float) __asm__("FUN_L13_002c13b0");

extern void FUN_L13_002ea400_c(float *, float *, float *, float) __asm__("FUN_L13_002ea400");

unsigned char *
fire_starfighter_homing_rocket(struct Moby   *firing_moby,
                               unsigned char *starfighter_data) __asm__("FUN_L13_002ea540");

unsigned char *fire_starfighter_homing_rocket(struct Moby *firing_moby,

                                              unsigned char *starfighter_data) {
    float muzzle_position[4];
    struct {
        int   a;
        float b;
        float c;
    } s;
    int            muzzle_joint_index;
    float          projectile_speed;
    unsigned char *spawned_rocket_moby;
    struct Hero   *hero_state;
    muzzle_joint_index = (starfighter_data[0xFC] & 1) ? 0x12 : 0x10;
    if (firing_moby->command != 6 || (FUN_001160d8_e5() & 1)) {
        starfighter_data[0xFC] ^= 1;
    }
    FUN_L00_0024f7c8_e5(firing_moby, muzzle_joint_index, muzzle_position);
    s.a = 0;
    if (firing_moby->state != 5) {
        FUN_L13_002ea400_c(muzzle_position, &s.b, &s.c, frame_time * 17.0f);
    } else {
        FUN_L13_002ea400_c(muzzle_position, &s.b, &s.c, -1.0f);
    }
    hero_state = &hero;
    projectile_speed = frame_time * 18.0f + *(float *)(starfighter_data + 0xAC);
    s.c = firing_moby->rot.z;
    s.b = 0.43633232f;
    spawned_rocket_moby = FUN_L13_002c13b0_e5(firing_moby,
                                              (char *)muzzle_position,
                                              ((char *)hero_state->ship_moby),
                                              (char *)&s,
                                              projectile_speed,
                                              scale_game_frames(300),
                                              frame_time * 18.0f);
    if (spawned_rocket_moby != 0) {
        allocate_voice_for_target_entry(2, 0, firing_moby);
    }
    return spawned_rocket_moby;
}

unsigned char *fire_level_13_mine(char *firing_moby,
                                  char *velocity_source_data) __asm__("FUN_L13_002ea6b8");

unsigned char *fire_level_13_mine(char *firing_moby, char *velocity_source_data) {
    float spawn_position[4], projectile_velocity[4], predicted_target_position[4],
        scratch_vector[4], random_velocity_offset[4];
    struct Hero   *x;
    unsigned char *spawned_projectile_moby;
    moby_point_position(firing_moby, 0xE, spawn_position);
    x = &hero;
    if (((char *)x->ship_moby) != 0 && *(short *)(((char *)x->ship_moby) + 0xA6) == 0x45) {
        predict_hero_ship_projectile_lead_position(
            (char *)predicted_target_position, (char *)spawn_position, frame_time * 20.0f);
        fast_vec_sub(scratch_vector, predicted_target_position, spawn_position);
        fast_vec_normalize(projectile_velocity, scratch_vector, frame_time * 20.0f);
        fast_vec_normalize(random_velocity_offset,
                           ((char *)x->ship_moby) + 0xD0,
                           random_float_between(-(frame_time * 30.0f), frame_time * 30.0f));
        fast_vec_normalize(scratch_vector,
                           ((char *)x->ship_moby) + 0xE0,
                           random_float_between(-(frame_time * 30.0f), frame_time * 30.0f));
        fast_vec_add(random_velocity_offset, random_velocity_offset, scratch_vector);
        fast_vec_normalize(random_velocity_offset,
                           random_velocity_offset,
                           random_float_between(frame_time * 5.0f, frame_time * 30.0f));
        fast_vec_add(projectile_velocity, projectile_velocity, random_velocity_offset);
    } else {
        random_vector(projectile_velocity, frame_time * 0.0f, frame_time * 30.0f);
    }
    fast_vec_add(projectile_velocity, projectile_velocity, velocity_source_data + 0x70);
    spawned_projectile_moby =
        spawn_level_13_mine(firing_moby, (char *)spawn_position, (char *)projectile_velocity);
    if (spawned_projectile_moby)
        allocate_voice_for_target_entry(3, 0, (int)firing_moby);
    return spawned_projectile_moby;
}

extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_starfighter_tractor_beam(struct Moby *moby) __asm__("FUN_L13_002ea8c8");

void draw_starfighter_tractor_beam(struct Moby *moby) {
    char               *vars = (char *)moby->pvars;
    struct GeometryQuad quad;
    float               start[4];
    float               direction[4];
    float               position[4];
    float               offset[4];
    float               side[4];
    float               up[4];
    float               edge[4];
    float               length;
    float               step;
    float               distance;
    float               width;
    float               scale;
    float               zero = 0.0f;
    float               one = 1.0f;
    u64                 texture;

    texture = get_effect_texture(0x34);
    quad.colors[3] = 0x40808080;
    quad.colors[2] = 0x40808080;
    quad.colors[1] = 0x40808080;
    quad.colors[0] = 0x40808080;
    quad.texture = texture;
    quad.texture_state = 0xFF9000000260LL;
    quad.reserved.value = 4;
    quad.primitive = 0x8000000048LL;
    quad.texture_coordinates[0][0] = zero;
    quad.texture_coordinates[2][0] = zero;
    quad.texture_coordinates[1][0] = one;
    quad.texture_coordinates[3][0] = one;
    quad.texture_coordinates[0][1] = zero;
    quad.texture_coordinates[1][1] = zero;
    quad.texture_coordinates[2][1] = one;
    quad.texture_coordinates[3][1] = one;
    moby_point_position(moby, 0x13, start);
    fast_vec_sub(direction, vars + 0x80, start);
    clamp_vector_length(direction, direction, 100.0f);
    length = fast_vec_length(direction);
    scale = length / 100.0f;
    if (scale < 1.0f)
        step = 1.0f;
    else
        step = scale;
    fast_vec_normalize(direction, direction, step);
    qcopy(position, start);
    distance = zero;
    fast_vec_normalize(offset, direction, convert_integer_to_float(frame_count % 20) / 20.0f);
    fast_vec_sub(position, position, offset);
    while (distance < length) {
        width = (distance * 0.0f) / (length + length) + 0.5f;
        fast_vec_cross(side, direction, D_L13_00160680);
        fast_vec_normalize(side, side, width);
        fast_vec_cross(up, direction, side);
        fast_vec_normalize(up, up, width);
        fast_vec_sub(edge, position, up);
        fast_vec_sub(&quad.positions[0], edge, side);
        fast_vec_add(&quad.positions[1], edge, side);
        fast_vec_add(edge, position, up);
        fast_vec_sub(&quad.positions[2], edge, side);
        fast_vec_add(&quad.positions[3], edge, side);
        if (10.0f < length && length - 10.0f < distance) {
            int color = truncate_float_to_s32((length - distance) * 6.4f) << 24 | 0x808080;
            quad.colors[3] = color;
            quad.colors[2] = color;
            quad.colors[1] = color;
            quad.colors[0] = color;
        }
        draw_geometry_quad(&quad, 0, 1);
        distance += step;
        fast_vec_add(position, position, direction);
    }
}

extern char D_0013E533[];

extern int FUN_L00_001fefc8(void *);

extern char D_0013F3D0[];

extern char D_00140940[];

void update_qwarks_starfighter_attack_commands(void *mobyp, void *pp) __asm__("FUN_L13_002eac00");

void update_qwarks_starfighter_attack_commands(void *mobyp, void *pp) {
    enum {
        STARFIGHTER_COMMAND_TIMED_VOLLEY = 1,
        STARFIGHTER_COMMAND_ALTERNATING_ATTACKS = 2,
        STARFIGHTER_COMMAND_REPEATED_TARGETED_ATTACK = 3,
        STARFIGHTER_COMMAND_WAIT_FOR_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM = 4,
        STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM = 5,
        STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_CONDITIONAL_MUZZLE_SWAP_AND_TRACTOR_BEAM = 6,
        STARFIGHTER_COMMAND_REPEATED_VECTOR_ATTACK = 7
    };
    char         *moby = mobyp;
    char         *starfighter_data = pp;
    float         sv[4];
    float         joint_to_hero_distance;
    int           voice_slot_index;
    char         *voice_slot_entry;
    unsigned char c, attack_mode;

    switch (((unsigned char *)moby)[0xBC]) {
    case STARFIGHTER_COMMAND_TIMED_VOLLEY:
        goto s1;
    case STARFIGHTER_COMMAND_ALTERNATING_ATTACKS:
        goto s2;
    case STARFIGHTER_COMMAND_REPEATED_TARGETED_ATTACK:
        goto s3;
    case STARFIGHTER_COMMAND_WAIT_FOR_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM:
        goto s4;
    case STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_TRACTOR_BEAM:
        goto s3;
    case STARFIGHTER_COMMAND_ALTERNATING_ATTACKS_WITH_CONDITIONAL_MUZZLE_SWAP_AND_TRACTOR_BEAM:
        goto s3;
    case STARFIGHTER_COMMAND_REPEATED_VECTOR_ATTACK:
        goto s2;
    default:
        return;
    }

s1:
    if (((unsigned char *)moby)[0x20] != 4) {
        goto s1b;
    }
    if (!fast_dec_timer(starfighter_data + 0xF8)) {
        return;
    }
    fire_starfighter_homing_rocket(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    ((unsigned char *)starfighter_data)[0xFD]++;
    if (((unsigned char *)starfighter_data)[0xFD] < 11) {
        return;
    }
    ((unsigned char *)starfighter_data)[0xFD] = 0;
    moby[0x20] = 3;
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x12C);
    return;

s1b:
    if (!fast_dec_timer(starfighter_data + 0xF8)) {
        return;
    }
    if (!(*(unsigned short *)(starfighter_data + 0x138) & 1)) {
        if (queue_starfighter_voice_line(moby, starfighter_data, 1, 2)) {
            *(unsigned short *)(starfighter_data + 0x138) |= 1;
        }
    }
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x1E);
    moby[0x20] = 4;
    return;

s3:
    fast_dec_timer(starfighter_data + 0x124);
    fast_dec_timer(starfighter_data + 0x100);
    fast_dec_timer(starfighter_data + 0xF8);
    if (((unsigned char *)moby)[0x20] == 5) {
        goto s308;
    }
    voice_slot_index = *(int *)(starfighter_data + 0x11C);
    if (voice_slot_index != -1) {
        voice_slot_entry = ((char *)&voice_pool) + voice_slot_index * 0x70;
        if (*(int *)(voice_slot_entry + 0x88) == (int)moby &&
            ((unsigned char *)voice_slot_entry)[0x74]) {
            release_voice_slot(voice_slot_index);
        }
        *(int *)(starfighter_data + 0x11C) = -1;
    }
    moby_point_position(moby, 0x13, sv);
    joint_to_hero_distance = fast_vec_dist(sv, ((char *)&D_0013F3D0));
    if (*(int *)(starfighter_data + 0x100) == 0) {
        if (joint_to_hero_distance < 100.0f) {
            if (60.0f < joint_to_hero_distance) {
                if (*(int *)(starfighter_data + 0xF8) != 0) {
                    return;
                }
                if (*(int *)(starfighter_data + 0x124) != 0) {
                    return;
                }
                *(int *)(starfighter_data + 0x100) = scale_game_frames(0x258);
                qcopy(starfighter_data + 0x80, sv);
                ((unsigned char *)starfighter_data)[0xFF] = 0;
                moby[0x20] = 5;
                return;
            }
        }
    }
    if (((unsigned char *)moby)[0xBC] == 3) {
        moby[0x20] = 4;
        if (*(int *)(starfighter_data + 0xF8) != 0) {
            return;
        }
        goto s384;
    }
    if (*(int *)(starfighter_data + 0xF8) != 0) {
        return;
    }
    if (FUN_L00_001fefc8(starfighter_data + 0xFD) == 0) {
        goto s2e;
    }
    if (*(int *)(starfighter_data + 0x124) != 0) {
        goto s2e;
    }
    attack_mode = ((unsigned char *)starfighter_data)[0xFE] ^ 1;
    ((unsigned char *)starfighter_data)[0xFE] = attack_mode;
    if (!attack_mode) {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(4, 6);
        moby[0x20] = 4;
    } else {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(0x14, 0x32);
        moby[0x20] = 3;
    }
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x78);
    return;

s2e:
    if (((unsigned char *)starfighter_data)[0xFE] == 0) {
        goto s380;
    }
    fire_level_13_mine(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x14);
    *(int *)(starfighter_data + 0x124) = scale_game_frames(0xF0);
    return;

s308:
    if (*(int *)(starfighter_data + 0x100) != 0) {
        goto s358;
    }
    if (*(int *)(starfighter_data + 0x124) != 0) {
        return;
    }
    *(int *)(starfighter_data + 0x100) = scale_game_frames(0x258);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x78);
    moby[0x20] = 4;
    allocate_voice_for_target_entry(5, 0, (int)moby);
    return;

s358:
    if ((*(long *)(starfighter_data + 0xF8) & ((((0xFF00L << 32) | 0xFFFFL) << 16) | 0xFFFFL)) !=
        (0x8000L << 41)) {
        return;
    }
s380:
s384:
    fire_starfighter_homing_rocket(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    *(int *)(starfighter_data + 0x124) = scale_game_frames(0x78);
    return;

s2:
    if (!fast_dec_timer(starfighter_data + 0xF8)) {
        return;
    }
    if (((unsigned char *)moby)[0xBC] != 2) {
        goto s2b;
    }
    if (FUN_L00_001fefc8(starfighter_data + 0xFD) == 0) {
        goto s2z;
    }
    attack_mode = ((unsigned char *)starfighter_data)[0xFE] ^ 1;
    ((unsigned char *)starfighter_data)[0xFE] = attack_mode;
    if (!attack_mode) {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(4, 6);
        moby[0x20] = 4;
    } else {
        ((unsigned char *)starfighter_data)[0xFD] = random_int_between(0x14, 0x32);
        moby[0x20] = 3;
    }
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    return;

s2z:
    if (((unsigned char *)starfighter_data)[0xFE] != 0) {
        goto s2y;
    }
    fire_starfighter_homing_rocket(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x3C);
    return;

s2y:
    fire_level_13_mine(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x14);
    return;

s2b:
    moby[0x20] = 3;
    fire_level_13_mine(moby, starfighter_data);
    *(int *)(starfighter_data + 0xF8) = scale_game_frames(0x14);
    return;

s4:
    moby[0x20] = 3;
    if (((unsigned char *)starfighter_data)[0x10E] != 0) {
        return;
    }
    moby[0xBC] = 5;
    queue_starfighter_voice_line(moby, starfighter_data, 0x1B, 2);
    {
        char *z = *(char **)(*(int *)(((char *)&D_00140940)) + 0x78);
        *(int *)(z + 0x88) = 0;
        *(int *)(z + 0xEC) = 0;
        *(int *)(z + 0x8C) = 0;
        *(int *)(z + 0x118) = 0;
    }
    return;
}

typedef struct {
    char           pad0[0x20];
    float          f20;
    short          s24;
    char           pad26[0x4A];
    float          f70;
    float          f74;
    int            i78;
    int            pad7C;
    float          v80[4];
    char           pad90[0xC];
    short          s9C;
    short          s9E;
    int            iA0;
    float          fA4;
    float          fA8;
    float          fAC;
    int            paths[5];
    char          *parts[10];
    char          *owner;
    char          *pF0;
    int            iF4;
    int            iF8;
    unsigned char  bFC;
    unsigned char  bFD;
    unsigned char  bFE;
    unsigned char  bFF;
    int            i100;
    int            hud104;
    int            i108;
    unsigned char  b10C;
    unsigned char  b10D;
    unsigned char  b10E;
    unsigned char  b10F;
    char          *child110;
    int            snd114;
    int            snd118;
    int            snd11C;
    int            light120;
    int            i124;
    int            pad128;
    int            i12C;
    int            pad130;
    int            i134;
    unsigned short h138;
    unsigned char  b13A;
    unsigned char  b13B;
    int            i13C;
    int            pad140;
    short          h144;
    short          h146;
    unsigned char  b148;
} Ship388;

typedef struct {
    char           pad0[0x10];
    float          pos[4];
    unsigned char  state;
    char           pad21[3];
    char          *cls;
    int            pad28;
    float          scale;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short flags;
    char           pad36[0xA];
    float          rot[4];
    char           pad50[0x28];
    Ship388       *d;
    char           pad7C[0x18];
    int            i94;
    char           pad98[0xE];
    short          cls_id;
    char           padA8[8];
    unsigned char  bB0;
    char           padB1[0xB];
    unsigned char  bBC;
    char           padBD[3];
    float          mat[3][4];
} Moby388;

typedef float PVec388[4] __attribute__((aligned(16)));

typedef struct {
    int     n;
    int     pad4[3];
    PVec388 pts[1];
} Path388;

extern Moby388 *D_L13_00174158[];

extern char *spawn_moby_joint_attachment(char *, int, int) __asm__("FUN_L13_002ecd10");

extern char D_L13_001F4F38[];

extern char qwarks_starfighter_high_elapsed_time_adjustment_message[] __asm__("D_L13_00161958");

extern char qwarks_starfighter_mid_elapsed_time_adjustment_message[] __asm__("D_L13_00161968");

extern char jet_camera47[] __asm__("D_L13_00166F80");

extern char qwarks_starfighter_boss_start_time_format[] __asm__("D_L13_001F4EF0");

extern char level13_point_light_slots[] __asm__("D_L13_001805C0");

extern char qwarks_starfighter_tractor_offscreen_abort_message[] __asm__("D_L13_001F4F18");

extern float D_L13_00160660[] MACRO_ADDR;

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

extern int D_0015ED84;

extern int D_0015EEA4;

int D_L13_00161950 = 0;

int D_L13_00161954 = -1;

extern int qwarks_starfighter_command_path_slots[] __asm__("D_L13_001D3448");

extern int qwarks_starfighter_attachment_class_ids[] __asm__("D_L13_001D34D8");

extern int DebugPrint_alt() __asm__("FUN_001e93b0");

extern int allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");

extern int queue_animation_update(int, int, int, int, int, int, int) __asm__("FUN_001ff308");

extern short dialogue_playback_phase[] __asm__("D_0015172A");

extern void reset_numeric_hud_element() __asm__("FUN_L00_00235d80");

extern void hud_update_counter() __asm__("FUN_L00_00235ea0");

extern void FUN_L00_0024f7c8(void *, int, void *);

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern void spawn_explosion(void *,
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

extern void draw_starfighter_tractor_beam();

extern void update_qwarks_starfighter_attack_commands(void *, void *) __asm__("FUN_L13_002eac00");

extern void flag_vendor_item(int) __asm__("FUN_L13_00316f48");

extern void FUN_L06_00249738();

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern char D_0013F350[];

extern void func_L13_002E9F90_2(void *, void *) __asm__("FUN_L13_002e8b58");

extern char D_0013E633_snd[] __asm__("D_0013E533");

void update_qwarks_starfighter(Moby388 *moby) __asm__("FUN_L13_002eb098");

void update_qwarks_starfighter(Moby388 *moby) {
    enum {
        STARFIGHTER_STATE_INITIALIZE = 0,
        STARFIGHTER_STATE_WAIT_FOR_HERO_SHIP = 1,
        STARFIGHTER_STATE_FOLLOW_INITIAL_PATH = 2,
        STARFIGHTER_STATE_FOLLOW_COMMAND_PATH = 3,
        STARFIGHTER_STATE_FOLLOW_COMMAND_PATH_WITH_TIMED_VOLLEY = 4,
        STARFIGHTER_STATE_TRACTOR_BEAM = 5,
        STARFIGHTER_STATE_OWNER_UNAVAILABLE = 6,
        STARFIGHTER_STATE_DEATH_FLIGHT = 7,
        STARFIGHTER_STATE_EXPLODE_AND_REMOVE = 8,
        STARFIGHTER_STATE_RESET = 9
    };
    Ship388 *data = moby->d;
    float    offset[4];
    float    joint[4];
    float    ray_start[4];
    float    probe[4];
    int      i;

    data->i108 = truncate_float_to_s32(data->f20 * 16.0f) & 0xFFF0;
    if (data->i108 >= truncate_float_to_s32(((float)data->s24 - 0.5f) * 16.0f)) {
        data->i108 |= 0xF;
    }
    if (0.5f < data->f20) {
        int segment = ((truncate_float_to_s32(data->f20) % (data->s24 / 6)) * 16) / (data->s24 / 6);
        if (segment < 0)
            segment = 0;
        data->i108 += segment & 0xF;
    } else {
        data->i108 = 0;
    }

    switch (moby->state) {
    case STARFIGHTER_STATE_INITIALIZE: {
        int j;
        if (moby->bB0 != 0xFF && visited52[moby->bB0 + (D_0015ED84 << 4)] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->scale = *(float *)(moby->cls + 0x24) * 4.0f;
        for (i = 0; i < 10; i++) {
            data->parts[i] = spawn_moby_joint_attachment(
                (char *)moby, i, qwarks_starfighter_attachment_class_ids[i]);
        }
        data->owner = (char *)moby;
        for (j = 0; j < 5; j++) {
            char *path = walker_paths[data->paths[j]];
            if (fast_vec_dist(path + 0x10, path + *(int *)path * 16) < 0.5f) {
                *(int *)path -= 1;
            }
        }
        data->fAC = frame_time * 42.0f * 0.1f;
        data->f70 = fast_cos(moby->rot[2]) * data->fAC;
        data->f74 = fast_sin(moby->rot[2]) * data->fAC;
        data->i78 = 0;
        qcopy(moby->pos, walker_paths[data->paths[0]] + 0x10);
        data->s9E = 0;
        data->s9C = 1;
        moby->b30 = 0xFF;
        moby->s32 = 0xFF;
        data->b10C = 1;
        data->bFE = 0;
        data->bFC = 0;
        moby->state = 1;
        data->b10E = 8;
        data->child110 = 0;
        data->snd114 = -1;
        data->snd118 = -1;
        data->snd11C = -1;
        data->f20 = data->s24;
        data->light120 = -1;
        data->h138 = 0;
        data->b148 = 0;
        data->i13C = 0;
        if (data->hud104 != -1) {
            set_animation_parameter(data->hud104, 0);
            data->hud104 = -1;
        }
        moby->i94 = 0;
        moby->flags |= 0x41;
        break;
    }
    case STARFIGHTER_STATE_WAIT_FOR_HERO_SHIP: {
        char *hero = ((char *)&D_0013F350);
        if (*(int *)(hero + 0x2084) == 0x32 && *(Moby388 **)(hero + 0x15F0) != 0) {
            moby->flags &= 0xFFBE;
            moby->i94 = *(int *)(moby->cls + 0x10);
            moby->s32 = 0x3FF;
            if (*(Moby388 **)(hero + 0x15F0) != 0) {
                char *ship = (char *)(*(Moby388 **)(hero + 0x15F0))->d;
                int   elapsed;
                if (D_L13_00161954 == -1) {
                    D_L13_00161954 = D_0015EEA4;
                    DebugPrint_alt(qwarks_starfighter_boss_start_time_format,
                                   D_0015EEA4,
                                   D_0015EEA4 / 60,
                                   D_0015EEA4 / 360);
                }
                elapsed = (unsigned int)(D_0015EEA4 - D_L13_00161954) / 360;
                if (elapsed >= 0x29) {
                    DebugPrint_alt(qwarks_starfighter_high_elapsed_time_adjustment_message);
                    data->s24 = 0x66;
                    data->f20 = 102.0f;
                    *(int *)(ship + 0x138) = 0x221;
                } else if (elapsed >= 0x1A) {
                    DebugPrint_alt(qwarks_starfighter_mid_elapsed_time_adjustment_message);
                    data->s24 = 0x78;
                    data->f20 = 120.0f;
                    *(int *)(ship + 0x138) = 0x234;
                }
            }
            moby->state = 2;
        }
        break;
    }
    case STARFIGHTER_STATE_FOLLOW_INITIAL_PATH: {
        {
            char *hero = ((char *)&D_0013F350);
            if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45) {
                (*(Moby388 **)(hero + 0x15F0))->bBC = 0;
            }
        }
        data->hud104 = queue_animation_update(0x11,
                                              0xFFFF,
                                              (int)reset_numeric_hud_element,
                                              (int)hud_update_counter,
                                              (int)FUN_L06_00249738,
                                              (int)&data->i108,
                                              (data->s24 << 4) | 0xF);
        data->fAC += (frame_time * 42.0f - data->fAC) * 0.1f;
        if (advance_moby_along_selected_path(moby, data, 0, 1) != 0) {
            char *hero = ((char *)&D_0013F350);
            char *ship;
            moby->bBC = 1;
            ship = (char *)(*(Moby388 **)(hero + 0x15F0))->d;
            *(int *)(ship + 0x88) = 0;
            *(int *)(ship + 0xEC) = 0;
            *(int *)(ship + 0x8C) = 0;
            *(int *)(ship + 0x118) = 0;
            data->b13B = qwarks_starfighter_command_path_slots[moby->bBC];
            data->iF8 = scale_game_frames(0x14);
            moby->state = 3;
            data->s9C = select_point_index_by_distance(
                moby->pos, walker_paths[data->paths[data->b13B]], 0.0f);
            data->s9C = *(unsigned short *)((char *)data + 0x9C) + random_int_between(3, 7);
            data->s9C = data->s9C % *(int *)walker_paths[data->paths[data->b13B]];
            data->s9E = data->s9C - 1;
            if (data->s9E < 0) {
                data->s9E = *(unsigned short *)walker_paths[data->paths[data->b13B]] - 1;
            }
        }
        break;
    }
    case STARFIGHTER_STATE_FOLLOW_COMMAND_PATH:
    case STARFIGHTER_STATE_FOLLOW_COMMAND_PATH_WITH_TIMED_VOLLEY:
        data->hud104 = queue_animation_update(0x11,
                                              0xFFFF,
                                              (int)reset_numeric_hud_element,
                                              (int)hud_update_counter,
                                              (int)FUN_L06_00249738,
                                              (int)&data->i108,
                                              (data->s24 << 4) | 0xF);
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        update_qwarks_starfighter_attack_commands(moby, data);
        if (update_moby_path_following_away_from_hero(moby, data, data->b13B, 1) & 2) {
            if (data->b13B != qwarks_starfighter_command_path_slots[moby->bBC]) {
                data->b13B = qwarks_starfighter_command_path_slots[moby->bBC];
                data->s9C = select_point_index_by_distance(
                    moby->pos,
                    walker_paths[data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]],
                    0.0f);
                data->s9C = *(unsigned short *)((char *)data + 0x9C) + random_int_between(3, 7);
                data->s9C =
                    data->s9C %
                    *(int *)
                        walker_paths[data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]];
                data->s9E = data->s9C - 1;
                if (data->s9E < 0) {
                    data->s9E =
                        *(unsigned short *)walker_paths
                            [data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]] -
                        1;
                }
            }
        }
        if (moby->bBC != 4) {
            process_moby_attachment_damage(moby, data);
        } else {
            trooper_anim_finish(moby, (char *)data + 0x60);
        }
        break;
    case STARFIGHTER_STATE_TRACTOR_BEAM: {
        float hero_distance;
        data->hud104 = queue_animation_update(0x11,
                                              0xFFFF,
                                              (int)reset_numeric_hud_element,
                                              (int)hud_update_counter,
                                              (int)FUN_L06_00249738,
                                              (int)&data->i108,
                                              (data->s24 << 4) | 0xF);
        update_qwarks_starfighter_attack_commands(moby, data);
        FUN_L00_0024f7c8(moby, 0x13, joint);
        hero_distance = fast_vec_dist(data->v80, ((char *)&D_0013F3D0));
        if (data->snd11C != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd11C * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0)
                goto have11C;
        }
        data->snd11C = -1;
        data->snd11C = allocate_voice_for_target_entry(4, 4, (int)moby);
    have11C:
        if (data->bFF == 0) {
            {
                char *hero = ((char *)&D_0013F350);
                if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                    (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45) {
                    (*(Moby388 **)(hero + 0x15F0))->bBC = 0;
                }
            }
            data->fAC += (frame_time * 30.0f - data->fAC) * 0.1f;
            if (frame_time * 40.0f < hero_distance) {
                fast_vec_sub(offset, ((char *)&D_0013F3D0), data->v80);
                fast_vec_normalize(offset, offset, frame_time * 40.0f);
                fast_vec_add(data->v80, data->v80, offset);
            } else {
                qcopy(offset, ((char *)&D_0013F3D0));
                data->iF8 = scale_game_frames(0x78);
                if (!(data->h138 & 4)) {
                    if (queue_starfighter_voice_line(moby, data, 0x17, 4) != 0) {
                        data->h138 |= 4;
                    }
                }
                data->bFF = 1;
            }
            if (advance_moby_along_selected_path(moby, data, data->b13B, 1) & 2) {
                if (data->b13B != qwarks_starfighter_command_path_slots[moby->bBC]) {
                    data->b13B = qwarks_starfighter_command_path_slots[moby->bBC];
                    data->s9C = select_point_index_by_distance(
                        moby->pos,
                        walker_paths[data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]],
                        0.0f);
                    data->s9C = *(unsigned short *)((char *)data + 0x9C) + random_int_between(3, 7);
                    data->s9C = data->s9C %
                                *(int *)walker_paths
                                    [data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]];
                    data->s9E = data->s9C - 1;
                    if (data->s9E < 0) {
                        data->s9E =
                            *(unsigned short *)walker_paths
                                [data->paths[qwarks_starfighter_command_path_slots[moby->bBC]]] -
                            1;
                    }
                }
            }
            fast_vec_sub(ray_start, data->v80, joint);
            fast_vec_normalize(ray_start, ray_start, 8.0f);
            fast_vec_add(ray_start, ray_start, joint);
            if (collision_line(ray_start, data->v80, 0, moby, 0) != 0) {
                if (D_L13_00174158[0] == 0 || D_L13_00174158[0]->cls_id == 0xD4) {
                    data->i100 = 0;
                }
            }
            qcopy(probe, data->v80);
            probe[3] = 1.0f;
            if (fast_bsphere_check(probe, 100.0f) <= 0 && moby->b31 == 0) {
                DebugPrint_alt(qwarks_starfighter_tractor_offscreen_abort_message);
                data->i100 = 0;
            }
            if (fast_vec_dist(ray_start, ((char *)&D_0013F3D0)) > 100.0f) {
                DebugPrint_alt(D_L13_001F4F38);
                data->i100 = 0;
            }
        } else {
            char *hero = ((char *)&D_0013F350);
            float yaw, pitch, distance;
            yaw = fast_arc_tan(*(float *)(hero + 0x80) - moby->pos[0],
                               *(float *)(hero + 0x84) - moby->pos[1]);
            pitch = -fast_arc_tan(vector_distance_xy(moby->pos, hero + 0x80),
                                  *(float *)(hero + 0x88) - moby->pos[2]);
            update_moby_spring_steering_and_banking(moby, data, yaw, pitch);
            if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45) {
                (*(Moby388 **)(hero + 0x15F0))->bBC = 1;
            }
            data->fAC *= 0.8f;
            fast_vec_dist(moby->pos, ((char *)&D_0013F3D0));
            FUN_L00_0024f7c8(moby, 0x13, probe);
            fast_vec_sub(ray_start, data->v80, probe);
            fast_vec_normalize(ray_start, ray_start, 30.0f);
            fast_vec_add(ray_start, ray_start, probe);
            fast_vec_sub(offset, ray_start, data->v80);
            distance = fast_vec_dist(ray_start, ((char *)&D_0013F3D0));
            if (distance != 30.0f) {
                fast_vec_normalize(offset,
                                   offset,
                                   (frame_time * 15.0f < distance) ? frame_time * 15.0f : distance);
                fast_vec_add(data->v80, data->v80, offset);
            }
            {
                char *hero = ((char *)&D_0013F350);
                if (*(Moby388 **)(hero + 0x15F0) != 0) {
                    Moby388 *ship_moby = *(Moby388 **)(hero + 0x15F0);
                    char    *ship;
                    float    yaw_error, abs_yaw_error, pitch_error, abs_pitch_error;
                    qcopy(ship_moby->pos, data->v80);
                    ship = (char *)ship_moby->d;
                    yaw_error =
                        fast_subtract_rotations(fast_arc_tan(moby->pos[0] - ship_moby->pos[0],
                                                             moby->pos[1] - ship_moby->pos[1]),
                                                *(float *)(ship + 0x38));
                    abs_yaw_error = absolute_float(yaw_error);
                    pitch_error = fast_subtract_rotations(
                        -fast_arc_tan(
                            vector_distance_xy((*(Moby388 **)(hero + 0x15F0))->pos, moby->pos),
                            moby->pos[2] - (*(Moby388 **)(hero + 0x15F0))->pos[2]),
                        *(float *)(ship + 0x34));
                    abs_pitch_error = absolute_float(pitch_error);
                    if (abs_yaw_error > 0.38397244f) {
                        *(float *)(ship + 0x38) =
                            fast_add_rotations(*(float *)(ship + 0x38),
                                               normalize_angle((abs_yaw_error - 0.38397244f) /
                                                               0.17453292f * yaw_error));
                    }
                    if (abs_pitch_error > 0.38397244f) {
                        *(float *)(ship + 0x34) =
                            fast_add_rotations(*(float *)(ship + 0x34),
                                               normalize_angle((abs_pitch_error - 0.38397244f) /
                                                               0.17453292f * pitch_error));
                    }
                }
            }
        }
        if (moby->bBC != 4) {
            process_moby_attachment_damage(moby, data);
        } else {
            trooper_anim_finish(moby, (char *)data + 0x60);
        }
        {
            char *hero = ((char *)&D_0013F350);
            if (fast_difference_between_rotations(
                    moby->rot[2],
                    fast_arc_tan(*(float *)(hero + 0x80) - moby->pos[0],
                                 *(float *)(hero + 0x84) - moby->pos[1])) < 1.0471976f) {
                add_draw_callback(draw_starfighter_tractor_beam, moby);
            }
        }
        break;
    }
    case STARFIGHTER_STATE_OWNER_UNAVAILABLE: {
        char *dialog;
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        advance_moby_along_selected_path(moby, data, data->b13B, 1);
        dialog = (char *)&music_stream_state;
        if (*(int *)(dialog + 0x50) == 0) {
            int line = *(int *)(dialog + 0x1C);
            if (line == -1 && (short)data->h138 <= 0) {
                if (queue_starfighter_voice_line(moby, data, D_L13_00161950 + 0x1D, 1) != 0) {
                    D_L13_00161950 = (D_L13_00161950 + 1) % 4;
                    data->h138 += 1;
                }
            }
        }
        break;
    }
    case STARFIGHTER_STATE_DEATH_FLIGHT: {
        float target_yaw, target_pitch;
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        if (data->iF8 == 1) {
            detach_moby_joint_attachment(moby, data, 8);
            allocate_voice_for_target_entry(7, 0, (int)moby);
        }
        fast_dec_timer(&data->iF8);
        if (advance_moby_along_selected_path(moby, data, data->b13B, 0) != 0 ||
            data->s9C >= *(int *)walker_paths[data->paths[data->b13B]] - 1) {
            data->iF8 = scale_game_frames(0xB4);
            moby->state = 8;
        }
        moby->rot[0] = fast_add_rotations(moby->rot[0], frame_time * 4.1887903f);
        {
            Path388 *path = (Path388 *)walker_paths[data->paths[data->b13B]];
            int      i = data->s9C;
            int      n = path->n;
            target_yaw =
                fast_arc_tan(path->pts[(i + 3 >= n - 1) ? n - 1 : i + 3][0] - path->pts[i][0],
                             path->pts[(i + 3 < n - 1) ? i + 3 : n - 1][1] - path->pts[i][1]);
        }
        {
            char *path = walker_paths[data->paths[data->b13B]];
            int   i = data->s9C;
            int   n = *(int *)path;
            float flat_distance = vector_distance_xy(
                path + (i * 16 + 0x10), path + (((i + 3 >= n - 1) ? n - 1 : i + 3) * 16 + 0x10));
            Path388 *path2 = (Path388 *)walker_paths[data->paths[data->b13B]];
            int      i2 = data->s9C;
            int      path_point_count = path2->n;
            target_pitch = -fast_arc_tan(
                flat_distance,
                path2->pts[(i2 + 3 >= path_point_count - 1) ? path_point_count - 1 : i2 + 3][2] -
                    path2->pts[i2][2]);
        }
        FUN_L00_00258278(moby,
                         &data->fA8,
                         target_yaw,
                         frame_time_sq * 3.1415927f,
                         frame_time_sq * 6.2831855f,
                         frame_time * 6.2831855f);
        moby->rot[1] = spring_turn(&data->fA4,
                                   moby->rot[1],
                                   target_pitch,
                                   frame_time_sq * 3.1415927f,
                                   frame_time_sq * 6.2831855f,
                                   frame_time * 6.2831855f);
        moby->scale *= 0.99925f;
        data->fAC *= 0.9992f;
        if (random_integer_below(0x11) == 0) {
            float size_ratio = moby->scale / (*(float *)(moby->cls + 0x24) * 3.0f);
            fast_vec_normalize(joint, moby->mat[0], random_float_between(-5.0f, 5.0f) * size_ratio);
            fast_vec_add(offset, moby->pos, joint);
            fast_vec_normalize(joint, moby->mat[1], random_float_between(-4.0f, 4.0f) * size_ratio);
            fast_vec_add(offset, offset, joint);
            fast_vec_normalize(joint, moby->mat[2], random_float_between(-4.0f, 4.0f) * size_ratio);
            fast_vec_add(offset, offset, joint);
            cart_burst(moby, offset, -1, size_ratio * 1.6f, 13.0f);
        }
        func_L13_002E9F90_2(moby, data);
        {
            char *dialog = (char *)&music_stream_state;
            if (*(int *)(dialog + 0x50) == 0 && *(int *)(dialog + 0x1C) == -1) {
                if (data->iF8 < scale_game_frames(0x1A4) && data->i13C < 0) {
                    if (queue_starfighter_voice_line(moby, data, 0x23, 1) != 0) {
                        data->i13C &= 0x7FFFFFFF;
                    }
                } else if (data->iF8 < scale_game_frames(0xB4) && (data->i13C & 0x40000000) &&
                           queue_starfighter_voice_line(moby, data, 0x24, 1) != 0) {
                    data->i13C &= 0xBFFFFFFF;
                }
            }
        }
        break;
    }
    case STARFIGHTER_STATE_EXPLODE_AND_REMOVE: {
        char *view;
        if (*(Moby388 **)(((char *)&D_00140940)) != 0 &&
            (*(Moby388 **)(((char *)&D_00140940)))->cls_id == 0x45) {
            (*(Moby388 **)(((char *)&D_00140940)))->bBC = 0;
        }
        moby->rot[0] = fast_add_rotations(moby->rot[0], frame_time * 4.1887903f);
        view = jet_camera47;
        moby->rot[2] = *(float *)(view + 0x158);
        moby->rot[1] = *(float *)(view + 0x154);
        moby->scale *= 0.98f;
        if (data->iF8 == scale_game_frames(0x1E)) {
            spawn_explosion(moby,
                            &data->f70,
                            0,
                            0.0f,
                            0.0f,
                            0x1E,
                            8,
                            0x20,
                            5.0f,
                            3.0f,
                            0.0f,
                            7,
                            1.0f,
                            0.0f,
                            1,
                            5,
                            -1,
                            0);
        }
        func_L13_002E9F90_2(moby, data);
        if (fast_dec_timer(&data->iF8) != 0) {
            Moby388 *owner = (Moby388 *)((char *)moby_base + (data->i134 << 8));
            owner->state = 8;
            owner->flags |= 2;
            if (data->light120 != -1) {
                release_point_light_slot(data->light120);
            }
            flag_vendor_item(data->i12C);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case STARFIGHTER_STATE_RESET: {
        char  *path;
        char **parts;
        if (data->pF0 != 0) {
            mark_moby_for_removal(data->pF0);
            data->pF0 = 0;
        }
        if (data->child110 != 0) {
            mark_moby_for_removal(data->child110);
            data->child110 = 0;
        }
        parts = data->parts;
        for (i = 0; i < 10; i++) {
            if (parts[i] != 0) {
                mark_moby_for_removal(parts[i]);
                parts[i] = 0;
            }
        }
        if (data->snd118 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd118 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                release_voice_slot(data->snd118);
            }
            data->snd118 = -1;
        }
        if (data->snd114 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd114 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                release_voice_slot(data->snd114);
            }
            data->snd114 = -1;
        }
        if (data->snd11C != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd11C * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                release_voice_slot(data->snd11C);
            }
            data->snd11C = -1;
        }
        if (data->light120 != -1) {
            release_point_light_slot(data->light120);
        }
        if (data->hud104 != -1) {
            set_animation_parameter(data->hud104, 0);
            data->hud104 = -1;
        }
        path = walker_paths[data->paths[0]];
        qcopy(moby->pos, path + 0x10);
        moby->rot[2] = fast_arc_tan(*(float *)(path + 0x20) - moby->pos[0],
                                    *(float *)(path + 0x24) - moby->pos[1]);
        data->b10C = 1;
        data->s9C = 0;
        data->s9E = 0;
        data->iA0 = 0;
        data->fA4 = 0;
        data->fA8 = 0;
        data->fAC = 0;
        data->iF4 = 0;
        data->iF8 = 0;
        data->bFC = 0;
        data->bFD = 0;
        data->bFE = 0;
        data->bFF = 0;
        data->i100 = 0;
        data->b10D = 0;
        data->b10E = 0;
        data->b10F = 0;
        data->i124 = 0;
        data->h138 = 0;
        data->b13A = 0;
        data->b13B = 0;
        data->i13C = 0;
        data->h144 = 0;
        data->h146 = 0;
        data->b148 = 0;
        moby->bBC = 0;
        qzero(&data->f70);
        qzero(data->v80);
        moby->state = 0;
        return;
    }
    }

    if (moby->pos[0] < 22.0f)
        moby->pos[0] = 22.0f;
    if (moby->pos[0] > 1001.0f)
        moby->pos[0] = 1001.0f;
    if (moby->pos[1] < 22.0f)
        moby->pos[1] = 22.0f;
    if (moby->pos[1] > 1001.0f)
        moby->pos[1] = 1001.0f;
    if (moby->pos[2] < 22.0f)
        moby->pos[2] = 22.0f;
    if (moby->pos[2] > 1001.0f)
        moby->pos[2] = 1001.0f;

    if (moby->bBC == 4) {
        if (data->child110 == 0) {
            data->child110 = spawn_qwarks_starfighter_shield(moby);
        } else if (((Moby388 *)data->child110)->state >= 3) {
            ((Moby388 *)data->child110)->state = 1;
        }
        if (data->snd118 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd118 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0)
                goto have118;
        }
        data->snd118 = -1;
        data->snd118 = allocate_voice_for_target_entry(6, 4, (int)moby);
    have118:;
    } else {
        if (data->child110 != 0) {
            Moby388 *child;
            if (data->snd118 != -1) {
                char *voice = D_0013E633_snd + 0x1D + data->snd118 * 0x70;
                if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0) {
                    release_voice_slot(data->snd118);
                }
                data->snd118 = -1;
            }
            child = (Moby388 *)data->child110;
            if (child == 0) {
                data->child110 = 0;
            } else if (child->cls_id != 0x160) {
                data->child110 = 0;
            } else if (child->state == 0xFE) {
                data->child110 = 0;
            } else if (child->state == 0xFD) {
                data->child110 = 0;
            } else if (child->state < 3) {
                child->state = 3;
            }
        }
    }

    if (moby->state >= 2) {
        update_moby_joint_attachments(moby, data);
        if (moby->state != 8 || scale_game_frames(0x78) < data->iF8) {
            emit_gated_moby_joint_effects(moby, data);
        }
        if (data->snd114 != -1) {
            char *voice = D_0013E633_snd + 0x1D + data->snd114 * 0x70;
            if (*(Moby388 **)(voice + 0x88) == moby && *(unsigned char *)(voice + 0x74) != 0)
                goto have114;
        }
        data->snd114 = -1;
        data->snd114 = allocate_voice_for_target_entry(0, 4, (int)moby);
    have114:
        transform_vector_by_basis(offset, D_L13_00160660, moby->mat[0]);
        fast_vec_normalize(offset, offset, 7.0f);
        fast_vec_sub(offset, moby->pos, offset);
        offset[3] = 50.0f;
        if (data->light120 == -1) {
            data->light120 = allocate_colored_effect_slot(offset, 50.0f, 0.0f, 3.0f, 0.5f, 0.01f);
        } else {
            char *light = level13_point_light_slots + (data->light120 << 5);
            qcopy(light + 0x10, offset);
        }
    }
    data->b10F = 0;

    {
        Moby388 *owner = (Moby388 *)((char *)moby_base + (data->i134 << 8));
        if (moby->state != 6) {
            if (owner == 0 || owner->cls_id != 0x45 || owner->state == 0xFE ||
                owner->state == 0xFD || owner->state == 7) {
                moby->state = 6;
                if (queue_starfighter_voice_line(moby, data, D_L13_00161950 + 0x1D, 1) != 0) {
                    D_L13_00161950 = (D_L13_00161950 + 1) % 4;
                    data->h138 = 1;
                } else {
                    data->h138 = 0;
                }
            }
        }
    }

    if (data->b13A != 0) {
        if (dialogue_playback_phase[0] == 3) {
            continue_audio_stream_if_ready();
            data->h144 = scale_game_frames(0x168);
            data->h146 = scale_game_frames(0x3C);
            data->b13A = 0;
        }
    } else {
        if (moby->state != 6) {
            char *hero = ((char *)&D_0013F350);
            if (*(int *)(hero + 0x2084) == 0x32) {
                if (*(Moby388 **)(hero + 0x15F0) != 0 &&
                    (*(Moby388 **)(hero + 0x15F0))->cls_id == 0x45 &&
                    (unsigned int)(*(int *)((char *)(*(Moby388 **)(hero + 0x15F0))->d + 0x120) -
                                   1) < 0x270F &&
                    random_integer_below(2) == 0) {
                    queue_starfighter_voice_line(moby, data, 3, 9);
                }
            }
        }
    }
    fast_dec_timer_s16(&data->h144);
    fast_dec_timer_s16(&data->h146);
    if (moby->state >= 2 && moby->state <= 5 && (((unsigned char *)D_0013E533)[0x242C] & 1)) {
        moby->state = 9;
    }
}

extern char *create_moby(int);

extern char *create_moby(int) __asm__("FUN_0020c4f8");

char *spawn_moby_joint_attachment(char *parent_moby, int attachment_joint_index, int id) {
    char *moby = create_moby(id);
    if (moby != 0) {
        char *data;
        float f = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * MOBY(parent_moby)->scale /
                  *(float *)(((char *)MOBY(parent_moby)->pclass) + 0x24);
        data = ((char *)MOBY(moby)->pvars);
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        MOBY(moby)->scale = f;
        *(int *)(data + 0x44) = attachment_joint_index;
        moby_point_position(parent_moby, attachment_joint_index, moby + 0x10);
        *(char **)(data + 0x3C) = parent_moby;
        update_entity(moby);
        qcopy(moby + 0x40, parent_moby + 0x40);
        *(OvlQuad *)&MOBY(moby)->rot_row0 = *(OvlQuad *)&MOBY(parent_moby)->rot_row0;
        *(OvlQuad *)&MOBY(moby)->rot_row1 = *(OvlQuad *)&MOBY(parent_moby)->rot_row1;
        *(OvlQuad *)&MOBY(moby)->rot_row2 = *(OvlQuad *)&MOBY(parent_moby)->rot_row2;
        *(int *)(data + 0x40) = scale_game_frames(0x258);
        update_entity(moby);
    }
    return moby;
}

extern int scale_game_frames(int);

char D_L13_001F4F38[] = "tractor too long, abort\n";
