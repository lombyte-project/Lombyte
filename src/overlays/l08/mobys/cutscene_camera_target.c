/* Level 08 moby class 0x611: cutscene camera target; aims the cutscene camera at the moby of the current cutscene stage. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"

extern int level_game_mode __asm__("D_L08_0015F5C4");

#include "rnc/game/item_state.h"

/* In game mode 2, aims the cutscene camera at the moby picked by the current cutscene stage. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_00309998), where it is exact; names translated to the US level program. */
struct G08c {
    char  pad0[0x30];
    int   a;
    int   b;
    char  pad1[0xC];
    short n;
    char  pad2[0x132];
    char *objs[1];
};

extern struct G08c D_L08_0016D060_c __asm__("D_L08_0016D060");
extern void        cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");
void               update_cutscene_camera_target(struct Moby *moby) __asm__("FUN_L08_003084d8");

void update_cutscene_camera_target(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_ACTIVE = 1
    };
    int k;
    switch (moby->state) {
    case MOBY_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_ACTIVE:
        if (level_game_mode == 2) {
            int object_selector = D_L08_0016D060_c.a;
            if ((object_selector >= 1 && object_selector <= 2) || object_selector == 4 ||
                object_selector == 5) {
                if (D_L08_0016D060_c.a != 1 || scale_game_frames(0x8CE) < D_L08_0016D060_c.b) {
                    k = 0;
                    if (D_L08_0016D060_c.a == 1 || D_L08_0016D060_c.a == 4)
                        k = 3;
                    else if (D_L08_0016D060_c.a == 2 || D_L08_0016D060_c.a == 5)
                        k = 2;
                    cutscene_set_camera_target((int)D_L08_0016D060_c.objs[k]);
                }
            }
        }
        break;
    }
}
