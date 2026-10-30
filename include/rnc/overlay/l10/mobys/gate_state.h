#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_GATE_STATE_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_GATE_STATE_H

#include "types.h"

/* Level 10 paired gates: the states gate_1 and gate_2 share. */

enum GateState {
    GATE_STATE_INITIALIZE = 0x0,
    GATE_STATE_CLOSED = 0x1,
    GATE_STATE_OPENING = 0x2,
    GATE_STATE_OPEN = 0x3
};

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_GATE_STATE_H */
