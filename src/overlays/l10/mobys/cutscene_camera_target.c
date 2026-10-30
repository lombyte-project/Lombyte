/* Level 10 moby class cutscene_camera_target (class 1555): points the cutscene camera at the moby the current level mode selects. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_globals.h"

/* update: wait for state 1, then act on the level's current mode */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002EC098), where it is exact; names translated to the US level program. */

struct L10Data {
    char pad0[0x30];
    int  mode;
    char pad1[0x178 - 0x34];
    int  slot[4];
};

extern struct L10Data l10_level_mode_dispatch_data __asm__("D_L10_0016CCE0");
extern void           cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void update_cutscene_camera_target(struct Moby *moby) __asm__("FUN_L10_002eacd8");

void update_cutscene_camera_target(struct Moby *moby) {
    enum {
        LEVEL_MODE_STATE_INITIALIZE = 0,
        LEVEL_MODE_STATE_DISPATCH = 1
    };
    switch (moby->state) {
    case LEVEL_MODE_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case LEVEL_MODE_STATE_DISPATCH:
        if (D_L10_0015F5C4_vendor == 2) {
            if (l10_level_mode_dispatch_data.mode == 3 || l10_level_mode_dispatch_data.mode == 5 ||
                l10_level_mode_dispatch_data.mode == 6 || l10_level_mode_dispatch_data.mode == 7) {
                int level_mode = l10_level_mode_dispatch_data.mode;
                int i;
                i = 0;
                if (level_mode == 3)
                    i = 1;
                else if (level_mode == 5 || level_mode == 6)
                    i = 2;
                else if (level_mode == 7)
                    i = 3;
                cutscene_set_camera_target(l10_level_mode_dispatch_data.slot[i]);
            }
        }
        break;
    }
}
