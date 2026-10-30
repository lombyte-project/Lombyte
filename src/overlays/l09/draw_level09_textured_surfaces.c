/* Level 09 textured surfaces: scrolling strips and scaled entries. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/rendering/view/update_camera_view_matrices.h"
#include "rnc/rendering/vu1/vu1_add_data_ref.h"

extern union PacketCursor l09_packet __asm__("D_L09_001611C0") MACRO_ADDR;

#define PACKET_CURSOR  (l09_packet.words)
#define TEXTURE_CURSOR (gs_texture_allocation.cursor)

extern char *texture_vertices __asm__("D_L09_0015F420") MACRO_ADDR;
extern char  level09_strip_texture_entry_table[] __asm__("D_L09_0016D300");
extern long  level09_strip_texture_transfer_packet[] __asm__("D_L09_00161FC0");
extern long  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  FUN_L02_0024fca0(void *, void *, void *, void *, void *, int);

typedef struct {
    char           unused[8];
    unsigned short first;
    unsigned short second;
    char           tail[4];
} TextureEntry;

/* Queues the GS packet for one texture (tex0 == tex1), or builds the strip between two texture entries and queues it. */
void queue_level09_strip_texture_packets(int first_texture_index,
                                         int second_texture_index) __asm__("FUN_L09_0021e560");

void queue_level09_strip_texture_packets(int first_texture_index, int second_texture_index) {
    if (first_texture_index == second_texture_index) {
        int  *packet;
        long *words;

        PACKET_CURSOR[0] = 0x10000002;
        PACKET_CURSOR[1] = 0;
        PACKET_CURSOR[2] = 0;
        PACKET_CURSOR[3] = 0x50000002;
        packet = PACKET_CURSOR;
        PACKET_CURSOR = packet + 4;
        words = (long *)(packet + 4);
        words[0] = 0x1000000000008001L;
        words[1] = 0xEEEEEEE;
        words[2] = get_effect_texture(second_texture_index);
        words[3] = 6;
        PACKET_CURSOR = packet + 12;
    } else {
        int  *packet;
        long *words;
        int   a_second =
            ((TextureEntry *)level09_strip_texture_entry_table)[first_texture_index].second;
        int a_first =
            ((TextureEntry *)level09_strip_texture_entry_table)[first_texture_index].first;
        int b_second =
            ((TextureEntry *)level09_strip_texture_entry_table)[second_texture_index].second;
        int b_first =
            ((TextureEntry *)level09_strip_texture_entry_table)[second_texture_index].first;

        level09_strip_texture_transfer_packet[0] = 0x0800000000000400L;
        level09_strip_texture_transfer_packet[1] = 0;
        FUN_L02_0024fca0(&level09_strip_texture_transfer_packet[2],
                         texture_vertices + (a_first << 4),
                         texture_vertices + (a_second << 4),
                         texture_vertices + (b_first << 4),
                         texture_vertices + (b_second << 4),
                         0x1000);
        PACKET_CURSOR[0] = 0x10000006;
        PACKET_CURSOR[1] = 0;
        PACKET_CURSOR[2] = 0;
        PACKET_CURSOR[3] = 0x50000006;
        packet = PACKET_CURSOR;
        PACKET_CURSOR = packet + 4;
        words = (long *)(packet + 4);
        words[0] = 0x5000000000008001L;
        words[1] = 0xEEEEEEE;
        words[2] = (long)(TEXTURE_CURSOR >> 8) | 0x18100000 | (0xB000L << 19);
        words[3] = 6;
        words[4] = (long)(TEXTURE_CURSOR >> 8) << 32 | 0x1000000000000L;
        words[5] = 0x50;
        words[6] = 0;
        words[7] = 0x51;
        words[8] = 0x4000000040L;
        words[9] = 0x52;
        words[10] = 0;
        words[11] = 0x53;
        PACKET_CURSOR = packet + 28;
        PACKET_CURSOR[0] = 0x30000401;
        PACKET_CURSOR[1] = (int)level09_strip_texture_transfer_packet;
        PACKET_CURSOR[2] = 0;
        PACKET_CURSOR[3] = 0x50000401;
        PACKET_CURSOR += 4;
        PACKET_CURSOR[0] = 0x10000002;
        PACKET_CURSOR[1] = 0;
        PACKET_CURSOR[2] = 0;
        PACKET_CURSOR[3] = 0x50000002;
        PACKET_CURSOR += 4;
        packet = PACKET_CURSOR;
        words = (long *)packet;
        words[0] = 0x1000000000008001L;
        words[1] = 0xE;
        words[2] = 0;
        words[3] = 0x3F;
        PACKET_CURSOR = packet + 8;
        vu1_add_g_sregister(7, (long)(TEXTURE_CURSOR >> 8) | 0x18100000 | (0xB000L << 19));
        TEXTURE_CURSOR += 0x4000;
    }
}

#undef PACKET_CURSOR
#undef TEXTURE_CURSOR

/* Draws a list of scrolling-texture strips (water) into the scratchpad. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/drawquad_0021E3E8.c: func_L09_0021E770), where it is exact; names translated to the US level program. */
typedef struct {
    char  pad[0x20];
    short idx[16];
} Strip;

extern char level09_scrolling_strip_gs_setup_packet[] __asm__("D_L09_0016EAA0");
extern int *D_L09_001611C0;
extern void FUN_L01_0021fa98(int, int, int, int, int, int);
extern void queue_level09_strip_texture_packets(int, int) __asm__("FUN_L09_0021e560");
extern int *D_L09_001611B8[] MACRO_ADDR;
void        draw_level09_scrolling_textured_strips(char *list,
                                                   int   count,
                                                   float unused,
                                                   long  first_texture_index,
                                                   long  second_texture_index) __asm__("FUN_L09_0021e8c0");

void draw_level09_scrolling_textured_strips(

    char *list, int count, float unused, long first_texture_index, long second_texture_index) {
    int   i;
    int   j;
    int   strip_vertex_index;
    int  *sp;
    int   idx;
    float first_scroll_offset;
    float second_scroll_offset;
    queue_level09_strip_texture_packets(first_texture_index, second_texture_index);
    D_L09_001611B8[2][0] = 0x30000009;
    D_L09_001611B8[2][1] = (int)level09_scrolling_strip_gs_setup_packet;
    D_L09_001611B8[2][2] = 0;
    D_L09_001611B8[2][3] = 0x50000009;
    D_L09_001611C0 = D_L09_001611B8[2] + 4;
    font_queue_vu_state();
    first_scroll_offset = 1.0f - (float)(D_L09_0015F5CC & 0x7FF) * 0.00048828125f;
    second_scroll_offset = first_scroll_offset + first_scroll_offset + 0.5f;
    sp = (int *)0x70000000;
    for (i = 0; i < 256; i++) {
        sp[i] = ((i * 0x89) & 0xF) * 0x20200 + 0x80787070;
    }
    for (i = 0; i < count; i++) {
        char *strip_entry = list + i * 64;
        idx = fast_bsphere_check(strip_entry, 256.0f);
        if (idx == -1)
            continue;
        for (j = 0; j < *(int *)(strip_entry + 0x1C); j++) {
            int    strip_vertex_count;
            float *strip_source_pairs;
            float *first_scroll_destination = (float *)0x70001000;
            float *second_scroll_destination = (float *)0x70002000;
            strip_vertex_count =
                ((Strip *)strip_entry)->idx[j + 1] - ((Strip *)strip_entry)->idx[j];
            strip_source_pairs =
                (float *)(*(int *)(strip_entry + 0x14) + ((Strip *)strip_entry)->idx[j] * 8);
            for (strip_vertex_index = 0; strip_vertex_index < strip_vertex_count;
                 strip_vertex_index++) {
                float first_source_component = *strip_source_pairs++;
                *second_scroll_destination++ = first_source_component;
                *first_scroll_destination++ = first_source_component;
                *first_scroll_destination++ = *strip_source_pairs + first_scroll_offset;
                *second_scroll_destination++ = *strip_source_pairs++ + second_scroll_offset;
            }
            FUN_L01_0021fa98(strip_vertex_count,
                             *(int *)(strip_entry + 0x10) + ((Strip *)strip_entry)->idx[j] * 12,
                             0x70000000,
                             0x70001000,
                             0x70002000,
                             idx == 0);
        }
    }
}

extern void setup_offset_vector(float, void *, void *) __asm__("FUN_001f9a68");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void restore_view_after_textured_grid(void *) __asm__("FUN_L02_002a46e0");

#include "rnc/overlay/l09/draw_level09_textured_surfaces_globals.h"

extern u64            scaled_entries_texture_register_value __asm__("D_L09_001612F0");
extern unsigned short l09_scaled_entries_vu1_setup_qword_count[] __asm__("D_L09_0010E800");
extern int            scaled_entries_vu1_setup_data[] __asm__("D_L09_0010E810");
extern int            scaled_entries_gs_setup_packet[] __asm__("D_L09_001CB470");
extern float          D_L09_00166F40[];
extern float          level09_view_context[] __asm__("D_L09_0016CBC0");

#include "rnc/overlay/l09/draw_level09_textured_surfaces_data/build_packet_and_draw_scaled_entries.h"

extern float D_L09_00161304 MACRO_ADDR;

#include "qcopy.h"

#define PACKET_CURSOR (*(int *volatile *)&D_L09_001611C0)

void build_packet_and_draw_scaled_entries(void *data_ptr,
                                          void *list_ptr,
                                          int   count) __asm__("FUN_L09_002c1978");

void build_packet_and_draw_scaled_entries(void *data_ptr, void *list_ptr, int count) {
    char  *data = data_ptr;
    char  *list = list_ptr;
    int    i;
    int    j;
    int    entry_offset;
    int   *packet;
    u64   *words;
    float  matrix[12];
    float  offset[4];
    u64    color;
    int   *scratch;
    int    scratch_color;
    float *old = (float *)0x001612f8;
    float *current = level09_view_context;
    float  saved0 = current[0x218 / 4];
    float  saved1 = current[0x21c / 4];
    float  saved2 = current[0x228 / 4];
    float  saved3 = current[0x22c / 4];
    saved_textured_grid_fog_near_distance = saved0;
    saved_textured_grid_fog_far_distance = saved1;
    saved_textured_grid_fog_near_intensity = saved2;
    D_L09_00161304 = saved3;
    if (textured_grid_view_settings_mode != 1) {
        current[0x218 / 4] = *(float *)(data + 0x20);
        current[0x21c / 4] = *(float *)(data + 0x24);
        current[0x228 / 4] = *(float *)(data + 0x28);
        current[0x22c / 4] = *(float *)(data + 0x2c);
    }
    update_view_context();
    update_camera_view_matrices();
    set_scale_matrix(matrix, 1024.0f);
    setup_offset_vector(-1024.0f, offset, D_L09_00166F40);
    offset[3] = 1.0f;
    vu1_add_data_ref(scaled_entries_vu1_setup_data, l09_scaled_entries_vu1_setup_qword_count[0]);
    {
        int *packet;
        int *packet_end;
        PACKET_CURSOR[0] = 0x10000000;
        PACKET_CURSOR[1] = 0;
        PACKET_CURSOR[2] = 0x11000000;
        PACKET_CURSOR[3] = 0x01000404;
        packet = PACKET_CURSOR;
        packet[4] = 0;
        packet[5] = 0;
        packet[6] = 0;
        packet[7] = 0x6c0c43a4;
        multiply_matrix(packet + 8, D_L09_00166F40 - 64, matrix);
        multiply_matrix(packet + 24, D_L09_00166F40 - 32, matrix);
        packet[40] = 0x8000;
        packet[41] = 0x303ec000;
        packet[42] = 0x412;
        *(float *)(packet + 43) = current[0x210 / 4];
        packet_end = packet + 44;
        qcopy(packet_end, current + 0x190 / 4);
        packet_end = packet + 48;
        qcopy(packet_end, current + 0x1a0 / 4);
        *(float *)(packet + 52) = current[0x22c / 4];
        {
            float saved0 = current[0x228 / 4];
            packet[56] = 0x03000000;
            packet[57] = 0x020001d2;
            *(float *)(packet + 53) = saved0;
        }
        packet[58] = 0x15000000;
        packet[54] = 0;
        packet[55] = 0;
        packet[59] = 0;
        packet_end = packet + 60;
        {
            int *words = PACKET_CURSOR;
            *words |= (((int)packet_end - (int)words) >> 4) - 1;
        }
        D_L09_001611C0 = packet_end;
        packet[60] = 0x30000003;
    }
    PACKET_CURSOR[1] = (int)scaled_entries_gs_setup_packet;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000003;
    {
        int *packet = PACKET_CURSOR + 4;
        D_L09_001611C0 = packet;
        packet[0] = 0x10000008;
    }
    PACKET_CURSOR[1] = 0;
    PACKET_CURSOR[2] = 0;
    PACKET_CURSOR[3] = 0x50000008;
    packet = PACKET_CURSOR;
    words = (u64 *)(packet + 4);
    D_L09_001611C0 = packet + 4;
    color = (unsigned char)data[0x30] | (unsigned char)data[0x31] << 8 |
            (unsigned char)data[0x32] << 16;
    /* Retail reads the mode after the packet cursor stores. */
    if (*(volatile int *)&textured_grid_view_settings_mode == 1)
        color = (long)(*(int *)(current + 0x230 / 4) | (*(int *)(current + 0x234 / 4) << 8) |
                       (*(int *)(current + 0x238 / 4) << 16));
    {
        u64 alpha = (unsigned char)data[0x3f];
        u64 blend;
        words[0] = 0x7000000000008001ULL;
        words[1] = 0xeeeeeeeeULL;
        blend = (unsigned char)data[0x3f];
        words[3] = 0x42;
        /* Retail stores packed alpha before reading D_L09_001612F0. */
        *(volatile u64 *)&words[2] = (blend << 32) | 100;
        alpha = alpha <= 0x60U ? 0x5370b : 0x5360a;
        blend = alpha;
        words[4] = *(volatile u64 *)&scaled_entries_texture_register_value;
        scratch_color = 0x757c8e;
        words[5] = 6;
        words[6] = (long)color;
        words[7] = 0x3d;
        words[8] = 0;
        words[9] = 8;
        words[10] = 0x50000;
        words[11] = 0x47;
        words[12] = 0xff9000000260ULL;
        words[13] = 0x14;
        words[14] = blend;
        words[15] = 0x47;
        D_L09_001611C0 = (int *)(words + 16);
    }
    scratch = (int *)0x70000000;
    for (i = 255; i >= 0; i--)
        scratch[i] = scratch_color;
    i = 0;
    if (count > 0)
        do {
            char *entry;
            int   visible;
            entry_offset = i * 32;
            entry = (char *)((int)list + entry_offset);
            visible = fast_bsphere_check(entry, 256.0f);
            if (visible != -1) {
                int    float_pair_count = *(int *)(entry + 0x1c);
                float *source = *(float **)(entry + 0x14);
                float *destination = (float *)0x70001000;
                if (float_pair_count > 0) {
                    j = float_pair_count;
                    do {
                        *destination++ = *source++ * 0.5f;
                        *destination++ = *source++ * 0.5f;
                    } while (--j != 0);
                }
                FUN_L00_001fde98(float_pair_count,
                                 ((int *)list)[(i << 3) + 4],
                                 0x70000000,
                                 (void *)0x70001000,
                                 visible == 0);
            }
            i++;
        } while (i < count);
    restore_view_after_textured_grid(data);
}

#undef PACKET_CURSOR

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002F0AA0), where it is exact; names translated to the US level program. */
extern char level09_scrolling_strip_entries[] __asm__("D_L09_001F3080");
extern char level09_scaled_surface_entries[] __asm__("D_L09_001F63C0");
extern char level09_textured_surface_draw_settings[] __asm__("D_L09_001FC740");
extern void draw_textured_grid(char *moby) __asm__("FUN_L03_00291918");
extern void build_packet_and_draw_scaled_entries(void *, void *, int) __asm__("FUN_L09_002c1978");
void        draw_level09_textured_surfaces(void) __asm__("FUN_L09_002ef750");

void draw_level09_textured_surfaces(void) {
    int   texture_sequence_numerator = D_L09_0015F5CC;
    int   texture_sequence_step = texture_sequence_numerator / 20;
    float texture_sequence_position = (float)texture_sequence_numerator / 20.0f;
    float texture_sequence_step_float = (float)texture_sequence_step;
    draw_level09_scrolling_textured_strips(level09_scrolling_strip_entries,
                                           0x6C,
                                           texture_sequence_position - texture_sequence_step_float,
                                           (texture_sequence_step & 15) + 0x2C,
                                           ((texture_sequence_step + 1) & 15) + 0x2C);
    draw_textured_grid(level09_textured_surface_draw_settings);
    build_packet_and_draw_scaled_entries(
        level09_textured_surface_draw_settings, level09_scaled_surface_entries, 10);
}

u64 scaled_entries_texture_register_value __asm__("D_L09_001612F0") = 0;
