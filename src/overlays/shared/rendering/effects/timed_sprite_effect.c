/* Timed sprite effect (class 0x76A): a screen-space sprite at a point that grows, fades between two colours and expires; its spawner and draw. */
#include "types.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/entities.h"
#include "qcopy.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern u32   fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern void  FUN_L02_0020bc88(void *, void *, void *, s64, s32);
extern s32   D_0013E500[];
extern s32   projected_quad_texture_selector_gp __asm__("D_L06_00162414") __attribute__((sda));
extern s32   projected_quad_base_radius_gp __asm__("D_L06_00162418") __attribute__((sda));
extern s32   projected_quad_top_right_uv_gp __asm__("D_L06_0016241C") __attribute__((sda));
extern s32   projected_quad_top_left_uv_gp __asm__("D_L06_00162420") __attribute__((sda));
extern s32   projected_quad_bottom_right_uv_gp __asm__("D_L06_00162424") __attribute__((sda));
extern s32   projected_quad_bottom_left_uv_gp __asm__("D_L06_00162428") __attribute__((sda));

/* UpdateMoby: state 0 deletes, state 1 draws and times out */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_0030D2A0), where it is exact; names translated to the US level program. */

void draw_projected_fading_quad(char *moby) __asm__("FUN_L06_0030bfa8");

void update_timed_sprite_effect(struct Moby *moby) __asm__("FUN_L06_0030be60");

void update_timed_sprite_effect(struct Moby *moby) {
    enum {
        SPRITE_EFFECT_STATE_REMOVE = 0,
        SPRITE_EFFECT_STATE_ANIMATE = 1
    };
    float *effect_data = ((float *)moby->pvars);
    switch ((unsigned char)((char)moby->state)) {
    case SPRITE_EFFECT_STATE_REMOVE:
        mark_moby_for_removal(moby);
        break;
    case SPRITE_EFFECT_STATE_ANIMATE:
        add_draw_callback(draw_projected_fading_quad, moby);
        effect_data[0] = effect_data[0] + effect_data[1];
        if (2.0f < effect_data[0]) {
            mark_moby_for_removal(moby);
        }
        break;
    }
}

void spawn_timed_projected_sprite_effect(char *world_position,
                                         int   duration_updates,
                                         int   start_color,
                                         int   end_color) __asm__("FUN_L06_0030bef8");

void spawn_timed_projected_sprite_effect(char *world_position,
                                         int   duration_updates,
                                         int   start_color,

                                         int end_color) {
    OvlQuad        position_copy;
    char          *vp = (char *)&position_copy;
    unsigned char *effect_moby;
    position_copy = *(OvlQuad *)world_position;
    effect_moby = (unsigned char *)create_moby(0x76A);
    if (effect_moby) {
        char *effect_data = ((char *)MOBY(effect_moby)->pvars);
        float duration_as_float;
        qcopy(effect_moby + 0x10, vp);
        duration_as_float = convert_integer_to_float(duration_updates);
        *(int *)(effect_data + 0xC) = end_color;
        *(int *)(effect_data + 0x8) = start_color;
        *(float *)(effect_data + 0x4) = 2.0f / duration_as_float;
        effect_moby[0x30] = 0xFF;
        effect_moby[0x20] = 1;
    }
}

/* Draws a screen-space sprite at the moby, grown and faded in by the first data float. */
void draw_projected_fading_quad(char *moby) {
    float  *effect_data = ((float *)MOBY(moby)->pvars);
    OvlVec4 world_position;
    s64     corners[4];
    u32     colors[4];
    s32     uvs[4];
    float   screen[4];
    float   size;
    float   fade;
    s32     y, x, radius;
    s32     raw_x;
    u64     texture;
    u32     color;
    s32    *screen_center;
    float   screen_offset;

    qcopy(&world_position, moby + 0x10);
    size = effect_data[0];
    if (1.0f < size)
        size = 1.0f;
    fade = effect_data[0] - 1.0f;
    if (1.0f < fade)
        fade = 1.0f;
    else if (fade < 0.0f)
        fade = 0.0f;
    vu1_add_g_sregister(0x47, 0x5380B);
    project_to_screen(screen, &world_position);
    screen_center = D_0013E500;
    raw_x = truncate_float_to_s32(screen[0] - (float)screen_center[4]);
    screen_offset = (float)screen_center[4];
    y = truncate_float_to_s32(screen[1] - screen_offset);
    x = raw_x - 0x1000;
    y -= 0x1000;
    uvs[0] = projected_quad_top_right_uv_gp;
    uvs[1] = projected_quad_top_left_uv_gp;
    uvs[2] = projected_quad_bottom_right_uv_gp;
    uvs[3] = projected_quad_bottom_left_uv_gp;
    color = fast_tween_color(fade, *(s32 *)&effect_data[2], *(s32 *)&effect_data[3]);
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    radius = truncate_float_to_s32((float)(projected_quad_base_radius_gp << 4) * size);
    corners[3] = 0xfffff080008000LL;
    corners[2] = 0xfffff080008000LL;
    corners[1] = 0xfffff080008000LL;
    corners[0] = 0xfffff080008000LL;
    corners[0] += (y - radius) << 16;
    corners[0] += x + radius;
    corners[1] += (y - radius) << 16;
    corners[1] += x - radius;
    corners[2] += (y + radius) << 16;
    corners[2] += x + radius;
    corners[3] += (y + radius) << 16;
    corners[3] += x - radius;
    texture = get_effect_texture(projected_quad_texture_selector_gp);
    FUN_L02_0020bc88(corners, uvs, colors, texture, 1);
    vu1_add_g_sregister(0x47, 0x5360B);
}

s32 D_L18_001625D4 = 0xb;
s32 D_L18_001625D8 = 0x320;
s32 D_L18_001625DC = 0;
s32 D_L18_001625E0 = 0x4000000;
s32 D_L18_001625E4 = 0x400;
s32 D_L18_001625E8 = 0x4000400;

s32 projected_quad_texture_selector_gp __asm__("D_L06_00162414") = 0xb;
s32 projected_quad_base_radius_gp __asm__("D_L06_00162418") = 0x320;
s32 projected_quad_top_right_uv_gp __asm__("D_L06_0016241C") = 0;
s32 projected_quad_top_left_uv_gp __asm__("D_L06_00162420") = 0x4000000;
s32 projected_quad_bottom_right_uv_gp __asm__("D_L06_00162424") = 0x400;
s32 projected_quad_bottom_left_uv_gp __asm__("D_L06_00162428") = 0x4000400;
