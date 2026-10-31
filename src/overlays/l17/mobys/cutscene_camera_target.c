/* Level 17 moby class cutscene_camera_target (0x61a): points the cutscene camera at the moby of the current cutscene step. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F49B8), where it is exact; names translated to the US level program. */
extern void cutscene_set_camera_target() __asm__("FUN_L00_002637f8");

extern struct {
    int pad0[12];
    int v;
    int pad1[81];
    int tbl[3];
} D_L17_0016D160;

void cutscene_camera_target_update(struct Moby *moby) __asm__("FUN_L17_002f2e78");

/* Moby class 1562 */
void cutscene_camera_target_update(struct Moby *moby) {
    enum {
        MOBY_UPDATE_DISTANCE_INITIALIZE = 0,
        MOBY_GUARDED_TABLE_CALL = 1
    };
    switch (moby->state) {
    case MOBY_UPDATE_DISTANCE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_GUARDED_TABLE_CALL:
        if (D_L17_0015F5C4 == 2) {
            int v = D_L17_0016D160.v;
            int i;
            if (v != 0 && v != D_L17_0015F5C4)
                return;
            i = 0;
            if (v == 0 || v == D_L17_0015F5C4)
                i = 2;
            cutscene_set_camera_target(D_L17_0016D160.tbl[i]);
        }
        break;
    }
}
