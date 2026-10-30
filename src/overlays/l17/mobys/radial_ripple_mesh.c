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

T4 D_L17_00162268 = {
    {{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}
};

extern float FUN_L00_00200260(float, float);
int          D_L17_00162248 = 128;
int          D_L17_0016224C = 45;
int          D_L17_00162250 = 64;

#include "rnc/overlay/l17/mobys/radial_ripple_mesh_globals.h"

extern float ripple_pair_radial_phase_distance_scale_gp __asm__("D_L17_00162254")
    __attribute__((sda));
extern float ripple_pair_phase_period_gp __asm__("D_L17_00162258") __attribute__((sda));
extern float ripple_pair_z_amplitude_gp __asm__("D_L17_0016225C") __attribute__((sda));
extern float ripple_pair_uv_scroll_rate_gp __asm__("D_L17_00162260") __attribute__((sda));
extern void  FUN_L00_001fde98(int, int, int, void *, int);
void         font_queue_vu_state(void) __asm__("FUN_001f76a0");

int D_L17_00162210[2] = {0x94, 0xb};

extern char D_L17_001DAF30[];
extern char D_L17_001DBD10[];
int         D_L17_00162218[2] = {(int)D_L17_001DAF30, (int)D_L17_001DBD10};

extern char D_L17_001DB620[];
extern char D_L17_001DBD98[];
int         D_L17_00162220[2] = {(int)D_L17_001DB620, (int)D_L17_001DBD98};

extern char D_L17_001DBAC0[];
extern char D_L17_001DBDF0[];
int         D_L17_00162228[2] = {(int)D_L17_001DBAC0, (int)D_L17_001DBDF0};

extern char D_L17_001DA040[];
extern char D_L17_001DA730[];
int         D_L17_00162230[2] = {(int)D_L17_001DA040, (int)D_L17_001DA730};

extern char D_L17_001DA7B8[];
extern char D_L17_001DAC58[];
int         D_L17_00162238[2] = {(int)D_L17_001DA7B8, (int)D_L17_001DAC58};

void draw_moby_radial_ripple_mesh_pair(struct Moby *moby) __asm__("FUN_L17_002f04d0");

void draw_moby_radial_ripple_mesh_pair(struct Moby *moby) {
    RippleData *ripple_vars = (RippleData *)moby->pvars;
    float       scale;
    int         r, g, b;
    int         color;
    int         i, j, uv_strip_index, uv_vertex_index, draw_strip_index;

    scale = convert_integer_to_float(D_L17_0015F5CC) * (ripple_pair_uv_scroll_rate_gp * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(D_L17_0016224C));
    vu1_add_g_sregister(0x42, ((long)D_L17_00162248 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    ripple_vars->f0 = ripple_vars->f4;
    ripple_vars->f4 = fast_add_rotations(ripple_vars->f4,
                                         360.0f / scale_time(ripple_pair_phase_period_gp) *
                                             DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L17_00162250 << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < D_L17_00162210[i]; j++) {
            int   *template_vertex_array_slot = D_L17_00162230 + i;
            float *p = (float *)(*template_vertex_array_slot + j * 12);
            float  t = ripple_pair_z_amplitude_gp *
                      fast_sin(fast_add_rotations(
                          fast_normalize_angle(
                              FUN_L00_00200260(square_root(p[0] * p[0] + p[1] * p[1]),
                                               ripple_pair_radial_phase_distance_scale_gp) *
                              6.2831855f / ripple_pair_radial_phase_distance_scale_gp),
                          ripple_vars->f4));
            *(float *)(j * 12 + D_L17_00162218[i]) = p[0] + moby->pos.x;
            *(float *)(j * 12 + D_L17_00162218[i] + 4) = p[1] + moby->pos.y;
            *(float *)(j * 12 + D_L17_00162218[i] + 8) = p[2] + moby->pos.z + t;
            *(int *)(j * 4 + D_L17_00162228[i]) = color;
        }
    }
    for (i = 0; i < 2; i++) {
        for (uv_strip_index = 0; uv_strip_index < 2; uv_strip_index++) {
            for (uv_vertex_index = 0; uv_vertex_index < D_L17_00162210[uv_strip_index];
                 uv_vertex_index++) {
                T4 uv_scroll_factors = D_L17_00162268;
                *(float *)(uv_vertex_index * 8 + D_L17_00162220[uv_strip_index]) =
                    *(float *)(uv_vertex_index * 8 + D_L17_00162238[uv_strip_index]) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[0], 1.0f);
                *(float *)(uv_vertex_index * 8 + D_L17_00162220[uv_strip_index] + 4) =
                    *(float *)(uv_vertex_index * 8 + D_L17_00162238[uv_strip_index] + 4) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[1], 1.0f);
            }
        }
        font_queue_vu_state();
        for (draw_strip_index = 0; draw_strip_index < 2; draw_strip_index++) {
            FUN_L00_001fde98(D_L17_00162210[draw_strip_index],
                             D_L17_00162218[draw_strip_index],
                             D_L17_00162228[draw_strip_index],
                             (void *)D_L17_00162220[draw_strip_index],
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

extern RingPt l17_ripple_ring_template_vertices[] __asm__("D_L17_001DBE20");
extern RingPt l17_ripple_ring_world_vertices[] __asm__("D_L17_001DC680");

T4_c D_L17_001622D0 = {
    {{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}
};

#include "rnc/overlay/l17/mobys/radial_ripple_mesh_data/draw_ripple_ring_2.h"

extern float ring_template_uvs[][2] __asm__("D_L17_001DC250");
extern float ring_scrolled_uvs[][2] __asm__("D_L17_001DCAB0");
int          D_L17_00162280[2] = {0x59, 0};

int D_L17_001622A0[2] = {(int)l17_ripple_ring_world_vertices, 0};

int D_L17_001622A8[2] = {(int)ring_scrolled_uvs, 0};

extern int ring_vertex_colors[] __asm__("D_L17_001DCD78");
int        D_L17_001622B0[2] = {(int)ring_vertex_colors, 0x80};

int D_L17_001622B4 = 128;
int D_L17_001622B8 = 45;
int D_L17_001622BC = 64;

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
    vu1_add_g_sregister(6, get_effect_texture(D_L17_001622B8));
    vu1_add_g_sregister(0x42, ((long)D_L17_001622B4 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    ring_ripple_vars->f0 = ring_ripple_vars->f4;
    ring_ripple_vars->f4 = fast_add_rotations(ring_ripple_vars->f4,
                                              360.0f / scale_time(ring_ripple_phase_period_gp) *
                                                  DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L17_001622BC << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < D_L17_00162280[0]; i++) {
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
        for (ring_uv_vertex_index = 0; ring_uv_vertex_index < D_L17_00162280[0];
             ring_uv_vertex_index++) {
            ring_uv_scroll_factors = D_L17_001622D0;
            ring_scrolled_uvs[ring_uv_vertex_index][0] =
                ring_template_uvs[ring_uv_vertex_index][0] +
                FUN_L00_00200260(scale * ring_uv_scroll_factors.p[j].v[0], 1.0f);
            ring_scrolled_uvs[ring_uv_vertex_index][1] =
                ring_template_uvs[ring_uv_vertex_index][1] +
                FUN_L00_00200260(scale * ring_uv_scroll_factors.p[j].v[1], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(D_L17_00162280[k],
                             D_L17_001622A0[k],
                             D_L17_001622B0[k],
                             (void *)D_L17_001622A8[k],
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

Quad4 D_L17_00162390 = {
    {1.0f, -0.62831855f, 1.0f, 0.62831855f}
};

extern float ripple_uv_scroll_rate_gp __asm__("D_L17_0016238C") __attribute__((sda));

float D_L17_001DD760[135] = {
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

float D_L17_001DD980[45][2] = {
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

float D_L17_001DDBA0[135] = {0};

float D_L17_001DDDC0[45][2] = {0};

int D_L17_00162340[2] = {0x2d, 0};

int D_L17_00162360[2] = {(int)D_L17_001DDBA0, 0};

int D_L17_00162368[2] = {(int)D_L17_001DDDC0, 0};

int D_L17_001DDF28[45] = {0};

int D_L17_00162370[1] = {(int)D_L17_001DDF28};

int          D_L17_00162374 = 128;
int          D_L17_00162378 = 45;
int          D_L17_0016237C = 64;
extern float ripple_radial_phase_distance_scale_gp __asm__("D_L17_00162380") __attribute__((sda));
extern float ripple_phase_period_gp __asm__("D_L17_00162384") __attribute__((sda));
extern float ripple_z_amplitude_gp __asm__("D_L17_00162388") __attribute__((sda));

void draw_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L17_002f10c8");

void draw_moby_radial_ripple_mesh(struct Moby *moby) {
    unsigned int i = 0;
    unsigned int output_vertex_byte_offset = 0;
    RingVars    *data = (RingVars *)moby->pvars;
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
    vu1_add_g_sregister(6, get_effect_texture(D_L17_00162378));
    vu1_add_g_sregister(0x42, ((long)D_L17_00162374 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = fast_add_rotations(
        data->angle, 360.0f / scale_time(ripple_phase_period_gp) * DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    /* The second float of each pair in t: p->f[0] is t.f[1], p->f[2] is t.f[3]. */
    v_scroll_factors = (Quad4 *)&uv_scroll_factors.f[1];
    color = (D_L17_0016237C << 24) | (b << 16) | (g << 8) | r;
    output_vertex = D_L17_001DDBA0;
    do {
        float *template_vertex = &D_L17_001DD760[i * 3];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radial_phase_input = FUN_L00_00200260(rad, ripple_radial_phase_distance_scale_gp);
        float  w = fast_normalize_angle(radial_phase_input * 6.2831855f /
                                       ripple_radial_phase_distance_scale_gp);
        float  z = fast_sin(fast_add_rotations(w, data->angle));
        output_vertex[0] = template_vertex[0] + moby->pos.x;
        *(float *)((char *)D_L17_001DDBA0 + 4 + output_vertex_byte_offset) =
            template_vertex[1] + moby->pos.y;
        output_vertex[2] = template_vertex[2] + moby->pos.z + ripple_z_amplitude_gp * z;
        D_L17_001DDF28[i] = color;
        i++;
        output_vertex_byte_offset += 12;
        output_vertex += 3;
    } while (i < 45);
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 45; uv_vertex_index++) {
            uv_scroll_factors = D_L17_00162390;
            D_L17_001DDDC0[uv_vertex_index][0] =
                D_L17_001DD980[uv_vertex_index][0] +
                FUN_L00_00200260(scale * uv_scroll_factors.f[j * 2], 1.0f);
            D_L17_001DDDC0[uv_vertex_index][1] =
                D_L17_001DD980[uv_vertex_index][1] +
                FUN_L00_00200260(scale * v_scroll_factors->f[j * 2], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(D_L17_00162340[k],
                             D_L17_00162360[k],
                             D_L17_00162370[k],
                             (void *)D_L17_00162368[k],
                             1);
        }
    }
}
