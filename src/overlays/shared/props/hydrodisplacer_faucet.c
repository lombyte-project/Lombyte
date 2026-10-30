/* Shared moby class hydrodisplacer_faucet. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

extern short D_L05_0015FFD8;
extern void  update_hydrodisplacer_faucet_pulse_color(void *) __asm__("FUN_L05_002f81b8");

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern short faucet_base_color_gp __asm__("D_L05_001618C0") __attribute__((sda));
extern short faucet_item_16_pulse_start_color_gp __asm__("D_L05_001618C4") __attribute__((sda));
extern short faucet_item_16_pulse_end_color_gp __asm__("D_L05_001618C8") __attribute__((sda));
extern short faucet_fallback_pulse_start_color_gp __asm__("D_L05_001618CC") __attribute__((sda));
extern short faucet_fallback_pulse_end_color_gp __asm__("D_L05_001618D0") __attribute__((sda));
extern short D_0015EE6C_s __asm__("D_0015ED6C");

/* Moby update: plays an animation when its state and a flag allow, then calls the next stage. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F9478), where it is exact; names translated to the US level program. */

void hydrodisplacer_faucet_update(char *m) __asm__("FUN_L05_002f8080");

void hydrodisplacer_faucet_update(char *faucet_moby) {
    enum {
        HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_1 = 0,
        HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_2_WAIT = 1,
        HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_2 = 2,
        HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_1_WAIT = 3
    };
    struct Hero *player_hero = &hero;
    int          hero_on_faucet_in_mode_13 = 0;
    int         *faucet_vars = ((int *)MOBY(faucet_moby)->pvars);
    if (((char *)player_hero->ground_moby) == faucet_moby) {
        hero_on_faucet_in_mode_13 = player_hero->state.control_mode == 0x13;
    }
    switch ((unsigned char)faucet_moby[0x53]) {
    case HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_1:
        if (faucet_vars[0] >= 0 &&
            (*(unsigned char *)(*(int *)&D_L05_0015FFD8 + faucet_vars[0] * 256 + 0xBC) & 1) &&
            !hero_on_faucet_in_mode_13)
            blend_moby_animation(faucet_moby, 1, 0, 0);
        break;
    case HYDRODISPLACER_FAUCET_ANIM_WAIT_FOR_LINK_FLAG_2:
        if (faucet_vars[0] >= 0 &&
            (*(unsigned char *)(*(int *)&D_L05_0015FFD8 + faucet_vars[0] * 256 + 0xBC) & 2) &&
            !hero_on_faucet_in_mode_13)
            blend_moby_animation(faucet_moby, 3, 0, 0);
        break;
    case HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_1_WAIT:
        if (faucet_moby[0x70] & 2)
            blend_moby_animation(faucet_moby, 0, 0, 0);
        break;
    case HYDRODISPLACER_FAUCET_ANIM_TRANSITION_TO_LINK_FLAG_2_WAIT:
        if (faucet_moby[0x70] & 2)
            blend_moby_animation(faucet_moby, 2, 0, 0);
        break;
    }
    update_hydrodisplacer_faucet_pulse_color(faucet_moby);
}

/* eases the pulse level of the moby that holds the focus and mixes its colour from two palettes */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F95B0), where it is exact; names translated to the US level program. */

void update_hydrodisplacer_faucet_pulse_color(void *mv) {
    struct Moby *moby = mv;
    struct Hero *base = &hero;
    char        *data = ((char *)moby->pvars);
    int          pulse_color;
    float        pulse_phase;
    if (((char *)base->ground_moby) == moby) {
        approach_value((float *)(data + 0x1C), 1.0f, frame_time + frame_time);
        if (((int)base->items[0].moby) != 0) {
            if (base->items[0].item_id == 0x16)
                ((unsigned char *)moby)[0xBC] = 1;
            else
                ((unsigned char *)moby)[0xBC] = 0;
        } else {
            ((unsigned char *)moby)[0xBC] = 0;
        }
    } else {
        approach_value((float *)(data + 0x1C), 0.0f, *(float *)&D_0015EE6C_s);
    }
    if (*(float *)(data + 0x1C) != 0.0f) {
        pulse_phase = fast_add_rotations(*(float *)(data + 0x18), frame_time * 6.2831855f);
        *(float *)(data + 0x18) = pulse_phase;
        if (((unsigned char *)moby)[0xBC] == 1) {
            pulse_color = fast_tween_color(*(int *)&faucet_item_16_pulse_start_color_gp,
                                           *(int *)&faucet_item_16_pulse_end_color_gp,
                                           (fast_sin(pulse_phase) + 1.0f) * 0.5f);
        } else {
            pulse_color = fast_tween_color(*(int *)&faucet_fallback_pulse_start_color_gp,
                                           *(int *)&faucet_fallback_pulse_end_color_gp,
                                           (fast_sin(pulse_phase) + 1.0f) * 0.5f);
        }
        moby->glow_color =
            fast_tween_color(*(int *)&faucet_base_color_gp, pulse_color, *(float *)(data + 0x1C));
    } else {
        moby->glow_color = *(int *)&faucet_base_color_gp;
    }
}

short D_L07_00161870 = -32640;
short D_L07_00161874 = -32640;
short D_L07_00161878 = -16288;
short D_L07_0016187C = -32640;
short D_L07_00161880 = 0x60c0;
short D_L11_00161944 = -32640;
short D_L11_00161948 = -32640;
short D_L11_0016194C = -16288;
short D_L11_00161950 = -32640;
short D_L11_00161954 = 0x60c0;
short D_L12_001619B8 = -32640;
short D_L12_001619BC = -32640;
short D_L12_001619C0 = -16288;
short D_L12_001619C4 = -32640;
short D_L12_001619C8 = 0x60c0;
short D_L18_00161910 = -32640;
short D_L18_00161914 = -32640;
short D_L18_00161918 = -16288;
short D_L18_0016191C = -32640;
short D_L18_00161920 = 0x60c0;

short faucet_base_color_gp __asm__("D_L05_001618C0") = -32640;
