#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/map/draw_map_overlay/FUN_00205640.s", FUN_00205640);
#else
#include "types.h"
#include "sda.h"

struct DmaTag {
    u32 dma_control;
    u32 addr;
    u32 vif0;
    u32 vif1;
};

struct TagPtr {
    struct DmaTag *p;
};

struct ScreenOffset {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

typedef struct {
    s16 id;      /* 0x00 */
    u16 pad02;
    u16 flags;   /* 0x04 */
    u16 texture_id;     /* 0x06 */
    s16 frame_index;   /* 0x08 */
    u16 pad0A[2];
    u16 label_width;       /* 0x0E */
    s16 label_height;       /* 0x10 */
    s16 label_offset_x;    /* 0x12 */
    s16 label_offset_y;    /* 0x14 */
    u16 pad16;
    f32 x;       /* 0x18 */
    f32 y;       /* 0x1C */
    f32 angle;   /* 0x20 */
    s32 active;  /* 0x24 */
} MapIcon;

typedef struct {
    s32 cell;
    s32 pad[3];
} MapMark;

typedef struct {
    u8 pad0[0x8];
    s32 marks_enabled;      /* 0x08 */
    u8 padC[0xC];
    s32 z;             /* 0x18 */
    s32 grid;          /* 0x1C */
    MapIcon *icons;    /* 0x20 */
    s32 enabled;       /* 0x24 */
    u8 pad28[0x4];
    s32 show_markers;    /* 0x2C */
    MapMark marks[8];  /* 0x30 */
    u8 padB0[0x4];
    f32 zoom[20];      /* 0xB4 */
    s32 offset_x[20];     /* 0x104 */
    s32 offset_y[20];     /* 0x154 */
    u8 pad1A4[0x84];
    s32 selected_map;           /* 0x228 */
    u8 pad22C[0x14];
    s32 texture_address;           /* 0x240 */
} MapState;

typedef struct {
    u8 pad0[0x80];
    f32 x;        /* 0x80 */
    f32 y;        /* 0x84 */
    u8 pad88[0x10];
    f32 angle;    /* 0x98 */
    u8 pad9C[0x1FF0];
    s32 mode;     /* 0x208C */
} Player;

typedef struct {
    s16 pad0;
    s16 image_index;
} MapTextureReference;

typedef struct {
    u8 pad0[6];
    u8 width_exponent;
    u8 height_exponent;
} MapTextureInfo;

typedef struct {
    u8 pad0[0x20];
    MapTextureReference *refs;     /* 0x20 */
    MapTextureInfo *infos;   /* 0x24 */
} MapTextureBank;

typedef struct {
    s16 s[12];
} FontWindow;

typedef struct {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MapIconBounds;

#define SPR ((MapIconBounds *)0x70000000)

extern struct TagPtr D_00160F00;
extern struct ScreenOffset D_0013E500;
extern MapState D_001A00F0;
extern Player D_0013F350;
extern MapTextureBank D_0019A3E8;
extern u8 D_0013D5BC[];
extern u16 D_001518D2[];
extern s32 D_0015ED84;
extern u8 D_0015EDB4;
extern s32 D_0015FD60 __attribute__((sda));
extern f32 D_0015FD80 __attribute__((sda));
extern f32 D_0015FD84 __attribute__((sda));
extern f32 D_0015FD88 __attribute__((sda));
extern f32 D_0015FD8C __attribute__((sda));
extern f32 D_0015FD90 __attribute__((sda));
extern f32 D_0015FD94 __attribute__((sda));
extern u8 D_001E8068[];

extern void func_001F0C50(s32, s32, s32, u8 *);
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void func_001F5F18(s32, s32, s32, s32, s32);
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");
extern f32 func_001FA580(f32, f32);
extern f32 func_001FA5C8(f32, f32);
extern s32 find_valid_animation_frame_index(s32, s32) __asm__("func_001FF960");
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");
extern void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");
extern void func_00200600(f32, f32, f32, f32, f32, s32, s32, u64);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern void func_00208280(s32, void *);
extern void func_00208408(f32 *, f32 *, s32, f32, f32);
extern void func_00208508(s32, s32, s32, s32);
extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");
extern void *memset(void *, s32, u32);

void draw_map_overlay(void) __asm__("FUN_00205640");

extern MapState D_001A00F0_far __asm__("D_001A00F0") __attribute__((section(".data")));
void draw_map_overlay(void) {
    union {
        u8 b[0x80];
        f32 f[2];
    } buf;
    FontWindow text_window;
    s32 rx0, ry0, rx1, ry1;
    struct DmaTag *tag;
    u64 *packet_words;
    u64 background_tex0;
    u64 map_tex0;
    s32 palette_block;
    s32 mirror_sign;
    s32 tile_size;
    s32 left_tile_count, top_tile_count, right_tile_count, bottom_tile_count;
    s32 x0, y0, x1, y1;
    s32 u, v;
    s32 dx, dy;
    s32 i, j;
    s32 cell, cell_y, cell_x;
    MapMark *mk;
    MapIconBounds *r;
    MapIconBounds *icon_bounds;
    MapIcon *ic;
    MapTextureInfo *ti;
    f32 zoom;
    f32 scale;
    f32 k;
    f32 s;
    f32 fx, fy;
    s32 ox0, ox1, oy0, oy1;
    s32 ax, ay, bx, by;
    s32 id, frame_index, texture_width;
    f32 angle, sprite_width, sprite_height, center_x, center_y;
    s16 label_width;
    s32 label_height, label_offset_x, label_offset_y;
    s32 label_x, label_y;
    s32 image_index;
    s32 flip;
    s32 texture_id;
    s32 tile_limit;

    if (D_001A00F0.enabled == 0) {
        u8 *font = D_001E8068;

        setup_gif_paging(0);
        func_001F0C50(0x100, (s16)D_001518D2[0] >> 1, 0x80909090, font);
        do_gif_paging();
        return;
    }
    if (D_001A00F0.selected_map < 0) {
        return;
    }

    rx0 = 0x1000;
    setup_gif_paging(0);
    ry0 = 0x800;
    mirror_sign = D_0015EDB4 ? -1 : 1;
    zoom = D_001A00F0.zoom[D_001A00F0.selected_map];
    oy0 = zoom * (f32)(D_001A00F0.offset_y[D_001A00F0.selected_map] >> 15);
    ox1 = zoom * 8192.0f;
    ox0 = zoom * (f32)(D_001A00F0.offset_x[D_001A00F0.selected_map] >> 15);
    tile_size = zoom * 512.0f;
    tile_limit = tile_size + 0x2000;
    rx0 -= ox0 * mirror_sign;
    ry0 -= oy0;
    rx1 = rx0 + ox1 * mirror_sign;
    ry1 = ry0 + ox1;
    left_tile_count = (rx0 + tile_size - 1) / tile_size;
    top_tile_count = (ry0 + tile_size - 1) / tile_size;
    right_tile_count = (tile_limit - rx1 - 1) / tile_size;
    bottom_tile_count = (tile_limit - ry1 - 1) / tile_size;
    x0 = rx0 - tile_size * left_tile_count;
    y0 = ry0 - tile_size * top_tile_count;
    x1 = rx1 + tile_size * right_tile_count;
    y1 = ry1 + tile_size * bottom_tile_count;
    u = ((left_tile_count + right_tile_count) << 9) + 0x2000;
    v = ((top_tile_count + bottom_tile_count) << 9) + 0x2000;

    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x47, 0);
    D_00160F00.p->dma_control = 0x10000005;
    D_00160F00.p->addr = 0;
    D_00160F00.p->vif0 = 0;
    D_00160F00.p->vif1 = 0x50000005;
    D_00160F00.p++;
    background_tex0 = get_frame_texture(find_valid_animation_frame_index(0xE999, D_001A00F0.selected_map));
    packet_words = (u64 *)D_00160F00.p;
    packet_words[0] = 0x7400000000008001;
    packet_words[1] = 0x5353106;
    packet_words[2] = background_tex0;
    packet_words[3] = 0x156;
    packet_words[4] = 0x80808080;
    packet_words[5] = 0;
    packet_words[6] = ((x0 + D_0013E500.x) - 8) | ((u64)((y0 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    packet_words[7] = u | ((u64)v << 16);
    packet_words[8] = ((x1 + D_0013E500.x) - 8) | ((u64)((y1 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    packet_words[9] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x50);
    vu1_add_g_sregister(8, 5);
    vu1_add_g_sregister(0x47, 0x60B);

    palette_block = (background_tex0 >> 37) & 0x3FFF;
    D_00160F00.p->dma_control = 0x10000005;
    D_00160F00.p->addr = 0;
    D_00160F00.p->vif0 = 0;
    D_00160F00.p->vif1 = 0x50000005;
    map_tex0 = (u64)((D_001A00F0.texture_address >> 8) | (8 << 14) | (0x13 << 20) | (9 << 26)) | ((u64)9 << 30) | ((u64)1 << 34) | ((u64)palette_block << 37) | ((long)0x8000000000000000ULL);
    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    packet_words = (u64 *)(tag + 1);
    packet_words[0] = 0x7400000000008001;
    packet_words[1] = 0x5353106;
    packet_words[2] = map_tex0;
    packet_words[3] = 0x156;
    packet_words[4] = 0x80808080;
    packet_words[5] = 0;
    packet_words[6] = ((rx0 + D_0013E500.x) - 8) | ((u64)((ry0 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    packet_words[7] = 0x20002000;
    packet_words[8] = ((rx1 + D_0013E500.x) - 8) | ((u64)((ry1 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    packet_words[9] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x50);
    vu1_add_g_sregister(0x47, 0x360B);

    if (D_001A00F0.show_markers != 0 && D_001A00F0.marks_enabled != 0) {
        dx = rx1 - rx0;
        dy = ry1 - ry0;
        for (i = 0; i < 8; i++) {
            cell = D_001A00F0.marks[i].cell;
            if (cell >= 0) {
                cell_x = cell % 16;
                cell_y = cell / 16;
                func_00200E08(rx0 + cell_x * dx / 16, ry0 + cell_y * dy / 16,
                              rx0 + (cell_x + 1) * dx / 16, ry0 + (cell_y + 1) * dy / 16,
                              0x20000000, 1);
            }
        }
    }

    if (D_001A00F0.icons != 0) {
        icon_bounds = SPR;
        scale = (D_001A00F0.zoom[D_001A00F0.selected_map] * 2.0f + 5.0f) / 13.0f;
        if (!(D_001A00F0.icons[0].flags & 4)) {
            MapIconBounds *r;
            s32 i;
            MapIcon *ic;
            f32 k;
            s32 image_index;
            f32 fx, fy;
            MapTextureInfo *ti;
            f32 s;
            s32 texture_id;
            

            i = 0;
            do {
                if (D_001A00F0.icons[i].active != 0 && (texture_id = D_001A00F0.icons[i].texture_id) != 0 && !(D_001A00F0.icons[i].flags & 1)) {
                    k = 1.0f;
                    if (D_001A00F0.icons[i].flags & 0x80) {
                        k = 1.5f;
                    }
                    image_index = find_valid_animation_frame_index(texture_id, D_001A00F0.icons[i].frame_index);
                    fx = (f32)rx0 + D_001A00F0.icons[i].x * (f32)(rx1 - rx0);
                    fy = (f32)ry0 + D_001A00F0.icons[i].y * (f32)(ry1 - ry0);
                    ti = &D_0019A3E8.infos[D_0019A3E8.refs[image_index].image_index];
                    if (D_001A00F0.icons[i].flags & 0x200) {
                        s = D_001A00F0.zoom[D_001A00F0.selected_map];
                    } else {
                        s = scale;
                    }
                    icon_bounds[i].x0 = fx - k * s * (f32)(1 << (ti->width_exponent + 3));
                    icon_bounds[i].x1 = (f32)icon_bounds[i].x0 + k * s * (f32)(1 << (ti->width_exponent + 4));
                    icon_bounds[i].y0 = fy - k * s * (f32)(1 << (ti->height_exponent + 3));
                    icon_bounds[i].y1 = (f32)icon_bounds[i].y0 + k * s * (f32)(1 << (ti->height_exponent + 4));
                }
                i++;
            } while (!(D_001A00F0.icons[i].flags & 4));
        }

        {
            s32 i, j;
            s32 ox0, ox1, oy0, oy1;
            s32 ax, ay, bx, by;
            MapIcon *ic;

            for (i = 0; !(D_001A00F0.icons[i + 1].flags & 4); i++) {
                if (D_001A00F0.icons[i].active == 0 || D_001A00F0.icons[i].texture_id == 0 || (D_001A00F0.icons[i].flags & 3)) {
                    continue;
                }
                for (j = i + 1; !(D_001A00F0.icons[j].flags & 4); j++) {
                    ox0 = icon_bounds[j].x1 - icon_bounds[i].x0;
                    if (ox0 <= 0) continue;
                    ox1 = icon_bounds[i].x1 - icon_bounds[j].x0;
                    if (ox1 <= 0) continue;
                    oy0 = icon_bounds[j].y1 - icon_bounds[i].y0;
                    if (oy0 <= 0) continue;
                    oy1 = icon_bounds[i].y1 - icon_bounds[j].y0;
                    if (oy1 <= 0) continue;
                    if (D_001A00F0.icons[j].active == 0 || D_001A00F0.icons[j].texture_id == 0 || (D_001A00F0.icons[j].flags & 3)) {
                        continue;
                    }
                    ax = 0;
                    ay = 0;
                    bx = 0;
                    by = 0;
                    if (ox0 <= ox1 && ox0 <= oy0 && ox0 <= oy1) {
                        ax = ox0 >> 1;
                        bx = ax - ox0;
                    } else if (ox1 <= oy0 && ox1 <= oy1) {
                        bx = ox1 >> 1;
                        ax = bx - ox1;
                    } else if (oy0 <= oy1) {
                        ay = oy0 >> 1;
                        by = ay - oy0;
                    } else {
                        by = oy1 >> 1;
                        ay = by - oy1;
                    }
                    icon_bounds[i].x0 += ax;
                    icon_bounds[i].x1 += ax;
                    icon_bounds[i].y0 += ay;
                    icon_bounds[i].y1 += ay;
                    icon_bounds[j].x0 += bx;
                    icon_bounds[j].x1 += bx;
                    icon_bounds[j].y0 += by;
                    icon_bounds[j].y1 += by;
                }
            }
        }

        {
            s32 i;

        i = 0;
        if (!(D_001A00F0.icons[0].flags & 4)) {
            do {
                if (D_001A00F0.icons[i].active != 0 && !(D_001A00F0.icons[i].flags & 1) && (id = D_001A00F0.icons[i].texture_id) != 0) {
                    frame_index = D_001A00F0.icons[i].frame_index;
                    if (D_001A00F0.icons[i].flags & 0x40) {
                        func_00200E08(icon_bounds[i].x0 - 0x20, icon_bounds[i].y0 - 0x20, icon_bounds[i].x1 + 0x20, icon_bounds[i].y1 + 0x20,
                                      0x80000000, 1);
                    }
                    if (D_001A00F0.icons[i].flags & 0x200) {
                        s = D_001A00F0.zoom[D_001A00F0.selected_map];
                    } else {
                        s = scale;
                    }
                        if (D_001A00F0.icons[i].flags & 0x100) {
                        angle = D_001A00F0.icons[i].angle;
                        texture_width = 0x20;
                        sprite_height = s * 256.0f;
                        sprite_width = sprite_height;
                        if (D_001A00F0.icons[i].flags & 0x400) {
                            angle = func_001FA580(angle, 1.5707964f);
                            texture_width = 0x40;
                            sprite_width = s * D_0015FD88;
                            sprite_height = s * D_0015FD8C;
                            if (*(s32 *)(D_0013D5BC + D_001A00F0.icons[i].id * 16) & 2) {
                                frame_index++;
                            }
                        }
                        if (D_001A00F0.icons[i].flags & 0x800) {
                            angle = func_001FA580(angle, 1.5707964f);
                            texture_width = 0x40;
                            sprite_width = s * D_0015FD80;
                            sprite_height = s * D_0015FD84;
                            if (*(s32 *)(D_0013D5BC + D_001A00F0.icons[i].id * 16) & 2) {
                                frame_index++;
                            }
                        }
                        if (D_001A00F0.icons[i].flags & 0x1000) {
                            angle = func_001FA580(angle, 1.5707964f);
                            sprite_width = s * D_0015FD90;
                            sprite_height = s * D_0015FD94;
                        }
                        center_x = (f32)(icon_bounds[i].x0 + icon_bounds[i].x1) * 0.5f;
                        center_y = (f32)(icon_bounds[i].y0 + icon_bounds[i].y1) * 0.5f;
                        func_00200600(center_x, center_y, sprite_width, sprite_height, angle, texture_width, 0x20, get_frame_texture(find_valid_animation_frame_index(id, frame_index)));
                    } else {
                        draw_hud_sprite_subpixel(find_valid_animation_frame_index(id, frame_index), icon_bounds[i].x0, icon_bounds[i].y0, icon_bounds[i].x1 - icon_bounds[i].x0,
                                      icon_bounds[i].y1 - icon_bounds[i].y0, 0x80);
                    }
                        if (D_001A00F0.icons[i].flags & 0x10) {
                        label_width = D_001A00F0.icons[i].label_width;
                        label_offset_x = D_001A00F0.icons[i].label_offset_x;
                        label_height = D_001A00F0.icons[i].label_height;
                        label_offset_y = D_001A00F0.icons[i].label_offset_y;
                        if (label_offset_x == 0) {
                            label_x = ((icon_bounds[i].x0 + icon_bounds[i].x1) >> 5) - label_width / 2;
                        } else {
                            label_x = ((label_offset_x > 0 ? icon_bounds[i].x1 : icon_bounds[i].x0) >> 4) - label_width / 2 + label_offset_x;
                        }
                        if (label_offset_y == 0) {
                            label_y = ((icon_bounds[i].y0 + icon_bounds[i].y1) >> 5) - label_height / 2;
                        } else {
                            label_y = ((label_offset_y > 0 ? icon_bounds[i].y1 : icon_bounds[i].y0) >> 4) - label_height / 2 + label_offset_y;
                        }
                        func_001F5F18(label_y, label_y + label_height, label_x, label_x + label_width, 0x40);
                        func_00208280(i, &buf);
                        memset(&text_window, 0, sizeof(text_window));
                        text_window.s[8] = 0xF;
                        text_window.s[1] = label_y + label_height;
                        text_window.s[3] = label_x + label_width;
                        text_window.s[4] = label_x + label_width / 2;
                        text_window.s[5] = label_y + 4;
                        text_window.s[9] = 1;
                        text_window.s[0] = label_y;
                        text_window.s[2] = label_x;
                        font_print_window_small(&text_window, 0x80FFA888, &buf, -1);
                    }
                }
                i++;
            } while (!(D_001A00F0.icons[i].flags & 4));
        }
        }
    }

    {
        MapState *m = &D_001A00F0;

    if (D_0015ED84 == m->selected_map) {
        s32 image_index;
        f32 s;
        f32 angle;
        s32 flip;
        

        image_index = find_valid_animation_frame_index(0xE99A, 5);
        s = ((m->zoom[m->selected_map] * 4.0f + 10.0f) * 0.75f) / 13.0f;
        angle = D_0013F350.angle;
        flip = D_0013F350.mode == 0xF;
        if (flip) {
            angle = func_001FA580(angle, 1.5707964f);
        }
        if (D_0015FD60 != 0) {
            func_00208408(&buf.f[0], &buf.f[1], D_0015ED84 + 100, D_0013F350.x, D_0013F350.y);
            angle = func_001FA580(angle, 1.5707964f);
        } else {
            func_00208408(&buf.f[0], &buf.f[1], D_0015ED84, D_0013F350.x, D_0013F350.y);
        }
        buf.f[0] = (f32)rx0 + buf.f[0] * (f32)(rx1 - rx0);
        buf.f[1] = (f32)ry0 + buf.f[1] * (f32)(ry1 - ry0);
        if (D_0015EDB4 != 0) {
            angle = func_001FA5C8(-func_001FA580(angle, 1.5707964f), 1.5707964f);
        }
        {
            f32 sz = s * 256.0f;

            func_00200600(buf.f[0], buf.f[1], sz, sz, angle, 0x40, 0x40, get_frame_texture(image_index));
        }
    }
    }
    do_gif_paging();
    if (D_001A00F0.grid != 0) {
        setup_gif_paging(0);
        func_00208508(rx0, ry0, rx1, ry1);
        do_gif_paging();
    }
}
extern __typeof__(draw_map_overlay) func_00205640 __attribute__((alias("FUN_00205640")));
#endif /* NON_MATCHING */
