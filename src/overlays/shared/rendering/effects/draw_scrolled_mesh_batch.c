/* Scrolling mesh and texture batches: draws the scrolled batches, scales their colour tables and advances their scroll offsets. */
#include "types.h"
#include "rnc/overlay/entities.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"

typedef struct {
    float x;
    float y;
} V2_12a0;

extern Pair D_L08_001E8348[];
extern char D_L08_001E8360[];
extern int  D_L08_001E2858[];
extern int  scrolled_mesh_position_streams[] __asm__("D_L08_001E8268");
extern int  scrolled_mesh_uv_source_streams[] __asm__("D_L08_001E82A0");
extern int  D_L08_001E8310[];
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L08_00258090(void *, int, int, void *);

extern Pair D_L08_00161E10[] MACRO_ADDR;
extern int  D_L08_001DDF40[];
extern int  eleven_entry_scrolled_base_uv_tables[] __asm__("D_L08_001E27C8");
extern int  eleven_entry_scrolled_vertex_position_tables[] __asm__("D_L08_001E2798");
extern int  D_L08_001E2828[];

extern float frame_delta_time __asm__("D_0015ED7C");

extern V2_12a0 D_L08_001E8348_12a0[] __asm__("D_L08_001E8348");

extern V2_12a0 l08_scroll_pair_rates[] __asm__("D_L08_001DB500");

extern float scroll_pos_x[] __asm__("D_L08_00161E10") MACRO_ADDR;
extern float scroll_pos_y[] __asm__("D_L08_00161E14") MACRO_ADDR;

extern float scroll_entry_x_rates_gp[2] __asm__("D_L08_00161DE0") __attribute__((sda));
extern float D_L08_00161DE4[] MACRO_ADDR;

/* Runs the two per-entry update calls over a 13-entry table. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2000), where it is exact; names translated to the US level program. */

void draw_scrolled_mesh_batch(int pair_index) __asm__("FUN_L08_002f0b40");

void draw_scrolled_mesh_batch(int pair_index) {
    int i;
    for (i = 0; i < 13; i++) {
        FUN_L08_00258090(D_L08_001E8360,
                         scrolled_mesh_uv_source_streams[i],
                         D_L08_001E2858[i],
                         &D_L08_001E8348[pair_index]);
        FUN_L00_001fde98(D_L08_001E2858[i],
                         scrolled_mesh_position_streams[i],
                         D_L08_001E8310[i],
                         D_L08_001E8360,
                         1);
    }
}

void draw_eleven_entry_scrolled_texture_batch(int scroll_pair_index) __asm__("FUN_L08_002f0c18");

void draw_eleven_entry_scrolled_texture_batch(int scroll_pair_index) {
    int i;
    for (i = 0; i < 11; i++) {
        FUN_L08_00258090(D_L08_001E8360,
                         eleven_entry_scrolled_base_uv_tables[i],
                         D_L08_001DDF40[i],
                         &D_L08_00161E10[scroll_pair_index]);
        FUN_L00_001fde98(D_L08_001DDF40[i],
                         eleven_entry_scrolled_vertex_position_tables[i],
                         D_L08_001E2828[i],
                         D_L08_001E8360,
                         1);
    }
}

/* Scales every RGBA word of a set of colour tables by four factors, optionally using the flat 255 for colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2428), where it is exact; names translated to the US level program. */

void scale_rgba_tables(int **tabs,
                       int   table_count,
                       int  *counts,
                       int   flat,
                       float r,
                       float g,
                       float b,
                       float alpha_scale) __asm__("FUN_L08_002f0f68");

void scale_rgba_tables(int **tabs,
                       int   table_count,
                       int  *counts,
                       int   flat,
                       float r,
                       float g,
                       float b,

                       float alpha_scale) {
    int i, j;
    for (i = 0; i < table_count; i++) {
        for (j = 0; j < counts[i]; j++) {
            int   packed_rgba = tabs[i][j];
            int   red_channel = packed_rgba & 0xFF;
            int   green_channel = (packed_rgba & 0xFF00) >> 8;
            int   blue_channel = ((unsigned)packed_rgba >> 16) & 0xFF;
            int   alpha_channel = (unsigned)packed_rgba >> 24;
            float fr, fg, fb, fa;
            if (flat == 0) {
                fr = r * convert_integer_to_float(red_channel);
                fg = g * convert_integer_to_float(green_channel);
                fb = b * convert_integer_to_float(blue_channel);
                fa = alpha_scale * convert_integer_to_float(alpha_channel);
            } else {
                fr = r * 255.0f;
                fg = g * 255.0f;
                fb = b * 255.0f;
                fa = alpha_scale * convert_integer_to_float(alpha_channel);
            }
            tabs[i][j] = (truncate_float_to_s32(fa) << 24) + (truncate_float_to_s32(fb) << 16) +
                         (truncate_float_to_s32(fg) << 8) + truncate_float_to_s32(fr);
        }
    }
}

/* advance a 2-D wrapped phase by a and b and write scaled sines */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_002F2618), where it is exact; names translated to the US level program. */

void advance_wrapped_sine_pair(float *out,
                               float *ph,
                               float  first_phase_rate,
                               float  second_phase_rate,
                               float  scale) __asm__("FUN_L08_002f1158");

void advance_wrapped_sine_pair(

    float *out, float *ph, float first_phase_rate, float second_phase_rate, float scale) {
    ph[0] += first_phase_rate * frame_delta_time;
    if (ph[0] > 1.0f) {
        ph[0] -= 2.0f;
    }
    if (ph[0] < -1.0f) {
        ph[0] += 2.0f;
    }
    ph[1] += second_phase_rate * frame_delta_time;
    if (ph[1] > 1.0f) {
        ph[1] -= 2.0f;
    }
    if (ph[1] < -1.0f) {
        ph[1] += 2.0f;
    }
    out[0] = fast_sin(ph[0] * 3.1415927f) * scale;
    out[1] = fast_sin(ph[1] * 3.1415927f) * scale;
}

void advance_scroll_offsets(int scroll_pair_index) __asm__("FUN_L08_002f12a0");

void advance_scroll_offsets(int scroll_pair_index) {
    D_L08_001E8348_12a0[scroll_pair_index].x +=
        l08_scroll_pair_rates[scroll_pair_index].x * frame_delta_time;
    if (D_L08_001E8348_12a0[scroll_pair_index].x > 1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].x -= 1.0f;
    }
    if (D_L08_001E8348_12a0[scroll_pair_index].x < -1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].x += 1.0f;
    }
    D_L08_001E8348_12a0[scroll_pair_index].y +=
        l08_scroll_pair_rates[scroll_pair_index].y * frame_delta_time;
    if (D_L08_001E8348_12a0[scroll_pair_index].y > 1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].y -= 1.0f;
    }
    if (D_L08_001E8348_12a0[scroll_pair_index].y < -1.0f) {
        D_L08_001E8348_12a0[scroll_pair_index].y += 1.0f;
    }
}

/* Advances the X and Y scroll offsets of entry `i` by speed * frame step, wrapping them into [-1, 1]. */

void advance_scroll_entry(int i) __asm__("FUN_L08_002f1378");

void advance_scroll_entry(int i) {
    float *scroll_offset_ptr;
    scroll_offset_ptr = &scroll_pos_x[i * 2];
    *scroll_offset_ptr += scroll_entry_x_rates_gp[i * 2] * frame_delta_time;
    if (*scroll_offset_ptr > 1.0f)
        *scroll_offset_ptr -= 1.0f;
    if (*scroll_offset_ptr < -1.0f)
        *scroll_offset_ptr += 1.0f;
    scroll_offset_ptr = &scroll_pos_y[i * 2];
    *scroll_offset_ptr += D_L08_00161DE4[i * 2] * frame_delta_time;
    if (*scroll_offset_ptr > 1.0f)
        *scroll_offset_ptr -= 1.0f;
    if (*scroll_offset_ptr < -1.0f)
        *scroll_offset_ptr += 1.0f;
}
