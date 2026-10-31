/* Level 04 moby class platform_booster. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/level_labels.h"

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void release_voice_slot(int);

/* Pvars of a door driven by a lever (platform_booster_update update): follows the lever's travel and plays its sounds. */
typedef struct {
    s32 lever;  /* 0x00: moby index of the lever, -1 for none */
    s32 target; /* 0x04: moby index it carries along, -1 for none */
    s32 voice;  /* 0x08: looping sound, -1 for none */
    s32 locked; /* 0x0C */
} DoorVars;

/* Pvars of the lever (oclass 0x118). */
typedef struct {
    f32 travel; /* 0x00: 0..1 */
    u8  pad4[0x14];
    f32 spin; /* 0x18 */
    u8  pad1C[0x14];
    f32 rate; /* 0x30 */
} LeverVars;

extern int          FUN_L00_0028d8c0(void *, int);
extern int          allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern struct Moby *moby_array_macro __asm__("D_L04_0015FFD8") MACRO_ADDR;

/* Door update: follows its lever's travel, pushes the lever back while opening, and runs the motor sounds. */
void platform_booster_update(struct Moby *m) __asm__("FUN_L04_002c6bb8");

/* Moby class 434 */
void platform_booster_update(struct Moby *moby) {
    enum {
        PLATFORM_BOOSTER_STATE_INITIALIZE = 0,
        PLATFORM_BOOSTER_STATE_FOLLOW_LEVER = 1,
        PLATFORM_BOOSTER_STATE_ADVANCE_ANIMATION = 2
    };
    DoorVars    *door_vars = (DoorVars *)moby->pvars;
    f32          previous_anim_time;
    struct Moby *lever;
    LeverVars   *lv;
    u8          *slot;

    previous_anim_time = moby->anim_time;
    if (door_vars == NULL) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case PLATFORM_BOOSTER_STATE_INITIALIZE:
        blend_moby_animation(moby, 1, 0, scale_game_frames(300));
        moby->state = 1;
        moby->anim_speed = 0;
        door_vars->voice = -1;
        break;
    case PLATFORM_BOOSTER_STATE_FOLLOW_LEVER:
        if (door_vars->lever == -1) {
            break;
        }
        lever = &((struct Moby *)moby_array)[door_vars->lever];
        if (lever->oclass != 0x118) {
            break;
        }
        lv = (LeverVars *)lever->pvars;
        if (lever->state == 3) {
            moby->anim_time = 1.0f - lv->travel;
        } else if (lv->travel != 1.0f ||
                   (door_vars->target != -1 && door_vars->locked == 0 &&
                    ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9 ||
                     hero.state.control_mode == 12) &&
                    hero.ground_moby == &((struct Moby *)moby_array)[door_vars->target])) {
            moby->state = 2;
        }
        if (moby->anim_time != previous_anim_time) {
            if (lv->travel != 1.0f && lv->travel != 0.0f) {
                if (FUN_L00_0028d8c0(moby, door_vars->voice) == 0) {
                    door_vars->voice = allocate_voice_for_target_entry(0, 4, (int)moby);
                }
            } else {
                if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
                    if (door_vars->voice != -1) {
                        slot = (u8 *)&voice_pool + door_vars->voice * 0x70;
                        if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                            release_voice_slot(door_vars->voice);
                        }
                    }
                    door_vars->voice = -1;
                }
                allocate_voice_for_target_entry(1, 0, (int)moby);
            }
        } else if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
            if (door_vars->voice != -1) {
                slot = (u8 *)&voice_pool + door_vars->voice * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                    release_voice_slot(door_vars->voice);
                }
            }
            door_vars->voice = -1;
        }
        break;
    case PLATFORM_BOOSTER_STATE_ADVANCE_ANIMATION: {
        struct Moby *link;
        LeverVars   *link_vars;

        moby->anim_time = previous_anim_time + frame_time * 0.5f;
        if (1.0f < moby->anim_time) {
            moby->anim_time = 1.0f;
        }
        if (door_vars->lever == -1) {
            break;
        }
        link = &moby_array_macro[door_vars->lever];
        if (link->oclass != 0x118) {
            break;
        }
        link_vars = (LeverVars *)link->pvars;
        if (link->state == 3) {
            moby->anim_time = 1.0f - link_vars->travel;
            moby->state = 1;
        } else {
            link_vars->travel = 1.0f - moby->anim_time;
            link_vars->spin = link_vars->travel * (link_vars->rate * 6.2831855f);
        }
        if (moby->anim_time != previous_anim_time) {
            if (link_vars->travel == 1.0f || link_vars->travel == 0.0f) {
                if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
                    if (door_vars->voice != -1) {
                        slot = (u8 *)&voice_pool + door_vars->voice * 0x70;
                        if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                            release_voice_slot(door_vars->voice);
                        }
                    }
                    door_vars->voice = -1;
                }
                allocate_voice_for_target_entry(1, 0, (int)moby);
            } else if (FUN_L00_0028d8c0(moby, door_vars->voice) == 0) {
                door_vars->voice = allocate_voice_for_target_entry(0, 4, (int)moby);
            }
        } else if (FUN_L00_0028d8c0(moby, door_vars->voice) != 0) {
            if (door_vars->voice != -1) {
                slot = (u8 *)&voice_pool + door_vars->voice * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0) {
                    release_voice_slot(door_vars->voice);
                }
            }
            door_vars->voice = -1;
        }
        break;
    }
    }
    if (door_vars->target != -1) {
        moby_point_position(moby, 0, &moby_array_macro[door_vars->target].pos);
    }
}
