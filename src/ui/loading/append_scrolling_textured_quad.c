/* Ported from rac1-decomp (src/game/space.c, func_00232A00). */
#include "sda.h"
#include "qcopy.h"
#include "rnc/rendering/dma_tag.h"

#include "rnc/rendering/screen.h"

extern char loading_quad_packet_header_template[] __asm__("D_00160850");

typedef union {
    struct {
        float u;
        float v;
    } f;
    long bits;
} SpaceUvPair;

/* Append a textured four-corner GIF packet. Screen coordinates use 12.4
   fixed point relative to the viewport origin; UV pairs stay as floats. */
void append_scrolling_textured_quad(int           x,
                                    int           y,
                                    int           w,
                                    int           h,
                                    unsigned long rgba,
                                    unsigned long tex,
                                    float         u0,
                                    float         u1,
                                    float         v0,
                                    float         v1) __asm__("FUN_002316e8");

void append_scrolling_textured_quad(int           x,
                                    int           y,
                                    int           quad_width,
                                    int           quad_height,
                                    unsigned long rgba,
                                    unsigned long texture_register_value,
                                    float         texture_u_left,
                                    float         texture_u_right,
                                    float         texture_v_top,

                                    float texture_v_bottom) {
    int         x0 = x * 16 + screen_extent.left - 8;
    int         x1 = (x + quad_width) * 16 + screen_extent.left - 8;
    int         y0 = y * 16 + screen_extent.top - 8;
    int         y1 = (y + quad_height) * 16 + screen_extent.top - 8;
    SpaceUvPair uv[4];
    long       *quad_payload;
    int        *base;

    uv[0].f.u = texture_u_left;
    uv[0].f.v = texture_v_top;
    uv[1].f.u = texture_u_right;
    uv[1].f.v = texture_v_top;
    uv[2].f.u = texture_u_left;
    uv[2].f.v = texture_v_bottom;
    uv[3].f.u = texture_u_right;
    uv[3].f.v = texture_v_bottom;

    render_packet_cursor.words[0] = 0x10000007;
    render_packet_cursor.words[1] = 0;
    render_packet_cursor.words[2] = 0;
    render_packet_cursor.words[3] = 0x50000007;
    base = render_packet_cursor.words;
    render_packet_cursor.words = base + 4;
    qcopy(base + 4, loading_quad_packet_header_template);
    quad_payload = (long *)(base + 8);
    render_packet_cursor.words = base + 8;
    quad_payload[0] = texture_register_value;
    quad_payload[1] = 0x54;
    quad_payload[2] = (rgba & 0xFFFFFFFFL) | (0xFE00L << 46);
    quad_payload[3] = uv[0].bits;
    quad_payload[4] = x0 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    quad_payload[5] = uv[1].bits;
    quad_payload[6] = x1 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    quad_payload[7] = uv[2].bits;
    quad_payload[8] = x0 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    quad_payload[9] = uv[3].bits;
    quad_payload[10] = x1 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    quad_payload[11] = 0;
    render_packet_cursor.words = (int *)((char *)render_packet_cursor.words + 0x60);
}

extern __typeof__(append_scrolling_textured_quad) func_002316E8
    __attribute__((alias("FUN_002316e8")));
