/* Shared moby class swingshot_hook: the swingshot hook, and the function that spawns it. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
int        FUN_001f96f8_2c7070(int) __asm__("FUN_001f96f8");

extern unsigned char D_001413FF NOT_SDA;

unsigned char *spawn_swingshot_hook(unsigned char *owner_moby,
                                    float          initial_hook_speed) __asm__("FUN_L00_002c6f88");

unsigned char *spawn_swingshot_hook(unsigned char *owner_moby, float initial_hook_speed) {
    enum {
        HOOK_INITIAL_ANIMATION = 0
    };
    unsigned char *spawned_moby = create_moby(0xd1);
    if (spawned_moby) {
        char *moby_vars = *(char **)(spawned_moby + 0x78);
        *(float *)(moby_vars + 4) = initial_hook_speed;
        *(unsigned char **)moby_vars = owner_moby;
        MOBY(spawned_moby)->update_dist = 0xff;
        MOBY(spawned_moby)->draw_dist = 0xff;
        MOBY(spawned_moby)->visible = 1;
        set_moby_color(spawned_moby, 0x80, 0x80, 0x80);
        moby_point_position(owner_moby, 0, &MOBY(spawned_moby)->pos);
        *(u128 *)(spawned_moby + 0xc0) = *(u128 *)(owner_moby + 0xc0);
        *(u128 *)(spawned_moby + 0xd0) = *(u128 *)(owner_moby + 0xd0);
        *(u128 *)(spawned_moby + 0xe0) = *(u128 *)(owner_moby + 0xe0);
        if (MOBY(spawned_moby)->prev_seq)
            blend_moby_animation(spawned_moby, 0, 0, scale_game_frames(10));
        update_entity(spawned_moby);
        MOBY(spawned_moby)->state = HOOK_INITIAL_ANIMATION;
        MOBY(spawned_moby)->flags |= 4;
    }
    return spawned_moby;
}

void swingshot_hook_update(unsigned char *m) __asm__("FUN_L00_002c7070");

/* Moby class 209 */
void swingshot_hook_update(unsigned char *hook_moby) {
    enum {
        HOOK_INITIAL_ANIMATION = 0,
        HOOK_IDLE_ANIMATION = 1,
        HOOK_REMOVAL_ANIMATION = 2,
        HOOK_LAUNCH_ANIMATION = 3,
        HOOK_FLIGHT_ANIMATION = 4,
        HOOK_ATTACHED_ANIMATION = 5,
        HOOK_SKIP_ANIMATION_TRANSITIONS = 6,
        HOOK_RETURN_ANIMATION = 7
    };
    enum {
        MOBY_STATE_MARKED_FOR_REMOVAL = 0xFE
    };
    enum {
        MOBY_STATE_REMOVAL_PENDING = 0xFD
    };
    unsigned char **hook_vars;
    struct Moby    *owner_moby;
    if (hook_moby == 0)
        return;
    hook_vars = *(unsigned char ***)(hook_moby + 0x78);
    if (hook_vars == 0)
        return;
    owner_moby = hook_vars[0];
    if (owner_moby == 0 || owner_moby->oclass != 0xD0 ||
        owner_moby->state == MOBY_STATE_MARKED_FOR_REMOVAL ||
        owner_moby->state == MOBY_STATE_REMOVAL_PENDING ||
        *(unsigned char **)(((unsigned char *)owner_moby->pvars) + 0x14) != hook_moby) {
        goto kill;
    }
    if ((owner_moby->flags & 1) || D_001413FF) {
        MOBY(hook_moby)->flags |= 0x41;
    } else {
        MOBY(hook_moby)->flags &= ~0x41;
    }
    switch (MOBY(hook_moby)->state) {
    case HOOK_INITIAL_ANIMATION:
        if (MOBY(hook_moby)->anim_flags & 2) {
            if (MOBY(hook_moby)->prev_seq != 1)
                blend_moby_animation(hook_moby, 1, 0, FUN_001f96f8_2c7070(10));
            MOBY(hook_moby)->state = HOOK_IDLE_ANIMATION;
            MOBY(hook_moby)->anim_speed = 1.0f;
        }
        break;
    case HOOK_IDLE_ANIMATION:
        if (hook_vars[0][0x20] == 2) {
            if (MOBY(hook_moby)->prev_seq != 3)
                blend_moby_animation(hook_moby, 3, 0, FUN_001f96f8_2c7070(10));
            MOBY(hook_moby)->state = HOOK_LAUNCH_ANIMATION;
            MOBY(hook_moby)->anim_speed = 1.0f;
        } else if (hook_vars[0][0x20] == 8) {
            if (MOBY(hook_moby)->prev_seq != 2)
                blend_moby_animation(hook_moby, 2, 0, FUN_001f96f8_2c7070(10));
            MOBY(hook_moby)->state = HOOK_REMOVAL_ANIMATION;
            MOBY(hook_moby)->anim_speed = 1.0f;
        }
        break;
    case HOOK_REMOVAL_ANIMATION:
        if (MOBY(hook_moby)->anim_flags & 2) {
        kill:
            mark_moby_for_removal(hook_moby);
            return;
        }
        break;
    case HOOK_LAUNCH_ANIMATION:
        if (MOBY(hook_moby)->anim_flags & 2) {
            if (MOBY(hook_moby)->prev_seq != 4)
                blend_moby_animation(hook_moby, 4, 0, FUN_001f96f8_2c7070(10));
            MOBY(hook_moby)->state = HOOK_FLIGHT_ANIMATION;
        }
        break;
    case HOOK_FLIGHT_ANIMATION:
        if ((unsigned char)(hook_vars[0][0x20] - 3) < 3) {
            if (MOBY(hook_moby)->prev_seq != 5)
                blend_moby_animation(hook_moby, 5, 0, FUN_001f96f8_2c7070(10));
            MOBY(hook_moby)->state = HOOK_ATTACHED_ANIMATION;
        }
        break;
    case HOOK_ATTACHED_ANIMATION:
        if (MOBY(hook_moby)->anim_flags & 2)
            *(int *)(hook_moby + 0x58) = 0;
        if ((unsigned char)(hook_vars[0][0x20] - 6) < 2) {
            MOBY(hook_moby)->anim_speed = 1.0f;
            if (MOBY(hook_moby)->prev_seq != 7)
                blend_moby_animation(hook_moby, 7, 0, FUN_001f96f8_2c7070(10));
            MOBY(hook_moby)->state = HOOK_RETURN_ANIMATION;
        }
        break;
    case HOOK_SKIP_ANIMATION_TRANSITIONS:
        break;
    case HOOK_RETURN_ANIMATION:
        if (!(MOBY(hook_moby)->anim_flags & 2))
            break;
    default:
        if (MOBY(hook_moby)->prev_seq != 1)
            blend_moby_animation(hook_moby, 1, 0, FUN_001f96f8_2c7070(10));
        MOBY(hook_moby)->state = HOOK_IDLE_ANIMATION;
        break;
    }
    refresh_moby_spatial_bounds_from_basis(hook_moby);
}
