/* Level 10 moby class gate_2: the second of the paired gates. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/l10/mobys/gate_state.h"
#include "rnc/game/item_state.h"

typedef struct Moby Moby;

#include "rnc/overlay/quad.h"

extern float         FUN_L04_001f1978(float);
extern float         fast_cos(float);
extern float         fast_sin(float);
extern unsigned char D_001413F4[];
extern int           D_L10_001BA950[] __asm__("D_L10_001BA950");
extern void          activate_descriptor(int) __asm__("FUN_L10_002f5a50");
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002EAD50), where it is exact; names translated to the US level program. */

typedef OvlQuad Vec;

typedef struct {
    Vec  home;  /* 0x00 */
    int  sound; /* 0x10 */
    int  timer; /* 0x14 */
    char pad18[0x4];
    int  door; /* 0x1C: index of the linked moby */
} GateVars;

typedef struct {
    char           pad00[0x10];
    Vec            pos;   /* 0x10 */
    unsigned char  state; /* 0x20 */
    char           pad21[0xF];
    unsigned char  f30; /* 0x30 */
    char           pad31[0x3];
    unsigned short flags; /* 0x34 */
    char           pad36[0x12];
    float          rotz; /* 0x48 */
    char           pad4C[0x2C];
    GateVars      *vars; /* 0x78 */
    char           pad7C[0x36];
    unsigned short uid; /* 0xB2 */
    char           padB4[0x4C];
} GateMoby; /* no foreign declaration */

extern struct Moby  *D_L10_0015FFD8_gp __asm__("D_L10_0015FFD8") __attribute__((sda));
void                 activate_descriptor(int i) __asm__("FUN_L10_002f5a50");
extern unsigned char D_0013DE4B_2EAD50[] __asm__("D_0013DD4C");

void gate_2_update(GateMoby *m) __asm__("FUN_L10_002e9990");

/* Moby class 1424 */
void gate_2_update(GateMoby *moby) {
    GateVars     *vars = moby->vars;
    float         opening_offset[3];
    Vec           opening_position;
    unsigned char entry_gate_state = moby->state;

    switch (entry_gate_state) {
    case GATE_STATE_INITIALIZE:
        moby->f30 = 0xFF;
        vars->home = moby->pos;
        if (D_001413F4[0] != 0 || l10_save_state.cleared[(short)moby->uid] != 0 ||
            ((*(int *)((unsigned char *)D_0014C190_257218 +
                       ((((short)moby->uid >> 5) * 4) + (current_level_index << 8))) >>
              (moby->uid & 0x1F)) &
             1) != 0) {
            opening_offset[0] = fast_cos(fast_add_rotations(moby->rotz, 3.14159265f)) * 1.5f;
            opening_offset[1] = fast_sin(fast_add_rotations(moby->rotz, 3.14159265f)) * 1.5f;
            opening_offset[2] = 0.0f;
            fast_vec_add(&opening_position, &vars->home, opening_offset);
            moby->state = 3;
            moby->pos = opening_position;
        } else {
            if (vars->door > 0) {
                (*(GateMoby **)&D_L10_0015FFD8_gp)[vars->door].flags |= 2;
            }
            moby->state = 1;
        }
        break;
    case GATE_STATE_CLOSED: {
        unsigned char *bits;
        if (D_001413F4[0] != 0 || l10_save_state.cleared[(short)moby->uid] != 0 ||
            ((*(int *)((bits = (char *)D_0014C190_257218) +
                       ((((short)moby->uid >> 5) * 4) + (current_level_index << 8))) >>
              (moby->uid & 0x1F)) &
             1) != 0) {
            moby->pos = vars->home;
            moby->state = 3;
            if (vars->door > 0) {
                (*(GateMoby **)&D_L10_0015FFD8_gp)[vars->door].flags &= 0xFFFD;
            }
        } else if (D_0013DE4B_2EAD50[0] != 0) {
            if (vars->door > 0) {
                (*(GateMoby **)&D_L10_0015FFD8_gp)[vars->door].flags &= 0xFFFD;
            }
            *(int *)(bits + ((((short)moby->uid >> 5) * 4) + (current_level_index << 8))) |=
                entry_gate_state << (moby->uid & 0x1F);
            D_L10_001BA950[(short)moby->uid >> 5] |= entry_gate_state << (moby->uid & 0x1F);
            if (vars->sound > 0) {
                activate_descriptor(vars->sound);
            }
            vars->timer = truncate_float_to_s32(FUN_L04_001f1978(90.0f));
            moby->state = 2;
        }
        break;
    }
    case GATE_STATE_OPENING:
        opening_offset[0] =
            fast_cos(fast_add_rotations(moby->rotz, 3.14159265f)) * 1.5f *
            convert_integer_to_float(truncate_float_to_s32(FUN_L04_001f1978(90.0f)) - vars->timer) /
            FUN_L04_001f1978(90.0f);
        opening_offset[1] =
            fast_sin(fast_add_rotations(moby->rotz, 3.14159265f)) * 1.5f *
            convert_integer_to_float(truncate_float_to_s32(FUN_L04_001f1978(90.0f)) - vars->timer) /
            FUN_L04_001f1978(90.0f);
        opening_offset[2] = 0.0f;
        fast_vec_add(&opening_position, &vars->home, opening_offset);
        moby->pos = opening_position;
        if (fast_dec_timer(&vars->timer)) {
            moby->state = 3;
        }
        break;
    case GATE_STATE_OPEN:
        if (D_001413F4[0] == 0 && l10_save_state.cleared[(short)moby->uid] == 0 &&
            (((*(int *)((unsigned char *)D_0014C190_257218 +
                        ((((short)moby->uid >> 5) * 4) + (current_level_index << 8))) >>
               (moby->uid & 0x1F)) ^
              1) &
             1)) {
            moby->pos = vars->home;
            moby->state = 0;
        }
        break;
    }
}
