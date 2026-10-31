/* Shared moby class underwater_door (and door 0xC4/0xC5): slides away from its origin and back. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"

typedef struct {
    Vec4 origin;
    s32  trigger;
    f32  travel;
    s32  start_volume;
    s32  stop_volume;
} MovingMobyVars;

extern float        fast_sin(float);
extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

void underwater_door_update(struct Moby *moby) __asm__("FUN_L15_002bddb0");

/* Moby classes 196, 197, 1958 */
void underwater_door_update(struct Moby *moby) {
    enum {
        DOOR_INITIALIZE = 0,
        DOOR_WAIT_FOR_TRIGGER = 1,
        DOOR_MOVE_AWAY_FROM_ORIGIN = 2,
        DOOR_WAIT_FOR_START_VOLUME_ENTRY = 4,
        DOOR_WAIT_FOR_STOP_VOLUME_EXIT = 5,
        DOOR_RETURN_TO_ORIGIN = 6
    };
    MovingMobyVars *door_motion_data = (MovingMobyVars *)moby->pvars;
    Vec4            offset;
    Vec4            updated_door_position;
    f32             step;
    f32             magnitude;
    f32             travel;
    s32             reversed;
    s32             next_state;

    reversed = moby->oclass == 0xc4 || moby->oclass == 0x7a6;
    if (door_motion_data->trigger == -1 &&
        (door_motion_data->start_volume == -1 || door_motion_data->stop_volume == -1)) {
        mark_moby_for_removal(moby);
        return;
    }

    switch (moby->state) {
    case DOOR_INITIALIZE:
        door_motion_data->origin.q = *(u128 *)&moby->pos;
        if (door_motion_data->trigger != 0) {
            moby->state = 1;
            return;
        }
        moby->state = 4;
        return;
    case DOOR_WAIT_FOR_TRIGGER:
        if (((struct Moby *)((int)l15_moby_array + door_motion_data->trigger * 0x100))->state != 4)
            return;
        if (reversed)
            allocate_voice_for_target_entry(0, 0, (int)moby);
        moby->state = 2;
        break;
    case DOOR_MOVE_AWAY_FROM_ORIGIN: {
        union {
            u32 bits;
            f32 value;
        } triple;
        f32 increment;
        f32 scale;
        travel = door_motion_data->travel;
        increment = (moby->scale * 3.0f / moby->pclass->scale) / 3.0f * frame_time;
        increment = reversed ? -increment : increment;
        door_motion_data->travel = travel + increment;
        offset.f[0] = fast_cos(moby->rot.z) * door_motion_data->travel;
        offset.f[1] = fast_sin(moby->rot.z) * door_motion_data->travel;
        offset.f[2] = 0.0f;
        fast_vec_add(&updated_door_position, &door_motion_data->origin, &offset);
        *(u128 *)&moby->pos = updated_door_position.q;
        magnitude = absolute_float(door_motion_data->travel);
        scale = moby->scale;
        triple.bits = 0x40400000;
        __builtin_memcpy(&step, &triple.bits, sizeof(step));
        step = step + 0.0f;
        if (!(scale * step / moby->pclass->scale < magnitude))
            return;
        if (door_motion_data->trigger != -1) {
            moby->state = 3;
            return;
        }
        moby->state = 5;
        return;
    }
    case DOOR_WAIT_FOR_START_VOLUME_ENTRY:
        door_motion_data->travel = 0.0f;
        if (!is_point_inside_clip_volume(&hero.motion.pos, door_motion_data->start_volume))
            return;
        if (reversed)
            allocate_voice_for_target_entry(0, 0, (int)moby);
        moby->state = 2;
        break;
    case DOOR_WAIT_FOR_STOP_VOLUME_EXIT:
        if (is_point_inside_clip_volume(&hero.motion.pos, door_motion_data->stop_volume))
            return;
        if (reversed)
            allocate_voice_for_target_entry(0, 0, (int)moby);
        moby->state = 6;
        break;
    case DOOR_RETURN_TO_ORIGIN: {
        f32 old_travel;
        f32 new_travel;
        magnitude = absolute_float(door_motion_data->travel);
        step = (moby->scale * 3.0f / moby->pclass->scale) / 3.0f * frame_time;
        if (step < magnitude) {
            old_travel = door_motion_data->travel;
            if (reversed) {
                f32 reverse_step = -step;
                new_travel = old_travel - reverse_step;
            } else {
                new_travel = old_travel - step;
            }
            door_motion_data->travel = new_travel;
        } else {
            door_motion_data->travel = 0.0f;
        }
        offset.f[0] = fast_cos(moby->rot.z) * door_motion_data->travel;
        offset.f[1] = fast_sin(moby->rot.z) * door_motion_data->travel;
        offset.f[2] = 0.0f;
        fast_vec_add(&updated_door_position, &door_motion_data->origin, &offset);
        *(u128 *)&moby->pos = updated_door_position.q;
        if (door_motion_data->travel != 0.0f)
            return;
        moby->state = 4;
        break;
    }
    }
}
