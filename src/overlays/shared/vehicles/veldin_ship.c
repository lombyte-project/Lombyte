/* Shared moby class veldin_ship. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/rendering/debug/debug_menu.h"

typedef struct {
    u128 points[4];
    u32  colors[4];
    struct {
        f32 u, v;
    } uv[4];
    u64 texture[4];
} __attribute__((aligned(16))) Quad_2d19b0;

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) Vec_2d19b0;

typedef struct M2d1e80 {
    u8   p0[0x20];
    u8   state;
    u8   p21[0x10];
    u8   vis;
    u8   p32[2];
    u16  flags;
    u8   p36[0x42];
    s32 *vars;
    u8   p7c[0x40];
    u8   bc;
} M2d1e80;

extern Vec_2d19b0 ship_normals[] __asm__("D_L00_001DF9A0");
extern Vec_2d19b0 ship_world_vertices[] __asm__("D_L00_001E04A0");
extern Vec_2d19b0 ship_vertices[] __asm__("D_L00_001DE7B0");
extern f32        ship_reflection_uvs[][2] __asm__("D_L00_001E0B00");
extern f32        ship_previous_uvs[][2] __asm__("D_L00_001E0E30");
extern s16        ship_quad_indices[][4][3] __asm__("D_L00_001DEE10");
extern s64        get_effect_texture(s32) __asm__("func_001F44B8");
extern short      D_L00_001618E8 __attribute__((sda));
extern short      D_L00_001618E0 __attribute__((sda));
extern short      D_L00_001618EC __attribute__((sda));
extern short      D_L00_001618F0 __attribute__((sda));
extern short      D_L00_001618F4 __attribute__((sda));
extern short      D_L00_001618DC __attribute__((sda));
extern short      D_L00_001618E4 __attribute__((sda));
extern void       draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
float             absolute_float(float input) __asm__("func_001F99C0");

extern s32 D_L00_0015F5C4;
extern s32 veldin_ship_visibility_selector[] __asm__("D_L00_0016C890");

#define G_DC           (*(s32 *)&D_L00_001618DC)
#define G_E0           (*(s32 *)&D_L00_001618E0)
#define G_E4           (*(s32 *)&D_L00_001618E4)
#define G_E8           (*(s32 *)&D_L00_001618E8)
#define G_EC           (*(s32 *)&D_L00_001618EC)
#define G_F0           (*(s32 *)&D_L00_001618F0)
#define G_F4           (*(s32 *)&D_L00_001618F4)
#define XFORM(a, b, c) transform_vector((Vec4f *)(a), (Vec4f *)(b), (c))

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D2E60), where it is exact; names translated to the US level program. */

void veldin_ship_draw(struct Moby *moby) __asm__("FUN_L00_002d19b0");

void veldin_ship_draw(struct Moby *moby) {
    Quad_2d19b0 quad;
    Vec_2d19b0  normal, reflection, view;
    u8         *ship_render_state = ((u8 *)moby->pvars);
    s32         i, j, index, active = 0;
    f32         blend, reflection_projection_factor, x, y;

    quad.texture[1] = get_effect_texture(G_F0);
    quad.texture[2] = 0xff9000000260ULL;
    quad.texture[3] = (u64)(s64)G_DC | ((u64)(s64)G_E0 << 2) | ((u64)(s64)G_E4 << 4) |
                      ((u64)(s64)G_E8 << 6) | ((u64)(s64)G_EC << 32);
    quad.texture[0] = 0;
    quad.colors[0] = quad.colors[1] = quad.colors[2] = quad.colors[3] = G_F4;
    if (absolute_float(l00_camera.pos.f[0] - moby->pos.x) < 16.0f &&
        absolute_float(l00_camera.pos.f[1] - moby->pos.y) < 16.0f)
        active = 1;
    if (active || moby->state == 1) {
        moby->command = 1;
        fast_dec_timer((s32 *)(ship_render_state + 0x40));
        blend = convert_integer_to_float(*(s32 *)(ship_render_state + 0x40));
        blend /= convert_integer_to_float(scale_game_frames(0x3c));
        for (i = 0; i < 0x66; i++) {
            XFORM(&ship_world_vertices[i], &ship_vertices[i], ship_render_state);
            fast_vec_sub(&view, &ship_world_vertices[i], g_debug_camera.position);
            fast_vec_normalize(&view, &view, 1.0f);
            XFORM(&normal, &ship_normals[i], ship_render_state);
            fast_vec_normalize(&normal, &normal, 0.1f);
            reflection_projection_factor = fast_vec_dot(&normal, &view);
            fast_vec_scale(
                &reflection, &normal, reflection_projection_factor + reflection_projection_factor);
            fast_vec_sub(&reflection, &view, &reflection);
            fast_vec_normalize(&reflection, &reflection, 1.0f);
            reflection.z += 1.0f;
            reflection_projection_factor = square_root(reflection.z + reflection.z);
            reflection_projection_factor += reflection_projection_factor;
            if (moby->state == 1 || *(s32 *)(ship_render_state + 0x40) == 0) {
                ship_reflection_uvs[i][0] = reflection.x / reflection_projection_factor + 0.5f;
                ship_reflection_uvs[i][1] = reflection.y / reflection_projection_factor + 0.5f;
            } else {
                x = reflection.x / reflection_projection_factor + 0.5f;
                y = reflection.y / reflection_projection_factor;
                ship_reflection_uvs[i][0] = x + (ship_previous_uvs[i][0] - x) * blend;
                x = y + 0.5f;
                ship_reflection_uvs[i][1] = x + (ship_previous_uvs[i][1] - x) * blend;
            }
        }
        if (moby->state == 1)
            moby->state = 2;
    } else {
        if (moby->command == 1) {
            moby->command = 0;
            for (i = 0; i < 0x66; i++) {
                ship_previous_uvs[i][0] = ship_reflection_uvs[i][0];
                ship_previous_uvs[i][1] = ship_reflection_uvs[i][1];
                XFORM(&ship_world_vertices[i], &ship_vertices[i], ship_render_state);
            }
        } else {
            for (i = 0; i < 0x66; i++)
                XFORM(&ship_world_vertices[i], &ship_vertices[i], ship_render_state);
        }
        *(s32 *)(ship_render_state + 0x40) = scale_game_frames(0x3c);
    }
    for (i = 0; i < 0x4a; i++) {
        for (j = 0; j < 4; j++) {
            index = ship_quad_indices[i][j][0];
            qcopy(&quad.points[j], &ship_world_vertices[index]);
            quad.uv[j].u = ship_reflection_uvs[index][0];
            quad.uv[j].v = ship_reflection_uvs[index][1];
        }
        draw_geometry_quad(&quad, 0, 0);
    }
}

void veldin_ship_update(M2d1e80 *m) __asm__("FUN_L00_002d1e80");

/* Moby class 530 */
void veldin_ship_update(M2d1e80 *moby) {
    s32 *ship_vars = moby->vars;
    if (moby->state == 0) {
        moby->state = 1;
        moby->bc = 1;
        ship_vars[0x10] = 0;
    }
    if (D_L00_0015F5C4 == 2 &&
        (veldin_ship_visibility_selector[0] < 3 || veldin_ship_visibility_selector[0] == 4)) {
        moby->vis = 0;
        moby->flags |= 1;
    } else {
        moby->vis = 1;
        moby->flags &= 0xFFFE;
    }
    build_moby_bone_transform(moby, 0, ship_vars);
    if (moby->vis)
        add_draw_callback(veldin_ship_draw, moby);
}

short                D_L01_00161980 = 1;
short                D_L01_00161978 = 1;
short                D_L01_00161984 = 0x80;
short                D_L01_00161988 = 0x15;
short                D_L01_0016198C = 0x7060;
short                D_L01_00161974 = 0;
short                D_L01_0016197C = 0;
short D_L09_001B09B0 NOT_SDA = 0;
short D_L10_001B0CB0 NOT_SDA = 0;
short D_L11_001B1230 NOT_SDA = 0;
short D_L12_001B0CB0 NOT_SDA = 0;
short D_L15_001B0E30 NOT_SDA = 0;
short D_L16_001B0CB0 NOT_SDA = 0;
short D_L17_001B1130 NOT_SDA = 0;
short D_L18_001B1230 NOT_SDA = 0;
