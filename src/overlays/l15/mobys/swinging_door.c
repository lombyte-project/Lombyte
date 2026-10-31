/* Level 15 swinging door moby class. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/l15/mobys/swinging_door_globals.h"

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

extern char D_L15_001673C0[];

typedef struct {
    f32 closed_yaw;   /* 0x00: yaw when shut */
    s32 open_volume;  /* 0x04: someone inside opens it */
    s32 close_volume; /* 0x08: nobody inside lets it close */
    f32 swing;        /* 0x0C: current swing from closed_yaw */
    s32 reverse;      /* 0x10: swings the other way */
    s32 trigger;      /* 0x14: moby index that opens it, -1 for the volumes */
    s32 sound;        /* 0x18 */
} DoorVars;

extern char *D_L15_0015FFE4;

f32 swinging_door_swing_angle_degrees_gp __asm__("D_L15_001614FC") = -35.0f;

f32 swinging_door_swing_duration_gp __asm__("D_L15_00161500") = 0.2f;

void swinging_door_update(struct Moby *moby) __asm__("FUN_L15_002a3ba8");

/* Moby class 93 */
void swinging_door_update(struct Moby *moby) {
    enum {
        SWINGING_DOOR_INITIALIZE = 0,
        SWINGING_DOOR_CLOSED = 1,
        SWINGING_DOOR_OPENING = 2,
        SWINGING_DOOR_OPEN = 3,
        SWINGING_DOOR_CLOSING = 4
    };
    DoorVars    *door_vars = (DoorVars *)moby->pvars;
    s32          open = 0;
    s32          close = 0;
    struct Moby *candidate_moby;
    f32          step;
    f32          limit;
    f32          yaw;

    if (door_vars->trigger == -1) {
        if (moby->state != 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, door_vars->open_volume) ||
                is_point_inside_clip_volume(D_L15_001673C0, door_vars->open_volume)) {
                open = 1;
            } else if (!is_point_inside_clip_volume(&hero.motion.pos, door_vars->close_volume) &&
                       !is_point_inside_clip_volume(D_L15_001673C0, door_vars->close_volume)) {
                close = 1;
            }
            for (candidate_moby = (struct Moby *)D_L15_0015FFE4; candidate_moby != 0;
                 candidate_moby = candidate_moby->next) {
                if (get_moby_target_info(candidate_moby) == 0) {
                    continue;
                }
                if (is_point_inside_clip_volume(&candidate_moby->pos, door_vars->open_volume)) {
                    open = 1;
                } else if (!is_point_inside_clip_volume(&candidate_moby->pos,
                                                        door_vars->close_volume)) {
                    close = 1;
                }
            }
        }
    } else {
        if (l15_moby_array[door_vars->trigger].oclass == 0x49B) {
            open = l15_moby_array[door_vars->trigger].state == 2;
        }
    }
    if (open) {
        close = 0;
    }
    switch (moby->state) {
    case SWINGING_DOOR_INITIALIZE:
        if (door_vars->reverse) {
            moby->flags |= 0x8000;
        }
        step = swinging_door_swing_angle_degrees_gp * DEG_TO_RAD;
        if (!door_vars->reverse) {
            step = -step;
        }
        yaw = fast_add_rotations(moby->rot.z, step);
        door_vars->swing = 0.0f;
        door_vars->closed_yaw = yaw;
        moby->rot.z = yaw;
        moby->state = 1;
        break;
    case SWINGING_DOOR_CLOSED:
        door_vars->swing = 0.0f;
        moby->rot.z = door_vars->closed_yaw;
        if (open) {
            moby->state = 2;
            door_vars->sound = allocate_voice_for_target_entry(0, 0, (int)moby);
        }
        break;
    case SWINGING_DOOR_OPENING:
        door_vars->swing =
            fast_add_rotations(door_vars->swing,
                               swinging_door_swing_angle_degrees_gp /
                                   swinging_door_swing_duration_gp * DEG_TO_RAD * frame_time);
        limit = swinging_door_swing_angle_degrees_gp * DEG_TO_RAD;
        if (door_vars->swing < limit) {
            door_vars->swing = limit;
            moby->state = 3;
        }
        step = door_vars->swing;
        if (door_vars->reverse) {
            step = -step;
        }
        moby->rot.z = fast_add_rotations(door_vars->closed_yaw, step);
        break;
    case SWINGING_DOOR_OPEN:
        if (close) {
            moby->state = 4;
            door_vars->sound = allocate_voice_for_target_entry(0, 0, (int)moby);
        }
        break;
    case SWINGING_DOOR_CLOSING:
        door_vars->swing =
            fast_add_rotations(door_vars->swing,
                               -(swinging_door_swing_angle_degrees_gp /
                                 swinging_door_swing_duration_gp * DEG_TO_RAD * frame_time));
        if (0.0f < door_vars->swing) {
            door_vars->swing = 0.0f;
            moby->state = 1;
        }
        step = door_vars->swing;
        if (door_vars->reverse) {
            step = -step;
        }
        moby->rot.z = fast_add_rotations(door_vars->closed_yaw, step);
        if (open) {
            moby->state = 2;
        }
        break;
    }
}
