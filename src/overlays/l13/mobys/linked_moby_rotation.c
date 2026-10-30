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

void update_linked_moby_rotation(struct Moby *moby) __asm__("FUN_L13_002c7f38");

void update_linked_moby_rotation(struct Moby *moby) {
    enum {
        INITIALIZE_ROTATION = 0,
        WAIT_TO_ADVANCE_ROTATION = 1,
        ADVANCE_ROTATION = 2,
        HOLD_ROTATION_OFFSET = 3,
        REVERSE_ROTATION = 4
    };
    Data        *data = (Data *)moby->pvars;
    struct Moby *linked;
    float        step;
    float        radians;

    if (moby->mission != 0xFF && visited52[moby->mission + current_level_index * 16] == 0xFF) {
        data->linked_index = -1;
    }

    switch (moby->state) {
    case INITIALIZE_ROTATION:
        qcopy(data, &moby->rot);
        moby->state = data->start_active == 0 ? 1 : 2;
        data->progress = 0.0f;
        if (data->progress < data->initial_scale) {
            moby->scale = moby->pclass->scale * data->initial_scale;
        }
        return;
    case WAIT_TO_ADVANCE_ROTATION:
        if (data->linked_index != -1 &&
            (linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100)) != 0 &&
            linked->state != 0xFE && linked->state != 0xFD && linked->state != data->active_state) {
            return;
        }
        if (data->start_sound != -1) {
            allocate_voice_for_target_entry(data->start_sound, 0, moby);
        }
        data->progress = 0.0f;
        moby->state = 2;
        break;
    case ADVANCE_ROTATION:
        if (data->linked_index != -1 &&
            (linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100)) != 0 &&
            linked->state != 0xFE && linked->state != 0xFD &&
            linked->state == data->inactive_state) {
            if (data->start_sound != -1) {
                allocate_voice_for_target_entry(data->start_sound, 0, moby);
            }
            moby->state = 4;
            break;
        }
        {
            step = data->duration;
            step *= 60.0f;
            step = 1.0f / step;
            radians = 0.017453292f;
            data->progress += step;
            moby->rot.x = fast_add_rotations(moby->rot.x, data->angle[0] * radians * step);
            moby->rot.y = fast_add_rotations(moby->rot.y, data->angle[1] * radians * step);
            moby->rot.z = fast_add_rotations(moby->rot.z, data->angle[2] * radians * step);
            if (!(1.0f <= data->progress))
                return;
            moby->rot.x = fast_add_rotations(data->initial_rot[0], data->angle[0] * radians);
            moby->rot.y = fast_add_rotations(data->initial_rot[1], data->angle[1] * radians);
            moby->rot.z = fast_add_rotations(data->initial_rot[2], data->angle[2] * radians);
            data->progress = 1.0f;
            moby->state = 3;
            if (data->end_sound != -1) {
                allocate_voice_for_target_entry(data->start_sound, 0, moby);
            }
            return;
        }
        break;
    case HOLD_ROTATION_OFFSET:
        if (data->linked_index == -1)
            return;
        linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100);
        if (linked == 0 || linked->state == 0xFE || linked->state == 0xFD ||
            linked->state != data->inactive_state)
            return;
        data->progress = 1.0f;
        if (data->start_sound != -1) {
            allocate_voice_for_target_entry(data->start_sound, 0, moby);
        }
        moby->state = 4;
        break;
    case REVERSE_ROTATION:
        if (data->linked_index == -1 ||
            (linked = (struct Moby *)(D_L13_0015FFD8 + data->linked_index * 0x100)) == 0 ||
            linked->state == 0xFE || linked->state == 0xFD || linked->state == data->active_state) {
            if (data->start_sound != -1) {
                allocate_voice_for_target_entry(data->start_sound, 0, moby);
            }
            moby->state = 2;
        } else {
            int invalid_index = -1;
            radians = 0.017453292f;
            step = 1.0f;
            step /= data->duration * 60.0f;
            data->progress -= step;
            moby->rot.x = fast_subtract_rotations(moby->rot.x, data->angle[0] * radians * step);
            moby->rot.y = fast_subtract_rotations(moby->rot.y, data->angle[1] * radians * step);
            moby->rot.z = fast_subtract_rotations(moby->rot.z, data->angle[2] * radians * step);
            if (!(data->progress <= 0.0f))
                return;
            moby->rot.x = data->initial_rot[0];
            moby->rot.y = data->initial_rot[1];
            moby->rot.z = data->initial_rot[2];
            data->progress = 0.0f;
            if (data->start_sound != invalid_index) {
                allocate_voice_for_target_entry(data->end_sound, 0, moby);
            }
            moby->state = 1;
        }
        break;
    case 5:
        break;
    }
}
