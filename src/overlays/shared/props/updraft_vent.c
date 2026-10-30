/* Updraft vent (class 0x562): sets the hero's vent updraft flag while it is in the box above it, and draws its animated quad layers. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

#include "qcopy.h"
#include "rnc/overlay/shared/props/updraft_vent_globals.h"

/* UpdateMoby_1378 (names.tsv role). State 0 sets flag bit 0 and goes to state 1. In state 1,
 * when the hero (D_0013E633 + 0xE9D) is within 8.0: takes the vector from the moby to the point
 * hero+0x50, rotates it with the matrix from func_001FA4A0(moby + 0xC0), and if that point lies
 * in the box -5 < x < 5, 0 < z < 8 sets hero byte 0x224B to (y < 0.5), else (y > 0) clears
 * D_0014171B. Then queues func_L10_002EA578. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00299AF0.c: func_L10_002EA3E8), where it is exact; names translated to the US level program. */

extern void draw_moby_animated_quad_layers(unsigned char *m) __asm__("FUN_L10_002e91b8");

void update_hero_flag_from_moby_local_box(unsigned char *moby) __asm__("FUN_L10_002e9028");

void update_hero_flag_from_moby_local_box(unsigned char *moby) {
    enum {
        MOBY_UPDATE_INITIALIZE = 0,
        MOBY_UPDATE_ACTIVE = 1
    };
    float        hero_point_local_offset[4] __attribute__((aligned(16)));
    float        mtx[16] __attribute__((aligned(16)));
    char        *player;
    unsigned int update_state = moby[0x20];
    switch (update_state) {
    case MOBY_UPDATE_INITIALIZE:
        moby[0x20] = 1;
        MOBY(moby)->flags |= 1;
        moby[0x31] = 0;
        break;
    case MOBY_UPDATE_ACTIVE:
        player = (&hero.motion.pos);
        if (fast_vec_dist(moby + 0x10, player) < 8.0f) {
            fast_vec_sub(hero_point_local_offset, player + 0x50, moby + 0x10);
            transpose_rotation_matrix(mtx, moby + 0xC0);
            transform_vector(hero_point_local_offset, hero_point_local_offset, mtx);
            if (-5.0f < hero_point_local_offset[0] && hero_point_local_offset[0] < 5.0f &&
                0.0f < hero_point_local_offset[2] && hero_point_local_offset[2] < 8.0f) {
                if (hero_point_local_offset[1] < 0.5f) {
                    player[0x224B] = 1;
                } else {
                    player[0x224B] = 0;
                }
            } else if (0.0f < hero_point_local_offset[1]) {
                hero.vent_updraft = 0;
            }
        }
        add_draw_callback(draw_moby_animated_quad_layers, moby);
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00299AF0.c: func_L10_002EA578), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
} QuadCorner;

typedef struct {
    QuadCorner corner[4];
    int        color[4];
    float      uv[4][2];
    long       unk70, tex, unk80, alpha;
} TrailQuad;

extern QuadCorner l10_animated_quad_second_base_corners[] __asm__("D_L10_001DA310");
extern QuadCorner l10_animated_quad_first_base_corners[] __asm__("D_L10_001DA350");
extern float      quad_layer_uv_phase_divisors[][2] __asm__("D_L10_001DA270");
extern float      quad_corner_base_uvs[][2] __asm__("D_L10_001DA2F0");

extern int  quad_layer_colors[] __asm__("D_L10_001DA250");
extern int  quad_layer_uv_cosine_selectors[][2] __asm__("D_L10_001DA2B0");
extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern int  quad_layer_count_gp __asm__("D_L10_00161F40") __attribute__((sda));
extern int  quad_layer_alpha_a_selectors[2] __asm__("D_L10_00161F48") __attribute__((sda));
extern int  quad_layer_alpha_b_selectors[2] __asm__("D_L10_00161F50") __attribute__((sda));
extern int  quad_layer_alpha_c_selectors[2] __asm__("D_L10_00161F58") __attribute__((sda));
extern int  quad_layer_alpha_d_selectors[2] __asm__("D_L10_00161F60") __attribute__((sda));
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

#define GS_ALPHA(a, b, k, d, fix)                                                 \
    ((unsigned long)(a) | ((unsigned long)(b) << 2) | ((unsigned long)(k) << 4) | \
     ((unsigned long)(d) << 6) | ((unsigned long)(fix) << 32))

void draw_moby_animated_quad_layers(unsigned char *moby) {
    TrailQuad   first, second;
    float       transform[12] __attribute__((aligned(16)));
    float       position[4] __attribute__((aligned(16)));
    float      *scale;
    int         pass, axis, k, i;
    float      *first_corner, *second_corner, *second_dst;
    QuadCorner *first_src, *second_src;
    TrailQuad  *second_quad;

    scale = ((float *)MOBY(moby)->pvars);
    copy_matrix_rotation(transform, moby + 0xC0);
    qcopy(position, moby + 0x10);
    position[3] = 1.0f;
    first.tex = get_effect_texture(0x2C);
    first.unk80 = 0xFF9000000260L;
    first.unk70 = 0;
    second.tex = get_effect_texture(0x2C);
    second.unk80 = 0xFF9000000260L;
    second.unk70 = 0;
    second.color[3] = 0;
    second.color[1] = 0;
    second_quad = &second;
    second_corner = (float *)second_quad;
    second_dst = (float *)second_quad;
    first_corner = (float *)&first;
    first_src = l10_animated_quad_first_base_corners;
    second_src = l10_animated_quad_second_base_corners;
    for (i = 0; i < 4; i++) {
        qcopy(first_corner, &first_src[i]);
        qcopy(second_dst, second_src);
        first_corner[0] = first_corner[0] * scale[0];
        second_corner[0] = second_corner[0] * scale[0];
        first_corner[2] = first_corner[2] * scale[1];
        second_corner[2] = second_corner[2] * scale[1];
        first_corner += 4;
        second_corner += 4;
        second_src++;
        second_dst += 4;
    }
    for (pass = 0; pass < quad_layer_count_gp; pass++) {
        int parity = pass & 1;
        int shade;
        first.alpha = GS_ALPHA(quad_layer_alpha_a_selectors[parity],
                               quad_layer_alpha_b_selectors[parity],
                               quad_layer_alpha_c_selectors[parity],
                               quad_layer_alpha_d_selectors[parity],
                               0x80);
        second.alpha = GS_ALPHA(quad_layer_alpha_a_selectors[parity],
                                quad_layer_alpha_b_selectors[parity],
                                quad_layer_alpha_c_selectors[parity],
                                quad_layer_alpha_d_selectors[parity],
                                0x80);
        shade = quad_layer_colors[pass];
        second.color[2] = shade;
        second.color[0] = shade;
        first.color[3] = shade;
        first.color[2] = shade;
        first.color[1] = shade;
        first.color[0] = shade;
        for (i = 0; i < 4; i++) {
            for (axis = 0; axis < 2; axis++) {
                int limit =
                    truncate_float_to_s32(quad_layer_uv_phase_divisors[pass][axis] * 360.0f);
                float angle = convert_integer_to_float(quad_uv_animation_phase_input % limit) /
                                  quad_layer_uv_phase_divisors[pass][axis] -
                              180.0f;
                float wave;
                if (quad_layer_uv_cosine_selectors[pass][axis] != 0) {
                    wave = fast_cos(angle * 0.017453292f);
                } else {
                    wave = fast_sin(angle * 0.017453292f);
                }
                first.uv[i][axis] = quad_corner_base_uvs[i][axis] + wave * 0.125f;
            }
        }
        for (k = 0; k < 2; k++) {
            float step = (float)k * 0.125f;
            second.uv[0][k] = first.uv[1][k];
            second.uv[1][k] = first.uv[1][k] + step;
            second.uv[2][k] = first.uv[3][k];
            second.uv[3][k] = first.uv[3][k] + step;
        }
        draw_geometry_quad(&first, transform, 0);
        draw_geometry_quad(second_quad, transform, 0);
        {
            float *second_corner_cursor = (float *)second_quad;
            float *first_corner_cursor = (float *)&first;
            for (i = 0; i < 4; i++) {
                first_corner_cursor[1] += 0.1f;
                second_corner_cursor[1] += 0.1f;
                first_corner_cursor += 4;
                second_corner_cursor += 4;
            }
        }
    }
}

int D_L17_00161FB0 = 6;
int D_L17_00161FB8[2] = {0, 1};

int D_L17_00161FC0[2] = {2, 0};

int D_L17_00161FC8[2] = {2, 2};

int D_L17_00161FD0[2] = {1, 2};
