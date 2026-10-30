/* Level 17 moby class fleet_backdrop (0x6ec): animates and draws the Drek's Fleet backdrop quads around the hero's ship. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "rnc/overlay/l17/mobys/fleet_backdrop_globals.h"

/* Draws the Drek's Fleet reflection/backdrop quads around a moby. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F4E30), where it is exact; names translated to the US level program. */

typedef struct {
    OvlQuad m[4];
    int     col[4];
    float   uv[8];
    int     q[8];
} Pkt;

extern char  D_L17_001676C0[];
extern float frame_delta_time __asm__("D_0015ED7C");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
float        D_L17_001623C0[4] __attribute__((section(".sdata"))) = {-0.5f, -1.0f, 1.0f, 1.0f};

extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

extern float D_L17_0016D3E4 NOT_SDA;
extern float                backdrop_quad_side_lengths[] __asm__("D_L17_001DDFF8");
extern float                backdrop_quad_color_blend_factors[] __asm__("D_L17_001DE040");
extern float                D_L17_001DE0A0[];
extern float                backdrop_scrolling_quad_start_half_widths[] __asm__("D_L17_001DE0B8");
extern float                backdrop_quad_uv_rotation_angles[] __asm__("D_L17_001DE0D0");
extern float                backdrop_scrolling_quad_base_uv_coordinates[] __asm__("D_L17_001E6FA0");
extern int                  backdrop_quad_color_blend_source_colors[] __asm__("D_L17_001DE010");
extern int                  backdrop_quad_color_blend_target_colors[] __asm__("D_L17_001DE028");
extern int                  backdrop_scrolling_quad_texture_indices[] __asm__("D_L17_001DE088");
int                         D_L17_001623B8 = 0x807e4931;
void draw_fleet_backdrop_quads(struct Moby *moby) __asm__("FUN_L17_002f32f0");

void draw_fleet_backdrop_quads(struct Moby *moby) {
    Pkt      pkt;
    OvlQuad  vv[4];
    char    *data = (char *)moby->pvars;
    char    *backdrop_endpoint_position;
    int      i;
    int      k;
    int      j;
    OvlQuad *vb;
    float   *pu;
    float   *quad_vertex_v_coordinates;
    vu1_add_g_sregister(0x47, 0x513F1);
    vb = &vv[2];
    backdrop_endpoint_position = data + 0x10;
    pu = &pkt.uv[0];
    quad_vertex_v_coordinates = &pkt.uv[1];
    D_L17_0016D3E4 = 8192000.0f;
    *(float *)(data + 0x14) = -8000.0f;
    for (i = 0; i < 2; i++) {
        fast_vec_sub(vb, data, backdrop_endpoint_position);
        fast_vec_sub(&vv[0], data, D_L17_001676C0);
        fast_vec_sub(&vv[1], backdrop_endpoint_position, D_L17_001676C0);
        fast_vec_cross(&vv[0], &vv[0], vb);
        fast_vec_normalize(&vv[0], &vv[0], backdrop_scrolling_quad_start_half_widths[i]);
        fast_vec_cross(&vv[1], &vv[1], vb);
        fast_vec_normalize(&vv[1], &vv[1], *(float *)(data + 0x28));
        *(long *)&pkt.q[4] = 0x0000FF9000000260LL;
        *(long *)&pkt.q[6] = 0x0000008000000048LL;
        *(long *)&pkt.q[0] = 0;
        pkt.col[3] = D_L17_001623B8;
        pkt.col[2] = D_L17_001623B8;
        pkt.col[1] = D_L17_001623B8;
        pkt.col[0] = D_L17_001623B8;
        {
            float *qu = &pkt.uv[0];
            float *qv = &pkt.uv[1];
            int    m;
            for (m = 0; m < 4; m++) {
                qu[m * 2] = backdrop_scrolling_quad_base_uv_coordinates[m * 2];
                qv[m * 2] = backdrop_scrolling_quad_base_uv_coordinates[m * 2 + 1];
            }
        }
        fast_vec_sub(&vv[3], data, &vv[0]);
        pkt.m[0] = vv[3];
        fast_vec_add(&vv[3], data, &vv[0]);
        pkt.m[1] = vv[3];
        fast_vec_sub(&vv[3], backdrop_endpoint_position, &vv[1]);
        pkt.m[2] = vv[3];
        fast_vec_add(&vv[3], backdrop_endpoint_position, &vv[1]);
        pkt.m[3] = vv[3];
        *(long *)&pkt.q[2] = get_effect_texture(backdrop_scrolling_quad_texture_indices[i]);
        for (j = 0; j < 4; j++) {
            pu[j * 2] = backdrop_scrolling_quad_base_uv_coordinates[j * 2] + D_L17_001DE0A0[i];
            quad_vertex_v_coordinates[j * 2] =
                backdrop_scrolling_quad_base_uv_coordinates[j * 2 + 1];
        }
        D_L17_001DE0A0[i] += D_L17_001623C0[i] * frame_delta_time;
        draw_geometry_quad(&pkt, 0, 0);
    }
    *(long *)&pkt.q[2] = get_effect_texture(0x2F);
    for (k = 0; k < 5; k++) {
        float quad_side_length = backdrop_quad_side_lengths[k];
        float uv_rotation_angle = backdrop_quad_uv_rotation_angles[k];
        float x = *(float *)(data + 0x10) - quad_side_length * 0.5f;
        float z;
        ((float *)&pkt)[0] = x;
        ((float *)&pkt)[1] = *(float *)(data + 0x14) + 1000.0f;
        z = *(float *)(data + 0x18) + quad_side_length * 0.5f;
        ((float *)&pkt)[3] = 1.0f;
        ((float *)&pkt)[2] = z;
        pkt.m[3] = pkt.m[0];
        pkt.m[2] = pkt.m[0];
        pkt.m[1] = pkt.m[0];
        ((float *)&pkt)[0] = x + 0.0f;
        ((float *)&pkt)[2] = z + 0.0f;
        ((float *)&pkt)[4] = ((float *)&pkt)[4] + 0.0f;
        ((float *)&pkt)[6] = ((float *)&pkt)[6] - quad_side_length;
        ((float *)&pkt)[8] = ((float *)&pkt)[8] + quad_side_length;
        ((float *)&pkt)[10] = ((float *)&pkt)[10] + 0.0f;
        ((float *)&pkt)[12] = ((float *)&pkt)[12] + quad_side_length;
        ((float *)&pkt)[14] = ((float *)&pkt)[14] - quad_side_length;
        pkt.uv[0] = fast_sin(fast_add_rotations(uv_rotation_angle, 4.712389f)) * 0.5f + 0.5f;
        pkt.uv[1] = fast_cos(fast_add_rotations(uv_rotation_angle, 4.712389f)) * 0.5f + 0.5f;
        pkt.uv[2] = fast_sin(fast_add_rotations(uv_rotation_angle, 3.1415927f)) * 0.5f + 0.5f;
        pkt.uv[3] = fast_cos(fast_add_rotations(uv_rotation_angle, 3.1415927f)) * 0.5f + 0.5f;
        pkt.uv[4] = fast_sin(uv_rotation_angle) * 0.5f + 0.5f;
        pkt.uv[5] = fast_cos(uv_rotation_angle) * 0.5f + 0.5f;
        pkt.uv[6] = fast_sin(fast_add_rotations(uv_rotation_angle, 1.5707964f)) * 0.5f + 0.5f;
        pkt.uv[7] = fast_cos(fast_add_rotations(uv_rotation_angle, 1.5707964f)) * 0.5f + 0.5f;
        {
            int interpolated_vertex_color =
                fast_tween_color(backdrop_quad_color_blend_source_colors[k],
                                 backdrop_quad_color_blend_target_colors[k],
                                 backdrop_quad_color_blend_factors[k]);
            pkt.col[3] = interpolated_vertex_color;
            pkt.col[2] = interpolated_vertex_color;
            pkt.col[1] = interpolated_vertex_color;
            pkt.col[0] = interpolated_vertex_color;
        }
        draw_geometry_quad(&pkt, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F5388), where it is exact; names translated to the US level program. */

extern float backdrop_shared_radius_numerator_gp __asm__("D_L17_001623D0") __attribute__((sda));
extern float backdrop_offset_radius_divisor_gp __asm__("D_L17_001623D8") __attribute__((sda));

extern float backdrop_quad_uv_rotation_rate_factors[] __asm__("D_L17_001DDFE0");
extern float backdrop_quad_color_blend_direction_multipliers[] __asm__("D_L17_001DE058");
extern float backdrop_quad_color_blend_rates[] __asm__("D_L17_001DE070");
extern float backdrop_heading_radius_numerator_addend_gp __asm__("D_L17_001623D4")
    __attribute__((sda));
void update_fleet_backdrop_animation(struct Moby *moby) __asm__("FUN_L17_002f3848");

void update_fleet_backdrop_animation(struct Moby *moby) {
    char *moby_pvars = (char *)moby->pvars;
    float offset_accumulator[4];
    float pvar_angle_offset[4];
    float vector_sum[4];
    int   i;
    if (*(int *)(moby_pvars + 0x4C) != -1) {
        struct Hero *hero_data = &hero;
        if (hero_data->state.current != 0x32 || ((char *)hero_data->ship_moby) == 0 ||
            *(short *)(((char *)hero_data->ship_moby) + 0xA6) != 0x563) {
            if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(moby_pvars + 0x4C))) {
                D_L17_0015F638 = 150.0f;
                goto done;
            }
        }
        D_L17_0015F638 = 10.0f;
    }
done:
    if (moby->state == 0) {
        moby->state = 1;
        for (i = 0; i < 5; i++) {
            backdrop_quad_uv_rotation_angles[i] = (float)i * 25.0f;
        }
        qcopy(moby_pvars, &moby->pos);
        build_spherical_offset(
            offset_accumulator,
            (backdrop_heading_radius_numerator_addend_gp + backdrop_shared_radius_numerator_gp) /
                backdrop_offset_radius_divisor_gp,
            moby->rot.z,
            0.0f);
        build_spherical_offset(pvar_angle_offset,
                               backdrop_shared_radius_numerator_gp /
                                   backdrop_offset_radius_divisor_gp,
                               *(float *)(moby_pvars + 0x44),
                               0.0f);
        fast_vec_add(vector_sum, offset_accumulator, pvar_angle_offset);
        *(OvlQuad *)offset_accumulator = *(OvlQuad *)vector_sum;
        fast_vec_add(vector_sum, moby_pvars, offset_accumulator);
        *(OvlQuad *)(moby_pvars + 0x10) = *(OvlQuad *)vector_sum;
        moby->update_dist = 0xFF;
        *(float *)(moby_pvars + 0x10) -= 300.0f;
    } else {
        for (i = 0; i < 5; i++) {
            backdrop_quad_color_blend_factors[i] +=
                backdrop_quad_color_blend_rates[i] *
                backdrop_quad_color_blend_direction_multipliers[i] * frame_delta_time;
            if (1.0f < backdrop_quad_color_blend_factors[i]) {
                backdrop_quad_color_blend_factors[i] = 1.0f;
                backdrop_quad_color_blend_direction_multipliers[i] = -1.0f;
            }
            if (backdrop_quad_color_blend_factors[i] < 0.0f) {
                backdrop_quad_color_blend_factors[i] = 0.0f;
                backdrop_quad_color_blend_direction_multipliers[i] = 1.0f;
            }
        }
        for (i = 0; i < 5; i++) {
            backdrop_quad_uv_rotation_angles[i] = fast_add_rotations(
                backdrop_quad_uv_rotation_angles[i],
                backdrop_quad_uv_rotation_rate_factors[i] * 0.1f * frame_delta_time);
        }
        for (i = 0; i < 2; i++) {
            D_L17_001DE0A0[i] += D_L17_001623C0[i] * frame_delta_time;
            if (1.0f <= D_L17_001DE0A0[i]) {
                D_L17_001DE0A0[i] -= 1.0f;
            }
            if (D_L17_001DE0A0[i] < 0.0f) {
                D_L17_001DE0A0[i] += 1.0f;
            }
        }
        add_draw_callback(draw_fleet_backdrop_quads, moby);
    }
}
