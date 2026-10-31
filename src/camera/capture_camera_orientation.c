/* Ported from rac1-decomp (src/game/camera.c, func_001ECAB8). */
#include "qcopy.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"

extern void compute_camera_angles(
    float *out, void *p0, void *p1, void *dir0, void *dir1, void *axis) __asm__("func_001EC530");
/* The hero moby basis rows are re-read at each call as retail does. */
void capture_camera_orientation(void) __asm__("FUN_001ec710");

void capture_camera_orientation(void) {
    struct Hero             *hero_state = &hero;
    struct CameraTransition *transition = &cam_trans_state;
    char                     orbit_reference_direction[16];
    char                     orbit_yaw_side_direction[16];
    char                     orbit_axis[16];

    fast_vec_normalize(orbit_reference_direction, &hero_state->moby->rot_row0, 1.0f);
    fast_vec_normalize(orbit_yaw_side_direction, &hero_state->moby->rot_row1, 1.0f);
    fast_vec_normalize(orbit_axis, &hero_state->moby->rot_row2, 1.0f);

    qcopy(transition->orbit.forward, orbit_reference_direction);
    qcopy(transition->orbit.up, orbit_axis);

    compute_camera_angles(transition->orbit.angles,
                          transition->orbit.saved_position,
                          (char *)&hero_state->motion.pos,
                          orbit_reference_direction,
                          orbit_yaw_side_direction,
                          orbit_axis);

    qcopy(transition->orbit.rotation, transition->orbit.saved_rotation);
}

extern __typeof__(capture_camera_orientation) func_001EC710 __attribute__((alias("FUN_001ec710")));
