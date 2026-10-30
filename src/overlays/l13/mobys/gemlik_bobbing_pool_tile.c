/* Level 13 Gemlik bobbing pool tile moby class: bobbing, water grid drawing and tile neighbours. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/rendering/render_water_grids.h"

extern int random_integer_below(int) __asm__("FUN_00213260");

#include "rnc/overlay/l13/mobys/gemlik_bobbing_pool_tile_globals.h"

typedef struct {
    float          x;
    float          y;
    float          z;
    char           pad0C[0x10];
    unsigned char  c1C;
    unsigned char  c1D;
    unsigned short c1E;
    char           rest[0x1170];
} Tile_9e88;

typedef struct {
    float top;
    float bottom;
    float angle;
    float speed;
    int   idx;
} Bob_9e88;

typedef struct {
    float         f00, f04, f08, f0C, f10, f14, f18, f1C, f20, f24;
    int           i28, i2C;
    float         f30, f34, f38;
    unsigned char c3C, c3D;
} Light_9e88;

extern Tile_9e88 Tiles_9e88[] __asm__("D_L13_001D9BC0");

extern Light_9e88 D_L13_001CAB80;

extern float D_L13_001801C0[];

extern unsigned short D_L13_001F1E20[];

extern char gemlik_pool_tile_binding_records[] __asm__("D_L13_001F1E80");

extern int D_L13_00161208;

extern float D_L13_0016120C;

extern float D_L13_00161210;

extern float D_L13_00161214;

extern void *D_L13_001612D0;

extern void *D_L13_001612D4;

int D_L13_00161EF0 = -1;

unsigned short D_L13_00161EF4 = 0xffff;

extern void pool_init_tiles(void *, int) __asm__("FUN_L01_002b7a48");

extern void pool_bind_tile(void *, void *) __asm__("FUN_L01_002b7c68");

extern void pool_set_scale(float) __asm__("FUN_L01_002b7ef0");

extern void pool_flush(void *, int) __asm__("FUN_L01_002b7fe0");

extern void pool_ripple(float, float, float, float, void *, int) __asm__("FUN_L01_002b8288");

extern void pool_splash(float, float, float, float, void *, int, int) __asm__("FUN_L00_002a3ec8");

extern int check_scaled_vector_bsphere(float, void *) __asm__("FUN_L01_00275690");

void render_level13_water_grids(void) __asm__("FUN_L13_0030a460");

void initialize_gemlik_pool_tile_neighbors(int tile_index) __asm__("FUN_L13_0030a488");

extern void initialize_gemlik_pool_tile_neighbors(int) __asm__("FUN_L13_0030a488");

void update_gemlik_bobbing_pool_tile(struct Moby *moby) __asm__("FUN_L13_00309e88");

void update_gemlik_bobbing_pool_tile(struct Moby *moby) {
    enum {
        BOBBING_PLATFORM_INITIALIZE = 0,
        BOBBING_PLATFORM_UPDATE = 1
    };
    Bob_9e88 *bobbing_platform_vars;
    float     height_half_span_or_tile_enabled;
    int       i, j, k, tile_index;
    int       cr, cg;

    struct Hero *player_hero = &hero;
    struct Moby *ship_moby = ((char *)player_hero->ship_moby);
    if (ship_moby != 0 && ship_moby->oclass == 0x45 &&
        (unsigned char)((char)ship_moby->state) != 0xFE &&
        (unsigned char)((char)ship_moby->state) != 0xFD && player_hero->state.current == 0x32)
        return;
    bobbing_platform_vars = (Bob_9e88 *)moby->pvars;
    if (bobbing_platform_vars == 0)
        return;
    switch (moby->state) {
    case BOBBING_PLATFORM_INITIALIZE: {
        float rate = frame_time;
        moby->flags |= 1;
        bobbing_platform_vars->angle = bobbing_platform_vars->angle * DEG_TO_RAD;
        bobbing_platform_vars->speed = bobbing_platform_vars->speed * DEG_TO_RAD * rate;
        if (bobbing_platform_vars->idx == 0) {
            Light_9e88 *tile_lighting;
            char       *tb;
            float       light_direction_angle;
            pool_init_tiles(Tiles_9e88, 22);
            pool_set_scale(1.0f);
            moby->update_dist = 0xFF;
            tile_lighting = &D_L13_001CAB80;
            tile_lighting->f00 = 16.0f;
            tile_lighting->f04 = 16.0f;
            tile_lighting->f08 = -8.0f;
            tile_lighting->f0C = -8.0f;
            tile_lighting->f10 = 1.0f;
            tile_lighting->f14 = 1.0f;
            tile_lighting->f18 = 0.9f;
            tile_lighting->f20 = 0.1f;
            light_direction_angle = fast_arc_tan(D_L13_001801C0[4], D_L13_001801C0[5]);
            tile_lighting->f30 = fast_cos(light_direction_angle) * 0.57735f;
            tile_lighting->f34 = fast_sin(light_direction_angle) * 0.57735f;
            tile_lighting->f38 = -0.57735f;
            tile_lighting->i28 = 0x2D;
            tile_lighting->i2C = 0x2E;
            tile_lighting->c3C = 0x60;
            tile_lighting->c3D = 0x40;
            tb = gemlik_pool_tile_binding_records;
            gemlik_pool_tile_count = 22;
            D_L13_0016120C = 32768.0f;
            D_L13_00161210 = 255.0f;
            D_L13_00161214 = 48.0f;
            D_L13_001612D0 = Tiles_9e88;
            D_L13_001612D4 = tb;
            D_L13_00161208 = 0;
            for (i = 0; i < 22; i++) {
                pool_bind_tile(&Tiles_9e88[i], tb + i * 16);
                Tiles_9e88[bobbing_platform_vars->idx].c1E =
                    D_L13_001F1E20[bobbing_platform_vars->idx];
            }
            for (k = 0; k < 22; k++) {
                for (j = 0; j < 2; j++) {
                    Tile_9e88 *t = &Tiles_9e88[k];
                    float      x = t->x + random_float_between(-6.0f, 6.0f);
                    float      y = t->y + random_float_between(-6.0f, 6.0f);
                    pool_ripple(x, y, 2.0f, 0.2f, Tiles_9e88, 22);
                }
            }
            for (tile_index = 0; tile_index < 22; tile_index++)
                initialize_gemlik_pool_tile_neighbors(tile_index);
        }
        moby->draw_dist = 0;
        moby->state = 1;
        break;
    }
    case BOBBING_PLATFORM_UPDATE: {
        struct Hero *hero_context;
        height_half_span_or_tile_enabled = bobbing_platform_vars->top;
        height_half_span_or_tile_enabled -= bobbing_platform_vars->bottom;
        height_half_span_or_tile_enabled *= 0.5f;
        moby->pos.z = bobbing_platform_vars->bottom +
                      (fast_cos(bobbing_platform_vars->angle) * height_half_span_or_tile_enabled +
                       height_half_span_or_tile_enabled);
        hero_context = &hero;
        if (hero_context->state.control_mode != 0x14 && hero_context->state.control_mode != 0x19 &&
            hero_context->state.current != 0x3D && hero_context->state.current != 0x7B &&
            hero_context->state.current != 0x7C)
            bobbing_platform_vars->angle =
                fast_add_rotations(bobbing_platform_vars->angle, bobbing_platform_vars->speed);
        Tiles_9e88[bobbing_platform_vars->idx].c1C = D_L13_001CAB80.c3C;
        Tiles_9e88[bobbing_platform_vars->idx].c1D = D_L13_001CAB80.c3D;
        if (random_integer_below(200) == 0) {
            float x = Tiles_9e88[bobbing_platform_vars->idx].x + random_float_between(-4.0f, 4.0f);
            float y = random_float_between(-4.0f, 4.0f);
            Tile_9e88 *t = &Tiles_9e88[bobbing_platform_vars->idx];
            pool_splash(x, t->y + y, 1.0f, -0.05f, t, 1, 1);
        }
        break;
    }
    }
    if (bobbing_platform_vars->idx >= 0) {
        height_half_span_or_tile_enabled = 0.0f;
        (Tiles_9e88 + bobbing_platform_vars->idx)->z = moby->pos.z;
        if (check_scaled_vector_bsphere(48.0f, moby) != -1)
            height_half_span_or_tile_enabled = 1.0f;
        if (height_half_span_or_tile_enabled == 0.0f)
            Tiles_9e88[bobbing_platform_vars->idx].c1E = 0;
        else
            Tiles_9e88[bobbing_platform_vars->idx].c1E = D_L13_001F1E20[bobbing_platform_vars->idx];
        if (bobbing_platform_vars->idx == 0) {
            pool_flush(Tiles_9e88, 22);
            add_draw_callback(render_level13_water_grids, moby);
        }
        if (bobbing_platform_vars->idx == D_L13_00161EF0) {
            Tiles_9e88[bobbing_platform_vars->idx].c1E = D_L13_00161EF4;
            D_L13_001F1E20[bobbing_platform_vars->idx] = D_L13_00161EF4;
        }
    }
}

typedef struct {
    float         x;
    float         y;
    int           pad;
    unsigned char n[0x14];
    char          rest[0x1170];
} Ent;

extern Ent D_L13_001D9BC0[];

/* Draws the pool tiles as water grids. */

void render_level13_water_grids(void) __asm__("FUN_L13_0030a460");

void render_level13_water_grids(void) {
    render_water_grids(D_L13_001D9BC0, 0x16);
}

void initialize_gemlik_pool_tile_neighbors(int tile_index) {
    int i;
    D_L13_001D9BC0[tile_index].n[0] = 0xFF;
    D_L13_001D9BC0[tile_index].n[2] = 0xFF;
    D_L13_001D9BC0[tile_index].n[1] = 0xFF;
    D_L13_001D9BC0[tile_index].n[3] = 0xFF;
    D_L13_001D9BC0[tile_index].n[4] = 0xFF;
    D_L13_001D9BC0[tile_index].n[5] = 0xFF;
    D_L13_001D9BC0[tile_index].n[6] = 0xFF;
    D_L13_001D9BC0[tile_index].n[7] = 0xFF;
    for (i = 0; i < 22; i++) {
        if (tile_index != i) {
            Ent  *tile_table = D_L13_001D9BC0;
            Ent  *selected_tile = &tile_table[tile_index];
            float x = selected_tile->x - tile_table[i].x;
            float y = selected_tile->y - tile_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_tile->n[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_tile->n[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_tile->n[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_tile->n[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->n[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->n[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->n[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->n[6] = i;
                }
            }
        }
    }
}
