/* Ported from rac1-decomp (src/game/camera.c, func_001ED658). */

#include "sda.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"

#include "rnc/camera/update_cam.h"
#include "rnc/camera.h"

extern int step_camera_blend(void *, struct CameraBlend *) __asm__("func_001ECAF8");

/* The last destination is camera_rows - 0x210, a different member reached
   by pointer arithmetic on the same char array. */
void update_camera_blend(char *to_cam) __asm__("FUN_001ed2b0");

void update_camera_blend(char *to_cam) {
    enum {
        CAMERA_TRANSITION_PHASE_IDLE = 0
    };
    enum {
        CAMERA_TRANSITION_KIND_PRIMARY = 0
    };
    struct CameraTransition *transition = &cam_trans_state;
    int                      transition_complete;

    if (transition->kind == CAMERA_TRANSITION_KIND_PRIMARY) {
        transition_complete = step_camera_blend(to_cam, &transition->blend);
    } else {
        transition_complete = advance_alternate_camera_transition(to_cam, &transition->orbit);
    }
    if (transition_complete != 0) {
        if (camera_position_publication_suppressed[0] == 0) {
            qcopy(camera_rows, to_cam);
            qcopy(camera_rows + 0x10, to_cam + 0x10);
            qcopy(camera_rows + 0x20, to_cam + 0x20);
            qcopy(camera_rows - 0x210, to_cam + 0x30);
        }
        transition->mode = CAMERA_TRANSITION_PHASE_IDLE;
        transition->kind = CAMERA_TRANSITION_KIND_PRIMARY;
    }
}

extern __typeof__(update_camera_blend) func_001ED2B0 __attribute__((alias("FUN_001ed2b0")));
