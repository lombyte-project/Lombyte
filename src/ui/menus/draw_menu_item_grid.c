#include "types.h"

typedef struct {
    u8 pad_0[0x40];
    f32 w;
    f32 h;
} Font;

typedef struct {
    u8 pad_0[0x78];
    Font *font;
} FontHolder;

typedef struct {
    s32 slot;
    u8 pad_4[0x48];
} ItemInfo;

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

extern ItemInfo D_001863D8[];
extern u8 D_0013D388[];
extern u8 D_0013D4C0[];
extern u8 D_0013E520[];
extern s32 D_0015F438;
extern s32 D_001601B0 __attribute__((sda));
extern f32 D_00160290 __attribute__((sda));
extern f32 D_00160294 __attribute__((sda));

extern s32 SubtractIntegerWithClamp(s32);
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 get_icon_frame(s32, s32) __asm__("FUN_001ff960");
extern void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");
extern void append_screen_sprite(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");
extern s32 vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");

s32 draw_menu_item_grid(struct MenuScreen *grid) __asm__("FUN_0021d948");

s32 draw_menu_item_grid(struct MenuScreen *grid) {
    Font *font;
    struct MenuGridCell *cell;
    s32 focused;
    f32 start_x, column_step, start_y, y, row_step, x;
    f32 scale;
    s32 largest_dimension;
    s32 icon_width, icon_height;
    s32 i, j;
    s32 left, top, right, bottom;
    s32 id, frame_offset;
    u32 color;

    font = ((FontHolder *)grid->moby)->font;
    focused = menu_system.current->focus == grid;
    cell = grid->data.grid.cells;
    vu1_add_g_sregister(0x42, 0x8000000044L);
    vu1_add_g_sregister(0x47, 0xB);
    setup_gif_paging(0);

    if (grid->data.grid.cols >= 2) {
        start_x = grid->data.grid.margin_x;
        column_step = D_00160290 +
                      (font->w - (start_x + start_x) - D_00160290 * grid->data.grid.cols) / (grid->data.grid.cols - 1);
    } else {
        column_step = 0.0f;
        start_x = (font->w - D_00160290) * 0.5f;
    }

    if (grid->data.grid.flags & 2) {
        row_step = D_00160294 + 0.15f;
        start_y = grid->data.grid.margin_y;
    } else if (grid->data.grid.rows >= 2) {
        start_y = grid->data.grid.margin_y;
        row_step = D_00160294 +
                   (font->h - (start_y + start_y) - D_00160294 * grid->data.grid.rows) / (grid->data.grid.rows - 1);
    } else {
        row_step = 0.0f;
        start_y = (font->h - D_00160294) * 0.5f;
    }

    largest_dimension = grid->height;
    if (largest_dimension < grid->width) {
        largest_dimension = grid->width;
    }
    scale = (f32)(largest_dimension << 4) / (font->h < font->w ? font->w : font->h);
    icon_width = scale * D_00160290;
    icon_height = scale * D_00160294;

    y = start_y;
    for (i = 0; i < grid->data.grid.rows; i++) {
        x = start_x;
        for (j = 0; j < grid->data.grid.cols; j++) {
            top = scale * y;
            bottom = top + icon_height;
            left = scale * x;
            right = left + icon_width;
            if (focused && grid->data.grid.selected_cell == cell - grid->data.grid.cells) {
                color = ((SubtractIntegerWithClamp((D_0015F438 & 0x3F) - 0x20) + 0x40) * 0x10202) |
                        0x80000000;
                append_screen_sprite(left - 0x30, top - 0x30, right + 0x30, bottom + 0x30, color,
                                     1);
                append_screen_sprite(left - 0x10, top - 0x10, right + 0x10, bottom + 0x10,
                                     D_001601B0, 1);
            }
            if (cell->kind == 0 ? D_0013D4C0[cell->id] : D_0013D388[cell->id]) {
                frame_offset = 0;
                if ((u16)cell->kind == 0) {
                    id = cell->id;
                    if (menu_system.equipped[D_001863D8[id].slot] == id && !(grid->data.grid.flags & 0x20)) {
                        frame_offset = 1;
                    }
                    if (frame_offset == 0) {
                        frame_offset = D_0013E520[id] ? 4 : 0;
                    }
                    if (menu_system.unk134 != 0 && (grid->data.grid.flags & 8)) {
                        frame_offset = 2;
                    }
                    if (menu_system.unk138 != 0 && (grid->data.grid.flags & 4)) {
                        frame_offset = 2;
                    }
                }
                draw_hud_sprite_subpixel(
                    get_icon_frame(cell->icon, cell->frame + frame_offset), left,
                    top, icon_width, icon_height, 0x80);
            }
            cell++;
            x += column_step;
        }
        y += row_step;
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_menu_item_grid) func_0021D948 __attribute__((alias("FUN_0021d948")));
