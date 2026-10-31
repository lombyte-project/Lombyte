/* Level 11 moby class water_tile: the tiles of the water grid and their neighbour rows. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/props/pool_set_scale.h"
#include "rnc/overlay/shared/props/pool_init_tiles.h"
#include "rnc/overlay/shared/rendering/render_water_grids.h"
#include "rnc/overlay/water_grid.h"

extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");

/* The level's 20 water grids: pool_init_tiles/pool_flush/render_water_grids all take 20 (0x14),
   and the neighbour scan loops over 20 rows. */
void populate_water_tile_neighbor_indices(int selected_tile_index) __asm__("FUN_L11_0030e730");

void populate_water_tile_neighbor_indices(int arg) __asm__("FUN_L11_0030e730");

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
    s32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8  unk3C;
    u8  unk3D;
} Light_e088;

extern Light_e088 l11_water_tile_light __asm__("D_L11_001CB280");
int               D_L11_00161F64 = 0;
char              D_L11_0021B4C0[] =
    "I'm really sorry, but you can't have more than 128 water tiles in this level";
/* One fixed-point bounds record (pool_bind_tile: x, y, z, two scale shorts) per water grid;
   the pool reads 20 of them (D_L11_001612D8 = 20 next to their base). */
int water_tile_binding_records[20][4] __asm__("D_L11_001DA740") = {0};

extern float water_light_heading_reference[] __asm__("D_L11_001808C0");
extern void *D_L11_001612D0;
extern void *l11_water_tile_binding_record_base __asm__("D_L11_001612D4");
extern int   D_L11_001612D8;
extern float D_L11_0016120C;
extern float D_L11_00161210;
extern float D_L11_00161214;
extern int   D_L11_00161208;
extern void  print_e088(char *) __asm__("FUN_001e93b0");
extern void  set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern void  synchronize_linked_moby_heights(struct Moby *) __asm__("FUN_L11_0030e690");
extern void  pool_splash(float, float, float, float, void *, int, int) __asm__("FUN_L00_002a3ec8");
extern int   check_scaled_vector_bsphere(float, void *) __asm__("FUN_L01_00275690");
void         render_level_11_water_grids(void) __asm__("FUN_L11_0030e708");
extern void  add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Tile moby: claims a water grid row on spawn (row 0 also sets up the shared light block), then moves
   up or down once its switch fires and throws random sparks; row 0 flushes the visible rows each frame. */
void water_tile_update(struct Moby *water_tile_moby) __asm__("FUN_L11_0030e088");

/* Moby class 1158 */
void water_tile_update(struct Moby *water_tile_moby) {
    enum {
        WATER_TILE_STATE_INITIALIZE = 0,
        WATER_TILE_STATE_FIND_NEIGHBORS = 1,
        WATER_TILE_STATE_APPROACH_UPPER_HEIGHT = 2,
        WATER_TILE_STATE_APPROACH_LOWER_HEIGHT = 3
    };
    int  *water_tile_vars = (int *)water_tile_moby->pvars;
    int   i;
    float tile_query_has_result;

    switch (water_tile_moby->state) {
    case WATER_TILE_STATE_INITIALIZE:
        water_tile_moby->update_dist = 0xFF;
        water_tile_vars[1] = D_L11_00161F64;
        D_L11_00161F64 = D_L11_00161F64 + 1;
        if (D_L11_00161F64 >= 0x80) {
            print_e088(D_L11_0021B4C0);
        }
        water_tile_moby->update_dist = 0xFF;
        water_tile_moby->flags |= 1;
        water_tile_moby->scale = water_tile_moby->pclass->scale + water_tile_moby->pclass->scale;
        if (water_tile_vars[1] == 0) {
            Light_e088 *water_tile_light;
            float       light_direction_angle;
            for (i = 19; i >= 0; i--) {
                l11_water_grids[i].visible_subtiles = 0;
            }
            pool_init_tiles(l11_water_grids, 20);
            pool_set_scale(1.0f);
            water_tile_light = &l11_water_tile_light;
            water_tile_light->unk0 = 16.0f;
            water_tile_light->unk4 = 16.0f;
            water_tile_light->unk8 = -8.0f;
            water_tile_light->unkC = -8.0f;
            water_tile_light->unk10 = 1.0f;
            water_tile_light->unk14 = 1.0f;
            water_tile_light->unk18 = 0.9f;
            water_tile_light->unk20 = 0.1f;
            light_direction_angle =
                fast_arc_tan(water_light_heading_reference[4], water_light_heading_reference[5]);
            water_tile_light->unk30 = fast_cos(light_direction_angle) * 0.57735f;
            water_tile_light->unk34 = fast_sin(light_direction_angle) * 0.57735f;
            water_tile_light->unk38 = -0.57735f;
            water_tile_light->unk28 = 0x38;
            water_tile_light->unk2C = 0x39;
            water_tile_light->unk3C = 0x1C;
            water_tile_light->unk3D = 0x40;
            D_L11_001612D0 = l11_water_grids;
            l11_water_tile_binding_record_base = water_tile_binding_records;
            D_L11_001612D8 = 20;
            D_L11_0016120C = 32768.0f;
            D_L11_00161210 = 255.0f;
            D_L11_00161214 = 48.0f;
            D_L11_00161208 = 0;
        }
        l11_water_grids[water_tile_vars[1]].x = water_tile_moby->pos.x;
        l11_water_grids[water_tile_vars[1]].y = water_tile_moby->pos.y;
        l11_water_grids[water_tile_vars[1]].z = water_tile_moby->pos.z;
        l11_water_grids[water_tile_vars[1]].texture_id = 0x38;
        l11_water_grids[water_tile_vars[1]].second_texture_id = 0x39;
        l11_water_grids[water_tile_vars[1]].alpha_2_fix = 0x1C;
        l11_water_grids[water_tile_vars[1]].alpha_1_fix = 0x44;
        l11_water_grids[water_tile_vars[1]].visible_subtiles = 0xFFFF;
        pool_bind_tile(&l11_water_grids[water_tile_vars[1]],
                       water_tile_binding_records[water_tile_vars[1]]);
        water_tile_moby->state = 1;
        break;
    case WATER_TILE_STATE_FIND_NEIGHBORS:
        D_L11_00161F64 = 0;
        populate_water_tile_neighbor_indices(water_tile_vars[1]);
        water_tile_moby->state = 2;
        break;
    case WATER_TILE_STATE_APPROACH_LOWER_HEIGHT:
        if (water_tile_moby->command & 4) {
            water_tile_vars[2] = 1;
            set_moby_list_state(water_tile_moby->group, 2);
        }
        water_tile_moby->command = 1;
        if (water_tile_vars[2] != 0) {
            advance_accelerated_scalar(&water_tile_moby->pos.z,
                                       water_tile_vars,
                                       132.36f,
                                       frame_time_sq * 4.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time * 4.0f);
            synchronize_linked_moby_heights(water_tile_moby);
        }
        if (random_integer_below(200) == 0) {
            float x = l11_water_grids[water_tile_vars[1]].x + random_float_between(-4.0f, 4.0f);
            float y = random_float_between(-4.0f, 4.0f);
            WaterGrid *t = &l11_water_grids[water_tile_vars[1]];
            pool_splash(x, t->y + y, 1.0f, -0.03f, t, 1, 1);
        }
        break;
    case WATER_TILE_STATE_APPROACH_UPPER_HEIGHT:
        if (water_tile_moby->command & 8) {
            water_tile_vars[2] = 1;
            set_moby_list_state(water_tile_moby->group, 3);
        }
        water_tile_moby->command = 2;
        if (water_tile_vars[2] != 0) {
            advance_accelerated_scalar(&water_tile_moby->pos.z,
                                       water_tile_vars,
                                       137.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time_sq * 4.0f,
                                       frame_time * 4.0f);
            synchronize_linked_moby_heights(water_tile_moby);
        }
        if (random_integer_below(200) == 0) {
            float x = l11_water_grids[water_tile_vars[1]].x + random_float_between(-4.0f, 4.0f);
            float y = random_float_between(-4.0f, 4.0f);
            WaterGrid *t = &l11_water_grids[water_tile_vars[1]];
            pool_splash(x, t->y + y, 1.0f, -0.03f, t, 1, 1);
        }
        break;
    }
    if (water_tile_vars[1] >= 0) {
        tile_query_has_result = 0.0f;
        if (check_scaled_vector_bsphere(64.0f, water_tile_moby) != -1) {
            tile_query_has_result = 1.0f;
        }
        if (tile_query_has_result == 0.0f) {
            l11_water_grids[water_tile_vars[1]].visible_subtiles = 0;
        } else {
            l11_water_grids[water_tile_vars[1]].visible_subtiles = 0xFFFF;
        }
        if (water_tile_vars[1] == 0) {
            pool_flush(l11_water_grids, 20);
            add_draw_callback(render_level_11_water_grids, water_tile_moby);
        }
    }
}

/* Draw callback the water tiles queue: renders the level's water grids. */
void render_level_11_water_grids(void) {
    render_water_grids(l11_water_grids, 0x14);
}
/* Level overlay code; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Same routine as FUN_L05_0030d6a0, with this level's table and entry count. */

void populate_water_tile_neighbor_indices(int selected_tile_index) {
    int i;
    l11_water_grids[selected_tile_index].neighbors[0] = 0xFF;
    l11_water_grids[selected_tile_index].neighbors[2] = 0xFF;
    l11_water_grids[selected_tile_index].neighbors[1] = 0xFF;
    l11_water_grids[selected_tile_index].neighbors[3] = 0xFF;
    l11_water_grids[selected_tile_index].neighbors[4] = 0xFF;
    l11_water_grids[selected_tile_index].neighbors[5] = 0xFF;
    l11_water_grids[selected_tile_index].neighbors[6] = 0xFF;
    l11_water_grids[selected_tile_index].neighbors[7] = 0xFF;
    for (i = 0; i < 20; i++) {
        if (selected_tile_index != i) {
            WaterGrid *tile_table = l11_water_grids;
            WaterGrid *selected_tile = &tile_table[selected_tile_index];
            float      x = selected_tile->x - tile_table[i].x;
            float      y = selected_tile->y - tile_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_tile->neighbors[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_tile->neighbors[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_tile->neighbors[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_tile->neighbors[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->neighbors[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->neighbors[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->neighbors[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->neighbors[6] = i;
                }
            }
        }
    }
}

WaterGrid l11_water_grids[20] = {0};
