/* Ported from rac1-decomp (src/game/camera.c, func_001ECB98). */
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"

void save_camera_vectors(void) __asm__("FUN_001ec7f0");

void save_camera_vectors(void) {
    enum {
        CAMERA_TRANSITION_PRIMARY_SOLVER = 0
    };
    struct CameraTransition *transition = &cam_trans_state;
    if (transition->kind == CAMERA_TRANSITION_PRIMARY_SOLVER) {
        qcopy(transition->orbit.saved_position, transition->blend.position);
        if (transition->requested_kind == 2) {
            fast_vec_add(
                transition->orbit.saved_position, D_0013F490, transition->orbit.saved_position);
        }
        qcopy(transition->orbit.saved_rotation, transition->blend.rotation);
    }
}

extern __typeof__(save_camera_vectors) func_001EC7F0 __attribute__((alias("FUN_001ec7f0")));
