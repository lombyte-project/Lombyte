/* Shared moby class giant_clank_pad: the pad where the hero becomes Giant Clank. */

#include "rnc/input/pad_globals.h"
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "sda.h"
#include "rnc/ui/help/try_set_help_message.h"

extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void enter_hero_vehicle_mode(int, int, struct Moby *) __asm__("FUN_L00_00210a08");

extern int          D_L15_0015F5C4;
extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");
extern int          D_L15_0015F594 __attribute__((sda));
extern u8           D_0014C050[];

void giant_clank_pad_update(struct Moby *moby) __asm__("FUN_L15_002ed068");

/* Moby classes 1451, 1899 */
void giant_clank_pad_update(struct Moby *moby) {
    enum {
        GIANT_CLANK_PAD_INITIALIZE = 0,
        GIANT_CLANK_PAD_ARM_ACTIVATION = 1,
        GIANT_CLANK_PAD_CHECK_ACTIVATION = 2
    };
    enum {
        GIANT_CLANK_PAD_ACTIVATION_DISABLED = 3
    };
    struct Moby *linked = moby->pvars;
    struct Moby *linked_vehicle_moby;
    struct Moby *pad;
    u8           state = moby->state;
    u8           position[16] __attribute__((aligned(16)));

    switch (state) {
    case GIANT_CLANK_PAD_INITIALIZE: {
        s32 level;
        if (*(int *)((u8 *)linked + 0x60) == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        linked_vehicle_moby =
            (struct Moby *)((char *)l15_moby_array + (*(int *)((u8 *)linked + 0x60) << 8));
        linked_vehicle_moby->flags |= 1;
        linked_vehicle_moby->collision = 0;
        linked_vehicle_moby->visible = 0;
        level = current_level_index;
        moby->state = (level != 0x12 || D_0014C050[0x120 + moby->mission] != 0xff) ? 1 : 3;
        break;
    }
    case GIANT_CLANK_PAD_ARM_ACTIVATION:
        /* Fakematch: the second store gives retail's register for moby. */
        if (moby)
            moby->state = GIANT_CLANK_PAD_CHECK_ACTIVATION;
        else
            moby->state = GIANT_CLANK_PAD_CHECK_ACTIVATION;
        return;
    case GIANT_CLANK_PAD_CHECK_ACTIVATION:
        if (hero.ground_moby == moby && hero.air_frames.s == 0 && D_L15_0015F5C4 != 2 &&
            hero.state.current != 0x1d) {
            if (hero.vehicle_mode == HERO_VEHICLE_MODE_ON_FOOT) {
                try_set_help_message(4, 0x3aa4);
            } else if (hero.vehicle_mode == HERO_VEHICLE_MODE_GIANT_CLANK &&
                       current_level_index != LEVEL_VELDIN2) {
                try_set_help_message(4, 0x3aa5);
            }
            if (!(((u32)pad_pressed_lui) & 0x10))
                return;
            if (D_L15_0015F594 != 4)
                return;
            if (hero.vehicle_mode == HERO_VEHICLE_MODE_GIANT_CLANK &&
                current_level_index == LEVEL_QUARTU) {
                pad = l15_moby_array;
                pad += *(int *)((u8 *)linked + 0x60);
                pad->flags |= 1;
                pad->collision = 0;
                pad->visible = 0;
                restore_saved_hero_moby_and_clear_vehicle_mode();
                set_moby_animation(pad, 0, 0);
                if (current_level_index != LEVEL_QUARTU)
                    return;
                start_scene(9);
                request_music_track(0, 5);
                return;
            }
            if (hero.vehicle_mode != HERO_VEHICLE_MODE_ON_FOOT)
                return;
            pad = l15_moby_array;
            pad += *(int *)((u8 *)linked + 0x60);
            *(u128 *)position = *(u128 *)&moby->pos;
            *(float *)(position + 8) = probe_ground_height(position, 0, 0.5f) + 0.1f;
            set_race_camera(position, &moby->rot, 0, 1);
            pad->collision = pad->pclass->collision;
            pad->visible = 1;
            pad->flags &= ~1;
            enter_hero_vehicle_mode(2, 0x5a, pad);
            if (current_level_index == LEVEL_QUARTU) {
                start_scene(8);
                *(u128 *)hero.saved_pos_rot = *(u128 *)&hero.motion.pos;
                *(u128 *)(hero.saved_pos_rot + 16) = *(u128 *)&hero.motion.rot;
                request_music_track(2, 4);
                return;
            }
            if (current_level_index != LEVEL_VELDIN2)
                return;
            start_scene(6);
            *(u128 *)hero.saved_pos_rot = *(u128 *)&hero.motion.pos;
            *(u128 *)(hero.saved_pos_rot + 16) = *(u128 *)&hero.motion.rot;
            moby->state = GIANT_CLANK_PAD_ACTIVATION_DISABLED;
            return;
        }
        break;
    default:
        return;
    }
    *(int *)((u8 *)linked + 0x68) = -1;
}
