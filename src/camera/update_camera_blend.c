/* Ported from rac1-decomp (src/game/camera.c, func_001ED658). */

#include "sda.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"

#include "rnc/camera/update_cam.h"

extern int step_camera_blend(void *, void *) __asm__("func_001ECAF8");
extern int advance_alternate_camera_transition(void *, void *) __asm__("FUN_001eccd8");

/* Picks the update path by the flag at D_001871B0+2 (step_camera_blend when
   clear, advance_alternate_camera_transition when set), each passed arg0 and a slot inside
   D_001871B0. On success, unless camera_position_publication_suppressed is set, copies four 16-byte
   vectors from arg0 into D_00187290's block (retail's qcopy); the last
   destination is -0x210 from D_00187290, a different member reached by
   pointer arithmetic on the same char array. Either way it clears
   D_001871B0's leading halfword and its flag byte. */
void update_camera_blend(char *to_cam) __asm__("FUN_001ed2b0");

void update_camera_blend(char *to_cam) {
    char *camera_transition_state = cam_trans_state;
    int   transition_complete;

    if (*(unsigned char *)(camera_transition_state + 2) == 0) {
        transition_complete = step_camera_blend(to_cam, camera_transition_state + 0x10);
    } else {
        transition_complete =
            advance_alternate_camera_transition(to_cam, camera_transition_state + 0x70);
    }
    if (transition_complete != 0) {
        if (camera_position_publication_suppressed[0] == 0) {
            qcopy(camera_rows, to_cam);
            qcopy(camera_rows + 0x10, to_cam + 0x10);
            qcopy(camera_rows + 0x20, to_cam + 0x20);
            qcopy(camera_rows - 0x210, to_cam + 0x30);
        }
        *(short *)camera_transition_state = 0;
        camera_transition_state[2] = 0;
    }
}

extern __typeof__(update_camera_blend) func_001ED2B0 __attribute__((alias("FUN_001ed2b0")));
