/* Level 18 class 0x57a: a lift and its quad meshes. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

/* Runs the lift's state machine, then queues the draw callback for its current look. */
typedef struct {
    char           p0[0x34];
    unsigned short h34;
} M_2310;

f32 lift_texture_u_offset_gp __asm__("D_L18_00162308") = 0;
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
void synchronize_lift_group_height(void *source_moby) __asm__("FUN_L18_002f2598");

extern void synchronize_lift_group_height(void *a) __asm__("FUN_L18_002f2598");
void        draw_lift_25_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2620");

extern void draw_lift_25_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2620");
void        draw_lift_38_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f27c8");

extern void draw_lift_38_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f27c8");
void        draw_lift_22_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2970");

extern void draw_lift_22_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2970");

#include "rnc/overlay/l18/mobys/lift_globals.h"

int                             lift_alternate_height_requested_gp __asm__("D_L18_001622FC") = 0;
int                             next_lift_instance_index_gp __asm__("D_L18_001622F8") = 0;
float                           lift_texture_u_scroll_rate_gp __asm__("D_L18_0016230C") = 0.05f;
extern unsigned char D_00141400 NOT_SDA;
extern unsigned char D_0013D406 NOT_SDA;
void                            lift_update(struct Moby *lift_moby) __asm__("FUN_L18_002f2310");

/* Moby class 1402 */
void lift_update(struct Moby *lift_moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_SELECT_INITIAL_HEIGHT = 1,
        LIFT_APPROACH_BASE_HEIGHT = 2,
        LIFT_APPROACH_ALTERNATE_HEIGHT = 3
    };
    enum {
        LIFT_DRAW_22_QUAD_MESH = 0,
        LIFT_DRAW_25_QUAD_MESH = 1,
        LIFT_DRAW_38_QUAD_MESH = 2
    };
    char *lift_vars = (char *)lift_moby->pvars;
    int   initial_height_selector;
    switch (lift_moby->state) {
    case LIFT_INITIALIZE:
        if (lift_alternate_height_requested_gp != 0) {
            D_00141400 = *(unsigned char *)&lift_alternate_height_requested_gp;
        }
        *(int *)(lift_vars + 0x1C) = next_lift_instance_index_gp++;
        lift_moby->update_dist = 0xFF;
        lift_moby->state = 1;
        ((M_2310 *)lift_moby)->h34 |= 1;
        lift_moby->draw_dist = 0;
        lift_moby->collision = 0;
        break;
    case LIFT_SELECT_INITIAL_HEIGHT:
        if (D_0013D406 != 0) {
            *(int *)(lift_vars + 8) = (*(int *)(lift_vars + 8) + 1) & 1;
        }
        initial_height_selector = *(int *)(lift_vars + 8);
        next_lift_instance_index_gp = 0;
        if (initial_height_selector != 0 && D_00141400 == 0) {
            lift_moby->state = 2;
            lift_moby->pos.z = *(float *)(lift_vars + 0);
        } else {
            lift_moby->state = 3;
            lift_moby->pos.z = *(float *)(lift_vars + 4);
        }
        break;
    case LIFT_APPROACH_ALTERNATE_HEIGHT:
        if (lift_moby->command & 4) {
            lift_alternate_height_requested_gp = 0;
            lift_moby->state = 2;
            if (*(int *)(lift_vars + 8) == 0) {
                D_0013D406 = 1;
            }
        }
        advance_accelerated_scalar(&lift_moby->pos.z,
                                   (float *)(lift_vars + 0x18),
                                   *(float *)(lift_vars + 4),
                                   frame_time_sq * 4.0f,
                                   frame_time_sq * 4.0f,
                                   frame_time * 4.0f);
        synchronize_lift_group_height(lift_moby);
        lift_moby->command = 1;
        break;
    case LIFT_APPROACH_BASE_HEIGHT:
        if (lift_moby->command & 8) {
            lift_moby->state = 3;
            lift_alternate_height_requested_gp = 1;
        }
        lift_moby->command = 2;
        synchronize_lift_group_height(lift_moby);
        advance_accelerated_scalar(&lift_moby->pos.z,
                                   (float *)(lift_vars + 0x18),
                                   *(float *)(lift_vars + 0),
                                   frame_time_sq * 4.0f,
                                   frame_time_sq * 4.0f,
                                   frame_time * 4.0f);
        break;
    }
    switch (*(int *)(lift_vars + 0x14)) {
    case LIFT_DRAW_25_QUAD_MESH:
        lift_texture_u_offset_gp =
            lift_texture_u_offset_gp + lift_texture_u_scroll_rate_gp * frame_time;
        if (1.0f < lift_texture_u_offset_gp) {
            lift_texture_u_offset_gp -= 1.0f;
        }
        add_draw_callback(draw_lift_25_quad_mesh, lift_moby);
        break;
    case LIFT_DRAW_38_QUAD_MESH:
        add_draw_callback(draw_lift_38_quad_mesh, lift_moby);
        break;
    case LIFT_DRAW_22_QUAD_MESH:
        add_draw_callback(draw_lift_22_quad_mesh, lift_moby);
        break;
    }
}

extern unsigned char *D_L18_0015FFD8;
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F39F8), where it is exact; names translated to the US level program. */

typedef struct {
    int pad[7];
    int idx;
} Lx;

typedef struct {
    char  pad[0x18];
    float f;
    char  pad1[0x78 - 0x1C];
    Lx   *l;
    char  pad2[0xA6 - 0x7C];
    short kind;
    char  pad3[0x100 - 0xA8];
} Mx;

typedef struct {
    int   pad[2];
    float f;
    char  pad2[0x1190 - 12];
} E;

extern E      l18_lift_height_records[] __asm__("D_L18_001DB500");
extern short *D_L18_001AC240_c[] __asm__("D_L18_001AC240");

void synchronize_lift_group_height(void *source_moby) {
    unsigned char *source_moby_bytes = source_moby;
    short         *group_entry = D_L18_001AC240_c[source_moby_bytes[0x21]];
    E             *height_records = l18_lift_height_records;
    do {
        Mx *group_moby =
            (Mx *)(((*(unsigned short *)group_entry & 0x7FFF) << 8) + (int)D_L18_0015FFD8);
        group_moby->f = *(float *)(source_moby_bytes + 0x18);
        if (group_moby->kind == 0x57A) {
            height_records[group_moby->l->idx].f = *(float *)(source_moby_bytes + 0x18);
        }
    } while (*group_entry++ >= 0);
}

/* Draws 25 textured quads from the L18 tables, in the moby's frame. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3A80), where it is exact; names translated to the US level program. */

typedef float FVec4[4] __attribute__((aligned(16)));

typedef struct {
    FVec4 v;
} FRow;

extern short lift_mesh_blend_minuend_selector_gp __asm__("D_L18_00162310") __attribute__((sda));
extern short lift_mesh_blend_subtrahend_selector_gp __asm__("D_L18_00162314") __attribute__((sda));
extern short lift_mesh_blend_factor_selector_gp __asm__("D_L18_00162318") __attribute__((sda));
extern short lift_mesh_blend_addend_selector_gp __asm__("D_L18_0016231C") __attribute__((sda));
extern short lift_mesh_blend_upper_word_gp __asm__("D_L18_00162320") __attribute__((sda));
extern short lift_mesh_effect_texture_index_gp __asm__("D_L18_00162324") __attribute__((sda));
extern short lift_mesh_quad_color_gp __asm__("D_L18_00162328") __attribute__((sda));
extern f32   lift_texture_u_offset_gp __asm__("D_L18_00162308") __attribute__((sda));

/* The three lift meshes. Each quad table holds one {vertex, uv} index pair per corner for every quad the
 * draw loop walks (25, 38, 22 quads); the vertex and uv tables hold every index those quads name
 * (0..36, 0..52, 0..32), and each vertex table ends where the next table starts. */
float lift_25_quad_mesh_uvs[37][2] __asm__("D_L18_001DA7A0") = {
    { 0.991085f,  0.014108f},
    { 0.622955f, -0.048334f},
    { 0.832989f, -0.765736f},
    { 1.046957f, -0.431929f},
    {  0.28113f,  0.539943f},
    { 0.651018f,  0.517551f},
    { 1.588688f,  0.479163f},
    { 1.411064f,   0.82839f},
    { 1.803197f, -0.085286f},
    { 2.172167f,  0.551416f},
    { 2.324286f,  0.171767f},
    { 1.928179f,  1.072332f},
    { 2.961047f,  0.418306f},
    { 2.761576f,  0.883494f},
    { 2.563714f,  1.348922f},
    {-1.879298f, -0.480442f},
    {-2.825109f, -0.261904f},
    {-2.932241f, -0.884101f},
    {-2.146629f, -1.155054f},
    {-1.344083f, -1.666325f},
    {-1.561614f, -2.607748f},
    {-0.935767f, -2.716869f},
    {-0.665758f, -1.936035f},
    { -0.23978f, -1.104623f},
    { 0.790086f, -1.355174f},
    { 0.027547f,  -0.43001f},
    {-0.690948f,  0.049263f},
    {-0.473416f,   0.99068f},
    {-1.099256f,  1.099795f},
    {-1.369265f,  0.318966f},
    {-2.802197f, -1.604041f},
    {-2.347656f, -2.179348f},
    {-1.099949f, -0.873386f},
    {-0.213719f,   -2.5895f},
    { 0.362182f, -2.138316f},
    {-1.821297f,  0.972423f},
    {-2.397209f,  0.521245f},
};

extern FRow lift_25_quad_mesh_vertices[37] __asm__("D_L18_001DA3C0") NOT_SDA;

short lift_25_quad_mesh_quads[25][4][2] __asm__("D_L18_001DA610") = {
    {      {1, 1},       {7, 2},       {4, 0},       {6, 3}},
    {      {2, 5},       {3, 4},       {4, 0},       {1, 1}},
    {      {5, 7},       {2, 5},       {0, 6},       {4, 0}},
    {      {4, 0},       {6, 3},       {0, 6},       {8, 8}},
    {      {0, 6},       {8, 8},     {0xa, 9},     {9, 0xa}},
    {    {0xa, 9},   {0xb, 0xb},       {0, 6},       {5, 7}},
    {  {0xc, 0xd},     {0xa, 9},   {0xd, 0xc},     {9, 0xa}},
    {    {0xa, 9},   {0xc, 0xd},   {0xb, 0xb},   {0xe, 0xe}},
    {{0x18, 0x11}, {0x17, 0x12}, {0x16, 0x10},  {0x1b, 0xf}},
    {{0x1d, 0x15}, {0x1c, 0x16}, {0x13, 0x14}, {0x1a, 0x13}},
    {      {7, 2}, {0x20, 0x19}, {0x14, 0x18}, {0x1f, 0x17}},
    {{0x23, 0x1c}, {0x22, 0x1d}, {0x15, 0x1b}, {0x21, 0x1a}},
    {{0x17, 0x12}, {0x18, 0x11}, {0x19, 0x1f},  {0xf, 0x1e}},
    {{0x1a, 0x13}, {0x17, 0x12}, {0x13, 0x14}, {0x19, 0x1f}},
    {{0x17, 0x12}, {0x1a, 0x13},  {0x1b, 0xf}, {0x12, 0x20}},
    {{0x1c, 0x16}, {0x1d, 0x15}, {0x1e, 0x22}, {0x10, 0x21}},
    {{0x1f, 0x17}, {0x1c, 0x16}, {0x14, 0x18}, {0x1e, 0x22}},
    {{0x1c, 0x16}, {0x1f, 0x17}, {0x1a, 0x13}, {0x12, 0x20}},
    {      {7, 2},       {1, 1}, {0x20, 0x19}, {0x20, 0x19}},
    {{0x20, 0x19},       {1, 1},       {3, 4},       {3, 4}},
    {      {3, 4}, {0x15, 0x1b}, {0x20, 0x19}, {0x21, 0x1a}},
    {{0x20, 0x19}, {0x21, 0x1a}, {0x1f, 0x17}, {0x12, 0x20}},
    {{0x22, 0x1d}, {0x23, 0x1c}, {0x24, 0x24}, {0x11, 0x23}},
    { {0x1b, 0xf}, {0x22, 0x1d}, {0x16, 0x10}, {0x24, 0x24}},
    {{0x22, 0x1d},  {0x1b, 0xf}, {0x21, 0x1a}, {0x12, 0x20}},
};

extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void fast_draw_quad_real(void *, void *, int) __asm__("FUN_001f7d30");

void draw_lift_25_quad_mesh(struct Moby *moby) {
    FRow          quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *lift_private_vars = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)lift_private_vars;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 25; i++) {
        float ofs = *(float *)&lift_texture_u_offset_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = lift_25_quad_mesh_quads[i][j][0];
            int uv_index = lift_25_quad_mesh_quads[i][j][1];
            qcopy(&quad_positions[j], &lift_25_quad_mesh_vertices[vertex_index]);
            uv[j][0] = lift_25_quad_mesh_uvs[uv_index][0] + ofs;
            uv[j][1] = lift_25_quad_mesh_uvs[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

typedef float FVec4_l18[4] __attribute__((aligned(16)));

typedef struct {
    FVec4_l18 v;
} FRow_l18;

float lift_38_quad_mesh_uvs[53][2] __asm__("D_L18_001DAE80") = {
    { -0.05899f,  0.273338f},
    { -0.27086f,  0.168349f},
    {-0.060827f, -0.549053f},
    { 0.188736f, -0.179442f},
    {-0.612686f,  0.756626f},
    {-0.280541f,  0.738764f},
    {-2.773114f, -0.263759f},
    {-3.718925f, -0.045221f},
    {-3.826056f, -0.667419f},
    {-3.040444f, -0.938371f},
    {-2.237899f, -1.449642f},
    {-2.455429f, -2.391065f},
    {-1.829582f, -2.500186f},
    {-1.559573f, -1.719353f},
    {-1.133595f, -0.887941f},
    {-0.103729f, -1.138492f},
    {-0.866268f, -0.213327f},
    {-1.584763f,  0.265945f},
    {-1.367231f,  1.207363f},
    {-1.993072f,  1.316477f},
    {-2.263081f,  0.535648f},
    {-3.696013f, -1.387359f},
    {-3.241472f, -1.962665f},
    {-1.993764f, -0.656704f},
    {-1.107535f, -2.372817f},
    {-0.531634f, -1.921633f},
    {-2.715113f,  1.189106f},
    {-3.291025f,  0.737928f},
    { 0.172998f,   0.38696f},
    {-0.091616f,  1.086073f},
    {  3.22803f,  0.051469f},
    { 2.688549f, -0.442687f},
    { 0.272176f,  2.490812f},
    { 0.559078f, -0.173269f},
    { 3.653335f,  1.493818f},
    { 2.849206f,  1.703515f},
    { 2.634582f,  1.010334f},
    { 3.594398f,  0.865223f},
    { 1.521757f,  3.167525f},
    { 1.312634f,  2.368229f},
    { 2.009701f,  2.151516f},
    { 2.154145f,  3.106886f},
    { 0.737271f,  0.813321f},
    { 0.951894f,  1.506505f},
    {-0.094198f,  1.677065f},
    {  1.97845f, -0.625242f},
    { 2.187575f,  0.174049f},
    { 1.490515f,  0.390756f},
    {  1.34607f, -0.564608f},
    { 3.468279f,  2.201618f},
    { 2.970818f,  2.740242f},
    { 1.827307f,  1.342142f},
    { 0.811652f,  2.984973f},
};

extern FRow_l18 lift_38_quad_mesh_vertices[53] __asm__("D_L18_001DA8D0") NOT_SDA;

short lift_38_quad_mesh_quads[38][4][2] __asm__("D_L18_001DAC20") = {
    {      {0, 1},       {5, 2},       {3, 0},       {4, 3}},
    {      {1, 5},       {2, 4},       {3, 0},       {0, 1}},
    {    {0xf, 8},     {0xe, 9},     {0xd, 7},    {0x12, 6}},
    { {0x14, 0xc},  {0x13, 0xd},   {0xa, 0xb},  {0x11, 0xa}},
    {      {5, 2}, {0x17, 0x10},   {0xb, 0xf},  {0x16, 0xe}},
    {{0x1a, 0x13}, {0x19, 0x14},  {0xc, 0x12}, {0x18, 0x11}},
    {    {0xe, 9},     {0xf, 8}, {0x10, 0x16},    {6, 0x15}},
    { {0x11, 0xa},     {0xe, 9},   {0xa, 0xb}, {0x10, 0x16}},
    {    {0xe, 9},  {0x11, 0xa},    {0x12, 6},    {9, 0x17}},
    { {0x13, 0xd},  {0x14, 0xc}, {0x15, 0x19},    {7, 0x18}},
    { {0x16, 0xe},  {0x13, 0xd},   {0xb, 0xf}, {0x15, 0x19}},
    { {0x13, 0xd},  {0x16, 0xe},  {0x11, 0xa},    {9, 0x17}},
    {      {5, 2},       {0, 1}, {0x17, 0x10}, {0x17, 0x10}},
    {{0x17, 0x10},       {0, 1},       {2, 4},       {2, 4}},
    {      {2, 4},  {0xc, 0x12}, {0x17, 0x10}, {0x18, 0x11}},
    {{0x17, 0x10}, {0x18, 0x11},  {0x16, 0xe},    {9, 0x17}},
    {{0x19, 0x14}, {0x1a, 0x13}, {0x1b, 0x1b},    {8, 0x1a}},
    {   {0x12, 6}, {0x19, 0x14},     {0xd, 7}, {0x1b, 0x1b}},
    {{0x19, 0x14},    {0x12, 6}, {0x18, 0x11},    {9, 0x17}},
    {{0x1c, 0x1c}, {0x1e, 0x1d},       {3, 0},       {1, 5}},
    {      {4, 3}, {0x1d, 0x21},       {3, 0}, {0x1c, 0x1c}},
    {{0x28, 0x22}, {0x27, 0x23}, {0x26, 0x25}, {0x2b, 0x24}},
    {{0x2d, 0x26}, {0x2c, 0x27}, {0x23, 0x29}, {0x2a, 0x28}},
    {{0x1e, 0x1d}, {0x30, 0x2a}, {0x24, 0x2c}, {0x2f, 0x2b}},
    {{0x33, 0x2d}, {0x32, 0x2e}, {0x25, 0x30}, {0x31, 0x2f}},
    {{0x27, 0x23}, {0x28, 0x22}, {0x29, 0x32}, {0x1f, 0x31}},
    {{0x2a, 0x28}, {0x27, 0x23}, {0x23, 0x29}, {0x29, 0x32}},
    {{0x27, 0x23}, {0x2a, 0x28}, {0x2b, 0x24}, {0x22, 0x33}},
    {{0x2c, 0x27}, {0x2d, 0x26}, {0x2e, 0x20}, {0x20, 0x34}},
    {{0x2f, 0x2b}, {0x2c, 0x27}, {0x24, 0x2c}, {0x2e, 0x20}},
    {{0x2c, 0x27}, {0x2f, 0x2b}, {0x2a, 0x28}, {0x22, 0x33}},
    {{0x1e, 0x1d}, {0x1c, 0x1c}, {0x30, 0x2a}, {0x30, 0x2a}},
    {{0x30, 0x2a}, {0x1c, 0x1c}, {0x1d, 0x21}, {0x1d, 0x21}},
    {{0x1d, 0x21}, {0x25, 0x30}, {0x30, 0x2a}, {0x31, 0x2f}},
    {{0x30, 0x2a}, {0x31, 0x2f}, {0x2f, 0x2b}, {0x22, 0x33}},
    {{0x32, 0x2e}, {0x33, 0x2d}, {0x34, 0x1e}, {0x21, 0x1f}},
    {{0x2b, 0x24}, {0x32, 0x2e}, {0x26, 0x25}, {0x34, 0x1e}},
    {{0x32, 0x2e}, {0x2b, 0x24}, {0x31, 0x2f}, {0x22, 0x33}},
};

extern int get_effect_texture(int) __asm__("FUN_001f44b8");

void draw_lift_38_quad_mesh(struct Moby *moby) {
    FRow_l18      quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *lift_mesh_private_vars = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)lift_mesh_private_vars;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 38; i++) {
        float ofs = *(float *)&lift_texture_u_offset_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = lift_38_quad_mesh_quads[i][j][0];
            int uv_index = lift_38_quad_mesh_quads[i][j][1];

            qcopy(&quad_positions[j], &lift_38_quad_mesh_vertices[vertex_index]);
            uv[j][0] = lift_38_quad_mesh_uvs[uv_index][0] + ofs;
            uv[j][1] = lift_38_quad_mesh_uvs[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

/* Draws 22 textured quads from the L18 tables, in the moby's frame. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3DD0), where it is exact; names translated to the US level program. */

float lift_22_quad_mesh_uvs[33][2] __asm__("D_L18_001DB3A0") = {
    {   -2.009f,  0.006532f},
    {-1.943015f,  0.624087f},
    { -3.58522f,  0.915478f},
    {-2.898298f, -0.041709f},
    {-1.977804f,  1.475602f},
    {-2.165067f,    2.5826f},
    {-2.981234f,  2.771913f},
    {-3.220577f,  1.850971f},
    {-0.407407f, -1.146144f},
    { 1.260549f,  -1.76459f},
    {  0.79458f, -0.702642f},
    {-0.760162f, -0.246067f},
    {-0.748426f,  0.742158f},
    { 0.283627f,  0.406374f},
    { -0.15364f,  1.955618f},
    {-0.988016f,  1.722411f},
    {-1.268178f,  2.527367f},
    {-0.293786f,  2.738372f},
    {-0.570533f,  3.476167f},
    {-1.500379f,  3.598064f},
    {-2.543128f, -0.458205f},
    {-1.963157f,  -0.69763f},
    {-2.140291f,  3.338851f},
    {-2.683285f,  3.092206f},
    { 3.391456f, -2.320742f},
    { 2.936514f,  -0.40155f},
    { 1.025043f, -2.870421f},
    { 2.705978f, -3.544365f},
    { -0.77812f, -2.167609f},
    { 0.841727f,  0.356447f},
    { 1.038983f,  2.049229f},
    { 0.383608f,  2.753696f},
    { -0.08205f,   3.19345f},
};

extern FRow_l18 lift_22_quad_mesh_vertices[33] __asm__("D_L18_001DB030") NOT_SDA;

short lift_22_quad_mesh_quads[22][4][2] __asm__("D_L18_001DB240") = {
    {      {0, 0},       {1, 1},       {3, 3},       {2, 2}},
    {      {4, 4},       {5, 5},       {7, 7},       {6, 6}},
    {      {8, 8},       {9, 9},   {0xb, 0xb},   {0xa, 0xa}},
    {  {0xc, 0xc},   {0xd, 0xd},   {0xf, 0xf},   {0xe, 0xe}},
    {{0x10, 0x10}, {0x11, 0x11}, {0x13, 0x13}, {0x12, 0x12}},
    {{0x14, 0x14}, {0x15, 0x15},       {3, 3},       {0, 0}},
    {      {0, 0}, {0x15, 0x15},   {0xb, 0xb},       {8, 8}},
    {      {2, 2},       {1, 1},       {7, 7},       {4, 4}},
    {      {4, 4},       {1, 1},   {0xf, 0xf},   {0xc, 0xc}},
    {  {0xc, 0xc},       {1, 1},   {0xb, 0xb},       {0, 0}},
    {      {6, 6},       {5, 5}, {0x17, 0x17}, {0x16, 0x16}},
    {{0x16, 0x16},       {5, 5}, {0x13, 0x13}, {0x10, 0x10}},
    {{0x10, 0x10},       {5, 5},   {0xf, 0xf},       {4, 4}},
    {  {0xa, 0xa},       {9, 9}, {0x19, 0x19}, {0x18, 0x18}},
    {{0x18, 0x18},       {9, 9}, {0x1b, 0x1b}, {0x1a, 0x1a}},
    {{0x1a, 0x1a},       {9, 9}, {0x1c, 0x1c},       {8, 8}},
    {  {0xe, 0xe},   {0xd, 0xd}, {0x1e, 0x1e}, {0x1d, 0x1d}},
    {{0x1d, 0x1d},   {0xd, 0xd}, {0x19, 0x19},   {0xa, 0xa}},
    {  {0xa, 0xa},   {0xd, 0xd},   {0xb, 0xb},   {0xc, 0xc}},
    {{0x12, 0x12}, {0x11, 0x11}, {0x20, 0x20}, {0x1f, 0x1f}},
    {{0x1f, 0x1f}, {0x11, 0x11}, {0x1e, 0x1e},   {0xe, 0xe}},
    {  {0xe, 0xe}, {0x11, 0x11},   {0xf, 0xf}, {0x10, 0x10}},
};

void draw_lift_22_quad_mesh(struct Moby *moby) {
    FRow_l18      quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *lift_draw_private_vars = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)lift_draw_private_vars;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 22; i++) {
        float ofs = *(float *)&lift_texture_u_offset_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = lift_22_quad_mesh_quads[i][j][0];
            int uv_index = lift_22_quad_mesh_quads[i][j][1];
            qcopy(&quad_positions[j], &lift_22_quad_mesh_vertices[vertex_index]);
            uv[j][0] = lift_22_quad_mesh_uvs[uv_index][0] + ofs;
            uv[j][1] = lift_22_quad_mesh_uvs[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}
