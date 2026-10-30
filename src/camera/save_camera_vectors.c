/* Ported from rac1-decomp (src/game/camera.c, func_001ECB98). */
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"

/* When the flag at +2 is clear, copies the 16-byte vector at +0x50 to
   +0xC0 with retail's qcopy, optionally runs fast_vec_add on +0x60,
   then copies +0x60 to +0xD0. Sibling of func_001ECC10 just above it. */
void save_camera_vectors(void) __asm__("FUN_001ec7f0");

void save_camera_vectors(void) {
    char *camera_transition_state = cam_trans_state;
    if (*(unsigned char *)(camera_transition_state + 2) == 0) {
        qcopy(camera_transition_state + 0xC0, camera_transition_state + 0x50);
        if (*(unsigned char *)(camera_transition_state + 3) == 2) {
            fast_vec_add(
                camera_transition_state + 0xC0, D_0013F490, camera_transition_state + 0xC0);
        }
        qcopy(camera_transition_state + 0xD0, camera_transition_state + 0x60);
    }
}

extern __typeof__(save_camera_vectors) func_001EC7F0 __attribute__((alias("FUN_001ec7f0")));
