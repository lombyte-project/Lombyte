/* Level 01 moby class rising_particle_and_debris_emitter. */
#include "types.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

#include "sda.h"

#include "eetypes.h"
#include "qcopy.h"

extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;

typedef union {
    u128 q;
    f32  f[4];
} SteamVec;

typedef struct {
    u32 c[3];
} SteamColors;

typedef struct {
    s32 active;
    s16 puff_timer;
    s16 burst_timer;
    s32 x8;
    f32 height;
} SteamVars;

typedef struct {
    u8         pad0[0x10];
    SteamVec   pos;
    u8         pad20[0x58];
    SteamVars *pvars;
} SteamMoby;

typedef struct {
    u8  pad0[0x2C];
    f32 floor_z;
} SteamParticle;

SteamColors D_L01_00161C08 = {
    {0x2b8, 0x2b9, 0x2ba}
};

extern SteamColors l01_rising_particle_variant_indices __asm__("D_L01_00161C18");
extern void
spawn_debris_chunk(void *, void *, u32, s32, f32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");
extern void *mine_debris(void *, void *, u32, u32, f32, s32, u32) __asm__("FUN_L00_0026bed0");

void update_rising_particle_and_debris_emitter(SteamMoby *steam_moby) __asm__("FUN_L01_002f8c58");

void update_rising_particle_and_debris_emitter(SteamMoby *steam_moby) {
    SteamVars     *steam_vars;
    SteamColors    colors;
    SteamVec       particle_initial_velocity;
    SteamVec       pos;
    SteamVec       puff;
    SteamParticle *debris_particle;
    f32            spawn_radius_or_vertical_velocity_increment;
    f32            spawn_offset_angle_radians;
    f32            size;
    s32            life;
    s32            i;

    steam_vars = steam_moby->pvars;
    fast_dec_timer_s16(&steam_vars->puff_timer);
    fast_dec_timer_s16(&steam_vars->burst_timer);
    if (steam_vars->active == 0) {
        return;
    }
    if (fast_dec_timer(steam_vars)) {
        steam_vars->puff_timer = scale_game_frames(0x78);
    }
    if (steam_vars->burst_timer == 0 && random_integer_below(100) < 5) {
        colors = D_L01_00161C08;
        pos.q = 0;
        pos.f[0] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        pos.f[1] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        particle_initial_velocity.q = pos.q;
        spawn_radius_or_vertical_velocity_increment = random_float_between(0.0f, 0.25f);
        spawn_offset_angle_radians = random_angle_radians();
        pos.f[0] =
            fast_cos(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        pos.f[1] =
            fast_sin(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        pos.f[2] = 0.0f;
        fast_vec_add(&pos, &pos, &steam_moby->pos);
        pos.f[2] += steam_vars->height;
        spawn_debris_chunk(&pos,
                           &particle_initial_velocity,
                           colors.c[random_integer_below(3)],
                           random_int_between(0xB4, 0x12C),
                           0.05f,
                           1.0f,
                           1.0f,
                           0.75f,
                           0);
    }
    for (i = 0; i < 2; i++) {
        colors = l01_rising_particle_variant_indices;
        puff.q = 0;
        puff.f[0] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        puff.f[1] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        puff.f[2] = random_float_between(frame_time_macro * 0.0f, frame_time_macro * 3.0f);
        particle_initial_velocity.q = puff.q;
        spawn_radius_or_vertical_velocity_increment = random_float_between(0.0f, 0.25f);
        spawn_offset_angle_radians = random_angle_radians();
        puff.f[0] =
            fast_cos(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        puff.f[1] =
            fast_sin(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        puff.f[2] = 0.0f;
        fast_vec_add(&puff, &puff, &steam_moby->pos);
        spawn_radius_or_vertical_velocity_increment *= frame_time_macro;
        spawn_radius_or_vertical_velocity_increment *= 8.0f;
        puff.f[2] += steam_vars->height;
        particle_initial_velocity.f[2] += spawn_radius_or_vertical_velocity_increment;
        size = random_float_between(70000.0f, 140000.0f);
        life = scale_game_frames(0xB4);
        debris_particle = mine_debris(&puff,
                                      &particle_initial_velocity,
                                      0x0F081020,
                                      0x00081020,
                                      size,
                                      life,
                                      colors.c[random_integer_below(3)]);
        if (debris_particle != 0) {
            debris_particle->floor_z = steam_moby->pos.f[2] - 0.5f;
        }
    }
}
