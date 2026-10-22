#include "types.h"
#include "rnc/gameplay/surface_height_grid.h"

typedef struct {
    f32 samples[16][16];
    f32 pad_400[0x20];
    f32 bottom_edge[16];
    f32 right_edge[16];
    f32 pad_500[0x23];
    f32 corner;
    f32 pad_590[0xC];
} SurfaceHeightLayer;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 pad_C[0x44];
    SurfaceHeightLayer layers[3];
} SurfaceHeightTile;

extern SurfaceHeightTile *surface_height_tiles __asm__("D_00161190");
extern s32 surface_height_layer __asm__("D_001610E0") __attribute__((sda));

extern s32 find_surface_height_map(f32, f32, f32) __asm__("func_00239D60");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void fast_vec_cross(f32 *, f32 *, f32 *) __asm__("func_001F9AD8");
extern void fast_vec_normalize(f32 *, f32 *, f32) __asm__("func_001F9BF8");

/* Each tile is 0x1190 bytes, with three 0x5C0-byte sample banks. The rounded
   cell indices use signed fractions, so interpolation can extrapolate on
   either side of a sample. Cell 15 uses dedicated edge and corner samples.
   Either output may be omitted without suppressing a successful lookup. */
s32 sample_surface_height_map(f32 *height, f32 *normal, f32 x, f32 y,
                              f32 z) __asm__("FUN_00239f58");

s32 sample_surface_height_map(f32 *height, f32 *normal, f32 x, f32 y, f32 z) {
    f32 x_tangent[4] __attribute__((aligned(16)));
    f32 y_tangent[4] __attribute__((aligned(16)));
    SurfaceHeightTile *tile;
    s32 tile_index;
    s32 column;
    s32 row;
    f32 fraction_x;
    f32 fraction_y;
    f32 height_at_cell_origin;
    f32 height_at_next_column;
    f32 height_at_next_row;
    f32 height_at_diagonal_corner;
    f32 first_row_height;
    f32 weight_x;
    f32 weight_y;

    tile_index = find_surface_height_map(x, y, z);
    if (tile_index < 0) {
        return 0;
    }
    tile = &surface_height_tiles[tile_index];
    fraction_x = tile->x;
    fraction_y = tile->y;
    fraction_x += surface_height_grid.origin_x;
    fraction_y += surface_height_grid.origin_y;
    fraction_x = x - fraction_x;
    fraction_y = y - fraction_y;
    column = truncate_float_to_s32(fraction_x / surface_height_grid.cell_width);
    row = truncate_float_to_s32(*&fraction_y / surface_height_grid.cell_height);
    fraction_x -= convert_integer_to_float(column) * surface_height_grid.cell_width;
    weight_x = fraction_x / surface_height_grid.cell_width;
    fraction_y -= convert_integer_to_float(row) * surface_height_grid.cell_height;
    /* Divide the signed remainder after rounding the row, before choosing the sample bank. */
    weight_y = fraction_y / surface_height_grid.cell_height;
    height_at_cell_origin = tile->layers[surface_height_layer].samples[row][column];
    if (column == 15) {
        height_at_next_column = tile->layers[surface_height_layer].right_edge[row];
    } else {
        height_at_next_column = tile->layers[surface_height_layer].samples[row][column + 1];
    }
    if (row == 15) {
        height_at_next_row = tile->layers[surface_height_layer].bottom_edge[column];
    } else {
        height_at_next_row = tile->layers[surface_height_layer].samples[row + 1][column];
    }
    if (column == 15) {
        if (row == 15) {
            height_at_diagonal_corner = tile->layers[surface_height_layer].corner;
        } else {
            height_at_diagonal_corner = tile->layers[surface_height_layer].right_edge[row + 1];
        }
    } else if (row == 15) {
        height_at_diagonal_corner = tile->layers[surface_height_layer].bottom_edge[column + 1];
    } else {
        height_at_diagonal_corner = tile->layers[surface_height_layer].samples[row + 1][column + 1];
    }
    if (height != NULL) {
        first_row_height = height_at_cell_origin + (height_at_next_column - height_at_cell_origin) * weight_x;
        *height = first_row_height +
                  ((height_at_next_row + (height_at_diagonal_corner - height_at_next_row) * weight_x) - first_row_height) * weight_y + tile->z;
    }
    if (normal != NULL) {
        x_tangent[0] = surface_height_grid.cell_width;
        x_tangent[1] = 0.0f;
        x_tangent[2] = height_at_next_column - height_at_cell_origin;
        x_tangent[3] = 1.0f;
        y_tangent[0] = 0.0f;
        y_tangent[1] = surface_height_grid.cell_height;
        y_tangent[2] = height_at_next_row - height_at_cell_origin;
        y_tangent[3] = 1.0f;
        fast_vec_cross(normal, x_tangent, y_tangent);
        fast_vec_normalize(normal, normal, 1.0f);
    }
    return 1;
}

extern __typeof__(sample_surface_height_map) func_00239F58 __attribute__((alias("FUN_00239f58")));

