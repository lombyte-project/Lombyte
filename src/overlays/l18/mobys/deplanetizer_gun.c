/* Level 18 moby class deplanetizer_gun (0x59a and 0x59b): during the deplanetizer cutscene flips over and mirrors its height, and turns back when it ends. */
#include "types.h"
#include "qcopy.h"
#include "sda.h"

extern int   D_L18_0015F5C4;
extern int   D_L18_0016D290_lui __asm__("D_L18_0016D290") NOT_SDA;
extern int   game_frame __asm__("D_L18_0015F5CC");
extern float fast_add_rotations(float, float);

extern int D_L18_00162384_gp __asm__("D_L18_00162384") __attribute__((sda));
extern int D_L18_001623A0_gp __asm__("D_L18_001623A0") __attribute__((sda));

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F8F38), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x18];
    float          f18;
    char           pad1C[4];
    unsigned char  state;
    char           pad21[0x10];
    unsigned char  substate;
    char           pad32[2];
    unsigned short flags;
    char           pad36[0x0A];
    float          f40;
} DeplanetizerGunSceneMoby;

void deplanetizer_gun_update(DeplanetizerGunSceneMoby *moby) __asm__("FUN_L18_002f7ad8");

/* Moby classes 1434, 1435 */
void deplanetizer_gun_update(DeplanetizerGunSceneMoby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_WAIT_FOR_SCENE_TRIGGER = 1,
        MOBY_WAIT_FOR_SCENE_END = 2
    };
    switch (moby->state) {
    case MOBY_INITIALIZE:
        moby->state = 1;
    case MOBY_WAIT_FOR_SCENE_TRIGGER:
        if ((D_L18_0015F5C4 == 2 && D_L18_0016D290_lui == 3) ||
            (game_frame < 5 && D_L18_001623A0_gp != 0 && moby->f40 == 0.0f)) {
            moby->state = 2;
            moby->flags |= 1;
            moby->substate = 0;
        }
        break;
    case MOBY_WAIT_FOR_SCENE_END:
        /* retail reaches D_L18_001623A0 here as the word 0x1C past D_L18_00162384 */
        if ((&D_L18_00162384_gp)[7] != 0 && moby->f40 == 0.0f) {
            float height_reflection_delta = 101.6f - moby->f18;
            moby->f40 = fast_add_rotations(moby->f40, 3.14159274f);
            moby->f18 += height_reflection_delta + height_reflection_delta;
        }
        if (D_L18_0015F5C4 != 2) {
            moby->substate = 1;
            moby->state = 1;
            moby->flags &= 0xFFFE;
        }
        break;
    }
}
