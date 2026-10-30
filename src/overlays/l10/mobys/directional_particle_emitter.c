/* Level 10 moby class directional_particle_emitter (class 1240): sprays particles along its heading. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"

#include "rnc/math/vector.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"

typedef struct Moby Moby;
extern float        fast_cos(float);
extern float        fast_sin(float);

#include "rnc/overlay/l10/mobys/directional_particle_emitter_globals.h"

extern s32 directional_particle_color_range_start_gp __asm__("D_L10_00161EF0") __attribute__((sda));
extern s32 directional_particle_color_range_end_gp __asm__("D_L10_00161EF4") __attribute__((sda));
extern f32 directional_particle_speed_range_end_gp __asm__("D_L10_00161EF8") __attribute__((sda));
extern f32 directional_particle_speed_range_start_gp __asm__("D_L10_00161EFC") __attribute__((sda));

#include "rnc/overlay/l10/mobys/directional_particle_emitter_data/update_directional_particle_emitter.h"

extern s32 directional_particle_variant_threshold_percent_gp __asm__("D_L10_00161F04")
    __attribute__((sda));
extern f32            frame_time_absolute __asm__("D_L10_0015ED6C") MACRO_ADDR;
extern f32            multiply_global_factor_ed64(f32) __asm__("FUN_L10_001f2578");
extern s32            fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern unsigned char *spawn_gravity_floor_burst_particle(
    void *, void *, s32, f32, f32, f32, s32, s32, s32) __asm__("FUN_L00_002738e8");

void update_directional_particle_emitter(struct Moby *moby) __asm__("FUN_L10_002e5be0");

void update_directional_particle_emitter(struct Moby *moby) {
    f32  *pvars = (f32 *)moby->pvars;
    Vec4 *moby_position;
    s32   i = 0;

    moby->update_dist = 0x80;
    if (((s32 *)pvars)[3] > 0) {
        moby_position = (Vec4 *)&moby->pos.x;
        for (; i < ((s32 *)pvars)[3]; ++i) {
            Vec4 position;
            Vec4 velocity;
            Vec4 offset;
            f32  amount;
            f32  radius;
            f32  x;
            f32  y;
            f32  z;
            s32  color;
            s32  fade_color;
            s32  first_color;
            s32  life;
            s32  variant;

            /* Save the loop counter while selecting the particle variant. */
            life = i;
            i = random_integer_below(100);
            variant = i < directional_particle_variant_threshold_percent_gp;
            i = life;
            qcopy(&position, moby_position);
            offset.f[0] = fast_cos(fast_add_rotations(moby->rot.z, 1.5707964f)) *
                          random_float_between(-pvars[4], pvars[4]);
            offset.f[1] = fast_sin(fast_add_rotations(moby->rot.z, 1.5707964f)) *
                          random_float_between(-pvars[4], pvars[4]);
            *(volatile f32 *)&offset.f[2] = 0.0f;
            offset.f[2] = random_float_between(-pvars[5], pvars[5]);
            fast_vec_add(&position, &position, &offset);

            x = random_float_between(directional_particle_speed_range_start_gp,
                                     directional_particle_speed_range_end_gp);
            y = random_float_between(directional_particle_speed_range_start_gp,
                                     directional_particle_speed_range_end_gp);
            z = random_float_between(directional_particle_speed_range_start_gp,
                                     directional_particle_speed_range_end_gp);
            radius = x;
            if (radius < y)
                radius = y;
            if (radius < z)
                radius = z;
            build_spherical_offset(
                velocity.f, radius * frame_time_absolute, moby->rot.z, -moby->rot.y);

            if (variant) {
                fast_vec_scale(velocity.f, velocity.f, random_float_between(0.5f, 1.5f));
                x = random_float_between(0.24f, 0.36f);
                amount = pvars[1] * x;
            } else {
                x = random_float_between(0.8f, 1.2f);
                amount = pvars[1] * x;
            }
            x = random_float_between(0.5f, 1.0f);
            color = fast_tween_color(x,
                                     directional_particle_color_range_start_gp,
                                     directional_particle_color_range_end_gp);
            first_color = directional_particle_color_range_end_gp & 0xff000000;
            x = random_float_between(0.25f, 0.5f);
            fade_color = fast_tween_color(x, directional_particle_color_range_end_gp, first_color);
            life = truncate_float_to_s32(multiply_global_factor_ed64(pvars[2] * 60.0f));
            spawn_gravity_floor_burst_particle(&position,
                                               &velocity,
                                               life,
                                               amount,
                                               pvars[0],
                                               directional_particle_gravity_gp * frame_time_sq,
                                               color,
                                               fade_color,
                                               variant);
        }
    }
}
