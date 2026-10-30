/* Shared moby class 0x1AC: a feather that falls and swings, then fades out. */
#include "types.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/math/fast_math.h"

extern float D_0015ED70_lui __asm__("D_0015ED70");

void         FUN_001f9a10_2cc210(void *, void *, void *) __asm__("FUN_001f9a10");
int          FUN_001f96f8_2cc210(int) __asm__("FUN_001f96f8");
extern float falling_moby_horizontal_velocity_scale_gp __asm__("D_L00_001618A8")
    __attribute__((sda));

void update_damped_falling_moby(unsigned char *updated_moby) __asm__("FUN_L00_002cc210");

void update_damped_falling_moby(unsigned char *updated_moby) {
    float *moby_vars = *(float **)(updated_moby + 0x78);
    float  sc;
    FUN_001f9a10_2cc210(updated_moby + 0x10, updated_moby + 0x10, moby_vars);
    moby_vars[2] -= D_0015ED70_lui * 15.0f;
    sc = falling_moby_horizontal_velocity_scale_gp;
    moby_vars[0] *= sc;
    moby_vars[1] *= sc;
    if (moby_vars[2] < moby_vars[8])
        moby_vars[2] = moby_vars[8];
    *(float *)(updated_moby + 0x48) =
        fast_add_rotations(*(float *)(updated_moby + 0x48), moby_vars[10]);
    *(float *)(updated_moby + 0x40) = fast_add_rotations(
        *(float *)(updated_moby + 0x40), moby_vars[11] * (float)((int *)moby_vars)[9]);
    if (0.7853982f < absolute_float(*(float *)(updated_moby + 0x40))) {
        ((int *)moby_vars)[9] = -((int *)moby_vars)[9];
    }
    if (fast_dec_timer(&moby_vars[7]) != 0 || updated_moby[0x31] == 0) {
        mark_moby_for_removal(updated_moby);
    } else if (((int *)moby_vars)[7] < FUN_001f96f8_2cc210(0x1E) &&
               fast_subtract_rotations_cf(updated_moby + 0x23)) {
        mark_moby_for_removal(updated_moby);
    }
}

float D_L01_001618E0 = 0.97f;
float D_L02_00161878 = 0.97f;
float D_L03_00161888 = 0.97f;
float D_L04_001618E8 = 0.97f;
float D_L05_001618DC = 0.97f;
float D_L06_001618C8 = 0.97f;
float D_L07_0016188C = 0.97f;
float D_L08_001619E0 = 0.97f;
float D_L09_00161958 = 0.97f;
float D_L10_001618B4 = 0.97f;
float D_L11_00161960 = 0.97f;
float D_L12_00161A38 = 0.97f;
float D_L13_00161980 = 0.97f;
float D_L14_00161B54 = 0.97f;
float D_L15_00161AE0 = 0.97f;
float D_L16_00161938 = 0.97f;
float D_L17_001619B4 = 0.97f;
float D_L18_0016192C = 0.97f;
