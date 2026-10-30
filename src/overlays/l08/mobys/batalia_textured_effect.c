/* Level 08 moby class 0x671: batalia textured effect, and the draw callback that queues its textured batches. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/entities.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/l08/mobys/batalia_textured_effect_globals.h"

/* Builds and queues the eight textured batches for scroll pair `pair_index`. */

extern Pair D_L08_001F8548[];
extern int  batalia_eight_batch_packet_triplet_sources[] __asm__("D_L08_001F84C8");
extern int  batalia_eight_batch_offset_source_streams[] __asm__("D_L08_001F84E8");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L08_00258090(void *, int, int, void *);

void queue_batalia_eight_textured_batches(int pair_index) __asm__("FUN_L08_00308db8");

void queue_batalia_eight_textured_batches(int pair_index) {
    int i;
    for (i = 0; i < 0x8; i++) {
        FUN_L08_00258090(D_L08_001F8560,
                         batalia_eight_batch_offset_source_streams[i],
                         D_L08_001F4C60[i],
                         &D_L08_001F8548[pair_index]);
        FUN_L00_001fde98(D_L08_001F4C60[i],
                         batalia_eight_batch_packet_triplet_sources[i],
                         D_L08_001F8528[i],
                         D_L08_001F8560,
                         1);
    }
}

extern Pair D_L08_001623F8[] MACRO_ADDR;
extern int  batalia_six_batch_offset_source_streams[] __asm__("D_L08_001F4C18");
extern int  batalia_six_batch_packet_triplet_sources[] __asm__("D_L08_001F4C00");
/* Builds and queues the six textured batches for scroll pair `pair_index`. */
void queue_batalia_six_textured_batches(int pair_index) __asm__("FUN_L08_00308e90");

void queue_batalia_six_textured_batches(int pair_index) {
    int i;
    for (i = 0; i < 0x6; i++) {
        FUN_L08_00258090(D_L08_001F8560,
                         batalia_six_batch_offset_source_streams[i],
                         D_L08_001F01C8[i],
                         &D_L08_001623F8[pair_index]);
        FUN_L00_001fde98(D_L08_001F01C8[i],
                         batalia_six_batch_packet_triplet_sources[i],
                         D_L08_001F4C48[i],
                         D_L08_001F8560,
                         1);
    }
}

/* Runs the two-entry setup loop for slot `x`: one call fills from the tables, the second applies them. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030A428), where it is exact; names translated to the US level program. */
extern Pair  D_L08_00162408[] MACRO_ADDR;
extern char  D_L08_001F8560[];
extern float D_L08_001EF7D0[];
extern float D_L08_001EF978[];
extern float D_L08_001EFB20[];
extern float D_L08_001EFC40[];
extern int   D_L08_001F00A8[];
extern int   D_L08_001F0138[];

int D_L08_001623C8[2] = {0x23, 0x23};

int *D_L08_001623E8[2] = {D_L08_001F00A8, D_L08_001F0138};

/* The batch routines of the functions above, declared with the table pointer types this one passes. */
extern void FUN_L00_001fde98_tables(int, float *, int *, void *, int) __asm__("FUN_L00_001fde98");
extern void FUN_L08_00258090_tables(void *, float *, int, void *) __asm__("FUN_L08_00258090");

float *D_L08_001623D0[2] = {D_L08_001EF7D0, D_L08_001EF978};

float *D_L08_001623D8[2] = {D_L08_001EFB20, D_L08_001EFC40};

void queue_batalia_offset_textured_batches(int x) __asm__("FUN_L08_00308f68");

extern int *D_L08_001623E8[2] __attribute__((sda));
extern int  D_L08_001623C8[2] __attribute__((sda));

void queue_batalia_offset_textured_batches(int x) {
    int     i;
    int    *element_counts = D_L08_001623C8;
    float **float_pair_tables = D_L08_001623D8;
    float **float_triplet_tables = D_L08_001623D0;
    int   **element_word_tables = D_L08_001623E8;

    for (i = 0; i < 2; i++) {
        FUN_L08_00258090_tables(
            D_L08_001F8560, float_pair_tables[i], element_counts[i], &D_L08_00162408[x]);
        FUN_L00_001fde98_tables(
            element_counts[i], float_triplet_tables[i], element_word_tables[i], D_L08_001F8560, 1);
    }
}

int                  batalia_texture_3a_blend_factor_gp __asm__("D_L08_00162390") = 0x40;
int                  batalia_texture_3c_pair_zero_blend_factor_gp __asm__("D_L08_001623B0") = 0x20;
int                  batalia_texture_3d_blend_factor_gp __asm__("D_L08_001623C0") = 0x30;
extern unsigned char D_L08_00161E08_u __asm__("D_L08_001623F0");
extern int           get_effect_texture(int) __asm__("FUN_001f44b8");

/* Draw callback: queues the batches with effect textures 0x3A to 0x3D, two of them faded by distance. */
void queue_batalia_textured_batch_passes(void) __asm__("FUN_L08_00309030");

void queue_batalia_textured_batch_passes(void) {
    vu1_add_g_sregister(0x42, 0x8000000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    font_queue_vu_state();
    vu1_add_g_sregister(6, get_effect_texture(0x3A));
    vu1_add_g_sregister(0x42, (long)batalia_texture_3a_blend_factor_gp << 32 | 0x64);
    queue_batalia_eight_textured_batches(0);
    vu1_add_g_sregister(6, get_effect_texture(0x3B));
    vu1_add_g_sregister(
        0x42,
        (long)((batalia_texture_3b_pair_one_blend_factor_base * D_L08_00161E08_u) >> 8) << 32 |
            0x62);
    queue_batalia_eight_textured_batches(1);
    vu1_add_g_sregister(
        0x42,
        (long)((batalia_texture_3b_pair_two_blend_factor_base * D_L08_00161E08_u) >> 8) << 32 |
            0x68);
    queue_batalia_eight_textured_batches(2);
    vu1_add_g_sregister(6, get_effect_texture(0x3C));
    vu1_add_g_sregister(0x42, (long)batalia_texture_3c_pair_zero_blend_factor_gp << 32 | 0x48);
    queue_batalia_six_textured_batches(0);
    vu1_add_g_sregister(0x42, (long)batalia_texture_3c_pair_one_blend_factor << 32 | 0x48);
    queue_batalia_six_textured_batches(1);
    vu1_add_g_sregister(6, get_effect_texture(0x3D));
    vu1_add_g_sregister(0x42, (long)batalia_texture_3d_blend_factor_gp << 32 | 0x48);
    queue_batalia_offset_textured_batches(0);
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030AC70), where it is exact; names translated to the US level program. */
extern int  D_L08_001623F0_macro[] __asm__("D_L08_001623F0") MACRO_ADDR;
extern Pair D_L08_001F8548[];
extern char D_L08_001675C0[];
extern s32  D_L08_001F01C8[];
extern s32  D_L08_001F4C48[];
extern s32  D_L08_001F4C60[];
extern s32  D_L08_001F8528[];
extern char batalia_effect_sine_offsets[] __asm__("D_L08_001F8550");

extern int   D_L08_001623F0;
extern short batalia_effect_reference_position_x __asm__("D_L08_00162380") __attribute__((sda));

int D_L08_001623C4 = 0;

extern void
scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");
extern void
advance_wrapped_sine_pair(void *, float, float, float, void *) __asm__("FUN_L08_002f1158");
extern void advance_scroll_offsets(int) __asm__("FUN_L08_002f12a0");
extern void advance_scroll_entry(int) __asm__("FUN_L08_002f1378");
extern int  func_001FA898_r(float) __asm__("FUN_001fa6d0");
void        update_batalia_textured_effect(struct Moby *moby) __asm__("FUN_L08_003097b0");

void update_batalia_textured_effect(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_ACTIVE = 1
    };
    int   i;
    float sphere_check_input[4];
    float position_delta[4];
    float distance;

    switch (moby->state) {
    case MOBY_INITIALIZE:
        if (D_L08_001623C4 == 0) {
            scale_rgba_tables(D_L08_001F8528, 8, D_L08_001F4C60, 0.5f, 0.5f, 0.65f, 0.5f, 1);
            scale_rgba_tables(D_L08_001623E8, 2, D_L08_001623C8, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            scale_rgba_tables(D_L08_001F4C48, 6, D_L08_001F01C8, 0.25f, 0.25f, 0.25f, 1.0f, 1);
            D_L08_001623C4 = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L08_001F8548[i].a = 0;
            D_L08_001F8548[i].b = 0;
            D_L08_001623F0_macro[8] = 0;
            D_L08_001623F0_macro[9] = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L08_001623F0_macro[2 + i * 2] = 0;
            *(float *)&D_L08_001623F0_macro[3 + i * 2] = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L08_001623F0_macro[6 + i * 2] = 0;
            *(float *)&D_L08_001623F0_macro[7 + i * 2] = i * 0.4f;
        }
        break;
    case MOBY_ACTIVE:
        sphere_check_input[0] = *(float *)&batalia_effect_reference_position_x;
        sphere_check_input[1] = batalia_effect_reference_position_y;
        sphere_check_input[2] = batalia_effect_reference_position_z;
        sphere_check_input[3] = batalia_effect_culling_radius;
        if (fast_bsphere_check((char *)sphere_check_input, 1000.0f) >= 0) {
            position_delta[0] = *(float *)&batalia_effect_reference_position_x;
            position_delta[1] = batalia_effect_reference_position_y;
            position_delta[2] = batalia_effect_reference_position_z;
            position_delta[3] = 1.0f;
            fast_vec_sub(position_delta, D_L08_001675C0, position_delta);
            distance = fast_vec_length(position_delta);
            if (distance <= 84.0f) {
                advance_scroll_offsets(0);
                advance_wrapped_sine_pair(
                    batalia_effect_sine_offsets, 0.52f, 0.62f, 0.06f, &D_L08_001623F0_macro[8]);
                advance_scroll_offsets(2);
                if (distance < 20.0f) {
                    *(unsigned char *)&D_L08_001623F0 = 0xFF;
                } else if (distance < 84.0f) {
                    *(char *)&D_L08_001623F0 =
                        func_001FA898_r((1.0f - (distance - 20.0f) * 0.015625f) * 255.0f);
                } else {
                    *(char *)&D_L08_001623F0 = 0;
                }
            }
            advance_scroll_entry(0);
            advance_scroll_entry(0);
            advance_scroll_entry(1);
            add_draw_callback(queue_batalia_textured_batch_passes, moby);
        }
        break;
    }
}

int batalia_texture_3b_pair_one_blend_factor_base = 0x20;

int batalia_texture_3b_pair_two_blend_factor_base = 0x40;

int batalia_texture_3c_pair_one_blend_factor = 0x22;
