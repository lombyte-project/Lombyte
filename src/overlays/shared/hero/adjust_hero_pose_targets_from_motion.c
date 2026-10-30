/* FUN_L00_00215340 split out of 00214658.c so it builds with the SN
   compiler on its own. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/hero/set_second_hero_easing_parameters.h"

extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
extern void            set_first_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a1e0");
extern void            set_third_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a210");

void adjust_hero_pose_targets_from_motion(void) __asm__("FUN_L00_00215340");

void adjust_hero_pose_targets_from_motion(void) {
    f32 run_step_pose_input, nv, k, run_pose_magnitude, motion_correction_scale,
        relative_displacement_angle;
    f32 run_pose_weight, nw;
    f32 scaled_pose_input, unclamped_opposing_pose_target, unclamped_amplified_pose_target,
        unclamped_pose_magnitude;
    f32 x;

    if (hero.state.current == HERO_STATE_RUN) {
        set_first_hero_easing_parameters(0.04f, 0.2f);
        set_second_hero_easing_parameters(0.04f, 0.2f);
        set_third_hero_easing_parameters(0.04f, 0.2f);
        set_fourth_hero_easing_parameters(0.02f, 0.2f);
        if (hero.state.step == 1) {
            run_step_pose_input = hero.motion.yaw_error;
            if (1.4f < run_step_pose_input)
                run_step_pose_input = 1.4f;
            else if (run_step_pose_input < -1.4f)
                run_step_pose_input = -1.4f;
            nv = -run_step_pose_input;
            hero_eases[0].rot_target.f[0] = nv * 0.14f;
            k = hero.motion.flat_speed / (frame_time * 5.7f);
            if (1.0f < k)
                k = 1.0f;
            if (k < 0.0f)
                k = 0.0f;
            hero_eases[0].rot_target.f[0] = nv * 0.14f * k;
            hero_eases[1].rot_target.f[2] = run_step_pose_input * 0.16f;
            hero_eases[2].rot_target.f[0] = run_step_pose_input * -0.12f;
            hero_eases[3].rot_target.f[2] = run_step_pose_input * 0.65f;
            hero_eases[3].rot_target.f[1] = absolute_float(run_step_pose_input) * -0.15f;
            hero_eases[3].rot_target.f[0] = absolute_float(run_step_pose_input) * 0.16f;
            hero_eases[13].rot_target.f[2] = nv * 0.35f;
            hero_eases[14].rot_target.f[2] = nv * 0.3f;
            hero_eases[15].rot_target.f[2] = nv * 0.3f;
            hero_eases[16].rot_target.f[2] = nv * 0.3f;
            hero_eases[13].rot_target.f[1] = absolute_float(run_step_pose_input) * 0.35f;
            hero_eases[14].rot_target.f[1] = absolute_float(run_step_pose_input) * 0.2f;
        } else {
            run_pose_weight = hero.motion.yaw_error * 0.5f;
            if (0.8f < run_pose_weight)
                run_pose_weight = 0.8f;
            else if (run_pose_weight < -0.8f)
                run_pose_weight = -0.8f;
            run_pose_weight = run_pose_weight * absolute_float(run_pose_weight) * 1.25f;
            hero_eases[1].rot_target.f[2] = run_pose_weight;
            hero_eases[3].rot_target.f[2] = run_pose_weight;
            run_pose_magnitude = absolute_float(run_pose_weight);
            nw = -run_pose_weight;
            hero_eases[3].rot_target.f[1] = run_pose_magnitude * 0.25f;
            hero_eases[13].rot_target.f[2] = nw * 0.35f;
            hero_eases[14].rot_target.f[2] = nw * 0.3f;
            hero_eases[15].rot_target.f[2] = nw * 0.3f;
            hero_eases[16].rot_target.f[2] = nw * 0.3f;
        }
    } else if (hero.state.current == HERO_STATE_LAND || hero.state.current == 0x81) {
        set_first_hero_easing_parameters(0.008f, 0.08f);
        set_second_hero_easing_parameters(0.015f, 0.08f);
        set_third_hero_easing_parameters(0.015f, 0.08f);
        set_fourth_hero_easing_parameters(0.008f, 0.1f);
        scaled_pose_input = hero.motion.yaw_error * 1.6f;
        if (1.25f < scaled_pose_input)
            scaled_pose_input = 1.25f;
        else if (scaled_pose_input < -1.25f)
            scaled_pose_input = -1.25f;
        if (hero.state.current == 0x81) {
            scaled_pose_input = hero.motion.yaw_error * 0.8f;
            if (1.1f < scaled_pose_input)
                scaled_pose_input = 1.1f;
            else if (scaled_pose_input < -1.1f)
                scaled_pose_input = -1.1f;
        }
        unclamped_opposing_pose_target = -scaled_pose_input * 0.52f;
        hero_eases[0].rot_target.f[0] = unclamped_opposing_pose_target;
        if (0.28f < unclamped_opposing_pose_target)
            hero_eases[0].rot_target.f[0] = 0.28f;
        else if (unclamped_opposing_pose_target < -0.28f)
            hero_eases[0].rot_target.f[0] = -0.28f;
        unclamped_amplified_pose_target = scaled_pose_input * 1.2f;
        hero_eases[3].rot_target.f[2] = unclamped_amplified_pose_target;
        hero_eases[1].rot_target.f[2] = scaled_pose_input * 0.58f;
        hero_eases[2].rot_target.f[0] = scaled_pose_input * -0.28f;
        if (0.6f < unclamped_amplified_pose_target)
            hero_eases[3].rot_target.f[2] = 0.6f;
        else if (unclamped_amplified_pose_target < -0.6f)
            hero_eases[3].rot_target.f[2] = -0.6f;
        unclamped_pose_magnitude = absolute_float(scaled_pose_input) * 0.2f;
        hero_eases[3].rot_target.f[1] = unclamped_pose_magnitude;
        if (0.2f < unclamped_pose_magnitude)
            hero_eases[3].rot_target.f[1] = 0.2f;
        else if (unclamped_pose_magnitude < -0.2f)
            hero_eases[3].rot_target.f[1] = -0.2f;
        hero_eases[3].rot_target.f[0] = absolute_float(scaled_pose_input) * 0.16f;
        if (hero.state.current == 0x81) {
            if (frame_time * 0.5f < hero.motion.flat_speed) {
                relative_displacement_angle = fast_subtract_rotations(
                    fast_arc_tan(hero.motion.displacement.f[0], hero.motion.displacement.f[1]),
                    hero.motion.rot.f[2]);
                motion_correction_scale = hero.motion.flat_speed * 3.0f;
                hero_eases[0].rot_target.f[0] =
                    -fast_sin(relative_displacement_angle) * motion_correction_scale;
                hero_eases[0].rot_target.f[1] =
                    fast_cos(relative_displacement_angle) * motion_correction_scale;
                if (0.43633232f < hero_eases[0].rot_target.f[0])
                    hero_eases[0].rot_target.f[0] = 0.43633232f;
                else if (hero_eases[0].rot_target.f[0] < -0.43633232f)
                    hero_eases[0].rot_target.f[0] = -0.43633232f;
                if (0.43633232f < hero_eases[0].rot_target.f[1])
                    hero_eases[0].rot_target.f[1] = 0.43633232f;
                else if (hero_eases[0].rot_target.f[1] < -0.43633232f)
                    hero_eases[0].rot_target.f[1] = -0.43633232f;
            }
            hero_eases[0].rot_target.f[1] =
                fast_add_rotations(hero_eases[0].rot_target.f[1], -0.12217305f);
        }
        hero_eases[13].rot_target.f[2] = -scaled_pose_input * 0.35f;
        hero_eases[14].rot_target.f[2] = -scaled_pose_input * 0.3f;
        hero_eases[15].rot_target.f[2] = -scaled_pose_input * 0.3f;
        hero_eases[16].rot_target.f[2] = -scaled_pose_input * 0.3f;
    } else if (hero.state.control_mode == 2 || hero.state.control_mode == 4) {
        set_first_hero_easing_parameters(0.04f, 0.2f);
        set_second_hero_easing_parameters(0.04f, 0.2f);
        set_third_hero_easing_parameters(0.04f, 0.2f);
        set_fourth_hero_easing_parameters(0.02f, 0.2f);
        x = hero.motion.yaw_error;
        if (1.4f < x)
            x = 1.4f;
        else if (x < -1.4f)
            x = -1.4f;
        hero_eases[1].rot_target.f[2] = x * 0.29f;
        hero_eases[2].rot_target.f[0] = x * -0.27f;
        hero_eases[3].rot_target.f[2] = x * 0.87f;
        hero_eases[13].rot_target.f[2] = -x * 0.35f;
        hero_eases[14].rot_target.f[2] = -x * 0.3f;
        hero_eases[15].rot_target.f[2] = -x * 0.3f;
        hero_eases[16].rot_target.f[2] = -x * 0.3f;
    }
}
