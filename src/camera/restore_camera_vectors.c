/* Ported from rac1-decomp (src/game/camera.c, func_001ECC10). */
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"

/* When the flag at +2 is set, copies the 16-byte vectors at +0xC0 and
   +0xD0 back to +0x50 and +0x60 with retail's qcopy (see common.h). */
void restore_camera_vectors(void) __asm__("FUN_001ec868");

void restore_camera_vectors(void) {
    char *camera_transition_state = cam_trans_state;
    if (*(unsigned char *)(camera_transition_state + 2) != 0) {
        qcopy(camera_transition_state + 0x50, camera_transition_state + 0xC0);
        qcopy(camera_transition_state + 0x60, camera_transition_state + 0xD0);
    }
}

extern __typeof__(restore_camera_vectors) func_001EC868 __attribute__((alias("FUN_001ec868")));
