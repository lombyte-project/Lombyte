/* Level 08 moby class 0x1cf: heavy gunship, its part hierarchy and part hits. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/swarm_path.h"
#include "rnc/game/game_globals.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/math_consts.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/l08/mobys/rocket.h"

/* Pvars of the heavy gunship. */
typedef struct {
    SwarmPathFollower follow[2];    /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    SwarmPart         root;         /* 0x60 */
    SwarmPart         parts[16];    /* 0x70 */
    s32               target_index; /* 0x170: moby slot of the target */
    f32               turn_speed;   /* 0x174 */
    s32               fire_timer;   /* 0x178 */
    s16               pad17C;
    s16               sound; /* 0x17E */
} HeavyGunshipVars;

s32                 heavy_gunship_updates_disabled_gp __asm__("D_L08_00161D40") = 0;
f32                 heavy_gunship_path_speed_gp __asm__("D_L08_00161D34") = 5.0f;
f32                 heavy_gunship_part_spin_speed_degrees_gp __asm__("D_L08_00161D38") = 3e+01f;
extern float        D_0015ED6C;
extern struct Moby *D_L08_0015FFD8;

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
void *
FUN_L08_002e1c98_c2(char *parent, int oClass, int joint, int pjoint) __asm__("FUN_L08_002e1c98");

/* Builds the moby's part hierarchy: body, head, arms and eight trailing segments. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002EA930), where it is exact; names translated to the US level program. */
void spawn_heavy_gunship_parts(struct Moby *moby) __asm__("FUN_L08_002e9558");

void spawn_heavy_gunship_parts(struct Moby *moby) {
    char        *part_hierarchy_vars = ((char *)moby->pvars);
    char        *anchor_joint_cursor = part_hierarchy_vars + 0xFC;
    int          i = 0;
    struct Moby *child_moby;
    char        *attachment_anchor_moby;
    struct Moby *root_joint_attachment_moby;
    struct Moby *anchor_joint_attachment_moby;
    *(char **)(part_hierarchy_vars + 0x60) = moby;
    *(int *)(part_hierarchy_vars + 0x64) = 0;
    *(int *)(part_hierarchy_vars + 0x68) = 0;
    *(int *)(part_hierarchy_vars + 0x6C) = 0;
    *(char **)(part_hierarchy_vars + 0x90) = FUN_L08_002e1c98_c2(moby, 0x1C0, 0, 0);
    *(char **)(part_hierarchy_vars + 0x94) = moby;
    *(int *)(part_hierarchy_vars + 0x98) = 0;
    *(int *)(part_hierarchy_vars + 0x9C) = 0;
    child_moby = FUN_L08_002e1c98_c2(moby, 0x1BD, 0, 2);
    *(char **)(part_hierarchy_vars + 0x70) = child_moby;
    *(int *)(part_hierarchy_vars + 0x7C) = 2;
    *(char **)(part_hierarchy_vars + 0x74) = moby;
    *(int *)(part_hierarchy_vars + 0x78) = 0;
    child_moby->scale = *(float *)(((char *)child_moby->pclass) + 0x24) * 0.75f;
    attachment_anchor_moby =
        FUN_L08_002e1c98_c2(*(char **)(part_hierarchy_vars + 0x90), 0x1BA, 1, 1);
    *(char **)(part_hierarchy_vars + 0x80) = attachment_anchor_moby;
    *(char **)(part_hierarchy_vars + 0x84) = *(char **)(part_hierarchy_vars + 0x90);
    *(int *)(part_hierarchy_vars + 0x88) = 1;
    *(int *)(part_hierarchy_vars + 0x8C) = 1;
    *(char **)(part_hierarchy_vars + 0xA0) =
        FUN_L08_002e1c98_c2(attachment_anchor_moby, 0x1B9, 0, 0);
    *(char **)(part_hierarchy_vars + 0xA4) = *(char **)(part_hierarchy_vars + 0x80);
    *(int *)(part_hierarchy_vars + 0xA8) = 0;
    *(int *)(part_hierarchy_vars + 0xAC) = 0;
    *(char **)(part_hierarchy_vars + 0xB0) = FUN_L08_002e1c98_c2(moby, 0x1D0, 0, 1);
    *(char **)(part_hierarchy_vars + 0xB4) = moby;
    *(int *)(part_hierarchy_vars + 0xB8) = 0;
    *(int *)(part_hierarchy_vars + 0xBC) = 1;
    root_joint_attachment_moby = FUN_L08_002e1c98_c2(moby, 0x1D0, 0, 1);
    *(char **)(part_hierarchy_vars + 0xC0) = root_joint_attachment_moby;
    *(char **)(part_hierarchy_vars + 0xC4) = moby;
    *(int *)(part_hierarchy_vars + 0xC8) = 0;
    *(int *)(part_hierarchy_vars + 0xCC) = 1;
    root_joint_attachment_moby->flags |= 0x8000;
    *(char **)(part_hierarchy_vars + 0xD0) =
        FUN_L08_002e1c98_c2(*(char **)(part_hierarchy_vars + 0x80), 0x1D1, 0, 0);
    *(char **)(part_hierarchy_vars + 0xD4) = *(char **)(part_hierarchy_vars + 0x80);
    *(int *)(part_hierarchy_vars + 0xD8) = 0;
    *(int *)(part_hierarchy_vars + 0xDC) = 0;
    anchor_joint_attachment_moby =
        FUN_L08_002e1c98_c2(*(char **)(part_hierarchy_vars + 0x80), 0x1D1, 0, 0);
    *(char **)(part_hierarchy_vars + 0xE0) = anchor_joint_attachment_moby;
    *(char **)(part_hierarchy_vars + 0xE4) = *(char **)(part_hierarchy_vars + 0x80);
    *(int *)(part_hierarchy_vars + 0xE8) = 0;
    *(int *)(part_hierarchy_vars + 0xEC) = 0;
    anchor_joint_attachment_moby->flags |= 0x8000;
    do {
        int j = i + 2;
        i++;
        *(char **)(anchor_joint_cursor - 0xC) =
            FUN_L08_002e1c98_c2(*(char **)(part_hierarchy_vars + 0x90), 0x1BB, 0, j);
        *(char **)(anchor_joint_cursor - 8) = *(char **)(part_hierarchy_vars + 0x90);
        *(int *)(anchor_joint_cursor - 4) = 0;
        *(int *)anchor_joint_cursor = j;
        anchor_joint_cursor += 0x10;
    } while (i < 8);
}

/* Collects the hits on the parts; every twelve hit points one part breaks off (a hit
 * segment first, else a random trailing segment), and with none left the gunship breaks apart. */
void process_heavy_gunship_part_hits(struct Moby *moby) __asm__("FUN_L08_002e9758");

void process_heavy_gunship_part_hits(struct Moby *moby) {
    int        selected = 0;
    char      *linked_part_hit_vars = (char *)moby->pvars;
    SwarmPart *slots = (SwarmPart *)(linked_part_hit_vars + 0x60);
    SwarmPart *slot = slots;
    int        i = 0;
    int        hits = 0;
    int        sound = -1;
    int        total;
    u8         mark = 0xff;

    do {
        if (slot->moby != 0) {
            if (find_moby_hit(slot->moby, 0x10000, 0) != 0) {
                if (i < 3) {
                    if (hits < 1) {
                        hits = 1;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (i < 9) {
                    if (hits < 2) {
                        hits = 2;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (hits < 4) {
                    hits += 4;
                    selected = i;
                    sound = 2;
                    goto mark_hit;
                }
            mark_hit:
                slot->moby->hit_slot = mark;
            }
        }
        i++;
        slot++;
    } while (i < 17);
    total = *(u16 *)(linked_part_hit_vars + 0x17c) + hits;
    *(s16 *)(linked_part_hit_vars + 0x17c) = total;
    if ((s16)total >= 12) {
        *(s16 *)(linked_part_hit_vars + 0x17c) = (s16)total % 12;
        if (selected != 0) {
            SwarmPart *chosen = &slots[selected];
            u8        *vector;
            char      *position = linked_part_hit_vars;
            vector = chosen->moby->pvars;
            cart_burst(moby, &chosen->moby->pos, -1, 2.0f, 13.0f);
            position += selected * 16;
            fast_vec_sub(vector, &chosen->moby->pos, &(*(struct Moby **)(position + 0x64))->pos);
            fast_vec_normalize(vector, vector, D_0015ED6C * 10.0f);
            chosen->moby->state = 1;
            chosen->moby = 0;
            return;
        }
        {
            int        start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            SwarmPart *linked_part_table = slots;
            i = 0;
            do {
                int        index = (i + start) % 8 + 9;
                SwarmPart *candidate;
                slots = &linked_part_table[index];
                candidate = slots;
                if (candidate->moby != 0) {
                    u8   *vector = candidate->moby->pvars;
                    char *position = linked_part_hit_vars;
                    cart_burst(moby, &candidate->moby->pos, -1, 2.0f, 13.0f);
                    position += index * 16;
                    fast_vec_sub(
                        vector, &candidate->moby->pos, &(*(struct Moby **)(position + 0x64))->pos);
                    fast_vec_normalize(vector, vector, D_0015ED6C * 10.0f);
                    candidate->moby->state = 1;
                    candidate->moby = 0;
                    return;
                }
                i++;
            } while (i < 8);
            moby->state = 99;
        }
    }
    if (sound >= 0)
        allocate_voice_for_target_entry(sound, 0, moby);
}

/* Re-aims the sixteen parts at their anchor joints, spins the class 0x1C0 part, then updates the parts. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002EADF0), where it is exact; names translated to the US level program. */
void update_heavy_gunship_parts(struct Moby *parent_moby) __asm__("FUN_L08_002e9a18");

void update_heavy_gunship_parts(struct Moby *parent_moby) {
    char *commando_vars = (char *)parent_moby->pvars;
    float part_joint_position[4];
    float anchor_joint_position[4];
    float joint_alignment_offset[4];
    int   i;
    char *part_entry_cursor;
    char *parts = commando_vars + 0x60;
    for (i = 0; i < 16; i++) {
        char *anchor_joint_cursor = parts + 0x1C + i * 16;
        char *part = *(char **)(anchor_joint_cursor - 0xC);
        if (part != 0) {
            *(float *)(part + 0x48) = parent_moby->rot.z;
            moby_point_position(part, *(int *)(anchor_joint_cursor - 4), part_joint_position);
            moby_point_position(*(void **)(anchor_joint_cursor - 8),
                                *(int *)anchor_joint_cursor,
                                anchor_joint_position);
            fast_vec_sub(joint_alignment_offset, anchor_joint_position, part_joint_position);
            fast_vec_add(part + 0x10, part + 0x10, joint_alignment_offset);
        }
    }
    if (*(char **)(commando_vars + 0x90) != 0) {
        *(float *)(*(char **)(commando_vars + 0x90) + 0x40) =
            fast_add_rotations(*(float *)(*(char **)(commando_vars + 0x90) + 0x40),
                               heavy_gunship_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    part_entry_cursor = parts + 0x10;
    for (i = 0; i < 16; i++) {
        char *updated_part = *(char **)part_entry_cursor;
        if (updated_part != 0) {
            update_entity(updated_part);
            updated_part = *(char **)part_entry_cursor;
            if (*(unsigned short *)(updated_part + 0x34) & 0x8000) {
                fast_vec_scale(updated_part + 0xD0, updated_part + 0xD0, -1.0f);
            }
        }
        part_entry_cursor += 16;
    }
}

void heavy_gunship_update(struct Moby *moby) __asm__("FUN_L08_002e9b70");

/* Moby class 463 */
void heavy_gunship_update(struct Moby *moby) {
    enum {
        HEAVY_GUNSHIP_INITIALIZE = 0,
        HEAVY_GUNSHIP_FOLLOW_PRIMARY_PATH = 1,
        HEAVY_GUNSHIP_FOLLOW_SECONDARY_PATH = 2,
        HEAVY_GUNSHIP_START_PRIMARY_PATH = 3,
        HEAVY_GUNSHIP_DISPERSE_AND_REMOVE = 0x63
    };
    HeavyGunshipVars  *swarm_vars = (HeavyGunshipVars *)moby->pvars;
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
    s32                next_waypoint;
    s32                after;
    s32                i;
    s32                slot;
    f32                distance;
    f32                heading;
    f32                next_heading;
    f32                turn;
    f32                pitch;
    f32                yaw;

    if (heavy_gunship_updates_disabled_gp != 0) {
        return;
    }
    if (!voice_is_playing(moby, swarm_vars->sound)) {
        swarm_vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case HEAVY_GUNSHIP_INITIALIZE:
        if (visited52[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        swarm_vars->follow[0].path = l08_paths[swarm_vars->follow[0].path_id];
        swarm_vars->follow[1].path = l08_paths[swarm_vars->follow[1].path_id];
        swarm_vars->fire_timer =
            truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby->flags |= 0x4000;
        qcopy(&moby->pos, &swarm_vars->follow[1].path->points[0]);
        spawn_heavy_gunship_parts(moby);
        moby->state = 2;
        moby->rot.z = fast_arc_tan(swarm_vars->follow[1].path->points[1].f[0] - moby->pos.x,
                                   swarm_vars->follow[1].path->points[1].f[1] - moby->pos.y);
        break;
    case HEAVY_GUNSHIP_FOLLOW_PRIMARY_PATH:
        waypoint = (swarm_vars->follow[0].point + swarm_vars->follow[0].path->count +
                    swarm_vars->follow[0].direction) %
                   swarm_vars->follow[0].path->count;
        target = &D_L08_0015FFD8[swarm_vars->target_index];
        FUN_L00_00258278(
            moby,
            fast_arc_tan(swarm_vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                         swarm_vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
            &swarm_vars->turn_speed,
            0.004f,
            0.3f,
            0.02f);
        qcopy(&point, &swarm_vars->follow[0].path->points[waypoint]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            swarm_vars->follow[0].point = waypoint;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > heavy_gunship_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, heavy_gunship_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        process_heavy_gunship_part_hits(moby);
        update_heavy_gunship_parts(moby);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        moby_point_position(swarm_vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        fast_vec_normalize(&velocity,
                           &step,
                           (heavy_gunship_path_speed_gp + heavy_gunship_path_speed_gp) *
                               frame_time);
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
    case HEAVY_GUNSHIP_FOLLOW_SECONDARY_PATH:
        follower = &swarm_vars->follow[1];
        next_waypoint = (swarm_vars->follow[1].point + 1) % follower->path->count;
        after = (swarm_vars->follow[1].point + 2) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next_waypoint].f[0] -
                                   follower->path->points[swarm_vars->follow[1].point].f[0],
                               follower->path->points[next_waypoint].f[1] -
                                   follower->path->points[swarm_vars->follow[1].point].f[1]);
        next_heading = fast_arc_tan(
            follower->path->points[after].f[0] - follower->path->points[next_waypoint].f[0],
            follower->path->points[after].f[1] - follower->path->points[next_waypoint].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = fast_vec_dist(&moby->pos, &follower->path->points[next_waypoint]);
        distance /= fast_vec_dist(&follower->path->points[swarm_vars->follow[1].point],
                                  &follower->path->points[next_waypoint]);
        moby->rot.z = fast_add_rotations(turn * distance, next_heading);
        qcopy(&point, &follower->path->points[next_waypoint]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            swarm_vars->follow[1].point = next_waypoint;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > heavy_gunship_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, heavy_gunship_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        update_heavy_gunship_parts(moby);
        if (!fast_dec_timer(&swarm_vars->fire_timer)) {
            break;
        }
        swarm_vars->fire_timer =
            truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby_point_position(swarm_vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        fast_vec_normalize(&velocity,
                           &step,
                           (heavy_gunship_path_speed_gp + heavy_gunship_path_speed_gp) *
                               frame_time);
        bolt = spawn_rocket(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = swarm_vars->parts[3].moby->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case HEAVY_GUNSHIP_START_PRIMARY_PATH: {
        LevelPath *path;

        moby->state = 1;
        path = swarm_vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z =
            fast_arc_tan(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        update_heavy_gunship_parts(moby);
        break;
    }
    case HEAVY_GUNSHIP_DISPERSE_AND_REMOVE:
        for (i = 0; i < 16; i++) {
            struct Moby *part = swarm_vars->parts[i].moby;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                cart_burst(moby, &part->pos, -1, 3.0f, 13.0f);
                fast_vec_sub(drift, &part->pos, &moby->pos);
                fast_vec_normalize(drift, drift, frame_time * 5.0f);
                part->state = 1;
                swarm_vars->parts[i].moby = NULL;
            }
        }
        cart_burst(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}
