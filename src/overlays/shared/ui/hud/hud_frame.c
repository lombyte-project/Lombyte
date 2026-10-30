/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite.h"
#include "rnc/rendering/texture/get_frame_texture.h"

typedef struct {
    u8 p[6];
    u8 lw;
    u8 lh;
} R0023b120;

typedef struct {
    s16 a;
    s16 k;
} T0023b120;

typedef struct {
    u8         p[0x20];
    T0023b120 *tab;
    R0023b120 *rec;
} G0023b120;

typedef struct {
    u8  p[0x48];
    s16 x;
    s16 y;
} O0023b120;

extern G0023b120 D_L00_0017E4D8;
void             hud_frame(O0023b120 *o, s32 id, s32 x, s32 y, u32 flags, s32 extra) __asm__("FUN_L00_0023b120");

void hud_frame(O0023b120 *hud_element, s32 id, s32 x, s32 y, u32 flags, s32 extra) {
    s32        base_width, base_height, draw_width, draw_height;
    R0023b120 *image_page;
    if (id < 0)
        return;
    x += hud_element->x;
    y += hud_element->y;
    image_page = &D_L00_0017E4D8.rec[D_L00_0017E4D8.tab[id].k];
    base_width = 1 << image_page->lw;
    base_height = 1 << image_page->lh;
    draw_width = base_width;
    draw_height = base_height;
    if (flags & 1) {
        x -= draw_width >> 1;
        y -= draw_height >> 1;
    }
    if (flags & 2) {
        x += draw_width >> 2;
        y += draw_height >> 2;
        draw_width >>= 1;
        draw_height >>= 1;
    }
    if (flags & 4) {
        x -= base_width >> 1;
        y -= base_height >> 1;
        draw_width <<= 1;
        draw_height <<= 1;
    }
    draw_hud_sprite(id, x, y, draw_width, draw_height, extra);
}

extern char *D_L00_001611C0_23b6c0 __asm__("D_L00_001611C0");
extern int   screen_extent[] __asm__("D_0013E500");
extern int   D_L00_0017E4D8_lui[] __asm__("D_L00_0017E4D8") NOT_SDA;

void draw_cropped_hud_sprite(
    int tex, int x, int y, int draw_width, int draw_height, int alpha) __asm__("FUN_L00_0023b6c0");

void draw_cropped_hud_sprite(

    int frame_id, int x, int y, int draw_width, int draw_height, int alpha) {
    u64 *packet_words;
    int *coordinate_offsets;
    int *vertex_depth_source;
    *(int *)(D_L00_001611C0_23b6c0 + 0) = 0x10000005;
    *(int *)(D_L00_001611C0_23b6c0 + 4) = 0;
    *(int *)(D_L00_001611C0_23b6c0 + 8) = 0;
    *(int *)(D_L00_001611C0_23b6c0 + 12) = 0x50000005;
    D_L00_001611C0_23b6c0 += 0x10;
    packet_words = (u64 *)D_L00_001611C0_23b6c0;
    packet_words[0] = 0x7400000000008001ULL;
    packet_words[1] = 0x5353106;
    packet_words[2] = get_frame_texture(frame_id);
    packet_words[3] = 0x156;
    packet_words[4] = ((u64)alpha << 24) | 0x7F7F7F;
    packet_words[5] = 0;
    coordinate_offsets = screen_extent;
    vertex_depth_source = D_L00_0017E4D8_lui;
    packet_words[6] = (u64)(((x << 4) + coordinate_offsets[4] - 8) |
                            (((u64)((y << 4) + coordinate_offsets[5] - 8)) << 16)) |
                      ((u64)vertex_depth_source[3] << 32);
    packet_words[7] = (draw_height << 20) + (draw_width << 4);
    x += draw_width;
    y += draw_height;
    packet_words[8] = (u64)(((x << 4) + coordinate_offsets[4] - 8) |
                            (((u64)((y << 4) + coordinate_offsets[5] - 8)) << 16)) |
                      ((u64)vertex_depth_source[3] << 32);
    packet_words[9] = 0;
    *(char *volatile *)&D_L00_001611C0_23b6c0 += 0x50;
}

typedef unsigned long u64_23bac0;

typedef struct {
    unsigned char p[6];
    unsigned char lw;
    unsigned char lh;
} R_23bac0;

typedef struct {
    short a;
    short k;
} T_23bac0;

typedef struct {
    unsigned char p[0xC];
    int           zc;
    unsigned char p10[0x10];
    T_23bac0     *tab;
    R_23bac0     *rec;
} G_23bac0;

extern char    *D_L00_001611C0_23bac0 __asm__("D_L00_001611C0");
extern int      D_0013E500_23bac0[] __asm__("D_0013E500");
extern G_23bac0 D_L00_0017E4D8_23bac0 __asm__("D_L00_0017E4D8") NOT_SDA;
void            draw_rotated_hud_sprite(
               int id, int x, int y, int draw_width, int draw_height, int alpha) __asm__("FUN_L00_0023bac0");

void draw_rotated_hud_sprite(int id, int x, int y, int draw_width, int draw_height, int alpha) {
    u64_23bac0 *packet_words;
    int        *coordinate_offsets;
    G_23bac0   *sprite_render_data = &D_L00_0017E4D8_23bac0;
    R_23bac0   *image_page = &sprite_render_data->rec[sprite_render_data->tab[id].k];
    int         ww = 1 << image_page->lw;
    int         hh = 1 << image_page->lh;
    *(int *)(D_L00_001611C0_23bac0 + 0) = 0x10000007;
    *(int *)(D_L00_001611C0_23bac0 + 4) = 0;
    *(int *)(D_L00_001611C0_23bac0 + 8) = 0;
    *(int *)(D_L00_001611C0_23bac0 + 12) = 0x50000007;
    D_L00_001611C0_23bac0 += 0x10;
    packet_words = (u64_23bac0 *)D_L00_001611C0_23bac0;
    packet_words[0] = 0xB400000000008001UL;
    packet_words[1] = 0x53535353106UL;
    packet_words[2] = get_frame_texture(id);
    packet_words[3] = 0x154;
    packet_words[4] = ((u64_23bac0)alpha << 24) | 0x7F7F7F;
    packet_words[5] = ww << 4;
    coordinate_offsets = D_0013E500_23bac0;
    packet_words[6] = (u64_23bac0)(((x << 4) + coordinate_offsets[4] - 8) |
                                   (((u64_23bac0)((y << 4) + coordinate_offsets[5] - 8)) << 16)) |
                      ((u64_23bac0)sprite_render_data->zc << 32);
    packet_words[7] = (hh << 20) + (ww << 4);
    packet_words[8] = (u64_23bac0)((((x + draw_width) << 4) + coordinate_offsets[4] - 8) |
                                   (((u64_23bac0)((y << 4) + coordinate_offsets[5] - 8)) << 16)) |
                      ((u64_23bac0)sprite_render_data->zc << 32);
    packet_words[9] = 0;
    packet_words[10] =
        (u64_23bac0)(((x << 4) + coordinate_offsets[4] - 8) |
                     (((u64_23bac0)(((y + draw_height) << 4) + coordinate_offsets[5] - 8)) << 16)) |
        ((u64_23bac0)sprite_render_data->zc << 32);
    packet_words[11] = hh << 20;
    packet_words[12] =
        (u64_23bac0)((((x + draw_width) << 4) + coordinate_offsets[4] - 8) |
                     (((u64_23bac0)(((y + draw_height) << 4) + coordinate_offsets[5] - 8)) << 16)) |
        ((u64_23bac0)sprite_render_data->zc << 32);
    packet_words[13] = 0;
    *(char *volatile *)&D_L00_001611C0_23bac0 += 0x70;
}

typedef struct O23cdb8 {
    u8   pad0[0x8];
    s32  max;
    s32 *src;
    u8   pad10[0x5C];
    s32  f6C;
    u8   c[4];
    s32  val;
    u8   pad78[4];
    s32  timer;
} O23cdb8;

extern s32 D_L00_0015F91C_gp __asm__("D_L00_0015F91C") __attribute__((sda));
extern s32 D_L00_0015F920_gp __asm__("D_L00_0015F920") __attribute__((sda));
void       hud_weapon_update(O23cdb8 *value_state) __asm__("FUN_L00_0023cdb8");

void hud_weapon_update(O23cdb8 *value_state) {
    u8 *c = value_state->c;
    s32 source_value = *value_state->src;
    value_state->val = source_value;
    if (value_state->max < source_value)
        value_state->val = value_state->max;
    else if (source_value < 0)
        value_state->val = 0;
    if (value_state->timer >= scale_game_frames(5)) {
        value_state->timer = scale_game_frames(5);
        if (c[0] < D_L00_0015F91C_gp)
            c[0]++;
        else if (c[1] < D_L00_0015F920_gp)
            c[1]++;
    } else {
        value_state->f6C = 1;
        if (c[1] != 0)
            c[1]--;
        else if (c[0] != 0)
            c[0]--;
        else
            value_state->f6C = -6;
    }
}

s32 D_L01_0015F91C = 8;
s32 D_L01_0015F920 = 8;
s32 D_L03_0015F91C = 8;
s32 D_L03_0015F920 = 8;
s32 D_L04_0015F91C = 8;
s32 D_L04_0015F920 = 8;
s32 D_L05_0015F92C = 8;
s32 D_L05_0015F930 = 8;
s32 D_L06_0015F91C = 8;
s32 D_L06_0015F920 = 8;
s32 D_L07_0015F91C = 8;
s32 D_L07_0015F920 = 8;
s32 D_L08_0015F91C = 8;
s32 D_L08_0015F920 = 8;
s32 D_L09_0015F91C = 8;
s32 D_L09_0015F920 = 8;
s32 D_L10_0015F91C = 8;
s32 D_L10_0015F920 = 8;
s32 D_L11_0015F91C = 8;
s32 D_L11_0015F920 = 8;
s32 D_L12_0015F91C = 8;
s32 D_L12_0015F920 = 8;
s32 D_L13_0015F91C = 8;
s32 D_L13_0015F920 = 8;
s32 D_L14_0015F91C = 8;
s32 D_L14_0015F920 = 8;
s32 D_L15_0015F91C = 8;
s32 D_L15_0015F920 = 8;
s32 D_L16_0015F92C = 8;
s32 D_L16_0015F930 = 8;
s32 D_L17_0015F91C = 8;
s32 D_L17_0015F920 = 8;
s32 D_L18_0015F91C = 8;
s32 D_L18_0015F920 = 8;
