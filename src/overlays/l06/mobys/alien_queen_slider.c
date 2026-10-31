/* Level 06 moby class 0x16f: alien queen slider, moved out along its facing and back by the alien queen. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/overlay/l06/mobys/alien_queen_slider.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002DB0E0), where it is exact; names translated to the US level program. */
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
void       alien_queen_slider_update(unsigned char *moby) __asm__("FUN_L06_002d9d10");

#include "sda.h"

/* Moby class 367 */
void alien_queen_slider_update(unsigned char *moby) {
    enum {
        INITIALIZE_POSITION_ENDPOINTS = 0,
        INTERPOLATE_POSITION_TO_TARGET = 2
    };
    char *position_interpolation_vars;
    position_interpolation_vars = ((char *)MOBY(moby)->pvars);
    switch (moby[0x20]) {
    case INITIALIZE_POSITION_ENDPOINTS: {
        char *moby_position = (char *)moby + 0x10;
        char *offset_position_endpoint;
        qcopy(position_interpolation_vars, moby_position);
        offset_position_endpoint = position_interpolation_vars + 0x10;
        fast_vec_normalize(offset_position_endpoint, moby + 0xD0, 11.0f);
        fast_vec_add(offset_position_endpoint, offset_position_endpoint, moby_position);
        moby[0x20] = 1;
        break;
    }
    case INTERPOLATE_POSITION_TO_TARGET: {
        float blend_acceleration_step = (&frame_time)[1] * 10.0f;
        advance_accelerated_scalar((float *)(position_interpolation_vars + 0x20),
                                   (float *)(position_interpolation_vars + 0x24),
                                   *(float *)(position_interpolation_vars + 0x28),
                                   blend_acceleration_step,
                                   blend_acceleration_step,
                                   frame_time * 20.0f);
        lerp_vector(moby + 0x10,
                    position_interpolation_vars,
                    position_interpolation_vars + 0x10,
                    *(float *)(position_interpolation_vars + 0x20));
        if (*(float *)(position_interpolation_vars + 0x20) ==
            *(float *)(position_interpolation_vars + 0x28))
            moby[0x20] = 1;
        break;
    }
    }
}

extern int D_L06_001ABFC0[];
extern int D_L06_0015FFD8_d __asm__("D_L06_0015FFD8");

/* Sets each listed alien queen slider to state 2 with its float at +0x28 set to 1.0 (move out). */
/* 0x002d9e08, 108 bytes. Ported from rac1-decomp, where it is exact; names translated to the US level program. */
void set_listed_alien_queen_sliders_state_2_and_data_28_to_one(int index) __asm__("FUN_L06_002d9e08");

void set_listed_alien_queen_sliders_state_2_and_data_28_to_one(int index) {
    short *moby_index_cursor = D_L06_001ABFC0[index];
    if (moby_index_cursor != 0) {
        do {
            char *moby = (char *)(((*(unsigned short *)moby_index_cursor & 0x7FFF) << 8) +
                                  (int)D_L06_0015FFD8_d);
            if (MOBY(moby)->oclass == 0x16F) {
                moby[0x20] = 2;
                *(float *)(((char *)MOBY(moby)->pvars) + 0x28) = 1.0f;
            }
        } while (*moby_index_cursor++ >= 0);
    }
}

/* Sets each listed alien queen slider to state 2 with its word at +0x28 cleared (move back). */
/* 0x002d9e78, 100 bytes. Ported from rac1-decomp, where it is exact; names translated to the US level program. */
void set_listed_alien_queen_sliders_state_2_and_clear_data_28(int moby_list_index) __asm__("FUN_L06_002d9e78");

void set_listed_alien_queen_sliders_state_2_and_clear_data_28(int moby_list_index) {
    short *moby_index_cursor = D_L06_001ABFC0[moby_list_index];
    if (moby_index_cursor != 0) {
        do {
            char *moby = (char *)(((*moby_index_cursor & 0x7FFF) << 8) + (int)D_L06_0015FFD8_d);
            if (MOBY(moby)->oclass == 0x16F) {
                char *listed_moby_vars = ((char *)MOBY(moby)->pvars);
                moby[0x20] = 2;
                *(int *)(listed_moby_vars + 0x28) = 0;
            }
        } while (*moby_index_cursor++ >= 0);
    }
}
