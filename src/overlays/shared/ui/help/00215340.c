/* FUN_L00_00215340 split out of 00214658.c so it builds with the SN
   compiler on its own. Hero pose table offsets (D_L00_0017A680) are kept
   as byte offsets. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
typedef struct { f32 f[0x300]; } T_215340;
extern T_215340 D_L00_0017A680;
extern void set_first_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a1e0");
extern void FUN_L00_0020a1f8(f32, f32);
extern void set_third_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a210");
extern void set_fourth_hero_easing_parameters(f32, f32) __asm__("FUN_L00_0020a228");
extern f32 absolute_float(f32) __asm__("FUN_001f99c0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");

#define TF(o) (D_L00_0017A680.f[(o) / 4])

void adjust_hero_pose_targets_from_motion(void) __asm__("FUN_L00_00215340");

void adjust_hero_pose_targets_from_motion(void) {
    f32 run_step_pose_input, nv, k, run_pose_magnitude, motion_correction_scale, relative_displacement_angle;
    f32 run_pose_weight, nw;
    f32 scaled_pose_input, unclamped_opposing_pose_target, unclamped_amplified_pose_target, unclamped_pose_magnitude;
    f32 x;

    if (hero.state.current == HERO_STATE_RUN) {
        set_first_hero_easing_parameters(0.04f, 0.2f);
        FUN_L00_0020a1f8(0.04f, 0.2f);
        set_third_hero_easing_parameters(0.04f, 0.2f);
        set_fourth_hero_easing_parameters(0.02f, 0.2f);
        if (hero.state.step == 1) {
            run_step_pose_input = hero.motion.yaw_error;
            if (1.4f < run_step_pose_input) run_step_pose_input = 1.4f;
            else if (run_step_pose_input < -1.4f) run_step_pose_input = -1.4f;
            nv = -run_step_pose_input;
            TF(0x60) = nv * 0.14f;
            k = hero.motion.flat_speed / (frame_time * 5.7f);
            if (1.0f < k) k = 1.0f;
            if (k < 0.0f) k = 0.0f;
            TF(0x60) = nv * 0.14f * k;
            TF(0x118) = run_step_pose_input * 0.16f;
            TF(0x1C0) = run_step_pose_input * -0.12f;
            TF(0x278) = run_step_pose_input * 0.65f;
            TF(0x274) = absolute_float(run_step_pose_input) * -0.15f;
            TF(0x270) = absolute_float(run_step_pose_input) * 0.16f;
            TF(0x958) = nv * 0.35f;
            TF(0xA08) = nv * 0.3f;
            TF(0xAB8) = nv * 0.3f;
            TF(0xB68) = nv * 0.3f;
            TF(0x954) = absolute_float(run_step_pose_input) * 0.35f;
            TF(0xA04) = absolute_float(run_step_pose_input) * 0.2f;
        } else {
            run_pose_weight = hero.motion.yaw_error * 0.5f;
            if (0.8f < run_pose_weight) run_pose_weight = 0.8f;
            else if (run_pose_weight < -0.8f) run_pose_weight = -0.8f;
            run_pose_weight = run_pose_weight * absolute_float(run_pose_weight) * 1.25f;
            TF(0x118) = run_pose_weight;
            TF(0x278) = run_pose_weight;
            run_pose_magnitude = absolute_float(run_pose_weight);
            nw = -run_pose_weight;
            TF(0x274) = run_pose_magnitude * 0.25f;
            TF(0x958) = nw * 0.35f;
            TF(0xA08) = nw * 0.3f;
            TF(0xAB8) = nw * 0.3f;
            TF(0xB68) = nw * 0.3f;
        }
    } else if (hero.state.current == HERO_STATE_LAND || hero.state.current == 0x81) {
        set_first_hero_easing_parameters(0.008f, 0.08f);
        FUN_L00_0020a1f8(0.015f, 0.08f);
        set_third_hero_easing_parameters(0.015f, 0.08f);
        set_fourth_hero_easing_parameters(0.008f, 0.1f);
        scaled_pose_input = hero.motion.yaw_error * 1.6f;
        if (1.25f < scaled_pose_input) scaled_pose_input = 1.25f;
        else if (scaled_pose_input < -1.25f) scaled_pose_input = -1.25f;
        if (hero.state.current == 0x81) {
            scaled_pose_input = hero.motion.yaw_error * 0.8f;
            if (1.1f < scaled_pose_input) scaled_pose_input = 1.1f;
            else if (scaled_pose_input < -1.1f) scaled_pose_input = -1.1f;
        }
        unclamped_opposing_pose_target = -scaled_pose_input * 0.52f;
        TF(0x60) = unclamped_opposing_pose_target;
        if (0.28f < unclamped_opposing_pose_target) TF(0x60) = 0.28f;
        else if (unclamped_opposing_pose_target < -0.28f) TF(0x60) = -0.28f;
        unclamped_amplified_pose_target = scaled_pose_input * 1.2f;
        TF(0x278) = unclamped_amplified_pose_target;
        TF(0x118) = scaled_pose_input * 0.58f;
        TF(0x1C0) = scaled_pose_input * -0.28f;
        if (0.6f < unclamped_amplified_pose_target) TF(0x278) = 0.6f;
        else if (unclamped_amplified_pose_target < -0.6f) TF(0x278) = -0.6f;
        unclamped_pose_magnitude = absolute_float(scaled_pose_input) * 0.2f;
        TF(0x274) = unclamped_pose_magnitude;
        if (0.2f < unclamped_pose_magnitude) TF(0x274) = 0.2f;
        else if (unclamped_pose_magnitude < -0.2f) TF(0x274) = -0.2f;
        TF(0x270) = absolute_float(scaled_pose_input) * 0.16f;
        if (hero.state.current == 0x81) {
            if (frame_time * 0.5f < hero.motion.flat_speed) {
                relative_displacement_angle = fast_subtract_rotations(fast_arc_tan(hero.motion.displacement.f[0], hero.motion.displacement.f[1]), hero.motion.rot.f[2]);
                motion_correction_scale = hero.motion.flat_speed * 3.0f;
                TF(0x60) = -fast_sin(relative_displacement_angle) * motion_correction_scale;
                TF(0x64) = fast_cos(relative_displacement_angle) * motion_correction_scale;
                if (0.43633232f < TF(0x60)) TF(0x60) = 0.43633232f;
                else if (TF(0x60) < -0.43633232f) TF(0x60) = -0.43633232f;
                if (0.43633232f < TF(0x64)) TF(0x64) = 0.43633232f;
                else if (TF(0x64) < -0.43633232f) TF(0x64) = -0.43633232f;
            }
            TF(0x64) = fast_add_rotations(TF(0x64), -0.12217305f);
        }
        TF(0x958) = -scaled_pose_input * 0.35f;
        TF(0xA08) = -scaled_pose_input * 0.3f;
        TF(0xAB8) = -scaled_pose_input * 0.3f;
        TF(0xB68) = -scaled_pose_input * 0.3f;
    } else if (hero.state.control_mode == 2 || hero.state.control_mode == 4) {
        set_first_hero_easing_parameters(0.04f, 0.2f);
        FUN_L00_0020a1f8(0.04f, 0.2f);
        set_third_hero_easing_parameters(0.04f, 0.2f);
        set_fourth_hero_easing_parameters(0.02f, 0.2f);
        x = hero.motion.yaw_error;
        if (1.4f < x) x = 1.4f;
        else if (x < -1.4f) x = -1.4f;
        TF(0x118) = x * 0.29f;
        TF(0x1C0) = x * -0.27f;
        TF(0x278) = x * 0.87f;
        TF(0x958) = -x * 0.35f;
        TF(0xA08) = -x * 0.3f;
        TF(0xAB8) = -x * 0.3f;
        TF(0xB68) = -x * 0.3f;
    }
}
