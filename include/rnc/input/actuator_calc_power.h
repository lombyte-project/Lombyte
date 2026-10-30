#ifndef LOMBYTE_RNC_INPUT_ACTUATOR_CALC_POWER_H
#define LOMBYTE_RNC_INPUT_ACTUATOR_CALC_POWER_H

#include "types.h"

/* One vibration wave actuator_calc_power steps each frame: f00 is the
   waveform (0: inactive), f02 the motor, f06 the remaining frames. */
typedef struct {
    s16 f00;
    u8  f02;
    u8  f03;
    s16 f04;
    u16 f06;
    s16 f08;
    s16 f0a;
    s16 f0c;
    u8  f0e;
    u8  f0f;
} ActuatorWave;

/* The 8 wave slots actuator_calc_power walks. */
extern ActuatorWave actuator_wave[8] __asm__("D_00165480");

#endif /* LOMBYTE_RNC_INPUT_ACTUATOR_CALC_POWER_H */
