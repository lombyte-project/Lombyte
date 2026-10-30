/* Ported from rac1-decomp (src/game/map.c: DrawMapView). */
#include "rnc/globals.h"
#include "types.h"
#include "rnc/ui/text/text_region.h"
#include "sda.h"

#include "rnc/hero/hero.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/ui/map/map_icon.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/rendering/vu1_registers.h"

typedef struct {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MapIconBounds;

#define SPR ((MapIconBounds *)0x70000000)

extern u8  D_0013D5BC[];
extern u16 occl_cam_param1[] __asm__("D_001518D2");
extern u8  D_0015EDB4;
extern s32 D_0015FD60 __attribute__((sda));
extern f32 D_0015FD80 __attribute__((sda));
extern f32 D_0015FD84 __attribute__((sda));
extern f32 D_0015FD88 __attribute__((sda));
extern f32 D_0015FD8C __attribute__((sda));
extern f32 D_0015FD90 __attribute__((sda));
extern f32 D_0015FD94 __attribute__((sda));
extern u8  no_map_available_text[] __asm__("D_001E8068");

extern void print_debug_text_centered(s32, s32, s32, u8 *) __asm__("func_001F0C50");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void draw_ui_frame(s32, s32, s32, s32, s32) __asm__("func_001F5F18");
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");
extern f32  fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32  fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern s32  get_icon_frame(s32, s32) __asm__("func_001FF960");
extern u64  get_frame_texture(s32) __asm__("func_001FFA10");
extern void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");
extern void draw_rotated_sprite(f32, f32, f32, f32, f32, s32, s32, u64) __asm__("func_00200600");
extern void append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");
extern void format_menu_item_text(s32, void *) __asm__("func_00208280");
extern void
world_to_map_coords(f32 x, f32 y, f32 *map_x, f32 *map_y, s32 level) __asm__("func_00208408");
extern void  draw_map_markers(s32, s32, s32, s32) __asm__("func_00208508");
extern void *memset(void *, s32, u32);

void draw_map_overlay(void) __asm__("FUN_00205640");

#define MAP_ICON_AT(offset) ((struct MapIcon *)((offset) + (u32)level_map_selection.icons))

void draw_map_overlay(void) {
    union {
        u8  b[0x80];
        f32 f[2];
    } label_buffer;
    struct TextRegion text_window;
    s32               map_left, map_top, map_right, map_bottom;
    struct DmaTag    *tag;
    u64              *packet_words;
    u64               background_tex0;
    u64               map_tex0;
    s32               palette_block;
    s32               mirror_sign;
    s32               tile_size;
    s32               left_tile_count, top_tile_count, right_tile_count, bottom_tile_count;
    s32               x0, y0, x1, y1;
    s32               u, v;
    s32               dx, dy;
    s32               i;
    s32               cell, cell_y, cell_x;
    MapIconBounds    *icon_bounds;
    f32               zoom;
    f32               icon_scale;
    f32               scale;
    s32               pan_x, map_extent, pan_y;
    s32               id, frame_index, texture_width;
    f32               angle, sprite_width, sprite_height, center_x, center_y;
    s16               label_width;
    s32               label_height, label_offset_x, label_offset_y;
    s32               label_x, label_y;
    s32               tile_limit;

    if (level_map_selection.unk24 == 0) {
        u8 *font = no_map_available_text;

        setup_gif_paging(0);
        print_debug_text_centered(0x100, (s16)occl_cam_param1[0] >> 1, 0x80909090, font);
        do_gif_paging();
        return;
    }
    if (level_map_selection.loaded < 0) {
        return;
    }

    map_left = 0x1000;
    setup_gif_paging(0);
    map_top = 0x800;
    mirror_sign = D_0015EDB4 ? -1 : 1;
    zoom = level_map_selection.zoom[level_map_selection.loaded];
    pan_y = zoom * (f32)(level_map_selection.pan_y[level_map_selection.loaded] >> 15);
    map_extent = zoom * 8192.0f;
    pan_x = zoom * (f32)(level_map_selection.pan_x[level_map_selection.loaded] >> 15);
    tile_size = zoom * 512.0f;
    tile_limit = tile_size + 0x2000;
    map_left -= pan_x * mirror_sign;
    map_top -= pan_y;
    map_right = map_left + map_extent * mirror_sign;
    map_bottom = map_top + map_extent;
    left_tile_count = (map_left + tile_size - 1) / tile_size;
    top_tile_count = (map_top + tile_size - 1) / tile_size;
    right_tile_count = (tile_limit - map_right - 1) / tile_size;
    bottom_tile_count = (tile_limit - map_bottom - 1) / tile_size;
    x0 = map_left - tile_size * left_tile_count;
    y0 = map_top - tile_size * top_tile_count;
    x1 = map_right + tile_size * right_tile_count;
    y1 = map_bottom + tile_size * bottom_tile_count;
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
    packet_words[6] = ((x0 + screen_extent.left) - 8) |
                      ((u64)((y0 + screen_extent.top) - 8) << 16) |
                      ((u64)level_map_selection.z << 32);
    packet_words[7] = u | ((u64)v << 16);
    packet_words[8] = ((x1 + screen_extent.left) - 8) |
                      ((u64)((y1 + screen_extent.top) - 8) << 16) |
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
    map_tex0 =
        (u64)((level_map_selection.map_image_vram >> 8) | (8 << 14) | (0x13 << 20) | (9 << 26)) |
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
    packet_words[6] = ((map_left + screen_extent.left) - 8) |
                      ((u64)((map_top + screen_extent.top) - 8) << 16) |
                      ((u64)level_map_selection.z << 32);
    packet_words[7] = 0x20002000;
    packet_words[8] = ((map_right + screen_extent.left) - 8) |
                      ((u64)((map_bottom + screen_extent.top) - 8) << 16) |
                      ((u64)level_map_selection.z << 32);
    packet_words[9] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x50);
    vu1_add_g_sregister(0x47, 0x360B);

    if (level_map_selection.unk2C != 0 && level_map_selection.unk8 != 0) {
        dx = map_right - map_left;
        dy = map_bottom - map_top;
        for (i = 0; i < 8; i++) {
            cell = level_map_selection.unk30[i].unk0;
            if (cell >= 0) {
                cell_x = cell % 16;
                cell_y = cell / 16;
                append_screen_rect_packet(map_left + cell_x * dx / 16,
                                          map_top + cell_y * dy / 16,
                                          map_left + (cell_x + 1) * dx / 16,
                                          map_top + (cell_y + 1) * dy / 16,
                                          0x20000000,
                                          1);
            }
        }
    }

    if (level_map_selection.icons != 0) {
        icon_bounds = SPR;
        icon_scale = (level_map_selection.zoom[level_map_selection.loaded] * 2.0f + 5.0f) / 13.0f;
        if (!(level_map_selection.icons[0].flags & 4)) {
            s32                i;
            f32                icon_size_multiplier;
            s32                image_index;
            f32                icon_center_x, icon_center_y;
            struct HudTexPage *texture_info;
            f32                scale;
            s32                texture_id;

            i = 0;
            do {
                if (level_map_selection.icons[i].active != 0 &&
                    (texture_id = level_map_selection.icons[i].texture_id) != 0 &&
                    !(level_map_selection.icons[i].flags & 1)) {
                    icon_size_multiplier = 1.0f;
                    if (level_map_selection.icons[i].flags & 0x80) {
                        icon_size_multiplier = 1.5f;
                    }
                    image_index =
                        get_icon_frame(texture_id, level_map_selection.icons[i].frame_index);
                    icon_center_x = (f32)map_left +
                                    level_map_selection.icons[i].x * (f32)(map_right - map_left);
                    icon_center_y =
                        (f32)map_top + level_map_selection.icons[i].y * (f32)(map_bottom - map_top);
                    texture_info =
                        &hud_state.image_pages[hud_state.frame_refs[image_index].image_index];
                    if (level_map_selection.icons[i].flags & 0x200) {
                        scale = level_map_selection.zoom[level_map_selection.loaded];
                    } else {
                        scale = icon_scale;
                    }
                    icon_bounds[i].x0 =
                        icon_center_x -
                        icon_size_multiplier * scale * (f32)(1 << (texture_info->width_log2 + 3));
                    icon_bounds[i].x1 =
                        (f32)icon_bounds[i].x0 +
                        icon_size_multiplier * scale * (f32)(1 << (texture_info->width_log2 + 4));
                    icon_bounds[i].y0 =
                        icon_center_y -
                        icon_size_multiplier * scale * (f32)(1 << (texture_info->height_log2 + 3));
                    icon_bounds[i].y1 =
                        (f32)icon_bounds[i].y0 +
                        icon_size_multiplier * scale * (f32)(1 << (texture_info->height_log2 + 4));
                }
                i++;
            } while (!(level_map_selection.icons[i].flags & 4));
        }

        {
            s32             i, j;
            s32             overlap_left, overlap_right, overlap_top, overlap_bottom;
            s32             first_dx, first_dy, second_dx, second_dy;
            struct MapIcon *icon;
            s32             inner_offset;
            s32             inner_icon_offset;
            s32             inner_stride;
            s32             outer_stride;

            /* The flag-4 entry terminates the list. Separate overlapping icon
               bounds before either icon is drawn, using half of the smallest
               positive edge distance for each pair. */
            i = 0;
            if (!(level_map_selection.icons[1].flags & 4)) {
                /* Fakematch: the strides in variables keep retail's address arithmetic. */
                outer_stride = sizeof(struct MapIcon);
                inner_stride = sizeof(struct MapIcon);
                do {
                    if (MAP_ICON_AT(i * outer_stride)->active == 0 ||
                        MAP_ICON_AT(i * outer_stride)->texture_id == 0 ||
                        (MAP_ICON_AT(i * outer_stride)->flags & 3)) {
                        goto next_outer_icon;
                    }
                    j = i + 1;
                    if (!(MAP_ICON_AT(j * outer_stride)->flags & 4)) {
                        inner_offset = j * inner_stride;
                        inner_icon_offset = inner_offset;
                        do {
                            overlap_left = icon_bounds[j].x1 - icon_bounds[i].x0;
                            if (overlap_left <= 0)
                                goto next_inner_icon;
                            overlap_right = icon_bounds[i].x1 - icon_bounds[j].x0;
                            if (overlap_right <= 0)
                                goto next_inner_icon;
                            overlap_top = icon_bounds[j].y1 - icon_bounds[i].y0;
                            if (overlap_top <= 0)
                                goto next_inner_icon;
                            overlap_bottom = icon_bounds[i].y1 - icon_bounds[j].y0;
                            if (overlap_bottom <= 0)
                                goto next_inner_icon;
                            icon = MAP_ICON_AT(inner_icon_offset);
                            if (icon->active == 0 || icon->texture_id == 0 || (icon->flags & 3)) {
                                goto next_inner_icon;
                            }
                            first_dx = 0;
                            first_dy = 0;
                            second_dx = 0;
                            second_dy = 0;
                            if (overlap_left <= overlap_right && overlap_left <= overlap_top &&
                                overlap_left <= overlap_bottom) {
                                first_dx = overlap_left >> 1;
                                second_dx = first_dx - overlap_left;
                            } else if (overlap_right <= overlap_top &&
                                       overlap_right <= overlap_bottom) {
                                second_dx = overlap_right >> 1;
                                first_dx = second_dx - overlap_right;
                            } else if (overlap_top <= overlap_bottom) {
                                first_dy = overlap_top >> 1;
                                second_dy = first_dy - overlap_top;
                            } else {
                                second_dy = overlap_bottom >> 1;
                                first_dy = second_dy - overlap_bottom;
                            }
                            icon_bounds[i].x0 += first_dx;
                            icon_bounds[i].x1 += first_dx;
                            icon_bounds[i].y0 += first_dy;
                            icon_bounds[i].y1 += first_dy;
                            icon_bounds[j].x0 += second_dx;
                            icon_bounds[j].x1 += second_dx;
                            icon_bounds[j].y0 += second_dy;
                            icon_bounds[j].y1 += second_dy;
                        next_inner_icon:
                            j++;
                            inner_offset += inner_stride;
                            inner_icon_offset += sizeof(struct MapIcon);
                        } while (!(MAP_ICON_AT(inner_offset)->flags & 4));
                    }
                next_outer_icon:
                    i++;
                } while (!(MAP_ICON_AT(i * outer_stride)[1].flags & 4));
            }
        }

        {
            s32 i;

            i = 0;
            if (!(level_map_selection.icons[0].flags & 4)) {
                do {
                    if (level_map_selection.icons[i].active != 0 &&
                        !(level_map_selection.icons[i].flags & 1) &&
                        ((id = level_map_selection.icons[i].texture_id),
                         (frame_index = level_map_selection.icons[i].frame_index),
                         id != 0)) {
                        if (level_map_selection.icons[i].flags & 0x40) {
                            append_screen_rect_packet(icon_bounds[i].x0 - 0x20,
                                                      icon_bounds[i].y0 - 0x20,
                                                      icon_bounds[i].x1 + 0x20,
                                                      icon_bounds[i].y1 + 0x20,
                                                      0x80000000,
                                                      1);
                        }
                        if (level_map_selection.icons[i].flags & 0x200) {
                            scale = level_map_selection.zoom[level_map_selection.loaded];
                        } else {
                            scale = icon_scale;
                        }
                        if (level_map_selection.icons[i].flags & 0x100) {
                            angle = level_map_selection.icons[i].angle;
                            texture_width = 0x20;
                            sprite_height = scale * 256.0f;
                            sprite_width = sprite_height;
                            if (level_map_selection.icons[i].flags & 0x400) {
                                angle = fast_add_rotations(angle, 1.5707964f);
                                texture_width = 0x40;
                                sprite_width = scale * D_0015FD88;
                                sprite_height = scale * D_0015FD8C;
                                if (*(s32 *)(D_0013D5BC + level_map_selection.icons[i].id * 16) &
                                    2) {
                                    frame_index++;
                                }
                            }
                            if (level_map_selection.icons[i].flags & 0x800) {
                                angle = fast_add_rotations(angle, 1.5707964f);
                                texture_width = 0x40;
                                sprite_width = scale * D_0015FD80;
                                sprite_height = scale * D_0015FD84;
                                if (*(s32 *)(D_0013D5BC + level_map_selection.icons[i].id * 16) &
                                    2) {
                                    frame_index++;
                                }
                            }
                            if (level_map_selection.icons[i].flags & 0x1000) {
                                angle = fast_add_rotations(angle, 1.5707964f);
                                sprite_width = scale * D_0015FD90;
                                sprite_height = scale * D_0015FD94;
                            }
                            center_x = (f32)(icon_bounds[i].x0 + icon_bounds[i].x1) * 0.5f;
                            center_y = (f32)(icon_bounds[i].y0 + icon_bounds[i].y1) * 0.5f;
                            draw_rotated_sprite(center_x,
                                                center_y,
                                                sprite_width,
                                                sprite_height,
                                                angle,
                                                texture_width,
                                                0x20,
                                                get_frame_texture(get_icon_frame(id, frame_index)));
                        } else {
                            draw_hud_sprite_subpixel(get_icon_frame(id, frame_index),
                                                     icon_bounds[i].x0,
                                                     icon_bounds[i].y0,
                                                     icon_bounds[i].x1 - icon_bounds[i].x0,
                                                     icon_bounds[i].y1 - icon_bounds[i].y0,
                                                     0x80);
                        }
                        if (level_map_selection.icons[i].flags & 0x10) {
                            label_width = level_map_selection.icons[i].label_width;
                            label_offset_x = level_map_selection.icons[i].label_offset_x;
                            label_height = (s16)level_map_selection.icons[i].label_height;
                            label_offset_y = level_map_selection.icons[i].label_offset_y;
                            if (label_offset_x == 0) {
                                label_x = ((icon_bounds[i].x0 + icon_bounds[i].x1) >> 5) -
                                          label_width / 2;
                            } else {
                                if (label_offset_x > 0) {
                                    label_x =
                                        (icon_bounds[i].x1 >> 4) - label_width / 2 + label_offset_x;
                                } else {
                                    label_x =
                                        (icon_bounds[i].x0 >> 4) - label_width / 2 + label_offset_x;
                                }
                            }
                            if (label_offset_y == 0) {
                                label_y = ((icon_bounds[i].y0 + icon_bounds[i].y1) >> 5) -
                                          label_height / 2;
                            } else {
                                if (label_offset_y > 0) {
                                    label_y = (icon_bounds[i].y1 >> 4) - label_height / 2 +
                                              label_offset_y;
                                } else {
                                    label_y = (icon_bounds[i].y0 >> 4) - label_height / 2 +
                                              label_offset_y;
                                }
                            }
                            draw_ui_frame(label_y,
                                          label_y + label_height,
                                          label_x,
                                          label_x + label_width,
                                          0x40);
                            format_menu_item_text(i, &label_buffer);
                            memset(&text_window, 0, sizeof(text_window));
                            text_window.bottom = label_y + label_height;
                            text_window.right = label_x + label_width;
                            text_window.anchor_x = label_x + label_width / 2;
                            text_window.anchor_y = label_y + 4;
                            text_window.line_advance = 0xF;
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
        struct MapState *map_state = &level_map_selection;

        if (current_level_index == map_state->loaded) {
            s32 image_index;
            f32 scale;
            f32 angle;
            s32 flip;

            image_index = get_icon_frame(0xE99A, 5);
            scale = ((map_state->zoom[map_state->loaded] * 4.0f + 10.0f) * 0.75f) / 13.0f;
            angle = hero.motion.rot.f[2];
            flip = hero.state.control_mode == 0xF;
            if (flip) {
                angle = fast_add_rotations(angle, 1.5707964f);
            }
            if (D_0015FD60 != 0) {
                world_to_map_coords(hero.motion.pos.f[0],
                                    hero.motion.pos.f[1],
                                    &label_buffer.f[0],
                                    &label_buffer.f[1],
                                    current_level_index + 100);
                angle = fast_add_rotations(angle, 1.5707964f);
            } else {
                world_to_map_coords(hero.motion.pos.f[0],
                                    hero.motion.pos.f[1],
                                    &label_buffer.f[0],
                                    &label_buffer.f[1],
                                    current_level_index);
            }
            label_buffer.f[0] = (f32)map_left + label_buffer.f[0] * (f32)(map_right - map_left);
            label_buffer.f[1] = (f32)map_top + label_buffer.f[1] * (f32)(map_bottom - map_top);
            if (D_0015EDB4 != 0) {
                angle = fast_subtract_rotations(-fast_add_rotations(angle, 1.5707964f), 1.5707964f);
            }
            {
                f32 sprite_size = scale * 256.0f;

                draw_rotated_sprite(label_buffer.f[0],
                                    label_buffer.f[1],
                                    sprite_size,
                                    sprite_size,
                                    angle,
                                    0x40,
                                    0x40,
                                    get_frame_texture(image_index));
            }
        }
    }
    do_gif_paging();
    if (level_map_selection.markers != 0) {
        setup_gif_paging(0);
        draw_map_markers(map_left, map_top, map_right, map_bottom);
        do_gif_paging();
    }
}

extern __typeof__(draw_map_overlay) func_00205640 __attribute__((alias("FUN_00205640")));
