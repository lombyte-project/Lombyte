/* Level 08 moby class 0x1ce: shrub. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/overlay/l08/mobys/rocket.h"

/* spawns the child mobys this object owns and fills its part table */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9B60), where it is exact; names translated to the US level program. */
extern int FUN_L08_002e1c98_c() __asm__("FUN_L08_002e1c98");
void       spawn_shrub_parts(char *moby) __asm__("FUN_L08_002e8788");

/* Re-aims the twelve child mobys of a parent, adjusts the parent's spin, then runs the children's follow-up. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9F78), where it is exact; names translated to the US level program. */
extern float D_0015ED6C;
float        shrub_part_spin_speed_degrees_gp __asm__("D_L08_00161D28") = 3e+01f;
void         update_shrub_parts(struct Moby *parent_moby) __asm__("FUN_L08_002e8ba0");
extern u8    D_0014C050[];

#include "rnc/overlay/swarm_path.h"

extern struct Moby *D_L08_0015FFD8;
extern f32          scale_time(f32) __asm__("FUN_001f96b0");
extern void         FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9CB0), where it is exact; names translated to the US level program. */
void process_shrub_part_hits(struct Moby *parent_moby) __asm__("FUN_L08_002e88d8");

/* Flying gunship: cruises one path and fires at its target, or circles a second path and fires at random. */
typedef struct {
    SwarmPathFollower follow[2];    /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    SwarmPart         root;         /* 0x60 */
    SwarmPart         parts[12];    /* 0x70 */
    s32               target_index; /* 0x130: moby slot of the target */
    f32               turn_speed;   /* 0x134 */
    s16               can_skip;     /* 0x138: non-zero lets the circling jump ahead (see state 2) */
    s16               fire_timer;   /* 0x13A */
    s16               pad13C;
    s16               sound; /* 0x13E */
} SwarmVars;

s32                       shrub_updates_disabled_gp __asm__("D_L08_00161D30") = 0;
f32                       shrub_path_speed_gp __asm__("D_L08_00161D24") = 5.0f;
extern s32                level_game_mode __asm__("D_L08_0015F5C4");
extern s64 D_L08_0016D090 NOT_SDA;
extern void               spawn_shrub_parts(char *) __asm__("FUN_L08_002e8788");
extern void               update_shrub_parts(struct Moby *) __asm__("FUN_L08_002e8ba0");
void                      shrub_update(struct Moby *moby) __asm__("FUN_L08_002e8cd0");

void spawn_shrub_parts(char *moby) {
    char        *shrub_instance_vars = ((char *)MOBY(moby)->pvars);
    char        *anchor_joint_cursor = shrub_instance_vars + 0xBC;
    int          i = 0;
    struct Moby *child_moby;
    char        *muzzle_anchor_moby;
    char        *muzzle_part_moby;
    *(char **)(shrub_instance_vars + 0x60) = moby;
    *(int *)(shrub_instance_vars + 0x64) = 0;
    *(int *)(shrub_instance_vars + 0x68) = 0;
    *(int *)(shrub_instance_vars + 0x6C) = 0;
    *(char **)(shrub_instance_vars + 0x90) = FUN_L08_002e1c98_c(moby, 0x1C0, 1, 0);
    *(char **)(shrub_instance_vars + 0x94) = moby;
    *(int *)(shrub_instance_vars + 0x98) = 1;
    *(int *)(shrub_instance_vars + 0x9C) = 0;
    child_moby = FUN_L08_002e1c98_c(moby, 0x1BD, 0, 2);
    *(char **)(shrub_instance_vars + 0x70) = child_moby;
    *(int *)(shrub_instance_vars + 0x7C) = 2;
    *(char **)(shrub_instance_vars + 0x74) = moby;
    *(int *)(shrub_instance_vars + 0x78) = 0;
    child_moby->scale = *(float *)(((char *)child_moby->pclass) + 0x24) * 0.75f;
    muzzle_anchor_moby = FUN_L08_002e1c98_c(moby, 0x1BA, 1, 1);
    *(char **)(shrub_instance_vars + 0x80) = muzzle_anchor_moby;
    *(char **)(shrub_instance_vars + 0x84) = moby;
    *(int *)(shrub_instance_vars + 0x88) = 1;
    *(int *)(shrub_instance_vars + 0x8C) = 1;
    muzzle_part_moby = FUN_L08_002e1c98_c(muzzle_anchor_moby, 0x1B9, 0, 0);
    *(char **)(shrub_instance_vars + 0xA0) = muzzle_part_moby;
    *(char **)(shrub_instance_vars + 0xA4) = *(char **)(shrub_instance_vars + 0x80);
    *(int *)(shrub_instance_vars + 0xA8) = 0;
    *(int *)(shrub_instance_vars + 0xAC) = 0;
    do {
        int j = i + 2;
        i++;
        *(char **)(anchor_joint_cursor - 0xC) =
            FUN_L08_002e1c98_c(*(char **)(shrub_instance_vars + 0x90), 0x1BB, 0, j);
        *(char **)(anchor_joint_cursor - 8) = *(char **)(shrub_instance_vars + 0x90);
        *(int *)(anchor_joint_cursor - 4) = 0;
        *(int *)anchor_joint_cursor = j;
        anchor_joint_cursor += 0x10;
    } while (i < 8);
}

void process_shrub_part_hits(struct Moby *parent_moby) {
    char  *shrub_hit_vars = ((char *)parent_moby->pvars);
    char  *shrub_part_entry_address = shrub_hit_vars + 0x60;
    char **part_entry_cursor = (char **)shrub_part_entry_address;
    char **sq;
    int    weighted_hit_score = 0;
    int    selected_hit_part_index = 0;
    int    hit_sound_id = -1;
    int    i;
    for (i = 0; i < 13; i++, part_entry_cursor += 4) {
        if (*part_entry_cursor != 0) {
            if (find_moby_hit(*part_entry_cursor, 0x10000, 0) != 0) {
                if (i < 3) {
                    if (weighted_hit_score <= 0) {
                        weighted_hit_score = 1;
                        hit_sound_id = 1;
                    }
                } else if (i < 5) {
                    if (weighted_hit_score < 2) {
                        weighted_hit_score = 2;
                        hit_sound_id = 1;
                    }
                } else {
                    if (weighted_hit_score < 4) {
                        weighted_hit_score += 4;
                        selected_hit_part_index = i;
                        hit_sound_id = 2;
                    }
                }
                ((unsigned char *)*part_entry_cursor)[0xA4] = 0xFF;
            }
        }
    }
    *(unsigned short *)(shrub_hit_vars + 0x13C) += weighted_hit_score;
    if (*(short *)(shrub_hit_vars + 0x13C) >= 8) {
        *(short *)(shrub_hit_vars + 0x13C) = *(short *)(shrub_hit_vars + 0x13C) % 8;
        if (selected_hit_part_index != 0) {
            int    selected_part_byte_offset = selected_hit_part_index * 16;
            char **selected_part_entry =
                (char **)(shrub_part_entry_address + selected_part_byte_offset);
            char *selected_part_moby = *selected_part_entry;
            char *d2;
            d2 = ((char *)MOBY(selected_part_moby)->pvars);
            cart_burst(parent_moby, selected_part_moby + 0x10, -1, 2.0f, 13.0f);
            fast_vec_sub(d2,
                         *selected_part_entry + 0x10,
                         *(char **)(selected_part_byte_offset + shrub_hit_vars + 0x64) + 0x10);
            fast_vec_normalize(d2, d2, D_0015ED6C * 10.0f);
            (*selected_part_entry)[0x20] = 1;
            *selected_part_entry = 0;
            return;
        } else {
            int   random_search_start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            char *part_table_base = shrub_part_entry_address;
            for (i = 0; i < 8; i++) {
                int k = (i + random_search_start) % 8 + 5;
                int fallback_part_byte_offset = k * 16;
                shrub_part_entry_address = part_table_base + fallback_part_byte_offset;
                sq = (char **)shrub_part_entry_address;
                if (*sq != 0) {
                    char *d2 = *(char **)(*sq + 0x78);
                    cart_burst(parent_moby, *sq + 0x10, -1, 2.0f, 13.0f);
                    fast_vec_sub(d2,
                                 *sq + 0x10,
                                 *(char **)(shrub_hit_vars + fallback_part_byte_offset + 0x64) +
                                     0x10);
                    fast_vec_normalize(d2, d2, D_0015ED6C * 10.0f);
                    (*sq)[0x20] = 1;
                    *sq = 0;
                    return;
                }
            }
            parent_moby->state = 0x63;
        }
    }
    if (hit_sound_id >= 0)
        allocate_voice_for_target_entry(hit_sound_id, 0, (int)parent_moby);
}

void update_shrub_parts(struct Moby *parent_moby) {
    char *shrub_vars = (char *)parent_moby->pvars;
    float part_joint_position[4], anchor_joint_position[4], joint_alignment_offset[4];
    char *shrub_part_table = shrub_vars + 0x60;
    int   i;
    for (i = 0; i < 12; i++) {
        int  *anchor_joint_cursor = (int *)(shrub_part_table + 0x1C + i * 16);
        char *m = (char *)anchor_joint_cursor[-3];
        if (m != 0) {
            *(float *)(m + 0x48) = parent_moby->rot.z;
            moby_point_position(m, anchor_joint_cursor[-1], part_joint_position);
            moby_point_position(
                (void *)anchor_joint_cursor[-2], anchor_joint_cursor[0], anchor_joint_position);
            fast_vec_sub(joint_alignment_offset, anchor_joint_position, part_joint_position);
            fast_vec_add(m + 0x10, m + 0x10, joint_alignment_offset);
        }
    }
    if (*(int *)(shrub_vars + 0x90) != 0) {
        char *spinning_part_moby = *(char **)(shrub_vars + 0x90);
        *(float *)(*(char **)(shrub_vars + 0x90) + 0x40) =
            fast_add_rotations(MOBY(spinning_part_moby)->rot.x,
                               shrub_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    for (i = 0; i < 12; i++) {
        char *m = *(char **)(shrub_part_table + 0x10 + i * 16);
        if (m != 0)
            update_entity(m);
    }
}

/* Moby class 462 */
void shrub_update(struct Moby *moby) {
    enum {
        SHRUB_INITIALIZE = 0,
        SHRUB_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET = 1,
        SHRUB_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY = 2,
        SHRUB_BEGIN_PRIMARY_PATH = 3,
        SHRUB_BREAK_APART_AND_REMOVE = 0x63
    };
    SwarmVars         *shrub_control_vars = (SwarmVars *)moby->pvars;
    SwarmPathFollower *follower;
    struct Moby       *target;
    struct Moby       *shot;
    struct Moby       *bolt;
    SwarmTargetVars   *target_vars;
    Vec4               point;
    Vec4               step;
    Vec4               muzzle;
    Vec4               velocity;
    s32                waypoint;
    s32                secondary_waypoint_index;
    s32                after;
    s32                i;
    s32                slot;
    f32                distance;
    f32                heading;
    f32                next_heading;
    f32                turn;
    f32                pitch;
    f32                yaw;

    if (shrub_updates_disabled_gp != 0) {
        return;
    }
    if (!voice_is_playing(moby, shrub_control_vars->sound)) {
        shrub_control_vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case SHRUB_INITIALIZE:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        shrub_control_vars->follow[0].path = l08_paths[shrub_control_vars->follow[0].path_id];
        shrub_control_vars->follow[1].path = l08_paths[shrub_control_vars->follow[1].path_id];
        shrub_control_vars->fire_timer =
            truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby->flags |= 0x4000;
        qcopy(&moby->pos, &shrub_control_vars->follow[1].path->points[0]);
        spawn_shrub_parts((char *)moby);
        moby->state = 2;
        moby->rot.z =
            fast_arc_tan(shrub_control_vars->follow[1].path->points[1].f[0] - moby->pos.x,
                         shrub_control_vars->follow[1].path->points[1].f[1] - moby->pos.y);
        break;
    case SHRUB_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET:
        waypoint =
            (shrub_control_vars->follow[0].point + shrub_control_vars->follow[0].path->count +
             shrub_control_vars->follow[0].direction) %
            shrub_control_vars->follow[0].path->count;
        target = &D_L08_0015FFD8[shrub_control_vars->target_index];
        FUN_L00_00258278(
            moby,
            fast_arc_tan(shrub_control_vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                         shrub_control_vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
            &shrub_control_vars->turn_speed,
            0.004f,
            0.3f,
            0.02f);
        process_shrub_part_hits(moby);
        update_shrub_parts((char *)moby);
        qcopy(&point, &shrub_control_vars->follow[0].path->points[waypoint]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            shrub_control_vars->follow[0].point = waypoint;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > shrub_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, shrub_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        moby_point_position(shrub_control_vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        fast_vec_normalize(
            &velocity, &step, (shrub_path_speed_gp + shrub_path_speed_gp) * frame_time);
        shot = spawn_rocket(&muzzle, target, &velocity, pitch, yaw);
        shot->scale = shot->pclass->scale / 5.0f;
        target_vars = (SwarmTargetVars *)target->pvars;
        for (slot = 0; slot < 8; slot++) {
            if (target_vars->shots[slot] == NULL) {
                target_vars->shots[slot] = shot;
                break;
            }
        }
        break;
    case SHRUB_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY:
        follower = &shrub_control_vars->follow[1];
        if (level_game_mode == 2 && shrub_control_vars->can_skip != 0 &&
            D_L08_0016D090 == 0x200000000) {
            qcopy(&moby->pos, &follower->path->points[15]);
            follower->point = 15;
            moby->rot.z = fast_arc_tan(follower->path->points[11].f[0] - moby->pos.x,
                                       follower->path->points[11].f[1] - moby->pos.y);
        }
        secondary_waypoint_index = (follower->point + 1) % follower->path->count;
        after = (follower->point + 2) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[secondary_waypoint_index].f[0] -
                                   follower->path->points[follower->point].f[0],
                               follower->path->points[secondary_waypoint_index].f[1] -
                                   follower->path->points[follower->point].f[1]);
        next_heading = fast_arc_tan(follower->path->points[after].f[0] -
                                        follower->path->points[secondary_waypoint_index].f[0],
                                    follower->path->points[after].f[1] -
                                        follower->path->points[secondary_waypoint_index].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = fast_vec_dist(&moby->pos, &follower->path->points[secondary_waypoint_index]);
        distance /= fast_vec_dist(&follower->path->points[follower->point],
                                  &follower->path->points[secondary_waypoint_index]);
        moby->rot.z = fast_add_rotations(turn * distance, next_heading);
        qcopy(&point, &follower->path->points[secondary_waypoint_index]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = secondary_waypoint_index;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > shrub_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, shrub_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        update_shrub_parts((char *)moby);
        if (!fast_dec_timer_s16(&shrub_control_vars->fire_timer)) {
            break;
        }
        shrub_control_vars->fire_timer =
            truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby_point_position(shrub_control_vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        fast_vec_normalize(
            &velocity, &step, (shrub_path_speed_gp + shrub_path_speed_gp) * frame_time);
        bolt = spawn_rocket(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = shrub_control_vars->parts[3].moby->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case SHRUB_BEGIN_PRIMARY_PATH: {
        LevelPath *path;

        moby->state = 1;
        path = shrub_control_vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z =
            fast_arc_tan(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        update_shrub_parts((char *)moby);
        break;
    }
    case SHRUB_BREAK_APART_AND_REMOVE:
        for (i = 0; i < 12; i++) {
            struct Moby *part = shrub_control_vars->parts[i].moby;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                cart_burst(moby, &part->pos, -1, 3.0f, 13.0f);
                fast_vec_sub(drift, &part->pos, &moby->pos);
                fast_vec_normalize(drift, drift, frame_time * 5.0f);
                part->state = 1;
                shrub_control_vars->parts[i].moby = NULL;
            }
        }
        cart_burst(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}
