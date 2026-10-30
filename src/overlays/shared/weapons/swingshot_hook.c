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
    unsigned char *spawned_moby = create_moby(0xd1);
    if (spawned_moby) {
        char *moby_vars = *(char **)(spawned_moby + 0x78);
        *(float *)(moby_vars + 4) = initial_hook_speed;
        *(unsigned char **)moby_vars = owner_moby;
        spawned_moby[0x30] = 0xff;
        *(short *)(spawned_moby + 0x32) = 0xff;
        spawned_moby[0x31] = 1;
        set_moby_color(spawned_moby, 0x80, 0x80, 0x80);
        moby_point_position(owner_moby, 0, spawned_moby + 0x10);
        *(u128 *)(spawned_moby + 0xc0) = *(u128 *)(owner_moby + 0xc0);
        *(u128 *)(spawned_moby + 0xd0) = *(u128 *)(owner_moby + 0xd0);
        *(u128 *)(spawned_moby + 0xe0) = *(u128 *)(owner_moby + 0xe0);
        if (spawned_moby[0x53])
            blend_moby_animation(spawned_moby, 0, 0, scale_game_frames(10));
        update_entity(spawned_moby);
        spawned_moby[0x20] = 0;
        *(unsigned short *)(spawned_moby + 0x34) |= 4;
    }
    return spawned_moby;
}

void swingshot_hook_update(unsigned char *m) __asm__("FUN_L00_002c7070");

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
    unsigned char **hook_vars;
    struct Moby    *owner_moby;
    if (hook_moby == 0)
        return;
    hook_vars = *(unsigned char ***)(hook_moby + 0x78);
    if (hook_vars == 0)
        return;
    owner_moby = hook_vars[0];
    if (owner_moby == 0 || owner_moby->oclass != 0xD0 || owner_moby->state == 0xFE ||
        owner_moby->state == 0xFD ||
        *(unsigned char **)(((unsigned char *)owner_moby->pvars) + 0x14) != hook_moby) {
        goto kill;
    }
    if ((owner_moby->flags & 1) || D_001413FF) {
        *(unsigned short *)(hook_moby + 0x34) |= 0x41;
    } else {
        *(unsigned short *)(hook_moby + 0x34) &= ~0x41;
    }
    switch (hook_moby[0x20]) {
    case HOOK_INITIAL_ANIMATION:
        if (hook_moby[0x70] & 2) {
            if (hook_moby[0x53] != 1)
                blend_moby_animation(hook_moby, 1, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 1;
            *(float *)(hook_moby + 0x58) = 1.0f;
        }
        break;
    case HOOK_IDLE_ANIMATION:
        if (hook_vars[0][0x20] == 2) {
            if (hook_moby[0x53] != 3)
                blend_moby_animation(hook_moby, 3, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 3;
            *(float *)(hook_moby + 0x58) = 1.0f;
        } else if (hook_vars[0][0x20] == 8) {
            if (hook_moby[0x53] != 2)
                blend_moby_animation(hook_moby, 2, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 2;
            *(float *)(hook_moby + 0x58) = 1.0f;
        }
        break;
    case HOOK_REMOVAL_ANIMATION:
        if (hook_moby[0x70] & 2) {
        kill:
            mark_moby_for_removal(hook_moby);
            return;
        }
        break;
    case HOOK_LAUNCH_ANIMATION:
        if (hook_moby[0x70] & 2) {
            if (hook_moby[0x53] != 4)
                blend_moby_animation(hook_moby, 4, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 4;
        }
        break;
    case HOOK_FLIGHT_ANIMATION:
        if ((unsigned char)(hook_vars[0][0x20] - 3) < 3) {
            if (hook_moby[0x53] != 5)
                blend_moby_animation(hook_moby, 5, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 5;
        }
        break;
    case HOOK_ATTACHED_ANIMATION:
        if (hook_moby[0x70] & 2)
            *(int *)(hook_moby + 0x58) = 0;
        if ((unsigned char)(hook_vars[0][0x20] - 6) < 2) {
            *(float *)(hook_moby + 0x58) = 1.0f;
            if (hook_moby[0x53] != 7)
                blend_moby_animation(hook_moby, 7, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 7;
        }
        break;
    case HOOK_SKIP_ANIMATION_TRANSITIONS:
        break;
    case HOOK_RETURN_ANIMATION:
        if (!(hook_moby[0x70] & 2))
            break;
    default:
        if (hook_moby[0x53] != 1)
            blend_moby_animation(hook_moby, 1, 0, FUN_001f96f8_2c7070(10));
        hook_moby[0x20] = 1;
        break;
    }
    refresh_moby_spatial_bounds_from_basis(hook_moby);
}
