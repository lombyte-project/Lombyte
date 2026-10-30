/* Level 01 moby class water_lily. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/l01/mobys/water_lily_globals.h"

#include "sda.h"

#include "qcopy.h"

extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;

f32 FUN_L01_002f81c8(f32 *spring_axis_position,
                     f32 *spring_axis_velocity,
                     f32  target,
                     f32  velocity_adjustment_scale,
                     f32  damp,

                     f32 range) {
    f32 velocity_adjustment;
    f32 f;

    range = range * range;
    damp = ((1.0f - damp) - 1.0f) * frame_scale + 1.0f;
    *spring_axis_velocity = *spring_axis_velocity * damp;
    velocity_adjustment = *spring_axis_position - target;
    velocity_adjustment = velocity_adjustment * velocity_adjustment;
    if (range < velocity_adjustment) {
        velocity_adjustment = range;
    }
    velocity_adjustment = velocity_adjustment * velocity_adjustment_scale / range;
    if (target < *spring_axis_position) {
        *spring_axis_velocity = *spring_axis_velocity - velocity_adjustment;
    } else {
        *spring_axis_velocity = *spring_axis_velocity + velocity_adjustment;
    }
    *spring_axis_position = *spring_axis_position + *spring_axis_velocity;
    return absolute_float(target - *spring_axis_position);
}

typedef union {
    u128 q;
    struct {
        f32 x, y, z, w;
    } v;
} HoverVec;

typedef struct {
    f32 home_x;
    f32 home_y;
    f32 pad8[2];
    f32 vel_x;
    f32 vel_y;
} HoverVars;

typedef struct {
    u8         pad0[0x10];
    HoverVec   pos;
    u8         state;
    u8         pad21[0x57];
    HoverVars *pvars;
} HoverMoby;

extern f32 water_lily_spring_velocity_adjustment_scale_gp __asm__("D_L01_00161BF0")
    __attribute__((sda));
extern f32  water_lily_spring_damping_gp __asm__("D_L01_00161BF4") __attribute__((sda));
extern f32  water_lily_hero_separation_radius_gp __asm__("D_L01_00161BF8") __attribute__((sda));
extern f32  water_lily_max_horizontal_speed_gp __asm__("D_L01_00161BFC") __attribute__((sda));
extern f32  water_lily_collision_radius_gp __asm__("D_L01_00161C00") __attribute__((sda));
extern f32  spring_axis_to(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L01_002f81c8");
extern void FUN_L00_00258490(void *, void *, void *, s32, f32, f32, f32, f32);
extern f32  resolve_camera_surface_height(void *, s32) __asm__("FUN_002135f0");

void water_lily_update(HoverMoby *m) __asm__("FUN_L01_002f8268");

void water_lily_update(HoverMoby *water_lily) {
    enum {
        WATER_LILY_STATE_INITIALIZE = 0,
        WATER_LILY_STATE_UPDATE_POSITION = 1
    };
    HoverVars   *v;
    struct Hero *pl;
    HoverVec     old;
    HoverVec     collision_adjusted_position;
    HoverVec     step;
    HoverVec     vector_scratch;
    f32          hero_separation_angle;
    f32          dz;
    f32          max_vertical_step;

    old.q = water_lily->pos.q;
    v = water_lily->pvars;
    collision_adjusted_position.q = old.q;
    if (v == 0) {
        return;
    }
    switch (water_lily->state) {
    case WATER_LILY_STATE_INITIALIZE:
        *(u128 *)v = old.q;
        water_lily->state = 1;
        break;
    case WATER_LILY_STATE_UPDATE_POSITION:
        spring_axis_to(&water_lily->pos.v.x,
                       v->home_x,
                       &v->vel_x,
                       water_lily_spring_velocity_adjustment_scale_gp * frame_time_sq,
                       water_lily_spring_damping_gp,
                       3.0f);
        spring_axis_to(&water_lily->pos.v.y,
                       v->home_y,
                       &v->vel_y,
                       water_lily_spring_velocity_adjustment_scale_gp * frame_time_sq,
                       water_lily_spring_damping_gp,
                       3.0f);
        if (vector_distance_xy(&water_lily->pos, &hero.motion.pos) <
            water_lily_hero_separation_radius_gp) {
            if (absolute_float(water_lily->pos.v.z - hero.motion.pos.f[2]) < 0.7f) {
                hero_separation_angle = fast_arc_tan(water_lily->pos.v.x - hero.motion.pos.f[0],
                                                     water_lily->pos.v.y - hero.motion.pos.f[1]);
                water_lily->pos.v.x =
                    fast_cos(hero_separation_angle) * water_lily_hero_separation_radius_gp;
                water_lily->pos.v.y =
                    fast_sin(hero_separation_angle) * water_lily_hero_separation_radius_gp;
                water_lily->pos.v.x += hero.motion.pos.f[0];
                water_lily->pos.v.y += hero.motion.pos.f[1];
            }
        }
        FUN_L00_00258490(water_lily,
                         &collision_adjusted_position,
                         &water_lily->pos,
                         3,
                         0.1f,
                         water_lily_collision_radius_gp,
                         600.0f,
                         1.5707964f);
        water_lily->pos.v.x = collision_adjusted_position.v.x;
        water_lily->pos.v.y = collision_adjusted_position.v.y;
        water_lily->pos.v.z = old.v.z;
        if (fast_vec_dist(&old, &water_lily->pos) >
            water_lily_max_horizontal_speed_gp * frame_time_macro) {
            fast_vec_sub(&vector_scratch, &water_lily->pos, &old);
            step.q = vector_scratch.q;
            fast_vec_normalize(&step, &step, water_lily_max_horizontal_speed_gp * frame_time_macro);
            fast_vec_add(&vector_scratch, &old, &step);
            water_lily->pos.q = vector_scratch.q;
        }
        dz = resolve_camera_surface_height(&water_lily->pos, 0) - water_lily->pos.v.z;
        max_vertical_step = frame_time_macro * 3.0f;
        if (dz > max_vertical_step) {
            dz = max_vertical_step;
        } else if (dz < -max_vertical_step) {
            dz = -max_vertical_step;
        }
        water_lily->pos.v.z += dz;
        break;
    }
}
