/* Level 09 moby classes: asteroid, display, platform. */
#include "types.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"

#include "rnc/rendering/dma_tag.h"

extern union PacketCursor l09_packet __asm__("D_L09_001611C0") __attribute__((section(".sdata")));
extern struct GsTextureAllocation gs_texture_allocation __asm__("D_0015EE74") __attribute__((section(".sdata")));
#define PACKET_CURSOR (l09_packet.words)
#define TEXTURE_CURSOR (gs_texture_allocation.cursor)
extern char *texture_vertices __asm__("D_L09_0015F420") __attribute__((section(".sdata")));
extern char D_L09_0016D300[];
extern long D_L09_00161FC0[];
extern long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void FUN_L02_0024fca0(void *, void *, void *, void *, void *, int);
extern void vu1_add_g_sregister(int, long) __asm__("FUN_00233980");

typedef struct {
    char unused[8];
    unsigned short first;
    unsigned short second;
    char tail[4];
} TextureEntry;

/* Queues the GS packet for one texture (tex0 == tex1), or builds the strip between two texture entries and queues it. */
void queue_level09_strip_texture_packets(int first_texture_index, int second_texture_index) __asm__("FUN_L09_0021e560");

void queue_level09_strip_texture_packets(int first_texture_index, int second_texture_index) {
    if (first_texture_index == second_texture_index) {
        int *packet;
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
        int *packet;
        long *words;
        int a_second = ((TextureEntry *)D_L09_0016D300)[first_texture_index].second;
        int a_first = ((TextureEntry *)D_L09_0016D300)[first_texture_index].first;
        int b_second = ((TextureEntry *)D_L09_0016D300)[second_texture_index].second;
        int b_first = ((TextureEntry *)D_L09_0016D300)[second_texture_index].first;

        D_L09_00161FC0[0] = 0x0800000000000400L;
        D_L09_00161FC0[1] = 0;
        FUN_L02_0024fca0(&D_L09_00161FC0[2], texture_vertices + (a_first << 4), texture_vertices + (a_second << 4),
                         texture_vertices + (b_first << 4), texture_vertices + (b_second << 4), 0x1000);
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
        PACKET_CURSOR[1] = (int)D_L09_00161FC0;
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

#define NOT_SDA

#define MACRO_ADDR

/* Draws a list of scrolling-texture strips (water) into the scratchpad. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/drawquad_0021E3E8.c: func_L09_0021E770), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x20];
    short idx[16];
} Strip;

extern char D_L09_0016EAA0[];
extern int *D_L09_001611C0;
extern int D_L09_0015F5CC;
extern int fast_bsphere_check(char *, float) __asm__("FUN_001fa728");
extern void FUN_L01_0021fa98(int, int, int, int, int, int);
extern void queue_level09_strip_texture_packets(int, int) __asm__("FUN_L09_0021e560");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

extern int *D_L09_001611B8[] __attribute__((section(".sdata")));
void draw_level09_scrolling_textured_strips(char *list, int count, float unused, long first_texture_index, long second_texture_index) __asm__("FUN_L09_0021e8c0");

void draw_level09_scrolling_textured_strips(char *list, int count, float unused, long first_texture_index, long second_texture_index) {
    int i;
    int j;
    int strip_vertex_index;
    int *sp;
    int idx;
    float first_scroll_offset;
    float second_scroll_offset;
    queue_level09_strip_texture_packets(first_texture_index, second_texture_index);
    D_L09_001611B8[2][0] = 0x30000009;
    D_L09_001611B8[2][1] = (int)D_L09_0016EAA0;
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
            int strip_vertex_count;
            float *strip_source_pairs;
            float *first_scroll_destination = (float *)0x70001000;
            float *second_scroll_destination = (float *)0x70002000;
            strip_vertex_count = ((Strip *)strip_entry)->idx[j + 1] - ((Strip *)strip_entry)->idx[j];
            strip_source_pairs = (float *)(*(int *)(strip_entry + 0x14) + ((Strip *)strip_entry)->idx[j] * 8);
            for (strip_vertex_index = 0; strip_vertex_index < strip_vertex_count; strip_vertex_index++) {
                float first_source_component = *strip_source_pairs++;
                *second_scroll_destination++ = first_source_component;
                *first_scroll_destination++ = first_source_component;
                *first_scroll_destination++ = *strip_source_pairs + first_scroll_offset;
                *second_scroll_destination++ = *strip_source_pairs++ + second_scroll_offset;
            }
            FUN_L01_0021fa98(strip_vertex_count, *(int *)(strip_entry + 0x10) + ((Strip *)strip_entry)->idx[j] * 12, 0x70000000,
                             0x70001000, 0x70002000, idx == 0);
        }
    }
}
extern void setup_first_context(void) __asm__("FUN_001f2d98");
extern void setup_second_context(void) __asm__("FUN_001f2260");
extern void set_scale_matrix(float, void *) __asm__("FUN_001f9ff8");
extern void setup_offset_vector(float, void *, void *) __asm__("FUN_001f9a68");
extern void vu1_add_data_ref(void *, int) __asm__("FUN_00233830");
extern void transform_packet_data(void *, void *, void *) __asm__("FUN_001fa378");
extern int fast_bsphere_check(char *, float) __asm__("FUN_001fa728");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void restore_view_after_textured_grid(void *) __asm__("FUN_L02_002a46e0");
extern int D_L09_0015F458;
extern int D_L09_0016CDD0;
extern int D_L09_0016CDD8[];
extern int D_L09_0016CD50[];
extern int D_L09_0016CD60[];
extern int D_L09_0016CDF0[];
extern u64 D_L09_001612F0;
extern unsigned short D_L09_0010E800[];
extern int D_L09_0010E810[];
extern int D_L09_001CB470[];
extern char D_L09_00166E40[];
extern char D_L09_00166EC0[];

extern float D_L09_00166F40[];
extern float D_L09_0016CBC0[];
extern float D_L09_001612F8;
extern float D_L09_001612FC;
extern float D_L09_00161300;
extern float D_L09_00161304 __attribute__((section(".sdata")));
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#define PACKET_CURSOR (*(int * volatile *)&D_L09_001611C0)

void build_packet_and_draw_scaled_entries(void *data_ptr, void *list_ptr, int count) __asm__("FUN_L09_002c1978");

void build_packet_and_draw_scaled_entries(void *data_ptr, void *list_ptr, int count) {
    char *data = data_ptr;
    char *list = list_ptr;
    int i;
    int j;
    int entry_offset;
    int *packet;
    u64 *words;
    float matrix[12];
    float offset[4];
    u64 color;
    int *scratch;
    int scratch_color;
    float *old = (float *)0x001612f8;
    float *current = D_L09_0016CBC0;
    float saved0 = current[0x218 / 4];
    float saved1 = current[0x21c / 4];
    float saved2 = current[0x228 / 4];
    float saved3 = current[0x22c / 4];
    D_L09_001612F8 = saved0;
    D_L09_001612FC = saved1;
    D_L09_00161300 = saved2;
    D_L09_00161304 = saved3;
    if (D_L09_0015F458 != 1) {
        current[0x218 / 4] = *(float *)(data + 0x20);
        current[0x21c / 4] = *(float *)(data + 0x24);
        current[0x228 / 4] = *(float *)(data + 0x28);
        current[0x22c / 4] = *(float *)(data + 0x2c);
    }
    setup_first_context();
    setup_second_context();
    set_scale_matrix(1024.0f, matrix);
    setup_offset_vector(-1024.0f, offset, D_L09_00166F40);
    offset[3] = 1.0f;
    vu1_add_data_ref(D_L09_0010E810, D_L09_0010E800[0]);
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
        transform_packet_data(packet + 8, D_L09_00166F40 - 64, matrix);
        transform_packet_data(packet + 24, D_L09_00166F40 - 32, matrix);
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
    PACKET_CURSOR[1] = (int)D_L09_001CB470;
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
    color = (unsigned char)data[0x30] | (unsigned char)data[0x31] << 8 | (unsigned char)data[0x32] << 16;
    /* Retail reads the mode after the packet cursor stores. */
    if (*(volatile int *)&D_L09_0015F458 == 1)
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
        words[4] = *(volatile u64 *)&D_L09_001612F0;
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
    if (count > 0) do {
        char *entry;
        int visible;
        entry_offset = i * 32;
        entry = (char *)((int)list + entry_offset);
        visible = fast_bsphere_check(entry, 256.0f);
        if (visible != -1) {
            int float_pair_count = *(int *)(entry + 0x1c);
            float *source = *(float **)(entry + 0x14);
            float *destination = (float *)0x70001000;
            if (float_pair_count > 0) {
                j = float_pair_count;
                do {
                    *destination++ = *source++ * 0.5f;
                    *destination++ = *source++ * 0.5f;
                } while (--j != 0);
            }
            FUN_L00_001fde98(float_pair_count, ((int *)list)[(i << 3) + 4], 0x70000000,
                              (void *)0x70001000, visible == 0);
        }
        i++;
    } while (i < count);
    restore_view_after_textured_grid(data);
}
#undef PACKET_CURSOR
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002C3690), where it is exact; names translated to the US level program. */


extern char *func_0020D348_m(int);
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

char *spawn_airship_part(struct Moby *rotation_source_moby, char *spawn_reference_position, int spawned_moby_class) __asm__("FUN_L09_002c2500");

char *spawn_airship_part(struct Moby *rotation_source_moby, char *spawn_reference_position, int spawned_moby_class) {
    char *spawned_moby = func_0020D348_m(spawned_moby_class);
    if (spawned_moby) {
        OvlQuad point_position;
        OvlQuad position_offset;
        char *spawned_position = spawned_moby + 0x10;
        char *spawned_vars;
        ((unsigned char *)spawned_moby)[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_vars = *(char **)(spawned_moby + 0x78);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        qcopy(spawned_position, spawn_reference_position);
        qcopy(spawned_vars + 0x10, spawn_reference_position);
        *(char **)(spawned_vars + 0x2C) = rotation_source_moby;
        FUN_L00_00250df8(spawned_moby);
        moby_point_position(spawned_moby, 0, &point_position);
        subtract_vector_xyz(&position_offset, spawn_reference_position, &point_position);
        transform_vector_by_basis(&position_offset, &position_offset, &rotation_source_moby->rot_row0);
        add_vector_xyz(spawned_position, spawn_reference_position, &position_offset);
        qcopy(spawned_moby + 0x40, &rotation_source_moby->rot);
        *(OvlQuad *)(spawned_moby + 0xC0) = *(OvlQuad *)&rotation_source_moby->rot_row0;
        *(OvlQuad *)(spawned_moby + 0xD0) = *(OvlQuad *)&rotation_source_moby->rot_row1;
        *(OvlQuad *)(spawned_moby + 0xE0) = *(OvlQuad *)&rotation_source_moby->rot_row2;
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002C37A0), where it is exact; names translated to the US level program. */

extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");

char *find_nearest_moby_by_class(void *search_position, int type, float *nearest_distance_out) __asm__("FUN_L09_002c2610");

char *find_nearest_moby_by_class(void *search_position, int type, float *nearest_distance_out) {
    char *best = 0;
    float bestd = 100000.0f;
    char *candidate_moby;
    for (candidate_moby = D_L09_0015FFE4; candidate_moby != 0; candidate_moby = *(char **)(candidate_moby + 0x28)) {
        if (*(short *)(candidate_moby + 0xA6) == type) {
            float candidate_distance = fast_vec_dist(search_position, candidate_moby + 0x10);
            if (candidate_distance < bestd) {
                bestd = candidate_distance;
                best = candidate_moby;
            }
        }
    }
    if (nearest_distance_out != 0) {
        *nearest_distance_out = bestd;
    }
    return best;
}
/* spawns a debris moby with randomized velocity toward a point */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EB808), where it is exact; names translated to the US level program. */

extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float D_L09_00166F40[];
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");

char *spawn_fading_asteroid_debris(struct Moby *owner, float *inherited_velocity) __asm__("FUN_L09_002ea528");

char *spawn_fading_asteroid_debris(struct Moby *owner, float *inherited_velocity) {
    char *moby = func_0020D348_m(random_integer_below(4) + 0x107);
    if (moby != 0) {
        char *spawned_moby_vars = *(char **)(moby + 0x78);
        char *owner_vars = (char *)owner->pvars;
        char *dv;
        float reference_position_distance;
        *(float *)spawned_moby_vars = random_float_between(-2.0f, 2.0f);
        *(float *)(spawned_moby_vars + 4) = random_float_between(-2.0f, 2.0f);
        *(float *)(spawned_moby_vars + 8) = random_float_between(-2.0f, 2.0f);
        dv = spawned_moby_vars + 0x20;
        add_vector_xyz(dv, spawned_moby_vars, owner_vars);
        normalize_vector_xyz(spawned_moby_vars, spawned_moby_vars, frame_time * 50.0f);
        add_vector_xyz(spawned_moby_vars, spawned_moby_vars, inherited_velocity);
        scale_vector_xyz(spawned_moby_vars, spawned_moby_vars, frame_scale * -0.7f + 1.0f);
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        *(int *)(spawned_moby_vars + 0x4C) = 2;
        moby[0x23] = 0x7F;
        {
            float spawned_scale = owner->scale * random_float_between(0.125f, 0.25f);
            *(float *)(moby + 0x2C) = spawned_scale;
            *(float *)(spawned_moby_vars + 0x50) = spawned_scale;
        }
        *(float *)(spawned_moby_vars + 0x40) = random_float_between(-0.05235988f, 0.05235988f);
        *(float *)(spawned_moby_vars + 0x44) = random_float_between(-0.05235988f, 0.05235988f);
        subtract_vector_xyz(moby + 0x10, dv, D_L09_00166F40);
        reference_position_distance = vector_length_xyz(moby + 0x10);
        if (100.0f < reference_position_distance) {
            normalize_vector_xyz(moby + 0x10, moby + 0x10, 100.0f);
            add_vector_xyz(moby + 0x10, moby + 0x10, D_L09_00166F40);
            *(float *)(moby + 0x2C) = *(float *)(spawned_moby_vars + 0x50) * (100.0f / reference_position_distance);
        } else {
            qcopy(moby + 0x10, dv);
            *(float *)(moby + 0x2C) = *(float *)(spawned_moby_vars + 0x50);
        }
    }
    return moby;
}
/* spawns a debris moby at a random point of a ring above the arena, falling toward the centre */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EBA58), where it is exact; names translated to the US level program. */

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");

char *spawn_falling_asteroid(void) __asm__("FUN_L09_002ea778");

char *spawn_falling_asteroid(void) {
    float target_position[4];
    char *moby = func_0020D348_m(random_integer_below_c(4) + 0x107);
    if (moby != 0) {
        char *spawned_moby_position = moby + 0x10;
        char *moby_vars = *(char **)(moby + 0x78);
        float radius = random_float_between_c(0.0f, 250.0f);
        float angle_radians = random_float_between_c(-3.1415927f, 3.1415927f);
        float reference_position_distance;
        target_position[0] = fast_cos(angle_radians) * radius;
        target_position[1] = fast_sin(angle_radians) * radius;
        target_position[0] += 256.0f;
        target_position[1] += 256.0f;
        target_position[2] = 100.0f;
        *(float *)(moby + 0x10) = 1000.0f;
        *(float *)(moby + 0x14) = 500.0f;
        *(float *)(moby + 0x18) = random_float_between_c(200.0f, 250.0f);
        add_vector_xyz(spawned_moby_position, spawned_moby_position, target_position);
        qcopy(moby_vars, spawned_moby_position);
        *(float *)(moby + 0x2C) *= random_float_between_c(0.5f, 1.0f);
        *(float *)(moby_vars + 0x40) = random_float_between_c(-0.015707964f, 0.015707964f);
        *(float *)(moby_vars + 0x44) = random_float_between_c(-0.015707964f, 0.015707964f);
        target_position[2] = probe_ground_height(target_position, 0, 0.5f);
        if (target_position[2] < 25.0f) target_position[2] = 25.0f;
        qcopy(moby_vars + 0x20, target_position);
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        *(int *)(moby_vars + 0x4C) = 1;
        *(float *)(moby_vars + 0x54) = random_float_between_c(110.0f, 130.0f) * frame_time;
        *(float *)(moby_vars + 0x50) = *(float *)(moby + 0x2C);
        *(float *)moby_vars += random_float_between_c(-100.0f, 100.0f);
        *(float *)(moby_vars + 4) += random_float_between_c(-100.0f, 100.0f);
        subtract_vector_xyz(spawned_moby_position, moby_vars, D_L09_00166F40);
        reference_position_distance = vector_length_xyz(spawned_moby_position);
        if (100.0f < reference_position_distance) {
            normalize_vector_xyz(spawned_moby_position, spawned_moby_position, 100.0f);
            add_vector_xyz(spawned_moby_position, spawned_moby_position, D_L09_00166F40);
            *(float *)(moby + 0x2C) = *(float *)(moby_vars + 0x50) * (100.0f / reference_position_distance);
        } else {
            qcopy(spawned_moby_position, moby_vars);
            *(float *)(moby + 0x2C) = *(float *)(moby_vars + 0x50);
        }
        *(int *)(moby_vars + 0x58) = -1;
        FUN_L00_00250df8(moby);
    }
    return moby;
}
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"

/* Variables of the pebble mobys the l09 rockfall spawner creates (moby->pvars). */
typedef struct {
    f32 vel[3];          /* 0x00: sideways drift */
    f32 speed;           /* 0x0C */
    s32 path;            /* 0x10 */
    s32 unk14;           /* 0x14 */
    s32 count;           /* 0x18: pebbles spawned (on the spawner) */
    u8 pad1C[4];
    Vec4 start;          /* 0x20 */
    Vec4 offset;         /* 0x30 */
    f32 spin_x;          /* 0x40 */
    f32 spin_y;          /* 0x44 */
    s32 unk48;           /* 0x48 */
    u8 pad4C[4];
    struct Moby *owner;  /* 0x50 */
} PebbleVars;

typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 pts[1];         /* 0x10: count + 1 points, pts[-1] is this header */
} PebblePath;

extern PebblePath *D_L09_001B0630_2eaa50[] __asm__("D_L09_001B0630");
extern char D_L09_00166F40_2eaa50[] __asm__("D_L09_00166F40");
extern f32 D_0015ED6C_2eaa50 __asm__("D_0015ED6C");
extern s32 random_integer_below_2eaa50(s32) __asm__("FUN_00213260");
extern f32 random_float_2eaa50(f32, f32) __asm__("FUN_002132a8");
extern struct Moby *create_moby_2eaa50(s32) __asm__("FUN_0020c4f8");
extern void glow_n(struct Moby *, s32, s32, s32) __asm__("FUN_L00_002502f0");
extern void upd_a(struct Moby *) __asm__("FUN_L00_00250df8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("FUN_001f9ad8");

/* Spawns one of four pebble classes at the spawner, scattered sideways off the end of its path. */
struct Moby *FUN_L09_002eaa50(struct Moby *owner) {
    struct Moby *spawned_moby;
    PebbleVars *ovars;
    PebbleVars *vars;
    PebblePath *path;
    f32 lateral_scatter_distance;
    Vec4 path_displacement;
    Vec4 path_scatter_vector;

    spawned_moby = create_moby_2eaa50(random_integer_below_2eaa50(4) + 0x107);
    if (spawned_moby != 0) {
        ovars = (PebbleVars *)owner->pvars;
        vars = (PebbleVars *)spawned_moby->pvars;
        qcopy(&spawned_moby->pos, &owner->pos);
        ovars->count++;
        vars->owner = owner;
        vars->path = ovars->path;
        vars->unk48 = 1;
        vars->unk14 = 0;
        spawned_moby->command = 1;
        spawned_moby->visible = 1;
        spawned_moby->update_dist = 0xFF;
        spawned_moby->draw_dist = 0xFF;
        glow_n(spawned_moby, 0x80, 0x80, 0x80);
        spawned_moby->alpha = 0;
        spawned_moby->scale *= random_float_2eaa50(0.4f, 0.85f);
        vars->spin_x = random_float_2eaa50(-0.0052359877f, 0.0052359877f);
        vars->spin_y = random_float_2eaa50(-0.0052359877f, 0.0052359877f);
        if (vars->path >= 0) {
            path = D_L09_001B0630_2eaa50[vars->path];
            lateral_scatter_distance = random_float_2eaa50(-30.0f, 30.0f);
            subtract_vector_xyz(&path_displacement, &path->pts[path->count - 1], &path->pts[path->count - 2]);
            subtract_vector_xyz(&path_scatter_vector, &path->pts[1], &path->pts[0]);
            cross_vectors_xyz(&path_scatter_vector, &path_scatter_vector, &path_displacement);
            path_scatter_vector.f[2] = 0.0f;
            normalize_vector_xyz(vars, &path_scatter_vector, lateral_scatter_distance);
            subtract_vector_xyz(&path_displacement, &path->pts[path->count - 1], &path->pts[0]);
            cross_vectors_xyz(&path_scatter_vector, &path_scatter_vector, &path_displacement);
            normalize_vector_xyz(&path_scatter_vector, &path_scatter_vector, random_float_2eaa50(0.0f, 5.0f));
            add_vector_xyz(vars, vars, &path_scatter_vector);
            qcopy(&vars->start, &path->pts[0]);
            qcopy(&vars->offset, &ovars->offset);
            add_vector_xyz(&spawned_moby->pos, &path->pts[0], vars);
            add_vector_xyz(&spawned_moby->pos, &spawned_moby->pos, D_L09_00166F40_2eaa50);
            subtract_vector_xyz(&spawned_moby->pos, &spawned_moby->pos, &vars->offset);
            vars->speed = lateral_scatter_distance / 20.0f + 10.0f;
            spawned_moby->scale *= lateral_scatter_distance / 100.0f + 1.0f;
            vars->speed *= D_0015ED6C_2eaa50;
        }
        upd_a(spawned_moby);
    }
    return spawned_moby;
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* ticks a fading drifting moby: decays its velocity, follows it and eases its rotation */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EC010), where it is exact; names translated to the US level program. */

extern float D_L09_00166F40[];
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void delete_moby(struct Obj *obj) __asm__("FUN_0020c828");

void update_fading_asteroid_debris(unsigned char *moby) __asm__("FUN_L09_002ead30");

void update_fading_asteroid_debris(unsigned char *moby) {
    if (moby[0x23] < 3) {
        delete_moby(moby);
    } else {
        char *moby_vars;
        float reference_position_distance, updated_rotation_x;
        moby[0x23] -= 2;
        moby_vars = *(char **)(moby + 0x78);
        fast_vec_scale(moby_vars, moby_vars, frame_scale * -0.0199999809f + 1.0f);
        *(float *)(moby_vars + 8) -= frame_time_sq * 20.0f;
        fast_vec_add(moby_vars + 0x20, moby_vars + 0x20, moby_vars);
        fast_vec_sub(moby + 0x10, moby_vars + 0x20, D_L09_00166F40);
        reference_position_distance = fast_vec_length(moby + 0x10);
        if (reference_position_distance > 100.0f) {
            fast_vec_normalize(moby + 0x10, moby + 0x10, 100.0f);
            fast_vec_add(moby + 0x10, moby + 0x10, D_L09_00166F40);
            *(float *)(moby + 0x2C) = *(float *)(moby_vars + 0x50) * (100.0f / reference_position_distance);
        } else {
            qcopy(moby + 0x10, moby_vars + 0x20);
            *(float *)(moby + 0x2C) = *(float *)(moby_vars + 0x50);
        }
        updated_rotation_x = fast_add_rotations(*(float *)(moby + 0x40), *(float *)(moby_vars + 0x40));
        *(float *)(moby + 0x40) = updated_rotation_x;
        *(float *)(moby + 0x44) = fast_add_rotations(updated_rotation_x, *(float *)(moby_vars + 0x44));
    }
}
#include "sda.h"

/* ticks a falling rock: moves it toward its target, sheds sparks and debris on impact, shakes the camera and removes it */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EC188), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct { int v[6]; } V6_2EC188;

typedef struct {
    float pos[4];
    char pad10[0x10];
    float target[4];
    char pad30[0x10];
    float f40;
    float f44;
    char pad48[8];
    float f50;
    float f54;
    int f58;
} Data_2EC188;

typedef struct { char pad0[0x2084]; int f2084; } Hero_2EC188;

typedef struct { char pad0[0x140]; float pos[4]; char pad150[0x10]; float f160; int f164; int f168; } Cam_2EC188;

char *FUN_L09_002ea528_c(struct Moby *owner, float *vec) __asm__("FUN_L09_002ea528");
V6_2EC188 D_L09_00208B00 = {{0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}};
V6_2EC188 D_L09_00208B18 = {{0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}};
extern char *FUN_L09_002efe48_c(void *, void *, void *, float, float) __asm__("FUN_L09_002efe48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("FUN_001fa580");
extern f32 random_float_between_c2(f32, f32) __asm__("FUN_002132a8");
extern float D_L09_00173FE8 __attribute__((section(".data")));
extern float D_0015ED6C __attribute__((section(".sdata")));
extern float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern int FUN_001efa68(void *, void *, int, int, int);
extern int FUN_L00_00257b90(int, int);
extern int trooper_play_sound(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern int vendor_volume(int, int) __asm__("FUN_L01_002a1968");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below_c2(s32) __asm__("FUN_00213260");
extern unsigned char D_0013F350[];
extern void FUN_L00_0026a9f0(void *, void *, int, int, int, int, int, int, float, float);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");

extern f32 frame_scale_macro __asm__("D_0015ED60") __attribute__((section(".sdata")));
void update_falling_asteroid(Moby *falling_rock) __asm__("FUN_L09_002eaea8");

void update_falling_asteroid(Moby *falling_rock) {
    float movement_step[4];
    float collision_probe_offset[4];
    float position_or_movement_step[4];
    float offset_position[4];
    Data_2EC188 *moby_vars = (*(Data_2EC188 **)&falling_rock->pvars);
    float reference_position_distance;
    float updated_rotation_x;

    subtract_vector_xyz(movement_step, moby_vars->target, moby_vars->pos);
    normalize_vector_xyz(collision_probe_offset, movement_step, 25.0f);
    normalize_vector_xyz(movement_step, movement_step, moby_vars->f54);
    add_vector_xyz(moby_vars->pos, moby_vars->pos, movement_step);
    subtract_vector_xyz(((f32 *)&falling_rock->pos), moby_vars->pos, D_L09_00166F40);
    reference_position_distance = vector_length_xyz(((f32 *)&falling_rock->pos));
    if (reference_position_distance > 100.0f && moby_vars->pos[2] > 27.0f) {
        normalize_vector_xyz(((f32 *)&falling_rock->pos), ((f32 *)&falling_rock->pos), 100.0f);
        add_vector_xyz(((f32 *)&falling_rock->pos), ((f32 *)&falling_rock->pos), D_L09_00166F40);
        falling_rock->scale = moby_vars->f50 * (100.0f / reference_position_distance);
    } else {
        qcopy(((f32 *)&falling_rock->pos), moby_vars->pos);
        falling_rock->scale = moby_vars->f50;
    }
    updated_rotation_x = fast_add_rotations_c(falling_rock->rot.x, moby_vars->f40);
    falling_rock->rot.x = updated_rotation_x;
    falling_rock->rot.y = fast_add_rotations_c(updated_rotation_x, moby_vars->f44);
    if (falling_rock->state == 0) {
        if (moby_vars->pos[2] < 100.0f && moby_vars->pos[0] < 500.0f && moby_vars->pos[1] < 500.0f) {
            position_or_movement_step[0] = random_float_between_c2(-5.0f, 5.0f);
            position_or_movement_step[1] = random_float_between_c2(-5.0f, 5.0f);
            position_or_movement_step[2] = random_float_between_c2(-5.0f, 5.0f);
            add_vector_xyz(position_or_movement_step, position_or_movement_step, moby_vars->pos);
            add_vector_xyz(offset_position, position_or_movement_step, collision_probe_offset);
            if (FUN_001efa68(position_or_movement_step, offset_position, 4, (int)falling_rock, 0) != 0 && D_L09_00173FE8 > 26.5f) {
                float scaled_movement_step[4];
                V6_2EC188 A;
                V6_2EC188 B;
                float extra;
                int i;
                int h;
                extra = 0.0f;
                if (reference_position_distance < 7.0f) extra = 7.0f - reference_position_distance;
                scale_vector_xyz_c(scaled_movement_step, movement_step, 0.1f);
                for (i = 7; i >= 0; i--) {
                    float s;
                    float sc;
                    int *pa;
                    int *pb;
                    int a;
                    int b;
                    s = random_float_between_c2(8.0f, 10.0f);
                    A = D_L09_00208B00;
                    B = D_L09_00208B18;
                    sc = s * D_0015ED6C - extra * D_0015ED6C;
                    pa = &A.v[random_integer_below_c2(6)];
                    pb = &B.v[random_integer_below_c2(6)];
                    {
                        int lo = scale_game_frames(15), hi = scale_game_frames(20);
                        a = FUN_L00_00257b90(lo, hi);
                    }
                    {
                        int lo = scale_game_frames(25), hi = scale_game_frames(30);
                        b = FUN_L00_00257b90(lo, hi);
                    }
                    FUN_L00_0026a9f0(moby_vars->pos, scaled_movement_step, *pa, *pb, a, b, 0, 0, 400000.0f, sc);
                }
                for (i = 9; i >= 0; i--) {
                    FUN_L09_002ea528_c((char *)falling_rock, movement_step);
                }
                moby_vars->f58 = h = trooper_play_sound(6, 0, (int)falling_rock, 0xC1);
                if (h != 0) {
                    vendor_volume(h, 0x800);
                }
                if (((Hero_2EC188 *)(D_0013F350))->f2084 != 0x32) {
                    float dist = distance_xyz(moby_vars->pos, D_L09_00166F40);
                    if (dist < 15.0f) {
                        Cam_2EC188 *c = (Cam_2EC188 *)((char *)D_L09_00166F40 - 0x140);
                        c->f160 = (64.0f - dist) * 0.01f;
                        c->f168 = scale_game_frames(10);
                    }
                }
                falling_rock->state++;
                return;
            }
        }
        if (moby_vars->target[2] == 25.0f) {
            if (moby_vars->pos[2] < 27.0f) {
                int j;
                qcopy(position_or_movement_step, moby_vars->pos);
                position_or_movement_step[2] = 25.0f;
                for (j = 4; j >= 0; j--) {
                    FUN_L09_002efe48_c(falling_rock, position_or_movement_step, movement_step, moby_vars->f50 * 4.0f, moby_vars->f50 * 12.0f);
                }
                moby_vars->f54 = moby_vars->f54 * (frame_scale_macro * -0.100000024f + 1.0f);
                if (moby_vars->f58 <= 0) {
                    moby_vars->f58 = trooper_play_sound(0, 0, (int)falling_rock, 0x107);
                }
            }
        }
        if (moby_vars->pos[2] < moby_vars->target[2]) {
            if (moby_vars->target[2] > 26.5f) {
                V6_2EC188 A;
                V6_2EC188 B;
                float extra;
                int i;
                int h;
                extra = 0.0f;
                if (reference_position_distance < 7.0f) extra = 7.0f - reference_position_distance;
                scale_vector_xyz_c(position_or_movement_step, movement_step, 0.5f);
                for (i = 0; i < 8; i++) {
                    float s;
                    float sc;
                    int *pa;
                    int *pb;
                    int a;
                    int b;
                    s = random_float_between_c2(8.0f, 10.0f);
                    A = D_L09_00208B00;
                    B = D_L09_00208B18;
                    sc = s * D_0015ED6C - extra * D_0015ED6C;
                    offset_position[0] = random_float_between_c2(-3.0f, 3.0f);
                    offset_position[1] = random_float_between_c2(-3.0f, 3.0f);
                    offset_position[2] = random_float_between_c2(-3.0f, 3.0f);
                    add_vector_xyz(offset_position, offset_position, moby_vars->pos);
                    pa = &A.v[random_integer_below_c2(6)];
                    pb = &B.v[random_integer_below_c2(6)];
                    {
                        int lo = scale_game_frames(15), hi = scale_game_frames(20);
                        a = FUN_L00_00257b90(lo, hi);
                    }
                    {
                        int lo = scale_game_frames(25), hi = scale_game_frames(30);
                        b = FUN_L00_00257b90(lo, hi);
                    }
                    FUN_L00_0026a9f0(offset_position, position_or_movement_step, *pa, *pb, a, b, 0, 0, 1200000.0f, sc);
                }
                for (i = 9; i >= 0; i--) {
                    FUN_L09_002ea528_c((char *)falling_rock, movement_step);
                }
                moby_vars->f58 = h = trooper_play_sound(6, 0, (int)falling_rock, 0xC1);
                if (h != 0) {
                    vendor_volume(h, 0x800);
                }
                if (((Hero_2EC188 *)(D_0013F350))->f2084 != 0x32) {
                    float dist = distance_xyz(moby_vars->pos, D_L09_00166F40);
                    if (dist < 15.0f) {
                        Cam_2EC188 *c = (Cam_2EC188 *)((char *)D_L09_00166F40 - 0x140);
                        c->f160 = (64.0f - dist) * 0.01f;
                        c->f168 = scale_game_frames(10);
                    }
                }
            }
            falling_rock->state++;
        }
    } else {
        falling_rock->state++;
        if (moby_vars->target[2] == 25.0f) {
            int j;
            falling_rock->draw_dist = 0;
            falling_rock->visible = 0;
            qcopy(position_or_movement_step, moby_vars->pos);
            position_or_movement_step[2] = 25.0f;
            for (j = 1; j >= 0; j--) {
                FUN_L09_002efe48_c(falling_rock, position_or_movement_step, movement_step, moby_vars->f50 * 4.0f, moby_vars->f50 * 12.0f);
            }
            if (falling_rock->state > scale_game_frames(10)) {
                mark_moby_for_removal(falling_rock);
            }
        } else {
            V6_2EC188 A;
            V6_2EC188 B;
            float extra;
            int k;
            extra = 0.0f;
            if (reference_position_distance < 7.0f) extra = 7.0f - reference_position_distance;
            scale_vector_xyz_c(position_or_movement_step, movement_step, 0.25f);
            for (k = 0; k < 3; k++) {
                float s;
                float sc;
                int *pa;
                int *pb;
                int a;
                int b;
                s = random_float_between_c2(8.0f, 10.0f);
                A = D_L09_00208B00;
                B = D_L09_00208B18;
                sc = s * D_0015ED6C - extra * D_0015ED6C;
                offset_position[0] = random_float_between_c2(-1.0f, 1.0f);
                offset_position[1] = random_float_between_c2(-1.0f, 1.0f);
                offset_position[2] = random_float_between_c2(-1.0f, 1.0f);
                add_vector_xyz(offset_position, offset_position, moby_vars->pos);
                pa = &A.v[random_integer_below_c2(6)];
                pb = &B.v[random_integer_below_c2(6)];
                {
                    int lo = scale_game_frames(15), hi = scale_game_frames(20);
                    a = FUN_L00_00257b90(lo, hi);
                }
                {
                    int lo = scale_game_frames(25), hi = scale_game_frames(30);
                    b = FUN_L00_00257b90(lo, hi);
                }
                FUN_L00_0026a9f0(offset_position, position_or_movement_step, *pa, *pb, a, b, 0, 0, 1200000.0f, sc);
            }
            if (falling_rock->state >= 3) {
                mark_moby_for_removal(falling_rock);
            }
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002ECC50), where it is exact; names translated to the US level program. */

typedef struct {
    float pos[3];              /* 0x00 */
    float speed;               /* 0x0C */
    int path;                  /* 0x10 */
    int node;                  /* 0x14 */
    int count;                 /* 0x18 */
    int timer;                 /* 0x1C */
    float target[4];           /* 0x20 */
    float centre[4];           /* 0x30 */
    float f40;                 /* 0x40 */
    float f44;                 /* 0x44 */
    int init;                  /* 0x48 */
    int mode;                  /* 0x4C */
    struct Moby *owner;        /* 0x50 */
} Data_2ECC50;

typedef struct {
    int n;
    int pad[3];
    float pts[1][4];           /* 0x10 */
} Path_2ECC50;

char *FUN_L09_002ea778_c(void) __asm__("FUN_L09_002ea778");
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 random_float_between_c3(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C_c __asm__("D_0015ED6C");
extern float vector_length_xyz_c3(void *) __asm__("FUN_001f9af0");
extern int FUN_L09_002eaea8_c() __asm__("FUN_L09_002eaea8");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern s32 random_integer_below_c3(s32) __asm__("func_00213260");
extern void update_fading_asteroid_debris(unsigned char *m) __asm__("FUN_L09_002ead30");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
extern void scale_vector_xyz_c2(void *, void *, float) __asm__("FUN_001f9a68");
extern void normalize_vector_xyz_c3(void *, void *, float) __asm__("FUN_001f9bf8");
s32 compute_advanced_moby_path_position(struct Moby *m, void *path, void *out, s32 i, s32 dir, f32 dist) __asm__("FUN_L09_00294ae0");
struct Moby *FUN_L09_002eaa50_c(struct Moby *owner) __asm__("FUN_L09_002eaa50");
extern Path_2ECC50 *D_L09_001B0930_2ECC50[] __asm__("D_L09_001B0630");
extern struct Moby *func_L09_002EBD30_2ECC50(void *m) __asm__("FUN_L09_002eaa50");
extern struct Moby *func_L09_002EBA58_2ECC50(void) __asm__("FUN_L09_002ea778");
extern int func_L09_00295880_2ECC50(void *, void *, void *, int, int, float) __asm__("FUN_L09_00294ae0");
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");

void asteroid_update(struct Moby *m) __asm__("FUN_L09_002eb970");
void asteroid_update(struct Moby *asteroid_moby) {
    Data_2ECC50 *moby_vars = (*(Data_2ECC50 **)&asteroid_moby->pvars);
    Path_2ECC50 *path;
    float displacement_vector[4];
    float endpoint_offset[4];
    float endpoint_segment_length;
    float dot;
    int i;

    if (moby_vars->mode == 2) {
        update_fading_asteroid_debris((unsigned char *)asteroid_moby);
        return;
    }
    if (moby_vars->mode == 1) {
        FUN_L09_002eaea8_c((void *)asteroid_moby);
        return;
    }
    if (asteroid_moby->command == 0 && moby_vars->init == 0) {
        moby_vars->speed = D_0015ED6C_c * 10.0f;
        moby_vars->centre[0] = moby_vars->centre[1] = moby_vars->centre[2] = 0.0f;
        if (moby_vars->path >= 0) {
            Path_2ECC50 *ring = D_L09_001B0930_2ECC50[moby_vars->path];
            for (i = 0; i < ring->n; i++) {
                add_vector_xyz(moby_vars->centre, moby_vars->centre, ring->pts[i]);
            }
            scale_vector_xyz_c2(moby_vars->centre, moby_vars->centre, 1.0f / (float)ring->n);
            moby_vars->centre[2] = ring->pts[0][2] - 10.0f;
            for (i = 0; i < 23; i++) {
                struct Moby *q = func_L09_002EBA58_2ECC50();
                if (q != 0) {
                    Data_2ECC50 *e = (*(Data_2ECC50 **)&q->pvars);
                    subtract_vector_xyz(displacement_vector, e->target, e);
                    normalize_vector_xyz_c3(displacement_vector, displacement_vector, random_float_between_c3(0.0f, vector_length_xyz_c3(displacement_vector) - 32.0f));
                    add_vector_xyz(e, e, displacement_vector);
                }
            }
            for (i = 0; i < 50; i++) {
                struct Moby *q = func_L09_002EBD30_2ECC50(asteroid_moby);
                if (q != 0) {
                    Data_2ECC50 *e = (*(Data_2ECC50 **)&q->pvars);
                    int n = random_integer_below_c3(ring->n - 2);
                    float segment_fraction = random_float_between_c3(0.0f, 1.0f);
                    e->node = n;
                    subtract_vector_xyz(displacement_vector, ring->pts[n + 1], ring->pts[n]);
                    scale_vector_xyz_c2(displacement_vector, displacement_vector, segment_fraction);
                    add_vector_xyz(e->target, displacement_vector, ring->pts[n]);
                    qcopy(((f32 *)&q->pos), e->target);
                    add_vector_xyz(((f32 *)&q->pos), ring->pts[0], e);
                    add_vector_xyz(((f32 *)&q->pos), ((f32 *)&q->pos), D_L09_00166F40);
                    subtract_vector_xyz(((f32 *)&q->pos), ((f32 *)&q->pos), e->centre);
                }
            }
        }
        moby_vars->init = 1;
    }
    if (asteroid_moby->command == 0) {
        if (random_integer_below_c3(scale_game_frames(40)) == 0) {
            func_L09_002EBA58_2ECC50();
        }
    }
    asteroid_moby->update_dist = 0xFF;
    asteroid_moby->draw_dist = 0xFF;
    if (moby_vars->path < 0) {
        return;
    }
    qcopy_nc(((f32 *)&asteroid_moby->pos), moby_vars->target);
    path = D_L09_001B0930_2ECC50[moby_vars->path];
    if (asteroid_moby->command == 0 && moby_vars->count < 50 && fast_dec_timer(&moby_vars->timer)) {
        moby_vars->timer = scale_game_frames(15) + random_integer_below_c3(scale_game_frames(30));
        func_L09_002EBD30_2ECC50(asteroid_moby);
    }
    asteroid_moby->rot.x = fast_add_rotations_c2(asteroid_moby->rot.x, moby_vars->f40);
    asteroid_moby->rot.y = fast_add_rotations_c2(asteroid_moby->rot.y, moby_vars->f44);
    moby_vars->node = func_L09_00295880_2ECC50(asteroid_moby, path, ((f32 *)&asteroid_moby->pos), moby_vars->node, 1, moby_vars->speed);
    if (moby_vars->node == path->n - 1) {
        if (asteroid_moby->command != 0) {
            (*(Data_2ECC50 **)&(*(Data_2ECC50 **)&asteroid_moby->pvars)->owner->pvars)->count--;
            mark_moby_for_removal_c2(asteroid_moby);
        } else {
            moby_vars->node = 0;
            qcopy(((f32 *)&asteroid_moby->pos), path->pts[0]);
        }
    }
    if (moby_vars->node == 0) {
        subtract_vector_xyz(displacement_vector, path->pts[1], path->pts[0]);
        endpoint_segment_length = vector_length_xyz_c3(displacement_vector);
        normalize_vector_xyz_c3(displacement_vector, displacement_vector, 1.0f);
        subtract_vector_xyz(endpoint_offset, ((f32 *)&asteroid_moby->pos), path->pts[0]);
        dot = fast_vec_dot(endpoint_offset, displacement_vector);
        if (dot < 0.0f) {
            dot = 0.0f;
        }
        asteroid_moby->alpha = (int)(dot * 128.0f / endpoint_segment_length);
        if (asteroid_moby->alpha > 0x80) {
            asteroid_moby->alpha = 0x80;
        }
    } else if (moby_vars->node == path->n - 2) {
        subtract_vector_xyz(displacement_vector, path->pts[path->n - 2], path->pts[path->n - 1]);
        endpoint_segment_length = vector_length_xyz_c3(displacement_vector);
        normalize_vector_xyz_c3(displacement_vector, displacement_vector, 1.0f);
        subtract_vector_xyz(endpoint_offset, ((f32 *)&asteroid_moby->pos), path->pts[path->n - 1]);
        dot = fast_vec_dot(endpoint_offset, displacement_vector);
        if (dot < 0.0f) {
            dot = 0.0f;
        }
        asteroid_moby->alpha = (int)(dot * 128.0f / endpoint_segment_length);
        if (asteroid_moby->alpha > 0x80) {
            asteroid_moby->alpha = 0x80;
        }
    } else {
        asteroid_moby->alpha = 0x80;
    }
    qcopy(moby_vars->target, ((f32 *)&asteroid_moby->pos));
    add_vector_xyz(((f32 *)&asteroid_moby->pos), ((f32 *)&asteroid_moby->pos), moby_vars);
    add_vector_xyz(((f32 *)&asteroid_moby->pos), ((f32 *)&asteroid_moby->pos), D_L09_00166F40);
    subtract_vector_xyz(((f32 *)&asteroid_moby->pos), ((f32 *)&asteroid_moby->pos), moby_vars->centre);
}
/* spawns a moby of the given class at a source moby's position */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EEEB0), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern s32 rand();
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_0025d1b8(void *);

char *spawn_breakable_stack_replacement(char *source_moby, int spawned_moby_class) __asm__("FUN_L09_002edb60");

char *spawn_breakable_stack_replacement(char *source_moby, int spawned_moby_class) {
    char *moby = func_0020D348_m(spawned_moby_class);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        qcopy(moby + 0x10, source_moby + 0x10);
        qcopy(moby + 0x40, source_moby + 0x40);
        moby[0xBC] = scale_game_frames(10);
        FUN_L00_00250df8(moby);
        FUN_L00_0025d1b8(moby);
        *(unsigned short *)(moby + 0x34) = *(unsigned short *)(source_moby + 0x34);
        if (spawned_moby_class == 0x514) {
            if (rand() & 1) {
                *(unsigned short *)(moby + 0x34) |= 0x8000;
            }
        }
    }
    return moby;
}
extern u8 D_0014C050_edc30[] __asm__("D_0014C050");
extern float D_L09_00166F40_edc30[] __asm__("D_L09_00166F40");
extern float frame_time_edc30 __asm__("D_0015ED6C");
extern void mark_moby_for_removal_edc30(struct Moby *) __asm__("FUN_0020c828");
extern char *find_moby_hit_edc30(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern int FUN_L00_001fefc8(u8 *);
extern void FUN_L00_00257470_edc30(struct Moby *, int, int) __asm__("FUN_L00_00257470");
extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern int allocate_target_entry_edc30(int, int, struct Moby *) __asm__("FUN_0022da68");
extern void subtract_vector_edc30(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_edc30(void *, void *, void *) __asm__("FUN_001f9a10");
extern void set_vector_length_edc30(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_vector_edc30(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector_edc30(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float vector_length_edc30(void *) __asm__("FUN_001f9af0");
extern float FUN_001f99c0_edc30(float) __asm__("FUN_001f99c0");
extern float random_float_between_edc30(float, float) __asm__("FUN_002132a8");
extern int random_integer_below_edc30(int) __asm__("FUN_00213260");
extern int scale_game_frames_edc30(int) __asm__("FUN_001f96f8");
extern struct Moby *spawn_tumbling_debris(struct Moby *, Vec4 *, Vec4 *, s32, f32) __asm__("FUN_L09_002ef868");
extern void FUN_L00_0025f090_edc30(struct Moby *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0025e450_edc30(struct Moby *, void *, void *, float, float, int, int, int, float, float, float,
                                   float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

/* Level 9 breakable stack: when hit, swaps in the next damaged class (0x512 -> 0x513 -> 0x514), bursts debris and smoke and removes itself once gone. */
void display_update(struct Moby *moby) __asm__("FUN_L09_002edc30");
void display_update(struct Moby *moby) {
    enum {
        BREAKABLE_STACK_ANTEPENULTIMATE_CLASS = 0x512,
        BREAKABLE_STACK_PENULTIMATE_CLASS = 0x513
    };
    float center[4];
    float top[4];
    float push[4];
    float pos[4];
    float debris_offset_or_velocity[4];
    float pull[4];
    char *hit;
    int replaced;
    int next;
    int i;
    float spread;
    float falloff;
    if (moby->mission != 0xFF && D_0014C050_edc30[moby->mission + current_level_index * 16] == 0xFF) {
        mark_moby_for_removal_edc30(moby);
        return;
    }
    replaced = 0;
    hit = find_moby_hit_edc30(moby, 0x40000, 0);
    if (moby->state == 0) {
        moby->state = 1;
    }
    if (FUN_L00_001fefc8(&moby->command) == 0 || hit == 0) {
        return;
    }
    if (moby->oclass < 0x514) {
        next = 0x512;
        switch (moby->oclass) {
        case BREAKABLE_STACK_ANTEPENULTIMATE_CLASS:
            next = 0x513;
            break;
        case BREAKABLE_STACK_PENULTIMATE_CLASS:
            qcopy(center, &moby->pos);
            moby->pos.z += 4.0f;
            FUN_L00_00257470_edc30(moby, 0, -1);
            qcopy(&moby->pos, center);
            next = 0x514;
            break;
        }
        replaced = spawn_breakable_stack_replacement((char *)moby, next) != 0;
    }
    moby->command = scale_game_frames_edc30(10);
    qcopy(top, &moby->pos);
    top[2] += 1.25f;
    qcopy(center, top);
    add_random_offset_to_vector_xyz(center, 0.4f);
    if (moby->oclass < 0x513) {
        allocate_target_entry_edc30(0, 0, moby);
    } else {
        allocate_target_entry_edc30(0, 0, moby);
    }
    moby->hit_slot = 0xFF;
    subtract_vector_edc30(push, D_L09_00166F40_edc30, center);
    set_vector_length_edc30(push, push, 2.0f);
    spread = 4.0f;
    if (moby->oclass != 0x114) {
        spread = 2.0f;
    }
    for (i = 0; i < 20; i++) {
        *(u128 *)debris_offset_or_velocity = 0;
        debris_offset_or_velocity[0] = random_float_between_edc30(-0.5f, 0.5f);
        debris_offset_or_velocity[1] = random_float_between_edc30(-2.0f, 2.0f);
        debris_offset_or_velocity[2] = random_float_between_edc30(-spread, spread);
        *(u128 *)pos = *(u128 *)debris_offset_or_velocity;
        transform_vector_edc30(pos, pos, &moby->rot_row0);
        add_vector_edc30(pos, pos, top);
        subtract_vector_edc30(debris_offset_or_velocity, pos, center);
        falloff = FUN_001f99c0_edc30(3.0f - vector_length_edc30(debris_offset_or_velocity)) / 3.0f;
        set_vector_length_edc30(pull, hit + 0x10, falloff * falloff * 8.0f);
        add_vector_edc30(debris_offset_or_velocity, debris_offset_or_velocity, pull);
        set_vector_length_edc30(debris_offset_or_velocity, debris_offset_or_velocity, falloff * (random_float_between_edc30(6.0f, 10.0f) * frame_time_edc30));
        add_vector_edc30(pos, pos, push);
        if (moby->oclass == 0x514 || random_integer_below_edc30(4) == 0) {
            spawn_tumbling_debris(moby, (Vec4 *)pos, (Vec4 *)debris_offset_or_velocity, i & 1, random_float_between_edc30(1.0f, 2.0f));
        }
        if (i % 3 == 0) {
            FUN_L00_0025f090_edc30(moby, pos, -1, random_float_between_edc30(1.0f, 1.6f), 0.0f);
        }
    }
    scale_vector_edc30(pos, hit + 0x10, 0.5f);
    add_vector_edc30(center, center, push);
    FUN_L00_0025e450_edc30(moby, hit + 0x10, center, 0.0f, 0.0f, 10, 3, 16, 3.0f, 2.0f, 1.0f, 1.0f, -1, 15.0f, 1, 1,
                           -1, 0);
    if (moby->oclass == 0x514 || replaced) {
        mark_moby_for_removal_edc30(moby);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EF460), where it is exact; names translated to the US level program. */


typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vx;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *FUN_L00_0026dd20(char *pos, char *vel, int c, int d, float f);
extern char *spawn_class_11c_moby(char *owner, void *position, float angle) __asm__("FUN_L09_002ee380");
extern float fast_cos(float);
extern float fast_sin(float);
extern float vector_length_xyz(void *);
extern int FUN_L00_00257b90(int, int);
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void add_vector_xyz(void *, void *, void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void normalize_vector_xyz(void *, void *, float);

void handle_moby_damage_and_destruction(struct Moby *moby) __asm__("FUN_L09_002ee110");

void handle_moby_damage_and_destruction(struct Moby *moby) {
    char *moby_vars_or_random_frames = (char *)moby->pvars;
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    Vx v;
    Vx effect_velocity_or_spawn_position;
    int i;
    if (hit_record != 0) {
        *(float *)moby_vars_or_random_frames = *(float *)moby_vars_or_random_frames - *(float *)(hit_record + 0x2C);
        if (0.0f < *(float *)moby_vars_or_random_frames) {
            moby_vars_or_random_frames[0x47] = 0x46;
            trooper_anim_update(moby, (short *)(moby_vars_or_random_frames + 0x40));
        } else {
            char *pos = &moby->pos;
            v.x = fast_cos(moby->rot.z) * (frame_time * 20.0f);
            v.y = fast_sin(moby->rot.z) * (frame_time * 20.0f);
            v.z = 0.0f;
            for (i = 24; i >= 0; i--) {
                union {
                    OvlQuad q;
                    Vx v;
                } t;
                t.q = 0;
                t.v.x = random_float_between(-1.0f, 1.0f);
                t.v.y = random_float_between(-1.0f, 1.0f);
                t.v.z = random_float_between(-1.0f, 1.0f);
                *(OvlQuad *)&effect_velocity_or_spawn_position = t.q;
                normalize_vector_xyz(&effect_velocity_or_spawn_position, &effect_velocity_or_spawn_position,
                                     vector_length_xyz(&v) * (frame_scale * -0.35000002f + 1.0f));
                add_vector_xyz(&effect_velocity_or_spawn_position, &v, &effect_velocity_or_spawn_position);
                normalize_vector_xyz(&effect_velocity_or_spawn_position, &effect_velocity_or_spawn_position,
                                     random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                moby_vars_or_random_frames = (char *)FUN_L00_00257b90(scale_game_frames(5), scale_game_frames(10));
                FUN_L00_0026dd20(pos, (char *)&effect_velocity_or_spawn_position, 0x5F7F4F2F, (int)moby_vars_or_random_frames,
                                 random_float_between(20000.0f, 50000.0f));
            }
            *(OvlQuad *)&effect_velocity_or_spawn_position = *(OvlQuad *)&moby->pos;
            spawn_class_11c_moby(moby, &effect_velocity_or_spawn_position, moby->rot.z);
            mark_moby_for_removal(moby);
            return;
        }
    }
    moby->hit_slot = 0xFF;
}
typedef unsigned int u128_ee380 __attribute__((mode(TI)));
#include "qcopy.h"
extern void *create_moby_local() __asm__("FUN_0020c4f8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EF6D0), where it is exact; names translated to the US level program. */

extern void FUN_L00_00250df8(void *);

char *spawn_class_11c_moby(char *owner, void *position, float angle) __asm__("FUN_L09_002ee380");

char *spawn_class_11c_moby(char *owner, void *position, float angle) {
    u128_ee380 position_copy;
    void *copy = &position_copy;
    char *moby;
    position_copy = *(u128_ee380 *)position;
    moby = create_moby_local(0x11C);
    if (moby != 0) {
        unsigned char state;
        ((unsigned char *)moby)[0x30] = ((unsigned char *)owner)[0x30];
        state = ((unsigned char *)owner)[0x30];
        moby[0x31] = 1;
        *(short *)(moby + 0x32) = state;
        qcopy(moby + 0x10, copy);
        qcopy(moby + 0x40, owner + 0x40);
        *(float *)(moby + 0x48) = angle;
        *(long *)(moby + 0x38) = *(long *)(owner + 0x38);
        FUN_L00_00250df8(moby);
    }
    return moby;
}
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/quad.h"

extern char D_L09_00166F40_2ef430[] __asm__("D_L09_00166F40");
extern float D_0015ED6C_2ef430 __asm__("D_0015ED6C");
extern float D_0015ED70_2ef430 __asm__("D_0015ED70");
extern f32 random_float_2ef430(f32, f32) __asm__("FUN_002132a8");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_add_rotations_2ef430(f32, f32) __asm__("FUN_001fa580");
extern s32 scale_game_frames_2ef430(s32) __asm__("FUN_001f96f8");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void *spawn_spark_2ef430(void *, void *, int, int, float) __asm__("FUN_L00_0026dd20");
extern void reflect_2ef430(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void mark_moby_for_removal_2ef430(void *) __asm__("FUN_0020c828");

/* Per-frame update of a tumbling debris moby: moves and spins it under gravity, bounces it off what it hits, and
   bursts it into two sparks and removes it when its timer runs out or it leaves the area. */
void update_tumbling_debris(struct Moby *moby) __asm__("FUN_L09_002ef430");

void update_tumbling_debris(struct Moby *moby) {
    float old[4];
    float spark_velocity[4];
    float random_spark_vector[4];
    float *debris_vars = (float *)moby->pvars;
    float *pos = (float *)&moby->pos;
    int i;

    qcopy(old, pos);
    add_vector_xyz(pos, pos, debris_vars);
    if (moby->pos.x < 0.0f || moby->pos.y < 0.0f || moby->pos.z < 0.0f ||
        vector_distance(pos, D_L09_00166F40_2ef430) > 64.0f) {
        mark_moby_for_removal_2ef430(moby);
        return;
    }
    moby->rot.z = fast_add_rotations_2ef430(moby->rot.z, debris_vars[4]);
    moby->rot.y = fast_add_rotations_2ef430(moby->rot.y, debris_vars[5]);
    debris_vars[2] -= D_0015ED70_2ef430 * 9.8f;
    if (fast_dec_timer((int *)(debris_vars + 7))) {
        for (i = 0; i < 2; i++) {
            *(OvlQuad *)random_spark_vector = 0;
            random_spark_vector[0] = random_float_2ef430(-1.0f, 1.0f);
            random_spark_vector[1] = random_float_2ef430(-1.0f, 1.0f);
            random_spark_vector[2] = random_float_2ef430(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)random_spark_vector;
            normalize_vector_xyz(spark_velocity, spark_velocity, vector_length_xyz(random_spark_vector) * 0.5f);
            add_vector_xyz(spark_velocity, random_spark_vector, spark_velocity);
            normalize_vector_xyz(spark_velocity, spark_velocity, random_float_2ef430(D_0015ED6C_2ef430 * 3.0f, D_0015ED6C_2ef430 * 6.0f));
            spawn_spark_2ef430(&moby->pos, spark_velocity, 0x7F2F4F6F,
                               random_int_between(scale_game_frames_2ef430(10), scale_game_frames_2ef430(15)),
                               30000.0f);
        }
        mark_moby_for_removal_2ef430(moby);
        return;
    }
    if (collision_line(old, pos, 2, moby, 0)) {
        reflect_2ef430(debris_vars, debris_vars, D_L09_00174000);
        scale_vector_xyz(debris_vars, debris_vars, random_float_2ef430(0.45f, 0.75f));
        if (vector_length_xyz(debris_vars) < D_0015ED70_2ef430 * 10.0f) {
            clear_u64_value(debris_vars);
        }
        normalize_vector_xyz(D_L09_00174000, D_L09_00174000, 0.05f);
        add_vector_xyz(pos, D_L09_00174000 - 0x20, D_L09_00174000);
    }
}
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002F0AA0), where it is exact; names translated to the US level program. */

extern char D_L09_001F3080[];
extern char D_L09_001F63C0[];
extern char D_L09_001FC740[];
extern int D_L09_0015F5CC;
extern void draw_textured_grid(char *moby) __asm__("FUN_L03_00291918");
extern void build_packet_and_draw_scaled_entries(void *, void *, int) __asm__("FUN_L09_002c1978");

void draw_level09_textured_surfaces(void) __asm__("FUN_L09_002ef750");

void draw_level09_textured_surfaces(void) {
    int texture_sequence_numerator = D_L09_0015F5CC;
    int texture_sequence_step = texture_sequence_numerator / 20;
    float texture_sequence_position = (float)texture_sequence_numerator / 20.0f;
    float texture_sequence_step_float = (float)texture_sequence_step;
    draw_level09_scrolling_textured_strips(D_L09_001F3080, 0x6C, texture_sequence_position - texture_sequence_step_float, (texture_sequence_step & 15) + 0x2C, ((texture_sequence_step + 1) & 15) + 0x2C);
    draw_textured_grid(D_L09_001FC740);
    build_packet_and_draw_scaled_entries(D_L09_001FC740, D_L09_001F63C0, 10);
}
struct DebrisVars {
    Vec4 vel;           /* 0x00 */
    f32 spin;           /* 0x10 */
    f32 tumble;         /* 0x14 */
    struct Moby *src;   /* 0x18 */
    s32 life;           /* 0x1C */
};
extern struct Moby *create_moby(s32) __asm__("FUN_0020c4f8");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");

/* spawns a debris moby of class 0x140 + kind at pos with velocity vel and a random spin */
struct Moby *spawn_tumbling_debris(struct Moby *src, Vec4 *pos, Vec4 *vel, s32 kind, f32 scale) {
    Vec4 position_copy;
    Vec4 velocity_copy;
    Vec4 *pp = &position_copy;
    Vec4 *velocity_copy_source = &velocity_copy;
    struct Moby *spawned_moby;
    struct DebrisVars *debris_vars;

    position_copy.q = pos->q;
    velocity_copy.q = vel->q;
    spawned_moby = create_moby(kind + 0x140);
    if (spawned_moby != 0) {
        debris_vars = (struct DebrisVars *)spawned_moby->pvars;
        debris_vars->src = src;
        spawned_moby->update_dist = 0x7F;
        spawned_moby->draw_dist = 0x7F;
        spawned_moby->visible = 1;
        spawned_moby->spawn_frame = src->spawn_frame;
        spawned_moby->rot.x = random_angle_radians();
        spawned_moby->rot.y = random_angle_radians();
        spawned_moby->rot.z = random_angle_radians();
        qcopy(&spawned_moby->pos, pp);
        qcopy(&debris_vars->vel, velocity_copy_source);
        spawned_moby->scale = spawned_moby->pclass->scale * scale;
        debris_vars->spin = random_float_between(-(frame_time * 9.075712f), frame_time * 9.075712f);
        debris_vars->tumble = random_float_between(-(frame_time * 6.2831855f), frame_time * 6.2831855f);
        debris_vars->life = scale_game_frames(FUN_L00_00257b90(0x5A, 0x6E));
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/quad.h"

extern char D_L09_00166F40_2ef9b0[] __asm__("D_L09_00166F40");
extern float D_0015ED6C_2ef9b0 __asm__("D_0015ED6C");
extern float D_0015ED70_2ef9b0 __asm__("D_0015ED70");
extern f32 random_float_2ef9b0(f32, f32) __asm__("FUN_002132a8");
extern f32 fast_add_rotations_2ef9b0(f32, f32) __asm__("FUN_001fa580");
extern s32 scale_game_frames_2ef9b0(s32) __asm__("FUN_001f96f8");
extern void *spawn_spark_2ef9b0(void *, void *, int, int, float) __asm__("FUN_L00_0026dd20");
extern void reflect_2ef9b0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern void mark_moby_for_removal_2ef9b0(void *) __asm__("FUN_0020c828");

/* The heavier tumbling debris: as FUN_L09_002ef430, inside a 2..1021 box, with eight sparks; on a hit it either
   slows down or bounces off, shrinking, with a new random spin. */
void update_shrinking_tumbling_debris(struct Moby *moby) __asm__("FUN_L09_002ef9b0");

void update_shrinking_tumbling_debris(struct Moby *moby) {
    float old[4];
    float spark_velocity[4];
    float random_spark_vector[4];
    float *debris_vars = (float *)moby->pvars;
    float *pos = (float *)&moby->pos;
    int i;

    qcopy(old, pos);
    add_vector_xyz(pos, pos, debris_vars);
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
        moby->pos.z < 2.0f || moby->pos.z > 1021.0f ||
        vector_distance(pos, D_L09_00166F40_2ef9b0) > 64.0f) {
        mark_moby_for_removal_2ef9b0(moby);
        return;
    }
    moby->rot.z = fast_add_rotations_2ef9b0(moby->rot.z, debris_vars[4]);
    moby->rot.y = fast_add_rotations_2ef9b0(moby->rot.y, debris_vars[5]);
    debris_vars[2] -= D_0015ED70_2ef9b0 * 12.74f;
    if (fast_dec_timer((int *)(debris_vars + 7))) {
        for (i = 0; i < 8; i++) {
            *(OvlQuad *)random_spark_vector = 0;
            random_spark_vector[0] = random_float_2ef9b0(-1.0f, 1.0f);
            random_spark_vector[1] = random_float_2ef9b0(-1.0f, 1.0f);
            random_spark_vector[2] = random_float_2ef9b0(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)random_spark_vector;
            normalize_vector_xyz(spark_velocity, spark_velocity, vector_length_xyz(random_spark_vector) * 0.5f);
            add_vector_xyz(spark_velocity, random_spark_vector, spark_velocity);
            normalize_vector_xyz(spark_velocity, spark_velocity, random_float_2ef9b0(D_0015ED6C_2ef9b0 * 3.0f, D_0015ED6C_2ef9b0 * 6.0f));
            spawn_spark_2ef9b0(&moby->pos, spark_velocity, 0x7F2F4F6F,
                               random_int_between(scale_game_frames_2ef9b0(10), scale_game_frames_2ef9b0(15)),
                               100000.0f);
        }
        mark_moby_for_removal_2ef9b0(moby);
        return;
    }
    if (collision_line(old, pos, 2, moby, 0)) {
        if (collision_hit_material() == 1) {
            scale_vector_xyz(debris_vars, debris_vars, 0.4f);
            debris_vars[4] *= 0.25f;
            debris_vars[5] *= 0.25f;
        } else {
            moby->scale *= 0.8f;
            reflect_2ef9b0(debris_vars, debris_vars, D_L09_00174000);
            scale_vector_xyz(debris_vars, debris_vars, random_float_2ef9b0(0.3f, 0.1f));
            if (vector_length_xyz(debris_vars) < D_0015ED6C_2ef9b0 * 5.0f) {
                *(int *)(debris_vars + 7) = 0;
            }
            debris_vars[4] = random_float_2ef9b0(0.0f, -debris_vars[4]);
            debris_vars[5] = random_float_2ef9b0(0.0f, -debris_vars[5]);
            normalize_vector_xyz(D_L09_00174000, D_L09_00174000, 0.05f);
            add_vector_xyz(pos, D_L09_00174000 - 0x20, D_L09_00174000);
        }
    }
}
extern char *create_moby_c(int) __asm__("FUN_0020c4f8");

char *spawn_class_144_moby(struct Moby *owner, void *position, float angle) __asm__("FUN_L09_002efda8");

char *spawn_class_144_moby(struct Moby *owner, void *position, float angle) {
    u128_ee380 position_copy;
    void *copy = &position_copy;
    char *moby;

    position_copy = *(u128_ee380 *)position;
    moby = create_moby_c(0x144);
    if (moby != 0) {
        unsigned char state;

        ((unsigned char *)moby)[0x30] = owner->update_dist;
        state = owner->update_dist;
        moby[0x31] = 1;
        *(short *)(moby + 0x32) = state;
        qcopy(moby + 0x10, copy);
        qcopy(moby + 0x40, &owner->rot);
        *(long *)(moby + 0x38) = owner->spawn_frame;
        FUN_L00_00250df8(moby);
    }
    return moby;
}
#define MOBY(p) ((struct Moby *)(p))
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");

char *spawn_radially_scattered_moby(void *unused, float lower_radius, float upper_radius, void *origin, float *direction) __asm__("FUN_L09_002efe48");

char *spawn_radially_scattered_moby(void *unused, float lower_radius, float upper_radius, void *origin, float *direction) {
    float offset[4];
    char *moby = (char *)create_moby(0x1A1);
    if (moby != 0) {
        float pi = 3.1415927f;
        char *data = (char *)MOBY(moby)->pvars;
        float angle = random_float_between(-3.1415927f, 3.1415927f);
        float radius = random_float_between(lower_radius, upper_radius);
        float heading = angle_atan2(direction[0], direction[1]);
        float difference = fast_difference_between_rotations(heading, angle);
        float gap;

        MOBY(moby)->pos.x = fast_cos(angle) * radius;
        {
            float sine = fast_sin(angle);
            float speed = frame_time;
            MOBY(moby)->pos.y = sine * radius;
            MOBY(moby)->pos.z = 0.0f;
            normalize_vector_xyz(data, &MOBY(moby)->pos, difference * speed);
        }
        gap = pi - difference;
        normalize_vector_xyz(offset, direction, gap * gap * 0.25f * frame_time);
        add_vector_xyz(data, data, offset);
        *(int *)(data + 8) = 0;
        add_vector_xyz(&MOBY(moby)->pos, &MOBY(moby)->pos, origin);
        add_vector_xyz(&MOBY(moby)->pos, &MOBY(moby)->pos, direction);
        scale_vector_xyz(data, data, 0.35f);
        MOBY(moby)->anim_speed *= 0.5f;
        MOBY(moby)->rot.z = angle_atan2(*(float *)data, *(float *)(data + 4));
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        MOBY(moby)->visible = 1;
        pi -= difference;
        MOBY(moby)->scale *= (3.1415927f - difference) + (3.1415927f - difference) + 1.0f;
        FUN_L00_00250df8(moby);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002F1390), where it is exact; names translated to the US level program. */


void update_drifting_moby(struct Moby *moby) __asm__("FUN_L09_002f0040");

void update_drifting_moby(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    fast_vec_add(&moby->pos, &moby->pos, data);
    fast_vec_scale(data, data, 1.0f + frame_scale * -0.019999980926513672f);
    if (moby->anim_flags & 2) {
        delete_moby(moby);
    }
}
#include "rnc/gameplay/hero.h"

#include "qzero.h"

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002F9A50), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Vec_2F9A50;

typedef struct {
    char pad00[0x20];
    char f20[0x40];         /* 0x20: passed to func_L00_002617B0 */
    Vec_2F9A50 ang;         /* 0x60: tilt x, y */
    Vec_2F9A50 angvel;      /* 0x70 */
    int init;               /* 0x80 */
    float baseZ;            /* 0x84 */
    int timer;              /* 0x88 */
    int pad8C;
    float f90;              /* 0x90 */
    float f94;              /* 0x94 */
    float f98;              /* 0x98 */
    float f9C;              /* 0x9C */
} PlatVars_2F9A50;

extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern float D_0015ED6C_c2 __asm__("D_0015ED6C");
extern float D_0015ED70;
extern float D_L09_001619F4 __attribute__((sda));
extern float D_L09_001619F8 __attribute__((sda));
extern float D_L09_001619FC __attribute__((sda));
extern float D_L09_00161A04 __attribute__((sda));
extern float D_L09_00161A08 __attribute__((sda));
extern float D_L09_00161A0C __attribute__((sda));
extern float D_L09_00161A10 __attribute__((sda));
extern float D_L09_00161A14 __attribute__((sda));
extern float D_L09_00161A18 __attribute__((sda));
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void scale_vector_xyz_c3(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
float FUN_L00_0025bc98(float *p, float *q, int n, float x, float a, float b, float c);
void reorient_position_about_pivot(void *a0, void *a1, void *a2, void *a3, void *a4) __asm__("FUN_L07_00288968");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern int func_001F9850_2F9A50(int) __asm__("FUN_001f96f8");
extern int func_001F9908_2F9A50(int *) __asm__("FUN_001f9740");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_001F9C78_2F9A50(void *a, void *b) __asm__("FUN_001f9ab0");
extern float func_L00_0025CCF0_2F9A50(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern void reorient_position_about_pivot(void *a0, void *a1, void *a2, void *a3, void *a4) __asm__("FUN_L07_00288968");
extern void func_L00_00263B78_2F9A50(float, float, void *, float *, float *) __asm__("FUN_L00_00262b00");
extern void func_L00_00263BF8_2F9A50(void *, float *, float *, float, float, float) __asm__("FUN_L00_00262b80");
extern float func_001FA748_2F9A50(float, float) __asm__("FUN_001fa580");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_L00_002617B0_2F9A50(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern int func_0022ED80_2F9A50(int, int, void *) __asm__("FUN_0022da68");
extern unsigned char D_0013E633_2F9A50[] __asm__("D_0013F350");
extern unsigned char D_0013F3D0[];

void platform_update(Moby *moby) __asm__("FUN_L09_002f86a0");
void platform_update(Moby *moby)
{
    PlatVars_2F9A50 *data = (*(PlatVars_2F9A50 **)&moby->pvars);
    Vec_2F9A50 movement;
    Vec_2F9A50 old_rot;
    Vec_2F9A50 hero_offset;
    Vec_2F9A50 old_angles;
    Vec_2F9A50 old_pos;
    Vec_2F9A50 carried_pos;
    float sink_level;
    float old_z;
    float tilt;

    fast_vec_scale(&movement, &(*(Vec_2F9A50 *)&moby->pos), -1.0f);
    qcopy(&old_rot, &(*(Vec_2F9A50 *)&moby->rot));
    sink_level = data->baseZ - 0.25f;
    old_z = (*(Vec_2F9A50 *)&moby->pos).z;
    if (data->init == 0) {
        qcopy(&data->ang, &(*(Vec_2F9A50 *)&moby->rot));
        qzero(&data->angvel);
        (*(Vec_2F9A50 *)&moby->pos).z = (*(Vec_2F9A50 *)&moby->pos).z - 0.3f;
        data->init = 1;
        data->baseZ = (*(Vec_2F9A50 *)&moby->pos).z;
    }
    if (((Hero *)D_0013E633_2F9A50)->air_frames.s == 0 &&
        ((int)((Hero *)D_0013E633_2F9A50)->ground_moby) == (int)moby) {
        if (data->timer == 0) {
            func_0022ED80_2F9A50(0, 0, moby);
        }
        if (data->timer < func_001F9850_2F9A50(30)) {
            data->timer = func_001F9850_2F9A50(30);
        }
    }
    if (data->timer != 0) {
        float side_dot, front_dot, pitch_target;
        float deg_to_rad;
        func_001F9908_2F9A50(&data->timer);
        fast_vec_sub(&hero_offset, (D_0013E633_2F9A50 + 0x80), &(*(Vec_2F9A50 *)&moby->pos));
        deg_to_rad = 0.01745329238474369f;
        side_dot = func_001F9C78_2F9A50(&hero_offset, &(*(Vec_2F9A50 *)&moby->rot_row0));
        front_dot = func_001F9C78_2F9A50(&hero_offset, &(*(Vec_2F9A50 *)&moby->rot_row1));
        tilt = D_L09_001619F4;
        pitch_target = tilt * side_dot;
        *(u128 *)&old_angles = *(u128 *)&data->ang;
        *(u128 *)&old_pos = *(u128 *)&moby->pos;
        func_L00_0025CCF0_2F9A50(&data->ang.x, -tilt * front_dot, &data->angvel.x, 0,
            D_L09_00161A04 * deg_to_rad * D_0015ED70,
            D_L09_00161A08 * frame_scale_sq,
            D_L09_00161A0C * deg_to_rad * D_0015ED6C_c2);
        func_L00_0025CCF0_2F9A50(&data->ang.y, pitch_target, &data->angvel.y, 0,
            D_L09_00161A04 * deg_to_rad * D_0015ED70,
            D_L09_00161A08 * frame_scale_sq,
            D_L09_00161A0C * deg_to_rad * D_0015ED6C_c2);
        (*(Vec_2F9A50 *)&moby->pos).z = (*(Vec_2F9A50 *)&moby->pos).z - D_L09_001619F8 * D_0015ED6C_c2;
        if (sink_level < old_z && (*(Vec_2F9A50 *)&moby->pos).z <= sink_level) {
            func_0022ED80_2F9A50(1, 0, moby);
        }
        reorient_position_about_pivot((D_0013E633_2F9A50 + 0x80), &carried_pos, &(*(Vec_2F9A50 *)&moby->pos), &old_angles, &data->ang);
        fast_vec_sub(D_0013E633_2F9A50 + 0xF0, &carried_pos, (D_0013E633_2F9A50 + 0x80));
        ((Hero *)D_0013E633_2F9A50)->motion.platform_delta.f[2] =
            ((Hero *)D_0013E633_2F9A50)->motion.platform_delta.f[2] + ((*(Vec_2F9A50 *)&moby->pos).z - old_pos.z);
    } else {
        float deg_to_rad = 0.01745329238474369f;
        func_L00_0025CCF0_2F9A50(&data->ang.x, 0.0f, &data->angvel.x, 0,
            D_L09_00161A10 * deg_to_rad * D_0015ED70,
            D_L09_00161A14 * frame_scale_sq,
            D_L09_00161A18 * deg_to_rad * D_0015ED6C_c2);
        func_L00_0025CCF0_2F9A50(&data->ang.y, 0.0f, &data->angvel.y, 0,
            D_L09_00161A10 * deg_to_rad * D_0015ED70,
            D_L09_00161A14 * frame_scale_sq,
            D_L09_00161A18 * deg_to_rad * D_0015ED6C_c2);
        (*(Vec_2F9A50 *)&moby->pos).z = (*(Vec_2F9A50 *)&moby->pos).z + D_L09_001619FC * D_0015ED6C_c2 * (data->baseZ - (*(Vec_2F9A50 *)&moby->pos).z);
        if (data->baseZ < (*(Vec_2F9A50 *)&moby->pos).z) {
            (*(Vec_2F9A50 *)&moby->pos).z = data->baseZ;
        }
        if (old_z < sink_level && sink_level <= (*(Vec_2F9A50 *)&moby->pos).z) {
            func_0022ED80_2F9A50(2, 0, moby);
        }
    }
    func_L00_00263B78_2F9A50(0.14f, D_0015ED6C_c2 * 0.5235987901687622f, moby, &data->f98, &data->f9C);
    func_L00_00263BF8_2F9A50(moby, &data->f90, &data->f94, 0.0872664600610733f,
        D_0015ED6C_c2 * 0.3141592741012573f, D_0015ED6C_c2 * 0.48869219422340393f);
    (*(Vec_2F9A50 *)&moby->rot).x = func_001FA748_2F9A50(data->ang.x, (*(Vec_2F9A50 *)&moby->rot).x);
    (*(Vec_2F9A50 *)&moby->rot).y = func_001FA748_2F9A50(data->ang.y, (*(Vec_2F9A50 *)&moby->rot).y);
    fast_vec_add(&movement, &movement, &(*(Vec_2F9A50 *)&moby->pos));
    func_L00_002617B0_2F9A50(data->f20, &movement, &old_rot, &(*(Vec_2F9A50 *)&moby->rot));
}
/* UpdateMoby_1000: timer and sound bookkeeping */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00301C38), where it is exact; names translated to the US level program. */

extern int D_0015EEA4_q __asm__("D_0015EEA4");
extern int D_L09_0015F5CC;
extern int give_reward(int a, int b) __asm__("FUN_L00_00203908");
extern s32 scale_game_frames_q(s32) __asm__("FUN_001f96f8");
extern unsigned char item_unlocked[] __asm__("D_0013D408") __attribute__((section(".data")));
extern unsigned char D_0013D4C2_q[] __asm__("D_0013D4C2");
extern unsigned char D_0013F3D0_q[] __asm__("D_0013F3D0") __attribute__((section(".data")));
extern unsigned char D_00141968_q[] __asm__("D_00141968") __attribute__((section(".data")));
extern void queue_dialogue_message(int arg0, int arg1) __asm__("FUN_L00_00263d40");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");
s32 is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");

void update_level09_item_unlock_and_frame_record(struct Moby *moby) __asm__("FUN_L09_00300888");

void update_level09_item_unlock_and_frame_record(struct Moby *moby) {
    int *data = (int *)moby->pvars;
    unsigned char *item_unlock_flags;
    unsigned char *frame_and_level_record;
    unsigned short *tp;
    int scaled_frame_quotient;
    if (D_L09_0015F5CC > 0) {
        if (D_L09_001613C8 == 0) {
            item_unlock_flags = item_unlocked;
            if (item_unlock_flags[0xE] == 0) {
                item_unlock_flags[0xE] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
                D_L09_001613C8 = 0;
            } else {
                D_L09_001613C8 = 0;
            }
        } else {
            D_L09_001613C8 = 0;
        }
    }
    frame_and_level_record = D_00141968_q;
    tp = (unsigned short *)(frame_and_level_record + 0x2D8);
    if (*tp == 0) {
        if (D_0013D4C2_q[9] != 0) {
            (*tp)++;
        }
        scaled_frame_quotient = scale_game_frames_q(D_0015EEA4_q) / 600;
        if (*(unsigned short *)(frame_and_level_record + 0x2DA) < scaled_frame_quotient) {
            *(unsigned short *)(frame_and_level_record + 0x2DA) = scale_game_frames_q(D_0015EEA4_q) / 600;
        }
        *(int *)(frame_and_level_record + 0x2DC) = *(int *)(frame_and_level_record + 0x2DC) | (1 << current_level_index) | 0x80000000;
        if (is_point_inside_clip_volume(D_0013F3D0_q, *data) != 0) {
            give_reward(0x2329, 0x5B);
        }
    }
}
