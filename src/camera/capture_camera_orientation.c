/* Ported from rac1-decomp (src/game/camera.c, func_001ECAB8). */
#include "qcopy.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"

extern void compute_camera_angles(
    float *out, void *p0, void *p1, void *dir0, void *dir1, void *axis) __asm__("func_001EC530");
/* Builds three unit vectors from D_0013F350's +0x2080 pointer table
   (+0xC0/+0xD0/+0xE0 offsets, re-read at each call as retail does),
   stashes two of them into D_001871B0's record (+0x90, +0xA0), calls
   compute_camera_angles to compute the camera's yaw/pitch/dist into +0x70,
   then copies +0xD0 back over +0xB0 (retail's qcopy, see common.h). */
void capture_camera_orientation(void) __asm__("FUN_001ec710");

void capture_camera_orientation(void) {
    struct Hero *hero_state = &hero;
    char        *camera_transition_state = cam_trans_state;
    char         orbit_reference_direction[16];
    char         orbit_yaw_side_direction[16];
    char         orbit_axis[16];

    fast_vec_normalize(orbit_reference_direction, (char *)hero_state->moby + 0xC0, 1.0f);
    fast_vec_normalize(orbit_yaw_side_direction, (char *)hero_state->moby + 0xD0, 1.0f);
    fast_vec_normalize(orbit_axis, (char *)hero_state->moby + 0xE0, 1.0f);

    qcopy(camera_transition_state + 0x90, orbit_reference_direction);
    qcopy(camera_transition_state + 0xA0, orbit_axis);

    compute_camera_angles((float *)(camera_transition_state + 0x70),
                          camera_transition_state + 0xC0,
                          (char *)&hero_state->motion.pos,
                          orbit_reference_direction,
                          orbit_yaw_side_direction,
                          orbit_axis);

    qcopy(camera_transition_state + 0xB0, camera_transition_state + 0xD0);
}

extern __typeof__(capture_camera_orientation) func_001EC710 __attribute__((alias("FUN_001ec710")));
