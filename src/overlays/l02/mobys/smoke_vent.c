/* Level 02 moby class smoke_vent (0x5c7): once in view, puffs its alternating and view-gated smoke bursts. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/overlay/l02/mobys/smoke_vent_globals.h"

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern s32  fast_tween_color(s32, s32, f32) __asm__("FUN_001fa6e0");
/* fast_vec_normalize_scale_first: fast_vec_normalize (out = in scaled to length s), with the float declared first (same registers): the scale is then
 * evaluated before the vector addresses, as in retail. */
extern void fast_vec_normalize_scale_first(f32 s, void *out, void *in) __asm__("FUN_001f9bf8");

#include "rnc/overlay/l02/mobys/smoke_vent_data/update_smoke_vent_8.h"

extern s32 alternating_smoke_puff_second_timing_base_gp __asm__("D_L02_0016205C")
    __attribute__((sda));
extern s32 alternating_smoke_puff_third_timing_base_gp __asm__("D_L02_00162060")
    __attribute__((sda));

#include "rnc/overlay/l02/mobys/smoke_vent_data/update_smoke_vent_7.h"

extern f32 alternating_smoke_puff_velocity_w_random_range_start_gp __asm__("D_L02_00162068")
    __attribute__((sda));
extern f32 alternating_smoke_puff_velocity_w_random_range_end_gp __asm__("D_L02_0016206C")
    __attribute__((sda));
extern f32 alternating_smoke_puff_acceleration_w_random_range_start_gp __asm__("D_L02_00162070")
    __attribute__((sda));
extern f32 alternating_smoke_puff_acceleration_w_random_range_end_gp __asm__("D_L02_00162074")
    __attribute__((sda));
s32 D_L02_00162078 = 0x404080ff;
s32 D_L02_0016207C = 0x404080c0;
s32 D_L02_00162080 = 0x20804050;
s32 D_L02_00162084 = 0x20603038;

#include "rnc/overlay/l02/mobys/smoke_vent_data/update_smoke_vent_6.h"

extern s32 view_gated_smoke_puff_second_timing_base_gp __asm__("D_L02_0016209C")
    __attribute__((sda));
extern s32 view_gated_smoke_puff_third_timing_base_gp __asm__("D_L02_001620A0")
    __attribute__((sda));

#include "rnc/overlay/l02/mobys/smoke_vent_data/update_smoke_vent_5.h"

extern f32 view_gated_smoke_puff_velocity_w_random_range_start_gp __asm__("D_L02_001620A8")
    __attribute__((sda));
extern f32 view_gated_smoke_puff_velocity_w_random_range_end_gp __asm__("D_L02_001620AC")
    __attribute__((sda));
extern f32 view_gated_smoke_puff_acceleration_w_random_range_start_gp __asm__("D_L02_001620B0")
    __attribute__((sda));
extern f32 view_gated_smoke_puff_acceleration_w_random_range_end_gp __asm__("D_L02_001620B4")
    __attribute__((sda));
s32 D_L02_001620B8 = 0x6000ff80;
s32 D_L02_001620BC = 0x6020ff40;
s32 D_L02_001620C0 = 0x20408040;
s32 D_L02_001620C4 = 0x20206020;

#include "rnc/overlay/l02/mobys/smoke_vent_data/update_smoke_vent_4.h"

extern s32 view_gated_smoke_burst_second_timing_base_gp __asm__("D_L02_001620E0")
    __attribute__((sda));
extern s32 view_gated_smoke_burst_third_timing_base_gp __asm__("D_L02_001620E4")
    __attribute__((sda));

#include "rnc/overlay/l02/mobys/smoke_vent_data/update_smoke_vent_3.h"

extern f32 view_gated_smoke_burst_velocity_w_random_range_start_gp __asm__("D_L02_001620EC")
    __attribute__((sda));
extern f32 view_gated_smoke_burst_velocity_w_random_range_end_gp __asm__("D_L02_001620F0")
    __attribute__((sda));
extern f32 view_gated_smoke_burst_acceleration_w_random_range_start_gp __asm__("D_L02_001620F4")
    __attribute__((sda));
extern f32 view_gated_smoke_burst_acceleration_w_random_range_end_gp __asm__("D_L02_001620F8")
    __attribute__((sda));
s32        D_L02_001620FC = 0x804080ff;
s32        D_L02_00162100 = 0x8040ffff;
s32        D_L02_00162104 = 0x80204040;
s32        D_L02_00162108 = 0x80204080;
extern s32 view_gated_smoke_burst_interval_min_gp __asm__("D_L02_0016210C") __attribute__((sda));
extern s32 view_gated_smoke_burst_interval_max_gp __asm__("D_L02_00162110") __attribute__((sda));
s32        D_L02_00162114 = 0x10019;

#include "rnc/overlay/l02/mobys/smoke_vent_data/update_smoke_vent_2.h"

extern s32 alternating_smoke_burst_second_timing_base_gp __asm__("D_L02_00162130")
    __attribute__((sda));
extern s32 alternating_smoke_burst_third_timing_base_gp __asm__("D_L02_00162134")
    __attribute__((sda));

#include "rnc/overlay/l02/mobys/smoke_vent_data/smoke_vent_update.h"

extern f32 alternating_smoke_burst_velocity_w_random_range_start_gp __asm__("D_L02_0016213C")
    __attribute__((sda));
extern f32 alternating_smoke_burst_velocity_w_random_range_end_gp __asm__("D_L02_00162140")
    __attribute__((sda));
extern f32 alternating_smoke_burst_acceleration_w_random_range_start_gp __asm__("D_L02_00162144")
    __attribute__((sda));
extern f32 alternating_smoke_burst_acceleration_w_random_range_end_gp __asm__("D_L02_00162148")
    __attribute__((sda));
s32        D_L02_0016214C = 0x804080ff;
s32        D_L02_00162150 = 0x8040ffff;
s32        D_L02_00162154 = 0x80204040;
s32        D_L02_00162158 = 0x80204080;
extern s32 alternating_smoke_burst_interval_min_gp __asm__("D_L02_0016215C") __attribute__((sda));
extern s32 alternating_smoke_burst_interval_max_gp __asm__("D_L02_00162160") __attribute__((sda));
s32        D_L02_00162164 = 0x10019;

/* Smoke vent: once visible within 120 units it puffs bursts (D_L02_001620C8 / D_L02_00162118 parameter blocks)
 * on a timer plus occasional single puffs (D_L02_00162088 / D_L02_00162048); state 2 alternates its puff life.
 * Each burst and puff block has its own locals: shared ones become long-lived pseudos and shift the allocation. */
void smoke_vent_update(struct Moby *moby) __asm__("FUN_L02_002ef020");

/* Moby class 1479 */
void smoke_vent_update(struct Moby *moby) {
    enum {
        SMOKE_VENT_STATE_INITIALIZE = 0,
        SMOKE_VENT_STATE_VIEW_GATED_EMISSION = 1,
        SMOKE_VENT_STATE_ALTERNATING_PUFF_TIMING = 2,
        SMOKE_VENT_STATE_REMOVE = 3
    };
    char *smoke_vent_vars = (char *)moby->pvars;
    Vec4  view_sphere;
    Vec4  smoke_particle_velocity;
    Vec4  acc;
    Vec4  vector_perturbation;
    s32   count1;
    s32   k1;
    s32   time_a1_burst;
    s32   time_b1_burst;
    s32   color_a1_burst;
    s32   color_b1_burst;
    s32   count2;
    s32   k2;
    s32   time_a2_burst;
    s32   time_b2_burst;
    s32   color_a2_burst;
    s32   color_b2_burst;
    s32   time_a1_puff;
    s32   time_b1_puff;
    s32   color_a1_puff;
    s32   color_b1_puff;
    f32   rise1_puff;
    s32   time_a2_puff;
    s32   time_b2_puff;
    s32   color_a2_puff;
    s32   color_b2_puff;
    f32   rise2_puff;

    switch (moby->state) {
    case SMOKE_VENT_STATE_INITIALIZE:
        moby->state = smoke_vent_vars[0] + 1;
        euler_rotation_basis(&moby->rot_row0, &moby->rot);
        break;
    case SMOKE_VENT_STATE_VIEW_GATED_EMISSION:
        qcopy(&view_sphere, &moby->pos);
        view_sphere.f[3] = 6.0f;
        if (fast_bsphere_check(&view_sphere, 120.0f) == -1) {
            break;
        }
        if (fast_dec_timer((s32 *)(smoke_vent_vars + 0xC0)) != 0) {
            *(s32 *)(smoke_vent_vars + 0xC0) = truncate_float_to_s32(
                scale_time(random_float_between((f32)view_gated_smoke_burst_interval_min_gp,
                                                (f32)view_gated_smoke_burst_interval_max_gp)));
            count1 = random_integer_below(7) != 0 ? 1 : random_integer_below(5) + 5;
            for (k1 = 0; k1 < count1; k1++) {
                random_vector(&vector_perturbation,
                              0.0f,
                              view_gated_smoke_burst_jitter_magnitude_max_gp * frame_time);
                fast_vec_normalize_scale_first(
                    view_gated_smoke_burst_velocity_magnitude_scale_gp *
                        (random_float_between(-view_gated_smoke_burst_motion_variation_fraction_gp,
                                              view_gated_smoke_burst_motion_variation_fraction_gp) +
                         1.0f) *
                        frame_time,
                    &smoke_particle_velocity,
                    &moby->rot_row0);
                fast_vec_add(
                    &smoke_particle_velocity, &smoke_particle_velocity, &vector_perturbation);
                fast_vec_normalize_scale_first(
                    view_gated_smoke_burst_acceleration_magnitude_scale_gp *
                        (random_float_between(-view_gated_smoke_burst_motion_variation_fraction_gp,
                                              view_gated_smoke_burst_motion_variation_fraction_gp) +
                         1.0f) *
                        frame_time,
                    &acc,
                    &smoke_particle_velocity);
                acc.f[2] -=
                    view_gated_smoke_burst_vertical_acceleration_decrement_scale_gp * frame_time;
                smoke_particle_velocity.f[3] =
                    random_float_between(view_gated_smoke_burst_velocity_w_random_range_start_gp,
                                         view_gated_smoke_burst_velocity_w_random_range_end_gp);
                acc.f[3] = random_float_between(
                    view_gated_smoke_burst_acceleration_w_random_range_start_gp,
                    view_gated_smoke_burst_acceleration_w_random_range_end_gp);
                color_a1_burst = fast_tween_color(
                    D_L02_001620FC, D_L02_00162100, random_float_between(0.0f, 1.0f));
                color_b1_burst = fast_tween_color(
                    D_L02_00162104, D_L02_00162108, random_float_between(0.0f, 1.0f));
                time_a1_burst = truncate_float_to_s32(
                    scale_time((f32)view_gated_smoke_burst_timing_random_span_gp *
                                   random_float_between(0.0f, 1.0f) +
                               1.0f));
                time_b1_burst = truncate_float_to_s32(scale_time(
                    (f32)view_gated_smoke_burst_second_timing_base_gp *
                    (random_float_between(-view_gated_smoke_burst_timing_variation_fraction_gp,
                                          view_gated_smoke_burst_timing_variation_fraction_gp) +
                     1.0f)));
                spawn_spark(
                    &moby->pos,
                    &smoke_particle_velocity,
                    &acc,
                    color_a1_burst,
                    color_b1_burst,
                    time_a1_burst,
                    time_b1_burst,
                    truncate_float_to_s32(scale_time(
                        (f32)view_gated_smoke_burst_third_timing_base_gp *
                        (random_float_between(-view_gated_smoke_burst_timing_variation_fraction_gp,
                                              view_gated_smoke_burst_timing_variation_fraction_gp) +
                         1.0f))),
                    D_L02_00162114);
            }
        }
        if (random_integer_below(4) != 0) {
            break;
        }
        fast_vec_normalize_scale_first(
            view_gated_smoke_puff_velocity_magnitude_scale_gp *
                (random_float_between(-view_gated_smoke_puff_motion_variation_fraction_gp,
                                      view_gated_smoke_puff_motion_variation_fraction_gp) +
                 1.0f) *
                frame_time,
            &smoke_particle_velocity,
            &moby->rot_row0);
        rise1_puff = view_gated_smoke_puff_vertical_acceleration_scale_gp *
                     (random_float_between(-view_gated_smoke_puff_motion_variation_fraction_gp,
                                           view_gated_smoke_puff_motion_variation_fraction_gp) +
                      1.0f) *
                     frame_time;
        clear_u64_value(&acc);
        acc.f[2] = rise1_puff;
        fast_vec_normalize_scale_first(
            random_float_between(-view_gated_smoke_puff_lateral_acceleration_magnitude_bound_gp,
                                 view_gated_smoke_puff_lateral_acceleration_magnitude_bound_gp) *
                frame_time,
            &vector_perturbation,
            &moby->rot_row1);
        fast_vec_add(&acc, &acc, &vector_perturbation);
        smoke_particle_velocity.f[3] =
            random_float_between(view_gated_smoke_puff_velocity_w_random_range_start_gp,
                                 view_gated_smoke_puff_velocity_w_random_range_end_gp);
        acc.f[3] = random_float_between(view_gated_smoke_puff_acceleration_w_random_range_start_gp,
                                        view_gated_smoke_puff_acceleration_w_random_range_end_gp);
        color_a1_puff =
            fast_tween_color(D_L02_001620B8, D_L02_001620BC, random_float_between(0.0f, 1.0f));
        color_b1_puff =
            fast_tween_color(D_L02_001620C0, D_L02_001620C4, random_float_between(0.0f, 1.0f));
        time_a1_puff = truncate_float_to_s32(scale_time(
            (f32)view_gated_smoke_puff_timing_random_span_gp * random_float_between(0.0f, 1.0f) +
            1.0f));
        time_b1_puff = truncate_float_to_s32(
            scale_time((f32)view_gated_smoke_puff_second_timing_base_gp *
                       (random_float_between(-view_gated_smoke_puff_timing_variation_fraction_gp,
                                             view_gated_smoke_puff_timing_variation_fraction_gp) +
                        1.0f)));
        spawn_spark(&moby->pos,
                    &smoke_particle_velocity,
                    &acc,
                    color_a1_puff,
                    color_b1_puff,
                    time_a1_puff,
                    time_b1_puff,
                    truncate_float_to_s32(scale_time(
                        (f32)view_gated_smoke_puff_third_timing_base_gp *
                        (random_float_between(-view_gated_smoke_puff_timing_variation_fraction_gp,
                                              view_gated_smoke_puff_timing_variation_fraction_gp) +
                         1.0f))),
                    -1);
        break;
    case SMOKE_VENT_STATE_ALTERNATING_PUFF_TIMING:
        if (fast_dec_timer((s32 *)(smoke_vent_vars + 0xC4)) != 0) {
            if (*(s32 *)(smoke_vent_vars + 0xC8) < 21) {
                *(s32 *)(smoke_vent_vars + 0xC8) = 30;
            } else {
                *(s32 *)(smoke_vent_vars + 0xC8) = 10;
            }
            *(s32 *)(smoke_vent_vars + 0xC4) =
                truncate_float_to_s32(scale_time(random_float_between(120.0f, 180.0f)));
        }
        qcopy(&view_sphere, &moby->pos);
        view_sphere.f[3] = 6.0f;
        if (fast_bsphere_check(&view_sphere, 120.0f) == -1 && random_integer_below(4) != 0) {
            break;
        }
        if (fast_dec_timer((s32 *)(smoke_vent_vars + 0xC0)) != 0) {
            *(s32 *)(smoke_vent_vars + 0xC0) = truncate_float_to_s32(
                scale_time(random_float_between((f32)alternating_smoke_burst_interval_min_gp,
                                                (f32)alternating_smoke_burst_interval_max_gp)));
            count2 = random_integer_below(7) != 0 ? 1 : random_integer_below(5) + 5;
            for (k2 = 0; k2 < count2; k2++) {
                random_vector(&vector_perturbation,
                              0.0f,
                              alternating_smoke_burst_jitter_magnitude_max_gp * frame_time);
                fast_vec_normalize_scale_first(
                    alternating_smoke_burst_velocity_magnitude_scale_gp *
                        (random_float_between(
                             -alternating_smoke_burst_motion_variation_fraction_gp,
                             alternating_smoke_burst_motion_variation_fraction_gp) +
                         1.0f) *
                        frame_time,
                    &smoke_particle_velocity,
                    &moby->rot_row0);
                fast_vec_add(
                    &smoke_particle_velocity, &smoke_particle_velocity, &vector_perturbation);
                fast_vec_normalize_scale_first(
                    alternating_smoke_burst_acceleration_magnitude_scale_gp *
                        (random_float_between(
                             -alternating_smoke_burst_motion_variation_fraction_gp,
                             alternating_smoke_burst_motion_variation_fraction_gp) +
                         1.0f) *
                        frame_time,
                    &acc,
                    &smoke_particle_velocity);
                acc.f[2] -=
                    alternating_smoke_burst_vertical_acceleration_decrement_scale_gp * frame_time;
                smoke_particle_velocity.f[3] =
                    random_float_between(alternating_smoke_burst_velocity_w_random_range_start_gp,
                                         alternating_smoke_burst_velocity_w_random_range_end_gp);
                acc.f[3] = random_float_between(
                    alternating_smoke_burst_acceleration_w_random_range_start_gp,
                    alternating_smoke_burst_acceleration_w_random_range_end_gp);
                color_a2_burst = fast_tween_color(
                    D_L02_0016214C, D_L02_00162150, random_float_between(0.0f, 1.0f));
                color_b2_burst = fast_tween_color(
                    D_L02_00162154, D_L02_00162158, random_float_between(0.0f, 1.0f));
                time_a2_burst = truncate_float_to_s32(
                    scale_time((f32)alternating_smoke_burst_timing_random_span_gp *
                                   random_float_between(0.0f, 1.0f) +
                               1.0f));
                time_b2_burst = truncate_float_to_s32(scale_time(
                    (f32)alternating_smoke_burst_second_timing_base_gp *
                    (random_float_between(-alternating_smoke_burst_timing_variation_fraction_gp,
                                          alternating_smoke_burst_timing_variation_fraction_gp) +
                     1.0f)));
                spawn_spark(&moby->pos,
                            &smoke_particle_velocity,
                            &acc,
                            color_a2_burst,
                            color_b2_burst,
                            time_a2_burst,
                            time_b2_burst,
                            truncate_float_to_s32(scale_time(
                                (f32)alternating_smoke_burst_third_timing_base_gp *
                                (random_float_between(
                                     -alternating_smoke_burst_timing_variation_fraction_gp,
                                     alternating_smoke_burst_timing_variation_fraction_gp) +
                                 1.0f))),
                            D_L02_00162164);
            }
        }
        fast_vec_normalize_scale_first(
            alternating_smoke_puff_velocity_magnitude_scale_gp *
                (random_float_between(-alternating_smoke_puff_motion_variation_fraction_gp,
                                      alternating_smoke_puff_motion_variation_fraction_gp) +
                 1.0f) *
                frame_time,
            &smoke_particle_velocity,
            &moby->rot_row0);
        rise2_puff = alternating_smoke_puff_vertical_acceleration_scale_gp *
                     (random_float_between(-alternating_smoke_puff_motion_variation_fraction_gp,
                                           alternating_smoke_puff_motion_variation_fraction_gp) +
                      1.0f) *
                     frame_time;
        clear_u64_value(&acc);
        acc.f[2] = rise2_puff;
        fast_vec_normalize_scale_first(
            random_float_between(-alternating_smoke_puff_lateral_acceleration_magnitude_bound_gp,
                                 alternating_smoke_puff_lateral_acceleration_magnitude_bound_gp) *
                frame_time,
            &vector_perturbation,
            &moby->rot_row1);
        fast_vec_add(&acc, &acc, &vector_perturbation);
        smoke_particle_velocity.f[3] =
            random_float_between(alternating_smoke_puff_velocity_w_random_range_start_gp,
                                 alternating_smoke_puff_velocity_w_random_range_end_gp);
        acc.f[3] = random_float_between(alternating_smoke_puff_acceleration_w_random_range_start_gp,
                                        alternating_smoke_puff_acceleration_w_random_range_end_gp);
        color_a2_puff =
            fast_tween_color(D_L02_00162078, D_L02_0016207C, random_float_between(0.0f, 1.0f));
        color_b2_puff =
            fast_tween_color(D_L02_00162080, D_L02_00162084, random_float_between(0.0f, 1.0f));
        time_a2_puff = truncate_float_to_s32(scale_time(
            (f32) * (s32 *)(smoke_vent_vars + 0xC8) * random_float_between(0.0f, 1.0f) + 1.0f));
        time_b2_puff = truncate_float_to_s32(
            scale_time((f32)alternating_smoke_puff_second_timing_base_gp *
                       (random_float_between(-alternating_smoke_puff_timing_variation_fraction_gp,
                                             alternating_smoke_puff_timing_variation_fraction_gp) +
                        1.0f)));
        spawn_spark(&moby->pos,
                    &smoke_particle_velocity,
                    &acc,
                    color_a2_puff,
                    color_b2_puff,
                    time_a2_puff,
                    time_b2_puff,
                    truncate_float_to_s32(scale_time(
                        (f32)alternating_smoke_puff_third_timing_base_gp *
                        (random_float_between(-alternating_smoke_puff_timing_variation_fraction_gp,
                                              alternating_smoke_puff_timing_variation_fraction_gp) +
                         1.0f))),
                    -1);
        break;
    case SMOKE_VENT_STATE_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}
