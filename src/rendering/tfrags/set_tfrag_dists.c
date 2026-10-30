#include "types.h"
#include "rnc/rendering/tfrags/set_tfrag_dists_globals.h"
#include "rnc/math/asm_helpers.h"

extern f32 tfrag_render_ranges[3] __asm__("D_00160EA0");
extern s32 tfrag_lod_dists_raw[3] __asm__("D_00160EB0");
extern f32 fog_params[] __asm__("D_0018CF20");

void set_tfrag_dists(void) __asm__("FUN_00233068");

void set_tfrag_dists(void) {
    float scaled_far_range, scaled_middle_range, scaled_near_range, ab, bc;

    tfrag_lod_dists_raw[0] = truncate_float_to_s32(tfrag_render_ranges[0] * 1024.0f);
    tfrag_lod_dists_raw[1] = truncate_float_to_s32(tfrag_render_ranges[1] * 1024.0f);
    tfrag_lod_dists_raw[2] = truncate_float_to_s32(tfrag_render_ranges[2] * 1024.0f);
    scaled_far_range = tfrag_render_ranges[0] * fog_params[0];
    scaled_middle_range = tfrag_render_ranges[1] * fog_params[0];
    scaled_near_range = tfrag_render_ranges[2] * fog_params[0];
    ab = 1.0f / (scaled_far_range - scaled_middle_range);
    bc = 1.0f / (scaled_middle_range - scaled_near_range);
    fast_mem_zero16(tfrag_range_coefficients, 0x40);
    tfrag_range_coefficients[0][0] = ab * 0.5f;
    tfrag_range_coefficients[0][1] = -ab;
    tfrag_range_coefficients[1][0] = bc * 0.5f;
    tfrag_range_coefficients[1][1] = -bc;
    tfrag_range_coefficients[2][0] = scaled_middle_range * ab * -0.5f;
    tfrag_range_coefficients[2][1] = scaled_far_range * ab;
    tfrag_range_coefficients[3][0] = scaled_near_range * bc * -0.5f;
    tfrag_range_coefficients[3][1] = scaled_middle_range * bc;
    tfrag_range_coefficients[1][3] = scaled_middle_range;
    tfrag_range_coefficients[0][3] = scaled_far_range;
}

extern __typeof__(set_tfrag_dists) func_00233068 __attribute__((alias("FUN_00233068")));

f32 tfrag_range_coefficients[4][4] = {
    { -0.1633333f, 0.32666662f,    255.0f, -10.714285f},
    {-0.28583336f,  0.5716668f,   2048.0f,   -7.65306f},
    { -1.2499994f,   3.499999f, 1.46e-42f,        0.0f},
    { -1.6874998f,      4.375f, 1.46e-42f,        0.0f},
};
