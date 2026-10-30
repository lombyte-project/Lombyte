/* Level 08 particle line effect (no caller in C). */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/random.h"

extern int   FUN_L00_00257b90(int, int);
extern float line_puff_initial_z_velocity_range_end_gp __asm__("D_L08_00161998")
    __attribute__((sda));
extern float line_puff_position_jitter_range_end_gp __asm__("D_L08_001619A0") __attribute__((sda));
extern u16   line_puff_duration_frames_gp __asm__("D_L08_001619A8") __attribute__((sda));
extern unsigned char *
spawn_puff(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern char *spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");

/* Strings count glowing puffs from one point to another, growing from size0
 * to size1, then bursts a spark shower at each end. */
void spawn_particle_line_effect(struct Moby *moby,
                                void        *from,
                                void        *to,
                                int          count,
                                f32          size0,
                                f32          size1) __asm__("FUN_L08_002da3f0");

void spawn_particle_line_effect(

    struct Moby *moby, void *from, void *to, int count, f32 size0, f32 size1) {
    Vec4           step;
    Vec4           puff_spawn_position;
    Vec4           effect_initial_velocity;
    f32            grow;
    f32            size;
    unsigned char *puff;
    unsigned char *tail;
    int            alpha, grey;
    int            i;

    fast_vec_sub(&step, to, from);
    fast_vec_scale(&step, &step, 1.0f / (f32)count);
    grow = (size1 - size0) / (f32)count;
    effect_initial_velocity.q = 0;
    effect_initial_velocity.f[3] = 1.0f;
    effect_initial_velocity.f[2] = 0.01f;
    for (i = 0; i < count; i++) {
        fast_vec_scale(&puff_spawn_position, &step, convert_integer_to_float(i));
        fast_vec_add(&puff_spawn_position, from, &puff_spawn_position);
        effect_initial_velocity.f[0] = randf_sym(0.0f, 0.005f);
        effect_initial_velocity.f[1] = randf_sym(0.0f, 0.005f);
        effect_initial_velocity.f[2] =
            random_float_between(line_puff_initial_z_velocity_range_end_gp * 0.1f,
                                 line_puff_initial_z_velocity_range_end_gp);
        alpha = FUN_L00_00257b90(0x30, 0x70);
        grey = FUN_L00_00257b90(0x30, 0x7F);
        grey |= (grey << 16) | (grey << 8);
        size = random_float_between(1.0f, 1.02f);
        puff = spawn_puff(&puff_spawn_position,
                          line_puff_position_jitter_range_end_gp,
                          1.0f,
                          size,
                          FUN_L00_00257b90(-2, 2),
                          &effect_initial_velocity,
                          grow + size0 * 210000.0f,
                          (alpha << 24) | grey);
        if (puff) {
            tail = puff + 0x20;
            if (random_integer_below(2)) {
                puff[3] = 0x44;
                grey = FUN_L00_00257b90(0x60, 0xE0);
                grey |= (grey << 16) | (grey << 8);
                *(u32 *)(puff + 4) = (alpha << 24) | grey;
            }
            *(u16 *)(puff + 0xA) = line_puff_duration_frames_gp;
            *(s32 *)(tail + 4) = 2;
            tail[0xA] = alpha;
            tail[0xB] = line_puff_duration_frames_gp;
        }
    }
    effect_initial_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[2] = 0.0f;
    effect_initial_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(from,
                &effect_initial_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                1,
                10000.0f);
    effect_initial_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[2] = 0.0f;
    effect_initial_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(to,
                &effect_initial_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                1,
                20000.0f);
}
