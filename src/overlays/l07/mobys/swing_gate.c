/* Level 07 moby class 0x46d: swing gate; swings open and back as its target moby opens and shuts. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"

extern struct Moby *D_L07_0015FFD8;

struct SwingGateVars {
    int   target;    /* 0x00: moby index, -1 for none */
    int   openState; /* 0x04 */
    int   shutState; /* 0x08 */
    float speed;     /* 0x0C */
    float damp;      /* 0x10 */
    float rate;      /* 0x14 */
    float vel;       /* 0x18 */
};

void update_swing_gate(struct Moby *moby) __asm__("FUN_L07_0031b3f0");

void update_swing_gate(struct Moby *moby) {
    enum {
        SWING_GATE_INITIALIZE = 0,
        SWING_GATE_WAIT_FOR_OPEN_CONDITION = 1,
        SWING_GATE_HOLD_ROTATION = 2,
        SWING_GATE_SWINGING = 3,
        SWING_GATE_RETURN_TO_ZERO = 4
    };
    struct SwingGateVars *vars = (struct SwingGateVars *)moby->pvars;
    struct Moby          *target;
    float                 updated_y_rotation;

    if (vars == 0)
        return;
    switch (moby->state) {
    case SWING_GATE_INITIALIZE:
        moby->rot.y = 0;
        moby->state = 1;
        vars->vel = 0;
        break;
    case SWING_GATE_WAIT_FOR_OPEN_CONDITION:
        if (vars->target != -1 && (target = &D_L07_0015FFD8[vars->target]) != 0 &&
            target->state != 0xFE && target->state != 0xFD && target->state != vars->openState)
            return;
        vars->vel = vars->speed * frame_time;
        moby->rot.y = vars->vel;
        allocate_voice_for_target_entry(0, 0, moby);
        moby->state = 3;
        break;
    case SWING_GATE_HOLD_ROTATION:
        break;
    case SWING_GATE_SWINGING:
        vars->vel =
            fast_add_rotations(vars->vel, fast_cos(moby->rot.y) * (vars->speed * frame_time));
        moby->rot.y = fast_add_rotations(moby->rot.y, vars->vel);
        vars->vel = vars->vel * vars->damp;
        target = &D_L07_0015FFD8[vars->target];
        if (target == 0 || target->state == 0xFE || target->state == 0xFD)
            break;
        if (target->state != vars->shutState)
            return;
        moby->state = 4;
        break;
    case SWING_GATE_RETURN_TO_ZERO:
        if (vars->target == -1 || (target = &D_L07_0015FFD8[vars->target]) == 0 ||
            target->state == 0xFE || target->state == 0xFD || target->state == vars->openState) {
            allocate_voice_for_target_entry(0, 0, moby);
            moby->state = 2;
            break;
        }
        updated_y_rotation = fast_subtract_rotations(moby->rot.y, vars->rate);
        moby->rot.y = updated_y_rotation;
        if (0.0f < updated_y_rotation) {
            moby->rot.y = 0;
            vars->vel = 0;
            moby->state = 1;
        }
        break;
    case 5:
        break;
    }
}
