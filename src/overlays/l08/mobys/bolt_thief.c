/* Level 08 moby class 0x1c4: bolt thief. */
#include "types.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/moby_spawn.h"

/* finds the first of eight linked entries whose direction lines up with the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E15A0), where it is exact; names translated to the US level program. */
extern char D_L08_001675C0[];

#include "qcopy.h"

/* Re-aims the twelve child mobys of a parent, adjusts the parent's spin, then runs the children's follow-up. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9F78), where it is exact; names translated to the US level program. */
extern float D_0015ED6C;

#include "rnc/hero/hero.h"

extern char *D_L08_0015FFD8;

/* Selects a damaged child moby, launches it, and advances the parent's state. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E35C8), where it is exact; names translated to the US level program. */
extern void *FUN_L00_0025a420(void *, int, int);

#include "rnc/overlay/swarm_path.h"

extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);

/* updates matching objects in the selected object range */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E4118), where it is exact; names translated to the US level program. */
void        build_bolt_thief_target_list(struct Moby *moby) __asm__("FUN_L08_002e2da0");
float       bolt_thief_waypoint_speed_gp __asm__("D_L08_00161C50") = 1e+01f;
extern char D_001413D0[];
extern void FUN_L00_00263ac8(float, struct Moby *, int, void *);
extern void FUN_L00_0025a120(struct Moby *);
extern void
trooper_start_knockback(struct Moby *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void trooper_anim_finish(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025f730(void *, float);
extern int  find_listed_moby_by_data_id(struct Moby *) __asm__("FUN_L00_0025f878");
extern void
begin_bolt_attraction(struct Moby *, struct Moby *, float, float) __asm__("FUN_L00_002a7210");
extern void FUN_00212f90(struct Moby *, int, int, int);

typedef struct {
    char              pad0[0x29];
    unsigned char     b29;
    char              pad2A[0x16];
    Vec4              v40;
    char              pad50[0x10];
    char              x60[7];
    unsigned char     b67;
    char              pad68[8];
    char              x70[0x10];
    float             f80, f84, f88, f8C;
    int               w90, w94;
    char              pad98[0x15];
    unsigned char     bAD;
    char              padAE[0x22];
    SwarmPathFollower follow[2];
    char              pad130[0x28];
    struct Moby      *list[0x32];
    int               w220;
    float             f224;
    char              pad228[4];
    int               w22C;
    char              pad230[0x30];
    unsigned char     x260[1];
} BoltThiefVars;

void bolt_thief_update(struct Moby *moby) __asm__("FUN_L08_002e2df0");

void build_bolt_thief_target_list(struct Moby *moby) {
    char  *bolt_thief_vars = (char *)moby->pvars;
    int    oclass = 0x279;
    char  *moby_array_base = D_L08_0015FFD8;
    int   *ids = (int *)(bolt_thief_vars + 0x130);
    char **output = (char **)(bolt_thief_vars + 0x158);
    int    i = 9;
    for (;;) {
        char *candidate_target_moby = (char *)((*ids << 8) + (unsigned int)moby_array_base);
        if (*(short *)(candidate_target_moby + 0xA6) == oclass) {
            *output++ = candidate_target_moby;
        }
        --i;
        if (i < 0)
            return;
        ++ids;
    }
}

/* Moby class 452 */
void bolt_thief_update(struct Moby *moby) {
    enum {
        BOLT_THIEF_INITIALIZE = 0,
        BOLT_THIEF_WAIT_FOR_HERO_NEAR_TRIGGER = 1,
        BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_COLLECTION_POINT = 2,
        BOLT_THIEF_WAIT_FOR_ANIMATION_BEFORE_PULLING_PICKUPS = 3,
        BOLT_THIEF_PULL_PICKUPS_TO_JOINT = 4,
        BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_EXIT = 5,
        BOLT_THIEF_FOLLOW_SECONDARY_PATH = 6,
        BOLT_THIEF_FACE_HERO = 7,
        BOLT_THIEF_KNOCKBACK = 8
    };
    Vec4           point;
    Vec4           step;
    BoltThiefVars *bolt_thief_instance_vars;

    bolt_thief_instance_vars = moby->pvars;
    FUN_L00_00263ac8(2.5f, moby, 1, bolt_thief_instance_vars->x260);
    if (moby->visible != 0) {
        if (fast_vec_dist(&moby->pos, D_L08_001675C0) < 29.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x17;
        }
    }
    if (FUN_L00_0025a420(moby, 0x210000, 0) != 0 && moby->state != 8) {
        int i;

        for (i = 0; i < 0x32; i++) {
            struct Moby *other = bolt_thief_instance_vars->list[i];

            if (other != 0 && other->state == 3) {
                other->state = 1;
            }
        }
        {
            float speed8 = D_0015ED6C * 8.0f;
            float speed10 = D_0015ED6C * 10.0f;
            bolt_thief_instance_vars->f80 = 0.008f;
            bolt_thief_instance_vars->f84 = 0.0005f;
            bolt_thief_instance_vars->w94 = 1;
            bolt_thief_instance_vars->f88 = speed8;
            bolt_thief_instance_vars->f8C = speed10;
            bolt_thief_instance_vars->bAD = 0;
        }
        trooper_start_knockback(
            moby,
            bolt_thief_instance_vars->x70,
            fast_arc_tan(moby->pos.x - hero.motion.pos.f[0], moby->pos.y - hero.motion.pos.f[1]),
            4,
            1,
            0);
        bolt_thief_instance_vars->b67 = 0x78;
        FUN_L00_00257470(moby, 0, -1);
        moby->state = 8;
    }
    moby->hit_slot = 0xFF;
    switch (moby->state) {
    case BOLT_THIEF_INITIALIZE:
        bolt_thief_instance_vars->follow[0].path =
            l08_paths[bolt_thief_instance_vars->follow[0].path_id];
        qcopy(&moby->pos, &bolt_thief_instance_vars->follow[0].path->points[0]);
        bolt_thief_instance_vars->follow[0].point = 1;
        if (bolt_thief_instance_vars->follow[1].path_id >= 0) {
            bolt_thief_instance_vars->follow[1].path =
                l08_paths[bolt_thief_instance_vars->follow[1].path_id];
            bolt_thief_instance_vars->follow[1].point = 1;
        }
        moby->flags |= 0x1000;
        bolt_thief_instance_vars->b29 = 0;
        moby->spawn_frame = *(u64 *)(*(char **)D_001413D0 + 0x38);
        clear_u64_value(&bolt_thief_instance_vars->v40);
        moby->state = 1;
        if (moby->prev_seq != 1) {
            FUN_00212f90(moby, 1, 0, 0);
        }
        break;
    case BOLT_THIEF_WAIT_FOR_HERO_NEAR_TRIGGER:
        qcopy(&point, &l08_paths[bolt_thief_instance_vars->w220]->points[0]);
        if (fast_vec_dist(hero.motion.pos.f, &point) < 24.0f) {
            build_bolt_thief_target_list((char *)moby);
            moby->state = 5;
            if (moby->prev_seq != 3) {
                FUN_00212f90(moby, 3, 0, 6);
            }
        }
        break;
    case BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_COLLECTION_POINT: {
        SwarmPathFollower *follower;
        LevelPath         *path;
        int                next;
        float              heading;

        follower = &bolt_thief_instance_vars->follow[0];
        next =
            (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &bolt_thief_instance_vars->f224, 0.01f, 0.3f, 0.1f);
        path = follower->path;
        qcopy(&point, &path->points[next]);
        if (path->points[next].f[3] == 0.0f) {
            moby->state = 3;
            if (moby->prev_seq != 2) {
                FUN_00212f90(moby, 2, 0, 6);
            }
        } else {
            fast_vec_sub(&step, &point, &moby->pos);
            fast_vec_normalize(&step, &step, bolt_thief_waypoint_speed_gp * D_0015ED6C);
            fast_vec_add(&moby->pos, &moby->pos, &step);
            if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
                follower->point = next;
            }
        }
        break;
    }
    case BOLT_THIEF_WAIT_FOR_ANIMATION_BEFORE_PULLING_PICKUPS:
        moby->anim_speed = 3.0f;
        if (moby->anim_flags & 2) {
            int i;

            build_bolt_thief_target_list((char *)moby);
            for (i = 0; i < 0x32; i++) {
                struct Moby *other = bolt_thief_instance_vars->list[i];

                if (other != 0 && (signed char)other->state >= 0) {
                    other->state = 3;
                }
            }
            moby->state = 4;
            if (moby->prev_seq != 6) {
                FUN_00212f90(moby, 6, 0, 6);
            }
            moby->anim_speed = 1.0f;
        }
        break;
    case BOLT_THIEF_PULL_PICKUPS_TO_JOINT: {
        int active_count;
        int i;

        active_count = 0;
        for (i = 0; i < 0x32; i++) {
            struct Moby *other = bolt_thief_instance_vars->list[i];

            if (other != 0 && (signed char)other->state >= 0) {
                moby_point_position(moby, 0, &step);
                active_count++;
                fast_vec_sub(&point, &step, &other->pos);
                FUN_L00_0025f730(&point, D_0015ED6C * 4.0f);
                fast_vec_add(&other->pos, &other->pos, &point);
                if (fast_vec_length(&point) < 2.0f * D_0015ED6C) {
                    other->state = 4;
                    bolt_thief_instance_vars->list[i] = 0;
                    bolt_thief_instance_vars->w22C++;
                }
            }
        }
        if (active_count == 0) {
            moby->state = 5;
            if (moby->prev_seq != 3) {
                FUN_00212f90(moby, 3, 0, 6);
            }
        }
        break;
    }
    case BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_EXIT: {
        SwarmPathFollower *follower;
        int                next;
        int                found;
        int                i;
        float              heading;

        follower = &bolt_thief_instance_vars->follow[0];
        found = 0;
        next =
            (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &bolt_thief_instance_vars->f224, 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[next]);
        for (i = 0; i < 0x32; i++) {
            struct Moby *other = bolt_thief_instance_vars->list[i];

            if (other != 0 && (signed char)other->state >= 0) {
                if (vector_distance_xy(&moby->pos, &other->pos) < 6.0f) {
                    if (moby->prev_seq != 6) {
                        FUN_00212f90(moby, 6, 0, scale_game_frames(0x14));
                    }
                    begin_bolt_attraction(bolt_thief_instance_vars->list[i], moby, 0.0f, 0.0f);
                    bolt_thief_instance_vars->list[i] = 0;
                }
                found = 1;
            }
        }
        if (found == 0 && moby->prev_seq != 3) {
            FUN_00212f90(moby, 3, 0, scale_game_frames(0x14));
        }
        if (follower->path->count - 1 == next) {
            if (bolt_thief_instance_vars->follow[1].path_id >= 0) {
                moby->state = 6;
                break;
            }
            mark_moby_for_removal(moby);
            return;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        fast_vec_normalize(&step, &step, bolt_thief_waypoint_speed_gp * D_0015ED6C);
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        break;
    }
    case BOLT_THIEF_FOLLOW_SECONDARY_PATH: {
        SwarmPathFollower *follower;
        int                next;
        float              heading;

        follower = &bolt_thief_instance_vars->follow[1];
        next =
            (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &bolt_thief_instance_vars->f224, 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[next]);
        fast_vec_sub(&step, &point, &moby->pos);
        fast_vec_normalize(&step, &step, bolt_thief_waypoint_speed_gp * D_0015ED6C);
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        if (fast_vec_dist(hero.motion.pos.f, &moby->pos) > 19.0f) {
            moby->state = 7;
        }
        break;
    }
    case BOLT_THIEF_FACE_HERO: {
        float *hero_position;
        float  distance;
        float  heading;

        heading =
            fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
        hero_position = hero.motion.pos.f;
        FUN_L00_00258278(moby, heading, &bolt_thief_instance_vars->f224, 0.01f, 0.3f, 0.1f);
        distance = fast_vec_dist(hero_position, &moby->pos);
        if (distance < 17.0f || find_listed_moby_by_data_id(moby) != 0) {
            SwarmPathFollower *follower;
            int                next;

            follower = &bolt_thief_instance_vars->follow[1];
            next = (follower->point + follower->path->count + follower->direction) %
                   follower->path->count;
            moby->state = 6;
            if (fast_vec_dist(hero_position, &follower->path->points[next]) < distance) {
                follower->direction *= -1;
            }
        }
        break;
    }
    case BOLT_THIEF_KNOCKBACK:
        if (step_moby_motion(moby, bolt_thief_instance_vars->x70) & 3) {
            cart_burst(moby, &moby->pos, -1, 1.0f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, bolt_thief_instance_vars->x60);
}
