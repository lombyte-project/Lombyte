/* Level 11 moby class distance_faded_scroll_effect (class 1919): queues and draws its distance-faded scrolling layer batches, with the two pair-offset batch emitters it calls. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/l11/mobys/distance_faded_scroll_effect_globals.h"

extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void queue_oil_tanker_front_texture_2b_batches(int) __asm__("FUN_L11_0031e858");
void emit_oil_tanker_render_batches_with_pair_offset(int pair_index) __asm__("FUN_L11_0031e780");
extern void
scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");
extern void
advance_wrapped_sine_pair(void *, float, float, float, void *) __asm__("FUN_L08_002f1158");
extern void advance_scroll_offsets(int) __asm__("FUN_L08_002f12a0");
extern void advance_scroll_entry(int) __asm__("FUN_L08_002f1378");

typedef struct {
    int a, b;
} PairB;

extern PairB D_L11_00217758[];
extern PairB D_L11_00162618[] MACRO_ADDR;
extern s32   D_L11_00200728[];
extern s32   D_L11_002078D8[];
extern s32   D_L11_00207900[];
extern s32   D_L11_00215220[];
extern char  D_L11_00217770[];
extern int   render_batch_triplet_sources[] __asm__("D_L11_00215148");
extern int   render_batch_pair_sources[] __asm__("D_L11_00215190");
extern int   oil_tanker_render_batch_pair_sources[] __asm__("D_L11_00207888");
extern int   oil_tanker_render_batch_triplet_sources[] __asm__("D_L11_00207860");
extern void  FUN_L00_001fde98(int, int, int, void *, int);
extern void  FUN_L08_00258090(void *, int, int, void *);

/* Emits the 17 render batches offset by one pair entry (the oil tanker variant below emits 9). */
void emit_render_batches_with_pair_offset(int pair_index) __asm__("FUN_L11_0031e6a8");

void emit_render_batches_with_pair_offset(int pair_index) {
    int i;
    for (i = 0; i < 0x11; i++) {
        FUN_L08_00258090(D_L11_00217770,
                         render_batch_pair_sources[i],
                         D_L11_00207900[i],
                         &D_L11_00217758[pair_index]);
        FUN_L00_001fde98(D_L11_00207900[i],
                         render_batch_triplet_sources[i],
                         D_L11_00215220[i],
                         D_L11_00217770,
                         1);
    }
}

void emit_oil_tanker_render_batches_with_pair_offset(int pair_index) {
    int i;
    for (i = 0; i < 0x9; i++) {
        FUN_L08_00258090(D_L11_00217770,
                         oil_tanker_render_batch_pair_sources[i],
                         D_L11_00200728[i],
                         &D_L11_00162618[pair_index]);
        FUN_L00_001fde98(D_L11_00200728[i],
                         oil_tanker_render_batch_triplet_sources[i],
                         D_L11_002078D8[i],
                         D_L11_00217770,
                         1);
    }
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_0031EFC0.c: func_L11_0031FDA0), where it is exact; names translated to the US level program. */

extern int  D_L11_00162610; /* no foreign declaration */
extern char distance_faded_effect_texture_2c_fixed_alpha_gp[4] __asm__("D_L11_001625C8")
    __attribute__((sda));
extern char distance_faded_effect_texture_2a_pair_zero_fixed_alpha_gp[4] __asm__("D_L11_001625E8")
    __attribute__((sda));
extern char distance_faded_effect_texture_2b_fixed_alpha_gp[4] __asm__("D_L11_001625F8")
    __attribute__((sda));

void draw_level11_distance_faded_effect_batches(struct Moby *moby) __asm__("FUN_L11_0031e930");

void draw_level11_distance_faded_effect_batches(struct Moby *moby) {
    int  *moby_vars = (int *)moby->pvars;
    int   flag = 0;
    char *reference_position_base;
    vu1_add_g_sregister(0x42, 0x8000000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1L);
    font_queue_vu_state();
    reference_position_base = (char *)&l11_camera;
    if (*(float *)(reference_position_base + 0x148) > 255.0f ||
        is_point_inside_clip_volume(reference_position_base + 0x140, moby_vars[0]) ||
        is_point_inside_clip_volume(reference_position_base + 0x140, moby_vars[1]) ||
        is_point_inside_clip_volume(reference_position_base + 0x140, moby_vars[2])) {
        flag = 1;
    }
    if (flag) {
        vu1_add_g_sregister(6, get_effect_texture(0x2C));
        vu1_add_g_sregister(
            0x42, ((long)*(int *)&distance_faded_effect_texture_2c_fixed_alpha_gp << 32) | 0x44);
        emit_render_batches_with_pair_offset(0);
    }
    if (*(unsigned char *)&D_L11_00162610 != 0 && flag) {
        vu1_add_g_sregister(6, get_effect_texture(0x29));
        vu1_add_g_sregister(0x42,
                            ((long)((distance_faded_effect_texture_29_fixed_alpha_scale *
                                     *(unsigned char *)&D_L11_00162610) >>
                                    8)
                             << 32) |
                                0x68);
        emit_render_batches_with_pair_offset(1);
        vu1_add_g_sregister(0x42,
                            ((long)((distance_faded_effect_texture_29_fixed_alpha_scale *
                                     *(unsigned char *)&D_L11_00162610) >>
                                    8)
                             << 32) |
                                0x62);
        emit_render_batches_with_pair_offset(2);
    }
    vu1_add_g_sregister(6, get_effect_texture(0x2A));
    vu1_add_g_sregister(
        0x42,
        ((long)*(int *)&distance_faded_effect_texture_2a_pair_zero_fixed_alpha_gp << 32) | 0x48);
    emit_oil_tanker_render_batches_with_pair_offset(0);
    vu1_add_g_sregister(0x42,
                        ((long)distance_faded_effect_texture_2a_pair_one_fixed_alpha << 32) | 0x48);
    emit_oil_tanker_render_batches_with_pair_offset(1);
    if (flag) {
        vu1_add_g_sregister(6, get_effect_texture(0x2B));
        vu1_add_g_sregister(
            0x42, ((long)*(int *)&distance_faded_effect_texture_2b_fixed_alpha_gp << 32) | 0x48);
        queue_oil_tanker_front_texture_2b_batches(0);
    }
}
/* Level overlay code; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* triggers objects matching the moby's class */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_003065F8), where it is exact; names translated to the US level program. */

extern int D_L11_00162630[2] MACRO_ADDR;

typedef struct {
    float f;
    int   pad;
} FlB;

extern FlB   D_L11_0016261C[] MACRO_ADDR;
extern FlB   D_L11_0016262C[] MACRO_ADDR;
extern PairB D_L11_00162628[] MACRO_ADDR;
extern char  distance_faded_effect_sine_output_pair[] __asm__("D_L11_00217760");
extern s32   D_L11_00217740[];
extern s32   D_L11_00215268[];
extern float distance_faded_effect_position_x_gp __asm__("D_L11_001625B8") __attribute__((sda));

#include "rnc/overlay/l11/mobys/distance_faded_scroll_effect_data/update_level11_distance_faded_effect_and_queue_draw.h"

extern int distance_faded_effect_color_tables_initialized_gp __asm__("D_L11_001625FC")
    __attribute__((sda));
extern unsigned char D_L11_00162610_c __asm__("D_L11_00162610");

void update_level11_distance_faded_effect_and_queue_draw(struct Moby *moby) __asm__("FUN_L11_0031f150");

void update_level11_distance_faded_effect_and_queue_draw(struct Moby *moby) {
    enum {
        EFFECT_INITIALIZE = 0,
        EFFECT_UPDATE_AND_QUEUE_DRAW = 1
    };
    int   i;
    float effect_bounds[4];
    float effect_to_reference_delta[4];
    float effect_distance;
    char *reference_position_base;

    switch (moby->state) {
    case EFFECT_INITIALIZE:
        if (distance_faded_effect_color_tables_initialized_gp == 0) {
            scale_rgba_tables(D_L11_00215220, 17, D_L11_00207900, 0.7f, 1.0f, 0.9f, 0.5f, 1);
            scale_rgba_tables(D_L11_00217740, 5, D_L11_00215268, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            scale_rgba_tables(D_L11_002078D8, 9, D_L11_00200728, 0.25f, 0.25f, 0.25f, 0.7f, 1);
            distance_faded_effect_color_tables_initialized_gp = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L11_00217758[i].a = 0;
            D_L11_00217758[i].b = 0;
            D_L11_00162630[0] = 0;
            D_L11_00162630[1] = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L11_00162618[i].a = 0;
            D_L11_0016261C[i].f = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L11_00162628[i].a = 0;
            D_L11_0016262C[i].f = i * 0.4f;
        }
        break;
    case EFFECT_UPDATE_AND_QUEUE_DRAW:
        reference_position_base = (char *)&l11_camera;
        if (*(float *)(reference_position_base + 0x148) < 170.0f)
            return;
        effect_bounds[0] = distance_faded_effect_position_x_gp;
        effect_bounds[1] = distance_faded_effect_position_y;
        effect_bounds[2] = distance_faded_effect_position_z;
        effect_bounds[3] = distance_faded_effect_bounds_radius;
        if (fast_bsphere_check(effect_bounds, 1000.0f) < 0)
            return;
        if (!(180.0f < *(float *)(reference_position_base + 0x148)))
            return;
        effect_to_reference_delta[0] = distance_faded_effect_position_x_gp;
        effect_to_reference_delta[1] = distance_faded_effect_position_y;
        effect_to_reference_delta[2] = distance_faded_effect_position_z;
        effect_to_reference_delta[3] = 1.0f;
        fast_vec_sub(
            effect_to_reference_delta, reference_position_base + 0x140, effect_to_reference_delta);
        effect_distance = fast_vec_length(effect_to_reference_delta);
        if (effect_distance <= 100.0f) {
            advance_scroll_offsets(0);
            advance_wrapped_sine_pair(distance_faded_effect_sine_output_pair,
                                      0.52f,
                                      0.62f,
                                      0.06f,
                                      (void *)D_L11_00162630);
            advance_scroll_offsets(2);
            if (effect_distance < 80.0f) {
                D_L11_00162610_c = 0xFF;
            } else if (effect_distance < 100.0f) {
                D_L11_00162610_c =
                    truncate_float_to_s32((1.0f - (effect_distance - 80.0f) / 20.0f) * 255.0f);
            } else {
                D_L11_00162610_c = 0;
            }
        }
        advance_scroll_entry(0);
        advance_scroll_entry(0);
        advance_scroll_entry(1);
        add_draw_callback(draw_level11_distance_faded_effect_batches, moby);
        break;
    }
}

int D_L11_00162630[2] = {0};
