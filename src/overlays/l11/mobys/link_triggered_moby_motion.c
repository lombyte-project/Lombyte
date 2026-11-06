/* Level 11 moby class link_triggered_moby_motion. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/interpolation/advance_accelerated_scalar.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030FE40), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x57];
    char motion_triggered;
} Rec57;

extern float fast_add_rotations(float, float);
extern int   D_L11_0015FFD8;

void link_triggered_moby_motion_update(struct Moby *moving_moby) __asm__("FUN_L11_0030e978");
/* no foreign declaration */

typedef struct LinkTriggeredMobyMotionVars {
    u8  pad_0[0x4];
    f32 speed;
    s32 linked_moby;
    f32 size_scale;
    f32 travel_distance;
    s32 trigger_record_offset;
} LinkTriggeredMobyMotionVars;

/* Moby class 1159 */
void link_triggered_moby_motion_update(struct Moby *moving_moby) {
    enum {
        LINKED_MOTION_STATE_INITIALIZE = 0,
        LINKED_MOTION_STATE_WAIT_FOR_LINK_TRIGGER = 1,
        LINKED_MOTION_STATE_MOVE_ALONG_LOCAL_AXIS = 2
    };
    LinkTriggeredMobyMotionVars *private_data = (LinkTriggeredMobyMotionVars *)moving_moby->pvars;
    float                        motion_displacement[4] __attribute__((aligned(16)));
    struct Moby                 *spawned_moby;
    struct Moby                 *linked_moby;
    char                        *spawned_private_data;
    struct MobyClass            *source_moby_class;
    switch (moving_moby->state) {
    case LINKED_MOTION_STATE_INITIALIZE:
        moving_moby->pos.z = moving_moby->pos.z + private_data->size_scale * 1.9f;
        spawned_moby = (struct Moby *)create_moby(0x487);
        spawned_moby->visible = 1;
        spawned_moby->draw_dist = 0x40;
        spawned_moby->spawn_frame = moving_moby->spawn_frame;
        spawned_moby->flags = moving_moby->flags;
        qcopy(&spawned_moby->pos, &moving_moby->pos);
        qcopy(&spawned_moby->rot, &moving_moby->rot);
        spawned_moby->rot.x = fast_add_rotations(spawned_moby->rot.x, 3.1415927f);
        update_entity(spawned_moby);
        spawned_private_data = (char *)spawned_moby->pvars;
        *(int *)(spawned_private_data + 8) = private_data->linked_moby;
        *(float *)(spawned_private_data + 0x10) = private_data->travel_distance;
        *(int *)(spawned_private_data + 0x14) = private_data->trigger_record_offset;
        source_moby_class = moving_moby->pclass;
        moving_moby->scale = source_moby_class->scale * private_data->size_scale;
        spawned_moby->scale = source_moby_class->scale * private_data->size_scale;
        spawned_moby->state = LINKED_MOTION_STATE_WAIT_FOR_LINK_TRIGGER;
        moving_moby->state = LINKED_MOTION_STATE_WAIT_FOR_LINK_TRIGGER;
        break;
    case LINKED_MOTION_STATE_WAIT_FOR_LINK_TRIGGER:
        linked_moby = (struct Moby *)((private_data->linked_moby << 8) + (int)D_L11_0015FFD8);
        if (linked_moby->oclass == 0x267 && linked_moby->state == 4) {
            goto go;
        }
        if (linked_moby->oclass == 0x33E && linked_moby->command != 0) {
        go:
            moving_moby->state = LINKED_MOTION_STATE_MOVE_ALONG_LOCAL_AXIS;
            ((Rec57 *)(private_data->trigger_record_offset + (int)(alternate_item_available)))
                ->motion_triggered = 1;
            allocate_voice_for_target_entry(0, 0, moving_moby);
        }
        break;
    case LINKED_MOTION_STATE_MOVE_ALONG_LOCAL_AXIS:
        advance_accelerated_scalar((float *)private_data,
                                   &private_data->speed,
                                   private_data->travel_distance,
                                   frame_time_sq,
                                   frame_time_sq,
                                   frame_time + frame_time);
        fast_vec_normalize(motion_displacement, &moving_moby->rot_row1, -private_data->speed);
        fast_vec_add(&moving_moby->pos, &moving_moby->pos, motion_displacement);
        break;
    }
}
