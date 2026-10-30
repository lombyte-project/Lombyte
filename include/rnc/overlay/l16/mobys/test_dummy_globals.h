#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/mobys/test_dummy.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float test_dummy_bomb_horizontal_launch_speed __asm__("D_L16_001619CC");
extern float test_dummy_glow_phase_rate_degrees __asm__("D_L16_001619DC");
extern int   test_dummy_glow_pulse_color_start __asm__("D_L16_001619E0");
extern int   test_dummy_glow_pulse_color_end __asm__("D_L16_001619E4");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_GLOBALS_H */
