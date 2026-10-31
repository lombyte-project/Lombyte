/* Level 15 radial ripple mesh draws: the ripple strips' mesh buffers (queued by shared/props/ripple_strips.c), and the plain, scaled and 88-vertex scaled mesh draws. */
#include "types.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/game/level_state.h"
#include "rnc/overlay/l15/mobys/radial_ripple_mesh_globals.h"
#include "rnc/overlay/mesh_batch.h"

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");

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

T4 ripple_mesh_buffers_uv_scroll_factors __asm__("D_L15_00162108") = {
    {{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}
};

extern float FUN_L00_00200260(float, float);

extern float fast_add_rotations(float, float);

extern int D_L15_0015F5CC;

int ripple_mesh_buffers_alpha_1_fix __asm__("D_L15_001620EC") = 128;

int ripple_mesh_buffers_effect_texture_id __asm__("D_L15_001620F0") = 45;

int ripple_mesh_buffers_vertex_alpha __asm__("D_L15_001620F4") = 64;

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_moby_radial_ripple_mesh_buffers.h"

float ripple_mesh_buffers_z_amplitude_gp __asm__("D_L15_00162100") = 0.1f;

extern s64 get_effect_texture(s32) __asm__("func_001F44B8");

float ripple_mesh_buffers_uv_scroll_rate_gp __asm__("D_L15_00162104") = 0.1f;

extern void FUN_L00_001fde98(int, int, int, void *, int);

extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");

void font_queue_vu_state(void) __asm__("FUN_001f76a0");

/* The three strips of draw_moby_radial_ripple_mesh_buffers (its loops run
   i < 3 over every table): vertex counts 148, 148 and 31, the third read from
   the word after ripple_mesh_buffers_vertex_counts (31) and the third pointer after each table.
   Per vertex it reads a template position (3 floats, 12-byte stride) and a
   template uv (2 floats, 8-byte stride) and writes a world position, a
   scrolled uv and a colour word; FUN_L00_001fde98 reads the last three with
   the same strides. */
int ripple_mesh_buffers_vertex_counts[2] __asm__("D_L15_00162080") = {148, 148};

f32 ripple_mesh_buffers_world_vertices_0[148][3] __asm__("D_L15_001DBCF0") = {0};

f32 ripple_mesh_buffers_world_vertices_1[148][3] __asm__("D_L15_001DCAD0") = {0};

f32 ripple_mesh_buffers_world_vertices_2[31][3] __asm__("D_L15_001DD8B0") = {0};

int ripple_mesh_buffers_world_vertex_tables[2] __asm__("D_L15_00162090") = {
    (int)ripple_mesh_buffers_world_vertices_0, (int)ripple_mesh_buffers_world_vertices_1};

f32 ripple_mesh_buffers_scrolled_uvs_0[148][2] __asm__("D_L15_001DC3E0") = {0};

f32 ripple_mesh_buffers_scrolled_uvs_1[148][2] __asm__("D_L15_001DD1C0") = {0};

f32 ripple_mesh_buffers_scrolled_uvs_2[31][2] __asm__("D_L15_001DDA28") = {0};

int ripple_mesh_buffers_scrolled_uv_tables[2] __asm__("D_L15_001620A0") = {
    (int)ripple_mesh_buffers_scrolled_uvs_0, (int)ripple_mesh_buffers_scrolled_uvs_1};

u32 ripple_mesh_buffers_colours_0[148] __asm__("D_L15_001DC880") = {0};

u32 ripple_mesh_buffers_colours_1[148] __asm__("D_L15_001DD660") = {0};

u32 ripple_mesh_buffers_colours_2[31] __asm__("D_L15_001DDB20") = {0};

int ripple_mesh_buffers_colour_tables[2] __asm__("D_L15_001620B0") = {
    (int)ripple_mesh_buffers_colours_0, (int)ripple_mesh_buffers_colours_1};

f32 ripple_mesh_buffers_template_vertices_0[148][3] __asm__("D_L15_001D9E40") = {
    {  4.852857f, -18.111109f, 0.0f},
    {  1.226318f, -18.709852f, 0.0f},
    {  4.367578f, -16.299988f, 0.0f},
    {  1.103687f, -16.838867f, 0.0f},
    {    3.8823f, -14.488904f, 0.0f},
    {  0.976489f,  -14.95857f, 0.0f},
    {    3.8823f, -14.488904f, 0.0f},
    {  0.813721f, -12.465466f, 0.0f},
    {    3.8823f, -14.488904f, 0.0f},
    {  3.231519f, -12.060168f, 0.0f},
    {  6.634326f, -13.453076f, 0.0f},
    {  5.528028f, -11.202295f, 0.0f},
    {  9.131421f, -11.900306f, 0.0f},
    {  7.609521f,  -9.916919f, 0.0f},
    { 11.277588f,   -9.89021f, 0.0f},
    {  9.389795f,  -8.239038f, 0.0f},
    { 12.990381f,  -7.499982f, 0.0f},
    { 10.812866f,  -6.242798f, 0.0f},
    { 14.203979f,  -4.821582f, 0.0f},
    { 11.830127f,  -4.012323f, 0.0f},
    {  14.87168f,  -1.957892f, 0.0f},
    { 12.393091f,  -1.631567f, 0.0f},
    { 14.967872f,   0.981055f, 0.0f},
    { 12.465454f,   0.813733f, 0.0f},
    { 14.488868f,   3.882294f, 0.0f},
    { 12.060157f,   3.231488f, 0.0f},
    { 13.451831f,   6.637403f, 0.0f},
    { 11.202319f,   5.528021f, 0.0f},
    { 11.900293f,   9.131418f, 0.0f},
    {  9.916968f,   7.609531f, 0.0f},
    {   9.89021f,  11.277595f, 0.0f},
    {  8.239038f,   9.389792f, 0.0f},
    {       7.5f,   12.99039f, 0.0f},
    {  6.242798f,  10.812839f, 0.0f},
    {  4.821631f,  14.203961f, 0.0f},
    {  4.012305f,  11.830089f, 0.0f},
    {   1.95791f,  14.871676f, 0.0f},
    {  1.631616f,  12.393068f, 0.0f},
    { -0.981055f,  14.967882f, 0.0f},
    { -0.813721f,  12.465467f, 0.0f},
    {   -3.8823f,  14.488901f, 0.0f},
    { -3.231494f,  12.060167f, 0.0f},
    { -6.634302f,   13.45309f, 0.0f},
    { -5.528003f,  11.202291f, 0.0f},
    { -9.131396f,  11.900316f, 0.0f},
    { -7.609521f,    9.91695f, 0.0f},
    {-11.277588f,   9.890205f, 0.0f},
    { -9.389771f,    8.23902f, 0.0f},
    {-12.990381f,   7.499985f, 0.0f},
    {-10.812842f,   6.242796f, 0.0f},
    {-14.203931f,     4.8216f, 0.0f},
    {-11.830078f,   4.012299f, 0.0f},
    { -14.87168f,   1.957889f, 0.0f},
    {-12.393066f,   1.631558f, 0.0f},
    {-14.967896f,  -0.981058f, 0.0f},
    {-12.465479f,  -0.813745f, 0.0f},
    {-14.488868f,  -3.882269f, 0.0f},
    { -12.06001f,  -3.231457f, 0.0f},
    {-13.451831f,  -6.637384f, 0.0f},
    {-11.202271f,  -5.528021f, 0.0f},
    {-11.900293f,  -9.131403f, 0.0f},
    { -9.916895f,   -7.60954f, 0.0f},
    { -9.890185f, -11.277582f, 0.0f},
    { -8.239014f,  -9.389795f, 0.0f},
    {      -7.5f, -12.990387f, 0.0f},
    { -6.242798f, -10.812873f, 0.0f},
    {      -7.5f, -12.990387f, 0.0f},
    { -4.012329f, -11.830121f, 0.0f},
    {      -7.5f, -12.990387f, 0.0f},
    { -4.814771f, -14.196124f, 0.0f},
    {   -8.4375f, -14.614179f, 0.0f},
    { -5.424292f, -15.979444f, 0.0f},
    {    -9.375f,  -16.23797f, 0.0f},
    { -5.424292f, -15.979444f, 0.0f},
    { -6.026978f, -17.754938f, 0.0f},
    { -2.202637f, -16.730635f, 0.0f},
    { -2.447363f, -18.589588f, 0.0f},
    { -2.202637f, -16.730635f, 0.0f},
    {  1.226318f, -18.709852f, 0.0f},
    { -2.202637f, -16.730635f, 0.0f},
    {  1.103687f, -16.838867f, 0.0f},
    { -2.202637f, -16.730635f, 0.0f},
    {  0.976489f,  -14.95857f, 0.0f},
    { -2.202637f, -16.730635f, 0.0f},
    { -1.957861f, -14.871668f, 0.0f},
    { -5.424292f, -15.979444f, 0.0f},
    { -1.957861f, -14.871668f, 0.0f},
    { -4.814771f, -14.196124f, 0.0f},
    { -1.957861f, -14.871668f, 0.0f},
    { -4.012329f, -11.830121f, 0.0f},
    { -1.957861f, -14.871668f, 0.0f},
    { -1.631592f, -12.393073f, 0.0f},
    {  0.976489f,  -14.95857f, 0.0f},
    { -1.631592f, -12.393073f, 0.0f},
    {  0.813721f, -12.465466f, 0.0f},
    { -1.631592f, -12.393073f, 0.0f},
    {  0.651001f,  -9.972363f, 0.0f},
    { -1.631592f, -12.393073f, 0.0f},
    { -1.305249f,  -9.914447f, 0.0f},
    { -4.012329f, -11.830121f, 0.0f},
    { -3.209863f,  -9.464111f, 0.0f},
    { -6.242798f, -10.812873f, 0.0f},
    { -4.994238f,  -8.650287f, 0.0f},
    { -8.239014f,  -9.389795f, 0.0f},
    { -6.591211f,  -7.511841f, 0.0f},
    { -9.916895f,   -7.60954f, 0.0f},
    { -7.933521f,  -6.087635f, 0.0f},
    {-11.202271f,  -5.528021f, 0.0f},
    { -8.961817f,  -4.422412f, 0.0f},
    { -12.06001f,  -3.231457f, 0.0f},
    {  -9.64812f,  -2.585217f, 0.0f},
    {-12.465479f,  -0.813745f, 0.0f},
    { -9.972363f,  -0.650995f, 0.0f},
    {-12.393066f,   1.631558f, 0.0f},
    { -9.914454f,   1.305264f, 0.0f},
    {-11.830078f,   4.012299f, 0.0f},
    { -9.464087f,   3.209851f, 0.0f},
    {-10.812842f,   6.242796f, 0.0f},
    { -8.650269f,   4.994247f, 0.0f},
    { -9.389771f,    8.23902f, 0.0f},
    { -7.511817f,   6.591191f, 0.0f},
    { -7.609521f,    9.91695f, 0.0f},
    { -6.087622f,   7.933533f, 0.0f},
    { -5.528003f,  11.202291f, 0.0f},
    { -4.422363f,   8.961843f, 0.0f},
    { -3.231494f,  12.060167f, 0.0f},
    { -2.585181f,    9.64815f, 0.0f},
    { -0.813721f,  12.465467f, 0.0f},
    { -0.651001f,   9.972366f, 0.0f},
    {  1.631616f,  12.393068f, 0.0f},
    {  1.305273f,   9.914459f, 0.0f},
    {  4.012305f,  11.830089f, 0.0f},
    {  3.209863f,   9.464081f, 0.0f},
    {  6.242798f,  10.812839f, 0.0f},
    {  4.994238f,    8.65025f, 0.0f},
    {  8.239038f,   9.389792f, 0.0f},
    {  6.591211f,   7.511823f, 0.0f},
    {  9.916968f,   7.609531f, 0.0f},
    {  7.933545f,   6.087616f, 0.0f},
    { 11.202319f,   5.528021f, 0.0f},
    {  8.961817f,   4.422394f, 0.0f},
    { 12.060157f,   3.231488f, 0.0f},
    {  9.648096f,   2.585205f, 0.0f},
    { 12.465454f,   0.813733f, 0.0f},
    {  9.972412f,   0.650989f, 0.0f},
    { 12.393091f,  -1.631567f, 0.0f},
    {  9.914454f,  -1.305261f, 0.0f},
    { 11.830127f,  -4.012323f, 0.0f},
};

f32 ripple_mesh_buffers_template_vertices_1[148][3] __asm__("D_L15_001DA530") = {
    { 9.914454f,  -1.305261f, 0.0f},
    {11.830127f,  -4.012323f, 0.0f},
    { 9.464111f,  -3.209857f, 0.0f},
    {10.812866f,  -6.242798f, 0.0f},
    { 8.650293f,  -4.994232f, 0.0f},
    { 9.389795f,  -8.239038f, 0.0f},
    { 7.511865f,  -6.591211f, 0.0f},
    { 7.609521f,  -9.916919f, 0.0f},
    { 6.087646f,  -7.933539f, 0.0f},
    { 5.528028f, -11.202295f, 0.0f},
    { 4.422461f,  -8.961835f, 0.0f},
    { 3.231519f, -12.060168f, 0.0f},
    { 2.585205f,  -9.648132f, 0.0f},
    { 0.813721f, -12.465466f, 0.0f},
    { 2.585205f,  -9.648132f, 0.0f},
    { 0.651001f,  -9.972363f, 0.0f},
    {  0.79353f,  -7.799268f, 0.0f},
    { 0.651001f,  -9.972363f, 0.0f},
    {-1.126343f,  -8.555591f, 0.0f},
    {-1.305249f,  -9.914447f, 0.0f},
    {-1.126343f,  -8.555591f, 0.0f},
    {-3.209863f,  -9.464111f, 0.0f},
    {-2.785083f,  -7.328113f, 0.0f},
    {-4.994238f,  -8.650287f, 0.0f},
    {-4.308423f,  -5.900586f, 0.0f},
    {-6.591211f,  -7.511841f, 0.0f},
    {-5.690698f,  -4.366651f, 0.0f},
    {-7.933521f,  -6.087635f, 0.0f},
    {-6.814624f,  -2.634454f, 0.0f},
    {-8.961817f,  -4.422412f, 0.0f},
    {-7.799243f,  -0.793561f, 0.0f},
    { -9.64812f,  -2.585217f, 0.0f},
    {-8.555567f,   1.126361f, 0.0f},
    {-9.972363f,  -0.650995f, 0.0f},
    {-8.555567f,   1.126361f, 0.0f},
    {-9.914454f,   1.305264f, 0.0f},
    {-8.555567f,   1.126361f, 0.0f},
    {-9.464087f,   3.209851f, 0.0f},
    {-7.328149f,   2.785068f, 0.0f},
    {-8.650269f,   4.994247f, 0.0f},
    {-5.900586f,   4.308429f, 0.0f},
    {-7.511817f,   6.591191f, 0.0f},
    {-4.366626f,   5.690668f, 0.0f},
    {-6.087622f,   7.933533f, 0.0f},
    {-2.634424f,   6.814633f, 0.0f},
    {-4.422363f,   8.961843f, 0.0f},
    {-0.793555f,   7.799234f, 0.0f},
    {-2.585181f,    9.64815f, 0.0f},
    { 1.126392f,   8.555567f, 0.0f},
    {-0.651001f,   9.972366f, 0.0f},
    { 1.126392f,   8.555567f, 0.0f},
    { 1.305273f,   9.914459f, 0.0f},
    { 1.126392f,   8.555567f, 0.0f},
    { 3.209863f,   9.464081f, 0.0f},
    { 2.785107f,   7.328098f, 0.0f},
    { 4.994238f,    8.65025f, 0.0f},
    { 4.308472f,   5.900565f, 0.0f},
    { 6.591211f,   7.511823f, 0.0f},
    { 5.690674f,   4.366614f, 0.0f},
    { 7.933545f,   6.087616f, 0.0f},
    { 6.814649f,   2.634436f, 0.0f},
    { 8.961817f,   4.422394f, 0.0f},
    { 7.799219f,   0.793536f, 0.0f},
    { 9.648096f,   2.585205f, 0.0f},
    { 8.555591f,  -1.126349f, 0.0f},
    { 9.972412f,   0.650989f, 0.0f},
    { 8.555591f,  -1.126349f, 0.0f},
    { 9.914454f,  -1.305261f, 0.0f},
    { 8.555591f,  -1.126349f, 0.0f},
    { 9.464111f,  -3.209857f, 0.0f},
    { 7.328125f,  -2.785083f, 0.0f},
    { 8.650293f,  -4.994232f, 0.0f},
    { 5.900562f,  -4.308478f, 0.0f},
    { 7.511865f,  -6.591211f, 0.0f},
    { 4.366651f,  -5.690686f, 0.0f},
    { 6.087646f,  -7.933539f, 0.0f},
    { 2.634448f,  -6.814618f, 0.0f},
    { 4.422461f,  -8.961835f, 0.0f},
    { 2.634448f,  -6.814618f, 0.0f},
    { 2.585205f,  -9.648132f, 0.0f},
    { 2.634448f,  -6.814618f, 0.0f},
    {  0.79353f,  -7.799268f, 0.0f},
    { 1.085913f,   -4.97735f, 0.0f},
    {  0.79353f,  -7.799268f, 0.0f},
    {-0.811401f,  -6.163367f, 0.0f},
    {-1.126343f,  -8.555591f, 0.0f},
    {-0.811401f,  -6.163367f, 0.0f},
    {-2.785083f,  -7.328113f, 0.0f},
    {-2.337158f,  -4.526697f, 0.0f},
    {-4.308423f,  -5.900586f, 0.0f},
    {-3.749512f,   -2.87713f, 0.0f},
    {-5.690698f,  -4.366651f, 0.0f},
    {-4.977368f,  -1.085913f, 0.0f},
    {-6.814624f,  -2.634454f, 0.0f},
    {-6.163354f,    0.81142f, 0.0f},
    {-7.799243f,  -0.793561f, 0.0f},
    {-6.163354f,    0.81142f, 0.0f},
    {-8.555567f,   1.126361f, 0.0f},
    {-6.163354f,    0.81142f, 0.0f},
    {-7.328149f,   2.785068f, 0.0f},
    {-4.526685f,   2.337161f, 0.0f},
    {-5.900586f,   4.308429f, 0.0f},
    {-2.877124f,   3.749512f, 0.0f},
    {-4.366626f,   5.690668f, 0.0f},
    {-1.085889f,   4.977368f, 0.0f},
    {-2.634424f,   6.814633f, 0.0f},
    { 0.811426f,   6.163349f, 0.0f},
    {-0.793555f,   7.799234f, 0.0f},
    { 0.811426f,   6.163349f, 0.0f},
    { 1.126392f,   8.555567f, 0.0f},
    { 0.811426f,   6.163349f, 0.0f},
    { 2.785107f,   7.328098f, 0.0f},
    { 2.337183f,   4.526697f, 0.0f},
    { 4.308472f,   5.900565f, 0.0f},
    { 3.749561f,   2.877106f, 0.0f},
    { 5.690674f,   4.366614f, 0.0f},
    { 4.977368f,   1.085895f, 0.0f},
    { 6.814649f,   2.634436f, 0.0f},
    { 6.163404f,  -0.811408f, 0.0f},
    { 7.799219f,   0.793536f, 0.0f},
    { 6.163404f,  -0.811408f, 0.0f},
    { 8.555591f,  -1.126349f, 0.0f},
    { 6.163404f,  -0.811408f, 0.0f},
    { 7.328125f,  -2.785083f, 0.0f},
    { 4.526733f,  -2.337152f, 0.0f},
    { 5.900562f,  -4.308478f, 0.0f},
    {   2.8771f,  -3.749561f, 0.0f},
    { 4.366651f,  -5.690686f, 0.0f},
    {   2.8771f,  -3.749561f, 0.0f},
    { 2.634448f,  -6.814618f, 0.0f},
    {   2.8771f,  -3.749561f, 0.0f},
    { 1.085913f,   -4.97735f, 0.0f},
    { 1.416211f,   -1.84563f, 0.0f},
    { 1.085913f,   -4.97735f, 0.0f},
    {-0.414551f,  -3.148926f, 0.0f},
    {-0.811401f,  -6.163367f, 0.0f},
    {-0.414551f,  -3.148926f, 0.0f},
    {-2.337158f,  -4.526697f, 0.0f},
    {-1.845605f,  -1.416187f, 0.0f},
    {-3.749512f,   -2.87713f, 0.0f},
    {-3.148901f,   0.414563f, 0.0f},
    {-4.977368f,  -1.085913f, 0.0f},
    {-3.148901f,   0.414563f, 0.0f},
    {-6.163354f,    0.81142f, 0.0f},
    {-3.148901f,   0.414563f, 0.0f},
    {-4.526685f,   2.337161f, 0.0f},
    {-1.416162f,    1.84563f, 0.0f},
    {-2.877124f,   3.749512f, 0.0f},
};

f32 ripple_mesh_buffers_template_vertices_2[31][3] __asm__("D_L15_001DAC20") = {
    {-1.416162f,   1.84563f, 0.0f},
    {-2.877124f,  3.749512f, 0.0f},
    { 0.414575f,  3.148923f, 0.0f},
    {-1.085889f,  4.977368f, 0.0f},
    { 0.414575f,  3.148923f, 0.0f},
    { 0.811426f,  6.163349f, 0.0f},
    { 0.414575f,  3.148923f, 0.0f},
    { 2.337183f,  4.526697f, 0.0f},
    { 1.845605f,  1.416187f, 0.0f},
    { 3.749561f,  2.877106f, 0.0f},
    { 3.148926f, -0.414563f, 0.0f},
    { 4.977368f,  1.085895f, 0.0f},
    { 3.148926f, -0.414563f, 0.0f},
    { 6.163404f, -0.811408f, 0.0f},
    { 3.148926f, -0.414563f, 0.0f},
    { 4.526733f, -2.337152f, 0.0f},
    { 3.148926f, -0.414563f, 0.0f},
    {   2.8771f, -3.749561f, 0.0f},
    { 3.148926f, -0.414563f, 0.0f},
    { 1.416211f,  -1.84563f, 0.0f},
    { 1.845605f,  1.416187f, 0.0f},
    {      0.0f,       0.0f, 0.0f},
    { 0.414575f,  3.148923f, 0.0f},
    {      0.0f,       0.0f, 0.0f},
    {-1.416162f,   1.84563f, 0.0f},
    {      0.0f,       0.0f, 0.0f},
    {-3.148901f,  0.414563f, 0.0f},
    {      0.0f,       0.0f, 0.0f},
    {-1.845605f, -1.416187f, 0.0f},
    { 1.416211f,  -1.84563f, 0.0f},
    {-0.414551f, -3.148926f, 0.0f},
};

int ripple_mesh_buffers_template_vertex_tables[2] __asm__("D_L15_001620C0") = {
    (int)ripple_mesh_buffers_template_vertices_0, (int)ripple_mesh_buffers_template_vertices_1};

f32 ripple_mesh_buffers_template_uvs_0[148][2] __asm__("D_L15_001DAD98") = {
    { -1.66535f, -0.147049f},
    {-1.745182f,  0.336489f},
    {-1.423867f, -0.082345f},
    {-1.495718f,   0.35284f},
    {-1.182389f, -0.017642f},
    {-1.245011f,    0.3698f},
    {-1.182389f, -0.017642f},
    {-0.912598f,  0.391502f},
    {-1.182389f, -0.017642f},
    {-0.858558f,  0.069129f},
    {-1.044279f, -0.384579f},
    {-0.744175f, -0.237072f},
    {-0.837243f, -0.717524f},
    {-0.572791f, -0.514605f},
    { -0.56923f,  -1.00368f},
    {-0.349074f, -0.751974f},
    {-0.250533f, -1.232052f},
    {-0.082908f, -0.941717f},
    { 0.106587f, -1.393866f},
    { 0.214488f, -1.077352f},
    { 0.488412f, -1.482892f},
    { 0.531922f, -1.152414f},
    { 0.880272f, -1.495718f},
    { 0.857962f, -1.162062f},
    { 1.267104f, -1.431851f},
    {  1.18033f, -1.108023f},
    { 1.634452f, -1.293579f},
    { 1.486534f, -0.993644f},
    { 1.966987f, -1.086707f},
    { 1.764069f, -0.822264f},
    { 2.253144f, -0.818696f},
    { 2.001437f,  -0.59854f},
    { 2.481517f, -0.500002f},
    { 2.191176f, -0.332375f},
    { 2.643326f, -0.142886f},
    {  2.32681f, -0.034976f},
    { 2.732355f,  0.238944f},
    { 2.401874f,   0.28245f},
    { 2.745182f,  0.630806f},
    { 2.411527f,  0.608494f},
    { 2.681318f,  1.017638f},
    { 2.357487f,  0.930864f},
    {  2.54321f,  1.384572f},
    { 2.243104f,  1.237065f},
    { 2.336174f,  1.717518f},
    { 2.071725f,  1.514601f},
    { 2.068159f,  2.003677f},
    { 1.848001f,  1.751968f},
    { 1.749463f,  2.232049f},
    { 1.581838f,  1.941711f},
    { 1.392345f,  2.393856f},
    { 1.284438f,  2.077342f},
    { 1.010517f,  2.482889f},
    { 0.967006f,  2.152407f},
    { 0.618657f,  2.495718f},
    { 0.640965f,  2.162062f},
    { 0.231829f,  2.431847f},
    { 0.318604f,     2.108f},
    { -0.13552f,  2.293576f},
    { 0.012395f,  1.993634f},
    {-0.468056f,  2.086704f},
    {-0.265141f,  1.822251f},
    {-0.754213f,   1.81869f},
    {-0.502508f,  1.598534f},
    {-0.982587f,  1.499998f},
    {-0.692252f,  1.332371f},
    {-0.982587f,  1.499998f},
    {-0.827885f,  1.034976f},
    {-0.982587f,  1.499998f},
    {-1.143352f,  1.141968f},
    {-1.199093f,  1.624998f},
    {-1.381128f,  1.223237f},
    {-1.415598f,  1.749998f},
    {-1.381128f,  1.223237f},
    { -1.61786f,  1.303595f},
    {-1.481287f,  0.793683f},
    {-1.729147f,  0.826313f},
    {-1.481287f,  0.793683f},
    {-1.745182f,  0.336489f},
    {-1.481287f,  0.793683f},
    {-1.495718f,   0.35284f},
    {-1.481287f,  0.793683f},
    {-1.245011f,    0.3698f},
    {-1.481287f,  0.793683f},
    {-1.233424f,  0.761047f},
    {-1.381128f,  1.223237f},
    {-1.233424f,  0.761047f},
    {-1.143352f,  1.141968f},
    {-1.233424f,  0.761047f},
    {-0.827885f,  1.034976f},
    {-1.233424f,  0.761047f},
    {-0.902945f,  0.717544f},
    {-1.245011f,    0.3698f},
    {-0.902945f,  0.717544f},
    {-0.912598f,  0.391502f},
    {-0.902945f,  0.717544f},
    {-0.580184f,  0.413198f},
    {-0.902945f,  0.717544f},
    {-0.572462f,  0.674032f},
    {-0.827885f,  1.034976f},
    {-0.512417f,   0.92798f},
    {-0.692252f,  1.332371f},
    {-0.403907f,  1.165897f},
    {-0.502508f,  1.598534f},
    {-0.252114f,  1.378826f},
    {-0.265141f,  1.822251f},
    { -0.06222f,  1.557801f},
    { 0.012395f,  1.993634f},
    {  0.15981f,  1.694907f},
    { 0.318604f,     2.108f},
    { 0.404769f,  1.786414f},
    { 0.640965f,  2.162062f},
    { 0.662665f,  1.829647f},
    { 0.967006f,  2.152407f},
    {   0.9235f,  1.821926f},
    { 1.284438f,  2.077342f},
    { 1.177445f,  1.761877f},
    { 1.581838f,  1.941711f},
    { 1.415364f,  1.653368f},
    { 1.848001f,  1.751968f},
    {  1.62829f,  1.501574f},
    { 2.071725f,  1.514601f},
    { 1.807269f,  1.311681f},
    { 2.243104f,  1.237065f},
    { 1.944377f,  1.089647f},
    { 2.357487f,  0.930864f},
    { 2.035885f,  0.844689f},
    { 2.411527f,  0.608494f},
    { 2.079113f,  0.586798f},
    { 2.401874f,   0.28245f},
    { 2.071393f,  0.325962f},
    {  2.32681f, -0.034976f},
    { 2.011342f,  0.072017f},
    { 2.191176f, -0.332375f},
    { 1.902831f,   -0.1659f},
    { 2.001437f,  -0.59854f},
    { 1.751041f,  -0.37883f},
    { 1.764069f, -0.822264f},
    { 1.561147f, -0.557808f},
    { 1.486534f, -0.993644f},
    { 1.339117f, -0.694911f},
    {  1.18033f, -1.108023f},
    { 1.094159f, -0.786414f},
    { 0.857962f, -1.162062f},
    { 0.836263f, -0.829657f},
    { 0.531922f, -1.152414f},
    {  0.57543f, -0.821929f},
    { 0.214488f, -1.077352f},
};

f32 ripple_mesh_buffers_template_uvs_1[148][2] __asm__("D_L15_001DB238") = {
    {  0.57543f, -0.821929f},
    { 0.214488f, -1.077352f},
    { 0.321484f, -0.761883f},
    {-0.082908f, -0.941717f},
    { 0.083567f, -0.653374f},
    {-0.349074f, -0.751974f},
    {-0.129363f, -0.501584f},
    {-0.572791f, -0.514605f},
    {-0.308341f, -0.311688f},
    {-0.744175f, -0.237072f},
    {-0.445447f, -0.089663f},
    {-0.858558f,  0.069129f},
    {-0.536953f,  0.155304f},
    {-0.912598f,  0.391502f},
    {-0.536953f,  0.155304f},
    {-0.580184f,  0.413198f},
    {-0.290438f,  0.394194f},
    {-0.580184f,  0.413198f},
    {-0.391281f,  0.650177f},
    {-0.572462f,  0.674032f},
    {-0.391281f,  0.650177f},
    {-0.512417f,   0.92798f},
    {-0.227617f,  0.871343f},
    {-0.403907f,  1.165897f},
    { -0.03728f,  1.074455f},
    {-0.252114f,  1.378826f},
    { 0.167245f,  1.258758f},
    { -0.06222f,  1.557801f},
    { 0.398204f,  1.408615f},
    {  0.15981f,  1.694907f},
    { 0.643657f,  1.539897f},
    { 0.404769f,  1.786414f},
    { 0.899646f,  1.640741f},
    { 0.662665f,  1.829647f},
    { 0.899646f,  1.640741f},
    {   0.9235f,  1.821926f},
    { 0.899646f,  1.640741f},
    { 1.177445f,  1.761877f},
    { 1.120807f,  1.477085f},
    { 1.415364f,  1.653368f},
    { 1.323922f,  1.286743f},
    {  1.62829f,  1.501574f},
    {  1.50822f,  1.082215f},
    { 1.807269f,  1.311681f},
    { 1.658082f,  0.851255f},
    { 1.944377f,  1.089647f},
    { 1.789363f,  0.605806f},
    { 2.035885f,  0.844689f},
    { 1.890207f,  0.349813f},
    { 2.079113f,  0.586798f},
    { 1.890207f,  0.349813f},
    { 2.071393f,  0.325962f},
    { 1.890207f,  0.349813f},
    { 2.011342f,  0.072017f},
    { 1.726544f,  0.128651f},
    { 1.902831f,   -0.1659f},
    { 1.536207f, -0.074465f},
    { 1.751041f,  -0.37883f},
    {  1.33168f, -0.258758f},
    { 1.561147f, -0.557808f},
    { 1.100723f, -0.408621f},
    { 1.339117f, -0.694911f},
    { 0.855269f, -0.539898f},
    { 1.094159f, -0.786414f},
    { 0.599285f, -0.640747f},
    { 0.836263f, -0.829657f},
    { 0.599285f, -0.640747f},
    {  0.57543f, -0.821929f},
    { 0.599285f, -0.640747f},
    { 0.321484f, -0.761883f},
    {  0.37812f, -0.477085f},
    { 0.083567f, -0.653374f},
    { 0.175001f, -0.286743f},
    {-0.129363f, -0.501584f},
    {-0.009294f, -0.082222f},
    {-0.308341f, -0.311688f},
    {-0.159151f,  0.148739f},
    {-0.445447f, -0.089663f},
    {-0.159151f,  0.148739f},
    {-0.536953f,  0.155304f},
    {-0.159151f,  0.148739f},
    {-0.290438f,  0.394194f},
    { 0.085818f,   0.35521f},
    {-0.290438f,  0.394194f},
    {-0.072318f,  0.608185f},
    {-0.391281f,  0.650177f},
    {-0.072318f,  0.608185f},
    {-0.227617f,  0.871343f},
    { 0.145905f,  0.811619f},
    { -0.03728f,  1.074455f},
    { 0.365847f,  0.999933f},
    { 0.167245f,  1.258758f},
    { 0.604676f,  1.163647f},
    { 0.398204f,  1.408615f},
    { 0.857654f,  1.321779f},
    { 0.643657f,  1.539897f},
    { 0.857654f,  1.321779f},
    { 0.899646f,  1.640741f},
    { 0.857654f,  1.321779f},
    { 1.120807f,  1.477085f},
    { 1.061086f,  1.103556f},
    { 1.323922f,  1.286743f},
    {   1.2494f,  0.883615f},
    {  1.50822f,  1.082215f},
    { 1.413114f,  0.644783f},
    { 1.658082f,  0.851255f},
    { 1.571244f,  0.391808f},
    { 1.789363f,  0.605806f},
    { 1.571244f,  0.391808f},
    { 1.890207f,  0.349813f},
    { 1.571244f,  0.391808f},
    { 1.726544f,  0.128651f},
    { 1.353024f,  0.188374f},
    { 1.536207f, -0.074465f},
    { 1.133079f,   5.7e-05f},
    {  1.33168f, -0.258758f},
    { 0.894251f, -0.163651f},
    { 1.100723f, -0.408621f},
    { 0.641277f, -0.321789f},
    { 0.855269f, -0.539898f},
    { 0.641277f, -0.321789f},
    { 0.599285f, -0.640747f},
    { 0.641277f, -0.321789f},
    {  0.37812f, -0.477085f},
    { 0.437844f, -0.103566f},
    { 0.175001f, -0.286743f},
    { 0.249523f,  0.116385f},
    {-0.009294f, -0.082222f},
    { 0.249523f,  0.116385f},
    {-0.159151f,  0.148739f},
    { 0.249523f,  0.116385f},
    { 0.085818f,   0.35521f},
    { 0.503381f,   0.31117f},
    { 0.085818f,   0.35521f},
    { 0.329608f,  0.555272f},
    {-0.072318f,  0.608185f},
    { 0.329608f,  0.555272f},
    { 0.145905f,  0.811619f},
    {  0.56064f,  0.746079f},
    { 0.365847f,  0.999933f},
    {  0.80474f,  0.919852f},
    { 0.604676f,  1.163647f},
    {  0.80474f,  0.919852f},
    { 0.857654f,  1.321779f},
    {  0.80474f,  0.919852f},
    { 1.061086f,  1.103556f},
    { 0.995549f,   0.68882f},
    {   1.2494f,  0.883615f},
};

f32 ripple_mesh_buffers_template_uvs_2[31][2] __asm__("D_L15_001DB6D8") = {
    {0.995549f,   0.68882f},
    {  1.2494f,  0.883615f},
    {1.169321f,  0.444722f},
    {1.413114f,  0.644783f},
    {1.169321f,  0.444722f},
    {1.571244f,  0.391808f},
    {1.169321f,  0.444722f},
    {1.353024f,  0.188374f},
    { 0.93829f,  0.253918f},
    {1.133079f,   5.7e-05f},
    { 0.69419f,  0.080142f},
    {0.894251f, -0.163651f},
    { 0.69419f,  0.080142f},
    {0.641277f, -0.321789f},
    { 0.69419f,  0.080142f},
    {0.437844f, -0.103566f},
    { 0.69419f,  0.080142f},
    {0.249523f,  0.116385f},
    { 0.69419f,  0.080142f},
    {0.503381f,   0.31117f},
    { 0.93829f,  0.253918f},
    {0.749465f,  0.499998f},
    {1.169321f,  0.444722f},
    {0.749465f,  0.499998f},
    {0.995549f,   0.68882f},
    {0.749465f,  0.499998f},
    { 0.80474f,  0.919852f},
    {0.749465f,  0.499998f},
    { 0.56064f,  0.746079f},
    {0.503381f,   0.31117f},
    {0.329608f,  0.555272f},
};

int ripple_mesh_buffers_template_uv_tables[2] __asm__("D_L15_001620D0") = {
    (int)ripple_mesh_buffers_template_uvs_0, (int)ripple_mesh_buffers_template_uvs_1};

void draw_moby_radial_ripple_mesh_buffers(struct Moby *moby) __asm__("FUN_L15_002eac90");

void draw_moby_radial_ripple_mesh_buffers(struct Moby *moby) {
    RippleData *ripple_data = (RippleData *)moby->pvars;
    float       scale;
    int         r, g, b;
    int         color;
    int         i, j, uv_buffer_index, uv_vertex_index, draw_buffer_index;

    scale = convert_integer_to_float(D_L15_0015F5CC) *
            (ripple_mesh_buffers_uv_scroll_rate_gp * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(ripple_mesh_buffers_effect_texture_id));
    vu1_add_g_sregister(0x42, ((long)ripple_mesh_buffers_alpha_1_fix << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    ripple_data->f0 = ripple_data->f4;
    ripple_data->f4 = fast_add_rotations(
        ripple_data->f4,
        360.0f / multiply_global_factor_ed64(ripple_mesh_buffers_phase_advance_divisor_gp) *
            DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (ripple_mesh_buffers_vertex_alpha << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < ripple_mesh_buffers_vertex_counts[i]; j++) {
            int   *template_vertex_buffer_entry = ripple_mesh_buffers_template_vertex_tables + i;
            float *p = (float *)(*template_vertex_buffer_entry + j * 12);
            float  t = ripple_mesh_buffers_z_amplitude_gp *
                      fast_sin(fast_add_rotations(
                          fast_normalize_angle(
                              FUN_L00_00200260(square_root(p[0] * p[0] + p[1] * p[1]),
                                               ripple_mesh_buffers_radial_wavelength_gp) *
                              6.2831855f / ripple_mesh_buffers_radial_wavelength_gp),
                          ripple_data->f4));
            *(float *)(j * 12 + ripple_mesh_buffers_world_vertex_tables[i]) = p[0] + moby->pos.x;
            *(float *)(j * 12 + ripple_mesh_buffers_world_vertex_tables[i] + 4) =
                p[1] + moby->pos.y;
            *(float *)(j * 12 + ripple_mesh_buffers_world_vertex_tables[i] + 8) =
                p[2] + moby->pos.z + t;
            *(int *)(j * 4 + ripple_mesh_buffers_colour_tables[i]) = color;
        }
    }
    for (i = 0; i < 2; i++) {
        for (uv_buffer_index = 0; uv_buffer_index < 3; uv_buffer_index++) {
            for (uv_vertex_index = 0;
                 uv_vertex_index < ripple_mesh_buffers_vertex_counts[uv_buffer_index];
                 uv_vertex_index++) {
                T4 uv_scroll_factors = ripple_mesh_buffers_uv_scroll_factors;
                *(float *)(uv_vertex_index * 8 +
                           ripple_mesh_buffers_scrolled_uv_tables[uv_buffer_index]) =
                    *(float *)(uv_vertex_index * 8 +
                               ripple_mesh_buffers_template_uv_tables[uv_buffer_index]) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[0], 1.0f);
                *(float *)(uv_vertex_index * 8 +
                           ripple_mesh_buffers_scrolled_uv_tables[uv_buffer_index] + 4) =
                    *(float *)(uv_vertex_index * 8 +
                               ripple_mesh_buffers_template_uv_tables[uv_buffer_index] + 4) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[1], 1.0f);
            }
        }
        font_queue_vu_state();
        for (draw_buffer_index = 0; draw_buffer_index < 3; draw_buffer_index++) {
            FUN_L00_001fde98(ripple_mesh_buffers_vertex_counts[draw_buffer_index],
                             ripple_mesh_buffers_world_vertex_tables[draw_buffer_index],
                             ripple_mesh_buffers_colour_tables[draw_buffer_index],
                             (void *)ripple_mesh_buffers_scrolled_uv_tables[draw_buffer_index],
                             1);
        }
    }
}

typedef struct {
    float f[4];
} Quad4;

typedef struct {
    float prev;
    float angle;
} RingVars;

Quad4 ripple_mesh_uv_scroll_factors __asm__("D_L15_00162170") = {
    {1.0f, -0.62831855f, 1.0f, 0.62831855f}
};

float ripple_mesh_uv_scroll_rate_gp __asm__("D_L15_0016216C") = 0.025f;

float ripple_mesh_template_vertices[204] __asm__("D_L15_001DDBA0") = {
    3.485078f,  4.343359f,  0.0f, 5.279609f,  3.600078f,  0.0f,    2.980781f,  2.623281f,  0.0f,
    4.615703f,  1.997344f,  0.0f, 2.298828f,  0.976816f,  0.0f,    3.933828f,  0.351152f,  0.0f,
    1.616797f,  -0.669707f, 0.0f, 3.237344f,  -1.330234f, 0.0f,    0.850156f,  -2.520547f, 0.0f,
    2.454375f,  -3.220508f, 0.0f, -0.025f,    -4.63334f,  0.0f,    1.594062f,  -5.297578f, 0.0f,
    -0.025f,    -4.63334f,  0.0f, 0.733984f,  -7.373926f, -5e-06f, -0.025f,    -4.63334f,  0.0f,
    -1.060508f, -6.630625f, 0.0f, -1.641797f, -3.963652f, 0.0f,    -2.49957f,  -6.034551f, 0.0f,
    -3.258633f, -3.293945f, 0.0f, -3.938672f, -5.438457f, 0.0f,    -3.258633f, -3.293945f, 0.0f,
    -5.733164f, -4.695137f, 0.0f, -3.258633f, -3.293945f, 0.0f,    -4.873164f, -2.618789f, 0.0f,
    -2.383437f, -1.181152f, 0.0f, -4.012774f, -0.541719f, 0.0f,    -1.616797f, 0.669688f,  0.0f,
    -3.229805f, 1.348555f,  0.0f, -0.934805f, 2.316172f,  0.0f,    -2.53332f,  3.029922f,  0.0f,
    -0.252813f, 3.962656f,  0.0f, -1.851445f, 4.676094f,  0.0f,    -0.252813f, 3.962656f,  0.0f,
    -1.187617f, 6.278828f,  0.0f, -0.252813f, 3.962656f,  0.0f,    0.606953f,  5.535547f,  0.0f,
    1.363984f,  3.292969f,  0.0f, 2.046016f,  4.939453f,  0.0f,    1.363984f,  3.292969f,  0.0f,
    3.485078f,  4.343359f,  0.0f, 1.363984f,  3.292969f,  0.0f,    2.980781f,  2.623281f,  0.0f,
    1.363984f,  3.292969f,  0.0f, 2.298828f,  0.976816f,  0.0f,    1.363984f,  3.292969f,  0.0f,
    0.681953f,  1.646484f,  0.0f, -0.252813f, 3.962656f,  0.0f,    0.681953f,  1.646484f,  0.0f,
    -0.934805f, 2.316172f,  0.0f, 0.681953f,  1.646484f,  0.0f,    -1.616797f, 0.669688f,  0.0f,
    0.681953f,  1.646484f,  0.0f, 0.0f,       0.0f,       0.0f,    2.298828f,  0.976816f,  0.0f,
    0.0f,       0.0f,       0.0f, 1.616797f,  -0.669707f, 0.0f,    -0.766641f, -1.85084f,  0.0f,
    0.850156f,  -2.520547f, 0.0f, -0.766641f, -1.85084f,  0.0f,    -0.025f,    -4.63334f,  0.0f,
    -0.766641f, -1.85084f,  0.0f, -1.641797f, -3.963652f, 0.0f,    -0.766641f, -1.85084f,  0.0f,
    -3.258633f, -3.293945f, 0.0f, -0.766641f, -1.85084f,  0.0f,    -2.383437f, -1.181152f, 0.0f,
    0.0f,       0.0f,       0.0f, -1.616797f, 0.669688f,  0.0f};

float ripple_mesh_template_uvs[68][2] __asm__("D_L15_001DDED0") = {
    { 2.945454f, -1.355928f},
    { 2.573813f, -2.253193f},
    { 2.085415f, -1.103779f},
    { 1.772446f,  -1.92124f},
    { 1.262183f, -0.762803f},
    { 0.949351f, -1.580303f},
    { 0.438921f, -0.421787f},
    { 0.108657f,  -1.23206f},
    {-0.486499f, -0.038467f},
    {-0.836479f, -0.840576f},
    {-1.542895f,  0.399111f},
    {-1.875015f,  -0.41042f},
    {-1.542895f,  0.399111f},
    {-2.913188f,  0.019619f},
    {-1.542895f,  0.399111f},
    {-2.541538f,  0.916865f},
    {-1.208052f,   1.20751f},
    {-2.243501f,  1.636396f},
    {-0.873198f,  2.015928f},
    {-1.945454f,  2.355947f},
    {-0.873198f,  2.015928f},
    {-1.573794f,  3.253193f},
    {-0.873198f,  2.015928f},
    { -0.53562f,  2.823193f},
    { 0.183198f,   1.57833f},
    { 0.502915f,  2.392998f},
    { 1.108618f,   1.19501f},
    { 1.448052f,  2.001514f},
    {  1.93186f,  0.854014f},
    { 2.288735f,  1.653272f},
    { 2.755102f,  0.513018f},
    { 3.111821f,  1.312334f},
    { 2.755102f,  0.513018f},
    { 3.913188f,   0.98042f},
    { 2.755102f,  0.513018f},
    { 3.541548f,  0.083135f},
    { 2.420259f, -0.295381f},
    { 3.243501f, -0.636396f},
    { 2.420259f, -0.295381f},
    { 2.945454f, -1.355928f},
    { 2.420259f, -0.295381f},
    { 2.085415f, -1.103779f},
    { 2.420259f, -0.295381f},
    { 1.262183f, -0.762803f},
    { 2.420259f, -0.295381f},
    { 1.597017f,  0.045635f},
    { 2.755102f,  0.513018f},
    { 1.597017f,  0.045635f},
    {  1.93186f,  0.854014f},
    { 1.597017f,  0.045635f},
    { 1.108618f,   1.19501f},
    { 1.597017f,  0.045635f},
    { 0.773774f,  0.386611f},
    { 1.262183f, -0.762803f},
    { 0.773774f,  0.386611f},
    { 0.438921f, -0.421787f},
    {-0.151646f,  0.769932f},
    {-0.486499f, -0.038467f},
    {-0.151646f,  0.769932f},
    {-1.542895f,  0.399111f},
    {-0.151646f,  0.769932f},
    {-1.208052f,   1.20751f},
    {-0.151646f,  0.769932f},
    {-0.873198f,  2.015928f},
    {-0.151646f,  0.769932f},
    { 0.183198f,   1.57833f},
    { 0.773774f,  0.386611f},
    { 1.108618f,   1.19501f},
};

float ripple_mesh_world_vertices[204] __asm__("D_L15_001DE200") = {0};

float ripple_mesh_scrolled_uvs[68][2] __asm__("D_L15_001DE530") = {0};

int ripple_mesh_vertex_counts[2] __asm__("D_L15_00162120") = {68, 0};

int ripple_mesh_world_vertex_tables[2] __asm__("D_L15_00162140") = {(int)ripple_mesh_world_vertices,
                                                                    0};

int ripple_mesh_scrolled_uv_tables[2] __asm__("D_L15_00162148") = {(int)ripple_mesh_scrolled_uvs,
                                                                   0};

int ripple_mesh_colours[68] __asm__("D_L15_001DE750") = {0};

int ripple_mesh_colour_tables[1] __asm__("D_L15_00162150") = {(int)ripple_mesh_colours};

int ripple_mesh_alpha_1_fix __asm__("D_L15_00162154") = 128;

int ripple_mesh_effect_texture_id __asm__("D_L15_00162158") = 45;

int ripple_mesh_vertex_alpha __asm__("D_L15_0016215C") = 64;

float ripple_mesh_radial_wavelength_gp __asm__("D_L15_00162160") = 8.0f;

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_moby_radial_ripple_mesh.h"

float ripple_mesh_z_amplitude_gp __asm__("D_L15_00162168") = 0.1f;

void draw_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002eb108");

void draw_moby_radial_ripple_mesh(struct Moby *moby) {
    unsigned int i = 0;
    unsigned int output_vertex_byte_offset = 0;
    RingVars    *ripple_phase_state = (RingVars *)moby->pvars;
    unsigned int scrolled_uv_vertex_index;
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
    scale = convert_integer_to_float(D_L15_0015F5CC) * (ripple_mesh_uv_scroll_rate_gp * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(ripple_mesh_effect_texture_id));
    vu1_add_g_sregister(0x42, ((long)ripple_mesh_alpha_1_fix << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    ripple_phase_state->prev = ripple_phase_state->angle;
    ripple_phase_state->angle = fast_add_rotations(
        ripple_phase_state->angle,
        360.0f / multiply_global_factor_ed64(ripple_mesh_phase_advance_divisor_gp) * DEG_TO_RAD *
            frame_time);
    get_moby_color(moby, &r, &g, &b);
    /* The second float of each pair in t: p->f[0] is t.f[1], p->f[2] is t.f[3]. */
    v_scroll_factors = (Quad4 *)&uv_scroll_factors.f[1];
    color = (ripple_mesh_vertex_alpha << 24) | (b << 16) | (g << 8) | r;
    output_vertex = ripple_mesh_world_vertices;
    do {
        float *template_vertex = &ripple_mesh_template_vertices[i * 3];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radius_remainder = FUN_L00_00200260(rad, ripple_mesh_radial_wavelength_gp);
        float  w =
            fast_normalize_angle(radius_remainder * 6.2831855f / ripple_mesh_radial_wavelength_gp);
        float z = fast_sin(fast_add_rotations(w, ripple_phase_state->angle));
        output_vertex[0] = template_vertex[0] + moby->pos.x;
        *(float *)((char *)ripple_mesh_world_vertices + 4 + output_vertex_byte_offset) =
            template_vertex[1] + moby->pos.y;
        output_vertex[2] = template_vertex[2] + moby->pos.z + ripple_mesh_z_amplitude_gp * z;
        ripple_mesh_colours[i] = color;
        i++;
        output_vertex_byte_offset += 12;
        output_vertex += 3;
    } while (i < 68);
    for (j = 0; j < 2; j++) {
        for (scrolled_uv_vertex_index = 0; scrolled_uv_vertex_index < 68;
             scrolled_uv_vertex_index++) {
            uv_scroll_factors = ripple_mesh_uv_scroll_factors;
            ripple_mesh_scrolled_uvs[scrolled_uv_vertex_index][0] =
                ripple_mesh_template_uvs[scrolled_uv_vertex_index][0] +
                FUN_L00_00200260(scale * uv_scroll_factors.f[j * 2], 1.0f);
            ripple_mesh_scrolled_uvs[scrolled_uv_vertex_index][1] =
                ripple_mesh_template_uvs[scrolled_uv_vertex_index][1] +
                FUN_L00_00200260(scale * v_scroll_factors->f[j * 2], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(ripple_mesh_vertex_counts[k],
                             ripple_mesh_world_vertex_tables[k],
                             ripple_mesh_colour_tables[k],
                             (void *)ripple_mesh_scrolled_uv_tables[k],
                             1);
        }
    }
}

extern float FUN_001f96b0_2edbe0(float) __asm__("FUN_001f96b0");

extern float FUN_L00_00200260_2edbe0(float, float) __asm__("FUN_L00_00200260");

extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");

extern void FUN_L00_001fde98_2edbe0(int, int, int, void *, int) __asm__("FUN_L00_001fde98");

extern float frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;

extern int D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") MACRO_ADDR;

typedef struct {
    float f[2][2];
} Quad4_2edbe0;

typedef struct {
    float prev;
    float angle;
} RingVars_2edbe0;

float scaled_ripple_world_vertices[46][3] __asm__("D_L15_001DECB0") = {0};

float scaled_ripple_template_vertices[46][3] __asm__("D_L15_001DE860") = {
    { 11.779387f,  28.437962f, 0.0f},
    {  21.76547f,   21.76547f, 0.0f},
    {      15.0f,       15.0f, 0.0f},
    { 28.437956f,  11.779387f, 0.0f},
    {      15.0f,        0.0f, 0.0f},
    {     3e+01f,        0.0f, 0.0f},
    {      15.0f,      -15.0f, 0.0f},
    { 28.437956f, -11.779387f, 0.0f},
    {      15.0f,      -15.0f, 0.0f},
    {  21.76547f,  -21.76547f, 0.0f},
    {      15.0f,      -15.0f, 0.0f},
    { 11.779387f, -28.437956f, 0.0f},
    {       0.0f,      -15.0f, 0.0f},
    {       0.0f,     -3e+01f, 0.0f},
    {     -15.0f,      -15.0f, 0.0f},
    {-11.779387f, -28.437962f, 0.0f},
    {     -15.0f,      -15.0f, 0.0f},
    { -21.76547f,  -21.76547f, 0.0f},
    {     -15.0f,      -15.0f, 0.0f},
    {-28.437956f, -11.779387f, 0.0f},
    {     -15.0f,        0.0f, 0.0f},
    {    -3e+01f,        0.0f, 0.0f},
    {     -15.0f,       15.0f, 0.0f},
    {-28.437962f,  11.779387f, 0.0f},
    {     -15.0f,       15.0f, 0.0f},
    {-21.765469f,  21.765469f, 0.0f},
    {     -15.0f,       15.0f, 0.0f},
    {-11.779387f,  28.437962f, 0.0f},
    {       0.0f,       15.0f, 0.0f},
    {       0.0f,      3e+01f, 0.0f},
    {       0.0f,       15.0f, 0.0f},
    { 11.779387f,  28.437962f, 0.0f},
    {       0.0f,       15.0f, 0.0f},
    {      15.0f,       15.0f, 0.0f},
    {       0.0f,        0.0f, 0.0f},
    {      15.0f,        0.0f, 0.0f},
    {       0.0f,        0.0f, 0.0f},
    {      15.0f,      -15.0f, 0.0f},
    {       0.0f,        0.0f, 0.0f},
    {       0.0f,      -15.0f, 0.0f},
    {       0.0f,        0.0f, 0.0f},
    {     -15.0f,      -15.0f, 0.0f},
    {       0.0f,        0.0f, 0.0f},
    {     -15.0f,        0.0f, 0.0f},
    {       0.0f,       15.0f, 0.0f},
    {     -15.0f,       15.0f, 0.0f},
};

int scaled_ripple_colours[46] __asm__("D_L15_001DF048") = {0};

float scaled_ripple_template_uvs[46][2] __asm__("D_L15_001DEA88") = {
    { 2.949836f, -0.514755f},
    { 2.375023f, -1.375023f},
    {   1.7922f,   -0.7922f},
    { 1.514755f, -1.949835f},
    {      0.5f,   -0.7922f},
    {      0.5f,   -2.0844f},
    {  -0.7922f,   -0.7922f},
    {-0.514755f, -1.949835f},
    {  -0.7922f,   -0.7922f},
    {-1.375023f, -1.375023f},
    {  -0.7922f,   -0.7922f},
    {-1.949835f, -0.514755f},
    {  -0.7922f,       0.5f},
    {  -2.0844f,       0.5f},
    {  -0.7922f,    1.7922f},
    {-1.949835f,  1.514755f},
    {  -0.7922f,    1.7922f},
    {-1.375023f,  2.375022f},
    {  -0.7922f,    1.7922f},
    {-0.514755f,  2.949835f},
    {      0.5f,    1.7922f},
    {      0.5f,    3.0844f},
    {   1.7922f,    1.7922f},
    { 1.514755f,  2.949835f},
    {   1.7922f,    1.7922f},
    { 2.375023f,  2.375022f},
    {   1.7922f,    1.7922f},
    { 2.949835f,  1.514755f},
    {   1.7922f,       0.5f},
    {   3.0844f,       0.5f},
    {   1.7922f,       0.5f},
    { 2.949836f, -0.514755f},
    {   1.7922f,       0.5f},
    {   1.7922f,   -0.7922f},
    {      0.5f,       0.5f},
    {      0.5f,   -0.7922f},
    {      0.5f,       0.5f},
    {  -0.7922f,   -0.7922f},
    {      0.5f,       0.5f},
    {  -0.7922f,       0.5f},
    {      0.5f,       0.5f},
    {  -0.7922f,    1.7922f},
    {      0.5f,       0.5f},
    {      0.5f,    1.7922f},
    {   1.7922f,       0.5f},
    {   1.7922f,    1.7922f},
};

float scaled_ripple_scrolled_uvs[46][2] __asm__("D_L15_001DEED8") = {0};

extern Quad4_2edbe0 l15_scaled_ripple_uv_scroll_factors __asm__("D_L15_00162208");

int scaled_ripple_vertex_counts[1] __asm__("D_L15_001621B0") = {46};

int scaled_ripple_world_vertex_tables[1] __asm__("D_L15_001621D0") = {
    (int)scaled_ripple_world_vertices};

int scaled_ripple_scrolled_uv_tables[1] __asm__("D_L15_001621D8") = {
    (int)scaled_ripple_scrolled_uvs};

int scaled_ripple_colour_tables[1] __asm__("D_L15_001621E0") = {(int)scaled_ripple_colours};

int scaled_ripple_alpha_1_fix __asm__("D_L15_001621E4") = 128;

int scaled_ripple_effect_texture_id __asm__("D_L15_001621E8") = 45;

int scaled_ripple_vertex_alpha __asm__("D_L15_001621EC") = 64;

float scaled_ripple_radial_wavelength_gp __asm__("D_L15_001621F0") = 8.0f;

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_scaled_moby_radial_ripple_mesh.h"

float scaled_ripple_z_amplitude_gp __asm__("D_L15_001621F8") = 0.1f;

float scaled_ripple_uv_scroll_rate_gp __asm__("D_L15_001621FC") = 0.1f;

float scaled_ripple_template_vertex_scale_gp __asm__("D_L15_00162200") = 0.5f;

void draw_scaled_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002edbe0");

void draw_scaled_moby_radial_ripple_mesh(struct Moby *moby) {
    RingVars_2edbe0 *scaled_ripple_phase_state = (RingVars_2edbe0 *)moby->pvars;
    unsigned int     i;
    unsigned int     uv_vertex_index;
    int              j;
    int              k;
    int              r;
    int              g;
    int              b;
    int              color;
    float            scale;
    scale = convert_integer_to_float(D_L15_0015F5CC_macro) *
            (scaled_ripple_uv_scroll_rate_gp * frame_time_macro);
    vu1_add_g_sregister(6, get_effect_texture(scaled_ripple_effect_texture_id));
    vu1_add_g_sregister(0x42, ((long)scaled_ripple_alpha_1_fix << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    scaled_ripple_phase_state->prev = scaled_ripple_phase_state->angle;
    scaled_ripple_phase_state->angle =
        fast_add_rotations(scaled_ripple_phase_state->angle,
                           360.0f / FUN_001f96b0_2edbe0(scaled_ripple_phase_advance_divisor_gp) *
                               0.017453292f * frame_time_macro);
    get_moby_color(moby, &r, &g, &b);
    color = (scaled_ripple_vertex_alpha << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 46; i++) {
        float *template_vertex = scaled_ripple_template_vertices[i];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radius_remainder = FUN_L00_00200260_2edbe0(rad, scaled_ripple_radial_wavelength_gp);
        float  w = fast_normalize_angle(radius_remainder * 6.2831855f /
                                       scaled_ripple_radial_wavelength_gp);
        float  z = fast_sin(fast_add_rotations(w, scaled_ripple_phase_state->angle));
        scaled_ripple_world_vertices[i][0] =
            scaled_ripple_template_vertex_scale_gp * template_vertex[0] + moby->pos.x;
        scaled_ripple_world_vertices[i][1] =
            scaled_ripple_template_vertex_scale_gp * template_vertex[1] + moby->pos.y;
        scaled_ripple_world_vertices[i][2] =
            scaled_ripple_template_vertex_scale_gp * template_vertex[2] + moby->pos.z +
            scaled_ripple_z_amplitude_gp * z;
        scaled_ripple_colours[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 46; uv_vertex_index++) {
            Quad4_2edbe0 uv_scroll_factors = l15_scaled_ripple_uv_scroll_factors;
            scaled_ripple_scrolled_uvs[uv_vertex_index][0] =
                scaled_ripple_template_uvs[uv_vertex_index][0] +
                FUN_L00_00200260_2edbe0(scale * uv_scroll_factors.f[j][0], 1.0f);
            scaled_ripple_scrolled_uvs[uv_vertex_index][1] =
                scaled_ripple_template_uvs[uv_vertex_index][1] +
                FUN_L00_00200260_2edbe0(scale * uv_scroll_factors.f[j][1], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edbe0(scaled_ripple_vertex_counts[k],
                                    scaled_ripple_world_vertex_tables[k],
                                    scaled_ripple_colour_tables[k],
                                    (void *)scaled_ripple_scrolled_uv_tables[k],
                                    1);
        }
    }
}

extern float FUN_001f96b0_2edfc0(float) __asm__("FUN_001f96b0");

extern void FUN_L00_001fde98_2edfc0(int, int, int, void *, int) __asm__("FUN_L00_001fde98");

extern float FUN_L00_00200260_2edfc0(float, float) __asm__("FUN_L00_00200260");

typedef struct {
    float f[2][2];
} Quad4_2edfc0;

typedef struct {
    float prev;
    float angle;
} RingVars_2edfc0;

float ripple_88_world_vertices[88][3] __asm__("D_L15_001DF940") = {0};

float ripple_88_template_vertices[88][3] __asm__("D_L15_001DF100") = {
    {      0.0f,  -4.4e-05f,  -8.4e-05f},
    {-0.545705f, -0.418775f,  -7.9e-05f},
    {-1.422783f,  1.854165f, -0.000104f},
    {-1.968488f,  1.435435f,  -9.9e-05f},
    {-2.845565f,  3.708374f, -0.000121f},
    {-3.391273f,  3.289643f, -0.000116f},
    {-4.268348f,  5.562582f, -0.000138f},
    {-4.814055f,  5.143852f, -0.000135f},
    {-5.691131f,  7.416792f, -0.000157f},
    {-4.814055f,  5.143852f, -0.000135f},
    { -6.23684f,  6.998059f, -0.000155f},
    {-5.359763f,  4.725121f, -0.000133f},
    {-6.782547f,  6.579327f, -0.000152f},
    { -5.90547f,   4.30639f, -0.000128f},
    {-7.328256f,  6.160594f,  -0.00015f},
    {-6.451175f,  3.887654f, -0.000128f},
    {-7.873965f,  5.741859f, -0.000147f},
    {-6.954219f,  3.501659f, -0.000125f},
    {-8.377006f,  5.355859f, -0.000147f},
    {-7.457261f,  3.115659f, -0.000123f},
    { -8.88005f,  4.969857f, -0.000145f},
    {-7.960301f,  2.729661f, -0.000121f},
    {-9.383094f,  4.583856f, -0.000143f},
    {-7.960301f,  2.729661f, -0.000121f},
    {-9.886135f,  4.197855f, -0.000143f},
    {-7.960301f,  2.729661f, -0.000121f},
    {-8.463342f,  2.343659f, -0.000118f},
    {-6.537511f,  0.875461f,  -9.9e-05f},
    {-7.040548f,  0.489462f,  -9.6e-05f},
    {-5.114717f, -0.978736f,  -7.7e-05f},
    {-5.617753f, -1.364733f,  -7.4e-05f},
    {-5.114717f, -0.978736f,  -7.7e-05f},
    {-4.194959f, -3.218929f,    -5e-05f},
    {-5.114717f, -0.978736f,  -7.7e-05f},
    {-3.691922f, -2.832938f,  -5.5e-05f},
    {-4.611681f, -0.592744f,  -7.9e-05f},
    {-3.188887f, -2.446948f,  -5.7e-05f},
    {-4.108642f,  -0.20675f,  -8.2e-05f},
    {-2.685849f, -2.060957f,    -6e-05f},
    {-3.605601f,   0.17924f,  -8.4e-05f},
    {-2.182811f, -1.674966f,  -6.4e-05f},
    {-3.059897f,  0.597974f,  -9.1e-05f},
    {-1.637109f, -1.256235f,  -6.9e-05f},
    {-2.514193f,  1.016704f,  -9.4e-05f},
    {-1.091407f, -0.837505f,  -7.4e-05f},
    {-2.514193f,  1.016704f,  -9.4e-05f},
    {-0.545705f, -0.418775f,  -7.9e-05f},
    {-2.514193f,  1.016704f,  -9.4e-05f},
    {-1.968488f,  1.435435f,  -9.9e-05f},
    {-3.936981f,  2.870914f, -0.000111f},
    {-3.391273f,  3.289643f, -0.000116f},
    {-3.936981f,  2.870914f, -0.000111f},
    {-4.814055f,  5.143852f, -0.000135f},
    {-3.936981f,  2.870914f, -0.000111f},
    {-5.359763f,  4.725121f, -0.000133f},
    {-3.936981f,  2.870914f, -0.000111f},
    { -5.90547f,   4.30639f, -0.000128f},
    {-3.936981f,  2.870914f, -0.000111f},
    {-4.482684f,   2.45218f, -0.000108f},
    {-2.514193f,  1.016704f,  -9.4e-05f},
    {-4.482684f,   2.45218f, -0.000108f},
    {-3.059897f,  0.597974f,  -9.1e-05f},
    {-4.482684f,   2.45218f, -0.000108f},
    {-3.605601f,   0.17924f,  -8.4e-05f},
    {-4.482684f,   2.45218f, -0.000108f},
    {-5.028388f,  2.033447f, -0.000106f},
    { -5.90547f,   4.30639f, -0.000128f},
    {-5.028388f,  2.033447f, -0.000106f},
    {-6.451175f,  3.887654f, -0.000128f},
    {-5.028388f,  2.033447f, -0.000106f},
    {-6.954219f,  3.501659f, -0.000125f},
    {-5.028388f,  2.033447f, -0.000106f},
    { -5.53143f,  1.647454f, -0.000104f},
    {-3.605601f,   0.17924f,  -8.4e-05f},
    { -5.53143f,  1.647454f, -0.000104f},
    {-4.108642f,  -0.20675f,  -8.2e-05f},
    {-6.034472f,  1.261461f, -0.000104f},
    {-4.611681f, -0.592744f,  -7.9e-05f},
    {-6.034472f,  1.261461f, -0.000104f},
    {-5.114717f, -0.978736f,  -7.7e-05f},
    {-6.034472f,  1.261461f, -0.000104f},
    {-6.537511f,  0.875461f,  -9.9e-05f},
    {-6.034472f,  1.261461f, -0.000104f},
    {-7.960301f,  2.729661f, -0.000121f},
    {-6.034472f,  1.261461f, -0.000104f},
    {-7.457261f,  3.115659f, -0.000123f},
    { -5.53143f,  1.647454f, -0.000104f},
    {-6.954219f,  3.501659f, -0.000125f},
};

int ripple_88_colours[88] __asm__("D_L15_001E0020") = {0};

float ripple_88_template_uvs[88][2] __asm__("D_L15_001DF520") = {
    { 1.549487f,  2.971533f},
    { 1.758852f,  2.698682f},
    { 0.622383f,  2.260142f},
    { 0.831748f,  1.987289f},
    {-0.304721f,   1.54875f},
    {-0.095357f,  1.275897f},
    {-1.231826f,  0.837359f},
    {-1.022461f,  0.564505f},
    { -2.15893f,  0.125968f},
    {-1.022461f,  0.564505f},
    {-1.949564f, -0.146887f},
    {-0.813094f,  0.291651f},
    {-1.740197f, -0.419741f},
    {-0.603729f,  0.018797f},
    {-1.530831f, -0.692595f},
    {-0.394361f, -0.254055f},
    {-1.321465f, -0.965449f},
    {-0.201364f, -0.505576f},
    {-1.128464f,  -1.21697f},
    {-0.008365f, -0.757097f},
    {-0.935464f, -1.468491f},
    { 0.184635f, -1.008617f},
    {-0.742463f, -1.720012f},
    { 0.184635f, -1.008617f},
    {-0.549463f, -1.971533f},
    { 0.184635f, -1.008617f},
    { 0.377635f, -1.260136f},
    { 1.111734f, -0.297221f},
    { 1.304734f,  -0.54874f},
    { 2.038833f,  0.414175f},
    { 2.231832f,  0.162657f},
    { 2.038833f,  0.414175f},
    {  3.15893f,  0.874054f},
    { 2.038833f,  0.414175f},
    { 2.965935f,  1.125572f},
    { 1.845837f,  0.665694f},
    { 2.772939f,  1.377091f},
    {  1.65284f,  0.917213f},
    { 2.579944f,  1.628609f},
    { 1.459845f,  1.168733f},
    { 2.386948f,  1.880127f},
    { 1.250479f,  1.441585f},
    { 2.177583f,  2.152979f},
    { 1.041113f,  1.714437f},
    { 1.968218f,   2.42583f},
    { 1.041113f,  1.714437f},
    { 1.758852f,  2.698682f},
    { 1.041113f,  1.714437f},
    { 0.831748f,  1.987289f},
    { 0.114008f,  1.003043f},
    {-0.095357f,  1.275897f},
    { 0.114008f,  1.003043f},
    {-1.022461f,  0.564505f},
    { 0.114008f,  1.003043f},
    {-0.813094f,  0.291651f},
    { 0.114008f,  1.003043f},
    {-0.603729f,  0.018797f},
    { 0.114008f,  1.003043f},
    { 0.323375f,  0.730191f},
    { 1.041113f,  1.714437f},
    { 0.323375f,  0.730191f},
    { 1.250479f,  1.441585f},
    { 0.323375f,  0.730191f},
    { 1.459845f,  1.168733f},
    { 0.323375f,  0.730191f},
    { 0.532742f,  0.457339f},
    {-0.603729f,  0.018797f},
    { 0.532742f,  0.457339f},
    {-0.394361f, -0.254055f},
    { 0.532742f,  0.457339f},
    {-0.201364f, -0.505576f},
    { 0.532742f,  0.457339f},
    { 0.725738f,  0.205818f},
    { 1.459845f,  1.168733f},
    { 0.725738f,  0.205818f},
    {  1.65284f,  0.917213f},
    { 0.918734f, -0.045703f},
    { 1.845837f,  0.665694f},
    { 0.918734f, -0.045703f},
    { 2.038833f,  0.414175f},
    { 0.918734f, -0.045703f},
    { 1.111734f, -0.297221f},
    { 0.918734f, -0.045703f},
    { 0.184635f, -1.008617f},
    { 0.918734f, -0.045703f},
    {-0.008365f, -0.757097f},
    { 0.725738f,  0.205818f},
    {-0.201364f, -0.505576f},
};

float ripple_88_scrolled_uvs[88][2] __asm__("D_L15_001DFD60") = {0};

extern Quad4_2edfc0 l15_ripple_88_uv_scroll_factors __asm__("D_L15_00162278");

int ripple_88_vertex_counts[1] __asm__("D_L15_00162220") = {88};

int ripple_88_world_vertex_tables[1] __asm__("D_L15_00162240") = {(int)ripple_88_world_vertices};

int ripple_88_scrolled_uv_tables[1] __asm__("D_L15_00162248") = {(int)ripple_88_scrolled_uvs};

int ripple_88_colour_tables[1] __asm__("D_L15_00162250") = {(int)ripple_88_colours};

int ripple_88_alpha_1_fix __asm__("D_L15_00162254") = 128;

int ripple_88_effect_texture_id __asm__("D_L15_00162258") = 45;

int ripple_88_vertex_alpha __asm__("D_L15_0016225C") = 64;

float ripple_88_radial_wavelength_gp __asm__("D_L15_00162260") = 8.0f;

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_scaled_88_vertex_moby_radial_ripple_mesh.h"

float ripple_88_z_amplitude_gp __asm__("D_L15_00162268") = 0.1f;

float ripple_88_uv_scroll_rate_gp __asm__("D_L15_0016226C") = 0.1f;

float ripple_88_template_vertex_scale_gp __asm__("D_L15_00162270") = 1.1f;

void draw_scaled_88_vertex_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002edfc0");

void draw_scaled_88_vertex_moby_radial_ripple_mesh(struct Moby *moby) {
    RingVars_2edfc0 *ripple_88_phase_state = (RingVars_2edfc0 *)moby->pvars;
    unsigned int     i;
    unsigned int     uv_vertex_index;
    int              j;
    int              k;
    int              r;
    int              g;
    int              b;
    int              color;
    float            scale;
    scale = convert_integer_to_float(D_L15_0015F5CC_macro) *
            (ripple_88_uv_scroll_rate_gp * frame_time_macro);
    vu1_add_g_sregister(6, get_effect_texture(ripple_88_effect_texture_id));
    vu1_add_g_sregister(0x42, ((long)ripple_88_alpha_1_fix << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    ripple_88_phase_state->prev = ripple_88_phase_state->angle;
    ripple_88_phase_state->angle =
        fast_add_rotations(ripple_88_phase_state->angle,
                           360.0f / FUN_001f96b0_2edfc0(ripple_88_phase_advance_divisor_gp) *
                               0.017453292f * frame_time_macro);
    get_moby_color(moby, &r, &g, &b);
    color = (ripple_88_vertex_alpha << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 88; i++) {
        float *template_vertex = ripple_88_template_vertices[i];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radius_remainder = FUN_L00_00200260_2edfc0(rad, ripple_88_radial_wavelength_gp);
        float  w =
            fast_normalize_angle(radius_remainder * 6.2831855f / ripple_88_radial_wavelength_gp);
        float z = fast_sin(fast_add_rotations(w, ripple_88_phase_state->angle));
        ripple_88_world_vertices[i][0] =
            ripple_88_template_vertex_scale_gp * template_vertex[0] + moby->pos.x;
        ripple_88_world_vertices[i][1] =
            ripple_88_template_vertex_scale_gp * template_vertex[1] + moby->pos.y;
        ripple_88_world_vertices[i][2] = ripple_88_template_vertex_scale_gp * template_vertex[2] +
                                         moby->pos.z + ripple_88_z_amplitude_gp * z;
        ripple_88_colours[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 88; uv_vertex_index++) {
            Quad4_2edfc0 uv_scroll_factors = l15_ripple_88_uv_scroll_factors;
            ripple_88_scrolled_uvs[uv_vertex_index][0] =
                ripple_88_template_uvs[uv_vertex_index][0] +
                FUN_L00_00200260_2edfc0(scale * uv_scroll_factors.f[j][0], 1.0f);
            ripple_88_scrolled_uvs[uv_vertex_index][1] =
                ripple_88_template_uvs[uv_vertex_index][1] +
                FUN_L00_00200260_2edfc0(scale * uv_scroll_factors.f[j][1], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edfc0(ripple_88_vertex_counts[k],
                                    ripple_88_world_vertex_tables[k],
                                    ripple_88_colour_tables[k],
                                    (void *)ripple_88_scrolled_uv_tables[k],
                                    1);
        }
    }
}
