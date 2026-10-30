/* Level 08 moby class 0x258: layered mesh effect. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/entities.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

#include "sda.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

#include "rnc/overlay/l08/mobys/layered_mesh_effect_globals.h"

/* Draw callback: draws the scrolled mesh layers with effect textures 0x3A to 0x3D, two of them faded by distance. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_002F2288), where it is exact; names translated to the US level program. */

extern int scrolled_mesh_unscaled_fixed_alpha_gp __asm__("D_L08_00161DD0") __attribute__((sda));
extern int offset_mesh_first_pass_fixed_alpha_gp __asm__("D_L08_00161DF0") __attribute__((sda));
extern int pair_offset_mesh_fixed_alpha_gp __asm__("D_L08_00161E00") __attribute__((sda));
extern unsigned char layered_mesh_effect_fade_alpha __asm__("D_L08_00161E08");
extern int           get_effect_texture(int) __asm__("FUN_001f44b8");
extern void          draw_scrolled_mesh_batch(int) __asm__("FUN_L08_002f0b40");
extern void          draw_eleven_entry_scrolled_texture_batch(int) __asm__("FUN_L08_002f0c18");
extern void          emit_pair_offset_render_batch(int) __asm__("FUN_L08_002f0cf0");

void draw_layered_mesh_effect(void) __asm__("FUN_L08_002f0dc8");

void draw_layered_mesh_effect(void) {
    vu1_add_g_sregister(0x42, 0x8000000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    font_queue_vu_state();
    vu1_add_g_sregister(6, get_effect_texture(0x3A));
    vu1_add_g_sregister(0x42, (long)scrolled_mesh_unscaled_fixed_alpha_gp << 32 | 0x44);
    draw_scrolled_mesh_batch(0);
    vu1_add_g_sregister(6, get_effect_texture(0x3B));
    vu1_add_g_sregister(
        0x42,
        (long)((scrolled_mesh_subtractive_base_fixed_alpha * layered_mesh_effect_fade_alpha) >> 8)
                << 32 |
            0x62);
    draw_scrolled_mesh_batch(1);
    vu1_add_g_sregister(
        0x42,
        (long)((scrolled_mesh_additive_base_fixed_alpha * layered_mesh_effect_fade_alpha) >> 8)
                << 32 |
            0x68);
    draw_scrolled_mesh_batch(2);
    vu1_add_g_sregister(6, get_effect_texture(0x3C));
    vu1_add_g_sregister(0x42, (long)offset_mesh_first_pass_fixed_alpha_gp << 32 | 0x48);
    draw_eleven_entry_scrolled_texture_batch(0);
    vu1_add_g_sregister(0x42, (long)offset_mesh_second_pass_fixed_alpha << 32 | 0x48);
    draw_eleven_entry_scrolled_texture_batch(1);
    vu1_add_g_sregister(6, get_effect_texture(0x3D));
    vu1_add_g_sregister(0x42, (long)pair_offset_mesh_fixed_alpha_gp << 32 | 0x48);
    emit_pair_offset_render_batch(0);
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030AC70), where it is exact; names translated to the US level program. */

extern Pair  D_L08_00161E10[] MACRO_ADDR;
extern Pair  D_L08_00161E14[] MACRO_ADDR;
extern Pair  D_L08_00161E24[] MACRO_ADDR;
extern Pair  D_L08_001E8348[];
extern Pair  l08_layered_mesh_effect_sine_phase_pair __asm__("D_L08_00161E28");
extern char  D_L08_001675C0[];
extern s32   D_L08_001DDF40[];
extern s32   D_L08_001E2828[];
extern s32   D_L08_001E2858[];
extern s32   D_L08_001E8310[];
extern char  layered_mesh_effect_sine_output_pair[] __asm__("D_L08_001E8350");
extern int   D_L08_00161E08;
extern short layered_mesh_effect_center_x __asm__("D_L08_00161DC0") __attribute__((sda));
extern short layered_mesh_rgba_tables_initialized __asm__("D_L08_00161E04") __attribute__((sda));
extern s32   D_L08_001DDF20[];
extern s32   D_L08_001DB518[];
extern void  draw_layered_mesh_effect(void) __asm__("FUN_L08_002f0dc8");
extern void
scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");
extern void
advance_wrapped_sine_pair(void *, float, float, float, void *) __asm__("FUN_L08_002f1158");
extern void advance_scroll_offsets(int) __asm__("FUN_L08_002f12a0");
extern void advance_scroll_entry(int) __asm__("FUN_L08_002f1378");

extern Pair D_L08_00161E20[] MACRO_ADDR;
void        update_layered_mesh_effect(struct Moby *moby) __asm__("FUN_L08_002f1548");

void update_layered_mesh_effect(struct Moby *moby) {
    enum {
        DRAW_ELEMENTS_INITIALIZE = 0,
        DRAW_ELEMENTS_UPDATE = 1
    };
    int   i;
    float culling_sphere[4];
    float distance_vector[4];
    float distance;

    switch (moby->state) {
    case DRAW_ELEMENTS_INITIALIZE:
        if (*(int *)&layered_mesh_rgba_tables_initialized == 0) {
            scale_rgba_tables(D_L08_001E8310, 0xD, D_L08_001E2858, 0.5f, 0.5f, 0.65f, 0.5f, 1);
            scale_rgba_tables(D_L08_001DDF20, 8, D_L08_001DB518, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            scale_rgba_tables(D_L08_001E2828, 0xB, D_L08_001DDF40, 0.25f, 0.25f, 0.25f, 1.0f, 1);
            *(int *)&layered_mesh_rgba_tables_initialized = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L08_001E8348[i].a = 0;
            D_L08_001E8348[i].b = 0;
            l08_layered_mesh_effect_sine_phase_pair.a = 0;
            l08_layered_mesh_effect_sine_phase_pair.b = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L08_00161E10[i].a = 0;
            *(float *)&D_L08_00161E14[i].a = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L08_00161E20[i].a = 0;
            *(float *)&D_L08_00161E24[i].a = i * 0.4f;
        }
        break;
    case DRAW_ELEMENTS_UPDATE:
        culling_sphere[0] = *(float *)&layered_mesh_effect_center_x;
        culling_sphere[1] = layered_mesh_effect_center_y;
        culling_sphere[2] = layered_mesh_effect_center_z;
        culling_sphere[3] = layered_mesh_effect_culling_radius;
        if (fast_bsphere_check((char *)culling_sphere, 1000.0f) >= 0) {
            distance_vector[0] = *(float *)&layered_mesh_effect_center_x;
            distance_vector[1] = layered_mesh_effect_center_y;
            distance_vector[2] = layered_mesh_effect_center_z;
            distance_vector[3] = 1.0f;
            fast_vec_sub(distance_vector, D_L08_001675C0, distance_vector);
            distance = fast_vec_length(distance_vector);
            if (distance <= 84.0f) {
                advance_scroll_offsets(0);
                advance_wrapped_sine_pair(layered_mesh_effect_sine_output_pair,
                                          0.52f,
                                          0.62f,
                                          0.06f,
                                          (void *)&l08_layered_mesh_effect_sine_phase_pair);
                advance_scroll_offsets(2);
                if (distance < 20.0f) {
                    *(unsigned char *)&D_L08_00161E08 = 0xFF;
                } else if (distance < 84.0f) {
                    *(char *)&D_L08_00161E08 =
                        truncate_float_to_s32((1.0f - (distance - 20.0f) * 0.015625f) * 255.0f);
                } else {
                    *(char *)&D_L08_00161E08 = 0;
                }
            }
            advance_scroll_entry(0);
            advance_scroll_entry(0);
            advance_scroll_entry(1);
            add_draw_callback(draw_layered_mesh_effect, moby);
        }
        break;
    }
}

Pair l08_layered_mesh_effect_sine_phase_pair __asm__("D_L08_00161E28") = {0};

unsigned char layered_mesh_effect_fade_alpha __asm__("D_L08_00161E08") = 0;
