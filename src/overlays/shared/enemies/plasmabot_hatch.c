/* Shared moby class plasmabot_hatch: the hatch a plasmabot comes out of, and its reset delay the plasmabot starts. */
#include "types.h"
#include "rnc/overlay/shared/enemies/plasmabot_hatch.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/entities.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "qcopy.h"

extern u8 *D_L14_00167500 NOT_SDA;
extern char              *D_L14_ffd0_e_macro[] __asm__("D_L14_0015FFD0") MACRO_ADDR;

void plasmabot_hatch_update(struct Moby *moby) __asm__("FUN_L14_002d96e0");

/* Moby class 250 */
void plasmabot_hatch_update(struct Moby *moby) {
    enum {
        HATCH_WAIT_ACTIVATION = 0,
        HATCH_WAIT_ACTIVATION_DELAY = 1,
        HATCH_WAIT_ACTIVATION_ANIMATION = 2,
        HATCH_WAIT_ACTIVATION_GATE_CLEAR = 3,
        HATCH_IDLE_AFTER_ACTIVATION_GATE_CLEAR = 4,
        HATCH_WAIT_ANIMATION_BEFORE_RESET_DELAY = 5,
        HATCH_WAIT_RESET_DELAY = 6,
        HATCH_WAIT_RESET_ANIMATION = 7
    };
    char *hatch_private_vars = (char *)moby->pvars;
    if (hero.vehicle_mode == 2) {
        moby->collision = 0;
    } else {
        moby->collision = moby->pclass->collision;
    }
    switch (moby->state) {
    case HATCH_WAIT_ACTIVATION:
        if (*(s32 *)hatch_private_vars != 0 && *(s16 *)(D_L14_00167500 + 0x86) == 0x14) {
            moby->state = 1;
            *(s32 *)(hatch_private_vars + 8) = scale_game_frames(*(s32 *)(hatch_private_vars + 4));
        }
        break;
    case HATCH_WAIT_ACTIVATION_DELAY:
        if (fast_dec_timer(hatch_private_vars + 8) != 0) {
            moby->state = 2;
            set_moby_animation(moby, 1, 0);
        }
        break;
    case HATCH_WAIT_ACTIVATION_ANIMATION:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 3;
            set_moby_animation(moby, 2, 0);
        }
        break;
    case HATCH_WAIT_ACTIVATION_GATE_CLEAR:
        if (*(s16 *)(D_L14_00167500 + 0x86) != 0x14) {
            set_moby_animation(moby, 0, 0);
            moby->state = 4;
        }
        break;
    case HATCH_IDLE_AFTER_ACTIVATION_GATE_CLEAR:
        break;
    case HATCH_WAIT_ANIMATION_BEFORE_RESET_DELAY:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 6;
            set_moby_animation(moby, 2, 0);
        }
        break;
    case HATCH_WAIT_RESET_DELAY:
        if (fast_dec_timer(hatch_private_vars + 8) != 0) {
            set_moby_animation(moby, 3, 0);
            moby->state = 7;
        }
        break;
    case HATCH_WAIT_RESET_ANIMATION:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 0;
            set_moby_animation(moby, 0, 0);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002DAC00), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

int refresh_plasmabot_hatch_reset_delay(int i) __asm__("FUN_L14_002d9870");

int refresh_plasmabot_hatch_reset_delay(int i) {
    struct Moby *moby = (unsigned char *)(D_L14_ffd0_e_macro[2] + i * 256);
    int         *hatch_vars = ((int *)moby->pvars);
    int          hatch_reset_delay_active = 0;
    if (moby->state == 0) {
        moby->state = 5;
        set_moby_animation(moby, 1, 0);
    } else {
        hatch_reset_delay_active = moby->state == 6;
    }
    hatch_vars[2] = scale_game_frames(0x3C);
    return hatch_reset_delay_active;
}
