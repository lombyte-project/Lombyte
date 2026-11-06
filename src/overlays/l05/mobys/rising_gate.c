/* Level 05 moby class rising_gate (0x354, 0x355): once its trigger moby fires, rises to its top height and swings back to yaw 0, with a sound per class. */
#include "types.h"
#include "rnc/overlay/l05.h"
#include "rnc/overlay/l05/mobys/rising_gate_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern int allocate_voice_for_target_entry() __asm__("FUN_0022da68");

struct GateVars {
    s32 trigger; /* l05_moby_array index of the moby whose byte 0xBC opens it */
    s32 riseTimer;
    s32 swingTimer;
    f32 topZ;
    f32 riseSpeed;
    f32 swingSpeed;
};

float gate_rise_acceleration_gp __asm__("D_L05_00161E80") = 5e+01f;
float gate_rise_deceleration_gp __asm__("D_L05_00161E84") = 1e+01f;
float gate_rise_max_speed_gp __asm__("D_L05_00161E88") = 18.0f;
float gate_swing_acceleration_gp __asm__("D_L05_00161E8C") = 3.6e+02f;
float gate_swing_deceleration_gp __asm__("D_L05_00161E90") = 3.6e+02f;
float gate_swing_max_speed_gp __asm__("D_L05_00161E94") = 1.44e+03f;
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");

/* Gate: waits for its trigger moby, then rises to topZ and swings its yaw back to 0, with sounds per class. */
void rising_gate_update(struct Moby *gate_moby) __asm__("FUN_L05_00314eb0");

/* Moby classes 852, 853 */
void rising_gate_update(struct Moby *gate_moby) {
    enum {
        GATE_INITIALIZE = 0,
        GATE_WAIT_FOR_TRIGGER = 1,
        GATE_RAISE_AND_SWING = 2
    };
    enum {
        GATE_YAW_ALIGNED = 3
    };
    struct GateVars *gate_vars = (struct GateVars *)gate_moby->pvars;
    struct Moby     *trigger_moby;

    switch (gate_moby->state) {
    case GATE_INITIALIZE:
        gate_vars->topZ = gate_moby->pos.z + 1.0f;
        gate_moby->state = GATE_WAIT_FOR_TRIGGER;
        gate_moby->pos.z -= 8.0f;
        break;
    case GATE_WAIT_FOR_TRIGGER:
        trigger_moby = (struct Moby *)((gate_vars->trigger << 8) + (int)l05_moby_array);
        if (trigger_moby->command == 1) {
            gate_moby->state = GATE_RAISE_AND_SWING;
            break;
        }
        if (trigger_moby->command == 2) {
            gate_moby->pos.z = gate_vars->topZ;
            gate_moby->state = GATE_YAW_ALIGNED;
            gate_moby->rot.y = 0.0f;
        }
        break;
    case GATE_RAISE_AND_SWING:
        if (fast_dec_timer(&gate_vars->riseTimer)) {
            advance_accelerated_scalar(gate_vars->topZ,
                                       gate_rise_acceleration_gp * frame_time_sq,
                                       gate_rise_deceleration_gp * frame_time_sq,
                                       gate_rise_max_speed_gp * frame_time,
                                       &gate_moby->pos.z,
                                       &gate_vars->riseSpeed);
            if (gate_moby->oclass == 0x355 &&
                absolute_float(gate_moby->pos.z - gate_vars->topZ) < 3.0f &&
                3.0f < absolute_float(gate_vars->topZ - gate_moby->pos.z + gate_vars->riseSpeed))
                allocate_voice_for_target_entry(0, 0, gate_moby);
        }
        if (fast_dec_timer(&gate_vars->swingTimer)) {
            approach_angle(&gate_moby->rot.y,
                           &gate_vars->swingSpeed,
                           0.0f,
                           gate_swing_acceleration_gp * 0.017453292f * frame_time_sq,
                           gate_swing_deceleration_gp * 0.017453292f * frame_time_sq,
                           gate_swing_max_speed_gp * 0.017453292f * frame_time);
            if (gate_moby->rot.y == 0.0f)
                gate_moby->state = GATE_YAW_ALIGNED;
        } else if (gate_vars->swingTimer == 1 && gate_moby->oclass == 0x354) {
            allocate_voice_for_target_entry(0, 0, gate_moby);
        }
        break;
    }
}
