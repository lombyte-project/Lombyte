/* Shared moby class 0xEB: an effect that follows its parent, blends its colour and grows. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "qcopy.h"

extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void fast_add_rotations_cf(void *, void *, float, float, float) __asm__("FUN_L00_00262cf0");
void        mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

/* update: follow the parent, blend its color, scale by the wind factor */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002BC140), where it is exact; names translated to the US level program. */

void parent_following_effect_update(char *moby) __asm__("FUN_L03_002bae48");

/* Moby class 235 */
void parent_following_effect_update(char *moby) {
    char *moby_pvars = ((char *)MOBY(moby)->pvars);
    float ratio;
    float accumulated_offset;
    int   blended_color;
    int   g;
    int   b;
    if (fast_dec_timer_s16(moby_pvars + 0xE)) {
        mark_moby_for_removal(moby);
        return;
    }
    MOBY(moby)->rot.y = *(float *)(*(char **)moby_pvars + 0x44);
    MOBY(moby)->rot.z = *(float *)(*(char **)moby_pvars + 0x48);
    qcopy(&MOBY(moby)->pos, *(char **)(*(char **)moby_pvars + 0x78) + 0xF0);
    accumulated_offset = *(float *)(moby_pvars + 0x10) + *(float *)(moby_pvars + 0x14);
    *(float *)(moby_pvars + 0x10) = accumulated_offset;
    fast_add_rotations_cf(&MOBY(moby)->pos,
                          &MOBY(moby)->pos,
                          -accumulated_offset,
                          MOBY(moby)->rot.z,
                          MOBY(moby)->rot.y);
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, frame_time * 5.235987663269043f);
    ratio = convert_integer_to_float(*(short *)(moby_pvars + 0xE));
    ratio = ratio / convert_integer_to_float(*(short *)(moby_pvars + 0xC));
    blended_color = fast_tween_color(*(int *)(moby_pvars + 8), *(int *)(moby_pvars + 4), ratio);
    b = (blended_color >> 16) & 0xFF;
    g = (blended_color >> 8) & 0xFF;
    set_moby_color(moby, blended_color & 0xFF, g, b);
    MOBY(moby)->alpha = truncate_float_to_s32(ratio * 128.0f);
    MOBY(moby)->scale = MOBY(moby)->scale * (frame_scale * 0.019999981f + 1.0f);
}
