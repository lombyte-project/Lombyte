/* Level 05 moby class cutscene_camera_target (0x60e): points the cutscene camera at the moby of the current cutscene step. */
#include "types.h"
#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

/* UpdateMoby_1550 (0x60e). State 0 arms it: state 1 and byte 0x30 = 0xFF. In state 1,
 * while the level flag is 2 and the word at +0x30 of D_L05_0016CC60 is 10 or 11, passes one
 * entry of that table to cutscene_set_camera_target. The index select `s < t ? 0 : 3` is kept as retail
 * has it (it always gives 3 here since t < 2). The two state stores have to be written in
 * the order 0x30, 0x20 to come out as retail's. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0031EAD8), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x44];
    short n;
    char  pad1[0x132];
    char *m[1];
} G;

extern G D_L05_0016CC60;

void cutscene_camera_target_update(unsigned char *m) __asm__("FUN_L05_0031d5c8");

/* Moby class 1550 */
void cutscene_camera_target_update(unsigned char *m) {
    enum {
        INITIALIZE = 0,
        ARMED = 1
    };
    unsigned int object_state = m[0x20];
    unsigned int range_offset;
    G           *camera_target_table;
    switch (object_state) {
    case INITIALIZE:
        m[0x30] = 0xFF;
        m[0x20] = 1;
        break;
    case ARMED:
        if (D_L05_0015F5C4 == 2) {
            camera_target_table = &D_L05_0016CC60;
            range_offset = *(int *)((char *)camera_target_table + 0x30) - 10;
            if (range_offset < 2) {
                int k = 3;
                if (object_state < range_offset)
                    k = 0;
                cutscene_set_camera_target(camera_target_table->m[k]);
            }
        }
        break;
    }
}
