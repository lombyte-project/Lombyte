/* Level 10 moby class gun. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"

typedef struct Moby Moby;
extern struct Moby *D_L10_0015FFD0_macro[] __asm__("D_L10_0015FFD0") MACRO_ADDR;
extern float        D_0015ED6C;
extern float        D_0015ED70;
extern float        D_0015ED6C, D_0015ED70;
extern float        fast_cos(float);
extern float        fast_sin(float);

typedef struct {
    int   linked;
    int   targets[3];
    float turn_velocity;
    int   wake;
} Vars;

extern struct Moby *D_L10_0015FFD8_8ab0 __asm__("D_L10_0015FFD8") __attribute__((sda));
extern int          D_L10_001BA950[] __asm__("D_L10_001BA950");
extern float        FUN_L00_00258110(float *, float, float, float, float, float);
extern struct Moby *spawn_turret_shot(void *, void *, void *, int) __asm__("FUN_L09_00307ba8");

void gun_update(struct Moby *moby) __asm__("FUN_L10_002e8ab0");

void gun_update(struct Moby *moby) {
    Vars *vars = (Vars *)moby->pvars;
    int  *bits;
    float rotation[4];
    float position[4];
    float displacement[4];
    float angle;
    int   state;

    state = moby->state;
    if (state == 1)
        return;
    if (state < 2) {
        if (state == 0)
            goto state_zero;
        return;
    }
    if (state == 2)
        goto state_two;
    if (state == 3)
        goto state_three;
    return;
state_zero: {
    unsigned short save_id;
    int            signed_save_id;
    save_id = moby->save_id;
    signed_save_id = (short)save_id;
    if (l10_save_state.cleared[(short)save_id] != 0 ||
        ((*(int *)((char *)D_0014C190_257218 +
                   ((signed_save_id >> 5) * 4 + (current_level_index << 8))) >>
          (save_id & 31)) &
         1) != 0) {
        moby->state = 3;
        return;
    }
    {
        if (vars->targets[0] == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        vars->turn_velocity = 0.0f;
        moby->state = 1;
        return;
    }
}
state_two: {
    unsigned short save_id;
    int            signed_save_id;
    struct Moby   *target;
    struct Moby   *candidate;
    save_id = moby->save_id;
    signed_save_id = (short)save_id;
    if (l10_save_state.cleared[(short)save_id] != 0 ||
        ((*(int *)((char *)D_0014C190_257218 +
                   ((signed_save_id >> 5) * 4 + (current_level_index << 8))) >>
          (save_id & 31)) &
         1) != 0) {
        moby->state = 3;
        return;
    }
    {
        struct Moby *candidate;
        candidate = 0;
        if (vars->targets[0] != -1) {
            struct Moby *target;
            target = (struct Moby *)((char *)D_L10_0015FFD0_macro[2] + (vars->targets[0] << 8));
            candidate =
                (target != 0 && target->state != 0xfe && target->state != 0xfd) ? target : 0;
        }
        {
            struct Moby *target;
            target = candidate;
            if (candidate == 0) {
                if (vars->targets[1] != -1) {
                    struct Moby *candidate;
                    candidate =
                        (struct Moby *)((char *)D_L10_0015FFD8_8ab0 + (vars->targets[1] << 8));
                    if (candidate != 0 && candidate->state != 0xfe && candidate->state != 0xfd)
                        target = candidate;
                }
            }
            candidate = target;
        }
        target = candidate;
    }
    if (target == 0) {
        if (vars->targets[2] != -1) {
            candidate = (struct Moby *)((char *)D_L10_0015FFD8_8ab0 + (vars->targets[2] << 8));
            if (candidate != 0 && candidate->state != 0xfe && candidate->state != 0xfd)
                target = candidate;
        }
    }
    if (target != 0) {
        moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
        angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
        moby->rot.z = FUN_L00_00258110(&vars->turn_velocity,
                                       moby->rot.z,
                                       angle,
                                       D_0015ED70 * 1.5707964f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 0.7853982f);
        angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
        if (fast_difference_between_rotations(moby->rot.z, angle) < 0.04363323f) {
            position[0] = fast_cos(moby->rot.z) * 1.5f;
            position[1] = fast_sin(moby->rot.z) * 1.5f;
            position[2] = 0.0f;
            add_vectors(displacement, position, &moby->pos);
            build_spherical_offset(
                rotation,
                D_0015ED6C * 40.0f,
                fast_arc_tan(target->pos.x - position[0], target->pos.y - position[1]),
                fast_arc_tan(vector_distance_xy(position, &target->pos),
                             target->pos.z - position[2]));
            spawn_turret_shot(moby, position, rotation, scale_game_frames(30));
            if (target ==
                (struct Moby *)((char *)D_L10_0015FFD0_macro[2] + (vars->targets[0] << 8)))
                vars->targets[0] = -1;
            else if (target ==
                     (struct Moby *)((char *)D_L10_0015FFD0_macro[2] + (vars->targets[1] << 8)))
                vars->targets[1] = -1;
            else
                vars->targets[2] = -1;
        }
        moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
    } else {
        moby->state = 3;
        if (vars->wake != -1) {
            ((struct Moby *)((vars->wake << 8) + (int)D_L10_0015FFD8_8ab0))->state = 1;
            vars->wake = -1;
        }
    }
    if (vars->linked != -1) {
        ((struct Moby *)((vars->linked << 8) + (int)D_L10_0015FFD8_8ab0))->command = 1;
        vars->linked = -1;
    }
    return;
}
state_three: {
    unsigned short save_id;
    struct Moby   *target;
    struct Moby   *candidate;
    target = 0;
    if (vars->targets[0] != -1) {
        struct Moby *candidate =
            (struct Moby *)((char *)D_L10_0015FFD0_macro[2] + (vars->targets[0] << 8));
        if (candidate != 0 && candidate->state != 0xfe && candidate->state != 0xfd)
            target = candidate;
    }
    /* Retail removes a valid first target twice. */
    if (target != 0) {
        mark_moby_for_removal(target);
        candidate = target;
        goto remove_candidate;
    }
    if (vars->targets[1] == -1)
        goto no_second_target;
    candidate = (struct Moby *)((char *)D_L10_0015FFD8_8ab0 + (vars->targets[1] << 8));
    if (candidate == 0 || candidate->state == 0xfe || candidate->state == 0xfd)
        goto no_second_target;
    goto remove_candidate;
no_second_target:
    candidate = target;
remove_candidate:
    if (candidate != 0)
        mark_moby_for_removal(candidate);
    save_id = moby->save_id;
    bits = (int *)((char *)D_0014C190_257218 +
                   (((int)((unsigned int)save_id << 16) >> 21) * 4 + (current_level_index << 8)));
    *bits |= 1 << (save_id & 31);
    save_id = moby->save_id;
    D_L10_001BA950[(int)((unsigned int)save_id << 16) >> 21] |= 1 << (save_id & 31);
    return;
}
}
