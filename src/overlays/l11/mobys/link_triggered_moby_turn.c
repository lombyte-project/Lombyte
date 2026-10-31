/* Level 11 moby class link_triggered_moby_turn. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/level_labels.h"

extern int test_linked_moby_turn_trigger(void *) __asm__("FUN_L11_0030f270");

extern float fast_add_rotations(float, float);

/* Waits for its linked moby, then either hands off to a cutscene or turns toward its target. */
extern char *D_L11_0015FFD8_t __asm__("D_L11_0015FFD8");
extern char  D_0013D3E3_t[] __asm__("D_0013D3E3");
extern float
approach_angle(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

void link_triggered_moby_turn_update(struct Moby *turning_moby) __asm__("FUN_L11_0030f2a8");

/* Moby class 1180 */
void link_triggered_moby_turn_update(struct Moby *turning_moby) {
    enum {
        LINKED_TURN_STATE_INITIALIZE = 0,
        LINKED_TURN_STATE_WAIT_FOR_LINK = 1,
        LINKED_TURN_STATE_ROTATE_TO_TARGET = 2
    };
    int    entry_state = turning_moby->state;
    float *private_data = (float *)turning_moby->pvars;
    float  target_angle;
    float  turn_acceleration_step;
    switch (entry_state) {
    case LINKED_TURN_STATE_INITIALIZE:
        turning_moby->update_dist = 0xFF;
        turning_moby->state = 1;
        private_data[2] = turning_moby->rot.z;
        break;
    case LINKED_TURN_STATE_WAIT_FOR_LINK:
        if (test_linked_moby_turn_trigger(D_L11_0015FFD8_t + (*(int *)private_data << 8))) {
            if (level_frame >= 11) {
                D_0013D3E3_t[0] = entry_state;
                turning_moby->state = 2;
                allocate_voice_for_target_entry(0, 0, turning_moby);
            } else {
                turning_moby->state = 3;
                turning_moby->rot.z = fast_add_rotations(private_data[2], 1.0471976f);
            }
        }
        break;
    case LINKED_TURN_STATE_ROTATE_TO_TARGET:
        target_angle = fast_add_rotations(private_data[2], 1.0471976f);
        turn_acceleration_step = frame_time_sq * 0.34906584f;
        if (approach_angle(&turning_moby->rot.z,
                           target_angle,
                           private_data + 1,
                           turn_acceleration_step,
                           turn_acceleration_step,
                           frame_time * 0.7853982f) == 0.0f) {
            turning_moby->state = 3;
        }
        break;
    }
}
