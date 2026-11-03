/* Level 02 moby class skids_agent. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/game/usage_stats.h"

#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"

#include "rnc/overlay/l02/mobys/watcher_npc.h"
#include "rnc/overlay/l02/mobys.h"
#include "rnc/overlay/shared/streaming.h"

extern u8 D_0013D506 NOT_SDA;
extern u8            big_head_cheat_enabled_macro __asm__("D_0015EDB0") MACRO_ADDR;
extern s32           level_game_mode_macro __asm__("D_L02_0015F5C4") MACRO_ADDR;
extern WatcherReward D_L02_0016CE60;
extern char          skid_mcmarx_dialogue_data[] __asm__("D_L02_00161D00");

extern void begin_dialogue_camera_transition(struct Moby *, f32) __asm__("FUN_L01_002783a8");
extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void
update_joint_animation(f32, f32, struct Moby *, void *, s32) __asm__("FUN_L00_002628d8");

extern WatcherReward talk_camera __asm__("D_L02_0016CE60") NOT_SDA; /* dialogue camera spot */
extern char          skids_agent_dialogue_data[] __asm__("D_L02_00161CF0");

#include "rnc/overlay/shared/world/cuboid.h"

extern f32              D_L02_0015F3FC;
extern f32              skids_agent_prop_local_offset[2] __asm__("D_L02_00161CE0") __attribute__((sda)); /* prop offset vector; declared small so it is reached through $gp as in retail */
extern struct UsageStat D_00141968[] NOT_SDA; /* usage records 36 on (usage_stats + 0x120) */
extern u8               item_available[] __asm__("D_0013D4C0") NOT_SDA; /* by item id */
extern void advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern f32  approach_value(f32 *p, f32 target, f32 maxstep) __asm__("FUN_00213ed8");

/* Aridia NPC with a carried prop moby (data+0x178): dialogue state machine (the hint in state 1 is usage
 * record 50), a path-camera ride (3/4) that fades D_L02_0015F3FC in and out, then the shared head-tracking tail. */
void skids_agent_update(struct Moby *moby) __asm__("FUN_L02_002e0dc0");

typedef struct SkidsAgentVars {
    u8           pad_0[0x4];
    s16          dialogue_mode;
    u8           pad_6[0x2];
    u8           any_direction;
    u8           pad_9[0x3];
    f32          radius;
    u8           pad_10[0x10];
    char        *dialogue_data;
    u8           pad_24[0x1C];
    u8           joint_0_animation;
    u8           pad_41[0x63];
    f32          joint_0_target_pitch;
    f32          joint_0_target_yaw;
    u8           pad_AC[0x4];
    f32          joint_0_scale;
    u8           pad_B4[0xC];
    u8           joint_1_animation;
    u8           pad_C1[0x67];
    f32          joint_1_target_yaw;
    u8           pad_12C[0x24];
    s32          return_camera_cuboid_index;
    s32          clip_volume_154;
    s32          camera_start_cuboid_index;
    s32          camera_end_cuboid_index;
    u8           idle_look_target_position;
    u8           pad_161[0x11];
    s16          timer_172;
    s16          timer_174;
    s16          timer_176;
    struct Moby *carried_prop;
    f32          fade_amount;
    f32          camera_blend_t;
    f32          camera_blend_speed;
} SkidsAgentVars;

/* Moby class 786 */
void skids_agent_update(struct Moby *moby) {
    enum {
        SKIDS_AGENT_INITIALIZE = 0,
        SKIDS_AGENT_IDLE_AND_TALK = 1,
        SKIDS_AGENT_CONVERSATION_ACTIVE = 2,
        SKIDS_AGENT_INTERPOLATE_TRANSFORM_AND_FADE = 3,
        SKIDS_AGENT_FADE_TO_ZERO_AND_DELETE = 4,
        SKIDS_AGENT_END_CONVERSATION = 5
    };
    SkidsAgentVars *skids_agent_vars = (SkidsAgentVars *)moby->pvars;
    s32             usage_time_value;
    f32             fade;
    struct Moby    *prop;

    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    switch (moby->state) {
    case SKIDS_AGENT_INITIALIZE:
        if (item_available[0x2D] != 0) {
            goto kill;
        }
        skids_agent_vars->dialogue_data = skids_agent_dialogue_data;
        moby->state = SKIDS_AGENT_IDLE_AND_TALK;
        initialize_npc_dialogue(moby, skids_agent_vars);
        initialize_moby_grounding(moby);
        prop = create_moby(0x38);
        skids_agent_vars->carried_prop = prop;
        prop->draw_dist = 0x40;
        skids_agent_vars->carried_prop->visible = 1;
        skids_agent_vars->carried_prop->spawn_frame = moby->spawn_frame;
        skids_agent_vars->carried_prop->flags |= 0x100;
        break;
    case SKIDS_AGENT_IDLE_AND_TALK:
        if (skids_agent_vars->any_direction == 1 &&
            is_point_inside_clip_volume(&hero.motion.pos, skids_agent_vars->clip_volume_154) != 0) {
            skids_agent_vars->radius = 255.0f;
        } else {
            skids_agent_vars->radius = 3.7f;
        }
        if (update_npc_dialogue(moby, skids_agent_vars) != 0) {
            begin_dialogue_camera_transition(moby, 2.2f);
            moby->state = SKIDS_AGENT_CONVERSATION_ACTIVE;
        } else if (item_available[0x5] == 0 &&
                   vector_distance_xy(&moby->pos, &hero.motion.pos) < 3.0f) {
            if (D_00141968[14].count != 0) {
                usage_time_value =
                    scale_game_frames(D_0015EEA4) - D_00141968[14].latest_time_bucket * 600;
                if ((int)(scale_game_frames(0x12) * 60.0f) < usage_time_value ||
                    D_00141968[14].latest_time_bucket * 600 == 0) {
                    help_request(0x7D4, 0xE);
                } else {
                    usage_time_value = scale_game_frames(D_0015EEA4) / 600;
                    if (D_00141968[14].latest_time_bucket < usage_time_value) {
                        D_00141968[14].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
                    }
                }
            } else {
                D_00141968[14].count++;
                usage_time_value = scale_game_frames(D_0015EEA4) / 600;
                if (D_00141968[14].latest_time_bucket < usage_time_value) {
                    D_00141968[14].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
                }
                D_00141968[14].level_mask =
                    D_00141968[14].level_mask | (1 << current_level_index) | 0x80000000;
            }
        }
        if (moby->prev_seq == 0 && fast_dec_timer_s16(&skids_agent_vars->timer_172) != 0) {
            skids_agent_vars->timer_172 =
                truncate_float_to_s32(scale_time(random_float_between(1200.0f, 2400.0f)));
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (moby->prev_seq == 1 && (moby->anim_flags & 2)) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    case SKIDS_AGENT_CONVERSATION_ACTIVE:
        if (level_game_mode != 2) {
            moby->state = SKIDS_AGENT_IDLE_AND_TALK;
            if (skids_agent_vars->dialogue_mode == 0) {
                jet_clear_persistent(moby->mission);
                if (skids_agent_vars->return_camera_cuboid_index != -1) {
                    char *camera_transform_entry =
                        (char *)l02_cuboids + (skids_agent_vars->return_camera_cuboid_index << 7);
                    jet_reset_camera(camera_transform_entry + 0x30, camera_transform_entry + 0x70);
                }
            } else if (skids_agent_vars->dialogue_mode == 2) {
                moby->visible = 0;
                moby->collision = 0;
                moby->flags |= 1;
                if (skids_agent_vars->carried_prop != 0) {
                    skids_agent_vars->carried_prop->flags |= 1;
                    skids_agent_vars->carried_prop->visible = 0;
                }
                start_indexed_disc_stream(2);
                moby->state = SKIDS_AGENT_END_CONVERSATION;
            }
        } else if (talk_camera.placed == 0) {
            fast_vec_normalize(&talk_camera.pos, &moby->rot_row0, 2.5f);
            fast_vec_add(&talk_camera.pos, &talk_camera.pos, &moby->pos);
            talk_camera.pos.z = probe_ground_height(&talk_camera.pos, 0, 0.5f);
            clear_u64_value(&talk_camera.vel);
            talk_camera.vel.z = fast_add_rotations(moby->rot.z, 3.14159f);
            talk_camera.placed = 1;
        }
        break;
    case SKIDS_AGENT_END_CONVERSATION:
        grant_inventory_item(5, 1);
        memcard_save_data(0, -1);
        goto kill;
    case SKIDS_AGENT_INTERPOLATE_TRANSFORM_AND_FADE: {
        Vec4 pos;
        Vec4 rot;
        Vec4 from;
        Vec4 to;
        advance_accelerated_scalar(&skids_agent_vars->camera_blend_t,
                                   &skids_agent_vars->camera_blend_speed,
                                   1.0f,
                                   frame_time_sq * 0.1f,
                                   frame_time_sq * 0.25f,
                                   frame_time * 0.5f);
        lerp_vector(&pos,
                    (char *)l02_cuboids + (skids_agent_vars->camera_start_cuboid_index << 7) + 0x30,
                    (char *)l02_cuboids + (skids_agent_vars->camera_end_cuboid_index << 7) + 0x30,
                    skids_agent_vars->camera_blend_t);
        qcopy_nc(&from,
                 (char *)((skids_agent_vars->camera_start_cuboid_index << 7) + (s32)l02_cuboids) +
                     0x70);
        qcopy_nc(&to,
                 (char *)((skids_agent_vars->camera_end_cuboid_index << 7) + (s32)l02_cuboids) +
                     0x70);
        rot.f[2] = fast_add_rotations(fast_subtract_rotations(to.f[2], from.f[2]) *
                                          skids_agent_vars->camera_blend_t,
                                      from.f[2]);
        rot.f[1] = fast_add_rotations(fast_subtract_rotations(to.f[1], from.f[1]) *
                                          skids_agent_vars->camera_blend_t,
                                      from.f[1]);
        rot.f[0] = fast_add_rotations(fast_subtract_rotations(to.f[0], from.f[0]) *
                                          skids_agent_vars->camera_blend_t,
                                      from.f[0]);
        set_camera_position(&pos);
        set_camera_rotation(&rot);
        if (skids_agent_vars->camera_blend_t < 1.0f) {
            approach_value(&skids_agent_vars->fade_amount, 0.0f, 0.1f);
        } else {
            approach_value(&skids_agent_vars->fade_amount, 1.0f, 0.1f);
            if (skids_agent_vars->fade_amount == 1.0f) {
                moby->state = SKIDS_AGENT_FADE_TO_ZERO_AND_DELETE;
                jet_end_mount(0);
            }
        }
        D_L02_0015F3FC = skids_agent_vars->fade_amount;
        break;
    }
    case SKIDS_AGENT_FADE_TO_ZERO_AND_DELETE: {
        approach_value(&skids_agent_vars->fade_amount, 0.0f, 0.1f);
        fade = skids_agent_vars->fade_amount;
        D_L02_0015F3FC = fade;
        if (fade == 0.0f) {
            if (skids_agent_vars->carried_prop != 0) {
                mark_moby_for_removal(skids_agent_vars->carried_prop);
            }
        kill:
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    }

    /* A statement expression: retail reuses this scope's stack for the head-tracking block below. */
    ({
        float mtx[16];
        Vec4  prop_world_offset;
        if (skids_agent_vars->carried_prop != 0) {
            moby_point_position(moby, 2, &skids_agent_vars->carried_prop->pos);
            build_moby_bone_transform(moby, 2, mtx);
            copy_matrix3x4(&skids_agent_vars->carried_prop->rot_row0, mtx);
            transform_vector_by_basis(
                &prop_world_offset, skids_agent_prop_local_offset, &skids_agent_vars->carried_prop->rot_row0);
            fast_vec_add(&skids_agent_vars->carried_prop->pos,
                         &skids_agent_vars->carried_prop->pos,
                         &prop_world_offset);
            update_entity(skids_agent_vars->carried_prop);
        }
    });

    {
        Vec4 target;
        Vec4 head;
        Vec4 head_to_target_delta;
        s32  look_enabled;
        f32  rotation_step;
        f32  rotation_limit;
        f32  heading;
        f32  yaw;
        f32  pitch;
        f32  delta;

        rotation_step = 0.02f;
        rotation_limit = 0.3f;
        look_enabled = 0;
        if (moby->prev_seq == 0) {
            look_enabled = 1;
            if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 8.0f &&
                fast_difference_between_rotations(
                    moby->rot.z,
                    fast_arc_tan(hero.motion.body_center.f[0] - moby->pos.x,
                                 hero.motion.body_center.f[1] - moby->pos.y)) < 1.5707964f) {
                if (fast_vec_length(&hero.motion.displacement) > 0.01f) {
                    skids_agent_vars->timer_174 = scale_game_frames(120);
                } else {
                    fast_dec_timer_s16(&skids_agent_vars->timer_174);
                }
            } else if (skids_agent_vars->timer_174 != 0) {
                skids_agent_vars->timer_174 = 0;
                qcopy(&skids_agent_vars->idle_look_target_position, &hero.motion.body_center);
            }
            if (fast_dec_timer_s16(&skids_agent_vars->timer_176)) {
                skids_agent_vars->timer_176 =
                    truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
                heading = fast_add_rotations(moby->rot.z,
                                             random_float_between(-90.0f, 90.0f) * 0.017453292f);
                pitch = random_float_between(0.0f, 30.0f) * 0.017453292f;
                build_spherical_offset(
                    &skids_agent_vars->idle_look_target_position, 6.0f, heading, pitch);
                fast_vec_add(&skids_agent_vars->idle_look_target_position,
                             &skids_agent_vars->idle_look_target_position,
                             &moby->pos);
            }
            if (skids_agent_vars->timer_174 != 0) {
                qcopy(&target, &hero.motion.body_center);
                rotation_step = 0.04f;
                rotation_limit = 0.3f;
            } else {
                qcopy(&target, &skids_agent_vars->idle_look_target_position);
            }
        }
        if (look_enabled) {
            qcopy(&head, &moby->pos);
            head.f[2] += 1.0f;
            fast_vec_sub(&head_to_target_delta, &target, &head);
            yaw = fast_subtract_rotations(
                fast_arc_tan(head_to_target_delta.f[0], head_to_target_delta.f[1]), moby->rot.z);
            pitch =
                -fast_arc_tan(vector_length_xy(&head_to_target_delta), head_to_target_delta.f[2]);
            if (yaw > 1.5707964f)
                yaw = 1.5707964f;
            else if (yaw < -1.5707964f)
                yaw = -1.5707964f;
            if (pitch > 0.5235988f)
                pitch = 0.5235988f;
            else if (pitch < -0.5235988f)
                pitch = -0.5235988f;
            skids_agent_vars->joint_0_target_pitch = pitch;
            skids_agent_vars->joint_0_target_yaw = yaw * 0.7f;
            skids_agent_vars->joint_1_target_yaw = yaw * 0.3f;
        }
        if (big_head_cheat_enabled != 0) {
            skids_agent_vars->joint_0_scale = 2.75f;
        }
        delta = frame_scale_sq;
        update_joint_animation(rotation_step * delta,
                               rotation_limit * delta,
                               moby,
                               &skids_agent_vars->joint_0_animation,
                               0);
        update_joint_animation(rotation_step * frame_scale_sq,
                               rotation_limit * frame_scale_sq,
                               moby,
                               &skids_agent_vars->joint_1_animation,
                               1);
    }
}
