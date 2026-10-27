/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/options.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/gameplay/state/usage_stats.h"

typedef struct {
    unsigned char pad0[0x10];
    int flags;
    float f14;
    unsigned char pad18[4];
    float f1C;
    unsigned char pad20[4];
    float f24;
} S_2e89b0;
extern float D_0013CA40_2e89b0[] __asm__("D_0013CA40") __attribute__((section(".data")));
extern int FUN_001f96f8_2e89b0(int) __asm__("FUN_001f96f8");

void follow_camera_read_stick(char *o, float *a, float *b) __asm__("FUN_L00_002e89b0");
void follow_camera_read_stick(char *camera_object, float *turn_input_out, float *pitch_input_out) {
    S_2e89b0 *turn_pitch_state = (S_2e89b0 *)(*(char **)(camera_object + 0x70) + 0x1A8);
    struct UsageStats *usage_stats_state;

    if (turn_pitch_state->flags & 1) {
        *turn_input_out = 0.0f;
    } else {
        *turn_input_out = D_0013CA40_2e89b0[0];
        if (opt_cam_invert == 0) {
            *turn_input_out = -*turn_input_out;
        }
    }
    if (*turn_input_out == 0.0f) {
        D_L00_00161D8C = 0;
        D_L00_00161D90 = 0;
        if (turn_pitch_state->f1C != 0.0f) {
            *turn_input_out = turn_pitch_state->f1C;
        }
    } else {
        turn_pitch_state->f14 = D_L00_0015EF44;
        usage_stats_state = &usage_stats;
        if (usage_stats_state->stat[8].count < 3) {
            D_L00_00161D8C++;
            if (D_L00_00161D90 == 0 && FUN_001f96f8_2e89b0(0x3C) < D_L00_00161D8C) {
                if (usage_stats_state->stat[8].count <= 0xFFFE) {
                    usage_stats_state->stat[8].count++;
                }
                if (FUN_001f96f8_2e89b0(D_0015EEA4) / 600 > usage_stats_state->stat[8].unk2) {
                    usage_stats_state->stat[8].unk2 = FUN_001f96f8_2e89b0(D_0015EEA4) / 600;
                }
                D_L00_00161D90 = 1;
                usage_stats_state->stat[8].level_mask = usage_stats_state->stat[8].level_mask | (1 << current_level_index) | 0x80000000;
            }
        }
    }
    if (turn_pitch_state->flags & 2) {
        *pitch_input_out = 0.0f;
        *pitch_input_out = turn_pitch_state->f24 / 0.6981317f;
    } else {
        float raw_pitch_input = D_0013CA40_2e89b0[1];
        *pitch_input_out = -raw_pitch_input;
        if (invert52 == 0) {
            *pitch_input_out = raw_pitch_input;
        }
        if (*pitch_input_out == 0.0f) {
            *pitch_input_out = turn_pitch_state->f24 / 0.6981317f;
        }
    }
}
typedef float V_2e8bb8[4] __attribute__((aligned(16)));
extern unsigned char D_L00_00166E10_2e8bb8[] __asm__("D_L00_00166E10")
    __attribute__((section(".data")));
extern float FUN_001ebd78_2e8bb8(void *, float, float, float, float, float) __asm__("FUN_001ebd78");
extern float FUN_001f99c0_2e8bb8(float) __asm__("FUN_001f99c0");
extern void FUN_00214890_2e8bb8(void *, void *, void *, float) __asm__("FUN_00214890");
extern float FUN_001f9ab0_2e8bb8(void *, void *) __asm__("FUN_001f9ab0");
extern float FUN_001f9af0_2e8bb8(void *) __asm__("FUN_001f9af0");
extern void FUN_001f9ad8_2e8bb8(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void FUN_001f9bf8_2e8bb8(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_001f9e90_2e8bb8(float, float) __asm__("FUN_001f9e90");
extern float FUN_001fa580_2e8bb8(float, float) __asm__("FUN_001fa580");
extern float FUN_001fa5c8_2e8bb8(float, float) __asm__("FUN_001fa5c8");

void follow_camera_turn_and_pitch(unsigned char *o, float k, float in) __asm__("FUN_L00_002e8bb8");
#include "rnc/overlay/shared/gameplay/camera/follow_camera_turn_and_pitch_globals.h"
extern float follow_camera_input_stiffness_gp __asm__("D_L00_00161CAC") __attribute__((sda));
extern float follow_camera_input_damping_gp __asm__("D_L00_00161CB0") __attribute__((sda));
void follow_camera_turn_and_pitch(unsigned char *camera_object, float k, float in) {
    V_2e8bb8 axis;
    unsigned char *camera_data;
    unsigned char *base;
    unsigned char *turn_pitch_state;
    unsigned char *turn_pitch_vector;
    float smoothed_turn_input;
    float target_pitch_angle;
    float sel;
    float reference_dot_product;
    float turn_pitch_vector_length;
    float current_pitch_angle;
    float z;

    camera_data = *(unsigned char **)(camera_object + 0x70);
    base = D_L00_00166E10_2e8bb8;
    turn_pitch_state = camera_data + 0x1A8;
    turn_pitch_vector = camera_data + 0x130;
    smoothed_turn_input = FUN_001ebd78_2e8bb8(camera_data + 0x1AC, *(float *)(camera_data + 0x1A8), k, follow_camera_input_stiffness_gp,
                            follow_camera_input_damping_gp, 0.0f);
    *(float *)(camera_data + 0x1A8) = smoothed_turn_input;
    *(float *)(turn_pitch_vector + 0x34) = *(float *)(turn_pitch_state + 0x14) * smoothed_turn_input;
    if (in < -0.3f) {
        in = (in + 0.3f) * 1.4285715f;
    } else if (0.3f < in) {
        in = (in - 0.3f) * 1.4285715f;
    } else {
        in = 0.0f;
    }
    z = 0.0f;
    if (in == z) {
        sel = follow_camera_pitch_input_return_step_limit;
    } else if (!(FUN_001f99c0_2e8bb8(in) < FUN_001f99c0_2e8bb8(*(float *)(turn_pitch_state + 8)))) {
        sel = follow_camera_pitch_input_tracking_step_limit;
    } else if (z < in && z < *(float *)(turn_pitch_state + 8)) {
        sel = follow_camera_pitch_input_return_step_limit;
    } else if (in < z && *(float *)(turn_pitch_state + 8) < z) {
        sel = follow_camera_pitch_input_return_step_limit;
    } else {
        sel = follow_camera_pitch_input_tracking_step_limit;
    }
    *(float *)(turn_pitch_state + 8) = FUN_001ebd78_2e8bb8(turn_pitch_state + 0xC, *(float *)(turn_pitch_state + 8), in, follow_camera_input_stiffness_gp,
                                            follow_camera_input_damping_gp, sel);
    target_pitch_angle = *(float *)(turn_pitch_state + 8) * 0.69813168f;
    if (k == 0.0f) {
        FUN_00214890_2e8bb8(turn_pitch_vector, turn_pitch_vector, base + 0x20, *(float *)(turn_pitch_state + 0x20));
    } else {
        FUN_00214890_2e8bb8(turn_pitch_vector, turn_pitch_vector, base + 0x20, *(float *)(turn_pitch_vector + 0x34));
    }
    reference_dot_product = FUN_001f9ab0_2e8bb8(turn_pitch_vector, base + 0x20);
    turn_pitch_vector_length = FUN_001f9af0_2e8bb8(turn_pitch_vector);
    FUN_001f9ad8_2e8bb8(axis, base + 0x20, turn_pitch_vector);
    FUN_001f9bf8_2e8bb8(axis, axis, 1.0f);
    current_pitch_angle = FUN_001f9e90_2e8bb8(turn_pitch_vector_length, reference_dot_product);
    if (FUN_001f99c0_2e8bb8(current_pitch_angle) < FUN_001f99c0_2e8bb8(target_pitch_angle) && sel == follow_camera_pitch_input_return_step_limit) {
        *(float *)(turn_pitch_state + 8) = current_pitch_angle / 0.69813168f;
    }
    if (current_pitch_angle < target_pitch_angle) {
        if (target_pitch_angle < FUN_001fa580_2e8bb8(current_pitch_angle, follow_camera_pitch_angle_step_limit)) {
            *(float *)(turn_pitch_vector + 0x38) = FUN_001fa5c8_2e8bb8(target_pitch_angle, current_pitch_angle);
        } else {
            *(float *)(turn_pitch_vector + 0x38) = follow_camera_pitch_angle_step_limit;
        }
    } else if (target_pitch_angle < current_pitch_angle) {
        if (*(short *)(*(unsigned char **)(camera_object + 0x70) + 0x208) < 0) {
            *(float *)(turn_pitch_vector + 0x38) = 0.0f;
        } else if (FUN_001fa580_2e8bb8(current_pitch_angle, -follow_camera_pitch_angle_step_limit) < target_pitch_angle) {
            *(float *)(turn_pitch_vector + 0x38) = FUN_001fa5c8_2e8bb8(target_pitch_angle, current_pitch_angle);
        } else {
            *(float *)(turn_pitch_vector + 0x38) = -follow_camera_pitch_angle_step_limit;
        }
    } else {
        *(float *)(turn_pitch_vector + 0x38) = 0.0f;
    }
    if (target_pitch_angle < 0.0f) {
        *(short *)(*(unsigned char **)(camera_object + 0x70) + 0x22) = 1;
    }
    FUN_00214890_2e8bb8(turn_pitch_vector, turn_pitch_vector, axis, *(float *)(turn_pitch_vector + 0x38));
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002EA3F0), where it is exact; names translated to the US level program. */

extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void subtract_vector_xyz(float *, float *, float *) __asm__("FUN_001f9a28");
extern void lerp_vector(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void follow_camera_auto_turn(int m) __asm__("FUN_L00_002e8f40");
void follow_camera_auto_turn(int camera_object) {
    char *camera_data = *(char **)(camera_object + 0x70);
    char *auto_turn_length_state = camera_data + 0x1D0;
    char *auto_turn_vector = camera_data + 0x130;
    if (*(short *)(camera_data + 0x10) > 0) {
        float position_difference[4];
        float vector_length;
        float maximum_length;
        subtract_vector_xyz(position_difference, (float *)camera_data, (float *)(camera_data + 0x90));
        lerp_vector(D_L00_0015EF40, auto_turn_vector, auto_turn_vector, position_difference);
        vector_length = fast_vec_length(auto_turn_vector);
        maximum_length = *(float *)(auto_turn_vector + 0x2C);
        if (maximum_length < vector_length) {
            vector_length = maximum_length;
            fast_vec_normalize(auto_turn_vector, auto_turn_vector, vector_length);
            *(float *)(auto_turn_length_state + 0x30) = *(float *)(auto_turn_vector + 0x2C) - vector_length;
        } else {
            float minimum_length = maximum_length - *(float *)(auto_turn_length_state + 0x30);
            if (vector_length < minimum_length) {
                vector_length = minimum_length;
                fast_vec_normalize(auto_turn_vector, auto_turn_vector, vector_length);
            }
            *(float *)(auto_turn_length_state + 0x30) = *(float *)(auto_turn_vector + 0x2C) - vector_length;
        }
    }
}
