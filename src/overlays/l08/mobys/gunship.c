/* Level 08 moby class 0x1bc: gunship. */
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
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/overlay/l08/mobys/rocket.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E3010), where it is exact; names translated to the US level program. */
extern char D_0013E533[];
void       *spawn_joint_aligned_moby_part(struct Moby *parent,
                                          int          oClass,
                                          int          joint,
                                          int          pjoint) __asm__("FUN_L08_002e1c98");

/* Re-aims the twelve child mobys of a parent, adjusts the parent's spin, then runs the children's follow-up. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9F78), where it is exact; names translated to the US level program. */
extern float D_0015ED6C;

#include "rnc/hero/hero.h"

extern u8 D_0014C050[];

/* Builds the target board: spawns the frame, the three rows of eight targets
 * and their mounts, and records each piece with the piece it hangs from. */
typedef struct {
    struct Moby *moby;
    struct Moby *anchor;
    s32          joint;  /* joint on this piece */
    s32          pjoint; /* joint on the anchor it hangs from */
} TargetSlot;

typedef struct {
    u8         pad0[0x60];
    TargetSlot slots[34];
    u8         pad280[0xC];
    s16        points;
} TargetBoardVars;

void  spawn_gunship_parts(struct Moby *moby) __asm__("FUN_L08_002e1d70");
float gunship_part_spin_speed_degrees_gp __asm__("D_L08_00161C3C") = 3e+01f;

typedef struct {
    struct Moby *part; /* moved along with the anchor's joint */
    struct Moby *anchor;
    int          part_joint;
    int          anchor_joint;
} PartLink;

typedef struct {
    u8       pad0[0x60];
    PartLink links[34]; /* 0x60: entry 0 is unused */
} LinkedPartsVars;

/* Keeps the 33 linked parts glued to their anchors' joints and facing the moby, spins three of them (the middle
   one backwards), turns link 3 towards the hero, then updates every part. */
void update_linked_moby_parts(struct Moby *moby) __asm__("FUN_L08_002e2078");

/* Selects a damaged child moby, launches it, and advances the parent's state. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E35C8), where it is exact; names translated to the US level program. */
extern void *FUN_L00_0025a420(void *, int, int);
void         process_gunship_part_hits(struct Moby *moby) __asm__("FUN_L08_002e2250");

#include "rnc/overlay/swarm_path.h"

/* Gun turret on a gunship: cruises one path firing at its target, or circles a second path firing at random. */
typedef struct {
    SwarmPathFollower follow[2];    /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    PartLink          links[34];    /* 0x60: entry 0 is unused */
    s32               target_index; /* 0x280: moby slot of the target */
    f32               turn_speed;   /* 0x284 */
    s32               fire_timer;   /* 0x288 */
    s16               pad28C;
    s16               sound; /* 0x28E */
} GunshipVars;

s32                 gunship_updates_disabled_gp __asm__("D_L08_00161C4C") = 0;
f32                 gunship_path_speed_gp __asm__("D_L08_00161C38") = 5.0f;
f32                 gunship_turn_acceleration_degrees_gp __asm__("D_L08_00161C44") = 2e+01f;
f32                 gunship_max_turn_speed_degrees_gp __asm__("D_L08_00161C48") = 9e+01f;
extern struct Moby *D_L08_0015FFD8_m __asm__("D_L08_0015FFD8");
extern f32          scale_time(f32) __asm__("FUN_001f96b0");
extern void         FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern void         FUN_L08_002e2250_u(struct Moby *) __asm__("FUN_L08_002e2250");
void                gunship_update(struct Moby *moby) __asm__("FUN_L08_002e24e8");

void *spawn_joint_aligned_moby_part(struct Moby *parent, int oClass, int joint, int pjoint) {
    unsigned char *child_moby = create_moby(oClass);
    float          child_joint_position[4], parent_joint_position[4];
    if (child_moby != 0) {
        child_moby[0x30] = 0xFF;
        MOBY(child_moby)->draw_dist = 0xFF;
        child_moby[0x31] = 1;
        MOBY(child_moby)->flags |= 0x4000;
        MOBY(child_moby)->spawn_frame = *(long *)(*(char **)(D_0013E533 + 0x2E9D) + 0x38);
        qcopy(child_moby + 0x40, &parent->rot);
        moby_point_position(child_moby, joint, child_joint_position);
        moby_point_position(parent, pjoint, parent_joint_position);
        fast_vec_sub(child_moby + 0x10, parent_joint_position, child_joint_position);
        update_entity(child_moby);
    }
    return child_moby;
}

void spawn_gunship_parts(struct Moby *moby) {
    TargetBoardVars *gunship_part_link_vars = (TargetBoardVars *)moby->pvars;
    struct Moby     *child_moby;
    s32              i;
    s32              pjoint;

    gunship_part_link_vars->slots[0].moby = moby;
    gunship_part_link_vars->slots[0].anchor = 0;
    gunship_part_link_vars->slots[0].joint = 0;
    gunship_part_link_vars->slots[0].pjoint = 0;
    gunship_part_link_vars->slots[3].moby =
        spawn_joint_aligned_moby_part((char *)moby, 0x1C1, 0, 2);
    gunship_part_link_vars->slots[3].anchor = moby;
    gunship_part_link_vars->slots[3].joint = 0;
    gunship_part_link_vars->slots[3].pjoint = 2;
    pjoint = 3;
    gunship_part_link_vars->slots[1].moby =
        spawn_joint_aligned_moby_part((char *)moby, 0x1C2, 0, pjoint);
    gunship_part_link_vars->slots[1].anchor = moby;
    gunship_part_link_vars->slots[1].joint = 0;
    gunship_part_link_vars->slots[1].pjoint = pjoint;
    child_moby = spawn_joint_aligned_moby_part((char *)moby, 0x1C2, 0, 4);
    gunship_part_link_vars->slots[2].moby = child_moby;
    child_moby->flags |= 0x8000;
    gunship_part_link_vars->slots[2].anchor = moby;
    gunship_part_link_vars->slots[2].joint = 0;
    gunship_part_link_vars->slots[2].pjoint = 4;
    gunship_part_link_vars->slots[7].moby =
        spawn_joint_aligned_moby_part((char *)moby, 0x1C0, 0, 0);
    gunship_part_link_vars->slots[7].anchor = moby;
    gunship_part_link_vars->slots[7].joint = 0;
    gunship_part_link_vars->slots[7].pjoint = 0;
    gunship_part_link_vars->slots[8].moby =
        spawn_joint_aligned_moby_part((char *)moby, 0x1C0, 1, 1);
    gunship_part_link_vars->slots[8].anchor = moby;
    gunship_part_link_vars->slots[8].joint = 1;
    gunship_part_link_vars->slots[8].pjoint = 1;
    gunship_part_link_vars->slots[9].moby =
        spawn_joint_aligned_moby_part((char *)gunship_part_link_vars->slots[8].moby, 0x1C0, 1, 0);
    gunship_part_link_vars->slots[9].anchor = gunship_part_link_vars->slots[8].moby;
    gunship_part_link_vars->slots[9].joint = 1;
    gunship_part_link_vars->slots[9].pjoint = 0;
    gunship_part_link_vars->slots[5].moby =
        spawn_joint_aligned_moby_part((char *)gunship_part_link_vars->slots[9].moby, 0x1BD, 0, 0);
    gunship_part_link_vars->slots[5].anchor = gunship_part_link_vars->slots[9].moby;
    gunship_part_link_vars->slots[5].joint = 0;
    gunship_part_link_vars->slots[5].pjoint = 0;
    gunship_part_link_vars->slots[6].moby =
        spawn_joint_aligned_moby_part((char *)gunship_part_link_vars->slots[7].moby, 0x1BA, 1, 1);
    gunship_part_link_vars->slots[6].anchor = gunship_part_link_vars->slots[7].moby;
    gunship_part_link_vars->slots[6].joint = 1;
    gunship_part_link_vars->slots[6].pjoint = 1;
    gunship_part_link_vars->slots[4].moby =
        spawn_joint_aligned_moby_part((char *)moby, 0x1B9, 0, 0);
    gunship_part_link_vars->slots[4].anchor = gunship_part_link_vars->slots[6].moby;
    gunship_part_link_vars->slots[4].joint = 0;
    gunship_part_link_vars->slots[4].pjoint = 0;
    for (i = 0; i < 8; i++) {
        gunship_part_link_vars->slots[10 + i].moby = spawn_joint_aligned_moby_part(
            (char *)gunship_part_link_vars->slots[7].moby, 0x1BB, 0, i + 2);
        gunship_part_link_vars->slots[10 + i].anchor = gunship_part_link_vars->slots[7].moby;
        gunship_part_link_vars->slots[10 + i].joint = 0;
        gunship_part_link_vars->slots[10 + i].pjoint = i + 2;
    }
    for (i = 0; i < 8; i++) {
        gunship_part_link_vars->slots[18 + i].moby = spawn_joint_aligned_moby_part(
            (char *)gunship_part_link_vars->slots[8].moby, 0x1BB, 0, i + 2);
        gunship_part_link_vars->slots[18 + i].anchor = gunship_part_link_vars->slots[8].moby;
        gunship_part_link_vars->slots[18 + i].joint = 0;
        gunship_part_link_vars->slots[18 + i].pjoint = i + 2;
    }
    for (i = 0; i < 8; i++) {
        gunship_part_link_vars->slots[26 + i].moby = spawn_joint_aligned_moby_part(
            (char *)gunship_part_link_vars->slots[9].moby, 0x1BB, 0, i + 2);
        gunship_part_link_vars->slots[26 + i].anchor = gunship_part_link_vars->slots[9].moby;
        gunship_part_link_vars->slots[26 + i].joint = 0;
        gunship_part_link_vars->slots[26 + i].pjoint = i + 2;
    }
}

void update_linked_moby_parts(struct Moby *moby) {
    LinkedPartsVars *linked_parts_vars = (LinkedPartsVars *)moby->pvars;
    PartLink        *links = linked_parts_vars->links;
    float            part_joint_position[4];
    float            anchor_joint_position[4];
    float            joint_alignment_offset[4];
    int              i;

    for (i = 1; i < 34; i++) {
        struct Moby *part = linked_parts_vars->links[i].part;
        if (part != 0) {
            part->rot.z = moby->rot.z;
            moby_point_position(part, linked_parts_vars->links[i].part_joint, part_joint_position);
            moby_point_position(linked_parts_vars->links[i].anchor,
                                linked_parts_vars->links[i].anchor_joint,
                                anchor_joint_position);
            fast_vec_sub(joint_alignment_offset, anchor_joint_position, part_joint_position);
            fast_vec_add(&part->pos, &part->pos, joint_alignment_offset);
        }
    }
    if (linked_parts_vars->links[7].part != 0) {
        linked_parts_vars->links[7].part->rot.x =
            fast_add_rotations(linked_parts_vars->links[7].part->rot.x,
                               gunship_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    if (linked_parts_vars->links[8].part != 0) {
        linked_parts_vars->links[8].part->rot.x =
            fast_add_rotations(linked_parts_vars->links[8].part->rot.x,
                               -(gunship_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C));
    }
    if (linked_parts_vars->links[9].part != 0) {
        linked_parts_vars->links[9].part->rot.x =
            fast_add_rotations(linked_parts_vars->links[9].part->rot.x,
                               gunship_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    if (linked_parts_vars->links[3].part != 0) {
        linked_parts_vars->links[3].part->rot.z =
            fast_arc_tan(hero.motion.pos.f[0] - linked_parts_vars->links[3].part->pos.x,
                         hero.motion.pos.f[1] - linked_parts_vars->links[3].part->pos.y);
    }
    for (i = 1; i < 34; i++) {
        if (links[i].part != 0) {
            update_entity(links[i].part);
        }
    }
}

void process_gunship_part_hits(struct Moby *moby) {
    char *gunship_instance_vars = *(char **)((char *)moby + 0x78);
    char *entries = gunship_instance_vars + 0x60;
    char *part_entry_cursor = entries;
    int   i;
    int   score = 0;
    int   state = -1;
    int   chosen = 0;
    char *gunship_part_table;
    for (i = 0; i < 34; i++, part_entry_cursor += 16) {
        char *part_moby = *(char **)part_entry_cursor;
        if (part_moby != 0 && FUN_L00_0025a420(part_moby, 0x10000, 0) != 0) {
            if (i < 7) {
                if (score <= 0) {
                    score = 1;
                    state = 1;
                }
            } else if (i < 10) {
                if (score < 2) {
                    score = 2;
                    state = 1;
                }
            } else {
                if (score < 4) {
                    score += 4;
                    chosen = i;
                    state = 2;
                }
            }
            (*(unsigned char **)part_entry_cursor)[0xA4] = 0xFF;
        }
    }
    *(short *)(gunship_instance_vars + 0x28C) += score;
    if (*(short *)(gunship_instance_vars + 0x28C) >= 8) {
        state = 0;
        *(short *)(gunship_instance_vars + 0x28C) = *(short *)(gunship_instance_vars + 0x28C) % 8;
        if (chosen != 0) {
            int   off = chosen * 16;
            char *entry;
            char *v;
            gunship_part_table = entries;
            entry = gunship_part_table + off;
            v = *(char **)(*(char **)entry + 0x78);
            cart_burst(moby, *(char **)entry + 0x10, -1, 2.0f, 13.0f);
            fast_vec_sub(
                v, *(char **)entry + 0x10, *(char **)(gunship_instance_vars + off + 0x64) + 0x10);
            fast_vec_normalize(v, v, D_0015ED6C * 10.0f);
            (*(char **)entry)[0x20] = 1;
            *(char **)entry = 0;
            return;
        } else {
            int start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            for (i = 0; i < 24; i++) {
                int   off = ((i + start) % 24 + 10) * 16;
                char *entry;
                char *v;
                gunship_part_table = entries;
                entry = gunship_part_table + off;
                if (*(char **)entry != 0) {
                    v = *(char **)(*(char **)entry + 0x78);
                    cart_burst(moby, *(char **)entry + 0x10, -1, 2.0f, 13.0f);
                    fast_vec_sub(v,
                                 *(char **)entry + 0x10,
                                 *(char **)(gunship_instance_vars + off + 0x64) + 0x10);
                    fast_vec_normalize(v, v, D_0015ED6C * 10.0f);
                    (*(char **)entry)[0x20] = 1;
                    *(char **)entry = 0;
                    return;
                }
            }
            ((char *)moby)[0x20] = 99;
        }
    }
    if (state >= 0)
        allocate_voice_for_target_entry(state, 0, moby);
}

/* Moby class 444 */
void gunship_update(struct Moby *moby) {
    enum {
        GUNSHIP_INITIALIZE = 0,
        GUNSHIP_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET = 1,
        GUNSHIP_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY = 2,
        GUNSHIP_BEGIN_PRIMARY_PATH = 3,
        GUNSHIP_BREAK_APART_AND_REMOVE = 0x63
    };
    GunshipVars       *gunship_control_vars = (GunshipVars *)moby->pvars;
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
    s32                secondary_path_waypoint_index;
    s32                after;
    s32                i;
    s32                slot;
    f32                distance;
    f32                heading;
    f32                next_heading;
    f32                turn;
    f32                pitch;
    f32                yaw;

    if (gunship_updates_disabled_gp != 0) {
        return;
    }
    if (!voice_is_playing(moby, gunship_control_vars->sound)) {
        gunship_control_vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case GUNSHIP_INITIALIZE:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        gunship_control_vars->follow[0].path = l08_paths[gunship_control_vars->follow[0].path_id];
        gunship_control_vars->follow[1].path = l08_paths[gunship_control_vars->follow[1].path_id];
        gunship_control_vars->fire_timer =
            truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        qcopy(&moby->pos, &gunship_control_vars->follow[1].path->points[0]);
        spawn_gunship_parts(moby);
        moby->state = 2;
        moby->rot.z =
            fast_arc_tan(gunship_control_vars->follow[1].path->points[1].f[0] - moby->pos.x,
                         gunship_control_vars->follow[1].path->points[1].f[1] - moby->pos.y);
        moby->flags |= 0x4000;
        break;
    case GUNSHIP_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET:
        waypoint =
            (gunship_control_vars->follow[0].point + gunship_control_vars->follow[0].path->count +
             gunship_control_vars->follow[0].direction) %
            gunship_control_vars->follow[0].path->count;
        target = &D_L08_0015FFD8_m[gunship_control_vars->target_index];
        FUN_L00_00258278(
            moby,
            fast_arc_tan(gunship_control_vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                         gunship_control_vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
            &gunship_control_vars->turn_speed,
            0.004f,
            0.3f,
            0.02f);
        FUN_L08_002e2250_u(moby);
        update_linked_moby_parts(moby);
        qcopy(&point, &gunship_control_vars->follow[0].path->points[waypoint]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            gunship_control_vars->follow[0].point = waypoint;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > gunship_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, gunship_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        if (random_integer_below(0xFF) & 1) {
            moby_point_position(gunship_control_vars->links[3].part, 1, &muzzle);
        } else {
            moby_point_position(gunship_control_vars->links[4].part, 1, &muzzle);
        }
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
            &velocity, &step, (gunship_path_speed_gp + gunship_path_speed_gp) * frame_time);
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
    case GUNSHIP_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY:
        follower = &gunship_control_vars->follow[1];
        secondary_path_waypoint_index = (follower->point + 1) % follower->path->count;
        after = (follower->point + 2) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[secondary_path_waypoint_index].f[0] -
                                   follower->path->points[follower->point].f[0],
                               follower->path->points[secondary_path_waypoint_index].f[1] -
                                   follower->path->points[follower->point].f[1]);
        next_heading = fast_arc_tan(follower->path->points[after].f[0] -
                                        follower->path->points[secondary_path_waypoint_index].f[0],
                                    follower->path->points[after].f[1] -
                                        follower->path->points[secondary_path_waypoint_index].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance =
            fast_vec_dist(&moby->pos, &follower->path->points[secondary_path_waypoint_index]);
        distance /= fast_vec_dist(&follower->path->points[follower->point],
                                  &follower->path->points[secondary_path_waypoint_index]);
        /* the blended heading is computed but unused: the turn below steers towards the next point */
        fast_add_rotations(turn * distance, next_heading);
        heading =
            fast_arc_tan(follower->path->points[secondary_path_waypoint_index].f[0] - moby->pos.x,
                         follower->path->points[secondary_path_waypoint_index].f[1] - moby->pos.y);
        approach_angle(&moby->rot.z,
                       &gunship_control_vars->turn_speed,
                       heading,
                       gunship_turn_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq,
                       gunship_turn_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq,
                       gunship_max_turn_speed_degrees_gp * DEG_TO_RAD * frame_time);
        qcopy(&point, &follower->path->points[secondary_path_waypoint_index]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = secondary_path_waypoint_index;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > gunship_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, gunship_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        update_linked_moby_parts(moby);
        if (!fast_dec_timer(&gunship_control_vars->fire_timer)) {
            break;
        }
        gunship_control_vars->fire_timer =
            truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby_point_position(gunship_control_vars->links[4].part, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        fast_vec_normalize(
            &velocity, &step, (gunship_path_speed_gp + gunship_path_speed_gp) * frame_time);
        bolt = spawn_rocket(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = gunship_control_vars->links[4].part->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case GUNSHIP_BEGIN_PRIMARY_PATH: {
        LevelPath *path;

        moby->state = 1;
        path = gunship_control_vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z =
            fast_arc_tan(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        moby->rot.x = 0.0f;
        update_linked_moby_parts(moby);
        break;
    }
    case GUNSHIP_BREAK_APART_AND_REMOVE:
        for (i = 1; i < 34; i++) {
            struct Moby *part = gunship_control_vars->links[i].part;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                cart_burst(moby, &part->pos, -1, 3.0f, 13.0f);
                fast_vec_sub(drift, &part->pos, &moby->pos);
                fast_vec_normalize(drift, drift, frame_time * 5.0f);
                part->state = 1;
                gunship_control_vars->links[i].part = NULL;
            }
        }
        cart_burst(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}
