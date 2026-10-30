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

T4 D_L15_00162108 = {
    {{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}
};

extern float FUN_L00_00200260(float, float);

extern float fast_add_rotations(float, float);

extern int D_L15_0015F5CC;

int D_L15_001620EC = 128;

int D_L15_001620F0 = 45;

int D_L15_001620F4 = 64;

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_moby_radial_ripple_mesh_buffers.h"

extern float ripple_mesh_buffers_z_amplitude_gp __asm__("D_L15_00162100") __attribute__((sda));

extern s64 get_effect_texture(s32) __asm__("func_001F44B8");

extern float ripple_mesh_buffers_uv_scroll_rate_gp __asm__("D_L15_00162104") __attribute__((sda));

extern void FUN_L00_001fde98(int, int, int, void *, int);

extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");

void font_queue_vu_state(void) __asm__("FUN_001f76a0");

int D_L15_00162080[2] = {0x94, 0x94};

extern char D_L15_001DBCF0[];

extern char D_L15_001DCAD0[];

int D_L15_00162090[2] = {(int)D_L15_001DBCF0, (int)D_L15_001DCAD0};

extern char D_L15_001DC3E0[];

extern char D_L15_001DD1C0[];

int D_L15_001620A0[2] = {(int)D_L15_001DC3E0, (int)D_L15_001DD1C0};

extern char D_L15_001DC880[];

extern char D_L15_001DD660[];

int D_L15_001620B0[2] = {(int)D_L15_001DC880, (int)D_L15_001DD660};

extern char D_L15_001D9E40[];

extern char D_L15_001DA530[];

int D_L15_001620C0[2] = {(int)D_L15_001D9E40, (int)D_L15_001DA530};

extern char D_L15_001DAD98[];

extern char D_L15_001DB238[];

int D_L15_001620D0[2] = {(int)D_L15_001DAD98, (int)D_L15_001DB238};

void draw_moby_radial_ripple_mesh_buffers(struct Moby *moby) __asm__("FUN_L15_002eac90");

void draw_moby_radial_ripple_mesh_buffers(struct Moby *moby) {
    RippleData *ripple_data = (RippleData *)moby->pvars;
    float       scale;
    int         r, g, b;
    int         color;
    int         i, j, uv_buffer_index, uv_vertex_index, draw_buffer_index;

    scale = convert_integer_to_float(D_L15_0015F5CC) *
            (ripple_mesh_buffers_uv_scroll_rate_gp * frame_time);
    vu1_add_g_sregister(6, get_effect_texture(D_L15_001620F0));
    vu1_add_g_sregister(0x42, ((long)D_L15_001620EC << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, ((long)0xFF90 << 32) | 0x260);
    ripple_data->f0 = ripple_data->f4;
    ripple_data->f4 = fast_add_rotations(
        ripple_data->f4,
        360.0f / multiply_global_factor_ed64(ripple_mesh_buffers_phase_advance_divisor_gp) *
            DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L15_001620F4 << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < D_L15_00162080[i]; j++) {
            int   *template_vertex_buffer_entry = D_L15_001620C0 + i;
            float *p = (float *)(*template_vertex_buffer_entry + j * 12);
            float  t = ripple_mesh_buffers_z_amplitude_gp *
                      fast_sin(fast_add_rotations(
                          fast_normalize_angle(
                              FUN_L00_00200260(square_root(p[0] * p[0] + p[1] * p[1]),
                                               ripple_mesh_buffers_radial_wavelength_gp) *
                              6.2831855f / ripple_mesh_buffers_radial_wavelength_gp),
                          ripple_data->f4));
            *(float *)(j * 12 + D_L15_00162090[i]) = p[0] + moby->pos.x;
            *(float *)(j * 12 + D_L15_00162090[i] + 4) = p[1] + moby->pos.y;
            *(float *)(j * 12 + D_L15_00162090[i] + 8) = p[2] + moby->pos.z + t;
            *(int *)(j * 4 + D_L15_001620B0[i]) = color;
        }
    }
    for (i = 0; i < 2; i++) {
        for (uv_buffer_index = 0; uv_buffer_index < 3; uv_buffer_index++) {
            for (uv_vertex_index = 0; uv_vertex_index < D_L15_00162080[uv_buffer_index];
                 uv_vertex_index++) {
                T4 uv_scroll_factors = D_L15_00162108;
                *(float *)(uv_vertex_index * 8 + D_L15_001620A0[uv_buffer_index]) =
                    *(float *)(uv_vertex_index * 8 + D_L15_001620D0[uv_buffer_index]) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[0], 1.0f);
                *(float *)(uv_vertex_index * 8 + D_L15_001620A0[uv_buffer_index] + 4) =
                    *(float *)(uv_vertex_index * 8 + D_L15_001620D0[uv_buffer_index] + 4) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[1], 1.0f);
            }
        }
        font_queue_vu_state();
        for (draw_buffer_index = 0; draw_buffer_index < 3; draw_buffer_index++) {
            FUN_L00_001fde98(D_L15_00162080[draw_buffer_index],
                             D_L15_00162090[draw_buffer_index],
                             D_L15_001620B0[draw_buffer_index],
                             (void *)D_L15_001620A0[draw_buffer_index],
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

Quad4 D_L15_00162170 = {
    {1.0f, -0.62831855f, 1.0f, 0.62831855f}
};

extern float ripple_mesh_uv_scroll_rate_gp __asm__("D_L15_0016216C") __attribute__((sda));

float D_L15_001DDBA0[204] = {
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

float D_L15_001DDED0[68][2] = {
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

float D_L15_001DE200[204] = {0};

float D_L15_001DE530[68][2] = {0};

int D_L15_00162120[2] = {0x44, 0};

int D_L15_00162140[2] = {(int)D_L15_001DE200, 0};

int D_L15_00162148[2] = {(int)D_L15_001DE530, 0};

int D_L15_001DE750[68] = {0};

int D_L15_00162150[1] = {(int)D_L15_001DE750};

int D_L15_00162154 = 128;

int D_L15_00162158 = 45;

int D_L15_0016215C = 64;

extern float ripple_mesh_radial_wavelength_gp __asm__("D_L15_00162160") __attribute__((sda));

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_moby_radial_ripple_mesh.h"

extern float ripple_mesh_z_amplitude_gp __asm__("D_L15_00162168") __attribute__((sda));

void draw_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002eb108");

void draw_moby_radial_ripple_mesh(struct Moby *moby) {
    unsigned int i = 0;
    unsigned int output_vertex_byte_offset = 0;
    RingVars    *data = (RingVars *)moby->pvars;
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
    vu1_add_g_sregister(6, get_effect_texture(D_L15_00162158));
    vu1_add_g_sregister(0x42, ((long)D_L15_00162154 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = fast_add_rotations(
        data->angle,
        360.0f / multiply_global_factor_ed64(ripple_mesh_phase_advance_divisor_gp) * DEG_TO_RAD *
            frame_time);
    get_moby_color(moby, &r, &g, &b);
    /* The second float of each pair in t: p->f[0] is t.f[1], p->f[2] is t.f[3]. */
    v_scroll_factors = (Quad4 *)&uv_scroll_factors.f[1];
    color = (D_L15_0016215C << 24) | (b << 16) | (g << 8) | r;
    output_vertex = D_L15_001DE200;
    do {
        float *template_vertex = &D_L15_001DDBA0[i * 3];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radius_remainder = FUN_L00_00200260(rad, ripple_mesh_radial_wavelength_gp);
        float  w =
            fast_normalize_angle(radius_remainder * 6.2831855f / ripple_mesh_radial_wavelength_gp);
        float z = fast_sin(fast_add_rotations(w, data->angle));
        output_vertex[0] = template_vertex[0] + moby->pos.x;
        *(float *)((char *)D_L15_001DE200 + 4 + output_vertex_byte_offset) =
            template_vertex[1] + moby->pos.y;
        output_vertex[2] = template_vertex[2] + moby->pos.z + ripple_mesh_z_amplitude_gp * z;
        D_L15_001DE750[i] = color;
        i++;
        output_vertex_byte_offset += 12;
        output_vertex += 3;
    } while (i < 68);
    for (j = 0; j < 2; j++) {
        for (scrolled_uv_vertex_index = 0; scrolled_uv_vertex_index < 68;
             scrolled_uv_vertex_index++) {
            uv_scroll_factors = D_L15_00162170;
            D_L15_001DE530[scrolled_uv_vertex_index][0] =
                D_L15_001DDED0[scrolled_uv_vertex_index][0] +
                FUN_L00_00200260(scale * uv_scroll_factors.f[j * 2], 1.0f);
            D_L15_001DE530[scrolled_uv_vertex_index][1] =
                D_L15_001DDED0[scrolled_uv_vertex_index][1] +
                FUN_L00_00200260(scale * v_scroll_factors->f[j * 2], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(D_L15_00162120[k],
                             D_L15_00162140[k],
                             D_L15_00162150[k],
                             (void *)D_L15_00162148[k],
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

float D_L15_001DECB0[46][3] = {0};

float D_L15_001DE860[46][3] = {
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

int D_L15_001DF048[46] = {0};

float D_L15_001DEA88[46][2] = {
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

float D_L15_001DEED8[46][2] = {0};

extern Quad4_2edbe0 l15_scaled_ripple_uv_scroll_factors __asm__("D_L15_00162208");

int D_L15_001621B0[1] = {0x2e};

int D_L15_001621D0[1] = {(int)D_L15_001DECB0};

int D_L15_001621D8[1] = {(int)D_L15_001DEED8};

int D_L15_001621E0[1] = {(int)D_L15_001DF048};

int D_L15_001621E4 = 128;

int D_L15_001621E8 = 45;

int D_L15_001621EC = 64;

extern float scaled_ripple_radial_wavelength_gp __asm__("D_L15_001621F0") __attribute__((sda));

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_scaled_moby_radial_ripple_mesh.h"

extern float scaled_ripple_z_amplitude_gp __asm__("D_L15_001621F8") __attribute__((sda));

extern float scaled_ripple_uv_scroll_rate_gp __asm__("D_L15_001621FC") __attribute__((sda));

extern float scaled_ripple_template_vertex_scale_gp __asm__("D_L15_00162200") __attribute__((sda));

void draw_scaled_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002edbe0");

void draw_scaled_moby_radial_ripple_mesh(struct Moby *moby) {
    RingVars_2edbe0 *data = (RingVars_2edbe0 *)moby->pvars;
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
    vu1_add_g_sregister(6, get_effect_texture(D_L15_001621E8));
    vu1_add_g_sregister(0x42, ((long)D_L15_001621E4 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle =
        fast_add_rotations(data->angle,
                           360.0f / FUN_001f96b0_2edbe0(scaled_ripple_phase_advance_divisor_gp) *
                               0.017453292f * frame_time_macro);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L15_001621EC << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 46; i++) {
        float *template_vertex = D_L15_001DE860[i];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radius_remainder = FUN_L00_00200260_2edbe0(rad, scaled_ripple_radial_wavelength_gp);
        float  w = fast_normalize_angle(radius_remainder * 6.2831855f /
                                       scaled_ripple_radial_wavelength_gp);
        float  z = fast_sin(fast_add_rotations(w, data->angle));
        D_L15_001DECB0[i][0] =
            scaled_ripple_template_vertex_scale_gp * template_vertex[0] + moby->pos.x;
        D_L15_001DECB0[i][1] =
            scaled_ripple_template_vertex_scale_gp * template_vertex[1] + moby->pos.y;
        D_L15_001DECB0[i][2] = scaled_ripple_template_vertex_scale_gp * template_vertex[2] +
                               moby->pos.z + scaled_ripple_z_amplitude_gp * z;
        D_L15_001DF048[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 46; uv_vertex_index++) {
            Quad4_2edbe0 uv_scroll_factors = l15_scaled_ripple_uv_scroll_factors;
            D_L15_001DEED8[uv_vertex_index][0] =
                D_L15_001DEA88[uv_vertex_index][0] +
                FUN_L00_00200260_2edbe0(scale * uv_scroll_factors.f[j][0], 1.0f);
            D_L15_001DEED8[uv_vertex_index][1] =
                D_L15_001DEA88[uv_vertex_index][1] +
                FUN_L00_00200260_2edbe0(scale * uv_scroll_factors.f[j][1], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edbe0(D_L15_001621B0[k],
                                    D_L15_001621D0[k],
                                    D_L15_001621E0[k],
                                    (void *)D_L15_001621D8[k],
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

float D_L15_001DF940[88][3] = {0};

float D_L15_001DF100[88][3] = {
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

int D_L15_001E0020[88] = {0};

float D_L15_001DF520[88][2] = {
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

float D_L15_001DFD60[88][2] = {0};

extern Quad4_2edfc0 l15_ripple_88_uv_scroll_factors __asm__("D_L15_00162278");

int D_L15_00162220[1] = {0x58};

int D_L15_00162240[1] = {(int)D_L15_001DF940};

int D_L15_00162248[1] = {(int)D_L15_001DFD60};

int D_L15_00162250[1] = {(int)D_L15_001E0020};

int D_L15_00162254 = 128;

int D_L15_00162258 = 45;

int D_L15_0016225C = 64;

extern float ripple_88_radial_wavelength_gp __asm__("D_L15_00162260") __attribute__((sda));

#include "rnc/overlay/l15/mobys/radial_ripple_mesh_data/draw_scaled_88_vertex_moby_radial_ripple_mesh.h"

extern float ripple_88_z_amplitude_gp __asm__("D_L15_00162268") __attribute__((sda));

extern float ripple_88_uv_scroll_rate_gp __asm__("D_L15_0016226C") __attribute__((sda));

extern float ripple_88_template_vertex_scale_gp __asm__("D_L15_00162270") __attribute__((sda));

void draw_scaled_88_vertex_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002edfc0");

void draw_scaled_88_vertex_moby_radial_ripple_mesh(struct Moby *moby) {
    RingVars_2edfc0 *data = (RingVars_2edfc0 *)moby->pvars;
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
    vu1_add_g_sregister(6, get_effect_texture(D_L15_00162258));
    vu1_add_g_sregister(0x42, ((long)D_L15_00162254 << 32) | 0x44);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle =
        fast_add_rotations(data->angle,
                           360.0f / FUN_001f96b0_2edfc0(ripple_88_phase_advance_divisor_gp) *
                               0.017453292f * frame_time_macro);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L15_0016225C << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 88; i++) {
        float *template_vertex = D_L15_001DF100[i];
        float  x = template_vertex[0];
        float  y = template_vertex[1];
        float  rad = square_root(x * x + y * y);
        float  radius_remainder = FUN_L00_00200260_2edfc0(rad, ripple_88_radial_wavelength_gp);
        float  w =
            fast_normalize_angle(radius_remainder * 6.2831855f / ripple_88_radial_wavelength_gp);
        float z = fast_sin(fast_add_rotations(w, data->angle));
        D_L15_001DF940[i][0] =
            ripple_88_template_vertex_scale_gp * template_vertex[0] + moby->pos.x;
        D_L15_001DF940[i][1] =
            ripple_88_template_vertex_scale_gp * template_vertex[1] + moby->pos.y;
        D_L15_001DF940[i][2] = ripple_88_template_vertex_scale_gp * template_vertex[2] +
                               moby->pos.z + ripple_88_z_amplitude_gp * z;
        D_L15_001E0020[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 88; uv_vertex_index++) {
            Quad4_2edfc0 uv_scroll_factors = l15_ripple_88_uv_scroll_factors;
            D_L15_001DFD60[uv_vertex_index][0] =
                D_L15_001DF520[uv_vertex_index][0] +
                FUN_L00_00200260_2edfc0(scale * uv_scroll_factors.f[j][0], 1.0f);
            D_L15_001DFD60[uv_vertex_index][1] =
                D_L15_001DF520[uv_vertex_index][1] +
                FUN_L00_00200260_2edfc0(scale * uv_scroll_factors.f[j][1], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edfc0(D_L15_00162220[k],
                                    D_L15_00162240[k],
                                    D_L15_00162250[k],
                                    (void *)D_L15_00162248[k],
                                    1);
        }
    }
}
