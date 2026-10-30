/* Level 15 moby class that points the cutscene camera at a mission moby picked by the mission mode. */
#include "types.h"
#include "rnc/mobys/moby.h"

extern int D_L15_0015F5C4;

struct L15MissionState {
    char pad0[0x30];
    int  mode;
    char pad1[0x178 - 0x34];
    int  slot[5];
};

extern struct L15MissionState mission_2edb20 __asm__("D_L15_0016CE60");

extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void update_cutscene_camera_target(struct Moby *moby) __asm__("FUN_L15_002edb20");

void update_cutscene_camera_target(struct Moby *moby) {
    enum {
        INITIALIZE_UPDATE_DISTANCE = 0,
        DISPATCH_MODE_SELECTED_SLOT = 1
    };
    switch (moby->state) {
    case INITIALIZE_UPDATE_DISTANCE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case DISPATCH_MODE_SELECTED_SLOT:
        if (D_L15_0015F5C4 == 2) {
            if (mission_2edb20.mode == 1 || mission_2edb20.mode == 2 || mission_2edb20.mode == 3 ||
                mission_2edb20.mode == 4 || mission_2edb20.mode == 6) {
                int mission_mode = mission_2edb20.mode;
                int i;
                i = 0;
                if (mission_mode == 1)
                    i = 4;
                else if (mission_mode == 2 || mission_mode == 3 || mission_mode == 4)
                    i = 2;
                else if (mission_mode == 6)
                    i = 1;
                cutscene_set_camera_target(mission_2edb20.slot[i]);
            }
        }
        break;
    }
}
