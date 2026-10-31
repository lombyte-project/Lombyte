/* Level 01 moby class pool_tiles_and_effects. */
#include "types.h"
#include "rnc/globals.h"
#include "qzero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

extern void copy_hero_moby_field_38() __asm__("FUN_L00_0025d1b8");

extern void update_entity() __asm__("FUN_L00_00250df8");

#include "qcopy.h"

extern int   is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");

#include "sda.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8  neighbors
        [8]; /* 0x0C: index of the tile 16 units away in each direction, 0xFF: none (WaterGrid layout) */
    u32 texture_id;        /* 0x14 */
    u32 second_texture_id; /* 0x18 */
    u8  unk1C;             /* 0x1C */
    u8  unk1D;             /* 0x1D */
    u16 color;             /* 0x1E */
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
    u8  pad30[0x14];
    s32 i44;
    u8  rest[0x1148];
} PoolTile;

typedef struct {
    f32 f00, f04, f08, f0C, f10, f14, f18, f1C, f20, f24;
    s32 i28, i2C;
    f32 f30, f34, f38;
    u8  c3C, c3D;
} PoolLight;

typedef struct {
    f32 x, y, z, w;
} PoolVec;

/* 21 tiles: pool_init_tiles(pool_tiles, 21), pool_flush(pool_tiles, 21) and the bind/ripple loops. */
PoolTile pool_tiles[21] __asm__("D_L01_001E34C0") = {
    {155.0f,
     88.5f, 61.0f,
     {0xff, 0xff, 2, 1, 0xff, 0xff, 0xff, 3},
     0x28, 0x29,
     0x1c, 0x44,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {171.0f,
     88.5f, 61.0f,
     {0xff, 0, 3, 0xff, 0xff, 0xff, 2, 0xff},
     0x28, 0x29,
     0x1c, 0x44,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {155.0f,
     104.5f, 61.0f,
     {0, 0xff, 0xff, 3, 0xff, 1, 0xff, 0xff},
     0x28, 0x29,
     0x1c, 0x44,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {171.0f,
     104.5f, 61.0f,
     {1, 2, 0xff, 0xff, 0, 0xff, 0xff, 0xff},
     0x28, 0x29,
     0x1c, 0x44,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {125.5f,
     91.75f, 56.5f,
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     0x2a, 0x2b,
     0x48, 0x40,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {117.75f,
     115.0f, 56.0f,
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     0x2a, 0x2b,
     0x48, 0x48,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {166.0f,
     6e+01f, 65.5f,
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     0x2a, 0x2b,
     0x48, 0x40,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {151.0f,
     72.5f, 65.5f,
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     0x2a, 0x2b,
     0x48, 0x40,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {127.0f,
     53.0f, 65.5f,
     {0xff, 0xff, 0xa, 9, 0xff, 0xff, 0xff, 0xb},
     0x2a, 0x2b,
     0x48, 0x48,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {143.0f,
     53.0f, 65.5f,
     {0xff, 8, 0xb, 0xff, 0xff, 0xff, 0xa, 0xff},
     0x2a, 0x2b,
     0x48, 0x48,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {127.0f,
     69.0f, 65.5f,
     {8, 0xff, 0xff, 0xb, 0xff, 9, 0xff, 0xff},
     0x2a, 0x2b,
     0x48, 0x48,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {143.0f,
     69.0f, 65.5f,
     {9, 0xa, 0xff, 0xff, 8, 0xff, 0xff, 0xff},
     0x2a, 0x2b,
     0x48, 0x48,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {136.0f,
     128.5f, 56.0f,
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     0x2a, 0x2b,
     0x48, 0x48,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {186.1f,
     154.0f, 39.0f,
     {0xff, 0xff, 0xf, 0xe, 0xff, 0xff, 0xff, 0x10},
     0x28, 0x29,
     0x24, 0x54,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {202.1f,
     154.0f, 39.0f,
     {0xff, 0xd, 0x10, 0xff, 0xff, 0xff, 0xf, 0xff},
     0x28, 0x29,
     0x24, 0x54,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {186.1f,
     1.7e+02f, 39.0f,
     {0xd, 0xff, 0xff, 0x10, 0xff, 0xe, 0xff, 0xff},
     0x28, 0x29,
     0x24, 0x54,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {202.1f,
     1.7e+02f, 39.0f,
     {0xe, 0xf, 0xff, 0xff, 0xd, 0xff, 0xff, 0xff},
     0x28, 0x29,
     0x24, 0x54,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {155.5f,
     115.5f, 60.5f,
     {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     0x28, 0x29,
     0x1c, 0x48,
     0, 0.0f,
     0.0f, 0.0f,
     0.0f, {0},
     0, {0}},
    {0},
    {0},
    {0},
};

extern f32  pool_sun[] __asm__("D_L01_00180340");
extern u8   pool_tile_tex[] __asm__("D_L01_001FA6C0");
extern s32  pool_first[] __asm__("D_L01_001FA5E8");
extern s32  pool_count[] __asm__("D_L01_001FA608");
extern s32  pool_odds[] __asm__("D_L01_001FA628");
extern u32  pool_colors[] __asm__("D_L01_001FA590");
extern u128 pool_drips[] __asm__("D_L01_001FA650");
extern u8   pool_wave[] __asm__("D_L01_001FA6A0");
extern u8   camera_pos[] __asm__("D_L01_00167240");
extern s32  level_frame_count __asm__("D_L01_0015F5CC") MACRO_ADDR;
s32         pool_drip_timer __asm__("D_L01_00161C84") = 0;
extern u8  *pool_draw_tex __asm__("D_L01_001612D4") MACRO_ADDR;
extern void pool_init_tiles(void *, s32) __asm__("FUN_L01_002b7a48");
extern void pool_bind_tile(void *, void *) __asm__("FUN_L01_002b7c68");
extern void pool_set_scale(f32) __asm__("FUN_L01_002b7ef0");
extern void pool_flush(void *, s32) __asm__("FUN_L01_002b7fe0");
extern void pool_ripple(f32, f32, f32, f32, void *, s32) __asm__("FUN_L01_002b8288");
extern void pool_splash(f32, f32, f32, f32, void *, s32, s32) __asm__("FUN_L00_002a3ec8");
extern s32  is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern void render_level_water_grids(void) __asm__("FUN_L01_002fd0c0");
extern int  add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern void pool_drip(void *, f32) __asm__("FUN_L01_002ffcd0");
extern void pool_bubble(void *, void *, f32) __asm__("FUN_L01_00287b00");
extern void pool_bubbles(void *, void *, f32, f32) __asm__("FUN_L01_00287c80");

extern u8 D_L01_00161200  MACRO_ADDR;
extern u8 D_L01_00161201  MACRO_ADDR;
extern u8 D_L01_00161202  MACRO_ADDR;
extern u8                 water_tint __asm__("D_L01_00161203") MACRO_ADDR;
extern u8 D_L01_00161204  MACRO_ADDR;
extern u8 D_L01_00161205  MACRO_ADDR;
extern u8 D_L01_00161206  MACRO_ADDR;
extern f32 D_L01_00161208 MACRO_ADDR;
extern f32 D_L01_0016120C MACRO_ADDR;
extern f32 D_L01_00161210 MACRO_ADDR;
extern f32 D_L01_00161214 MACRO_ADDR;
void pool_tiles_and_effects_update(struct Moby *moby) __asm__("FUN_L01_002fd0e8");

/* Moby class 751 */
void pool_tiles_and_effects_update(struct Moby *moby) {
    enum {
        POOL_INITIALIZE = 0,
        POOL_UPDATE_TILES_AND_EFFECTS = 1
    };
    s32      *volumes;
    s32       found;
    s32       i;
    s32       j;
    PoolVec   drip_position_or_bubble_vector;
    PoolVec   bubble_position;
    s32       k;
    f32       bubble_path_fraction;
    f32       zero;
    u8       *tile_binding_output;
    s32       ripple_tile_index;
    s32       bind_tile_index;
    PoolTile *tiles;

    volumes = (s32 *)moby->pvars;
    switch (moby->state) {
    case POOL_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->pos.z = moby->pos.z + 0.5f;
        pool_init_tiles(pool_tiles, 21);
        zero = 0.0f;
        pool_tiles[15].i44 = 8;
        pool_tiles[16].f28 = -0.0016f;
        pool_tiles[15].f28 = -0.0016f;
        pool_tiles[14].f28 = -0.0016f;
        pool_tiles[16].f2C = zero;
        pool_tiles[13].f28 = -0.0016f;
        pool_tiles[14].f2C = zero;
        pool_tiles[15].f2C = zero;
        pool_tiles[13].f2C = zero;
        pool_set_scale(1.0f);
        {
            PoolLight *pool_lighting;
            f32        sun_angle;

            pool_lighting = (PoolLight *)pool_light;
            pool_lighting->f00 = 16.0f;
            pool_lighting->f04 = 16.0f;
            pool_lighting->f08 = -8.0f;
            pool_lighting->f0C = -8.0f;
            pool_lighting->f10 = 1.0f;
            pool_lighting->f14 = 1.0f;
            pool_lighting->f18 = 0.9f;
            pool_lighting->f20 = 0.1f;
            sun_angle = fast_arc_tan(pool_sun[4], pool_sun[5]);
            pool_lighting->f30 = fast_cos(sun_angle) * 0.57735f;
            pool_lighting->f34 = fast_sin(sun_angle) * 0.57735f;
            pool_lighting->f38 = -0.57735f;
            pool_lighting->i28 = 0x29;
            pool_lighting->i2C = 0x2A;
            pool_lighting->c3C = 0x1C;
            pool_lighting->c3D = 0x40;
        }
        pool_draw_count = 21;
        D_L01_00161208 = zero;
        tile_binding_output = pool_tile_tex;
        D_L01_0016120C = 32768.0f;
        D_L01_00161210 = 255.0f;
        D_L01_00161214 = 48.0f;
        pool_draw_tiles = (int)pool_tiles;
        pool_draw_tex = pool_tile_tex;
        for (bind_tile_index = 0; bind_tile_index < 21; bind_tile_index++) {
            pool_bind_tile(&pool_tiles[bind_tile_index],
                           tile_binding_output + bind_tile_index * 16);
        }
        for (ripple_tile_index = 0; ripple_tile_index < 21; ripple_tile_index++) {
            PoolTile *tile = &pool_tiles[ripple_tile_index];
            tiles = pool_tiles;

            for (j = 0; j < 2; j++) {
                f32 x = tile->x + random_float_between(-6.0f, 6.0f);
                f32 y = tile->y + random_float_between(-6.0f, 6.0f);

                pool_ripple(x, y, 2.0f, 0.2f, tiles, 21);
            }
        }
        break;
    case POOL_UPDATE_TILES_AND_EFFECTS:
        found = -1;
        for (i = 0; i < 7; i++) {
            if (*(s32 *)((u8 *)volumes + (i << 2)) == -1 ||
                is_point_inside_clip_volume(camera_pos, *(s32 *)((u8 *)volumes + (i << 2))) == 0) {
                for (k = 0; k < pool_count[i]; k++) {
                    pool_tiles[pool_first[i] + k].color = 0;
                }
            } else {
                found = i;
                for (j = 0; j < pool_count[i]; j++) {
                    tiles = pool_tiles;
                    tiles[pool_first[i] + j].color = pool_colors[pool_first[i] + j];
                    if (random_integer_below(pool_odds[i]) == 0) {
                        f32 x = tiles[pool_first[i] + j].x + random_float_between(-4.0f, 4.0f);
                        f32 y = tiles[pool_first[i] + j].y + random_float_between(-4.0f, 4.0f);

                        pool_splash(x,
                                    y,
                                    1.0f,
                                    -0.05f,
                                    (PoolTile *)pool_draw_tiles + pool_first[i] + j,
                                    1,
                                    1);
                    }
                }
            }
        }
        if (found >= 0) {
            if (found > 0 && found < 4) {
                D_L01_00161200 = 0;
                D_L01_00161201 = 0x28;
                D_L01_00161202 = 0x30;
                water_tint = 0x40;
                D_L01_00161204 = 0;
                D_L01_00161205 = 0x28;
                D_L01_00161206 = 0x30;
            } else {
                D_L01_00161200 = 0x18;
                D_L01_00161201 = 0x30;
                D_L01_00161202 = 0x90;
                water_tint = 0x30;
                D_L01_00161204 = 0x18;
                D_L01_00161205 = 0x30;
                D_L01_00161206 = 0x90;
            }
        }
        pool_flush(pool_tiles, 21);
        add_draw_callback(render_level_water_grids, moby);
        if (found == 0 || found == 6) {
            pool_drip_timer--;
            if (pool_drip_timer <= 0) {
                qcopy(&drip_position_or_bubble_vector, &pool_drips[random_integer_below(5)]);
                drip_position_or_bubble_vector.x =
                    drip_position_or_bubble_vector.x + random_float_between(-0.15f, 0.15f);
                drip_position_or_bubble_vector.y =
                    drip_position_or_bubble_vector.y + random_float_between(-0.15f, 0.15f);
                pool_drip(&drip_position_or_bubble_vector, 0.2f);
                pool_drip_timer = random_int_between(300, 1200);
            }
        }
        if (found == 5) {
            bubble_path_fraction = pool_wave[level_frame_count % 20] / 20.0f;
            drip_position_or_bubble_vector.x = 0.02f;
            drip_position_or_bubble_vector.y = 0.0f;
            drip_position_or_bubble_vector.z = 0.0f;
            bubble_position.x = random_float_between(-0.6f, 0.6f) + 177.0f;
            bubble_position.y =
                bubble_path_fraction * -11.0f + 176.0f + random_float_between(-0.1f, 0.1f);
            bubble_position.z = 39.0f;
            pool_bubbles(&bubble_position,
                         &drip_position_or_bubble_vector,
                         1.0f,
                         random_float_between(0.4f, 0.5f));
            drip_position_or_bubble_vector.x = 0.0f;
            drip_position_or_bubble_vector.y = 0.0f;
            drip_position_or_bubble_vector.z = 0.0f;
            for (bubble_path_fraction = drip_position_or_bubble_vector.x;
                 bubble_path_fraction < 1.0f;
                 bubble_path_fraction += 0.05f) {
                if (random_integer_below(32) == 0) {
                    bubble_position.x = random_float_between(-0.6f, 0.6f) + 178.0f;
                    bubble_position.y =
                        bubble_path_fraction * -11.0f + 176.0f + random_float_between(-0.1f, 0.1f);
                    bubble_position.z = random_float_between(-0.2f, 0.0f) + 39.0f;
                    pool_bubble(&bubble_position,
                                &drip_position_or_bubble_vector,
                                random_float_between(4.0f, 10.0f));
                }
            }
        }
        break;
    }
}

extern float fast_cos(float);
extern float fast_sin(float);
extern float random_float_between(float, float);

typedef struct {
    u8  pad0[0x8];
    f32 f8;
    u8  padC[4];
    f32 f10;
    s32 timer;
    u8  pad18[4];
    f32 spin;
} OrbVars;

typedef struct {
    u8       pad0[0x10];
    u128     pos;
    u8       pad20[0xC];
    f32      scale;
    u8       b30;
    u8       b31;
    s16      h32;
    u8       pad34[0xC];
    s32      w40;
    u8       pad44[4];
    f32      rotz;
    u8       pad4C[0x2C];
    OrbVars *pvars;
} OrbMoby;

extern void copy_hero_moby_field_38(OrbMoby *) __asm__("FUN_L00_0025d1b8");

OrbMoby *FUN_L01_002ffcd0(u128 *pos, f32 scale) {
    OrbMoby *spawned_moby;
    OrbVars *spawned_moby_vars;

    spawned_moby = create_moby(0x313);
    if (spawned_moby != 0) {
        spawned_moby_vars = spawned_moby->pvars;
        copy_hero_moby_field_38(spawned_moby);
        spawned_moby->w40 = 0;
        spawned_moby->h32 = 0x40;
        spawned_moby->b30 = 0xFF;
        spawned_moby->scale = spawned_moby->scale * scale;
        spawned_moby->rotz = random_angle_radians();
        qcopy(&spawned_moby->pos, pos);
        qzero(spawned_moby_vars);
        spawned_moby_vars->f8 = -frame_time;
        spawned_moby_vars->timer = 0x78;
        spawned_moby_vars->f10 = frame_time_sq * 15.0f;
        spawned_moby_vars->spin = random_float_between(0.0031415929f, 0.062831856f);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

s32 D_L01_001CAD28[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001CAD2C[1] __attribute__((section(".data"))) = {0};

f32 D_L01_001CAD30[1] __attribute__((section(".data"))) = {0};

f32 D_L01_001CAD34[1] __attribute__((section(".data"))) = {0};

f32 D_L01_001CAD38[1] __attribute__((section(".data"))) = {0};
