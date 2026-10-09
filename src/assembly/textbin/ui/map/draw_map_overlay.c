#include "types.h"
#include "asm.h"
#include "rnc/globals.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/map/draw_map_overlay/FUN_00205640.s",
            FUN_00205640);
#else
#include "types.h"
#include "rnc/ui/text/text_region.h"
#include "sda.h"

#include "rnc/gameplay/hero.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/ui/map/map_icon.h"
#include "rnc/ui/map/map_state.h"

typedef struct {
    s32 cell;
    s32 pad[3];
} MapHighlightedCell;

typedef struct {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MapIconBounds;

#define SPR ((MapIconBounds *)0x70000000)

extern u8 D_0013D5BC[];
extern u16 D_001518D2[];
extern u8 D_0015EDB4;
extern s32 D_0015FD60 __attribute__((sda));
extern f32 D_0015FD80 __attribute__((sda));
extern f32 D_0015FD84 __attribute__((sda));
extern f32 D_0015FD88 __attribute__((sda));
extern f32 D_0015FD8C __attribute__((sda));
extern f32 D_0015FD90 __attribute__((sda));
extern f32 D_0015FD94 __attribute__((sda));
extern u8 D_001E8068[];

extern void print_debug_text_centered(s32, s32, s32, u8 *) __asm__("func_001F0C50");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void draw_ui_frame(s32, s32, s32, s32, s32) __asm__("func_001F5F18");
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern s32 get_icon_frame(s32, s32) __asm__("func_001FF960");
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");
extern void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");
extern void draw_rotated_sprite(f32, f32, f32, f32, f32, s32, s32, u64) __asm__("func_00200600");
extern void append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");
extern void format_menu_item_text(s32, void *) __asm__("func_00208280");
extern void world_to_map_coords(f32 *, f32 *, s32, f32, f32) __asm__("func_00208408");
extern void draw_map_markers(s32, s32, s32, s32) __asm__("func_00208508");
extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");
extern void *memset(void *, s32, u32);

void draw_map_overlay(void) __asm__("FUN_00205640");

extern struct MapState D_001A00F0_far __asm__("D_001A00F0") __attribute__((section(".data")));
void draw_map_overlay(void) {
    union {
        u8 b[0x80];
        f32 f[2];
    } label_buffer;
    struct TextRegion text_window;
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
    MapHighlightedCell *highlighted_cell;
    MapIconBounds *r;
    MapIconBounds *icon_bounds;
    struct MapIcon *icon;
    struct HudTexPage *texture_info;
    f32 zoom;
    f32 icon_scale;
    f32 icon_size_multiplier;
    f32 s;
    f32 icon_center_x, icon_center_y;
    s32 pan_x, map_extent, pan_y, oy1;
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

    if (level_map_selection.unk24 == 0) {
        u8 *font = D_001E8068;

        setup_gif_paging(0);
        print_debug_text_centered(0x100, (s16)D_001518D2[0] >> 1, 0x80909090, font);
        do_gif_paging();
        return;
    }
    if (level_map_selection.loaded < 0) {
        return;
    }

    rx0 = 0x1000;
    setup_gif_paging(0);
    ry0 = 0x800;
    mirror_sign = D_0015EDB4 ? -1 : 1;
    zoom = level_map_selection.zoom[level_map_selection.loaded];
    pan_y = zoom * (f32)(level_map_selection.pan_y[level_map_selection.loaded] >> 15);
    map_extent = zoom * 8192.0f;
    pan_x = zoom * (f32)(level_map_selection.pan_x[level_map_selection.loaded] >> 15);
    tile_size = zoom * 512.0f;
    tile_limit = tile_size + 0x2000;
    rx0 -= pan_x * mirror_sign;
    ry0 -= pan_y;
    rx1 = rx0 + map_extent * mirror_sign;
    ry1 = ry0 + map_extent;
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
    render_packet_cursor.tag->tag = 0x10000005;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000005;
    render_packet_cursor.tag++;
    background_tex0 = get_frame_texture(get_icon_frame(0xE999, level_map_selection.loaded));
    packet_words = (u64 *)render_packet_cursor.tag;
    packet_words[0] = 0x7400000000008001;
    packet_words[1] = 0x5353106;
    packet_words[2] = background_tex0;
    packet_words[3] = 0x156;
    packet_words[4] = 0x80808080;
    packet_words[5] = 0;
    packet_words[6] = ((x0 + screen_extent.left) - 8) | ((u64)((y0 + screen_extent.top) - 8) << 16) |
                      ((u64)level_map_selection.z << 32);
    packet_words[7] = u | ((u64)v << 16);
    packet_words[8] = ((x1 + screen_extent.left) - 8) | ((u64)((y1 + screen_extent.top) - 8) << 16) |
                      ((u64)level_map_selection.z << 32);
    packet_words[9] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x50);
    vu1_add_g_sregister(8, 5);
    vu1_add_g_sregister(0x47, 0x60B);

    palette_block = (background_tex0 >> 37) & 0x3FFF;
    render_packet_cursor.tag->tag = 0x10000005;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000005;
    map_tex0 = (u64)((level_map_selection.map_image_vram >> 8) | (8 << 14) | (0x13 << 20) | (9 << 26)) |
               ((u64)9 << 30) | ((u64)1 << 34) | ((u64)palette_block << 37) |
               ((long)0x8000000000000000ULL);
    tag = render_packet_cursor.tag;
    render_packet_cursor.tag = tag + 1;
    packet_words = (u64 *)(tag + 1);
    packet_words[0] = 0x7400000000008001;
    packet_words[1] = 0x5353106;
    packet_words[2] = map_tex0;
    packet_words[3] = 0x156;
    packet_words[4] = 0x80808080;
    packet_words[5] = 0;
    packet_words[6] = ((rx0 + screen_extent.left) - 8) | ((u64)((ry0 + screen_extent.top) - 8) << 16) |
                      ((u64)level_map_selection.z << 32);
    packet_words[7] = 0x20002000;
    packet_words[8] = ((rx1 + screen_extent.left) - 8) | ((u64)((ry1 + screen_extent.top) - 8) << 16) |
                      ((u64)level_map_selection.z << 32);
    packet_words[9] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x50);
    vu1_add_g_sregister(0x47, 0x360B);

    if (level_map_selection.unk2C != 0 && level_map_selection.unk8 != 0) {
        dx = rx1 - rx0;
        dy = ry1 - ry0;
        for (i = 0; i < 8; i++) {
            cell = level_map_selection.unk30[i].unk0;
            if (cell >= 0) {
                cell_x = cell % 16;
                cell_y = cell / 16;
                append_screen_rect_packet(rx0 + cell_x * dx / 16, ry0 + cell_y * dy / 16,
                                          rx0 + (cell_x + 1) * dx / 16,
                                          ry0 + (cell_y + 1) * dy / 16, 0x20000000, 1);
            }
        }
    }

    if (level_map_selection.icons != 0) {
        icon_bounds = SPR;
        icon_scale = (level_map_selection.zoom[level_map_selection.loaded] * 2.0f + 5.0f) / 13.0f;
        if (!(level_map_selection.icons[0].flags & 4)) {
            MapIconBounds *r;
            s32 i;
            struct MapIcon *icon;
            f32 icon_size_multiplier;
            s32 image_index;
            f32 icon_center_x, icon_center_y;
            struct HudTexPage *texture_info;
            f32 s;
            s32 texture_id;

            i = 0;
            do {
                if (level_map_selection.icons[i].active != 0 &&
                    (texture_id = level_map_selection.icons[i].texture_id) != 0 &&
                    !(level_map_selection.icons[i].flags & 1)) {
                    icon_size_multiplier = 1.0f;
                    if (level_map_selection.icons[i].flags & 0x80) {
                        icon_size_multiplier = 1.5f;
                    }
                    image_index = get_icon_frame(texture_id,
                                                                   level_map_selection.icons[i].frame_index);
                    icon_center_x = (f32)rx0 + level_map_selection.icons[i].x * (f32)(rx1 - rx0);
                    icon_center_y = (f32)ry0 + level_map_selection.icons[i].y * (f32)(ry1 - ry0);
                    texture_info =
                        &hud_state.image_pages[hud_state.frame_refs[image_index].image_index];
                    if (level_map_selection.icons[i].flags & 0x200) {
                        s = level_map_selection.zoom[level_map_selection.loaded];
                    } else {
                        s = icon_scale;
                    }
                    icon_bounds[i].x0 =
                        icon_center_x -
                        icon_size_multiplier * s * (f32)(1 << (texture_info->width_log2 + 3));
                    icon_bounds[i].x1 =
                        (f32)icon_bounds[i].x0 +
                        icon_size_multiplier * s * (f32)(1 << (texture_info->width_log2 + 4));
                    icon_bounds[i].y0 =
                        icon_center_y -
                        icon_size_multiplier * s * (f32)(1 << (texture_info->height_log2 + 3));
                    icon_bounds[i].y1 =
                        (f32)icon_bounds[i].y0 +
                        icon_size_multiplier * s * (f32)(1 << (texture_info->height_log2 + 4));
                }
                i++;
            } while (!(level_map_selection.icons[i].flags & 4));
        }

        {
            s32 i, j;
            s32 pan_x, map_extent, pan_y, oy1;
            s32 ax, ay, bx, by;
            struct MapIcon *icon;
            s32 inner_offset;
            s32 inner_stride;

            /* The flag-4 entry terminates the list. Separate overlapping icon
               bounds before either icon is drawn, using half of the smallest
               positive edge distance for each pair. */
            i = 0;
            if (!(level_map_selection.icons[1].flags & 4)) {
                inner_stride = sizeof(struct MapIcon);
                do {
                    if (level_map_selection.icons[i].active == 0 || level_map_selection.icons[i].texture_id == 0 ||
                        (level_map_selection.icons[i].flags & 3)) {
                        goto next_outer_icon;
                    }
                    j = i + 1;
                    inner_offset = j * inner_stride;
                    for (;
                         !(((struct MapIcon *)((u8 *)level_map_selection.icons + inner_offset))->flags & 4);
                         j++, inner_offset += inner_stride) {
                        pan_x = icon_bounds[j].x1 - icon_bounds[i].x0;
                        if (pan_x <= 0)
                            continue;
                        map_extent = icon_bounds[i].x1 - icon_bounds[j].x0;
                        if (map_extent <= 0)
                            continue;
                        pan_y = icon_bounds[j].y1 - icon_bounds[i].y0;
                        if (pan_y <= 0)
                            continue;
                        oy1 = icon_bounds[i].y1 - icon_bounds[j].y0;
                        if (oy1 <= 0)
                            continue;
                        if (level_map_selection.icons[j].active == 0 ||
                            level_map_selection.icons[j].texture_id == 0 ||
                            (level_map_selection.icons[j].flags & 3)) {
                            continue;
                        }
                        ax = 0;
                        ay = 0;
                        bx = 0;
                        by = 0;
                        if (pan_x <= map_extent && pan_x <= pan_y && pan_x <= oy1) {
                            ax = pan_x >> 1;
                            bx = ax - pan_x;
                        } else if (map_extent <= pan_y && map_extent <= oy1) {
                            bx = map_extent >> 1;
                            ax = bx - map_extent;
                        } else if (pan_y <= oy1) {
                            ay = pan_y >> 1;
                            by = ay - pan_y;
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
                next_outer_icon:
                    i++;
                } while (!(level_map_selection.icons[i + 1].flags & 4));
            }
        }

        {
            s32 i;

            i = 0;
            if (!(level_map_selection.icons[0].flags & 4)) {
                do {
                    if (level_map_selection.icons[i].active != 0 && !(level_map_selection.icons[i].flags & 1) &&
                        (id = level_map_selection.icons[i].texture_id) != 0) {
                        frame_index = level_map_selection.icons[i].frame_index;
                        if (level_map_selection.icons[i].flags & 0x40) {
                            append_screen_rect_packet(
                                icon_bounds[i].x0 - 0x20, icon_bounds[i].y0 - 0x20,
                                icon_bounds[i].x1 + 0x20, icon_bounds[i].y1 + 0x20, 0x80000000, 1);
                        }
                        if (level_map_selection.icons[i].flags & 0x200) {
                            s = level_map_selection.zoom[level_map_selection.loaded];
                        } else {
                            s = icon_scale;
                        }
                        if (level_map_selection.icons[i].flags & 0x100) {
                            angle = level_map_selection.icons[i].angle;
                            texture_width = 0x20;
                            sprite_height = s * 256.0f;
                            sprite_width = sprite_height;
                            if (level_map_selection.icons[i].flags & 0x400) {
                                angle = fast_add_rotations(angle, 1.5707964f);
                                texture_width = 0x40;
                                sprite_width = s * D_0015FD88;
                                sprite_height = s * D_0015FD8C;
                                if (*(s32 *)(D_0013D5BC + level_map_selection.icons[i].id * 16) & 2) {
                                    frame_index++;
                                }
                            }
                            if (level_map_selection.icons[i].flags & 0x800) {
                                angle = fast_add_rotations(angle, 1.5707964f);
                                texture_width = 0x40;
                                sprite_width = s * D_0015FD80;
                                sprite_height = s * D_0015FD84;
                                if (*(s32 *)(D_0013D5BC + level_map_selection.icons[i].id * 16) & 2) {
                                    frame_index++;
                                }
                            }
                            if (level_map_selection.icons[i].flags & 0x1000) {
                                angle = fast_add_rotations(angle, 1.5707964f);
                                sprite_width = s * D_0015FD90;
                                sprite_height = s * D_0015FD94;
                            }
                            center_x = (f32)(icon_bounds[i].x0 + icon_bounds[i].x1) * 0.5f;
                            center_y = (f32)(icon_bounds[i].y0 + icon_bounds[i].y1) * 0.5f;
                            draw_rotated_sprite(center_x, center_y, sprite_width, sprite_height,
                                                angle, texture_width, 0x20,
                                                get_frame_texture(get_icon_frame(
                                                    id, frame_index)));
                        } else {
                            draw_hud_sprite_subpixel(
                                get_icon_frame(id, frame_index),
                                icon_bounds[i].x0, icon_bounds[i].y0,
                                icon_bounds[i].x1 - icon_bounds[i].x0,
                                icon_bounds[i].y1 - icon_bounds[i].y0, 0x80);
                        }
                        if (level_map_selection.icons[i].flags & 0x10) {
                            label_width = level_map_selection.icons[i].label_width;
                            label_offset_x = level_map_selection.icons[i].label_offset_x;
                            label_height = level_map_selection.icons[i].label_height;
                            label_offset_y = level_map_selection.icons[i].label_offset_y;
                            if (label_offset_x == 0) {
                                label_x = ((icon_bounds[i].x0 + icon_bounds[i].x1) >> 5) -
                                          label_width / 2;
                            } else {
                                label_x =
                                    ((label_offset_x > 0 ? icon_bounds[i].x1 : icon_bounds[i].x0) >>
                                     4) -
                                    label_width / 2 + label_offset_x;
                            }
                            if (label_offset_y == 0) {
                                label_y = ((icon_bounds[i].y0 + icon_bounds[i].y1) >> 5) -
                                          label_height / 2;
                            } else {
                                label_y =
                                    ((label_offset_y > 0 ? icon_bounds[i].y1 : icon_bounds[i].y0) >>
                                     4) -
                                    label_height / 2 + label_offset_y;
                            }
                            draw_ui_frame(label_y, label_y + label_height, label_x,
                                          label_x + label_width, 0x40);
                            format_menu_item_text(i, &label_buffer);
                            memset(&text_window, 0, sizeof(text_window));
                            text_window.line_advance = 0xF;
                            text_window.bottom = label_y + label_height;
                            text_window.right = label_x + label_width;
                            text_window.anchor_x = label_x + label_width / 2;
                            text_window.anchor_y = label_y + 4;
                            text_window.flags = 1;
                            text_window.top = label_y;
                            text_window.left = label_x;
                            font_print_window_small(&text_window, 0x80FFA888, &label_buffer, -1);
                        }
                    }
                    i++;
                } while (!(level_map_selection.icons[i].flags & 4));
            }
        }
    }

    {
        struct MapState *m = &level_map_selection;

        if (current_level_index == m->loaded) {
            s32 image_index;
            f32 s;
            f32 angle;
            s32 flip;

            image_index = get_icon_frame(0xE99A, 5);
            s = ((m->zoom[m->loaded] * 4.0f + 10.0f) * 0.75f) / 13.0f;
            angle = hero.motion.rot.f[2];
            flip = hero.state.control_mode == 0xF;
            if (flip) {
                angle = fast_add_rotations(angle, 1.5707964f);
            }
            if (D_0015FD60 != 0) {
                world_to_map_coords(&label_buffer.f[0], &label_buffer.f[1], current_level_index + 100,
                                    hero.motion.pos.f[0], hero.motion.pos.f[1]);
                angle = fast_add_rotations(angle, 1.5707964f);
            } else {
                world_to_map_coords(&label_buffer.f[0], &label_buffer.f[1], current_level_index,
                                    hero.motion.pos.f[0], hero.motion.pos.f[1]);
            }
            label_buffer.f[0] = (f32)rx0 + label_buffer.f[0] * (f32)(rx1 - rx0);
            label_buffer.f[1] = (f32)ry0 + label_buffer.f[1] * (f32)(ry1 - ry0);
            if (D_0015EDB4 != 0) {
                angle = fast_subtract_rotations(-fast_add_rotations(angle, 1.5707964f), 1.5707964f);
            }
            {
                f32 sz = s * 256.0f;

                draw_rotated_sprite(label_buffer.f[0], label_buffer.f[1], sz, sz, angle, 0x40, 0x40,
                                    get_frame_texture(image_index));
            }
        }
    }
    do_gif_paging();
    if (level_map_selection.markers != 0) {
        setup_gif_paging(0);
        draw_map_markers(rx0, ry0, rx1, ry1);
        do_gif_paging();
    }
}
extern __typeof__(draw_map_overlay) func_00205640 __attribute__((alias("FUN_00205640")));
#endif /* NON_MATCHING */
