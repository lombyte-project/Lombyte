/* Shared moby class 0x340: times the visibomb missile while it is outside every zone and ends it past a limit. */

#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/random.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/rendering/screen.h"

/* pvars of the moby run by FUN_L01_00302648: 20 trigger zones and a hold counter */
typedef struct {
    s32 path; /* -1, or an index into D_L01_001B0930 */
    f32 zmin;
    f32 zmax;
    s32 volume; /* clip volume is_point_inside_clip_volume tests */
} Zone;

typedef struct {
    Zone zones[20];
    s32  count;
} ZoneVars;

extern void
    draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, u64, u64) __asm__("FUN_001f5450");
extern s32  point_in_path_xy(void *, void *, s32) __asm__("FUN_L00_00259740");
extern void terminate_visibomb_missile(struct Moby *, void *) __asm__("FUN_L00_002b5b20");

extern struct Moby *D_L01_0015FFE4_302648 __asm__("D_L01_0015FFE4");
extern s32 visibomb_outside_zone_overlay_enabled_gp __asm__("D_L01_0015F30C") __attribute__((sda));

s32 D_L02_0015F30C = 0;
s32 D_L03_0015F30C = 0;
s32 D_L04_0015F30C = 0;
s32 D_L05_0015F30C = 0;
s32 D_L06_0015F30C = 0;
s32 D_L07_0015F30C = 0;
s32 D_L08_0015F30C = 0;
s32 D_L09_0015F30C = 0;
s32 D_L10_0015F30C = 0;
s32 D_L11_0015F30C = 0;
s32 D_L12_0015F30C = 0;
s32 D_L13_0015F30C = 0;
s32 D_L14_0015F30C = 0;
s32 D_L18_0015F30C = 0;

void draw_visibomb_outside_zone_overlay(struct Moby *moby) __asm__("FUN_L01_00302438");

void draw_visibomb_outside_zone_overlay(struct Moby *moby) {
    s32         *outside_zone_timeout_vars = (s32 *)moby->pvars;
    s32          row_count;
    s32          column_count;
    s32          row;
    s32          column;
    f32          screen_center[2];
    volatile f32 coordinate[2];
    f32          opacity;
    s32          color;
    s32          texture_x;
    s32          texture_y;
    s32          draw_x;
    s32          draw_y;
    s32          tile_size = 32;
    s32          tile_half;
    u64          texture;

    screen_center[0] = convert_integer_to_float(screen_extent.half_width);
    screen_center[1] = convert_integer_to_float(screen_extent.half_height);
    texture = get_effect_texture(30);
    row_count = screen_extent.width / 32 + 1;
    column_count = screen_extent.height / 32 + 1;
    for (row = 0; row < row_count; row++) {
        s32 base_color = 0xff7f7f;
        for (column = 0; column < column_count; column++) {
            coordinate[0] = (f32)(row * 32);
            coordinate[1] = (f32)(column * 32);
            tile_half = tile_size >> 1;
            opacity = convert_integer_to_float(outside_zone_timeout_vars[0x50]) * 127.5f /
                      convert_integer_to_float(scale_game_frames(90));
            color = (truncate_float_to_s32(opacity) << 24) + base_color;
            texture_x = random_integer_below(32);
            texture_y = random_integer_below(32);
            draw_x = truncate_float_to_s32(coordinate[0] - (f32)tile_half);
            draw_y = truncate_float_to_s32(coordinate[1] - (f32)tile_half);
            draw_textured_quad(
                draw_x, draw_y, 32, 32, texture_x, texture_y, 32, 32, color, texture);
        }
    }
}

/* While the camera is in mode 0x1D, counts the frames the class 0xAC moby spends outside every zone.
 * Entering a zone resets the count; past a random threshold FUN_L00_002b5b20 runs on that moby. */
void visibomb_zone_timer_update(struct Moby *moby) __asm__("FUN_L01_00302648");

/* Moby class 832 */
void visibomb_zone_timer_update(struct Moby *moby) {
    ZoneVars    *zone_vars = (ZoneVars *)moby->pvars;
    struct Moby *target_moby = 0;
    struct Moby *candidate_moby;
    s32          i;
    s32         *path;

    moby->update_dist = 0xFF;
    if (camera_secondary_mode[0] != 0x1D) {
        zone_vars->count = 0;
        return;
    }
    for (candidate_moby = D_L01_0015FFE4_302648; candidate_moby != 0;
         candidate_moby = candidate_moby->next) {
        if (candidate_moby->oclass == 0xAC) {
            target_moby = candidate_moby;
            break;
        }
    }
    if (target_moby == 0 || target_moby->oclass != 0xAC) {
        zone_vars->count = 0;
        return;
    }
    zone_vars->count++;
    for (i = 0; i < 20; i++) {
        if (zone_vars->zones[i].path != -1 && target_moby->pos.z > zone_vars->zones[i].zmin &&
            target_moby->pos.z < zone_vars->zones[i].zmax) {
            path = ((s32 *)l01_paths[zone_vars->zones[i].path]);
            if (point_in_path_xy(&target_moby->pos, path + 4, path[0])) {
                zone_vars->count = 0;
                break;
            }
        }
        if (is_point_inside_clip_volume(&target_moby->pos, zone_vars->zones[i].volume)) {
            zone_vars->count = 0;
            break;
        }
    }
    if (zone_vars->count > 0) {
        if (visibomb_outside_zone_overlay_enabled_gp) {
            add_draw_callback(draw_visibomb_outside_zone_overlay, moby);
        }
        if (scale_game_frames(90) < zone_vars->count) {
            terminate_visibomb_missile(target_moby, target_moby->pvars);
        }
    }
}
