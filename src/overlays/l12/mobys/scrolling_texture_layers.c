/* Level 12 moby class scrolling_texture_layers (class 1901): draws Hoven's scrolling texture layers. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

typedef struct Moby Moby;
extern char         D_L12_00167240[];

#include "rnc/overlay/l12/mobys/scrolling_texture_layers_globals.h"

extern long get_effect_texture(int) __asm__("FUN_001f44b8");

/* Draws the moby's 10-piece overlay when its trigger is visible, with its own GS state. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030D248), where it is exact; names translated to the US level program. */

extern float D_L12_00162108[2] MACRO_ADDR;
extern char  hoven_scrolling_clipped_scrolled_uv_buffer[] __asm__("D_L12_00208B00");
extern int   hoven_scrolling_clipped_vertex_counts[] __asm__("D_L12_001FBD10");
extern int   hoven_scrolling_clipped_vertex_position_tables[] __asm__("D_L12_002059E0");
extern int   hoven_scrolling_clipped_base_uv_tables[] __asm__("D_L12_00205A08");
extern int   hoven_scrolling_clipped_vertex_color_tables[] __asm__("D_L12_00205A58");
extern void  FUN_L00_001fde98(int, int, int, void *, int);
extern void  FUN_L08_00258090(void *, int, int, void *);
extern void  draw_eleven_entry_scrolled_texture_batch(int) __asm__("FUN_L08_002f0c18");
extern void  vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");

void draw_hoven_scrolling_texture_layers(struct Moby *moby) __asm__("FUN_L12_0030be68");

void draw_hoven_scrolling_texture_layers(struct Moby *moby) {
    int *moby_data = (int *)moby->pvars;
    int  i;
    font_queue_vu_state();
    vu1_add_g_sregister(0x42, 0x7F00000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x5360A);
    if (is_point_inside_clip_volume(D_L12_00167240, moby_data[0])) {
        vu1_add_g_sregister(6, get_effect_texture(0x2C));
        for (i = 0; i < 10; i++) {
            FUN_L08_00258090(hoven_scrolling_clipped_scrolled_uv_buffer,
                             hoven_scrolling_clipped_base_uv_tables[i],
                             hoven_scrolling_clipped_vertex_counts[i],
                             D_L12_00162108);
            FUN_L00_001fde98(hoven_scrolling_clipped_vertex_counts[i],
                             hoven_scrolling_clipped_vertex_position_tables[i],
                             hoven_scrolling_clipped_vertex_color_tables[i],
                             hoven_scrolling_clipped_scrolled_uv_buffer,
                             1);
        }
    }
    vu1_add_g_sregister(6, get_effect_texture(0x2F));
    draw_eleven_entry_scrolled_texture_batch(0);
    vu1_add_g_sregister(0x42, ((long)hoven_scrolling_second_layer_fixed_alpha << 32) | 0x68);
    vu1_add_g_sregister(6, get_effect_texture(0x30));
    draw_eleven_entry_scrolled_texture_batch(1);
}

/* Builds the colour tables once, then scrolls two pairs of wrapped phases each frame. */
extern int hoven_scrolling_color_tables_initialized_gp __asm__("D_L12_001620F0")
    __attribute__((sda));
/* The ready flag is followed, 8 bytes on, by two pairs of phases. */
extern float hoven_scrolling_layer_phase_rates[1] __asm__("D_L12_001620E0") __attribute__((sda));
extern float hoven_scrolling_clipped_uv_scroll_rates[2] __asm__("D_L12_001620D8")
    __attribute__((sda));
extern float frame_delta_time __asm__("D_0015ED7C") MACRO_ADDR;
extern int  *hoven_scrolling_color_tables[] __asm__("D_L12_00208AE0");
extern int   hoven_scrolling_color_table_entry_counts[] __asm__("D_L12_00205A80");
extern void
scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");

typedef struct {
    char          p[0x20];
    unsigned char state;
    char          q[0xF];
    unsigned char b30;
    char          r;
    short         h32;
} M_bff0;

void update_hoven_scrolling_texture_layers(M_bff0 *moby) __asm__("FUN_L12_0030bff0");

void update_hoven_scrolling_texture_layers(M_bff0 *moby) {
    enum {
        INITIALIZE_SCROLL_PHASES = 0,
        UPDATE_SCROLL_PHASES_AND_QUEUE_DRAW = 1
    };
    int i;
    switch (moby->state) {
    case INITIALIZE_SCROLL_PHASES:
        if (hoven_scrolling_color_tables_initialized_gp == 0) {
            scale_rgba_tables(hoven_scrolling_color_tables,
                              5,
                              hoven_scrolling_color_table_entry_counts,
                              0.1f,
                              0.5f,
                              0.4f,
                              0.5f,
                              1);
            scale_rgba_tables(hoven_scrolling_clipped_vertex_color_tables,
                              10,
                              hoven_scrolling_clipped_vertex_counts,
                              0.1f,
                              0.6f,
                              0.4f,
                              0.5f,
                              1);
            hoven_scrolling_color_tables_initialized_gp = 1;
        }
        moby->state = 1;
        moby->b30 = 0xFF;
        moby->h32 = 0xFF;
        for (i = 0; i < 2; i++) {
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] = 0.0f;
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] = 0.0f;
        }
        break;
    case UPDATE_SCROLL_PHASES_AND_QUEUE_DRAW:
        for (i = 0; i < 2; i++) {
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] +=
                hoven_scrolling_layer_phase_rates[i * 2] * frame_delta_time;
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] > 1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] -= 1.0f;
            }
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] < -1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] += 1.0f;
            }
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] +=
                hoven_scrolling_layer_phase_rates[i * 2 + 1] * frame_delta_time;
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] > 1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] -= 1.0f;
            }
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] < -1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] += 1.0f;
            }
        }
        D_L12_00162108[0] += hoven_scrolling_clipped_uv_scroll_rates[0] * frame_delta_time;
        if (D_L12_00162108[0] > 1.0f) {
            D_L12_00162108[0] -= 1.0f;
        }
        if (D_L12_00162108[0] < -1.0f) {
            D_L12_00162108[0] += 1.0f;
        }
        D_L12_00162108[1] += hoven_scrolling_clipped_uv_scroll_rates[1] * frame_delta_time;
        if (D_L12_00162108[1] > 1.0f) {
            D_L12_00162108[1] -= 1.0f;
        }
        if (D_L12_00162108[1] < -1.0f) {
            D_L12_00162108[1] += 1.0f;
        }
        add_draw_callback(draw_hoven_scrolling_texture_layers, moby);
        break;
    }
}

s32 D_L12_001FB6CC[1] __attribute__((section(".data"))) = {0};

float D_L12_00162108[2] = {0};
