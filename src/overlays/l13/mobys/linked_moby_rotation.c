/* Level 13 moby class that rotates with a linked moby. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"

extern char *D_L13_0015FFD8;

typedef struct Data {
    float initial_rot[3];
    float progress;
    int   linked_index;
    int   active_state;
    int   inactive_state;
    float angle[3];
    int   unknown_28;
    float duration;
    int   start_sound;
    int   end_sound;
    float initial_scale;
    int   start_active;
} Data;

void linked_moby_rotation_update(struct Moby *moby) __asm__("FUN_L13_002c7f38");

/* Moby classes 127, 128, 159, 169 */
void linked_moby_rotation_update(struct Moby *moby) {
    enum {
        INITIALIZE_ROTATION = 0,
        WAIT_TO_ADVANCE_ROTATION = 1,
        ADVANCE_ROTATION = 2,
        HOLD_ROTATION_OFFSET = 3,
        REVERSE_ROTATION = 4
    };
    Data        *rotation_vars = (Data *)moby->pvars;
    struct Moby *linked;
    float        step;
    float        radians;

    if (moby->mission != 0xFF && visited52[moby->mission + current_level_index * 16] == 0xFF) {
        rotation_vars->linked_index = -1;
    }

    switch (moby->state) {
    case INITIALIZE_ROTATION:
        qcopy(rotation_vars, &moby->rot);
        moby->state = rotation_vars->start_active == 0 ? 1 : 2;
        rotation_vars->progress = 0.0f;
        if (rotation_vars->progress < rotation_vars->initial_scale) {
            moby->scale = moby->pclass->scale * rotation_vars->initial_scale;
        }
        return;
    case WAIT_TO_ADVANCE_ROTATION:
        if (rotation_vars->linked_index != -1 &&
            (linked = (struct Moby *)(D_L13_0015FFD8 + rotation_vars->linked_index * 0x100)) != 0 &&
            linked->state != 0xFE && linked->state != 0xFD &&
            linked->state != rotation_vars->active_state) {
            return;
        }
        if (rotation_vars->start_sound != -1) {
            allocate_voice_for_target_entry(rotation_vars->start_sound, 0, moby);
        }
        rotation_vars->progress = 0.0f;
        moby->state = 2;
        break;
    case ADVANCE_ROTATION:
        if (rotation_vars->linked_index != -1 &&
            (linked = (struct Moby *)(D_L13_0015FFD8 + rotation_vars->linked_index * 0x100)) != 0 &&
            linked->state != 0xFE && linked->state != 0xFD &&
            linked->state == rotation_vars->inactive_state) {
            if (rotation_vars->start_sound != -1) {
                allocate_voice_for_target_entry(rotation_vars->start_sound, 0, moby);
            }
            moby->state = 4;
            break;
        }
        {
            step = rotation_vars->duration;
            step *= 60.0f;
            step = 1.0f / step;
            radians = 0.017453292f;
            rotation_vars->progress += step;
            moby->rot.x = fast_add_rotations(moby->rot.x, rotation_vars->angle[0] * radians * step);
            moby->rot.y = fast_add_rotations(moby->rot.y, rotation_vars->angle[1] * radians * step);
            moby->rot.z = fast_add_rotations(moby->rot.z, rotation_vars->angle[2] * radians * step);
            if (!(1.0f <= rotation_vars->progress))
                return;
            moby->rot.x = fast_add_rotations(rotation_vars->initial_rot[0],
                                             rotation_vars->angle[0] * radians);
            moby->rot.y = fast_add_rotations(rotation_vars->initial_rot[1],
                                             rotation_vars->angle[1] * radians);
            moby->rot.z = fast_add_rotations(rotation_vars->initial_rot[2],
                                             rotation_vars->angle[2] * radians);
            rotation_vars->progress = 1.0f;
            moby->state = 3;
            if (rotation_vars->end_sound != -1) {
                allocate_voice_for_target_entry(rotation_vars->start_sound, 0, moby);
            }
            return;
        }
        break;
    case HOLD_ROTATION_OFFSET:
        if (rotation_vars->linked_index == -1)
            return;
        linked = (struct Moby *)(D_L13_0015FFD8 + rotation_vars->linked_index * 0x100);
        if (linked == 0 || linked->state == 0xFE || linked->state == 0xFD ||
            linked->state != rotation_vars->inactive_state)
            return;
        rotation_vars->progress = 1.0f;
        if (rotation_vars->start_sound != -1) {
            allocate_voice_for_target_entry(rotation_vars->start_sound, 0, moby);
        }
        moby->state = 4;
        break;
    case REVERSE_ROTATION:
        if (rotation_vars->linked_index == -1 ||
            (linked = (struct Moby *)(D_L13_0015FFD8 + rotation_vars->linked_index * 0x100)) == 0 ||
            linked->state == 0xFE || linked->state == 0xFD ||
            linked->state == rotation_vars->active_state) {
            if (rotation_vars->start_sound != -1) {
                allocate_voice_for_target_entry(rotation_vars->start_sound, 0, moby);
            }
            moby->state = 2;
        } else {
            int invalid_index = -1;
            radians = 0.017453292f;
            step = 1.0f;
            step /= rotation_vars->duration * 60.0f;
            rotation_vars->progress -= step;
            moby->rot.x =
                fast_subtract_rotations(moby->rot.x, rotation_vars->angle[0] * radians * step);
            moby->rot.y =
                fast_subtract_rotations(moby->rot.y, rotation_vars->angle[1] * radians * step);
            moby->rot.z =
                fast_subtract_rotations(moby->rot.z, rotation_vars->angle[2] * radians * step);
            if (!(rotation_vars->progress <= 0.0f))
                return;
            moby->rot.x = rotation_vars->initial_rot[0];
            moby->rot.y = rotation_vars->initial_rot[1];
            moby->rot.z = rotation_vars->initial_rot[2];
            rotation_vars->progress = 0.0f;
            if (rotation_vars->start_sound != invalid_index) {
                allocate_voice_for_target_entry(rotation_vars->end_sound, 0, moby);
            }
            moby->state = 1;
        }
        break;
    case 5:
        break;
    }
}
