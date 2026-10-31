/* Level 06 moby classes 0x443-0x44a, 0x44f: breakaway pieces; they bob in place until the shuttle breaks them apart. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00302248), where it is exact; names translated to the US level program. */
extern char D_0013E533[];
float       moby_bobbing_amplitude_gp __asm__("D_L06_00162090") = 3.0f;
extern int  GB_FRAME_gp __asm__("D_L06_0015F5CC") __attribute__((sda));
void        breakaway_piece_update(char *moby) __asm__("FUN_L06_00300df0");

/* Moby classes 1091-1098, 1103 */
void breakaway_piece_update(char *moby) {
    enum {
        INITIALIZE_BOBBING_HEIGHT = 0,
        UPDATE_BOBBING_HEIGHT = 1,
        UPDATE_LINEAR_AND_ANGULAR_MOTION = 2
    };
    char *bobbing_and_motion_vars = ((char *)MOBY(moby)->pvars);
    int   state;
    if (bobbing_and_motion_vars == 0) {
        mark_moby_for_removal(moby);
        return;
    }
    state = (unsigned char)moby[0x20];
    switch (state) {
    case INITIALIZE_BOBBING_HEIGHT:
        MOBY(moby)->draw_dist = 0x200;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(float *)(bobbing_and_motion_vars + 0xC) = MOBY(moby)->pos.z;
        moby[0x20] = 1;
        break;
    case UPDATE_BOBBING_HEIGHT:
        if (*(int *)(D_0013E533 + 0x2EA1) == 0x72) {
            MOBY(moby)->pos.z = *(float *)(bobbing_and_motion_vars + 0xC);
        } else {
            float bobbing_cycle_fraction = convert_integer_to_float(GB_FRAME_gp % 600) / 600.0f;
            float bobbing_sine_factor = fast_sin(bobbing_cycle_fraction * 6.28318f);
            MOBY(moby)->pos.z = *(float *)(bobbing_and_motion_vars + 0xC) +
                                moby_bobbing_amplitude_gp * bobbing_sine_factor;
        }
        break;
    case UPDATE_LINEAR_AND_ANGULAR_MOTION:
        if (*(float *)(bobbing_and_motion_vars + 0xC) > 1.0f)
            fast_vec_scale(bobbing_and_motion_vars, bobbing_and_motion_vars, 0.985f);
        fast_vec_add(moby + 0x10, moby + 0x10, bobbing_and_motion_vars);
        fast_vec_add(moby + 0x40, moby + 0x40, bobbing_and_motion_vars + 0x10);
        if (*(int *)(D_0013E533 + 0x2EA1) != 0x72)
            mark_moby_for_removal(moby);
        break;
    }
}
