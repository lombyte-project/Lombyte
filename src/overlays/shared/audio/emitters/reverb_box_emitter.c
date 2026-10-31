/* Sound emitter type 3 (reverb box): while the hero is inside the emitter box, requests its state scaled by the hero position along the box; on leaving, sets it by the side left through. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

/* reverb box sound update: starts or stops the sound as the listener enters or leaves the box */
/* Ported from rac1-decomp (src/overlays/shared/vendor_0031AD00.c: func_L01_0031B2F0), where it is exact; names translated to the US level program. */

extern void
set_requested_state_and_mark_changes(int a, int b, int c, int d, int e) __asm__("FUN_L01_002a1a90");

void update_reverb_box_emitter(char *moby) __asm__("FUN_L01_00319f18");

void update_reverb_box_emitter(char *moby) {
    float          hero_offset_from_box[4];
    float          hero_offset_in_box_basis[4];
    unsigned char *box_state_request = *(unsigned char **)(moby + 8);
    fast_vec_sub(hero_offset_from_box, &hero.motion.pos, moby + 0x40);
    hero_offset_from_box[3] = 0;
    transform_vector_by_basis(hero_offset_in_box_basis, hero_offset_from_box, moby + 0x50);
    if (absolute_float(hero_offset_in_box_basis[0]) <= 1.0f &&
        absolute_float(hero_offset_in_box_basis[1]) <= 1.0f &&
        absolute_float(hero_offset_in_box_basis[2]) <= 1.0f) {
        float box_x_fraction = (hero_offset_in_box_basis[0] + 1.0f) * 0.5f;
        int   scaled_state_value = truncate_float_to_s32(
            convert_integer_to_float(*(int *)(box_state_request + 4)) * box_x_fraction);
        int clamped_state_value = *(int *)(box_state_request + 4);
        if (!(clamped_state_value < scaled_state_value))
            clamped_state_value = scaled_state_value;
        set_requested_state_and_mark_changes((int)moby,
                                             box_state_request[0],
                                             clamped_state_value,
                                             box_state_request[1],
                                             box_state_request[2]);
        box_state_request[3] = 1;
    } else if (box_state_request[3] != 0) {
        if (hero_offset_in_box_basis[0] > 0.0f) {
            set_requested_state_and_mark_changes(0,
                                                 box_state_request[0],
                                                 *(int *)(box_state_request + 4),
                                                 box_state_request[1],
                                                 box_state_request[2]);
        } else {
            set_requested_state_and_mark_changes((int)moby, 0, 0, 0, 0);
        }
        box_state_request[3] = 0;
    }
}
