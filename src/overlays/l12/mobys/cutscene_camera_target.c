/* Level 12 moby class cutscene_camera_target (class 1557): points the cutscene camera at the moby the current phase selects. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"

typedef struct Moby Moby;
extern int          D_L12_0015F5C4;

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030A880). */
typedef struct {
    char  pad0[0x30];
    int   phase;
    char  pad34[0x144];
    char *mobys[4];
} L12CutsceneState;

extern L12CutsceneState D_L12_0016CCE0;
extern void             cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

static inline unsigned char camera_target_index(int phase) {
    if (phase == 1)
        return 2;
    return 0;
}

void update_cutscene_camera_target(struct Moby *moby) __asm__("FUN_L12_003094a0");

void update_cutscene_camera_target(struct Moby *moby) {
    enum {
        INITIALIZE_UPDATE_DISTANCE = 0,
        DISPATCH_PHASE_SELECTED_MOBY = 1
    };
    L12CutsceneState *list;
    L12CutsceneState *target_list;
    int               phase;
    int               index;
    int               never;
    int               held;

    /* Fakematch: the dead call guarded by never keeps retail's branch shape. */
    never = 0;
    switch (moby->state) {
    case INITIALIZE_UPDATE_DISTANCE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case DISPATCH_PHASE_SELECTED_MOBY:
        if (D_L12_0015F5C4 == 2) {
            list = &D_L12_0016CCE0;
            phase = list->phase;
            if (phase == 1 || phase == 7) {
                /* Fakematch: see never. */
                held = phase + 1;
                if (never)
                    cutscene_set_camera_target(held);
                index = camera_target_index(list->phase);
                target_list = &D_L12_0016CCE0;
                cutscene_set_camera_target((int)target_list->mobys[index]);
            }
        }
        break;
    }
}
