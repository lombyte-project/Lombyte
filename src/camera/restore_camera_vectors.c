/* Ported from rac1-decomp (src/game/camera.c, func_001ECC10). */
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"

void restore_camera_vectors(void) __asm__("FUN_001ec868");

void restore_camera_vectors(void) {
    struct CameraTransition *transition = &cam_trans_state;
    if (transition->kind != 0) {
        qcopy(transition->blend.position, transition->orbit.saved_position);
        qcopy(transition->blend.rotation, transition->orbit.saved_rotation);
    }
}

extern __typeof__(restore_camera_vectors) func_001EC868 __attribute__((alias("FUN_001ec868")));
