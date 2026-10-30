/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"
#include "rnc/overlay/shared/mobys/ground_height_at.h"

extern void update_falling_effect_particle() __asm__("FUN_L00_00268d48");
extern void update_timed_effect_particle() __asm__("FUN_L00_00268fe8");
extern void update_three_phase_effect_particle(void) __asm__("FUN_L00_00269290");
extern void update_drifting_effect_particle_with_distance_fade(void) __asm__("FUN_L00_002694f8");
extern void update_damped_effect_particle(void) __asm__("FUN_L00_00269a70");
extern void update_scalar_and_fade_effect_particle(void) __asm__("FUN_L00_00269c70");
extern void update_colliding_effect_particle(void) __asm__("FUN_L00_00269d20");
extern void FUN_L00_0026a418(void);
extern void update_animated_fading_effect_particle(void) __asm__("FUN_L00_0026a5b8");
extern void update_rotating_grayscale_effect_particle(void) __asm__("FUN_L00_0026a6f0");
extern void update_damped_blue_shift_effect_particle(void) __asm__("FUN_L00_0026a880");
extern void update_branching_and_fading_effect_particle(void) __asm__("FUN_L00_0026ac78");
extern void update_spinning_and_fading_effect_particle(void) __asm__("FUN_L00_0026b500");
extern void update_spinning_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026b910");
extern void update_attached_or_fading_effect_particle(void) __asm__("FUN_L00_0026ba68");
extern void update_shrinking_effect_particle_with_gravity_and_secondary_spawns(void) __asm__("FUN_L00_0026bca8");
extern void update_fading_debris_particle_with_landing_spawn(void) __asm__("FUN_L00_0026c088");
extern void FUN_L00_0026c3c8(void);
extern void update_descending_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026c558");
extern void move_and_fade_two_point_particle(void) __asm__("FUN_L00_0026c7f0");
extern void update_spiraling_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026c860");
extern void
update_shrinking_effect_particle_with_secondary_spawns(void) __asm__("FUN_L00_0026ccb0");
extern void FUN_L00_00267b10(void);
extern void update_moving_spinning_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026d1b0");
extern void update_source_bound_effect_particle(void) __asm__("FUN_L00_0026d470");
extern void update_color_decaying_effect_particle_with_collision(void) __asm__("FUN_L00_0026d700");
extern void update_source_attached_fading_effect_particle(void) __asm__("FUN_L00_0026dbe0");
extern void move_and_fade_effect_particle_by_lifetime(void) __asm__("FUN_L00_0026de00");
extern void update_moving_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026e150");
extern void
update_animated_source_attached_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026e3a8");
extern void update_linked_effect_particle_with_collision_and_fade(void) __asm__("FUN_L00_0026e670");
extern void update_target_relative_two_point_effect_particle(void) __asm__("FUN_L00_0026eb88");
extern void update_moby_bound_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026ee48");
extern void tick_effect_particle_lifetime(void) __asm__("FUN_L00_0026f048");
extern void update_swaying_and_fading_effect_particle(void) __asm__("FUN_L00_0026f280");
extern void update_ballistic_effect_particle_with_contact_spawns(void) __asm__("FUN_L00_0026f660");
extern void update_flag_controlled_pulsing_effect_particle(void) __asm__("FUN_L00_0026f898");
extern void update_offset_and_pulse_effect_particle(void) __asm__("FUN_L00_0026fa00");
extern void update_damped_and_fading_effect_particle(void) __asm__("FUN_L00_0026fb00");
extern void update_animated_offset_effect_particle(void) __asm__("FUN_L00_0026fc38");
extern void
update_splitting_effect_particle_with_collision_response_and_fade(void) __asm__("FUN_L00_0026ff10");
extern void tick_finite_effect_particle_lifetime(void) __asm__("FUN_L00_00270710");
extern void
update_splitting_effect_particle_with_collision_and_fade(void) __asm__("FUN_L00_00270948");
extern void update_accelerating_fade_in_out_effect_particle(void) __asm__("FUN_L00_002710a0");
extern void update_moving_rotating_resizing_alpha_fade_particle(void) __asm__("FUN_L00_00271450");
extern void
update_moving_resizing_height_snapped_alpha_fade_particle(void) __asm__("FUN_L00_00271748");
extern void update_expanding_rotating_alpha_fade_particle(void) __asm__("FUN_L00_002719f0");
extern void update_expanding_damped_fading_particle(void) __asm__("FUN_L00_00271be8");
extern void update_color_tween_particle(void) __asm__("FUN_L00_00271c88");
extern void update_timed_line_collision_particle(void) __asm__("FUN_L00_00271d20");
extern void update_moving_height_culled_particle(void) __asm__("FUN_L00_00271df8");
extern void update_damped_fading_particle(void) __asm__("FUN_L00_00271ec8");
extern void update_color_size_tween_particle(void) __asm__("FUN_L00_00271fc0");
extern void update_moving_size_alpha_tween_particle(void) __asm__("FUN_L00_002721f0");
extern void update_timed_sphere_probed_particle(void) __asm__("FUN_L00_00272438");
extern void update_moby_point_attached_particle(void) __asm__("FUN_L00_002726d8");
extern void update_expanding_damped_rgb_fading_particle(void) __asm__("FUN_L00_00272838");
extern void update_expanding_alpha_pulse_particle(void) __asm__("FUN_L00_002728e8");
extern void update_color_tween_emitter_particle(void) __asm__("FUN_L00_00272bc0");
extern void update_timed_hero_relative_particle(void) __asm__("FUN_L00_00273088");
extern void update_moving_hero_relative_alpha_fade_particle(void) __asm__("FUN_L00_00273298");
extern void update_moby_point_attached_alpha_pulse_particle(void) __asm__("FUN_L00_00273448");
extern void update_mode_controlled_alpha_fade_particle(void) __asm__("FUN_L00_00273800");
extern void update_gravity_floor_burst_particle(void) __asm__("FUN_L00_00273a68");
extern void FUN_L00_002738b0(void);
extern void update_line_collision_rgb_fading_particle(void) __asm__("FUN_L00_00273ee0");
extern void update_moving_resizing_periodic_alpha_fade_particle(void) __asm__("FUN_L00_00274308");
extern void update_mode_controlled_orbiting_trail_particle(void) __asm__("FUN_L00_00274518");
extern void update_randomized_moby_point_attached_particle(void) __asm__("FUN_L00_00274818");
extern void update_mode_controlled_rotating_alpha_fade_particle(void) __asm__("FUN_L00_00274a70");
extern void update_periodic_alpha_fade_particle(void) __asm__("FUN_L00_00274de0");
extern void update_three_phase_motion_alpha_fade_particle(void) __asm__("FUN_L00_00274e48");
extern void update_size_gated_rotating_particle(void) __asm__("FUN_L00_002752e0");
extern void update_gravity_ground_culled_particle(void) __asm__("FUN_L00_00275320");
extern void update_damped_moby_relative_color_size_tween_particle(void) __asm__("FUN_L00_00275510");
extern void update_gravity_damped_resizing_particle(void) __asm__("FUN_L00_002756f0");
extern void update_timed_alpha_fade_particle(void) __asm__("FUN_L00_00275810");
extern void update_moving_lifetime_scaled_alpha_particle(void) __asm__("FUN_L00_00275860");
extern void update_proximity_corrected_size_alpha_tween_particle(void) __asm__("FUN_L00_00275aa0");
extern void update_moving_fading_particle(void) __asm__("FUN_L00_00275d68");
extern void update_mode_controlled_shrinking_color_tween_particle(void) __asm__("FUN_L00_00275eb0");

extern u32 effect_handlers[] __asm__("D_L00_001B1E80");

void register_part_types(void) __asm__("FUN_L00_00268900");

void register_part_types(void) {
    u32 *handlers = effect_handlers;

    handlers[0] = (u32)update_falling_effect_particle;
    handlers[1] = (u32)update_timed_effect_particle;
    handlers[2] = (u32)update_three_phase_effect_particle;
    handlers[3] = (u32)update_drifting_effect_particle_with_distance_fade;
    handlers[4] = (u32)update_damped_effect_particle;
    handlers[5] = (u32)update_scalar_and_fade_effect_particle;
    handlers[6] = (u32)update_colliding_effect_particle;
    handlers[7] = (u32)FUN_L00_0026a418;
    handlers[8] = (u32)update_animated_fading_effect_particle;
    handlers[9] = (u32)update_rotating_grayscale_effect_particle;
    handlers[10] = (u32)update_damped_blue_shift_effect_particle;
    handlers[11] = (u32)update_branching_and_fading_effect_particle;
    handlers[12] = (u32)update_spinning_and_fading_effect_particle;
    handlers[13] = (u32)update_spinning_fade_in_out_effect_particle;
    handlers[14] = (u32)update_attached_or_fading_effect_particle;
    handlers[15] = (u32)update_shrinking_effect_particle_with_gravity_and_secondary_spawns;
    handlers[16] = (u32)update_fading_debris_particle_with_landing_spawn;
    handlers[17] = (u32)FUN_L00_0026c3c8;
    handlers[18] = (u32)update_descending_fade_in_out_effect_particle;
    handlers[19] = (u32)move_and_fade_two_point_particle;
    handlers[20] = (u32)update_spiraling_fade_in_out_effect_particle;
    handlers[21] = (u32)update_shrinking_effect_particle_with_secondary_spawns;
    handlers[22] = (u32)FUN_L00_00267b10;
    handlers[23] = (u32)update_moving_spinning_fade_in_out_effect_particle;
    handlers[24] = (u32)update_source_bound_effect_particle;
    handlers[25] = (u32)update_color_decaying_effect_particle_with_collision;
    handlers[26] = (u32)update_source_attached_fading_effect_particle;
    handlers[27] = (u32)move_and_fade_effect_particle_by_lifetime;
    handlers[28] = (u32)update_moving_fade_in_out_effect_particle;
    handlers[29] = (u32)update_animated_source_attached_fade_in_out_effect_particle;
    handlers[30] = (u32)update_linked_effect_particle_with_collision_and_fade;
    handlers[31] = (u32)update_target_relative_two_point_effect_particle;
    handlers[32] = (u32)update_moby_bound_fade_in_out_effect_particle;
    handlers[33] = (u32)tick_effect_particle_lifetime;
    handlers[34] = (u32)update_swaying_and_fading_effect_particle;
    handlers[35] = (u32)update_ballistic_effect_particle_with_contact_spawns;
    handlers[36] = (u32)update_flag_controlled_pulsing_effect_particle;
    handlers[37] = (u32)update_offset_and_pulse_effect_particle;
    handlers[38] = (u32)update_damped_and_fading_effect_particle;
    handlers[39] = (u32)update_animated_offset_effect_particle;
    handlers[40] = (u32)update_splitting_effect_particle_with_collision_response_and_fade;
    handlers[41] = (u32)tick_finite_effect_particle_lifetime;
    handlers[42] = (u32)update_splitting_effect_particle_with_collision_and_fade;
    handlers[43] = (u32)update_accelerating_fade_in_out_effect_particle;
    handlers[44] = (u32)update_moving_rotating_resizing_alpha_fade_particle;
    handlers[45] = (u32)update_moving_resizing_height_snapped_alpha_fade_particle;
    handlers[46] = (u32)update_expanding_rotating_alpha_fade_particle;
    handlers[47] = (u32)update_expanding_damped_fading_particle;
    handlers[48] = (u32)update_color_tween_particle;
    handlers[49] = (u32)update_timed_line_collision_particle;
    handlers[50] = (u32)update_moving_height_culled_particle;
    handlers[51] = (u32)update_damped_fading_particle;
    handlers[52] = (u32)update_color_size_tween_particle;
    handlers[53] = (u32)update_moving_size_alpha_tween_particle;
    handlers[54] = (u32)update_timed_sphere_probed_particle;
    handlers[55] = (u32)update_moby_point_attached_particle;
    handlers[56] = (u32)update_expanding_damped_rgb_fading_particle;
    handlers[57] = (u32)update_expanding_alpha_pulse_particle;
    handlers[58] = (u32)update_color_tween_emitter_particle;
    handlers[59] = (u32)update_timed_hero_relative_particle;
    handlers[60] = (u32)update_moving_hero_relative_alpha_fade_particle;
    handlers[61] = (u32)update_moby_point_attached_alpha_pulse_particle;
    handlers[62] = (u32)update_mode_controlled_alpha_fade_particle;
    handlers[63] = (u32)FUN_L00_002738b0;
    handlers[64] = (u32)update_gravity_floor_burst_particle;
    handlers[65] = (u32)update_line_collision_rgb_fading_particle;
    handlers[66] = (u32)update_moving_resizing_periodic_alpha_fade_particle;
    handlers[67] = (u32)update_mode_controlled_orbiting_trail_particle;
    handlers[68] = (u32)update_randomized_moby_point_attached_particle;
    handlers[69] = (u32)update_mode_controlled_rotating_alpha_fade_particle;
    handlers[70] = (u32)update_periodic_alpha_fade_particle;
    handlers[71] = (u32)update_three_phase_motion_alpha_fade_particle;
    handlers[72] = (u32)update_size_gated_rotating_particle;
    handlers[73] = (u32)update_gravity_ground_culled_particle;
    handlers[74] = (u32)update_damped_moby_relative_color_size_tween_particle;
    handlers[75] = (u32)update_gravity_damped_resizing_particle;
    handlers[76] = (u32)update_timed_alpha_fade_particle;
    handlers[77] = (u32)update_moving_lifetime_scaled_alpha_particle;
    handlers[78] = (u32)update_proximity_corrected_size_alpha_tween_particle;
    handlers[79] = (u32)update_moving_fading_particle;
    handlers[80] = (u32)update_mode_controlled_shrinking_color_tween_particle;
}

/*
 * Falling effect particle (one kind of the pool behind FUN_L00_002678b8; its
 * slot is freed by FUN_L00_00267a08). Each frame both points move by the
 * velocity plus a global drift, gravity pulls the velocity down, and the
 * particle dies when it leaves the 512 x 512 map or drops below the ground.
 * The ground height is sampled again only when the probe enters a new
 * 1 x 1 cell, and cached in vel.w.
 */
typedef struct {
    u8   pad_00[0xA];
    s16  unk0A; /* 2 skips the ground test (a timer in sibling kinds) */
    u8   pad_0C[4];
    Vec4 pos;   /* 0x10: drawn position */
    Vec4 probe; /* 0x20: point tested against the ground */
    Vec4 vel;   /* 0x30: w = ground height + clearance */
} FallingParticle;

/* .sdata placement is load-bearing */
extern Vec4 particle_drift __asm__("D_L00_00160250") MACRO_ADDR;

typedef struct {
    f32 value;
} ParticleClearance;

/* read through a one-field struct: a plain f32 changes the addressing */
extern ParticleClearance particle_clearance __asm__("D_L00_00160258");
extern f32               particle_min_ground __asm__("D_L00_00160260");

void update_falling_effect_particle(FallingParticle *particle) {
    Vec4  step;
    Vec4  ground_test_probe;
    Vec4  ground_sample_probe;
    Vec4 *vel = &particle->vel;
    f32   ground = vel->f[3];
    s32   cell_x;
    s32   cell_y;

    if (particle->pos.f[2] < ground || particle->pos.f[0] < 0.0f || particle->pos.f[1] < 0.0f ||
        512.0f < particle->pos.f[0] || 512.0f < particle->pos.f[1])
        goto die;

    cell_x = particle->probe.f[0];
    cell_y = particle->probe.f[1];
    fast_vec_add(&step, vel, &particle_drift);
    fast_vec_add(&particle->probe, &particle->probe, &step);
    fast_vec_add(&particle->pos, &particle->pos, &step);
    if (particle->unk0A != 2) {
        if (cell_x != (s32)particle->probe.f[0] || cell_y != (s32)particle->probe.f[1]) {
            ground_test_probe.q = particle->probe.q;
            if (ground_height_at(&ground_test_probe) < particle_min_ground) {
                ground = particle_min_ground;
            } else {
                ground_sample_probe.q = particle->probe.q;
                ground = ground_height_at(&ground_sample_probe);
            }
            if (particle->probe.f[2] < ground) {
                /* shared exit; retail has one call site for all kill paths */
            die:
                free_effect_particle(particle);
                return;
            }
        }
    }
    vel->f[2] -= frame_time_sq * 9.8f;
    vel->f[3] = ground + particle_clearance.value;
}

extern unsigned char *D_L00_001B2084 NOT_SDA;
void                                 spawn_timed_effect_particle(Vec4 *spawn_position,
                                                                 int   initial_high_byte) __asm__("FUN_L00_00268f10");

void spawn_timed_effect_particle(Vec4 *spawn_position, int initial_high_byte) {
    unsigned char *particle = create_part(1);
    if (particle) {
        spawn_position->f[2] += 0.1f;
        qcopy(particle + 0x10, spawn_position);
        *(int *)(particle + 4) = (initial_high_byte << 24) | 0x907070;
        particle[9] = truncate_float_to_s32(2.0f) + 0x40;
        particle[3] = 0x48;
        particle[1] = 0;
        particle[2] = *D_L00_001B2084;
        *(float *)(particle + 0xc) = 31500.002f;
        particle[8] = 0xa0;
        *(short *)(particle + 0xa) = scale_game_frames(0x1b);
        *(int *)(particle + 0x20) = initial_high_byte;
    }
}

void FUN_L00_00267a08(unsigned char *);

void update_timed_effect_particle(unsigned char *particle) {
    int high_byte_duration_frames = scale_game_frames(0x1b);
    int k = *(short *)(particle + 0xa) * *(int *)(particle + 0x20) / high_byte_duration_frames;
    *(int *)(particle + 4) = (*(int *)(particle + 4) & 0xffffff) | (k << 24);
    if (fast_dec_timer_s16(particle + 0xa))
        FUN_L00_00267a08(particle);
    else {
        int lifetime_frames = scale_game_frames(0x1b);
        int table_duration_frames = scale_game_frames(0x1b);
        particle[2] = D_L00_001B2084[(lifetime_frames - *(short *)(particle + 0xa)) * 9 /
                                     table_duration_frames];
    }
}

s32 D_L00_001B1E84[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1E88[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1E8C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1E90[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1E94[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1E98[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1E9C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EA0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EA4[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EA8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EAC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EB0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EB4[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EB8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EBC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EC0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EC4[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EC8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1ECC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1ED0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1ED4[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1ED8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EDC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EE0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EE4[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EE8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EEC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EF0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EF8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1EFC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F00[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F04[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F08[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F0C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F10[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F14[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F18[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F1C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F20[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F24[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F28[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F2C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F30[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F34[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F38[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F3C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F40[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F44[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F48[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F4C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F50[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F54[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F58[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F5C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F60[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F64[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F70[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F74[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F78[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F7C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F80[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F84[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F88[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F8C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F90[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F94[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F98[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1F9C[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FA0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FA4[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FA8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FAC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FB0[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FB4[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FB8[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001B1FBC[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23C0[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23C4[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23C8[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23CC[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23D0[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23D4[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23D8[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23DC[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23E0[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23E4[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23E8[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23EC[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23F0[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B23F4[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2408[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2428[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B242C[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2430[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2434[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B2438[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001B243C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2484[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2488[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24A4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24A8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24AC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24B0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24B4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24B8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24BC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24C0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24C4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24C8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24CC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24D0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24D4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24D8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24DC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24E0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24E4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24E8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24EC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24F0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24F4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B24F8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2504[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2508[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2514[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2520[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2524[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2528[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B252C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2530[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2534[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2538[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B253C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2540[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2544[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2548[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B254C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2550[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2554[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2558[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B255C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2560[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2564[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2568[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B256C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2570[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2574[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2578[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2584[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B258C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2590[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2594[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B2598[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B259C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25A0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25A4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25A8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25AC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25B0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25B4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25B8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001B25BC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1F84[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1F88[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1F8C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1F90[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1F94[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1F98[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1F9C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FA0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FA4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FA8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FAC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FB0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FB4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FB8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FBC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FC0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FC4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FC8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FCC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FD0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FD4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FD8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FE4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FE8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FEC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B1FF0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2000[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2004[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2008[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B200C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2010[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2014[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2018[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2020[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2024[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2028[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B202C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2034[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B203C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2040[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2044[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2048[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2054[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2058[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B205C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2060[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2064[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B207C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2080[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2084[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2088[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B208C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2090[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2094[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B2098[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B209C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20A0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20A4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20A8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20AC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20B0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20B4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20B8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B20BC[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2004[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2008[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B200C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2010[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2014[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2018[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2020[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2024[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2028[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B202C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2034[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B203C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2040[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2044[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2048[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2054[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2058[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B205C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2060[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2064[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B207C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2080[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2084[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2088[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B208C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2090[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2094[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2098[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B209C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20A0[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20A4[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20A8[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20AC[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20B0[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20B4[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20B8[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20BC[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20C0[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20C4[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20C8[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20CC[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20D4[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20D8[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B20F0[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2108[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B210C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2110[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2114[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2118[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B2134[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23C0[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23C4[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23C8[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23CC[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23D0[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23D4[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23D8[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23DC[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23E0[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23E4[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23E8[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23EC[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23F0[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B23F4[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2408[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2428[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B242C[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2430[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2434[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B2438[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B243C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2604[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2608[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B260C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2610[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2614[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2618[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B261C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2620[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2624[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2628[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B262C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2630[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2634[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2638[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B263C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2640[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2644[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2648[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B264C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2650[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2654[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2658[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B265C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2660[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2664[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2668[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B266C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2670[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2674[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2688[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B268C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2690[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2694[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2698[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B269C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26A0[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26A4[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26A8[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26AC[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26B0[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26B4[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26B8[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26BC[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26C0[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26C4[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26C8[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26CC[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26D0[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26D4[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26D8[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26DC[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26E0[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26E4[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26E8[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26EC[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26F0[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B26F4[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2708[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B270C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2710[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2714[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2718[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B271C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2720[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2724[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2728[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B272C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2730[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2734[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2738[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B273C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F04[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F08[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F0C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F10[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F14[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F18[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F1C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F20[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F24[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F28[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F2C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F30[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F34[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F38[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F3C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F40[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F44[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F48[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F4C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F50[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F54[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F58[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F5C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F60[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F64[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F70[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F74[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F78[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F7C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F80[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F84[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F88[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F8C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F90[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F94[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F98[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1F9C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FA0[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FA4[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FA8[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FAC[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FB0[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FB4[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FB8[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FBC[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FC0[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FC4[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FC8[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FCC[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FD0[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FD4[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FD8[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FE4[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FE8[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FEC[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B1FF0[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2000[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2004[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2008[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B200C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2010[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2014[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2018[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2020[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2024[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2028[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B202C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B2034[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B203C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2688[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B268C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2690[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2694[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2698[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B269C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26A0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26A4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26A8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26AC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26B0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26B4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26B8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26BC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26C0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26C4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26C8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26CC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26D0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26D4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26D8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26DC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26E0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26E4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26E8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26EC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26F0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B26F4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2708[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B270C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2710[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2714[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2718[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B271C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2720[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2724[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2728[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B272C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2730[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2734[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2738[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B273C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2740[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2744[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2748[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B274C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2750[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2754[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2758[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B275C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2760[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2764[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2768[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B276C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2770[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2774[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2778[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2784[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2788[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B278C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2790[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B2794[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B27A0[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B27A4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B27A8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B27AC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B27B8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B27BC[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2004[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2008[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B200C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2010[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2014[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2018[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2020[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2024[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2028[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B202C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2034[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B203C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2040[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2044[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2048[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2054[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2058[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B205C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2060[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2064[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B207C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2080[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2084[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2088[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B208C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2090[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2094[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2098[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B209C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20A0[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20A4[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20A8[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20AC[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20B0[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20B4[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20B8[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20BC[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20C0[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20C4[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20C8[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20CC[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20D4[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20D8[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B20F0[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2108[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B210C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2110[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2114[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2118[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B2134[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23C0[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23C4[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23C8[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23CC[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23D0[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23D4[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23D8[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23DC[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23E0[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23E4[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23E8[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23EC[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23F0[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B23F4[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2408[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2428[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B242C[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2430[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2434[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B2438[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B243C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2888[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B288C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2890[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2894[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2898[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B289C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28A0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28A4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28A8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28AC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28B0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28B4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28B8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28BC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28C0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28C4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28C8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28CC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28D0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28D4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28D8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28DC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28E0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28E4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28E8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28EC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28F0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28F4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28F8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B28FC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2900[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2904[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2908[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B290C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2910[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2914[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2918[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B291C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2920[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2924[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2928[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B292C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2930[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2934[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2938[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B293C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2940[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2944[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2948[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B294C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2950[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2954[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2958[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B295C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2960[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2964[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2968[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B296C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2970[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2974[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2978[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B297C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2980[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2984[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2988[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B298C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2990[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2994[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2998[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B299C[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29A0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29A4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29A8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29AC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29B0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29B4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29B8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B29BC[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23C0[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23C4[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23C8[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23CC[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23D0[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23D4[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23D8[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23DC[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23E0[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23E4[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23E8[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23EC[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23F0[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B23F4[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2408[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2428[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B242C[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2430[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2434[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B2438[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B243C[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B2184[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B2188[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B2190[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B2198[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21A8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21AC[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21B0[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21B4[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21B8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21BC[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21C0[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21C4[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21C8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21CC[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21D4[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21D8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B21F0[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B220C[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B2210[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2584[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B258C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2590[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2594[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2598[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B259C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25A0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25A4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25A8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25AC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25B0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25B4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25B8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25BC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25C0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25C4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25C8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25CC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25D0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25D4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25D8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25DC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25E0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25E4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25E8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25EC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25F0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B25F8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2604[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2608[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B260C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2610[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2614[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2618[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B261C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2620[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2624[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2628[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B262C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2630[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2634[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2638[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B263C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2640[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2644[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2648[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B264C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2650[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2654[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2658[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B265C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2660[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2664[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2668[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B266C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2670[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2674[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2688[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B268C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2690[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2694[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B2698[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B269C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26A0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26A4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26A8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26AC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26B0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26B4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26B8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B26BC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2484[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2488[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24A4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24A8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24AC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24B0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24B4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24B8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24BC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24C0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24C4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24C8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24CC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24D0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24D4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24D8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24DC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24E0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24E4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24E8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24EC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24F0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24F4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B24F8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2504[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2508[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2514[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2520[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2524[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2528[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B252C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2530[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2534[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2538[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B253C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2540[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2544[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2548[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B254C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2550[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2554[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2558[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B255C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2560[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2564[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2568[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B256C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2570[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2574[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2578[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2584[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2588[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B258C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2590[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2594[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B2598[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B259C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25A0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25A4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25A8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25AC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25B0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25B4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25B8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B25BC[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23C0[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23C4[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23C8[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23CC[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23D0[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23D4[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23D8[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23DC[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23E0[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23E4[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23E8[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23EC[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23F0[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B23F4[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2408[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2428[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B242C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2430[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2434[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B2438[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001B243C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2784[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2788[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B278C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2790[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2794[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27A0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27A4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27A8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27AC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27B8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27BC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27C0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27C4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27D0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27D4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27D8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27DC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27E0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27E4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27E8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27EC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27F0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27F4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27F8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B27FC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2808[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B280C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2810[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2814[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2820[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2824[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2828[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B282C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2830[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2834[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2838[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B283C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2840[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2844[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2848[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B284C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2850[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2854[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2858[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B285C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2860[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2864[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2868[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B286C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2870[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2874[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2878[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B287C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2880[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2888[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B288C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2890[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2894[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B2898[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B289C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28A0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28A4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28A8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28AC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28B0[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28B4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28B8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B28BC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2888[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B288C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2890[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2894[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2898[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B289C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28A0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28A4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28A8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28AC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28B0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28B4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28B8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28BC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28C0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28C4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28C8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28CC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28D0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28D4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28D8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28DC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28E0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28E4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28E8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28EC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28F0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28F4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28F8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B28FC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2900[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2904[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2908[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B290C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2910[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2914[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2918[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B291C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2920[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2924[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2928[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B292C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2930[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2934[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2938[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B293C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2940[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2944[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2948[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B294C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2950[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2954[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2958[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B295C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2960[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2964[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2968[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B296C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2970[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2974[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2978[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B297C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2980[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2984[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2988[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B298C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2990[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2994[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B2998[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B299C[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29A0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29A4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29A8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29AC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29B0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29B4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29B8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001B29BC[1] __attribute__((section(".data"))) = {0};

Vec4 D_L01_00160250 = {0};

ParticleClearance D_L01_00160258 = {0};

f32                           D_L01_00160260 = 0;
unsigned char *D_L01_001B2504 NOT_SDA = 0;
Vec4                          D_L02_00160250 = {0};

ParticleClearance D_L02_00160258 = {0};

f32                           D_L02_00160260 = 0;
unsigned char *D_L02_001B2684 NOT_SDA = 0;
Vec4                          D_L03_00160250 = {0};

ParticleClearance D_L03_00160258 = {0};

f32                           D_L03_00160260 = 0;
unsigned char *D_L03_001B2184 NOT_SDA = 0;
Vec4                          D_L04_00160250 = {0};

ParticleClearance D_L04_00160258 = {0};

f32                           D_L04_00160260 = 0;
unsigned char *D_L04_001B2204 NOT_SDA = 0;
Vec4                          D_L05_00160250 = {0};

ParticleClearance D_L05_00160258 = {0};

f32                           D_L05_00160260 = 0;
unsigned char *D_L05_001B2504 NOT_SDA = 0;
Vec4                          D_L06_00160250 = {0};

ParticleClearance D_L06_00160258 = {0};

f32                           D_L06_00160260 = 0;
unsigned char *D_L06_001B2804 NOT_SDA = 0;
Vec4                          D_L07_00160250 = {0};

ParticleClearance D_L07_00160258 = {0};

f32                           D_L07_00160260 = 0;
unsigned char *D_L07_001B2104 NOT_SDA = 0;
ParticleClearance             D_L08_00160258 = {0};

unsigned char *D_L08_001B2884 NOT_SDA = 0;
Vec4                          D_L09_00160250 = {0};

ParticleClearance D_L09_00160258 = {0};

f32                           D_L09_00160260 = 0;
unsigned char *D_L09_001B2204 NOT_SDA = 0;
Vec4                          D_L10_00160250 = {0};

ParticleClearance D_L10_00160258 = {0};

f32                           D_L10_00160260 = 0;
unsigned char *D_L10_001B2504 NOT_SDA = 0;
Vec4                          D_L11_00160250 = {0};

ParticleClearance D_L11_00160258 = {0};

f32                           D_L11_00160260 = 0;
unsigned char *D_L11_001B2A84 NOT_SDA = 0;
ParticleClearance             D_L12_00160258 = {0};

unsigned char *D_L12_001B2504 NOT_SDA = 0;
Vec4                          D_L13_00160250 = {0};

ParticleClearance D_L13_00160258 = {0};

f32                           D_L13_00160260 = 0;
unsigned char *D_L13_001B2384 NOT_SDA = 0;
ParticleClearance             D_L14_00160258 = {0};

unsigned char *D_L14_001B2784 NOT_SDA = 0;
Vec4                          D_L15_00160250 = {0};

ParticleClearance D_L15_00160258 = {0};

f32                           D_L15_00160260 = 0;
unsigned char *D_L15_001B2684 NOT_SDA = 0;
Vec4                          D_L16_00160250 = {0};

ParticleClearance D_L16_00160258 = {0};

f32                           D_L16_00160260 = 0;
unsigned char *D_L16_001B2504 NOT_SDA = 0;
Vec4                          D_L17_00160250 = {0};

ParticleClearance D_L17_00160258 = {0};

f32                           D_L17_00160260 = 0;
unsigned char *D_L17_001B2984 NOT_SDA = 0;
Vec4                          D_L18_00160250 = {0};

ParticleClearance D_L18_00160258 = {0};

f32                           D_L18_00160260 = 0;
unsigned char *D_L18_001B2A84 NOT_SDA = 0;

unsigned char *D_L00_001B2084 NOT_SDA = 0;
