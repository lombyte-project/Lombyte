#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00200468/FUN_00200468.s", FUN_00200468);
#else
#include "types.h"

struct DmaTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
    s64 gif_tag;
    s64 gif_registers;
    s64 texture_tex0;
    s64 primitive;
    s64 color;
    s64 first_uv;
    s64 first_xyz;
    s64 second_uv;
};

struct TagPtr {
    struct DmaTag *p;
};

struct SpriteDepthState {
    u8 pad0[0xC];
    s32 z_and_fog;
};

struct ScreenOfs {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

extern struct TagPtr render_packet_cursor __asm__("D_00160F00");
extern struct ScreenOfs screen_offsets __asm__("D_0013E500");
extern struct SpriteDepthState sprite_depth_state __asm__("D_0019A3E8");

void append_power_of_two_textured_screen_sprite(u64 tex0, s32 screen_x, s32 screen_y, s32 texture_width_log2, s32 texture_height_log2, s32 screen_width, s32 screen_height, s32 texture_u, s32 texture_v,
                  s32 alpha) __asm__("FUN_00200468");

void append_power_of_two_textured_screen_sprite(u64 tex0, s32 screen_x, s32 screen_y, s32 texture_width_log2, s32 texture_height_log2, s32 screen_width, s32 screen_height, s32 texture_u, s32 texture_v,
                  s32 alpha) {
    struct DmaTag *tag;
    s64 *packet_words;

    render_packet_cursor.p->w0 = 0x10000005;
    render_packet_cursor.p->addr = 0;
    render_packet_cursor.p->w2 = 0;
    render_packet_cursor.p->w3 = 0x50000005;
    tag = render_packet_cursor.p;
    packet_words = (s64 *)((u8 *)tag + 0x10);
    render_packet_cursor.p = (struct DmaTag *)packet_words;
    tag->gif_tag = 0x7400000000008001;
    packet_words[2] = tex0;
    packet_words[1] = 0x5353106;
    packet_words[4] = ((s64)alpha << 24) | 0x7F7F7F;
    packet_words[3] = 0x156;
    packet_words[5] = texture_u | ((s64)texture_v << 16);
    packet_words[6] = (screen_x + screen_offsets.x - 8) | ((s64)(screen_y + screen_offsets.y - 8) << 16) |
           ((u64)sprite_depth_state.z_and_fog << 32);
    packet_words[7] = (texture_u + (1 << (texture_width_log2 + 4))) | ((s64)(texture_v + (1 << (texture_height_log2 + 4))) << 16);
    packet_words[8] = ((screen_x + screen_width) + screen_offsets.x - 8) | ((s64)((screen_y + screen_height) + screen_offsets.y - 8) << 16) |
           ((u64)sprite_depth_state.z_and_fog << 32);
    packet_words[9] = 0;
    render_packet_cursor.p = (struct DmaTag *)((u8 *)render_packet_cursor.p + 0x50);
}

extern __typeof__(append_power_of_two_textured_screen_sprite) func_00200468 __attribute__((alias("FUN_00200468")));

#endif /* NON_MATCHING */
