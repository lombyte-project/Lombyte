/* Shared moby class lamp: wobbles with random parameters and draws its wobble quads. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"

typedef struct {
    float vertex[4][4];
    int   color[4];
    float texcoord[4][2];
    u64   control[4];
} Quad;

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

extern void FUN_L00_001fff28(void *, int, float);
void        draw_moby_wobble_quads(struct Moby *moby) __asm__("FUN_L05_0030c220");

extern void draw_moby_wobble_quads(struct Moby *m) __asm__("FUN_L05_0030c220");

void moby_wobble_update(unsigned char *moby) __asm__("FUN_L05_0030c0a8");

extern void  draw_geometry_quad(void *, float *, int) __asm__("FUN_001f7d30");
extern float wobble_quad_texcoords[][2] __asm__("D_L05_001D67B0");
extern float wobble_quad_a_base_vertices[][4] __asm__("D_L05_001D67D0");
extern float wobble_quad_b_base_vertices[][4] __asm__("D_L05_001D6810");
extern float D_L05_001671C0_30c220[] __asm__("D_L05_001671C0");

int        wobble_quad_blend_a_selector_gp __asm__("D_L05_00161CF4") = 0;
int        wobble_quad_blend_b_selector_gp __asm__("D_L05_00161CF8") = 2;
int        wobble_quad_blend_c_selector_gp __asm__("D_L05_00161CFC") = 0;
int        wobble_quad_blend_d_selector_gp __asm__("D_L05_00161D00") = 1;
int        wobble_quad_start_color_gp __asm__("D_L05_00161D04") = 0x40807070; /* RGBA, 0xAABBGGRR */
extern int wobble_quad_end_color_gp __asm__("D_L05_00161D08") __attribute__((sda));
extern int wobble_quad_texture_index_gp __asm__("D_L05_00161D0C") __attribute__((sda));

/* UpdateMoby_823: pick random wobble parameters, then animate them each frame */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0030D500), where it is exact; names translated to the US level program. */

void moby_wobble_update(unsigned char *m) __asm__("FUN_L05_0030c0a8");

/* Moby class 823 */
void moby_wobble_update(unsigned char *moby) {
    enum {
        MOBY_WOBBLE_STATE_INITIALIZE = 0,
        MOBY_WOBBLE_STATE_ANIMATE = 1
    };
    char *wobble_vars = ((char *)MOBY(moby)->pvars);
    float random_angular_sample, lo, hi;
    switch (moby[0x20]) {
    case MOBY_WOBBLE_STATE_INITIALIZE:
        *(float *)(wobble_vars + 0x40) = MOBY(moby)->rot.z;
        *(float *)(wobble_vars + 0x48) = random_angle_radians();
        random_angular_sample = random_angle_radians();
        lo = frame_time * 0.47123888f;
        hi = frame_time * 0.5235988f;
        *(float *)(wobble_vars + 0x44) = random_angular_sample;
        random_angular_sample = randf_sym(lo, hi);
        lo = frame_time * 0.20943951f;
        hi = frame_time * 0.2617994f;
        *(float *)(wobble_vars + 0x50) = random_angular_sample;
        *(float *)(wobble_vars + 0x4C) = randf_sym(lo, hi);
        set_moby_color(moby, 0x80, 0x80, 0x80);
        attach_manipulator((char *)moby, 0, wobble_vars);
        moby[0x20] = 1;
        break;
    case MOBY_WOBBLE_STATE_ANIMATE: {
        float k = 0.5235988f;
        float wobble_phase;
        add_draw_callback((void *)draw_moby_wobble_quads, moby);
        wobble_phase =
            fast_add_rotations(*(float *)(wobble_vars + 0x44), *(float *)(wobble_vars + 0x4C));
        *(float *)(wobble_vars + 0x44) = wobble_phase;
        MOBY(moby)->rot.z =
            fast_add_rotations(*(float *)(wobble_vars + 0x40), fast_sin(wobble_phase) * k);
        wobble_phase =
            fast_add_rotations(*(float *)(wobble_vars + 0x48), *(float *)(wobble_vars + 0x50));
        *(float *)(wobble_vars + 0x48) = wobble_phase;
        FUN_L00_001fff28(wobble_vars + 0x10, 1, fast_sin(wobble_phase) * k + k);
        break;
    }
    }
}

void draw_moby_wobble_quads(struct Moby *moby) {
    Quad  quad_a, quad_b;
    float rotation[4];
    float position[4];
    /* Draw helper reads four consecutive vectors: side, direction, up, position. */
    float  matrix[16] __attribute__((aligned(16)));
    float *side;
    float *direction;
    float *up;
    u64    texture;
    u64    flags;
    float  phase;
    float  sign;
    float  pass;
    float  offset_a_x, offset_a_z, offset_b_x, offset_b_z;
    int    i, j, k;
    float *vertex_a, *vertex_b;
    char  *moby_wobble_vars = (char *)moby->pvars;

    moby_point_position(moby, 0, position);
    clear_u64_value(rotation);
    rotation[2] = moby->rot.z;
    rotation[1] = fast_sin(*(float *)(moby_wobble_vars + 0x48)) * 0.5235988f + 0.5235988f;
    side = matrix;
    euler_rotation_matrix(side, rotation);
    direction = matrix + 4;
    fast_vec_sub(direction, D_L05_001671C0_30c220, position);
    fast_vec_normalize(direction, direction, 1.0f);
    up = matrix + 8;
    fast_vec_cross(side, direction, up);
    fast_vec_cross(direction, up, side);
    qcopy(matrix + 12, position);

    texture = get_effect_texture(wobble_quad_texture_index_gp);
    flags = (u64)(s64)wobble_quad_blend_a_selector_gp |
            ((u64)(s64)wobble_quad_blend_b_selector_gp << 2) |
            ((u64)(s64)wobble_quad_blend_c_selector_gp << 4) |
            ((u64)(s64)wobble_quad_blend_d_selector_gp << 6) | 0x8000000000ULL;
    quad_b.control[0] = 0;
    quad_b.control[2] = 0xff9000000260ULL;
    quad_a.control[2] = 0xff9000000260ULL;
    quad_b.control[1] = texture;
    quad_a.control[1] = texture;
    quad_a.control[0] = 0;
    quad_b.control[3] = flags;
    quad_a.control[3] = flags;
    for (k = 0; k < 4; k++) {
        float u = wobble_quad_texcoords[k][0];
        float v = wobble_quad_texcoords[k][1];
        quad_a.texcoord[k][0] = u;
        quad_a.texcoord[k][1] = v;
        quad_b.texcoord[k][0] = u;
        quad_b.texcoord[k][1] = v;
    }
    for (i = 0; i < 10; i++) {
        pass = (float)i;
        j = 0;
        do {
            vertex_a = quad_a.vertex[j];
            qcopy(vertex_a, wobble_quad_a_base_vertices[j]);
            qcopy(quad_b.vertex[j], wobble_quad_b_base_vertices[j]);
            phase = (float)(i + (j & 1)) * 0.1f;
            sign = 1.0f;
            if (j > 1)
                sign = -1.0f;
            quad_a.color[j] = quad_b.color[j] =
                fast_tween_color(wobble_quad_start_color_gp, wobble_quad_end_color_gp, phase);
            offset_a_x = phase * 8.0f * sign;
            offset_a_z = pass * 10.0f;
            offset_b_x = phase * 6.0f * sign;
            offset_b_z = pass * 12.0f;
            vertex_a[0] += offset_a_x;
            vertex_a[2] += offset_a_z;
            vertex_b = quad_b.vertex[j];
            vertex_b[0] += offset_b_x;
            vertex_b[2] += offset_b_z;
        } while (++j < 4);
        draw_geometry_quad(&quad_a, matrix, 0);
        draw_geometry_quad(&quad_b, matrix, 0);
    }
}

int D_L07_00161A98 = 0;
int D_L07_00161A9C = 2;
int D_L07_00161AA0 = 0;
int D_L07_00161AA4 = 1;
int D_L07_00161AA8 = 0x40807070;
int D_L07_00161AAC = 0x804040;
int D_L07_00161AB0 = 0x13;

int wobble_quad_end_color_gp __asm__("D_L05_00161D08") = 0x804040;

int wobble_quad_texture_index_gp __asm__("D_L05_00161D0C") = 0x13;
