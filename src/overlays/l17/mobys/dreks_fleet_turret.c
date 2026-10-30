/* Level 17 moby class dreks_fleet_turret: waits for the hero, aims at its zone or at the hero's ship and fires; its hit handling and the spreading of its group's blarg ships along its path. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/world/collision/collision_line.h"
#include "qcopy.h"
#include "qzero.h"

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

/* Updates moby 347, a turret of Drek's fleet: waits for the hero, aims at its zone or at the hero's ship and fires. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002CC788), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x20];
    float f20;
    short s24;
    char  pad26[0x4A];
    int   zone;
    int   timer;
    float yawvel;
    int   list;
    short group;
    char  pad82[2];
    int   path;
    char  pad88[8];
    char  a90[0x44];
    float fD4;
    char  padD8[0x1C];
    float fF4;
} Vars347;

typedef struct {
    float pos[4];
    float rot[4];
    float v20[4];
    float v30[4];
    char *moby;
    int   pad[3];
} Aim347;

extern char *FUN_L00_0026dd20(char *pos, char *vel, int c, int d, float f);
extern char *spawn_class_558_moby_toward_target(char  *src,
                                                char  *pos,
                                                float *v,
                                                float  f) __asm__("FUN_L17_002e94d8");
extern char *try_activate_blarg_ship_from_list(int list) __asm__("FUN_L17_002f4fb8");
extern float vector_length_xy(void *);
extern char  fleet_turret_aim_vertex_indices[4] __asm__("D_L17_00161980") __attribute__((sda));
extern void *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float  scale,
                              float *v10,
                              float *v11,
                              float *v12);
extern void  select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void  process_dreks_fleet_turret_hits(struct Moby *m) __asm__("FUN_L17_002cbc10");
void distribute_group_blarg_ships_along_path(struct Moby *turret_moby) __asm__("FUN_L17_002cbd38");

extern void distribute_group_blarg_ships_along_path(struct Moby *m) __asm__("FUN_L17_002cbd38");
extern void
transform_scaled_vertex_batch(char *arg0, int arg1, void *arg2, char *arg3) __asm__("FUN_0020cd48");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void dreks_fleet_turret_update(struct Moby *m) __asm__("FUN_L17_002cb310");

void dreks_fleet_turret_update(struct Moby *turret_moby) {
    enum {
        FLEET_TURRET_INITIALIZE = 0,
        FLEET_TURRET_WAIT_FOR_TARGET = 1,
        FLEET_TURRET_AIM_AND_FIRE_AT_ZONE = 2,
        FLEET_TURRET_AIM_AND_FIRE_AT_HERO_SHIP = 3,
        FLEET_TURRET_DESTROY = 4
    };
    float    targeting_vector_scratch[4];
    float    from[4];
    Vars347 *turret_vars = (Vars347 *)turret_moby->pvars;

    process_dreks_fleet_turret_hits(turret_moby);
    switch (turret_moby->state) {
    case FLEET_TURRET_INITIALIZE:
        turret_moby->state = 1;
        turret_vars->f20 = 15.0f;
        turret_vars->s24 = 0xF;
        distribute_group_blarg_ships_along_path(turret_moby);
        if (visited52[turret_moby->mission + (current_level_index << 4)] == 0xFF) {
            mark_moby_for_removal(turret_moby);
        }
        break;
    case FLEET_TURRET_WAIT_FOR_TARGET: {
        struct Hero *g = &hero;
        if (g->unk308 != 0) {
            qcopy(targeting_vector_scratch, &turret_moby->pos);
            targeting_vector_scratch[2] += 10.0f;
            fast_vec_sub(
                from, D_L17_001600EC + (turret_vars->zone << 7) + 0x30, targeting_vector_scratch);
            fast_vec_scale(from, from, 0.9f);
            fast_vec_add(from, from, targeting_vector_scratch);
            from[2] += 0.5f;
            if (collision_line(targeting_vector_scratch, from, 2, turret_moby, 0) == 0) {
                turret_moby->state = 2;
            }
        } else if (g->state.current == 0x32) {
            char *hero_ship_moby = ((char *)g->ship_moby);
            if (hero_ship_moby != 0 && MOBY(hero_ship_moby)->oclass == 0x563) {
                if (vector_distance_xy(&turret_moby->pos, &g->motion.pos) < 254.0f) {
                    turret_moby->state = 3;
                }
            }
        }
        break;
    }
    case FLEET_TURRET_AIM_AND_FIRE_AT_ZONE: {
        if (hero.unk308 == 0) {
            turret_moby->state = 1;
        }
        {
            float  j[8];
            float  acc[4];
            float  tmp[4];
            float  zero[4];
            float *yaw;
            float  ang;
            float  turn;
            float  off;

            transform_scaled_vertex_batch(
                turret_moby, 2, fleet_turret_aim_vertex_indices, (char *)j);
            fast_vec_scale(
                tmp, D_L17_001600EC + (turret_vars->zone << 7), random_float_between(0.0f, 1.0f));
            fast_vec_add(acc, tmp, D_L17_001600EC + (turret_vars->zone << 7) + 0x30);
            fast_vec_scale(tmp,
                           D_L17_001600EC + (turret_vars->zone << 7) + 0x10,
                           random_float_between(0.0f, 1.0f));
            fast_vec_add(acc, acc, tmp);
            fast_vec_scale(tmp,
                           D_L17_001600EC + (turret_vars->zone << 7) + 0x20,
                           random_float_between(0.0f, 1.0f));
            fast_vec_add(acc, acc, tmp);
            fast_vec_sub(targeting_vector_scratch, acc, j);
            qcopy(from, j + 4);
            yaw = &turret_moby->rot.z;
            ang = fast_arc_tan(targeting_vector_scratch[0], targeting_vector_scratch[1]);
            turn = frame_time_sq * 0.5235988f;
            approach_angle(yaw, &turret_vars->yawvel, ang, turn, turn, frame_time * 1.0471976f);
            turret_vars->fF4 = -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                                             targeting_vector_scratch[2]);
            update_joint_animation(
                turret_moby, turret_vars->a90, 0, frame_scale_sq * 0.02f, frame_scale_sq * 0.3f);
            off = fast_difference_between_rotations(
                turret_vars->fD4,
                -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                              targeting_vector_scratch[2]));
            if (D_L17_0015F5CC % scale_game_frames(0x5A) == turret_vars->timer &&
                off < 0.034906585f) {
                if (fast_difference_between_rotations(
                        turret_moby->rot.z,
                        fast_arc_tan(*(float *)(D_L17_001600EC + turret_vars->zone * 0x80 + 0x30) -
                                         turret_moby->pos.x,
                                     *(float *)(D_L17_001600EC + turret_vars->zone * 0x80 + 0x34) -
                                         turret_moby->pos.y)) < 0.034906585f) {
                    char *p;
                    fast_vec_normalize(j, targeting_vector_scratch, frame_time * 60.0f);
                    fast_vec_add(j + 4, j, j + 4);
                    qzero(zero);
                    p = FUN_L00_0026dd20((char *)(j + 4), (char *)zero, 0x80FF8F6F, 3, 1260000.0f);
                    if (p != 0) {
                        p[9] = func_001FA898_r(4.0f) + 0x70;
                    }
                    allocate_voice_for_target_entry(0, 0, (int)turret_moby);
                    spawn_class_558_moby_toward_target(
                        turret_moby,
                        (char *)from,
                        (float *)(D_L17_001600EC + (turret_vars->zone << 7) + 0x30),
                        frame_time * 60.0f);
                }
            }
        }
        break;
    }
    case FLEET_TURRET_AIM_AND_FIRE_AT_HERO_SHIP: {
        struct Hero *g = &hero;
        if (g->state.current != 0x32 || ((char *)g->ship_moby) == 0 ||
            *(short *)(((char *)g->ship_moby) + 0xA6) != 0x563) {
            turret_moby->state = 1;
        } else {
            Aim347 aim;
            float  j[8];
            float  zero[4];
            float  tmp[4];
            float *yaw;
            float  ang;
            float  turn;
            float  off;

            transform_scaled_vertex_batch(
                turret_moby, 2, fleet_turret_aim_vertex_indices, (char *)j);
            select_moby_combat_target(turret_moby, &aim, 512.0f);
            fast_vec_sub(targeting_vector_scratch, aim.pos, j);
            qcopy(from, j + 4);
            yaw = &turret_moby->rot.z;
            ang = fast_arc_tan(targeting_vector_scratch[0], targeting_vector_scratch[1]);
            turn = frame_time_sq * 0.5235988f;
            approach_angle(yaw, &turret_vars->yawvel, ang, turn, turn, frame_time * 1.0471976f);
            turret_vars->fF4 = -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                                             targeting_vector_scratch[2]);
            update_joint_animation(
                turret_moby, turret_vars->a90, 0, frame_scale_sq * 0.02f, frame_scale_sq * 0.3f);
            off = fast_difference_between_rotations(
                turret_vars->fD4,
                -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                              targeting_vector_scratch[2]));
            if (fast_dec_timer(&turret_vars->timer) && off < 0.034906585f) {
                if (fast_difference_between_rotations(
                        turret_moby->rot.z,
                        fast_arc_tan(aim.pos[0] - turret_moby->pos.x,
                                     aim.pos[1] - turret_moby->pos.y)) < 0.034906585f) {
                    if (collision_line(from, aim.pos, 2, ((char *)g->ship_moby), 0) == 0) {
                        char *p;
                        qzero(zero);
                        fast_vec_sub(tmp, j + 4, j);
                        fast_vec_normalize(tmp, tmp, 250.0f);
                        fast_vec_add(tmp, tmp, j + 4);
                        p = FUN_L00_0026dd20(
                            (char *)(j + 4), (char *)zero, 0x80FF8F6F, 3, 1260000.0f);
                        if (p != 0) {
                            p[9] = func_001FA898_r(4.0f) + 0x70;
                        }
                        turret_vars->timer = scale_game_frames(0x5A) / 2;
                        allocate_voice_for_target_entry(0, 0, (int)turret_moby);
                        spawn_class_558_moby_toward_target(
                            turret_moby, (char *)from, tmp, frame_time * 60.0f);
                    }
                }
            }
            if (turret_vars->list != 0 && random_integer_below(0x1F4) == 0) {
                try_activate_blarg_ship_from_list(turret_vars->list);
            }
        }
        break;
    }
    case FLEET_TURRET_DESTROY:
        FUN_L00_00263fd8(turret_moby,
                         0x63A,
                         &turret_moby->pos.x,
                         &turret_moby->rot,
                         0,
                         0,
                         0.0f,
                         D_L17_0015F580,
                         D_L17_0015F580,
                         D_L17_0015F580);
        FUN_L00_00263fd8(turret_moby,
                         0x63B,
                         &turret_moby->pos.x,
                         &turret_moby->rot,
                         0,
                         0,
                         0.0f,
                         D_L17_0015F580,
                         D_L17_0015F580,
                         D_L17_0015F580);
        cart_burst(turret_moby, &turret_moby->pos, -1, 10.0f, 13.0f);
        mark_moby_for_removal(turret_moby);
        break;
    }
}

/* Steps a moby's countdown from a helper's output and retires it when it runs out. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002CD088), where it is exact; names translated to the US level program. */

void process_dreks_fleet_turret_hits(struct Moby *turret_moby) __asm__("FUN_L17_002cbc10");

void process_dreks_fleet_turret_hits(struct Moby *turret_moby) {
    char *turret_vars;
    char *hit_animation_state;
    char *hit_record;
    int   hit_reaction_status;
    float fv;
    int   depletion_status;
    turret_vars = ((char *)turret_moby->pvars);
    if (turret_moby->state != 0) {
        fv = 0.0f;
        hit_record = find_moby_hit(turret_moby, 0x330000, 0);
        if (hit_record != 0) {
            if (*(int *)(hit_record + 0x20) != 0) {
                int hit_moby_class_id = *(short *)(*(char **)(hit_record + 0x20) + 0xA6);
                if (hit_moby_class_id != 0x3F1) {
                    if (hit_moby_class_id != 0x127)
                        hit_record = 0;
                }
            }
        }
        hit_animation_state = turret_vars + 0x60;
        hit_reaction(
            turret_moby, hit_record, turret_vars + 0x20, 0, &hit_reaction_status, &fv, 0, 4);
        if (hit_reaction_status != 1 && turret_moby->state != 4) {
            float remaining_value = *(float *)(turret_vars + 0x20) - fv;
            *(float *)(turret_vars + 0x20) = remaining_value;
            depletion_status = 0xB;
            if (remaining_value <= 0.0f)
                depletion_status = 1;
            if (depletion_status == 1) {
                turret_moby->state = 4;
                turret_moby->flags &= 0xEFFF;
            }
            *(unsigned char *)(turret_vars + 0x67) = 0xF0;
            trooper_anim_update(turret_moby, (short *)hit_animation_state);
        }
        turret_moby->hit_slot = 0xFF;
        trooper_anim_finish(turret_moby, hit_animation_state);
    }
}

/* Second, %hi/%lo read of the moby array (retail reloads it for the placement pass). */
extern char *D_L17_0015FFD8_macro __asm__("D_L17_0015FFD8") MACRO_ADDR;

/* Spreads the class-0x733 mobies of this moby's group evenly along its path. */
void distribute_group_blarg_ships_along_path(struct Moby *turret_moby) {
    char  *turret_vars;
    char  *path;
    char  *mobys;
    char  *matched_moby;
    char  *od;
    short *group_entry;
    int    i;
    short  class_733_moby_count;
    short  k;
    short  step;

    turret_vars = ((char *)turret_moby->pvars);
    class_733_moby_count = 0;
    k = 0;
    i = *(short *)(turret_vars + 0x80);
    if (i == -1)
        return;
    group_entry = D_L17_001AC140[i];
    if (group_entry == 0)
        return;
    do {
        if (*(short *)(((*group_entry & 0x7FFF) << 8) + 0xA6 + D_L17_0015FFD8) == 0x733)
            class_733_moby_count++;
    } while (*group_entry++ >= 0);
    path = ((char **)D_L17_001B0DB0)[*(int *)(turret_vars + 0x84)];
    step = *(int *)path / class_733_moby_count;
    group_entry = D_L17_001AC140[i];
    mobys = D_L17_0015FFD8_macro;
    do {
        i = (*group_entry & 0x7FFF) << 8;
        if (*(short *)(i + 0xA6 + mobys) == 0x733) {
            matched_moby = i + mobys;
            od = ((char *)MOBY(matched_moby)->pvars);
            qcopy(matched_moby + 0x10, path + 0x10 + k * 0x10);
            *(int *)(od + 0x60) = *(int *)(turret_vars + 0x84);
            *(int *)(od + 0x64) = k;
            *(int *)(od + 0x68) = 0;
            k += step;
        }
    } while (*group_entry++ >= 0);
}
