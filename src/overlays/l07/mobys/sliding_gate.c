/* Level 07 moby class 0x468: sliding gate; slides to its offset and back as its target moby opens and shuts. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"

struct L07MovingVars {
    Vec4f home;
    int   target;
    int   openState;
    int   shutState;
    Vec4f destination;
    float speed;
    int   startSound;
    int   endSound;
    int   indicator;
    int   forceReturn;
};

extern struct Moby *D_L07_0015FFD8;

void sliding_gate_update(struct Moby *moby) __asm__("FUN_L07_0031a9e8");

/* Moby class 1128 */
void sliding_gate_update(struct Moby *moby) {
    enum {
        MOVING_MOBY_INITIALIZE = 0,
        MOVING_MOBY_WAIT_AT_HOME = 1,
        MOVING_MOBY_MOVE_TO_OFFSET = 2,
        MOVING_MOBY_WAIT_AT_OFFSET = 3,
        MOVING_MOBY_RETURN_HOME = 4
    };
    struct L07MovingVars *offset_motion = (struct L07MovingVars *)moby->pvars;
    struct Moby          *target;
    Vec4                  offset_vector;
    Vec4                  start;
    float                 distance;
    float                 time;
    int                   force_end;
    int                   force_start;
    u8                    spawn_id;
    int                   no_spawn;

    if (moby->mission != 0xff && visited52[moby->mission + current_level_index * 16] == 0xff)
        offset_motion->target = -1;
    spawn_id = moby->mission;
    force_end = 0;
    /* Fakematch: the 0xFF compare needs its own variable for retail register order. */
    no_spawn = 0xff;
    force_start = 0;
    if (D_L07_0015FC88[spawn_id] == no_spawn) {
        if (offset_motion->forceReturn != 0)
            force_end = 1;
        else
            force_start = 1;
    }

    switch (moby->state) {
    case MOVING_MOBY_INITIALIZE:
        qcopy(&offset_motion->home, &moby->pos);
        if (offset_motion->indicator != -1)
            D_L07_00184528[offset_motion->indicator] = 0;
        moby->state = 1;
        offset_motion->home.w = 0;
        return;
    case MOVING_MOBY_WAIT_AT_HOME:
        if (force_start)
            return;
        if (!force_end && offset_motion->target != -1 &&
            (target = &D_L07_0015FFD8[offset_motion->target]) != 0 && target->state != 0xfe &&
            target->state != 0xfd && target->state != offset_motion->openState)
            return;
        start.q = 0;
        start.f[0] = offset_motion->destination.x;
        start.f[1] = offset_motion->destination.y;
        start.f[2] = offset_motion->destination.z;
        offset_vector.q = start.q;
        offset_motion->home.w = fast_vec_length(&offset_vector);
        if (offset_motion->startSound != -1)
            allocate_voice_for_target_entry(offset_motion->startSound, 0, moby);
        moby->state = 2;
        break;
    case MOVING_MOBY_MOVE_TO_OFFSET:
        if (force_start ||
            (!force_end && offset_motion->target != -1 &&
             (target = &D_L07_0015FFD8[offset_motion->target]) != 0 && target->state != 0xfe &&
             target->state != 0xfd && target->state == offset_motion->shutState)) {
            if (offset_motion->startSound != -1)
                allocate_voice_for_target_entry(offset_motion->startSound, 0, moby);
            moby->state = 4;
            break;
        }
        start.q = 0;
        start.f[0] = offset_motion->destination.x;
        start.f[1] = offset_motion->destination.y;
        start.f[2] = offset_motion->destination.z;
        offset_vector.q = start.q;
        transform_vector_by_basis(&offset_vector, &offset_vector, &moby->rot_row0);
        fast_vec_normalize(&start, &offset_vector, offset_motion->destination.w * frame_time);
        fast_vec_add(&moby->pos, &moby->pos, &start);
        time = offset_motion->home.w - offset_motion->destination.w * frame_time;
        offset_motion->home.w = time;
        if (!(time < 0.0f))
            return;
        fast_vec_add(&moby->pos, &offset_motion->home, &offset_vector);
        offset_motion->home.w = 0;
        if (offset_motion->indicator != -1)
            D_L07_00184528[offset_motion->indicator] = 1;
        moby->state = 3;
        if (offset_motion->endSound != -1)
            allocate_voice_for_target_entry(offset_motion->startSound, 0, moby);
        return;
    case MOVING_MOBY_WAIT_AT_OFFSET:
        if (!force_start &&
            (force_end || offset_motion->target == -1 ||
             (target = &D_L07_0015FFD8[offset_motion->target]) == 0 || target->state == 0xfe ||
             target->state == 0xfd || target->state != offset_motion->shutState))
            return;
        offset_motion->home.w = 0;
        if (offset_motion->startSound != -1)
            allocate_voice_for_target_entry(offset_motion->startSound, 0, moby);
        moby->state = 4;
        break;
    case MOVING_MOBY_RETURN_HOME:
        if (!force_start &&
            (force_end || offset_motion->target == -1 ||
             (target = &D_L07_0015FFD8[offset_motion->target]) == 0 || target->state == 0xfe ||
             target->state == 0xfd || target->state == offset_motion->openState)) {
            if (offset_motion->startSound != -1)
                allocate_voice_for_target_entry(offset_motion->startSound, 0, moby);
            moby->state = 2;
            break;
        }
        start.q = 0;
        start.f[0] = offset_motion->destination.x;
        start.f[1] = offset_motion->destination.y;
        start.f[2] = offset_motion->destination.z;
        offset_vector.q = start.q;
        distance = fast_vec_length(&offset_vector);
        fast_vec_normalize(&offset_vector, &offset_vector, -(offset_motion->speed * frame_time));
        transform_vector_by_basis(&start, &offset_vector, &moby->rot_row0);
        fast_vec_add(&moby->pos, &moby->pos, &start);
        time = offset_motion->home.w + offset_motion->speed * frame_time;
        offset_motion->home.w = time;
        if (!(distance < time))
            return;
        qcopy(&moby->pos, &offset_motion->home);
        if (offset_motion->startSound != -1)
            allocate_voice_for_target_entry(offset_motion->endSound, 0, moby);
        if (offset_motion->indicator != -1)
            D_L07_00184528[offset_motion->indicator] = 0;
        moby->state = 1;
        break;
    case 5:
        break;
    }
}
