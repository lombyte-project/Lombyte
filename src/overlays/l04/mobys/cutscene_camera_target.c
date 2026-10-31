/* Level 04 moby class cutscene_camera_target (0x60d): points the cutscene camera at the moby of the current cutscene mode. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/game/game_globals.h"

/* Waits for state 1, then passes the slot for the level's current mode to cutscene_set_camera_target. */
struct L04MissionState {
    char pad0[0x30];
    int  mode;
    char pad1[0x178 - 0x34];
    int  slot[4];
};

extern int                    D_L04_0015F5C4;
extern struct L04MissionState l04_mode_slot_dispatch_state __asm__("D_L04_0016C9E0");

extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void cutscene_camera_target_update(struct Moby *moby) __asm__("FUN_L04_002e53a0");

/* Moby class 1549 */
void cutscene_camera_target_update(struct Moby *moby) {
    enum {
        MODE_SLOT_STATE_INITIALIZE = 0,
        MODE_SLOT_STATE_DISPATCH_SELECTED_SLOT = 1
    };
    switch (moby->state) {
    case MODE_SLOT_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MODE_SLOT_STATE_DISPATCH_SELECTED_SLOT:
        if (D_L04_0015F5C4 == 2) {
            if (l04_mode_slot_dispatch_state.mode == 0 || l04_mode_slot_dispatch_state.mode == 1) {
                int mission_mode = l04_mode_slot_dispatch_state.mode;
                int i;
                i = 0;
                if (mission_mode == 0)
                    i = 3;
                else if (mission_mode == 1)
                    i = 2;
                cutscene_set_camera_target(l04_mode_slot_dispatch_state.slot[i]);
            }
        }
        break;
    }
}
