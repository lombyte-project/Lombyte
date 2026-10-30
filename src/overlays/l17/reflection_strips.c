/* Level 17 reflection strips: sphere-map UVs for the reflective mesh strips and the draw callbacks that build and draw them. */
#include "types.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/mesh_batch.h"

/* Projects each direction pair into 2D texture coordinates (reflection mapping). */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7018), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z;
} V3;

typedef struct {
    V3    v;
    float w;
} V4;

void compute_reflection_sphere_map_uvs(int    vertex_count,
                                       V3    *input_directions,
                                       V3    *vertex_positions,
                                       int    unused,
                                       float *out) __asm__("FUN_L17_002f54d8");

void compute_reflection_sphere_map_uvs(

    int vertex_count,
    V3 *input_directions,
    V3 *vertex_positions,
    int unused,

    float *sphere_map_output_uvs) {
    V4  normalized_position_delta;
    V4  vertex_position;
    V4  reflection_mapping_vector;
    V4  normalized_input_direction;
    int i;
    for (i = 0; i < vertex_count; i++) {
        float direction_dot_product;
        float sphere_map_denominator;
        vertex_position.w = 1.0f;
        vertex_position.v.x = vertex_positions[i].x;
        vertex_position.v.y = vertex_positions[i].y;
        vertex_position.v.z = vertex_positions[i].z;
        fast_vec_sub(&normalized_position_delta, &vertex_position, D_L17_001676C0);
        fast_vec_normalize(&normalized_position_delta, &normalized_position_delta, 1.0f);
        normalized_input_direction.w = 1.0f;
        normalized_input_direction.v.x = input_directions[i].x;
        normalized_input_direction.v.y = input_directions[i].y;
        normalized_input_direction.v.z = input_directions[i].z;
        fast_vec_normalize(&normalized_input_direction, &normalized_input_direction, 1.0f);
        direction_dot_product =
            fast_vec_dot(&normalized_input_direction, &normalized_position_delta);
        fast_vec_scale(&reflection_mapping_vector,
                       &normalized_input_direction,
                       direction_dot_product + direction_dot_product);
        fast_vec_sub(
            &reflection_mapping_vector, &normalized_position_delta, &reflection_mapping_vector);
        fast_vec_normalize(&reflection_mapping_vector, &reflection_mapping_vector, 1.0f);
        reflection_mapping_vector.v.z = reflection_mapping_vector.v.z + 1.0f;
        sphere_map_denominator = fast_vec_length(&reflection_mapping_vector);
        sphere_map_denominator = sphere_map_denominator + sphere_map_denominator;
        sphere_map_output_uvs[i * 2] =
            (reflection_mapping_vector.v.x / sphere_map_denominator + 0.5f) * 4.0f;
        sphere_map_output_uvs[i * 2 + 1] =
            (reflection_mapping_vector.v.y / sphere_map_denominator + 0.5f) * 4.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F71D0), where it is exact; names translated to the US level program. */

extern char reflection_strip_uv_buffer[] __asm__("D_L17_001DE8A0");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
void        compute_reflection_sphere_map_uvs(int n, V3 *b, V3 *a, int unused, float *out) __asm__("FUN_L17_002f54d8");

int reflection_strip_pair0_vertex_counts[2] __asm__("D_L17_001623F0") = {26, 26};

/* Strip meshes. Each strip i has counts[i] vertices: a position (MeshPoint,
   read as x, y, z with a 12-byte stride by compute_reflection_sphere_map_uvs
   and as 3 words per vertex by FUN_L00_001fde98), a direction (MeshPoint, read
   the same way) and an RGBA colour word (one word per vertex in
   FUN_L00_001fde98). reflection_strip_pair0_* are draw_reflection_strips' tables
   (two strips), reflection_strip0_* draw_reflection_strip's (one strip); the
   code at the two other copies of each draw function reads its own tables,
   listed below. The *_unused_pair_tables (2 floats per vertex) are passed as
   compute_reflection_sphere_map_uvs's unused fourth argument. */
f32 reflection_strip_pair0_positions_0[26][3] __asm__("D_L17_001DE150") = {
    { 674.8834f, 541.42615f, 174.65663f},
    {674.67725f, 541.51013f, 169.23643f},
    {676.72876f,  545.3363f, 174.65668f},
    { 676.5449f, 545.46844f, 169.23647f},
    {679.49744f,  548.8621f, 174.65668f},
    { 679.3472f,  549.0375f,  169.2364f},
    { 683.0227f,  551.6304f, 174.65668f},
    { 682.9161f,  551.8398f,  169.2364f},
    {686.93317f,  553.4756f, 174.65666f},
    { 686.8745f,  553.7077f,  169.2364f},
    { 691.1043f,  554.5508f, 174.65668f},
    {691.09717f,   554.796f,  169.2364f},
    {695.33704f,   554.879f, 174.65668f},
    { 695.3814f,  555.1283f,  169.2364f},
    { 699.5695f, 554.55084f,  174.6567f},
    {699.66626f,   554.796f, 169.23642f},
    { 703.7409f,  553.4756f, 174.65668f},
    { 703.8888f,  553.7077f, 169.23642f},
    { 707.6515f, 551.63043f,  174.6567f},
    { 707.8471f,  551.8398f, 169.23647f},
    {711.17664f,   548.862f, 174.65663f},
    { 711.4158f, 549.03735f, 169.23647f},
    {713.94464f,  545.3362f, 174.65662f},
    { 714.2182f, 545.46844f, 169.23643f},
    {715.79004f,   541.426f, 174.65663f},
    {716.08606f, 541.51013f, 169.23643f},
};

f32 reflection_strip_pair0_positions_1[26][3] __asm__("D_L17_001DE288") = {
    { 676.6713f, 540.69055f, 180.74506f},
    { 675.4068f,  541.2112f, 176.10654f},
    {678.35925f,  544.2676f, 180.74507f},
    { 677.2046f,  545.0216f, 176.10658f},
    { 680.8917f, 547.49255f, 180.74506f},
    { 679.9022f,   548.457f, 176.10661f},
    {684.11664f,  550.0251f, 180.74506f},
    {683.33746f, 551.15466f, 176.10661f},
    { 687.6937f,  551.7128f, 180.74506f},
    {687.14795f,  552.9526f, 176.10658f},
    { 691.5093f, 552.69653f, 180.74506f},
    { 691.2124f, 554.00024f,  176.1066f},
    {695.38116f, 552.99677f, 180.74507f},
    {695.33704f,    554.32f, 176.10661f},
    {699.25256f, 552.69653f, 180.74512f},
    {  699.461f, 554.00024f, 176.10661f},
    {703.06866f,  551.7128f, 180.74506f},
    {703.57007f,  552.9334f, 176.10654f},
    { 706.6456f,  550.0251f, 180.74504f},
    {707.38043f, 551.13544f, 176.10658f},
    {709.87067f, 547.49255f, 180.74506f},
    { 710.8161f, 548.43787f, 176.10654f},
    {712.40295f,  544.2675f, 180.74506f},
    {713.51337f,  545.0024f, 176.10652f},
    { 714.0908f, 540.69055f, 180.74501f},
    { 715.3114f,  541.1921f, 176.10652f},
};

MeshPoint *reflection_strip_pair0_position_tables[2] __asm__("D_L17_001623F8") = {
    reflection_strip_pair0_positions_0, reflection_strip_pair0_positions_1};

extern float D_L17_001DE3C0[];
extern float D_L17_001DE490[];

float *reflection_strip_pair0_unused_pair_tables[2] __asm__("D_L17_00162400") = {D_L17_001DE3C0,
                                                                                 D_L17_001DE490};

f32 reflection_strip_pair0_directions_0[26][3] __asm__("D_L17_001DE560") = {
    {-0.903587f, 0.426433f, 0.041043f},
    {-0.903587f, 0.426433f, 0.041043f},
    {-0.844697f, 0.521747f, 0.041391f},
    {-0.844697f, 0.521747f, 0.041391f},
    {-0.701425f, 0.701422f, 0.042118f},
    {-0.701425f, 0.701422f, 0.042118f},
    {-0.521692f, 0.844653f, 0.042902f},
    {-0.521692f, 0.844653f, 0.042902f},
    {-0.337855f, 0.935464f, 0.043702f},
    {-0.337855f, 0.935464f, 0.043702f},
    {-0.163303f, 0.981696f, 0.044635f},
    {-0.163303f, 0.981696f, 0.044635f},
    {   -7e-06f,  0.99596f,   0.0458f},
    {   -7e-06f,  0.99596f,   0.0458f},
    { 0.163272f, 0.981576f, 0.047325f},
    { 0.163272f, 0.981576f, 0.047325f},
    {  0.33775f,  0.93523f, 0.049245f},
    {  0.33775f,  0.93523f, 0.049245f},
    { 0.521482f, 0.844306f, 0.051444f},
    { 0.521482f, 0.844306f, 0.051444f},
    { 0.701056f, 0.701019f,  0.05361f},
    { 0.701056f, 0.701019f,  0.05361f},
    { 0.844128f, 0.521402f, 0.055262f},
    { 0.844128f, 0.521402f, 0.055262f},
    { 0.902928f, 0.426137f, 0.055925f},
    { 0.902928f, 0.426137f, 0.055925f},
};

f32 reflection_strip_pair0_directions_1[26][3] __asm__("D_L17_001DE698") = {
    {-0.867531f, 0.409369f, 0.282502f},
    {-0.867531f, 0.409369f, 0.282502f},
    {-0.810818f, 0.500773f, 0.283248f},
    {-0.810818f, 0.500773f, 0.283248f},
    {-0.673222f, 0.673229f, 0.283592f},
    {-0.673222f, 0.673229f, 0.283592f},
    {-0.500975f, 0.811217f,  0.28171f},
    {-0.500975f, 0.811217f,  0.28171f},
    {-0.324751f, 0.899315f, 0.278555f},
    {-0.324751f, 0.899315f, 0.278555f},
    {-0.157119f, 0.944633f, 0.275553f},
    {-0.157119f, 0.944633f, 0.275553f},
    {  1.7e-05f, 0.958971f, 0.273576f},
    {  1.7e-05f, 0.958971f, 0.273576f},
    { 0.157322f,   0.9456f, 0.272078f},
    { 0.157322f,   0.9456f, 0.272078f},
    { 0.325372f, 0.901001f, 0.272253f},
    { 0.325372f, 0.901001f, 0.272253f},
    { 0.502126f, 0.813047f, 0.274199f},
    { 0.502126f, 0.813047f, 0.274199f},
    { 0.674972f,  0.67494f, 0.275103f},
    { 0.674972f,  0.67494f, 0.275103f},
    { 0.813055f, 0.502129f, 0.274186f},
    { 0.813055f, 0.502129f, 0.274186f},
    { 0.869938f, 0.410513f, 0.273289f},
    { 0.869938f, 0.410513f, 0.273289f},
};

MeshPoint *reflection_strip_pair0_direction_tables[2] __asm__("D_L17_00162408") = {
    reflection_strip_pair0_directions_0, reflection_strip_pair0_directions_1};

u32 reflection_strip_pair0_colours_0[26] __asm__("D_L17_001DE7D0") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 reflection_strip_pair0_colours_1[26] __asm__("D_L17_001DE838") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 *reflection_strip_pair0_colour_tables[2] __asm__("D_L17_00162410") = {
    reflection_strip_pair0_colours_0, reflection_strip_pair0_colours_1};

void draw_reflection_strips(void) __asm__("FUN_L17_002f5690");

void draw_reflection_strips(void) {
    int    i;
    float *reflection_uv_coordinates;
    vu1_add_g_sregister(6, get_effect_texture(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    reflection_uv_coordinates = (float *)reflection_strip_uv_buffer;
    for (i = 0; i < 2; i++) {
        compute_reflection_sphere_map_uvs(reflection_strip_pair0_vertex_counts[i],
                                          ((int *)reflection_strip_pair0_direction_tables)[i],
                                          ((int *)reflection_strip_pair0_position_tables)[i],
                                          ((int *)reflection_strip_pair0_unused_pair_tables)[i],
                                          reflection_uv_coordinates);
        FUN_L00_001fde98(reflection_strip_pair0_vertex_counts[i],
                         ((int *)reflection_strip_pair0_position_tables)[i],
                         ((int *)reflection_strip_pair0_colour_tables)[i],
                         reflection_uv_coordinates,
                         1);
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F74E8), where it is exact; names translated to the US level program. */

extern char reflection_sphere_map_uv_buffer[] __asm__("D_L17_001DF6A0");

f32 reflection_strip0_positions[66][3] __asm__("D_L17_001DED50") = {
    {  527.208f, 619.89685f, 152.49997f},
    { 524.0342f, 619.89685f, 160.56702f},
    { 527.0433f,  621.5328f, 152.49997f},
    {523.93036f,  620.9136f, 160.56702f},
    { 525.4494f,   626.787f, 152.49997f},
    { 522.5171f, 625.57245f, 160.56702f},
    { 522.8611f,  631.6294f, 152.49997f},
    { 520.2221f,  629.8661f, 160.56702f},
    {519.37787f,  635.8738f, 152.49997f},
    { 517.1336f, 633.62946f, 160.56702f},
    { 515.1335f,   639.357f, 152.49997f},
    { 513.3701f,   636.718f, 160.56702f},
    { 510.2911f,  641.9453f, 152.49997f},
    { 509.0765f, 639.01306f, 160.56702f},
    {505.03683f,  643.5392f, 152.49997f},
    {504.41763f, 640.42633f, 160.56702f},
    {499.57257f,  644.0774f, 152.49997f},
    {499.57257f,  640.9035f, 160.56702f},
    { 494.1083f,  643.5392f, 152.49997f},
    {494.72748f, 640.42633f, 160.56702f},
    {488.85406f,  641.9453f, 152.49997f},
    {490.06863f, 639.01306f, 160.56702f},
    {484.01166f, 639.35706f, 152.49997f},
    {  485.775f,  636.7181f, 160.56702f},
    { 479.7673f,  635.8738f, 152.49997f},
    {482.01157f,  633.6295f, 160.56702f},
    {476.28406f,  631.6294f, 152.49997f},
    {  478.923f,  629.8661f, 160.56702f},
    {473.69574f, 626.78705f, 152.49997f},
    {  476.628f, 625.57245f, 160.56702f},
    {472.10187f,  621.5328f, 152.49997f},
    {475.21475f,  620.9136f, 160.56702f},
    { 471.5637f,  616.0685f, 152.49997f},
    {474.73758f,  616.0685f, 160.56702f},
    {472.10187f, 610.60425f, 152.49997f},
    {475.21475f, 611.22345f, 160.56702f},
    {473.69574f,    605.35f, 152.49997f},
    {  476.628f,  606.5646f, 160.56702f},
    {476.28403f,  600.5076f, 152.49997f},
    {  478.923f, 602.27094f, 160.56702f},
    { 479.7673f, 596.26324f, 152.49997f},
    {482.01157f,  598.5075f, 160.56702f},
    {484.01166f, 592.77997f, 152.49997f},
    {  485.775f, 595.41895f, 160.56702f},
    {  488.854f, 590.19165f, 152.49997f},
    {490.06863f,  594.2832f, 162.70392f},
    { 494.1083f,  588.5978f, 152.49997f},
    {494.72748f, 592.86993f, 163.35786f},
    {499.57257f,  588.0596f, 152.49997f},
    {499.57257f,  592.3927f, 163.35786f},
    {505.03683f,  588.5978f, 152.49997f},
    {504.41763f, 592.86993f, 163.35786f},
    { 510.2911f, 590.19165f, 152.49997f},
    { 509.0765f,  594.2832f, 162.70392f},
    { 515.1335f, 592.77997f, 152.49997f},
    { 513.3702f, 595.41895f, 160.56702f},
    {519.37787f, 596.26324f, 152.49997f},
    { 517.1336f,  598.5075f, 160.56702f},
    {522.86115f,  600.5076f, 152.49997f},
    { 520.2221f,  602.2709f, 160.56702f},
    { 525.4494f,    605.35f, 152.49997f},
    {522.51715f,  606.5646f, 160.56702f},
    { 527.0433f, 610.60425f, 152.49997f},
    {523.93036f, 611.22345f, 160.56702f},
    {  527.208f, 611.92413f, 152.49997f},
    { 524.0342f, 611.92413f, 160.56702f},
};

MeshPoint *reflection_strip0_position_tables[2] __asm__("D_L17_00162440") = {
    reflection_strip0_positions, 0};

f32 reflection_strip0_directions[66][3] __asm__("D_L17_001DF280") = {
    { 0.926455f,  0.093292f, 0.364662f},
    { 0.926455f,  0.093292f, 0.364662f},
    { 0.908762f,    0.1818f, 0.364628f},
    { 0.908762f,    0.1818f, 0.364628f},
    { 0.856143f,  0.354628f, 0.364592f},
    { 0.856143f,  0.354628f, 0.364592f},
    { 0.770512f,  0.514834f, 0.364593f},
    { 0.770512f,  0.514834f, 0.364593f},
    { 0.655263f,  0.655263f, 0.364595f},
    { 0.655263f,  0.655263f, 0.364595f},
    { 0.514835f,  0.770511f, 0.364592f},
    { 0.514835f,  0.770511f, 0.364592f},
    { 0.354628f,  0.856144f, 0.364589f},
    { 0.354628f,  0.856144f, 0.364589f},
    {  0.18079f,  0.908878f, 0.364591f},
    {  0.18079f,  0.908878f, 0.364591f},
    {     -0.0f,  0.926684f,  0.36459f},
    {     -0.0f,  0.926684f,  0.36459f},
    {-0.180791f,  0.908878f, 0.364588f},
    {-0.180791f,  0.908878f, 0.364588f},
    {-0.354624f,  0.856146f,  0.36459f},
    {-0.354624f,  0.856146f,  0.36459f},
    {-0.514835f,  0.770511f, 0.364592f},
    {-0.514835f,  0.770511f, 0.364592f},
    {-0.655267f,  0.655262f,  0.36459f},
    {-0.655267f,  0.655262f,  0.36459f},
    { -0.77051f,  0.514839f, 0.364589f},
    { -0.77051f,  0.514839f, 0.364589f},
    {-0.856143f,  0.354629f,  0.36459f},
    {-0.856143f,  0.354629f,  0.36459f},
    {-0.908877f,  0.180787f, 0.364592f},
    {-0.908877f,  0.180787f, 0.364592f},
    {-0.926684f,    -1e-06f, 0.364592f},
    {-0.926684f,    -1e-06f, 0.364592f},
    {-0.908878f, -0.180787f,  0.36459f},
    {-0.908878f, -0.180787f,  0.36459f},
    {-0.856144f, -0.354627f,  0.36459f},
    {-0.856144f, -0.354627f,  0.36459f},
    { -0.77051f, -0.514838f, 0.364591f},
    { -0.77051f, -0.514838f, 0.364591f},
    {-0.655266f, -0.655263f, 0.364592f},
    {-0.655266f, -0.655263f, 0.364592f},
    {-0.513466f, -0.767935f, 0.371871f},
    {-0.513466f, -0.767935f, 0.371871f},
    {-0.353183f,  -0.85334f, 0.372488f},
    {-0.353183f,  -0.85334f, 0.372488f},
    {-0.180631f, -0.907759f, 0.367468f},
    {-0.180631f, -0.907759f, 0.367468f},
    {     -0.0f,  -0.92491f,  0.36911f},
    {     -0.0f,  -0.92491f,  0.36911f},
    { 0.179452f, -0.903869f, 0.377491f},
    { 0.179452f, -0.903869f, 0.377491f},
    { 0.353378f, -0.852022f, 0.375231f},
    { 0.353378f, -0.852022f, 0.375231f},
    { 0.514839f, -0.770509f, 0.364591f},
    { 0.514839f, -0.770509f, 0.364591f},
    { 0.655264f, -0.655264f, 0.364591f},
    { 0.655264f, -0.655264f, 0.364591f},
    { 0.770511f, -0.514836f, 0.364591f},
    { 0.770511f, -0.514836f, 0.364591f},
    { 0.856145f, -0.354625f, 0.364592f},
    { 0.856145f, -0.354625f, 0.364592f},
    { 0.907713f, -0.192836f, 0.364132f},
    { 0.907713f, -0.192836f, 0.364132f},
    { 0.924356f, -0.115368f,  0.36367f},
    { 0.924356f, -0.115368f,  0.36367f},
};

MeshPoint *reflection_strip0_direction_tables[2] __asm__("D_L17_00162450") = {
    reflection_strip0_directions, 0};

u32 reflection_strip0_colours[66] __asm__("D_L17_001DF598") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f};

u32 *reflection_strip0_colour_tables[2] __asm__("D_L17_00162458") = {reflection_strip0_colours, 0};

int reflection_strip0_vertex_counts[2] __asm__("D_L17_00162420") = {66, 0};

extern float D_L17_001DF070[];
float *reflection_strip0_unused_pair_tables[2] __asm__("D_L17_00162448") = {D_L17_001DF070, 0};

void draw_reflection_strip(void) __asm__("FUN_L17_002f59a8");

void draw_reflection_strip(void) {
    int    i;
    float *sphere_map_uv_coordinates;
    vu1_add_g_sregister(6, get_effect_texture(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    sphere_map_uv_coordinates = (float *)reflection_sphere_map_uv_buffer;
    for (i = 0; i < 1; i++) {
        compute_reflection_sphere_map_uvs(reflection_strip0_vertex_counts[i],
                                          (V3 *)((int *)reflection_strip0_direction_tables)[i],
                                          (V3 *)((int *)reflection_strip0_position_tables)[i],
                                          ((int *)reflection_strip0_unused_pair_tables)[i],
                                          sphere_map_uv_coordinates);
        FUN_L00_001fde98(reflection_strip0_vertex_counts[i],
                         ((int *)reflection_strip0_position_tables)[i],
                         ((int *)reflection_strip0_colour_tables)[i],
                         sphere_map_uv_coordinates,
                         1);
    }
}
/* Computes a per-vertex 2D sphere-map texture coordinate from position and normal arrays. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7C78), where it is exact; names translated to the US level program. */

void compute_vertex_to_reference_sphere_map_uvs(int    vertex_count,
                                                char  *nrm,
                                                char  *vertex_positions,
                                                int    unused,
                                                float *out) __asm__("FUN_L17_002f6138");

void compute_vertex_to_reference_sphere_map_uvs(

    int vertex_count, char *nrm, char *vertex_positions, int unused, float *vertex_sphere_map_uvs) {
    float normalized_position_delta[4];
    float vertex_position[4];
    float reflection_mapping_vector[4];
    float normalized_normal[4];
    int   i;
    for (i = 0; i < vertex_count; i++) {
        float *position_element = (float *)(i * 12 + (int)vertex_positions);
        float *normal_element;
        float  sphere_map_denominator;
        vertex_position[3] = 1.0f;
        vertex_position[0] = position_element[0];
        vertex_position[1] = position_element[1];
        vertex_position[2] = position_element[2];
        fast_vec_sub(normalized_position_delta, D_L17_001676C0, vertex_position);
        fast_vec_normalize(normalized_position_delta, normalized_position_delta, 1.0f);
        normalized_normal[3] = 1.0f;
        normal_element = (float *)(i * 12 + (int)nrm);
        normalized_normal[0] = normal_element[0];
        normalized_normal[1] = normal_element[1];
        normalized_normal[2] = normal_element[2];
        fast_vec_normalize(normalized_normal, normalized_normal, 1.0f);
        fast_vec_scale(reflection_mapping_vector,
                       normalized_normal,
                       fast_vec_dot(normalized_normal, normalized_position_delta) * 2.0f);
        fast_vec_sub(
            reflection_mapping_vector, normalized_position_delta, reflection_mapping_vector);
        fast_vec_normalize(reflection_mapping_vector, reflection_mapping_vector, 1.0f);
        reflection_mapping_vector[2] += 1.0f;
        sphere_map_denominator = fast_vec_length(reflection_mapping_vector) * 2.0f;
        vertex_sphere_map_uvs[i * 2] =
            (reflection_mapping_vector[0] / sphere_map_denominator + 0.5f) * 4.0f;
        vertex_sphere_map_uvs[i * 2 + 1] =
            (reflection_mapping_vector[1] / sphere_map_denominator + 0.5f) * 4.0f;
    }
}

/* Tables and strip meshes of the other two copies of draw_reflection_strips
   (pair1: the copy at 0x2F5CC0, pair2: 0x2F62F0) and of draw_reflection_strip
   (strip1: 0x2F5FD8, strip2: 0x2F6608): the same code reading its own count,
   position, unused pair, direction and colour tables. */
s32 reflection_strip_pair1_vertex_counts[2] __asm__("D_L17_00162460") = {14, 28};

extern float D_L17_001DFD70[];
extern float D_L17_001DFDE0[];

float *reflection_strip_pair1_unused_pair_tables[2] __asm__("D_L17_00162470") = {D_L17_001DFD70,
                                                                                 D_L17_001DFDE0};

f32 reflection_strip_pair1_positions_0[14][3] __asm__("D_L17_001DFB70") = {
    { 515.7769f, 351.20242f,  139.9068f},
    {515.38245f, 351.61414f, 146.90398f},
    { 517.1812f, 352.95706f,  139.9068f},
    { 516.7174f, 353.36877f, 146.90398f},
    { 519.1383f, 356.61844f,  139.9068f},
    { 518.6229f, 356.61844f, 146.90398f},
    {520.34344f, 360.59122f,  139.9068f},
    { 519.7963f,  360.5939f, 146.90398f},
    {520.75037f,  364.7228f,  139.9068f},
    { 520.1925f, 364.72278f, 146.90398f},
    {520.34344f, 368.85437f,  139.9068f},
    {519.79626f, 368.85437f, 146.90398f},
    {519.13824f, 373.37268f,  139.9068f},
    {518.62286f, 373.37268f, 146.90398f},
};

f32 reflection_strip_pair1_positions_1[28][3] __asm__("D_L17_001DFC18") = {
    { 509.7719f, 346.21225f,  139.9068f},
    {509.46198f, 346.59586f, 146.90398f},
    {507.67697f,  345.1571f,  139.9068f},
    {507.46347f,   345.627f, 146.90398f},
    {503.70416f, 343.95193f,  139.9068f},
    {503.59534f, 344.47507f, 146.90398f},
    {499.57257f, 343.54504f,  139.9068f},
    {499.57257f, 344.08612f, 146.90398f},
    {  495.441f, 343.95193f,  139.9068f},
    {495.54984f, 344.47507f, 146.90398f},
    { 491.4682f,  345.1571f,  139.9068f},
    {491.68167f,   345.627f, 146.90398f},
    {487.80682f, 347.11414f,  139.9068f},
    { 488.1168f, 347.49774f, 146.90398f},
    {484.59766f, 349.74783f,  139.9068f},
    { 484.9921f, 350.15958f, 146.90398f},
    { 481.9639f, 352.95706f,  139.9068f},
    {482.42776f, 353.36877f, 146.90398f},
    {480.00687f, 356.61844f,  139.9068f},
    {480.52225f, 356.61844f, 146.90398f},
    {478.80176f, 360.59122f,  139.9068f},
    {479.34885f,  360.5939f, 146.90398f},
    {478.39484f,  364.7228f,  139.9068f},
    { 478.9527f, 364.72278f, 146.90398f},
    {478.80176f, 368.85437f,  139.9068f},
    {479.34885f, 368.85437f, 146.90398f},
    {480.00687f,  373.3727f,  139.9068f},
    {480.52225f, 373.37268f, 146.90398f},
};

MeshPoint *reflection_strip_pair1_position_tables[2] __asm__("D_L17_00162468") = {
    reflection_strip_pair1_positions_0, reflection_strip_pair1_positions_1};

f32 reflection_strip_pair1_directions_0[14][3] __asm__("D_L17_001DFEC0") = {
    {0.777701f,  -0.62242f, 0.088173f},
    {0.777701f,  -0.62242f, 0.088173f},
    {0.828881f, -0.546417f, 0.076497f},
    {0.828881f, -0.546417f, 0.076497f},
    {0.917163f, -0.379945f, 0.069774f},
    {0.917163f, -0.379945f, 0.069774f},
    {0.973166f, -0.193592f,  0.07691f},
    {0.973166f, -0.193592f,  0.07691f},
    {0.992126f,     6e-06f, 0.078341f},
    {0.992126f,     6e-06f, 0.078341f},
    {0.977983f,  0.177399f, 0.074288f},
    {0.977983f,  0.177399f, 0.074288f},
    { 0.96378f,  0.257076f, 0.070988f},
    { 0.96378f,  0.257076f, 0.070988f},
};

f32 reflection_strip_pair1_directions_1[28][3] __asm__("D_L17_001DFF68") = {
    { 0.448766f, -0.891003f, 0.068725f},
    { 0.448766f, -0.891003f, 0.068725f},
    { 0.369142f, -0.922696f, 0.070827f},
    { 0.369142f, -0.922696f, 0.070827f},
    { 0.193623f,  -0.97336f, 0.074319f},
    { 0.193623f,  -0.97336f, 0.074319f},
    {    3e-06f,  -0.99229f, 0.076219f},
    {    3e-06f,  -0.99229f, 0.076219f},
    {-0.193588f, -0.973217f, 0.076284f},
    {-0.193588f, -0.973217f, 0.076284f},
    {-0.379789f, -0.916863f, 0.074625f},
    {-0.379789f, -0.916863f, 0.074625f},
    {-0.551473f,  -0.82532f, 0.071859f},
    {-0.551473f,  -0.82532f, 0.071859f},
    {-0.701658f, -0.701713f, 0.075475f},
    {-0.701658f, -0.701713f, 0.075475f},
    {-0.824578f, -0.550991f, 0.083263f},
    {-0.824578f, -0.550991f, 0.083263f},
    {-0.916619f, -0.379607f, 0.078097f},
    {-0.916619f, -0.379607f, 0.078097f},
    {-0.973378f, -0.193641f, 0.073961f},
    {-0.973378f, -0.193641f, 0.073961f},
    {-0.992125f,    -5e-06f, 0.078354f},
    {-0.992125f,    -5e-06f, 0.078354f},
    {-0.977772f,  0.177342f, 0.077213f},
    {-0.977772f,  0.177342f, 0.077213f},
    {-0.963478f,  0.256974f, 0.075331f},
    {-0.963478f,  0.256974f, 0.075331f},
};

MeshPoint *reflection_strip_pair1_direction_tables[2] __asm__("D_L17_00162478") = {
    reflection_strip_pair1_directions_0, reflection_strip_pair1_directions_1};

u32 reflection_strip_pair1_colours_0[14] __asm__("D_L17_001E00B8") = {0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f,
                                                                      0x7f7f7f7f};

u32 reflection_strip_pair1_colours_1[28] __asm__("D_L17_001E00F0") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 *reflection_strip_pair1_colour_tables[2] __asm__("D_L17_00162480") = {
    reflection_strip_pair1_colours_0, reflection_strip_pair1_colours_1};

s32 reflection_strip1_vertex_counts[1] __asm__("D_L17_00162490") = {46};

extern float D_L17_001E0840[];
float       *reflection_strip1_unused_pair_tables[1] __asm__("D_L17_001624B8") = {D_L17_001E0840};

f32 reflection_strip1_positions[46][3] __asm__("D_L17_001E0610") = {
    {351.31015f, 302.03717f, 180.51582f},
    {350.56842f,   301.991f, 173.53543f},
    {351.31015f, 307.33096f, 180.51582f},
    {350.56842f, 308.77643f, 173.53543f},
    {  351.456f, 312.04572f, 180.49402f},
    {  350.721f, 313.62613f, 173.51265f},
    { 351.9538f,  316.0683f, 180.43471f},
    { 351.2418f, 317.80566f, 173.45058f},
    {352.74014f,  319.1585f, 180.48004f},
    {352.06445f, 321.03864f, 173.49802f},
    {354.10004f, 322.04022f, 180.45811f},
    {353.48715f,  324.0534f,  173.4751f},
    {356.20038f, 324.37546f, 180.48788f},
    { 355.6845f, 326.49652f, 173.50623f},
    {358.46838f, 326.59317f, 180.46034f},
    {358.05722f, 328.81662f, 173.47742f},
    {361.32578f, 328.04016f, 180.48605f},
    { 361.0466f, 330.33047f, 173.50433f},
    { 362.2192f, 328.12415f, 180.49742f},
    { 361.9813f,  330.4183f, 173.48566f},
    { 364.6646f, 328.73438f, 180.49742f},
    { 364.5396f,  331.0567f, 173.48566f},
    { 367.4174f, 329.00522f, 180.49742f},
    {367.41953f, 331.34006f, 173.48566f},
    {370.07703f, 328.73438f, 180.49742f},
    {370.20203f,  331.0567f, 173.48566f},
    { 372.5224f, 328.12415f, 180.49742f},
    {372.76035f,  330.4183f, 173.48566f},
    {373.41586f, 328.04016f, 180.48605f},
    {373.69504f, 330.33047f, 173.50433f},
    {376.27322f, 326.59317f, 180.46034f},
    {376.68442f, 328.81662f, 173.47742f},
    {378.54123f, 324.37546f, 180.48788f},
    { 379.0571f, 326.49652f, 173.50623f},
    { 380.6416f, 322.04022f, 180.45811f},
    { 381.2545f,  324.0534f,  173.4751f},
    {382.00146f,  319.1585f, 180.48004f},
    { 382.6772f, 321.03864f, 173.49802f},
    {382.78784f,  316.0683f, 180.43471f},
    {383.49985f, 317.80566f, 173.45058f},
    { 383.2856f, 312.04572f, 180.49402f},
    {384.02063f, 313.62613f, 173.51265f},
    { 383.4315f, 307.33096f, 180.51582f},
    { 384.1732f, 308.77643f, 173.53543f},
    { 383.4315f, 302.03717f, 180.51582f},
    { 384.1732f,   301.991f, 173.53543f},
};

MeshPoint *reflection_strip1_position_tables[1] __asm__("D_L17_001624B0") = {
    reflection_strip1_positions};

f32 reflection_strip1_directions[46][3] __asm__("D_L17_001E09B0") = {
    { 0.994402f,       0.0f, -0.105664f},
    { 0.994402f,       0.0f, -0.105664f},
    {  0.99383f, -0.015621f, -0.108652f},
    {  0.99383f, -0.015621f, -0.108652f},
    {  0.98838f, -0.077869f, -0.121437f},
    {  0.98838f, -0.077869f, -0.121437f},
    { 0.970525f, -0.182922f, -0.144446f},
    { 0.970525f, -0.182922f, -0.144446f},
    { 0.921646f, -0.330425f, -0.178169f},
    { 0.921646f, -0.330425f, -0.178169f},
    { 0.803659f, -0.532657f, -0.224099f},
    { 0.803659f, -0.532657f, -0.224099f},
    { 0.697525f, -0.668895f, -0.254753f},
    { 0.697525f, -0.668895f, -0.254753f},
    { 0.553532f, -0.771459f, -0.278237f},
    { 0.553532f, -0.771459f, -0.278237f},
    { 0.258134f, -0.898638f, -0.304459f},
    { 0.258134f, -0.898638f, -0.304459f},
    { 0.156447f, -0.934446f, -0.311047f},
    { 0.156447f, -0.934446f, -0.311047f},
    {  0.16158f,  -0.93362f,   -0.3121f},
    {  0.16158f,  -0.93362f,   -0.3121f},
    {-0.001611f, -0.944529f, -0.314518f},
    {-0.001611f, -0.944529f, -0.314518f},
    {-0.163194f, -0.933465f, -0.312078f},
    {-0.163194f, -0.933465f, -0.312078f},
    {-0.161518f, -0.933765f, -0.311683f},
    {-0.161518f, -0.933765f, -0.311683f},
    {-0.263206f, -0.897956f, -0.305093f},
    {-0.263206f, -0.897956f, -0.305093f},
    {-0.553535f, -0.771458f, -0.278235f},
    {-0.553535f, -0.771458f, -0.278235f},
    {-0.697523f, -0.668897f, -0.254753f},
    {-0.697523f, -0.668897f, -0.254753f},
    { -0.80366f, -0.532654f, -0.224101f},
    { -0.80366f, -0.532654f, -0.224101f},
    {-0.921649f, -0.330418f, -0.178169f},
    {-0.921649f, -0.330418f, -0.178169f},
    {-0.970558f, -0.182498f, -0.144599f},
    {-0.970558f, -0.182498f, -0.144599f},
    {-0.988378f,  -0.07771f, -0.121827f},
    {-0.988378f,  -0.07771f, -0.121827f},
    {-0.993795f, -0.015885f, -0.108885f},
    {-0.993795f, -0.015885f, -0.108885f},
    {-0.994402f,       0.0f,  -0.10566f},
    {-0.994402f,       0.0f,  -0.10566f},
};

MeshPoint *reflection_strip1_direction_tables[1] __asm__("D_L17_001624C0") = {
    reflection_strip1_directions};

u32 reflection_strip1_colours[46] __asm__("D_L17_001E0BD8") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 *reflection_strip1_colour_tables[1] __asm__("D_L17_001624C8") = {reflection_strip1_colours};

s32 reflection_strip_pair2_vertex_counts[2] __asm__("D_L17_001624D0") = {119, 77};

extern float D_L17_001E1AA0[];
extern float D_L17_001E1E60[];

float *reflection_strip_pair2_unused_pair_tables[2] __asm__("D_L17_001624E0") = {D_L17_001E1AA0,
                                                                                 D_L17_001E1E60};

f32 reflection_strip_pair2_positions_0[119][3] __asm__("D_L17_001E1160") = {
    { 333.5018f,  536.7269f, 156.07521f},
    {332.18622f,  537.2214f, 158.25644f},
    { 333.9643f,  539.3643f, 156.07506f},
    { 332.9267f, 539.26245f, 158.06369f},
    { 334.4268f,  542.0018f,  156.0749f},
    {333.30328f,  541.3466f, 158.38705f},
    {334.00406f,  544.6283f, 156.20312f},
    { 332.8879f, 543.85333f, 158.37291f},
    {333.58133f,  547.2548f, 156.33136f},
    { 332.3653f, 546.29425f, 158.51753f},
    {332.54987f, 550.59174f, 156.33054f},
    {331.46237f, 549.49854f, 158.41344f},
    {331.51843f,  553.9287f, 156.32973f},
    {330.43655f,  552.5251f, 158.51683f},
    {329.35995f, 556.97394f, 156.32881f},
    {328.44357f,  555.4568f,  158.4022f},
    {327.20148f, 560.01917f, 156.32791f},
    {326.37906f,  558.1238f, 158.51596f},
    {324.63177f, 562.54144f, 156.32697f},
    { 323.9667f,   560.607f, 158.42082f},
    { 322.0621f, 565.06384f, 156.32603f},
    {321.53995f,  562.8342f, 158.51509f},
    { 318.6352f, 567.43555f, 156.32497f},
    {318.28995f, 565.23254f, 158.43275f},
    {315.20828f,  569.8072f,  156.3239f},
    { 314.9663f, 567.29346f, 158.51044f},
    { 312.3715f,  569.8123f, 156.36049f},
    {312.25723f,  567.5062f, 158.47612f},
    {309.53464f,  569.8175f, 156.39708f},
    {309.53378f,  567.4443f, 158.52608f},
    {306.77615f, 569.81226f, 156.36049f},
    {306.89627f,  567.5105f, 158.47266f},
    {304.01767f, 569.80707f,  156.3239f},
    { 304.2741f,  567.2934f, 158.51047f},
    {300.59076f, 567.43536f, 156.32495f},
    {300.95184f,   565.232f, 158.43282f},
    {297.16388f,  565.0637f, 156.32602f},
    {297.70288f,  562.8341f, 158.51514f},
    {294.59415f,  562.5415f, 156.32693f},
    { 295.2751f, 560.60657f,  158.4209f},
    { 292.0244f, 560.01935f, 156.32787f},
    {292.86368f, 558.12384f, 158.51596f},
    {289.86594f,  556.9742f, 156.32875f},
    { 290.7982f,  555.4565f,  158.4023f},
    {287.70743f,  553.9291f, 156.32965f},
    {288.80615f,  552.5253f, 158.51674f},
    {286.67606f,  550.5919f, 156.33049f},
    {287.77966f,  549.4983f, 158.41353f},
    {285.64465f,  547.2548f, 156.33134f},
    { 286.8775f, 546.29425f, 158.51752f},
    {285.08237f,  544.3034f, 156.33171f},
    {286.23358f,  543.7143f, 158.37308f},
    {284.52008f,  541.3521f, 156.33209f},
    {285.79758f, 541.01624f, 158.51784f},
    { 284.7524f, 538.47766f, 156.33223f},
    { 285.9087f,   538.499f, 158.33081f},
    { 284.9847f,  535.6032f, 156.33237f},
    { 285.9087f,   538.499f, 158.33081f},
    { 286.3046f,  536.0787f, 158.51794f},
    { 285.9087f,   538.499f, 158.33081f},
    {288.40036f,  537.0407f, 160.69595f},
    { 285.9087f,   538.499f, 158.33081f},
    {288.13412f,  539.0214f, 160.69598f},
    {285.79758f, 541.01624f, 158.51784f},
    {287.86795f, 541.00214f, 160.69601f},
    {286.23358f,  543.7143f, 158.37308f},
    {288.36755f, 543.24066f, 160.69608f},
    { 286.8775f, 546.29425f, 158.51752f},
    {288.86716f,  545.4793f, 160.69614f},
    {287.77966f,  549.4983f, 158.41353f},
    {289.73224f,  548.2693f, 160.69621f},
    {288.80615f,  552.5253f, 158.51674f},
    {290.59732f, 551.05927f,  160.6963f},
    { 290.7982f,  555.4565f,  158.4023f},
    {292.42865f,  553.5192f, 160.69643f},
    {292.86368f, 558.12384f, 158.51596f},
    {294.25998f,  555.9791f, 160.69653f},
    { 295.2751f, 560.60657f,  158.4209f},
    {296.44867f, 558.08856f, 160.69666f},
    {297.70288f,  562.8341f, 158.51514f},
    {298.63736f,   560.198f, 160.69678f},
    {300.95184f,   565.232f, 158.43282f},
    {301.58392f,  561.6882f,  160.6969f},
    { 304.2741f,  567.2934f, 158.51047f},
    {304.53052f,  563.2269f, 160.69702f},
    { 304.2741f,  567.2934f, 158.51047f},
    { 307.0317f,  564.0754f,  160.9283f},
    {306.89627f,  567.5105f, 158.47266f},
    {309.53293f, 564.05176f, 160.90732f},
    {309.53378f,  567.4443f, 158.52608f},
    {312.12863f,  564.0754f, 160.92827f},
    {312.25723f,  567.5062f, 158.47612f},
    {312.12863f,  564.0754f, 160.92827f},
    { 314.9663f, 567.29346f, 158.51044f},
    {314.72433f, 563.22687f, 160.69698f},
    {318.28995f, 565.23254f, 158.43275f},
    {317.26672f,  561.6786f, 160.69681f},
    {321.53995f,  562.8342f, 158.51509f},
    {320.61758f,  560.1981f, 160.69666f},
    { 323.9667f,   560.607f, 158.42082f},
    {322.80624f, 558.08856f, 160.69658f},
    {326.37906f,  558.1238f, 158.51596f},
    { 324.9949f, 555.97906f,  160.6965f},
    {328.44357f,  555.4568f,  158.4022f},
    {326.82623f, 553.51904f, 160.69646f},
    {330.43655f,  552.5251f, 158.51683f},
    {328.65753f,   551.059f,  160.6964f},
    {331.46237f, 549.49854f, 158.41344f},
    { 329.5226f, 548.26917f, 160.69627f},
    { 332.3653f, 546.29425f, 158.51753f},
    { 330.3877f,  545.4793f, 160.69615f},
    { 332.8879f, 543.85333f, 158.37291f},
    {330.88733f,  543.2407f, 160.69608f},
    {333.30328f,  541.3466f, 158.38705f},
    {331.38693f,  541.0022f, 160.69601f},
    { 332.9267f, 539.26245f, 158.06369f},
    {  330.751f, 539.58325f, 160.56741f},
    {332.18622f,  537.2214f, 158.25644f},
    {330.11508f, 538.16437f, 160.43881f},
};

f32 reflection_strip_pair2_positions_1[77][3] __asm__("D_L17_001E16F8") = {
    {304.73425f,  563.2344f, 161.98314f},
    {304.93796f, 560.63025f,  164.8313f},
    {307.12097f,  563.2594f, 161.91696f},
    { 307.2255f,   560.626f, 164.80464f},
    {309.52295f,  563.2065f,  161.9355f},
    {  309.513f,  560.6217f,   164.778f},
    {312.00882f,  563.3034f, 161.89162f},
    {311.93304f,  560.7326f, 164.73593f},
    {314.53873f, 563.34106f, 161.91438f},
    {311.93304f,  560.7326f, 164.73593f},
    {314.35312f,  560.8435f, 164.69385f},
    { 311.8769f,  559.0923f, 166.36867f},
    {314.19586f, 559.12854f, 166.37465f},
    {311.76743f,  557.3968f, 168.05542f},
    { 314.0386f,  557.4136f, 168.05542f},
    {311.63553f, 555.69586f, 169.60236f},
    {313.75662f, 555.66833f,  169.6541f},
    { 311.4958f,  553.8812f, 171.25278f},
    {313.47467f,  553.9231f, 171.25278f},
    {311.36322f,   551.796f, 173.01773f},
    { 313.1927f,  551.7832f, 173.07472f},
    {311.36322f,   551.796f, 173.01773f},
    { 312.9107f, 549.64325f, 174.89665f},
    {311.36322f,   551.796f, 173.01773f},
    {311.22418f, 549.57635f, 174.89665f},
    {309.52728f,  551.6744f, 173.07472f},
    {309.53763f, 549.50946f, 174.89665f},
    {307.79462f,  551.7915f, 173.02185f},
    {307.94086f,  549.5764f, 174.89668f},
    {307.79462f,  551.7915f, 173.02185f},
    {306.34406f,  549.6434f, 174.89668f},
    {307.79462f,  551.7915f, 173.02185f},
    {306.06824f,  551.7836f, 173.07472f},
    { 307.6547f,  553.8817f, 171.25278f},
    {305.79242f,   553.924f, 171.25279f},
    {307.50775f,  555.6929f, 169.60574f},
    { 305.5166f,  555.6697f, 169.65414f},
    { 307.3685f,  557.3977f, 168.05545f},
    {305.24078f,  557.4154f, 168.05547f},
    {307.30145f, 559.04047f, 166.40115f},
    {305.08936f,  559.0228f, 166.44339f},
    {307.30145f, 559.04047f, 166.40115f},
    {304.93796f, 560.63025f,  164.8313f},
    {307.30145f, 559.04047f, 166.40115f},
    { 307.2255f,   560.626f, 164.80464f},
    { 309.5046f, 559.00085f, 166.41672f},
    {  309.513f,  560.6217f,   164.778f},
    { 309.5046f, 559.00085f, 166.41672f},
    {311.93304f,  560.7326f, 164.73593f},
    { 309.5046f, 559.00085f, 166.41672f},
    { 311.8769f,  559.0923f, 166.36867f},
    { 309.5046f, 559.00085f, 166.41672f},
    {311.76743f,  557.3968f, 168.05542f},
    { 309.5046f, 559.00085f, 166.41672f},
    {309.49622f,    557.38f, 168.05542f},
    {307.30145f, 559.04047f, 166.40115f},
    {309.49622f,    557.38f, 168.05542f},
    { 307.3685f,  557.3977f, 168.05545f},
    {309.49622f,    557.38f, 168.05542f},
    {307.50775f,  555.6929f, 169.60574f},
    {309.49622f,    557.38f, 168.05542f},
    {309.50656f,  555.6097f,  169.6541f},
    {311.76743f,  557.3968f, 168.05542f},
    {309.50656f,  555.6097f,  169.6541f},
    {311.63553f, 555.69586f, 169.60236f},
    {309.51694f, 553.83936f, 171.25278f},
    { 311.4958f,  553.8812f, 171.25278f},
    {309.51694f, 553.83936f, 171.25278f},
    {311.36322f,   551.796f, 173.01773f},
    {309.51694f, 553.83936f, 171.25278f},
    {309.52728f,  551.6744f, 173.07472f},
    {309.51694f, 553.83936f, 171.25278f},
    {307.79462f,  551.7915f, 173.02185f},
    {309.51694f, 553.83936f, 171.25278f},
    { 307.6547f,  553.8817f, 171.25278f},
    {309.50656f,  555.6097f,  169.6541f},
    {307.50775f,  555.6929f, 169.60574f},
};

MeshPoint *reflection_strip_pair2_position_tables[2] __asm__("D_L17_001624D8") = {
    reflection_strip_pair2_positions_0, reflection_strip_pair2_positions_1};

f32 reflection_strip_pair2_directions_0[119][3] __asm__("D_L17_001E20C8") = {
    {-0.832169f,  0.145898f, -0.534985f},
    { -0.75591f,  0.238733f, -0.586767f},
    {-0.865414f,   0.19174f, -0.454716f},
    {-0.787152f,  0.244957f, -0.544839f},
    {-0.885428f,  0.021534f, -0.440741f},
    {-0.817822f,   0.03724f, -0.537073f},
    {-0.856444f, -0.113271f, -0.503293f},
    {-0.790646f, -0.116048f, -0.590827f},
    {-0.823298f, -0.179117f, -0.533572f},
    {-0.761259f, -0.171416f, -0.613693f},
    {-0.798069f, -0.246817f,  -0.54969f},
    {-0.741126f,  -0.22928f, -0.623788f},
    {-0.719286f, -0.350799f, -0.584498f},
    {-0.668699f, -0.328335f, -0.649383f},
    {-0.638311f, -0.452625f, -0.622604f},
    {-0.594515f, -0.425741f, -0.677904f},
    {-0.579919f, -0.492679f, -0.644884f},
    {-0.541368f, -0.462825f, -0.695243f},
    { -0.52367f, -0.533747f, -0.663982f},
    {-0.490194f, -0.501443f, -0.709916f},
    {-0.466986f, -0.563781f, -0.678159f},
    {-0.389787f, -0.520923f, -0.748113f},
    {-0.406212f, -0.587264f, -0.699936f},
    {-0.353218f, -0.515078f, -0.772347f},
    {-0.205736f, -0.617376f, -0.732528f},
    {-0.192689f, -0.541086f, -0.798395f},
    {-0.010833f, -0.665676f, -0.746042f},
    {-0.014657f, -0.623258f, -0.779838f},
    {-0.006216f,  -0.67173f, -0.740737f},
    {-0.003063f, -0.626599f, -0.777152f},
    { 0.004779f,  -0.66169f, -0.749691f},
    { 0.011936f, -0.621275f, -0.781644f},
    { 0.205531f, -0.616814f, -0.733192f},
    { 0.194308f, -0.540337f, -0.798535f},
    { 0.405391f, -0.586074f, -0.701413f},
    { 0.356757f, -0.519574f, -0.768775f},
    { 0.465822f, -0.562432f, -0.680101f},
    {  0.42812f, -0.514497f, -0.735171f},
    { 0.522244f, -0.532333f, -0.666237f},
    { 0.489038f, -0.500292f, -0.711585f},
    { 0.578226f, -0.491275f, -0.647503f},
    { 0.539989f,  -0.46168f, -0.697174f},
    { 0.636339f, -0.451242f,  -0.62562f},
    { 0.592906f, -0.424601f, -0.680122f},
    { 0.716944f, -0.349678f, -0.588163f},
    {  0.66676f,  -0.32741f, -0.652064f},
    { 0.795368f,  -0.24596f, -0.553972f},
    { 0.738863f, -0.228565f, -0.626908f},
    { 0.812842f,   -0.2022f, -0.544185f},
    { 0.753278f, -0.189063f, -0.620721f},
    { 0.834074f, -0.158973f, -0.528153f},
    { 0.771444f, -0.151732f, -0.609262f},
    { 0.852617f,  -0.04501f, -0.507422f},
    { 0.783514f, -0.039701f, -0.598172f},
    { 0.856615f,  0.069208f,  -0.51097f},
    { 0.783866f,  0.070589f, -0.605105f},
    { 0.847346f,  0.068453f,  -0.52661f},
    { 0.783866f,  0.070589f, -0.605105f},
    { 0.776514f,  0.064653f, -0.616261f},
    { 0.783866f,  0.070589f, -0.605105f},
    { 0.705681f,  0.060852f, -0.705912f},
    { 0.783866f,  0.070589f, -0.605105f},
    { 0.711116f,  0.071969f,  -0.69924f},
    { 0.783514f, -0.039701f, -0.598172f},
    { 0.714412f, -0.034391f, -0.688923f},
    { 0.771444f, -0.151732f, -0.609262f},
    { 0.708814f, -0.144491f, -0.690371f},
    { 0.753278f, -0.189063f, -0.620721f},
    { 0.693713f, -0.175925f, -0.697257f},
    { 0.738863f, -0.228565f, -0.626908f},
    { 0.682358f, -0.211169f, -0.699844f},
    {  0.66676f,  -0.32741f, -0.652064f},
    { 0.616577f, -0.305141f, -0.715966f},
    { 0.592906f, -0.424601f, -0.680122f},
    { 0.549472f, -0.397961f, -0.734625f},
    { 0.539989f,  -0.46168f, -0.697174f},
    { 0.501752f, -0.432086f, -0.746846f},
    { 0.489038f, -0.500292f, -0.711585f},
    { 0.455832f, -0.468251f, -0.756934f},
    {  0.42812f, -0.514497f, -0.735171f},
    { 0.390418f, -0.466562f, -0.790242f},
    { 0.356757f, -0.519574f, -0.768775f},
    { 0.308122f, -0.453074f, -0.836137f},
    { 0.194308f, -0.540337f, -0.798535f},
    { 0.262085f,  -0.44392f, -0.856319f},
    { 0.194308f, -0.540337f, -0.798535f},
    { 0.090116f, -0.536988f,  -0.82997f},
    { 0.011936f, -0.621275f, -0.781644f},
    {    9e-05f, -0.581469f, -0.813567f},
    {-0.003063f, -0.626599f, -0.777152f},
    { -0.08713f, -0.537592f, -0.830355f},
    {-0.014657f, -0.623258f, -0.779838f},
    { -0.08713f, -0.537592f, -0.830355f},
    {-0.192689f, -0.541086f, -0.798395f},
    {-0.258353f, -0.445218f,  -0.85662f},
    {-0.353218f, -0.515078f, -0.772347f},
    {-0.283248f, -0.483318f, -0.825546f},
    {-0.389787f, -0.520923f, -0.748113f},
    {-0.353397f, -0.515305f, -0.772088f},
    {-0.490194f, -0.501443f, -0.709916f},
    {-0.456717f, -0.469139f,  -0.75585f},
    {-0.541368f, -0.462825f, -0.695243f},
    {-0.502816f,  -0.43297f, -0.745602f},
    {-0.594515f, -0.425741f, -0.677904f},
    {-0.550719f, -0.398857f, -0.733203f},
    {-0.668699f, -0.328335f, -0.649383f},
    {-0.618112f, -0.305871f, -0.714268f},
    {-0.741126f,  -0.22928f, -0.623788f},
    {-0.684184f, -0.211742f, -0.697886f},
    {-0.761259f, -0.171416f, -0.613693f},
    {-0.699219f, -0.163715f, -0.693813f},
    {-0.790646f, -0.116048f, -0.590827f},
    {-0.724849f, -0.118826f, -0.678362f},
    {-0.817822f,   0.03724f, -0.537073f},
    {-0.726805f,  0.156134f, -0.634718f},
    {-0.787152f,  0.244957f, -0.544839f},
    {-0.670785f,  0.300297f,  -0.67168f},
    { -0.75591f,  0.238733f, -0.586767f},
    {-0.605954f,  0.336896f, -0.720639f},
};

f32 reflection_strip_pair2_directions_1[77][3] __asm__("D_L17_001E2660") = {
    {-0.010955f, -0.738362f, -0.674315f},
    {-0.010251f, -0.723455f, -0.689957f},
    {-0.011019f, -0.738702f, -0.673942f},
    { -0.00985f,  -0.72422f, -0.689175f},
    { 0.003008f, -0.740518f, -0.671878f},
    { 0.004557f, -0.724409f, -0.688808f},
    { 0.019725f, -0.741991f, -0.670114f},
    { 0.019797f, -0.722598f, -0.690407f},
    {  0.02235f, -0.741987f, -0.670041f},
    { 0.019797f, -0.722598f, -0.690407f},
    {  0.02103f, -0.721365f, -0.691592f},
    { 0.016344f, -0.703135f, -0.710851f},
    { 0.016251f, -0.700617f, -0.713344f},
    { 0.008902f, -0.688763f, -0.724646f},
    { 0.008894f, -0.688194f, -0.725263f},
    { 0.008374f, -0.673875f, -0.738786f},
    { 0.009621f, -0.674635f, -0.738073f},
    { 0.012736f, -0.660586f, -0.750448f},
    { 0.013992f, -0.661318f, -0.749803f},
    { 0.018062f, -0.647314f, -0.761992f},
    { 0.019696f, -0.648039f, -0.761328f},
    { 0.018062f, -0.647314f, -0.761992f},
    { 0.025655f, -0.646814f, -0.762216f},
    { 0.018062f, -0.647314f, -0.761992f},
    { 0.022416f, -0.646743f, -0.762372f},
    {-0.001944f, -0.645923f, -0.763175f},
    {-0.000637f, -0.646685f, -0.762499f},
    {-0.018924f, -0.647332f, -0.761963f},
    {-0.020381f, -0.648148f, -0.761239f},
    {-0.018924f, -0.647332f, -0.761963f},
    {-0.020313f, -0.649597f, -0.760007f},
    {-0.018924f, -0.647332f, -0.761963f},
    {-0.017532f, -0.649407f, -0.760234f},
    {-0.013773f, -0.660609f, -0.750353f},
    {-0.012391f, -0.662707f, -0.748556f},
    { -0.00785f, -0.674565f, -0.738168f},
    {-0.007829f, -0.676017f, -0.736841f},
    {-0.006711f,  -0.69176f, -0.721694f},
    {-0.006726f, -0.692155f, -0.721362f},
    {-0.008242f, -0.709416f,  -0.70474f},
    {-0.008685f, -0.708511f, -0.705646f},
    {-0.008242f, -0.709416f,  -0.70474f},
    {-0.010251f, -0.723455f, -0.689957f},
    {-0.008242f, -0.709416f,  -0.70474f},
    { -0.00985f,  -0.72422f, -0.689175f},
    { 0.004319f, -0.707987f, -0.706096f},
    { 0.004557f, -0.724409f, -0.688808f},
    { 0.004319f, -0.707987f, -0.706096f},
    { 0.019797f, -0.722598f, -0.690407f},
    { 0.004319f, -0.707987f, -0.706096f},
    { 0.016344f, -0.703135f, -0.710851f},
    { 0.004319f, -0.707987f, -0.706096f},
    { 0.008902f, -0.688763f, -0.724646f},
    { 0.004319f, -0.707987f, -0.706096f},
    { 0.001107f, -0.690348f, -0.723028f},
    {-0.008242f, -0.709416f,  -0.70474f},
    { 0.001107f, -0.690348f, -0.723028f},
    {-0.006711f,  -0.69176f, -0.721694f},
    { 0.001107f, -0.690348f, -0.723028f},
    { -0.00785f, -0.674565f, -0.738168f},
    { 0.001107f, -0.690348f, -0.723028f},
    {-0.000372f, -0.673114f, -0.739498f},
    { 0.008902f, -0.688763f, -0.724646f},
    {-0.000372f, -0.673114f, -0.739498f},
    { 0.008374f, -0.673875f, -0.738786f},
    {-0.001838f, -0.659182f, -0.751622f},
    { 0.012736f, -0.660586f, -0.750448f},
    {-0.001838f, -0.659182f, -0.751622f},
    { 0.018062f, -0.647314f, -0.761992f},
    {-0.001838f, -0.659182f, -0.751622f},
    {-0.001944f, -0.645923f, -0.763175f},
    {-0.001838f, -0.659182f, -0.751622f},
    {-0.018924f, -0.647332f, -0.761963f},
    {-0.001838f, -0.659182f, -0.751622f},
    {-0.013773f, -0.660609f, -0.750353f},
    {-0.000372f, -0.673114f, -0.739498f},
    { -0.00785f, -0.674565f, -0.738168f},
};

MeshPoint *reflection_strip_pair2_direction_tables[2] __asm__("D_L17_001624E8") = {
    reflection_strip_pair2_directions_0, reflection_strip_pair2_directions_1};

u32 reflection_strip_pair2_colours_0[119] __asm__("D_L17_001E2A00") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 reflection_strip_pair2_colours_1[77] __asm__("D_L17_001E2BE0") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 *reflection_strip_pair2_colour_tables[2] __asm__("D_L17_001624F0") = {
    reflection_strip_pair2_colours_0, reflection_strip_pair2_colours_1};

s32 reflection_strip2_vertex_counts[1] __asm__("D_L17_00162500") = {66};

extern float D_L17_001E34F0[];
float       *reflection_strip2_unused_pair_tables[1] __asm__("D_L17_00162528") = {D_L17_001E34F0};

f32 reflection_strip2_positions[66][3] __asm__("D_L17_001E31D0") = {
    { 554.8556f, 441.49615f, 147.95636f},
    {555.89044f, 441.49615f, 153.63228f},
    { 554.5512f, 444.58636f, 147.95636f},
    {555.56616f, 444.78824f, 153.63228f},
    {553.64984f,  447.5578f, 147.95636f},
    { 554.6059f, 447.95383f, 153.63228f},
    {552.18604f, 450.29626f, 147.95636f},
    {553.04645f, 450.87122f, 153.63228f},
    { 550.2162f,  452.6966f, 147.95636f},
    {550.94794f, 453.42834f, 153.63228f},
    {547.81586f, 454.66647f, 147.95636f},
    {548.39075f, 455.52692f, 153.63228f},
    { 545.0774f, 456.13025f, 147.95636f},
    { 545.4734f,  457.0863f, 153.63228f},
    { 542.1059f, 457.03162f, 147.95636f},
    { 542.3078f,  458.0466f, 153.63228f},
    {539.01575f, 457.33597f, 147.95636f},
    {539.01575f, 458.37082f, 153.63228f},
    {535.92554f, 457.03162f, 147.95636f},
    {535.72363f,  458.0466f, 153.63228f},
    { 532.9541f, 456.13025f, 147.95636f},
    {532.55804f,  457.0863f, 153.63228f},
    { 530.2156f,  454.6665f, 147.95636f},
    { 529.6406f, 455.52695f, 153.63228f},
    {527.81525f,  452.6966f, 147.95636f},
    {527.08356f, 453.42834f, 153.63228f},
    { 525.8454f, 450.29626f, 147.95636f},
    { 524.9849f, 450.87122f, 153.63228f},
    {524.38165f,  447.5578f, 147.95636f},
    { 523.4256f, 447.95383f, 153.63228f},
    { 523.4802f, 444.58636f, 147.95636f},
    { 522.4653f, 444.78824f, 153.63228f},
    { 523.1759f, 441.49615f, 147.95636f},
    {522.14105f, 441.49615f, 153.63228f},
    { 523.4802f, 438.40598f, 147.95636f},
    { 522.4653f, 438.20404f, 153.63228f},
    {524.38165f,  435.4345f, 147.95636f},
    { 523.4256f,  435.0385f, 153.63228f},
    { 525.8454f,   432.696f, 147.95636f},
    { 524.9849f,  432.1211f, 153.63228f},
    {527.81525f,  430.2957f, 147.95636f},
    {527.08356f, 429.56396f, 153.63228f},
    { 530.2156f,  428.3258f, 147.95636f},
    { 529.6406f,  427.4654f, 153.63228f},
    { 532.9541f, 426.86206f, 147.95636f},
    {532.55804f,   425.906f, 153.63228f},
    {535.92554f, 425.96066f, 147.95636f},
    {535.72363f, 424.94574f, 153.63228f},
    {539.01575f,  425.6563f, 147.95636f},
    {539.01575f, 424.62146f, 153.63228f},
    { 542.1059f, 425.96066f, 147.95636f},
    { 542.3078f, 424.94574f, 153.63228f},
    { 545.0774f, 426.86206f, 147.95636f},
    { 545.4734f, 425.90598f, 153.63228f},
    {547.81586f,  428.3258f, 147.95636f},
    {548.39075f,  427.4654f, 153.63228f},
    { 550.2162f,  430.2957f, 147.95636f},
    {550.94794f, 429.56393f, 153.63228f},
    { 552.1861f,   432.696f, 147.95636f},
    { 553.0465f,  432.1211f, 153.63228f},
    {553.64984f,  435.4345f, 147.95636f},
    { 554.6059f,  435.0385f, 153.63228f},
    { 554.5512f, 438.40598f, 147.95636f},
    {555.56616f, 438.20404f, 153.63228f},
    { 554.8556f, 441.49615f, 147.95636f},
    {555.89044f, 441.49615f, 153.63228f},
};

MeshPoint *reflection_strip2_position_tables[1] __asm__("D_L17_00162520") = {
    reflection_strip2_positions};

f32 reflection_strip2_directions[66][3] __asm__("D_L17_001E3700") = {
    { 0.979196f,      -0.0f, -0.178529f},
    { 0.979196f,      -0.0f, -0.178529f},
    { 0.960382f,  0.191034f, -0.178527f},
    { 0.960382f,  0.191034f, -0.178527f},
    { 0.904656f,   0.37473f, -0.178527f},
    { 0.904656f,   0.37473f, -0.178527f},
    { 0.814172f,  0.544016f, -0.178528f},
    { 0.814172f,  0.544016f, -0.178528f},
    { 0.692399f,  0.692394f, -0.178528f},
    { 0.692399f,  0.692394f, -0.178528f},
    { 0.544015f,  0.814172f, -0.178525f},
    { 0.544015f,  0.814172f, -0.178525f},
    { 0.374723f,  0.904659f, -0.178529f},
    { 0.374723f,  0.904659f, -0.178529f},
    { 0.191028f,  0.960382f, -0.178532f},
    { 0.191028f,  0.960382f, -0.178532f},
    {    1e-06f,  0.979196f, -0.178532f},
    {    1e-06f,  0.979196f, -0.178532f},
    { -0.19103f,  0.960382f, -0.178531f},
    { -0.19103f,  0.960382f, -0.178531f},
    {-0.374718f,  0.904662f,  -0.17853f},
    {-0.374718f,  0.904662f,  -0.17853f},
    { -0.54401f,  0.814174f, -0.178527f},
    { -0.54401f,  0.814174f, -0.178527f},
    {-0.692401f,  0.692391f,  -0.17853f},
    {-0.692401f,  0.692391f,  -0.17853f},
    {-0.814176f,  0.544008f, -0.178531f},
    {-0.814176f,  0.544008f, -0.178531f},
    {-0.904657f,  0.374731f, -0.178527f},
    {-0.904657f,  0.374731f, -0.178527f},
    { -0.96038f,  0.191034f, -0.178529f},
    { -0.96038f,  0.191034f, -0.178529f},
    {-0.979198f,      -0.0f, -0.178529f},
    {-0.979198f,      -0.0f, -0.178529f},
    {-0.960381f, -0.191033f, -0.178527f},
    {-0.960381f, -0.191033f, -0.178527f},
    {-0.904659f, -0.374727f, -0.178529f},
    {-0.904659f, -0.374727f, -0.178529f},
    {-0.814177f, -0.544007f, -0.178526f},
    {-0.814177f, -0.544007f, -0.178526f},
    {  -0.6924f, -0.692394f, -0.178523f},
    {  -0.6924f, -0.692394f, -0.178523f},
    {-0.544007f, -0.814176f, -0.178529f},
    {-0.544007f, -0.814176f, -0.178529f},
    {-0.374722f, -0.904661f, -0.178527f},
    {-0.374722f, -0.904661f, -0.178527f},
    {-0.191034f, -0.960381f, -0.178527f},
    {-0.191034f, -0.960381f, -0.178527f},
    {    1e-06f, -0.979197f, -0.178527f},
    {    1e-06f, -0.979197f, -0.178527f},
    { 0.191032f, -0.960382f, -0.178527f},
    { 0.191032f, -0.960382f, -0.178527f},
    { 0.374724f,  -0.90466f, -0.178526f},
    { 0.374724f,  -0.90466f, -0.178526f},
    { 0.544012f, -0.814174f, -0.178526f},
    { 0.544012f, -0.814174f, -0.178526f},
    { 0.692394f, -0.692399f, -0.178527f},
    { 0.692394f, -0.692399f, -0.178527f},
    { 0.814173f, -0.544013f, -0.178525f},
    { 0.814173f, -0.544013f, -0.178525f},
    { 0.904662f, -0.374719f, -0.178528f},
    { 0.904662f, -0.374719f, -0.178528f},
    { 0.960382f, -0.191033f,  -0.17853f},
    { 0.960382f, -0.191033f,  -0.17853f},
    { 0.979196f,      -0.0f, -0.178529f},
    { 0.979196f,      -0.0f, -0.178529f},
};

MeshPoint *reflection_strip2_direction_tables[1] __asm__("D_L17_00162530") = {
    reflection_strip2_directions};

u32 reflection_strip2_colours[66] __asm__("D_L17_001E3A18") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f};

u32 *reflection_strip2_colour_tables[1] __asm__("D_L17_00162538") = {reflection_strip2_colours};
