/* Level 17 radial ripple mesh draws (no caller found in C): the mesh pair, the ripple ring and the plain mesh. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_globals.h"
#include "rnc/mobys/moby_class_id.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/mesh_batch.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/ui/help/force_help_message.h"

extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");

extern float fast_add_rotations(float, float);

extern float scale_time(float) __asm__("FUN_001f96b0");

/* Builds the two rippling vertex strips around the moby (template points moved to its position, z waved by distance from the centre), then draws them twice with scrolled texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1FE0), where it is exact; names translated to the US level program. */

typedef struct {
    float f0;
    float f4;
} RippleData;

typedef struct {
    float v[2];
} P2;

typedef struct {
    P2 p[2];
} T4;

T4 ripple_pair_uv_scroll_factors __asm__("D_L17_00162268") = {
    {{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}
};

extern float FUN_L00_00200260(float, float);
int          ripple_pair_alpha_1_fix __asm__("D_L17_00162248") = 128;
int          ripple_pair_effect_texture_id __asm__("D_L17_0016224C") = 45;
int          ripple_pair_vertex_alpha __asm__("D_L17_00162250") = 64;

#include "rnc/overlay/l17/mobys/radial_ripple_mesh_globals.h"

extern float ripple_pair_radial_phase_distance_scale_gp __asm__("D_L17_00162254")
    __attribute__((sda));
extern float ripple_pair_phase_period_gp __asm__("D_L17_00162258") __attribute__((sda));
extern float ripple_pair_z_amplitude_gp __asm__("D_L17_0016225C") __attribute__((sda));
extern float ripple_pair_uv_scroll_rate_gp __asm__("D_L17_00162260") __attribute__((sda));
extern void  FUN_L00_001fde98(int, int, int, void *, int);
void         font_queue_vu_state(void) __asm__("FUN_001f76a0");

/* The two strips: ripple_pair_vertex_counts holds their vertex counts (148 and 11) and
   each table below one stream per strip. draw_moby_radial_ripple_mesh_pair
   reads 3 floats per template vertex (12-byte stride) and 2 per template uv
   (8-byte stride), writes 3 floats per world vertex, 2 per scrolled uv and
   one colour word per vertex, for j < count; FUN_L00_001fde98 reads the same
   streams with the same strides. */
int ripple_pair_vertex_counts[2] __asm__("D_L17_00162210") = {148, 11};

f32 ripple_pair_world_vertices_0[148][3] __asm__("D_L17_001DAF30") = {0};

f32 ripple_pair_world_vertices_1[11][3] __asm__("D_L17_001DBD10") = {0};

int ripple_pair_world_vertex_tables[2] __asm__("D_L17_00162218") = {
    (int)ripple_pair_world_vertices_0, (int)ripple_pair_world_vertices_1};

f32 ripple_pair_scrolled_uvs_0[148][2] __asm__("D_L17_001DB620") = {0};

f32 ripple_pair_scrolled_uvs_1[11][2] __asm__("D_L17_001DBD98") = {0};

int ripple_pair_scrolled_uv_tables[2] __asm__("D_L17_00162220") = {(int)ripple_pair_scrolled_uvs_0,
                                                                   (int)ripple_pair_scrolled_uvs_1};

u32 ripple_pair_colours_0[148] __asm__("D_L17_001DBAC0") = {0};

u32 ripple_pair_colours_1[11] __asm__("D_L17_001DBDF0") = {0};

int ripple_pair_colour_tables[2] __asm__("D_L17_00162228") = {(int)ripple_pair_colours_0,
                                                              (int)ripple_pair_colours_1};

f32 ripple_pair_template_vertices_0[148][3] __asm__("D_L17_001DA040") = {
    {-17.275997f,  17.276007f,    0.0f},
    {-13.571114f,  20.310587f,    0.0f},
    {-20.310606f,  13.571103f,    0.0f},
    {-14.902949f,  14.902954f, -2e-06f},
    {-22.572187f,   9.349717f,    0.0f},
    {-16.416876f,   9.933741f,    0.0f},
    {-23.957989f,   4.765554f,    0.0f},
    {-17.352987f,   4.899419f,    0.0f},
    {-24.431953f,        0.0f,    0.0f},
    { -17.76748f,        0.0f,    0.0f},
    {-23.957989f,  -4.765554f,    0.0f},
    {-17.352987f,  -4.899419f,    0.0f},
    {-22.572187f,  -9.349717f,    0.0f},
    {-16.416876f,  -9.933741f, -2e-06f},
    {-20.310606f, -13.571103f,    0.0f},
    {-14.902949f, -14.902954f, -2e-06f},
    {-17.275997f, -17.275969f,    0.0f},
    {-14.902949f, -14.902954f, -2e-06f},
    {-13.571114f, -20.310625f,    0.0f},
    {  -9.93375f,  -16.41687f,    0.0f},
    { -9.349727f, -22.572208f,    0.0f},
    { -4.899414f, -17.352983f,    0.0f},
    { -4.765547f,  -23.95798f,    0.0f},
    {       0.0f, -17.767485f,    0.0f},
    {       0.0f, -24.431934f,    0.0f},
    {  4.899414f, -17.352987f, -2e-06f},
    {  4.765508f,  -23.95798f,    0.0f},
    {  9.933711f,  -16.41687f,    0.0f},
    {  9.349688f, -22.572208f,    0.0f},
    {  14.90293f, -14.902954f,    0.0f},
    { 13.571134f, -20.310625f,    0.0f},
    {  14.90293f, -14.902954f,    0.0f},
    { 17.275976f, -17.275969f,    0.0f},
    {  14.90293f, -14.902954f,    0.0f},
    { 20.310587f, -13.571103f,    0.0f},
    { 16.416876f,  -9.933741f,    0.0f},
    { 22.572187f,  -9.349717f,    0.0f},
    {  17.35297f,  -4.899414f, -2e-06f},
    { 23.958008f,  -4.765554f,    0.0f},
    { 17.767462f,     -1e-06f, -2e-06f},
    { 24.431953f,     -1e-06f,    0.0f},
    {  17.35297f,   4.899412f, -2e-06f},
    { 23.958008f,   4.765552f,    0.0f},
    { 16.416876f,   9.933736f,    0.0f},
    { 22.572187f,   9.349717f,    0.0f},
    {  14.90293f,  14.902949f, -2e-06f},
    { 20.310587f,  13.571103f,    0.0f},
    {  14.90293f,  14.902949f, -2e-06f},
    { 17.275976f,  17.276007f,    0.0f},
    {  14.90293f,  14.902949f, -2e-06f},
    { 13.571134f,  20.310587f,    0.0f},
    {  9.933711f,   16.41687f,    0.0f},
    {  9.349688f,  22.572168f,    0.0f},
    {  4.899414f,  17.352983f,    0.0f},
    {  4.765508f,  23.957968f,    0.0f},
    {       0.0f,   17.76748f, -2e-06f},
    {       0.0f,  24.431934f,    0.0f},
    { -4.899434f,   17.35298f, -2e-06f},
    { -4.765547f,  23.957968f,    0.0f},
    {  -9.93375f,   16.41687f, -2e-06f},
    { -9.349727f,  22.572168f,    0.0f},
    {  -9.93375f,   16.41687f, -2e-06f},
    {-13.571114f,  20.310587f,    0.0f},
    {  -9.93375f,   16.41687f, -2e-06f},
    {-14.902949f,  14.902954f, -2e-06f},
    {-10.607012f,  10.606997f,    0.0f},
    {-16.416876f,   9.933741f,    0.0f},
    { -11.13959f,   5.161695f,    0.0f},
    {-17.352987f,   4.899419f,    0.0f},
    {-11.423183f,        0.0f,    0.0f},
    { -17.76748f,        0.0f,    0.0f},
    { -11.13959f,  -5.161695f,    0.0f},
    {-17.352987f,  -4.899419f,    0.0f},
    {-10.607012f, -10.606997f,    0.0f},
    {-16.416876f,  -9.933741f, -2e-06f},
    {-10.607012f, -10.606997f,    0.0f},
    {-14.902949f, -14.902954f, -2e-06f},
    {-10.607012f, -10.606997f,    0.0f},
    {  -9.93375f,  -16.41687f,    0.0f},
    {  -5.16168f, -11.139605f,    0.0f},
    { -4.899414f, -17.352983f,    0.0f},
    {       0.0f, -11.423194f,    0.0f},
    {       0.0f, -17.767485f,    0.0f},
    {   5.16168f, -11.139571f,    0.0f},
    {  4.899414f, -17.352987f, -2e-06f},
    { 10.606993f, -10.606997f,    0.0f},
    {  9.933711f,  -16.41687f,    0.0f},
    { 10.606993f, -10.606997f,    0.0f},
    {  14.90293f, -14.902954f,    0.0f},
    { 10.606993f, -10.606997f,    0.0f},
    { 16.416876f,  -9.933741f,    0.0f},
    { 11.139609f,  -5.161695f,    0.0f},
    {  17.35297f,  -4.899414f, -2e-06f},
    { 11.423164f,       -0.0f,    0.0f},
    { 17.767462f,     -1e-06f, -2e-06f},
    { 11.139609f,   5.161695f, -2e-06f},
    {  17.35297f,   4.899412f, -2e-06f},
    { 10.606993f,  10.606997f,    0.0f},
    { 16.416876f,   9.933736f,    0.0f},
    { 10.606993f,  10.606997f,    0.0f},
    {  14.90293f,  14.902949f, -2e-06f},
    { 10.606993f,  10.606997f,    0.0f},
    {  9.933711f,   16.41687f,    0.0f},
    {   5.16168f,  11.139565f,    0.0f},
    {  4.899414f,  17.352983f,    0.0f},
    {       0.0f,  11.423194f,    0.0f},
    {       0.0f,   17.76748f, -2e-06f},
    {  -5.16168f,  11.139561f, -2e-06f},
    { -4.899434f,   17.35298f, -2e-06f},
    {  -5.16168f,  11.139561f, -2e-06f},
    {  -9.93375f,   16.41687f, -2e-06f},
    {  -5.16168f,  11.139561f, -2e-06f},
    {-10.607012f,  10.606997f,    0.0f},
    {  -5.34584f,   5.345845f,    0.0f},
    { -11.13959f,   5.161695f,    0.0f},
    { -5.537441f,        0.0f,    0.0f},
    {-11.423183f,        0.0f,    0.0f},
    {  -5.34586f,  -5.345845f,    0.0f},
    { -11.13959f,  -5.161695f,    0.0f},
    {  -5.34586f,  -5.345845f,    0.0f},
    {-10.607012f, -10.606997f,    0.0f},
    {  -5.34586f,  -5.345845f,    0.0f},
    {  -5.16168f, -11.139605f,    0.0f},
    {       0.0f,  -5.537441f,    0.0f},
    {       0.0f, -11.423194f,    0.0f},
    {   5.34586f,  -5.345848f, -2e-06f},
    {   5.16168f, -11.139571f,    0.0f},
    {   5.34586f,  -5.345848f, -2e-06f},
    { 10.606993f, -10.606997f,    0.0f},
    {   5.34586f,  -5.345848f, -2e-06f},
    { 11.139609f,  -5.161695f,    0.0f},
    {  5.537422f,        0.0f,    0.0f},
    { 11.423164f,       -0.0f,    0.0f},
    {   5.34586f,   5.345848f, -2e-06f},
    { 11.139609f,   5.161695f, -2e-06f},
    {   5.34586f,   5.345848f, -2e-06f},
    { 10.606993f,  10.606997f,    0.0f},
    {   5.34586f,   5.345848f, -2e-06f},
    {   5.16168f,  11.139565f,    0.0f},
    {       0.0f,   5.537441f,    0.0f},
    {       0.0f,  11.423194f,    0.0f},
    {       0.0f,   5.537441f,    0.0f},
    {  -5.16168f,  11.139561f, -2e-06f},
    {       0.0f,   5.537441f,    0.0f},
    {  -5.34584f,   5.345845f,    0.0f},
    {       0.0f,        0.0f,    0.0f},
    { -5.537441f,        0.0f,    0.0f},
    {       0.0f,        0.0f,    0.0f},
};

f32 ripple_pair_template_vertices_1[11][3] __asm__("D_L17_001DA730") = {
    {-5.537441f,       0.0f,    0.0f},
    {      0.0f,       0.0f,    0.0f},
    { -5.34586f, -5.345845f,    0.0f},
    {      0.0f,       0.0f,    0.0f},
    {      0.0f, -5.537441f,    0.0f},
    {      0.0f,       0.0f,    0.0f},
    {  5.34586f, -5.345848f, -2e-06f},
    {      0.0f,       0.0f,    0.0f},
    { 5.537422f,       0.0f,    0.0f},
    {      0.0f,  5.537441f,    0.0f},
    {  5.34586f,  5.345848f, -2e-06f},
};

int ripple_pair_template_vertex_tables[2] __asm__("D_L17_00162230") = {
    (int)ripple_pair_template_vertices_0, (int)ripple_pair_template_vertices_1};

f32 ripple_pair_template_uvs_0[148][2] __asm__("D_L17_001DA7B8") = {
    { 3.639759f,  3.639754f},
    { 4.191267f,  2.966426f},
    { 2.966426f,  4.191267f},
    { 3.208478f,  3.208475f},
    { 2.199227f,  4.602288f},
    { 2.305368f,  3.483617f},
    { 1.366097f,  4.854145f},
    { 1.390425f,  3.653747f},
    {      0.5f,  4.940284f},
    {      0.5f,  3.729077f},
    {-0.366097f,  4.854145f},
    {-0.390425f,  3.653747f},
    {-1.199227f,  4.602288f},
    {-1.305368f,  3.483617f},
    {-1.966426f,  4.191267f},
    {-2.208478f,  3.208475f},
    {-2.639752f,  3.639754f},
    {-2.208478f,  3.208475f},
    {-3.191273f,  2.966426f},
    {-2.483618f,  2.305368f},
    {-3.602296f,  2.199227f},
    {-2.653749f,  1.390424f},
    {-3.854147f,  1.366094f},
    {-2.729081f,       0.5f},
    {-3.940284f,       0.5f},
    { -2.65375f, -0.390424f},
    {-3.854147f, -0.366087f},
    {-2.483618f, -1.305361f},
    {-3.602296f,  -1.19922f},
    {-2.208478f, -2.208471f},
    {-3.191273f, -1.966429f},
    {-2.208478f, -2.208471f},
    {-2.639752f,  -2.63975f},
    {-2.208478f, -2.208471f},
    {-1.966426f, -3.191263f},
    {-1.305368f, -2.483617f},
    {-1.199227f, -3.602288f},
    {-0.390424f, -2.653743f},
    {-0.366097f, -3.854148f},
    {      0.5f, -2.729073f},
    {      0.5f, -3.940284f},
    { 1.390424f, -2.653743f},
    { 1.366096f, -3.854148f},
    { 2.305367f, -2.483617f},
    { 2.199227f, -3.602288f},
    { 3.208477f, -2.208471f},
    { 2.966426f, -3.191263f},
    { 3.208477f, -2.208471f},
    { 3.639759f,  -2.63975f},
    { 3.208477f, -2.208471f},
    { 4.191267f, -1.966429f},
    { 3.483618f, -1.305361f},
    { 4.602289f,  -1.19922f},
    { 3.653749f, -0.390424f},
    { 4.854146f, -0.366087f},
    {  3.72908f,       0.5f},
    { 4.940284f,       0.5f},
    { 3.653748f,  1.390427f},
    { 4.854146f,  1.366095f},
    { 3.483618f,  2.305368f},
    { 4.602289f,  2.199227f},
    { 3.483618f,  2.305368f},
    { 4.191267f,  2.966426f},
    { 3.483618f,  2.305368f},
    { 3.208478f,  3.208475f},
    { 2.427726f,  2.427727f},
    { 2.305368f,  3.483617f},
    { 1.438092f,  2.524518f},
    { 1.390425f,  3.653747f},
    {      0.5f,  2.576059f},
    {      0.5f,  3.729077f},
    {-0.438092f,  2.524518f},
    {-0.390425f,  3.653747f},
    {-1.427726f,  2.427727f},
    {-1.305368f,  3.483617f},
    {-1.427726f,  2.427727f},
    {-2.208478f,  3.208475f},
    {-1.427726f,  2.427727f},
    {-2.483618f,  2.305368f},
    {-1.524523f,  1.438088f},
    {-2.653749f,  1.390424f},
    {-1.576063f,       0.5f},
    {-2.729081f,       0.5f},
    {-1.524517f, -0.438088f},
    { -2.65375f, -0.390424f},
    {-1.427726f, -1.427724f},
    {-2.483618f, -1.305361f},
    {-1.427726f, -1.427724f},
    {-2.208478f, -2.208471f},
    {-1.427726f, -1.427724f},
    {-1.305368f, -2.483617f},
    {-0.438091f, -1.524522f},
    {-0.390424f, -2.653743f},
    {      0.5f, -1.576055f},
    {      0.5f, -2.729073f},
    { 1.438092f, -1.524522f},
    { 1.390424f, -2.653743f},
    { 2.427726f, -1.427724f},
    { 2.305367f, -2.483617f},
    { 2.427726f, -1.427724f},
    { 3.208477f, -2.208471f},
    { 2.427726f, -1.427724f},
    { 3.483618f, -1.305361f},
    { 2.524516f, -0.438088f},
    { 3.653749f, -0.390424f},
    { 2.576063f,       0.5f},
    {  3.72908f,       0.5f},
    { 2.524515f,  1.438088f},
    { 3.653748f,  1.390427f},
    { 2.524515f,  1.438088f},
    { 3.483618f,  2.305368f},
    { 2.524515f,  1.438088f},
    { 2.427726f,  2.427727f},
    { 1.471559f,  1.471557f},
    { 1.438092f,  2.524518f},
    {      0.5f,  1.506379f},
    {      0.5f,  2.576059f},
    {-0.471559f,  1.471561f},
    {-0.438092f,  2.524518f},
    {-0.471559f,  1.471561f},
    {-1.427726f,  2.427727f},
    {-0.471559f,  1.471561f},
    {-1.524523f,  1.438088f},
    { -0.50638f,       0.5f},
    {-1.576063f,       0.5f},
    { -0.47156f, -0.471561f},
    {-1.524517f, -0.438088f},
    { -0.47156f, -0.471561f},
    {-1.427726f, -1.427724f},
    { -0.47156f, -0.471561f},
    {-0.438091f, -1.524522f},
    {      0.5f, -0.506376f},
    {      0.5f, -1.576055f},
    {  1.47156f, -0.471561f},
    { 1.438092f, -1.524522f},
    {  1.47156f, -0.471561f},
    { 2.427726f, -1.427724f},
    {  1.47156f, -0.471561f},
    { 2.524516f, -0.438088f},
    {  1.50638f,       0.5f},
    { 2.576063f,       0.5f},
    {  1.50638f,       0.5f},
    { 2.524515f,  1.438088f},
    {  1.50638f,       0.5f},
    { 1.471559f,  1.471557f},
    {      0.5f,       0.5f},
    {      0.5f,  1.506379f},
    {      0.5f,       0.5f},
};

f32 ripple_pair_template_uvs_1[11][2] __asm__("D_L17_001DAC58") = {
    {      0.5f,  1.506379f},
    {      0.5f,       0.5f},
    {-0.471559f,  1.471561f},
    {      0.5f,       0.5f},
    { -0.50638f,       0.5f},
    {      0.5f,       0.5f},
    { -0.47156f, -0.471561f},
    {      0.5f,       0.5f},
    {      0.5f, -0.506376f},
    {  1.50638f,       0.5f},
    {  1.47156f, -0.471561f},
};

int ripple_pair_template_uv_tables[2] __asm__("D_L17_00162238") = {(int)ripple_pair_template_uvs_0,
                                                                   (int)ripple_pair_template_uvs_1};

void draw_moby_radial_ripple_mesh_pair(struct Moby *moby) __asm__("FUN_L17_002f04d0");

void draw_moby_radial_ripple_mesh_pair(struct Moby *moby) {
    RippleData *ripple_vars = (RippleData *)moby->pvars;
    float       scale;
    int         r, g, b;
    int         color;
    int         i, j, uv_strip_index, uv_vertex_index, draw_strip_index;

    scale = convert_integer_to_float(D_L17_0015F5CC) * (ripple_pair_uv_scroll_rate_gp * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(ripple_pair_effect_texture_id));
    vu1_add_g_sregister(0x42, ((long)ripple_pair_alpha_1_fix << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    ripple_vars->f0 = ripple_vars->f4;
    ripple_vars->f4 = fast_add_rotations(ripple_vars->f4,
                                         360.0f / scale_time(ripple_pair_phase_period_gp) *
                                             DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (ripple_pair_vertex_alpha << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < ripple_pair_vertex_counts[i]; j++) {
            int   *template_vertex_array_slot = ripple_pair_template_vertex_tables + i;
            float *p = (float *)(*template_vertex_array_slot + j * 12);
            float  t = ripple_pair_z_amplitude_gp *
                      fast_sin(fast_add_rotations(
                          fast_normalize_angle(
                              FUN_L00_00200260(square_root(p[0] * p[0] + p[1] * p[1]),
                                               ripple_pair_radial_phase_distance_scale_gp) *
                              6.2831855f / ripple_pair_radial_phase_distance_scale_gp),
                          ripple_vars->f4));
            *(float *)(j * 12 + ripple_pair_world_vertex_tables[i]) = p[0] + moby->pos.x;
            *(float *)(j * 12 + ripple_pair_world_vertex_tables[i] + 4) = p[1] + moby->pos.y;
            *(float *)(j * 12 + ripple_pair_world_vertex_tables[i] + 8) = p[2] + moby->pos.z + t;
            *(int *)(j * 4 + ripple_pair_colour_tables[i]) = color;
        }
    }
    for (i = 0; i < 2; i++) {
        for (uv_strip_index = 0; uv_strip_index < 2; uv_strip_index++) {
            for (uv_vertex_index = 0; uv_vertex_index < ripple_pair_vertex_counts[uv_strip_index];
                 uv_vertex_index++) {
                T4 uv_scroll_factors = ripple_pair_uv_scroll_factors;
                *(float *)(uv_vertex_index * 8 + ripple_pair_scrolled_uv_tables[uv_strip_index]) =
                    *(float *)(uv_vertex_index * 8 +
                               ripple_pair_template_uv_tables[uv_strip_index]) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[0], 1.0f);
                *(float *)(uv_vertex_index * 8 + ripple_pair_scrolled_uv_tables[uv_strip_index] +
                           4) = *(float *)(uv_vertex_index * 8 +
                                           ripple_pair_template_uv_tables[uv_strip_index] + 4) +
                                FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[1], 1.0f);
            }
        }
        font_queue_vu_state();
        for (draw_strip_index = 0; draw_strip_index < 2; draw_strip_index++) {
            FUN_L00_001fde98(ripple_pair_vertex_counts[draw_strip_index],
                             ripple_pair_world_vertex_tables[draw_strip_index],
                             ripple_pair_colour_tables[draw_strip_index],
                             (void *)ripple_pair_scrolled_uv_tables[draw_strip_index],
                             1);
        }
    }
}

/* Builds the rippling 89-point ring around the moby (template points moved to its position, z waved by distance from the centre), then draws it twice with scrolled texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F2458), where it is exact; names translated to the US level program. */

typedef struct {
    float f0;
    float f4;
} RippleData_c;

typedef struct {
    float v[2];
} P2_c;

typedef struct {
    P2_c p[2];
} T4_c;

typedef struct {
    float x;
    float y;
    float z;
} RingPt;

/* The ring has ring_ripple_vertex_counts[0] = 89 vertices: draw_ripple_ring's loops run
   i < 89 over each of these streams. */
RingPt l17_ripple_ring_template_vertices[89] __asm__("D_L17_001DBE20") = {
    {-3.744727f,  12.785391f,  2e-06f},
    {-1.002891f,  15.031173f,  2e-06f},
    {-5.990547f,  10.043477f,  2e-06f},
    {-1.988516f,   11.02914f, -2e-06f},
    {-7.664258f,   6.919414f,  2e-06f},
    {-3.108945f,    7.35164f,  2e-06f},
    {-8.689883f,   3.526836f,  2e-06f},
    {-3.801718f,   3.625859f,  2e-06f},
    {-9.040626f,        0.0f,  2e-06f},
    {-4.108477f,        0.0f,  2e-06f},
    {-8.689883f,  -3.526836f,  2e-06f},
    {-3.801718f,  -3.625898f,  2e-06f},
    {-7.664258f,  -6.919414f,  2e-06f},
    {-3.108945f,   -7.35164f, -2e-06f},
    {-5.990547f, -10.043516f,  2e-06f},
    {-1.988516f,  -11.02918f, -2e-06f},
    {-3.744727f, -12.785352f,  2e-06f},
    {-1.988516f,  -11.02918f, -2e-06f},
    {-1.002891f, -15.031211f,  2e-06f},
    { 1.688985f,  -12.14957f,  2e-06f},
    { 2.121211f, -16.704924f,  2e-06f},
    { 5.414727f, -12.842343f,  2e-06f},
    { 5.513789f, -17.730469f,  2e-06f},
    { 5.414727f, -12.842343f,  2e-06f},
    { 9.040626f, -18.081251f,  2e-06f},
    { 5.414727f, -12.842343f,  2e-06f},
    { 9.040626f, -13.149141f,  2e-06f},
    { 5.220665f,  -8.244023f,  2e-06f},
    { 9.040626f,  -8.453906f,  2e-06f},
    { 5.084336f,  -3.956289f,  2e-06f},
    { 9.040626f,  -4.098125f,  2e-06f},
    { 4.942539f,        0.0f,  2e-06f},
    { 9.040626f,        0.0f,  2e-06f},
    { 5.084336f,   3.956289f,  2e-06f},
    { 9.040626f,   4.098047f,  2e-06f},
    { 5.220665f,   8.243984f, -2e-06f},
    { 9.040626f,   8.453906f,  2e-06f},
    { 5.414727f,  12.842305f, -2e-06f},
    { 9.040626f,  13.149101f, -2e-06f},
    { 5.414727f,  12.842305f, -2e-06f},
    { 9.040626f,  18.081251f,  2e-06f},
    { 5.414727f,  12.842305f, -2e-06f},
    { 5.513789f,  17.730429f,  2e-06f},
    { 1.688985f,   12.14957f, -2e-06f},
    { 2.121211f,  16.704844f,  2e-06f},
    { 1.688985f,   12.14957f, -2e-06f},
    {-1.002891f,  15.031173f,  2e-06f},
    { 1.688985f,   12.14957f, -2e-06f},
    {-1.988516f,   11.02914f, -2e-06f},
    { 1.688985f,   12.14957f, -2e-06f},
    {-3.108945f,    7.35164f,  2e-06f},
    { 1.688985f,   12.14957f, -2e-06f},
    { 1.190781f,   7.849844f,  2e-06f},
    { 5.414727f,  12.842305f, -2e-06f},
    { 1.190781f,   7.849844f,  2e-06f},
    { 5.220665f,   8.243984f, -2e-06f},
    { 1.190781f,   7.849844f,  2e-06f},
    { 5.084336f,   3.956289f,  2e-06f},
    { 1.190781f,   7.849844f,  2e-06f},
    { 0.796562f,   3.819961f,  2e-06f},
    {-3.108945f,    7.35164f,  2e-06f},
    { 0.796562f,   3.819961f,  2e-06f},
    {-3.801718f,   3.625859f,  2e-06f},
    { 0.796562f,   3.819961f,  2e-06f},
    {-4.108477f,        0.0f,  2e-06f},
    { 0.796562f,   3.819961f,  2e-06f},
    {  0.58668f,        0.0f,  2e-06f},
    { 5.084336f,   3.956289f,  2e-06f},
    {  0.58668f,        0.0f,  2e-06f},
    { 4.942539f,        0.0f,  2e-06f},
    {  0.58668f,        0.0f,  2e-06f},
    { 5.084336f,  -3.956289f,  2e-06f},
    {  0.58668f,        0.0f,  2e-06f},
    { 0.796562f,      -3.82f,  2e-06f},
    {-4.108477f,        0.0f,  2e-06f},
    { 0.796562f,      -3.82f,  2e-06f},
    {-3.801718f,  -3.625898f,  2e-06f},
    { 1.190781f,  -7.849883f,  2e-06f},
    {-3.108945f,   -7.35164f, -2e-06f},
    { 1.190781f,  -7.849883f,  2e-06f},
    {-1.988516f,  -11.02918f, -2e-06f},
    { 1.190781f,  -7.849883f,  2e-06f},
    { 1.688985f,  -12.14957f,  2e-06f},
    { 1.190781f,  -7.849883f,  2e-06f},
    { 5.414727f, -12.842343f,  2e-06f},
    { 1.190781f,  -7.849883f,  2e-06f},
    { 5.220665f,  -8.244023f,  2e-06f},
    { 0.796562f,      -3.82f,  2e-06f},
    { 5.084336f,  -3.956289f,  2e-06f},
};

RingPt l17_ripple_ring_world_vertices[89] __asm__("D_L17_001DC680") = {0};

T4_c ring_ripple_uv_scroll_factors __asm__("D_L17_001622D0") = {
    {{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}
};

#include "rnc/overlay/l17/mobys/radial_ripple_mesh_data/draw_ripple_ring_2.h"

float ring_template_uvs[89][2] __asm__("D_L17_001DC250") = {
    { 3.341198f,  1.332162f},
    { 3.840261f,  0.722865f},
    { 2.731884f,  1.831233f},
    {  2.95092f,  0.941892f},
    { 2.037648f,  2.203168f},
    { 2.133698f,  1.190877f},
    { 1.283741f,  2.431085f},
    { 1.305747f,  1.344826f},
    {      0.5f,  2.509028f},
    {      0.5f,  1.412995f},
    {-0.283741f,  2.431085f},
    {-0.305755f,  1.344826f},
    {-1.037648f,  2.203168f},
    {-1.133698f,  1.190877f},
    {-1.731892f,  1.831233f},
    {-1.950929f,  0.941892f},
    {-2.341189f,  1.332161f},
    {-1.950929f,  0.941892f},
    {-2.840269f,  0.722865f},
    {-2.199905f,   0.12467f},
    {-3.212205f,   0.02862f},
    {-2.353854f, -0.703273f},
    {-3.440104f, -0.725286f},
    {-2.353854f, -0.703273f},
    {-3.518056f, -1.509028f},
    {-2.353854f, -0.703273f},
    {-2.422031f, -1.509028f},
    {-1.332005f, -0.660148f},
    {-1.378646f, -1.509028f},
    {-0.379175f, -0.629852f},
    {-0.410694f, -1.509028f},
    {      0.5f, -0.598342f},
    {      0.5f, -1.509028f},
    { 1.379175f, -0.629852f},
    { 1.410677f, -1.509028f},
    { 2.331997f, -0.660148f},
    { 2.378646f, -1.509028f},
    { 3.353846f, -0.703273f},
    { 3.422023f, -1.509028f},
    { 3.353846f, -0.703273f},
    { 4.518056f, -1.509028f},
    { 3.353846f, -0.703273f},
    { 4.440096f, -0.725286f},
    { 3.199905f,   0.12467f},
    { 4.212188f,   0.02862f},
    { 3.199905f,   0.12467f},
    { 3.840261f,  0.722865f},
    { 3.199905f,   0.12467f},
    {  2.95092f,  0.941892f},
    { 3.199905f,   0.12467f},
    { 2.133698f,  1.190877f},
    { 3.199905f,   0.12467f},
    {  2.24441f,  0.235382f},
    { 3.353846f, -0.703273f},
    {  2.24441f,  0.235382f},
    { 2.331997f, -0.660148f},
    {  2.24441f,  0.235382f},
    { 1.379175f, -0.629852f},
    {  2.24441f,  0.235382f},
    {  1.34888f,  0.322986f},
    { 2.133698f,  1.190877f},
    {  1.34888f,  0.322986f},
    { 1.305747f,  1.344826f},
    {  1.34888f,  0.322986f},
    {      0.5f,  1.412995f},
    {  1.34888f,  0.322986f},
    {      0.5f,  0.369627f},
    { 1.379175f, -0.629852f},
    {      0.5f,  0.369627f},
    {      0.5f, -0.598342f},
    {      0.5f,  0.369627f},
    {-0.379175f, -0.629852f},
    {      0.5f,  0.369627f},
    {-0.348889f,  0.322986f},
    {      0.5f,  1.412995f},
    {-0.348889f,  0.322986f},
    {-0.305755f,  1.344826f},
    {-1.244419f,  0.235382f},
    {-1.133698f,  1.190877f},
    {-1.244419f,  0.235382f},
    {-1.950929f,  0.941892f},
    {-1.244419f,  0.235382f},
    {-2.199905f,   0.12467f},
    {-1.244419f,  0.235382f},
    {-2.353854f, -0.703273f},
    {-1.244419f,  0.235382f},
    {-1.332005f, -0.660148f},
    {-0.348889f,  0.322986f},
    {-0.379175f, -0.629852f},
};

float ring_scrolled_uvs[89][2] __asm__("D_L17_001DCAB0") = {0};

int ring_ripple_vertex_counts[2] __asm__("D_L17_00162280") = {89, 0};

int ring_ripple_world_vertex_tables[2] __asm__("D_L17_001622A0") = {
    (int)l17_ripple_ring_world_vertices, 0};

int ring_ripple_scrolled_uv_tables[2] __asm__("D_L17_001622A8") = {(int)ring_scrolled_uvs, 0};

int ring_vertex_colors[89] __asm__("D_L17_001DCD78") = {0};

/* draw_ripple_ring reads entry 0 only. */
int ring_ripple_colour_tables[2] __asm__("D_L17_001622B0") = {(int)ring_vertex_colors, 0x80};

int ring_ripple_alpha_1_fix __asm__("D_L17_001622B4") = 128;
int ring_ripple_effect_texture_id __asm__("D_L17_001622B8") = 45;
int ring_ripple_vertex_alpha __asm__("D_L17_001622BC") = 64;

#include "rnc/overlay/l17/mobys/radial_ripple_mesh_data/draw_ripple_ring.h"

void draw_ripple_ring(struct Moby *moby) __asm__("FUN_L17_002f0948");

void draw_ripple_ring(struct Moby *moby) {
    RippleData_c *ring_ripple_vars = (RippleData_c *)moby->pvars;
    T4_c          ring_uv_scroll_factors;
    float         scale;
    int           r, g, b;
    int           color;
    int           i, j, ring_uv_vertex_index, k;

    scale = convert_integer_to_float(D_L17_0015F5CC) * (ring_ripple_uv_scroll_rate_gp * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(ring_ripple_effect_texture_id));
    vu1_add_g_sregister(0x42, ((long)ring_ripple_alpha_1_fix << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    ring_ripple_vars->f0 = ring_ripple_vars->f4;
    ring_ripple_vars->f4 = fast_add_rotations(ring_ripple_vars->f4,
                                              360.0f / scale_time(ring_ripple_phase_period_gp) *
                                                  DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (ring_ripple_vertex_alpha << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < ring_ripple_vertex_counts[0]; i++) {
        RingPt *p = &l17_ripple_ring_template_vertices[i];
        float   w =
            ring_ripple_z_amplitude_gp *
            fast_sin(fast_add_rotations(
                fast_normalize_angle(FUN_L00_00200260(square_root(p->x * p->x + p->y * p->y),
                                                      ring_ripple_radial_phase_distance_scale_gp) *
                                     6.2831855f / ring_ripple_radial_phase_distance_scale_gp),
                ring_ripple_vars->f4));
        l17_ripple_ring_world_vertices[i].x = p->x + moby->pos.x;
        l17_ripple_ring_world_vertices[i].y = p->y + moby->pos.y;
        l17_ripple_ring_world_vertices[i].z = p->z + moby->pos.z + w;
        ring_vertex_colors[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (ring_uv_vertex_index = 0; ring_uv_vertex_index < ring_ripple_vertex_counts[0];
             ring_uv_vertex_index++) {
            ring_uv_scroll_factors = ring_ripple_uv_scroll_factors;
            ring_scrolled_uvs[ring_uv_vertex_index][0] =
                ring_template_uvs[ring_uv_vertex_index][0] +
                FUN_L00_00200260(scale * ring_uv_scroll_factors.p[j].v[0], 1.0f);
            ring_scrolled_uvs[ring_uv_vertex_index][1] =
                ring_template_uvs[ring_uv_vertex_index][1] +
                FUN_L00_00200260(scale * ring_uv_scroll_factors.p[j].v[1], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(ring_ripple_vertex_counts[k],
                             ring_ripple_world_vertex_tables[k],
                             ring_ripple_colour_tables[k],
                             (void *)ring_ripple_scrolled_uv_tables[k],
                             1);
        }
    }
}

/* Builds and draws the animated ring of lights around a moby: 45 vertices and colours, then two passes of 45 texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F2BD8), where it is exact; names translated to the US level program. */

typedef struct {
    float f[4];
} Quad4;

typedef struct {
    float prev;
    float angle;
} RingVars;

Quad4 radial_ripple_uv_scroll_factors __asm__("D_L17_00162390") = {
    {1.0f, -0.62831855f, 1.0f, 0.62831855f}
};

extern float ripple_uv_scroll_rate_gp __asm__("D_L17_0016238C") __attribute__((sda));

float radial_ripple_template_vertices[135] __asm__("D_L17_001DD760") = {
    -7.788906f, -7.788906f, 0.005259f,  -7.788906f, -3.894453f, 0.005259f,  -3.894453f, -7.788906f,
    0.005259f,  -3.894453f, -3.894453f, 0.005259f,  0.0f,       -7.788906f, 0.005259f,  0.0f,
    -3.894453f, 0.005259f,  3.894453f,  -7.788906f, 0.005259f,  3.894453f,  -3.894453f, 0.005259f,
    7.788906f,  -7.788906f, 0.005259f,  3.894453f,  -3.894453f, 0.005259f,  7.788906f,  -3.894453f,
    0.005259f,  3.894453f,  0.0f,       0.005259f,  7.788906f,  0.0f,       0.005259f,  3.894453f,
    3.894453f,  0.005259f,  7.788906f,  3.894453f,  0.005259f,  3.894453f,  3.894453f,  0.005259f,
    7.788906f,  7.788906f,  0.005259f,  3.894453f,  3.894453f,  0.005259f,  3.894453f,  7.788906f,
    0.005259f,  0.0f,       3.894453f,  0.005259f,  0.0f,       7.788906f,  0.005259f,  -3.894453f,
    3.894453f,  0.005259f,  -3.894453f, 7.788906f,  0.005259f,  -3.894453f, 3.894453f,  0.005259f,
    -7.788906f, 7.788906f,  0.005259f,  -3.894453f, 3.894453f,  0.005259f,  -7.788906f, 3.894453f,
    0.005259f,  -3.894453f, 0.0f,       0.005259f,  -7.788906f, 0.0f,       0.005259f,  -3.894453f,
    0.0f,       0.005259f,  -7.788906f, -3.894453f, 0.005259f,  -3.894453f, 0.0f,       0.005259f,
    -3.894453f, -3.894453f, 0.005259f,  0.0f,       0.0f,       0.005259f,  0.0f,       -3.894453f,
    0.005259f,  0.0f,       0.0f,       0.005259f,  3.894453f,  -3.894453f, 0.005259f,  0.0f,
    0.0f,       0.005259f,  3.894453f,  0.0f,       0.005259f,  0.0f,       0.0f,       0.005259f,
    3.894453f,  3.894453f,  0.005259f,  0.0f,       0.0f,       0.005259f,  0.0f,       3.894453f,
    0.005259f,  -3.894453f, 0.0f,       0.005259f,  -3.894453f, 3.894453f,  0.005259f};

float radial_ripple_template_uvs[45][2] __asm__("D_L17_001DD980") = {
    { 0.0f,  0.0f},
    { 0.0f, 0.25f},
    {0.25f,  0.0f},
    {0.25f, 0.25f},
    { 0.5f,  0.0f},
    { 0.5f, 0.25f},
    {0.75f,  0.0f},
    {0.75f, 0.25f},
    { 1.0f,  0.0f},
    {0.75f, 0.25f},
    { 1.0f, 0.25f},
    {0.75f,  0.5f},
    { 1.0f,  0.5f},
    {0.75f, 0.75f},
    { 1.0f, 0.75f},
    {0.75f, 0.75f},
    { 1.0f,  1.0f},
    {0.75f, 0.75f},
    {0.75f,  1.0f},
    { 0.5f, 0.75f},
    { 0.5f,  1.0f},
    {0.25f, 0.75f},
    {0.25f,  1.0f},
    {0.25f, 0.75f},
    { 0.0f,  1.0f},
    {0.25f, 0.75f},
    { 0.0f, 0.75f},
    {0.25f,  0.5f},
    { 0.0f,  0.5f},
    {0.25f,  0.5f},
    { 0.0f, 0.25f},
    {0.25f,  0.5f},
    {0.25f, 0.25f},
    { 0.5f,  0.5f},
    { 0.5f, 0.25f},
    { 0.5f,  0.5f},
    {0.75f, 0.25f},
    { 0.5f,  0.5f},
    {0.75f,  0.5f},
    { 0.5f,  0.5f},
    {0.75f, 0.75f},
    { 0.5f,  0.5f},
    { 0.5f, 0.75f},
    {0.25f,  0.5f},
    {0.25f, 0.75f},
};

float radial_ripple_world_vertices[135] __asm__("D_L17_001DDBA0") = {0};

float radial_ripple_scrolled_uvs[45][2] __asm__("D_L17_001DDDC0") = {0};

int radial_ripple_vertex_counts[2] __asm__("D_L17_00162340") = {45, 0};

int radial_ripple_world_vertex_tables[2] __asm__("D_L17_00162360") = {
    (int)radial_ripple_world_vertices, 0};

int radial_ripple_scrolled_uv_tables[2] __asm__("D_L17_00162368") = {
    (int)radial_ripple_scrolled_uvs, 0};

int radial_ripple_colours[45] __asm__("D_L17_001DDF28") = {0};

int radial_ripple_colour_tables[1] __asm__("D_L17_00162370") = {(int)radial_ripple_colours};

int          radial_ripple_alpha_1_fix __asm__("D_L17_00162374") = 128;
int          radial_ripple_effect_texture_id __asm__("D_L17_00162378") = 45;
int          radial_ripple_vertex_alpha __asm__("D_L17_0016237C") = 64;
extern float ripple_radial_phase_distance_scale_gp __asm__("D_L17_00162380") __attribute__((sda));
extern float ripple_phase_period_gp __asm__("D_L17_00162384") __attribute__((sda));
extern float ripple_z_amplitude_gp __asm__("D_L17_00162388") __attribute__((sda));

void draw_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L17_002f10c8");

void draw_moby_radial_ripple_mesh(struct Moby *moby) {
    unsigned int i = 0;
    unsigned int output_vertex_byte_offset = 0;
    RingVars    *ripple_phase_vars = (RingVars *)moby->pvars;
    unsigned int uv_vertex_index;
    int          j;
    int          k;
    int          color;
    Quad4        uv_scroll_factors;
    int          r;
    int          g;
    int          b;
    float        scale;
    Quad4       *v_scroll_factors;
    float       *output_vertex;
    scale = convert_integer_to_float(D_L17_0015F5CC) * (ripple_uv_scroll_rate_gp * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(radial_ripple_effect_texture_id));
    vu1_add_g_sregister(0x42, ((long)radial_ripple_alpha_1_fix << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    ripple_phase_vars->prev = ripple_phase_vars->angle;
    ripple_phase_vars->angle =
        fast_add_rotations(ripple_phase_vars->angle,
                           360.0f / scale_time(ripple_phase_period_gp) * DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    /* The second float of each pair in t: p->f[0] is t.f[1], p->f[2] is t.f[3]. */
    v_scroll_factors = (Quad4 *)&uv_scroll_factors.f[1];
    color = (radial_ripple_vertex_alpha << 24) | (b << 16) | (g << 8) | r;
    output_vertex = radial_ripple_world_vertices;
    do {
        float *template_vertex = &radial_ripple_template_vertices[i * 3];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radial_phase_input = FUN_L00_00200260(rad, ripple_radial_phase_distance_scale_gp);
        float  w = fast_normalize_angle(radial_phase_input * 6.2831855f /
                                       ripple_radial_phase_distance_scale_gp);
        float  z = fast_sin(fast_add_rotations(w, ripple_phase_vars->angle));
        output_vertex[0] = template_vertex[0] + moby->pos.x;
        *(float *)((char *)radial_ripple_world_vertices + 4 + output_vertex_byte_offset) =
            template_vertex[1] + moby->pos.y;
        output_vertex[2] = template_vertex[2] + moby->pos.z + ripple_z_amplitude_gp * z;
        radial_ripple_colours[i] = color;
        i++;
        output_vertex_byte_offset += 12;
        output_vertex += 3;
    } while (i < 45);
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 45; uv_vertex_index++) {
            uv_scroll_factors = radial_ripple_uv_scroll_factors;
            radial_ripple_scrolled_uvs[uv_vertex_index][0] =
                radial_ripple_template_uvs[uv_vertex_index][0] +
                FUN_L00_00200260(scale * uv_scroll_factors.f[j * 2], 1.0f);
            radial_ripple_scrolled_uvs[uv_vertex_index][1] =
                radial_ripple_template_uvs[uv_vertex_index][1] +
                FUN_L00_00200260(scale * v_scroll_factors->f[j * 2], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(radial_ripple_vertex_counts[k],
                             radial_ripple_world_vertex_tables[k],
                             radial_ripple_colour_tables[k],
                             (void *)radial_ripple_scrolled_uv_tables[k],
                             1);
        }
    }
}

/* The second copy of draw_ripple_ring (at 0x2F0D08) runs the same code over
   its own tables (ring_ripple_copy2_*): 45 vertices, the world vertex,
   scrolled uv and colour tables, and the templates and template uvs at the
   addresses the copy loads. The k < 1 draw loop reads entry 0 of each table. */
int ring_ripple_copy2_vertex_counts[1] __asm__("D_L17_001622E0") = {45};

extern RingPt ring_ripple_copy2_world_vertices[45] __asm__("D_L17_001DD320");
extern float  ring_ripple_copy2_scrolled_uvs[45][2] __asm__("D_L17_001DD540");
extern int    ring_ripple_copy2_colours[45] __asm__("D_L17_001DD6A8");

int ring_ripple_copy2_world_vertex_tables[1] __asm__("D_L17_00162300") = {
    (int)ring_ripple_copy2_world_vertices};

int ring_ripple_copy2_scrolled_uv_tables[1] __asm__("D_L17_00162308") = {
    (int)ring_ripple_copy2_scrolled_uvs};

int ring_ripple_copy2_colour_tables[1] __asm__("D_L17_00162310") = {(int)ring_ripple_copy2_colours};

RingPt ring_ripple_copy2_template_vertices[45] __asm__("D_L17_001DCEE0") = {
    {-7.788906f, -7.788906f, 0.005259f},
    {-7.788906f, -3.894453f, 0.005259f},
    {-3.894453f, -7.788906f, 0.005259f},
    {-3.894453f, -3.894453f, 0.005259f},
    {      0.0f, -7.788906f, 0.005259f},
    {      0.0f, -3.894453f, 0.005259f},
    { 3.894453f, -7.788906f, 0.005259f},
    { 3.894453f, -3.894453f, 0.005259f},
    { 7.788906f, -7.788906f, 0.005259f},
    { 3.894453f, -3.894453f, 0.005259f},
    { 7.788906f, -3.894453f, 0.005259f},
    { 3.894453f,       0.0f, 0.005259f},
    { 7.788906f,       0.0f, 0.005259f},
    { 3.894453f,  3.894453f, 0.005259f},
    { 7.788906f,  3.894453f, 0.005259f},
    { 3.894453f,  3.894453f, 0.005259f},
    { 7.788906f,  7.788906f, 0.005259f},
    { 3.894453f,  3.894453f, 0.005259f},
    { 3.894453f,  7.788906f, 0.005259f},
    {      0.0f,  3.894453f, 0.005259f},
    {      0.0f,  7.788906f, 0.005259f},
    {-3.894453f,  3.894453f, 0.005259f},
    {-3.894453f,  7.788906f, 0.005259f},
    {-3.894453f,  3.894453f, 0.005259f},
    {-7.788906f,  7.788906f, 0.005259f},
    {-3.894453f,  3.894453f, 0.005259f},
    {-7.788906f,  3.894453f, 0.005259f},
    {-3.894453f,       0.0f, 0.005259f},
    {-7.788906f,       0.0f, 0.005259f},
    {-3.894453f,       0.0f, 0.005259f},
    {-7.788906f, -3.894453f, 0.005259f},
    {-3.894453f,       0.0f, 0.005259f},
    {-3.894453f, -3.894453f, 0.005259f},
    {      0.0f,       0.0f, 0.005259f},
    {      0.0f, -3.894453f, 0.005259f},
    {      0.0f,       0.0f, 0.005259f},
    { 3.894453f, -3.894453f, 0.005259f},
    {      0.0f,       0.0f, 0.005259f},
    { 3.894453f,       0.0f, 0.005259f},
    {      0.0f,       0.0f, 0.005259f},
    { 3.894453f,  3.894453f, 0.005259f},
    {      0.0f,       0.0f, 0.005259f},
    {      0.0f,  3.894453f, 0.005259f},
    {-3.894453f,       0.0f, 0.005259f},
    {-3.894453f,  3.894453f, 0.005259f},
};

float ring_ripple_copy2_template_uvs[45][2] __asm__("D_L17_001DD100") = {
    { 0.0f,  0.0f},
    { 0.0f, 0.25f},
    {0.25f,  0.0f},
    {0.25f, 0.25f},
    { 0.5f,  0.0f},
    { 0.5f, 0.25f},
    {0.75f,  0.0f},
    {0.75f, 0.25f},
    { 1.0f,  0.0f},
    {0.75f, 0.25f},
    { 1.0f, 0.25f},
    {0.75f,  0.5f},
    { 1.0f,  0.5f},
    {0.75f, 0.75f},
    { 1.0f, 0.75f},
    {0.75f, 0.75f},
    { 1.0f,  1.0f},
    {0.75f, 0.75f},
    {0.75f,  1.0f},
    { 0.5f, 0.75f},
    { 0.5f,  1.0f},
    {0.25f, 0.75f},
    {0.25f,  1.0f},
    {0.25f, 0.75f},
    { 0.0f,  1.0f},
    {0.25f, 0.75f},
    { 0.0f, 0.75f},
    {0.25f,  0.5f},
    { 0.0f,  0.5f},
    {0.25f,  0.5f},
    { 0.0f, 0.25f},
    {0.25f,  0.5f},
    {0.25f, 0.25f},
    { 0.5f,  0.5f},
    { 0.5f, 0.25f},
    { 0.5f,  0.5f},
    {0.75f, 0.25f},
    { 0.5f,  0.5f},
    {0.75f,  0.5f},
    { 0.5f,  0.5f},
    {0.75f, 0.75f},
    { 0.5f,  0.5f},
    { 0.5f, 0.75f},
    {0.25f,  0.5f},
    {0.25f, 0.75f},
};

RingPt ring_ripple_copy2_world_vertices[45] __asm__("D_L17_001DD320") = {0};

float ring_ripple_copy2_scrolled_uvs[45][2] __asm__("D_L17_001DD540") = {0};

int ring_ripple_copy2_colours[45] __asm__("D_L17_001DD6A8") = {0};
