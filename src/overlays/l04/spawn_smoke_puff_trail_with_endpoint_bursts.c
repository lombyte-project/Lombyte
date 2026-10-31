/* Level 04 smoke puff trail effect, used by the class 0xb8 projectile. */
#include "types.h"
#include "rnc/overlay/l04/spawn_smoke_puff_trail_with_endpoint_bursts_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/math/vector.h"

extern float fast_cos(float);
extern float fast_sin(float);
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");

typedef struct {
    u8  pad0[4];
    s32 mode; /* 0x04 */
    u8  pad8[2];
    u8  tint; /* 0x0A */
    u8  fade; /* 0x0B */
} PuffTail;

/* A smoke puff from FUN_L00_0026d000. */
typedef struct {
    u8       pad0[3];
    u8       alpha; /* 0x03 */
    s32      color; /* 0x04 */
    u8       pad8[2];
    s16      life; /* 0x0A */
    u8       padC[0x14];
    PuffTail tail; /* 0x20 */
} Puff;

extern f32   randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
extern Puff *spawn_puff(void *, f32, f32, f32, int, void *, f32, int) __asm__("FUN_L00_0026d000");
extern void  spawn_burst(void *, void *, int, int, int, int, f32) __asm__("FUN_L00_0026cbb0");
extern f32   smoke_trail_puff_vertical_velocity_scale_gp __asm__("D_L04_001618B4")
    __attribute__((sda));

#include "rnc/overlay/l04/spawn_smoke_puff_trail_with_endpoint_bursts_data/spawn_smoke_puff_trail_with_endpoint_bursts.h"

/* Lays a trail of smoke puffs from FROM to TO with a burst at each end. */
void spawn_smoke_puff_trail_with_endpoint_bursts(void *owner,
                                                 void *from,
                                                 void *to,
                                                 int   count,
                                                 float from_puff_scale,
                                                 float to_puff_scale) __asm__("FUN_L04_002c41e0");

void spawn_smoke_puff_trail_with_endpoint_bursts(

    void *unused_moby,
    void *from,
    void *to,
    int   count,
    float from_puff_scale,

    float to_puff_scale) {
    Vec4      step;
    Vec4      at;
    Vec4      particle_spawn_velocity;
    float     grow;
    int       i;
    int       tint;
    int       grey;
    float     size;
    Puff     *puff;
    PuffTail *tail;
    int       spin;

    fast_vec_sub(&step, to, from);
    fast_vec_scale(&step, &step, 1.0f / (float)count);
    grow = (to_puff_scale - from_puff_scale) / (float)count;
    particle_spawn_velocity.q = 0;
    particle_spawn_velocity.f[3] = 1.0f;
    particle_spawn_velocity.f[2] = 0.01f;
    for (i = 0; i < count; i++) {
        fast_vec_scale(&at, &step, convert_integer_to_float(i));
        fast_vec_add(&at, from, &at);
        particle_spawn_velocity.f[0] = randf_sym(0.0f, 0.005f);
        particle_spawn_velocity.f[1] = randf_sym(0.0f, 0.005f);
        particle_spawn_velocity.f[2] =
            random_float_between(smoke_trail_puff_vertical_velocity_scale_gp * 0.1f,
                                 smoke_trail_puff_vertical_velocity_scale_gp);
        tint = random_int_between(0x40, 0x70);
        grey = random_int_between(0x40, 0x7F);
        grey = grey | (grey << 16 | grey << 8);
        size = random_float_between(1.0f, 1.02f);
        spin = random_int_between(-2, 2);
        puff = spawn_puff(&at,
                          smoke_trail_puff_position_jitter_scale_gp,
                          1.0f,
                          size,
                          spin,
                          &particle_spawn_velocity,
                          grow + from_puff_scale * 210000.0f,
                          tint << 24 | grey);
        if (puff != NULL) {
            tail = &puff->tail;
            if (random_integer_below(2) != 0) {
                puff->alpha = 0x7E;
                grey = random_int_between(0x60, 0xE0);
                grey = grey | (grey << 16 | grey << 8);
                puff->color = tint << 24 | grey;
            }
            puff->life = scale_game_frames(0x5A);
            tail->mode = 2;
            tail->tint = tint;
            tail->fade = scale_game_frames(0x5A);
        }
    }
    particle_spawn_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[2] = 0.0f;
    particle_spawn_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(from,
                &particle_spawn_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                random_int_between(scale_game_frames(10), scale_game_frames(20)),
                1,
                10000.0f);
    particle_spawn_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    particle_spawn_velocity.f[2] = 0.0f;
    particle_spawn_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(to,
                &particle_spawn_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                random_int_between(scale_game_frames(10), scale_game_frames(20)),
                1,
                20000.0f);
}
