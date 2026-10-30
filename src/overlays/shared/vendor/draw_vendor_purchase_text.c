/* Vendor: draws the purchase text; also the pool splash. */
#include "types.h"
#include "rnc/overlay/vendor_menu.h"
#include "asm.h"
#include "sda.h"

#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/game/item_state.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/ui/help/get_help_message_text.h"
#include "rnc/ui/text/font/font_print_small.h"
#include "rnc/ui/text/font/font_print_window_small.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_0029FD68.c: func_L00_0029FD68). */

typedef struct {
    s16 unused0, y, unused4, x, half_x, half_y, left, top, width, style;
    u8  rest[4];
} VendorTextBox;

extern VendorMenu vendor_menu __asm__("D_L00_001CA4C0");
extern s32        current_bolt_count_macro __asm__("D_0015ED98") MACRO_ADDR;
extern s32        D_L00_00161E34_macro __asm__("D_L00_00161E34") MACRO_ADDR;
extern char       vendor_purchase_decimal_format[] __asm__("D_L00_00161160");
extern char       vendor_ammo_quantity_left_arrow_text[] __asm__("D_L00_00161168");
extern char       vendor_ammo_quantity_right_arrow_text[] __asm__("D_L00_00161170");
extern void       draw_framebuffer_rect(s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_001fb8f0");
extern void      *memset(void *, s32, s32) __asm__("FUN_001153fc");
extern void       font_print_right_small(s32, s32, u64, char *, s32) __asm__("FUN_001f69d0");
extern void       draw_moby_list(s32, s32) __asm__("FUN_0020d330");
extern s32        sprintf(char *, const char *, ...) __asm__("FUN_00116248");

#define SELECTED_ITEM D_L00_001C40B0[vendor_menu.rows[vendor_menu.cursor].item]

void draw_vendor_purchase_text(void *moby, s32 x, s32 y) __asm__("FUN_L00_0029ead8");

void draw_vendor_purchase_text(void *moby, s32 x, s32 y) {
    VendorTextBox box;
    char          text[256];
    s32           cost;
    s32           message_id;
    s32           text_x;
    u16           ammo_cost;
    char         *format;
    s32           total;

    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    {
        s32 size = sizeof(box);
        memset((void *)&box, 0, size);
    }
    box.half_x = x / 2;
    box.half_y = y / 2;
    box.left = x - 8;
    box.top = y - 8;
    box.width = 0x10;
    box.style = 3;
    box.y = (s16)y;
    box.x = (s16)x;

    if (vendor_menu.rows[vendor_menu.cursor].is_ammo == 1 &&
        weapon_ammo_counts[vendor_menu.rows[vendor_menu.cursor].item] >= SELECTED_ITEM.hE) {
        message_id = 0x5236;
        goto message;
    }
    if (vendor_menu.rows[vendor_menu.cursor].is_ammo != 1)
        goto purchase;
    if (vendor_menu.mode)
        ammo_cost = SELECTED_ITEM.hA;
    else
        ammo_cost = SELECTED_ITEM.h8;
    if (current_bolt_count_macro < ammo_cost)
        goto no_money;
    draw_moby_list((s32)vendor_menu.menu_mobys + 0x500, 1);
    box.half_y = 6;
    box.style = 1;
    font_print_window_small((void *)&box, 0x80F0F0F0L, get_help_message_text(0x5237), -1);
    sprintf(text, vendor_purchase_decimal_format, D_L00_00161E34_macro);
    if (D_L00_00161E34_macro >= 100)
        text_x = 0x17;
    else {
        if (D_L00_00161E34_macro < 10)
            goto quantity_small;
        text_x = 0x1e;
    }
    font_print_small(text_x, 0x32, 0x80F0F0F0L, text, -1);
    goto quantity_done;
quantity_small:
    font_print_small(0x25, 0x32, 0x80F0F0F0L, text, -1);
quantity_done:
    font_print_small(0xc, 0x32, 0x80F0F0F0L, vendor_ammo_quantity_left_arrow_text, -1);
    font_print_small(0x40, 0x32, 0x80F0F0F0L, vendor_ammo_quantity_right_arrow_text, -1);
    format = vendor_purchase_decimal_format;
    if (vendor_menu.mode)
        total = D_L00_00161E34_macro * SELECTED_ITEM.hA;
    else
        total = D_L00_00161E34_macro * SELECTED_ITEM.h8;
    sprintf(text, format, total);
    font_print_right_small(0x40, 0x4c, 0x80F0F0F0L, text, -1);
    return;
no_money:
    message_id = 0x5238;
message:
    font_print_window_small((void *)&box, 0x80F0F0F0L, get_help_message_text(message_id), -1);
    return;
purchase:
    cost = discount_purchase_pricing[0] ? SELECTED_ITEM.f4 : SELECTED_ITEM.f0;
    if (current_bolt_count_macro >= cost) {
        font_print_small(6, 6, 0x80F0F0F0L, get_help_message_text(0x5239), -1);
        font_print_small(2, 0x20, 0x80F0F0F0L, get_help_message_text(0x524E), -1);
        font_print_small(2, 0x3e, 0x80F0F0F0L, get_help_message_text(0x524B), -1);
    } else {
        font_print_window_small((void *)&box, 0x80F0F0F0L, get_help_message_text(0x5238), -1);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A5158), where it is exact; names translated to the US level program. */

typedef struct {
    f32 x, z;
    u8  pad_8[4];
    u8  neighbours[8];
    u8  pad_14[0x3C];
    f32 heights[2][23][16];
    u8  pad_BD0[0x1190 - 0xBD0];
} HeightTile;

typedef struct {
    u8  pad_0[8];
    f32 origin_x, origin_z, cell_width, cell_height;
} HeightGridLayout;

extern HeightGridLayout D_L00_001CA880;
extern s32              D_L00_00161218 __attribute__((sda));
extern s32              D_L00_00161220 __attribute__((sda));

/* Splash on the pool height tiles: raises the heights around (x, z) and copies the edges into the neighbour tiles. */
void pool_splash(HeightTile *tile,
                 int         count,
                 int         mode,
                 float       x,
                 float       z,
                 float       radius,
                 float       amount) __asm__("FUN_L00_002a3ec8");

void pool_splash(

    HeightTile *tile, int count, int mode, float x, float z, float radius, float amount) {
    HeightTile *tiles = tile;
    int         row;
    int         i;

    for (i = 0; i < count; i++) {
        float tile_z = tile->z + D_L00_001CA880.origin_z;
        float cell_z = (z - tile_z) / D_L00_001CA880.cell_height;
        float cell_x = (x - (tile->x + D_L00_001CA880.origin_x)) / D_L00_001CA880.cell_width;
        int   row_first, row_last, col_first, col_last;
        float row_z;

        row_first = (int)(cell_z - radius);
        row_last = (int)(cell_z + radius);
        if (row_first < 0)
            row_first = 0;
        if (row_last >= 16)
            row_last = 15;
        col_first = (int)(cell_x - radius);
        col_last = (int)(cell_x + radius);
        if (col_last >= 16)
            col_last = 15;
        if (col_first < 0)
            col_first = 0;
        row_z = tile_z + D_L00_001CA880.cell_height * (float)row_first;

        for (row = row_first; row <= row_last; row++) {
            float col_x =
                (tile->x + D_L00_001CA880.origin_x) + D_L00_001CA880.cell_width * (float)col_first;
            int col;

            for (col = col_first; col <= col_last; col++) {
                float dist_sq = (x - col_x) * (x - col_x) + (z - row_z) * (z - row_z);
                float height;

                if (dist_sq < 1.0f) {
                    height = amount;
                } else {
                    height = amount / square_root(dist_sq);
                }
                if (mode == 0) {
                    float magnitude = absolute_float(height);
                    if (magnitude < absolute_float(tile->heights[D_L00_00161218][row][col])) {
                        height = tile->heights[D_L00_00161218][row][col];
                    }
                } else {
                    height += tile->heights[D_L00_00161220][row][col];
                }
                {
                    int cur = D_L00_00161218;
                    tile->heights[cur][row][col] = height;

                    if (row < 2 && tile->neighbours[0] != 0xFF) {
                        if (row == 0) {
                            tiles[tile->neighbours[0]].heights[cur][18][col] =
                                tile->heights[cur][0][col];
                        } else {
                            tiles[tile->neighbours[0]].heights[cur][20][col] = height;
                        }
                    }
                }
                if (col < 2 && tile->neighbours[1] != 0xFF) {
                    if (col == 0) {
                        tiles[tile->neighbours[1]].heights[D_L00_00161218][19][row] =
                            tile->heights[D_L00_00161218][row][0];
                    } else {
                        tiles[tile->neighbours[1]].heights[D_L00_00161218][21][row] =
                            tile->heights[D_L00_00161218][row][col];
                    }
                }
                if (row == 15 && tile->neighbours[2] != 0xFF) {
                    int cur = D_L00_00161218;
                    tiles[tile->neighbours[2]].heights[cur][16][col] = tile->heights[cur][row][col];
                }
                if (col == 15 && tile->neighbours[3] != 0xFF) {
                    int cur = D_L00_00161218;
                    tiles[tile->neighbours[3]].heights[cur][17][row] = tile->heights[cur][row][col];
                }
                if (tile->neighbours[4] != 0xFF) {
                    if (col == 0 && row == 0) {
                        int cur = D_L00_00161218;
                        tiles[tile->neighbours[4]].heights[cur][22][3] =
                            tile->heights[cur][row][col];
                    }
                    if (col == 1 && row == 0) {
                        int cur = D_L00_00161218;
                        tiles[tile->neighbours[4]].heights[cur][22][4] = tile->heights[cur][0][1];
                    }
                    if (col == 0 && row == 1) {
                        int cur = D_L00_00161218;
                        tiles[tile->neighbours[4]].heights[cur][22][5] = tile->heights[cur][1][0];
                    }
                }
                if (tile->neighbours[5] != 0xFF && col == 15 && row == 0) {
                    int cur = D_L00_00161218;
                    tiles[tile->neighbours[5]].heights[cur][22][2] = tile->heights[cur][0][15];
                }
                if (tile->neighbours[6] != 0xFF && col == 0 && row == 15) {
                    int cur = D_L00_00161218;
                    tiles[tile->neighbours[6]].heights[cur][22][1] = tile->heights[cur][15][0];
                }
                if (tile->neighbours[7] != 0xFF && col == 15 && row == 15) {
                    int cur = D_L00_00161218;
                    tiles[tile->neighbours[7]].heights[cur][22][0] = tile->heights[cur][15][15];
                }
                col_x += D_L00_001CA880.cell_width;
            }
            row_z += D_L00_001CA880.cell_height;
        }
        tile++;
    }
}

HeightGridLayout D_L02_001CAE80 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L02_00161218 = 0;
s32 D_L02_00161220 = 2;

HeightGridLayout D_L03_001CA980 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L03_00161218 = 0;
s32 D_L03_00161220 = 2;

HeightGridLayout D_L04_001CAA00 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L04_00161218 = 0;
s32 D_L04_00161220 = 2;

HeightGridLayout D_L06_001CB000 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L06_00161218 = 0;
s32 D_L06_00161220 = 2;

HeightGridLayout D_L08_001CB080 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L08_00161218 = 0;
s32 D_L08_00161220 = 2;

HeightGridLayout D_L09_001CAA00 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L09_00161218 = 0;
s32 D_L09_00161220 = 2;

HeightGridLayout D_L10_001CAD00 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L10_00161218 = 0;
s32 D_L10_00161220 = 2;

HeightGridLayout D_L14_001CAF80 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L14_00161218 = 0;
s32 D_L14_00161220 = 2;

HeightGridLayout D_L15_001CAE80 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L15_00161218 = 0;
s32 D_L15_00161220 = 2;

HeightGridLayout D_L16_001CAD00 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L16_00161218 = 0;
s32 D_L16_00161220 = 2;

HeightGridLayout D_L17_001CB180 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L17_00161218 = 0;
s32 D_L17_00161220 = 2;

HeightGridLayout D_L18_001CB280 = {
    {0, 0, 0, 0x42, 0, 0, 0, 0x42},
    -16.0f, -16.0f, 2.0f, 2.0f
};

s32 D_L18_00161218 = 0;
s32 D_L18_00161220 = 2;
