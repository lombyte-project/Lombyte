/* Level 13 moby class (0x616) that points the cutscene camera at the cutscene controller's target. */
#include "types.h"
#include "rnc/mobys/moby.h"

extern int D_L13_0015F5C4;

typedef struct {
    char pad[0x30];
    int  state;
    char pad34[0x14C];
    int  target;
} L13Action;

extern L13Action l13_cutscene_target_effect_controller __asm__("D_L13_0016CB60");

extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void cutscene_camera_target_update(struct Moby *moby) __asm__("FUN_L13_0030bdf0");

/* Moby class 1558 */
void cutscene_camera_target_update(struct Moby *moby) {
    enum {
        INITIALIZE_UPDATE_DISTANCE = 0,
        UPDATE_CUTSCENE_CAMERA_TARGET = 1
    };
    switch (moby->state) {
    case INITIALIZE_UPDATE_DISTANCE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case UPDATE_CUTSCENE_CAMERA_TARGET:
        if (D_L13_0015F5C4 == 2 && l13_cutscene_target_effect_controller.state == 2) {
            cutscene_set_camera_target(l13_cutscene_target_effect_controller.target);
        }
        break;
    }
}
