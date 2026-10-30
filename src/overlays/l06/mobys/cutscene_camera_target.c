/* Level 06 moby class 0x60f: points the cutscene camera at the cutscene controller's target. */
#include "types.h"
#include "asm.h"

#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_0030A788), where it is exact; names translated to the US level program. */

extern char level06_camera_target_update_data[] __asm__("D_L06_0016CFE0");
extern int  D_L06_0015F5C4;
extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void update_cutscene_camera_target(struct Moby *moby) __asm__("FUN_L06_00309348");

void update_cutscene_camera_target(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_CONDITIONAL_CAMERA_TARGET_UPDATE = 1
    };
    switch (moby->state) {
    case MOBY_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_CONDITIONAL_CAMERA_TARGET_UPDATE:
        if (D_L06_0015F5C4 == 2) {
            char *camera_target_update_data = level06_camera_target_update_data;
            if (*(int *)(camera_target_update_data + 0x30) == 5) {
                if (scale_game_frames(0x7F) >= *(int *)(camera_target_update_data + 0x34)) {
                    cutscene_set_camera_target(*(int *)(camera_target_update_data + 0x180));
                }
            }
        }
        break;
    }
}
