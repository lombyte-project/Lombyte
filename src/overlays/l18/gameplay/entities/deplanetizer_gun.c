/* Level 18 moby classes: deplanetizer_gun, gadgebot. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"

#include "rnc/math/vector.h"
#include "qcopy.h"
typedef struct { char pad[0x30]; int state; int n; char pad38[0x140]; struct Moby *objects[8]; } L18SequenceState;
extern L18SequenceState D_L18_0016D260_update __asm__("D_L18_0016D260");
extern int D_L18_0015F5C4;
extern int frames_9780(int) __asm__("FUN_001f96f8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void rot_9780(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_9780(void *, void *, void *) __asm__("FUN_001f9a10");
extern void norm_9780(void *, void *, float) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void draw_trail(void *, void *, void *, float, float) __asm__("FUN_L00_00263b70");
extern void enqueue_callback_list_1_overlay(void *, void *) __asm__("FUN_L18_002020c8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
void spawn_deplanetizer_gun_trail_effects(void *first_position_source, void *second_position_source, void *effect_velocity_source, float f0, float f1) __asm__("FUN_L18_002fa250");

extern void spawn_deplanetizer_gun_trail_effects(void *, void *, void *, float, float) __asm__("FUN_L18_002fa250");
void draw_three_scaled_camera_facing_quads(void) __asm__("FUN_L18_002f9780");

extern void draw_three_scaled_camera_facing_quads(void) __asm__("FUN_L18_002f9780");
void draw_moby_color_cycling_bone_glows(void *moby) __asm__("FUN_L18_002f9a98");

extern void draw_moby_color_cycling_bone_glows(void *) __asm__("FUN_L18_002f9a98");
void draw_moby_two_tint_glow_quads(struct Moby *m) __asm__("FUN_L18_002f9c20");

extern void draw_moby_two_tint_glow_quads(struct Moby *) __asm__("FUN_L18_002f9c20");
void draw_moby_three_scaled_two_tint_quads(struct Moby *m) __asm__("FUN_L18_002f9eb8");

extern void draw_moby_three_scaled_two_tint_quads(struct Moby *) __asm__("FUN_L18_002f9eb8");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int,
                               float, float, float, float, int, float, int, int, int, int)
    __asm__("FUN_L00_0025e450");
extern char *func_L00_0026DEA0_c(void *, int, void *, float, float, float, float, int)
    __asm__("FUN_L00_0026d000");
extern struct Moby *D_L18_0015FFE4;
extern struct Moby *D_L18_00162528;
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_globals.h"
extern float deplanetizer_camera_quad_scale_ramp_start_value_gp __asm__("D_L18_00162490") __attribute__((sda));
extern float deplanetizer_camera_quad_scale_ramp_end_value_gp __asm__("D_L18_00162494") __attribute__((sda));
extern int deplanetizer_camera_quad_scale_ramp_end_frame_gp __asm__("D_L18_0016249C") __attribute__((sda));
extern int deplanetizer_camera_quad_end_frame_gp __asm__("D_L18_001624A0") __attribute__((sda));
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_data/update_deplanetizer_gun_sequence_effects_3.h"
extern int deplanetizer_glow_alpha_ramp_end_frame_gp __asm__("D_L18_001624C0") __attribute__((sda));
extern int deplanetizer_three_quad_scale_ramp_start_frame_gp __asm__("D_L18_001624C4") __attribute__((sda));
extern int deplanetizer_three_quad_scale_ramp_end_frame_gp __asm__("D_L18_001624C8") __attribute__((sda));
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_data/update_deplanetizer_gun_sequence_effects_2.h"
/* First component of the vector passed at gp - 0x4720. */
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_data/update_deplanetizer_gun_sequence_effects.h"
extern int D_L18_001617F4, D_L18_001617F8;
extern float D_L18_0015ED60;

extern Vec4 D_L18_00162530 __attribute__((section(".sdata")));
extern Vec4 D_L18_00162540 __attribute__((section(".sdata")));
typedef struct { float radius[5]; } EffectRadii;
extern EffectRadii D_L18_001F2C20;
extern char D_L18_0015F580[] __attribute__((section(".sdata")));
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void rotate_vector_about_axis(void *, void *, void *, float) __asm__("FUN_00214890");
extern void update_beam_ring_rail_and_pulse_effects(void *) __asm__("FUN_L00_002bd0a0");

extern int deplanetizer_camera_quad_start_frame_gp __asm__("D_L18_00162498") __attribute__((sda));
extern int deplanetizer_glow_alpha_ramp_start_frame_gp __asm__("D_L18_001624BC") __attribute__((sda));
extern char D_L18_001679D0[];
/* Preserve the state-4 frame conversion and the state-7 raw-frame arithmetic. */
void update_deplanetizer_gun_sequence_effects(struct Moby *moby) __asm__("FUN_L18_002f88e8");

void update_deplanetizer_gun_sequence_effects(struct Moby *moby) {
    enum {
        INITIALIZE_EFFECT_TIMING = 0,
        UPDATE_SEQUENCE_EFFECTS = 1
    };
    union { EffectRadii radii; struct { Vec4 pos, velocity; } vectors; } scratch;
    Vec4 direction, interpolated_joint_zero_position, interpolated_joint_two_position;
    struct Moby *target;
    float radius, numerator, denominator, fraction, saved;
    float *data;
    unsigned char *effect;
    int i, n;

    switch (moby->state) {
    case INITIALIZE_EFFECT_TIMING:
        deplanetizer_camera_quad_start_frame_gp = 355;
        deplanetizer_camera_quad_scale_ramp_end_frame_gp = 365;
        deplanetizer_glow_alpha_ramp_start_frame_gp = 60;
        deplanetizer_glow_alpha_ramp_end_frame_gp = 200;
        deplanetizer_three_quad_scale_ramp_start_frame_gp = 190;
        deplanetizer_three_quad_scale_ramp_end_frame_gp = 365;
        moby->update_dist = 255;
        moby->state = 1;
        return;
    case UPDATE_SEQUENCE_EFFECTS:
        break;
    default:
        return;
    }
    if (D_L18_0015F5C4 != 2)
    return;
    if (D_L18_0016D260_update.state == 4) {
        if (D_L18_0016D260_update.n == 1) {
            {
                struct Moby *target;
                for (target = D_L18_0015FFE4; target; target = target->next) {
                    if (target->oclass == 0x59B) {
                        D_L18_00162528 = target;
                        break;
                    }
                }
            }
        }
        if (frames_9780(500) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n <= frames_9780(860)) {
            int i;
            scratch.radii = D_L18_001F2C20;
            for (i = 3; i < 8; i++) {
                if (D_L18_0016D260_update.objects[i]) {
                    moby_point_position(D_L18_0016D260_update.objects[i], 0, &direction);
                    radius = scratch.radii.radius[i - 3];
                    if (i != 3 || D_L18_0016D260_update.n < frames_9780(645) ||
                        frames_9780(736) < D_L18_0016D260_update.n) {
                        if (i == 5 && frames_9780(645) <= D_L18_0016D260_update.n &&
                            D_L18_0016D260_update.n <= frames_9780(730))
                        radius = 300000.0f;
                        draw_trail(&direction, &direction, D_L18_0015F580, radius, radius * 0.4f);
                    }
                }
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(706)) {
            struct Moby *target = D_L18_0016D260_update.objects[7];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(710)) {
            struct Moby *target = D_L18_0016D260_update.objects[4];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(720)) {
            struct Moby *target = D_L18_0016D260_update.objects[6];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(730)) {
            struct Moby *target = D_L18_0016D260_update.objects[5];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 50, 25, 25,
                    8.0f, 4.0f, 9.0f, 1.0f, -1, 30.0f, 0, 15, -1, 0);
            }
        }
        if (D_L18_0016D260_update.n == frames_9780(748)) {
            scratch.vectors.pos.q = 0;
            scratch.vectors.pos.f[0] = 634.556030273f;
            scratch.vectors.pos.f[1] = 454.162994385f;
            scratch.vectors.pos.f[2] = 102.999000549f;
            scratch.vectors.pos.f[3] = 1.0f;
            clear_u64_value(&scratch.vectors.velocity);
            scratch.vectors.velocity.f[2] = 1.0f;
            func_L00_0025F4A8_alt(moby, &scratch.vectors.velocity, &scratch.vectors.pos, 0.0f, 0.0f, 300, 200, 200,
                12.0f, 6.0f, 9.0f, 1.0f, -1, 60.0f, 0, 60, -1, 0);
        }
        if (frames_9780(deplanetizer_glow_alpha_ramp_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(deplanetizer_glow_alpha_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - frames_9780(deplanetizer_glow_alpha_ramp_start_frame_gp));
            denominator = convert_integer_to_float(frames_9780(deplanetizer_glow_alpha_ramp_end_frame_gp - deplanetizer_glow_alpha_ramp_start_frame_gp));
            numerator = numerator / denominator;
            deplanetizer_glow_quad_alpha = truncate_float_to_s32(numerator * 255.0f);
            enqueue_callback_list_1_overlay(draw_moby_two_tint_glow_quads, D_L18_00162528);
        }
        if (frames_9780(deplanetizer_three_quad_scale_ramp_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(deplanetizer_three_quad_scale_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - frames_9780(deplanetizer_three_quad_scale_ramp_start_frame_gp));
            denominator = convert_integer_to_float(frames_9780(deplanetizer_three_quad_scale_ramp_end_frame_gp - deplanetizer_three_quad_scale_ramp_start_frame_gp));
            numerator = numerator / denominator;
            numerator *= 200.0f;
            numerator += 0.25f;
            deplanetizer_three_quad_local_x_scale = numerator;
            enqueue_callback_list_1_overlay(draw_moby_three_scaled_two_tint_quads, D_L18_00162528);
        }
        if (frames_9780(deplanetizer_camera_quad_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(deplanetizer_camera_quad_end_frame_gp)) {
            enqueue_callback_list_1_overlay(draw_three_scaled_camera_facing_quads, D_L18_0016D260_update.objects[0]);
            if (D_L18_0016D260_update.n >= frames_9780(deplanetizer_camera_quad_scale_ramp_end_frame_gp)) {
                deplanetizer_camera_quad_base_scale = 10.0f;
            } else {
                numerator = convert_integer_to_float(D_L18_0016D260_update.n - frames_9780(deplanetizer_camera_quad_start_frame_gp));
                denominator = convert_integer_to_float(frames_9780(deplanetizer_camera_quad_scale_ramp_end_frame_gp - deplanetizer_camera_quad_start_frame_gp));
                numerator = numerator / denominator;
                deplanetizer_camera_quad_base_scale = deplanetizer_camera_quad_scale_ramp_start_value_gp + (deplanetizer_camera_quad_scale_ramp_end_value_gp - deplanetizer_camera_quad_scale_ramp_start_value_gp) * numerator;
            }
        }
    }
    if (D_L18_0016D260_update.state == 7) {
        if (D_L18_0016D260_update.n == 1) {
            deplanetizer_glow_alpha_ramp_start_frame_gp = deplanetizer_state_7_glow_alpha_ramp_start_frame_gp;
            deplanetizer_glow_alpha_ramp_end_frame_gp = deplanetizer_state_7_glow_alpha_ramp_end_frame_gp;
            deplanetizer_three_quad_scale_ramp_start_frame_gp = deplanetizer_state_7_three_quad_scale_ramp_start_frame_gp;
            deplanetizer_three_quad_scale_ramp_end_frame_gp = deplanetizer_state_7_three_quad_scale_ramp_end_frame_gp;
            deplanetizer_camera_quad_start_frame_gp = deplanetizer_state_7_camera_quad_start_frame_gp;
            deplanetizer_camera_quad_scale_ramp_end_frame_gp = deplanetizer_state_7_camera_quad_scale_ramp_end_frame_gp;
            {
                struct Moby *target;
                for (target = D_L18_0015FFE4; target; target = target->next) {
                    if (target->oclass == 0x59B) {
                        D_L18_00162528 = target;
                        break;
                    }
                }
            }
        }
        if (frames_9780(deplanetizer_glow_alpha_ramp_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(deplanetizer_glow_alpha_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - deplanetizer_glow_alpha_ramp_start_frame_gp);
            denominator = convert_integer_to_float(deplanetizer_glow_alpha_ramp_end_frame_gp - deplanetizer_glow_alpha_ramp_start_frame_gp);
            numerator = numerator / denominator;
            deplanetizer_glow_quad_alpha = truncate_float_to_s32(numerator * 255.0f);
            enqueue_callback_list_1_overlay(draw_moby_two_tint_glow_quads, D_L18_00162528);
        }
        if (frames_9780(deplanetizer_three_quad_scale_ramp_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(deplanetizer_three_quad_scale_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n - deplanetizer_three_quad_scale_ramp_start_frame_gp);
            denominator = convert_integer_to_float(deplanetizer_three_quad_scale_ramp_end_frame_gp - deplanetizer_three_quad_scale_ramp_start_frame_gp);
            numerator = numerator / denominator;
            numerator *= 200.0f;
            numerator += 0.25f;
            deplanetizer_three_quad_local_x_scale = numerator;
            enqueue_callback_list_1_overlay(draw_moby_three_scaled_two_tint_quads, D_L18_00162528);
        }
        if (frames_9780(deplanetizer_camera_quad_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < frames_9780(deplanetizer_camera_quad_end_frame_gp)) {
            enqueue_callback_list_1_overlay(draw_three_scaled_camera_facing_quads, D_L18_0016D260_update.objects[0]);
            if (D_L18_0016D260_update.n >= deplanetizer_camera_quad_scale_ramp_end_frame_gp) {
                deplanetizer_camera_quad_base_scale = 10.0f;
            } else {
                numerator = convert_integer_to_float(D_L18_0016D260_update.n - deplanetizer_camera_quad_start_frame_gp);
                denominator = convert_integer_to_float(deplanetizer_camera_quad_scale_ramp_end_frame_gp - deplanetizer_camera_quad_start_frame_gp);
                numerator = numerator / denominator;
                deplanetizer_camera_quad_base_scale = deplanetizer_camera_quad_scale_ramp_start_value_gp + (deplanetizer_camera_quad_scale_ramp_end_value_gp - deplanetizer_camera_quad_scale_ramp_start_value_gp) * numerator;
            }
        }
    }
    if (D_L18_0016D260_update.state == 1 && D_L18_0016D260_update.objects[3])
    enqueue_callback_list_1_overlay(draw_moby_color_cycling_bone_glows, D_L18_0016D260_update.objects[3]);
    if (D_L18_0016D260_update.state == 5 && frames_9780(138) <= D_L18_0016D260_update.n &&
        D_L18_0016D260_update.n <= frames_9780(264)) {
        struct Moby *target = D_L18_0016D260_update.objects[2];
        if (target) {
            if (frames_9780(140) < D_L18_0016D260_update.n) {
                int i;
                moby_point_position(target, 0, &scratch.vectors.pos);
                moby_point_position(target, 2, &scratch.vectors.velocity);
                for (i = 0; i < 12; i++) {
                    fraction = convert_integer_to_float(i) / 12.0f;
                    interpolate_vector(&interpolated_joint_zero_position, &scratch.vectors.pos, &D_L18_00162530, fraction);
                    interpolate_vector(&interpolated_joint_two_position, &scratch.vectors.velocity, &D_L18_00162540, fraction);
                    spawn_deplanetizer_gun_trail_effects(&interpolated_joint_zero_position, &interpolated_joint_zero_position, D_L18_0015F580, deplanetizer_joint_trail_base_size_gp, deplanetizer_joint_trail_base_size_gp * 0.4f);
                    spawn_deplanetizer_gun_trail_effects(&interpolated_joint_two_position, &interpolated_joint_two_position, D_L18_0015F580, deplanetizer_joint_trail_base_size_gp, deplanetizer_joint_trail_base_size_gp * 0.4f);
                }
                qcopy(&D_L18_00162530, &scratch.vectors.pos);
                qcopy(&D_L18_00162540, &scratch.vectors.velocity);
            } else {
                moby_point_position(target, 0, &D_L18_00162530);
                moby_point_position(target, 2, &D_L18_00162540);
            }
        }
    }
    if (D_L18_0016D260_update.state == 1 && frames_9780(446) <= D_L18_0016D260_update.n &&
        D_L18_0016D260_update.n < frames_9780(502)) {
        int color, color2;
        struct Moby *target = D_L18_0016D260_update.objects[4];
        if (target) {
            data = (float *)moby->pvars;
            data[4] = 0.0f;
            moby_point_position(target, 3, &scratch.vectors.pos);
            moby_point_position(target, 10, &scratch.vectors.velocity);
            subtract_vector_xyz(&direction, &scratch.vectors.velocity, &scratch.vectors.pos);
            norm_9780(&moby->rot_row1, &direction, 1.0f);
            qcopy(data, &scratch.vectors.velocity);
            color = 0x207F7F;
            color2 = 0x7F7F20;
            data[18] = fast_arc_tan(moby->rot_row1.x, moby->rot_row1.y);
            data[19] = fast_arc_tan(vector_length_xy(&moby->rot_row1), moby->rot_row1.z);
            saved = beam_effect_default_length;
            beam_effect_default_length = 24.0f;
            update_beam_ring_rail_and_pulse_effects(moby);
            beam_effect_default_length = saved;
            D_L18_001617F4 = color;
            D_L18_001617F8 = color2;
        }
    }
    if (D_L18_0016D260_update.state != 2 && D_L18_0016D260_update.state != 0) {
        if (D_L18_0016D260_update.state != 3)
        return;
        if (frames_9780(586) <= D_L18_0016D260_update.n)
        goto burst;
    }
    if (D_L18_0016D260_update.objects[2])
    enqueue_callback_list_1_overlay(draw_moby_color_cycling_bone_glows, D_L18_0016D260_update.objects[2]);
burst:
    if (D_L18_0016D260_update.state == 3 && D_L18_0016D260_update.n == frames_9780(586)) {
        char *matrix = D_L18_001679D0;
        rot_9780(&scratch.vectors.pos, &deplanetizer_burst_local_spawn_offset_x_gp, matrix);
        add_9780(&scratch.vectors.pos, &scratch.vectors.pos, matrix - 0x210);
        qcopy(&scratch.vectors.velocity, matrix + 0x10);
        for (i = 0; i < 32; i++) {
            rotate_vector_about_axis(&direction, &scratch.vectors.velocity, D_L18_001679D0,
                convert_integer_to_float(i) * 0.19634955f);
            norm_9780(&direction, &direction, deplanetizer_burst_velocity_scale_gp * D_L18_0015ED60);
            n = random_integer_below(16);
            if (random_integer_below(2))
            n = -n;
            effect = (unsigned char *)func_L00_0026DEA0_c(&scratch.vectors.pos, n, &direction,
                0.0f, 1.0f, 1.01f, deplanetizer_burst_particle_size_gp, 0x7F7F7F7F);
            if (effect) {
                unsigned char *data = effect + 0x20;
                *(short *)(effect + 10) = frames_9780(60);
                effect[9] = truncate_float_to_s32(4.0f) + 64;
                *(int *)(data + 4) = 2;
                data[10] = 127;
                data[11] = effect[10];
            }
        }
    }
}

typedef unsigned int u128_9780 __attribute__((mode(TI)));
typedef struct {
    float m[4][4];
    int col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_9780;
typedef struct { char pad[0x30]; int state; int n; } ML_9780;
extern ML_9780 D_L18_0016D260_9780 __asm__("D_L18_0016D260");
extern int frames_9780(int) __asm__("FUN_001f96f8");
extern unsigned long tex_9780(int) __asm__("FUN_001f44b8");
extern void gs_9780(int, long) __asm__("FUN_00233980");
extern void rot_9780(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_9780(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_9780(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_9780(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_9780(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void scale_9780(void *, void *, float) __asm__("FUN_001f9a68");
extern void xform_9780(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_9780(void *, void *, int) __asm__("FUN_001f7d30");
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_data/draw_three_scaled_camera_facing_quads_2.h"
extern volatile float D_L18_00162554;
extern volatile float D_L18_00162558;
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_data/draw_three_scaled_camera_facing_quads.h"
extern int deplanetizer_camera_quad_first_color_gp __asm__("D_L18_001624F8") __attribute__((sda));
extern int deplanetizer_camera_quad_smaller_color_gp __asm__("D_L18_001624FC") __attribute__((sda));
float D_L18_001DFB90[4][4] = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};
void draw_three_scaled_camera_facing_quads(void) {
    Pk_9780 pk;
    float quad_transform[4][4];
    float up[4];
    int i;
    int color;
    float tilt;
    char *target;

    if (D_L18_0016D260_9780.n == frames_9780(deplanetizer_camera_quad_start_frame_gp + 1)) {
        deplanetizer_camera_quad_local_offset_x = 1.0f;
        D_L18_00162554 = 0.0f;
        if (D_L18_0016D260_9780.state == 7) {
            tilt = -0.15f;
        } else {
            tilt = 0.15f;
        }
        D_L18_00162558 = tilt;
        deplanetizer_camera_quad_local_offset_w = 1.0f;
    }
    pk.b = tex_9780(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    gs_9780(0x4A, 0);
    gs_9780(0x47, 0x51001);
    rot_9780(quad_transform[3], &deplanetizer_camera_quad_local_offset_x, D_L18_001679D0);
    target = D_L18_001679D0;
    target -= 0x210;
    add_9780(quad_transform[3], quad_transform[3], target);
    sub_9780(quad_transform[0], quad_transform[3], target);
    norm_9780(quad_transform[0], quad_transform[0], 1.0f);
    *(u128_9780 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    cross_9780(quad_transform[1], quad_transform[0], up);
    norm_9780(quad_transform[1], quad_transform[1], 1.0f);
    cross_9780(quad_transform[2], quad_transform[1], quad_transform[0]);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    color = deplanetizer_camera_quad_first_color_gp;
    pk.col[3] = color;
    pk.col[2] = color;
    pk.col[1] = color;
    pk.col[0] = color;
    for (i = 0; i < 4; i++) {
        scale_9780(pk.m[i], D_L18_001DFB90[i], deplanetizer_camera_quad_base_scale);
        xform_9780(pk.m[i], pk.m[i], quad_transform);
    }
    draw_9780(&pk, 0, 0);
    color = deplanetizer_camera_quad_smaller_color_gp;
    pk.col[3] = color;
    pk.col[2] = color;
    pk.col[1] = color;
    pk.col[0] = color;
    for (i = 0; i < 4; i++) {
        scale_9780(pk.m[i], D_L18_001DFB90[i], deplanetizer_camera_quad_base_scale * 0.75f);
        xform_9780(pk.m[i], pk.m[i], quad_transform);
    }
    draw_9780(&pk, 0, 0);
    for (i = 0; i < 4; i++) {
        scale_9780(pk.m[i], D_L18_001DFB90[i], deplanetizer_camera_quad_base_scale * 0.5f);
        xform_9780(pk.m[i], pk.m[i], quad_transform);
    }
    draw_9780(&pk, 0, 0);
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FAEF8), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L18_00162454;
extern float D_L18_00162450;
extern float fast_sin(float) __asm__("func_001F9DE0");
extern int game_frame __asm__("D_L18_0015F5CC");
extern int D_L18_00162458;
extern int D_L18_0016245C;
extern int D_L18_00162460;
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern float D_L18_00162454 ;
extern float D_L18_00162450 ;

extern short D_L18_00162500 __attribute__((sda));
void draw_moby_color_cycling_bone_glows(void *moby) {
    char glow_position[16];
    char bone_transform_rows0_1[32];
    char bone_transform_row2[16];
    char bone_world_position[16];
    char scaled_bone_offset[16];
    int color_cycle_frames = scale_game_frames(D_L18_00162460);
    float color_cycle_value = convert_integer_to_float(game_frame % color_cycle_frames);
    int tweened_color;
    color_cycle_value = color_cycle_value / convert_integer_to_float(color_cycle_frames);
    color_cycle_value = color_cycle_value * 6.18318f;
    color_cycle_value = fast_sin(color_cycle_value - 3.14159f);
    tweened_color = fast_tween_color(D_L18_00162458, D_L18_0016245C, color_cycle_value * 0.5f + 0.5f);
    build_moby_bone_transform(moby, 1, bone_transform_rows0_1);
    scale_vector_xyz(scaled_bone_offset, bone_transform_row2, *(float *)&D_L18_00162500);
    subtract_vector_xyz(glow_position, bone_world_position, scaled_bone_offset);
    draw_offset_textured_effect_quad(glow_position, tweened_color, D_L18_00162450, D_L18_00162454);
    build_moby_bone_transform(moby, 2, bone_transform_rows0_1);
    scale_vector_xyz(scaled_bone_offset, bone_transform_row2, *(float *)&D_L18_00162500);
    subtract_vector_xyz(glow_position, bone_world_position, scaled_bone_offset);
    draw_offset_textured_effect_quad(glow_position, tweened_color, D_L18_00162450, D_L18_00162454);
}
/* Draws the moby's two glow quads facing the camera, in the inner and outer tints. */
typedef unsigned int u128_9c20 __attribute__((mode(TI)));
typedef struct {
    float m[4][4];
    int col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_9c20;
typedef struct {
    char pad[0x34];
    int n;
} ML_9c20;
extern ML_9c20 D_L18_0016D260_9c20 __asm__("D_L18_0016D260");
extern char D_L18_001677C0_9c20[] __asm__("D_L18_001677C0");
extern int frames_9c20(int) __asm__("FUN_001f96f8");
extern unsigned long tex_9c20(int) __asm__("FUN_001f44b8");
extern void gs_9c20(int, long) __asm__("FUN_00233980");
extern void rot_9c20(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_9c20(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_9c20(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_9c20(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_9c20(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void scale_9c20(void *, void *, float) __asm__("FUN_001f9a68");
extern void xform_9c20(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_9c20(void *, void *, int) __asm__("FUN_001f7d30");

extern int deplanetizer_glow_first_quad_base_tint_gp __asm__("D_L18_00162504") __attribute__((sda));
extern float deplanetizer_glow_first_quad_scale_gp __asm__("D_L18_00162508") __attribute__((sda));
extern float deplanetizer_glow_second_quad_scale_gp __asm__("D_L18_0016250C") __attribute__((sda));
extern int deplanetizer_glow_second_quad_base_tint_gp __asm__("D_L18_00162510") __attribute__((sda));
void draw_moby_two_tint_glow_quads(struct Moby *m) {
    Pk_9c20 pk;
    float quad_transform[4][4];
    float up[4];
    int c;
    int second_quad_color;
    int i;
    if (D_L18_0016D260_9c20.n == frames_9c20(deplanetizer_glow_alpha_ramp_start_frame_gp + 1)) {
        deplanetizer_glow_local_offset_z = -45.0f;
        deplanetizer_glow_local_offset_w = 1.0f;
        deplanetizer_glow_local_offset_x = 0;
        deplanetizer_glow_local_offset_y = 0;
    }
    pk.b = tex_9c20(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    gs_9c20(0x4A, 0);
    gs_9c20(0x47, 0x51001);
    rot_9c20(quad_transform[3], &deplanetizer_glow_local_offset_x, &m->rot_row0);
    add_9c20(quad_transform[3], quad_transform[3], &m->pos);
    sub_9c20(quad_transform[0], quad_transform[3], D_L18_001677C0_9c20);
    norm_9c20(quad_transform[0], quad_transform[0], 1.0f);
    *(u128_9c20 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    cross_9c20(quad_transform[1], quad_transform[0], up);
    norm_9c20(quad_transform[1], quad_transform[1], 1.0f);
    cross_9c20(quad_transform[2], quad_transform[1], quad_transform[0]);
    c = deplanetizer_glow_first_quad_base_tint_gp | (deplanetizer_glow_quad_alpha << 24);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    pk.col[3] = c;
    pk.col[2] = c;
    pk.col[1] = c;
    pk.col[0] = c;
    for (i = 0; i < 4; i++) {
        scale_9c20(pk.m[i], D_L18_001DFB90[i], deplanetizer_glow_first_quad_scale_gp);
        xform_9c20(pk.m[i], pk.m[i], quad_transform);
    }
    draw_9c20(&pk, 0, 0);
    second_quad_color = deplanetizer_glow_second_quad_base_tint_gp | (deplanetizer_glow_quad_alpha << 24);
    pk.col[3] = second_quad_color;
    pk.col[2] = second_quad_color;
    pk.col[1] = second_quad_color;
    pk.col[0] = second_quad_color;
    for (i = 0; i < 4; i++) {
        scale_9c20(pk.m[i], D_L18_001DFB90[i], deplanetizer_glow_second_quad_scale_gp);
        xform_9c20(pk.m[i], pk.m[i], quad_transform);
    }
    draw_9c20(&pk, 0, 0);
}

extern float deplanetizer_first_quad_local_z_scale_gp __asm__("D_L18_00162514") __attribute__((sda));
extern float deplanetizer_second_quad_local_z_scale_gp __asm__("D_L18_00162518") __attribute__((sda));
extern float deplanetizer_third_quad_local_z_scale_gp __asm__("D_L18_0016251C") __attribute__((sda));
extern float D_L18_001DFBD0[4][4];
void draw_moby_three_scaled_two_tint_quads(struct Moby *m) {
    Pk_9c20 pk;
    float quad_transform[4][4];
    float up[4];
    Vec4 vertices[4];
    int i = 0;
    int color;

    pk.b = tex_9c20(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    gs_9c20(0x4A, 0);
    gs_9c20(0x47, 0x51001);
    rot_9c20(quad_transform[3], &deplanetizer_glow_local_offset_x, &m->rot_row0);
    add_9c20(quad_transform[3], quad_transform[3], &m->pos);
    *(u128_9c20 *)up = 0;
    up[2] = -1.0f;
    up[3] = 1.0f;
    rot_9c20(quad_transform[0], up, &m->rot_row0);
    sub_9c20(quad_transform[1], quad_transform[3], D_L18_001677C0_9c20);
    cross_9c20(quad_transform[2], quad_transform[1], quad_transform[0]);
    norm_9c20(quad_transform[2], quad_transform[2], 1.0f);
    cross_9c20(quad_transform[1], quad_transform[0], quad_transform[2]);

    color = deplanetizer_glow_first_quad_base_tint_gp | 0xFF000000;
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;

    pk.col[3] = color;
    pk.col[2] = color;
    pk.col[1] = color;
    pk.col[0] = color;
    i = 0;
    do {
        qcopy_nc(&vertices[i], D_L18_001DFBD0[i]);
        {
            vertices[i].f[2] *= deplanetizer_first_quad_local_z_scale_gp;
            vertices[i].f[0] *= deplanetizer_three_quad_local_x_scale;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        xform_9c20(pk.m[i], &vertices[i], quad_transform);
    }
    draw_9c20(&pk, 0, 0);

    pk.col[3] = deplanetizer_glow_second_quad_base_tint_gp | 0x7F000000;
    pk.col[2] = pk.col[3];
    pk.col[1] = pk.col[3];
    pk.col[0] = pk.col[3];
    i = 0;
    do {
        qcopy_nc(&vertices[i], D_L18_001DFBD0[i]);
        {
            vertices[i].f[2] *= deplanetizer_second_quad_local_z_scale_gp;
            vertices[i].f[0] *= deplanetizer_three_quad_local_x_scale;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        xform_9c20(pk.m[i], &vertices[i], quad_transform);
    }
    draw_9c20(&pk, 0, 0);

    pk.col[3] = deplanetizer_glow_second_quad_base_tint_gp | 0x7F000000;
    pk.col[2] = pk.col[3];
    pk.col[1] = pk.col[3];
    pk.col[0] = pk.col[3];
    i = 0;
    do {
        qcopy_nc(&vertices[i], D_L18_001DFBD0[i]);
        {
            vertices[i].f[2] *= deplanetizer_third_quad_local_z_scale_gp;
            vertices[i].f[0] *= deplanetizer_three_quad_local_x_scale;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        xform_9c20(pk.m[i], &vertices[i], quad_transform);
    }
    draw_9c20(&pk, 0, 0);
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FB6B0), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
unsigned char *FUN_L00_0026d000(void *pos, int n, void *vel, int color, float r, float lo, float hi,
                                float size);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern char *func_L00_0026DEA0_c(void *, int, void *, float, float, float, float,
                                 int) __asm__("FUN_L00_0026d000");

void spawn_deplanetizer_gun_trail_effects(void *first_position_source, void *second_position_source, void *effect_velocity_source, float first_effect_size, float second_effect_size) {
    char first_effect_position[16];
    char second_effect_position[16];
    int i;
    char *random_magnitude_or_effect;
    char *effect_data;
    int x;
    int k2, k7f;
    qcopy(first_effect_position, first_position_source);
    qcopy(second_effect_position, second_position_source);
    for (i = 1; i >= 0; i--) {
        random_magnitude_or_effect = (char *)random_integer_below(16);
        random_magnitude_or_effect = func_L00_0026DEA0_c(first_effect_position, random_integer_below(2) ? -(int)random_magnitude_or_effect : (int)random_magnitude_or_effect, effect_velocity_source, 0.0f, 1.0f, 0.9f,
                                first_effect_size, 0x7F204080);
        effect_data = random_magnitude_or_effect + 0x20;
        if (random_magnitude_or_effect) {
            *(short *)(random_magnitude_or_effect + 0xA) = scale_game_frames(15);
            random_magnitude_or_effect[9] = func_001FA898_r(4.0f) + 0x40;
            *(int *)(effect_data + 4) = 2;
            effect_data[0xA] = 0x7F;
            effect_data[0xB] = random_magnitude_or_effect[0xA];
        }
    }
    x = 16;
    k2 = 2;
    k7f = 0x7F;
    i = 2;
    do {
        int y = x;
        x = -x;
        random_magnitude_or_effect = func_L00_0026DEA0_c(second_effect_position, y, effect_velocity_source, 0.0f, 1.0f, 0.97f, second_effect_size, 0x7FFFFFFF);
        effect_data = random_magnitude_or_effect + 0x20;
        if (random_magnitude_or_effect) {
            random_magnitude_or_effect[9] = func_001FA898_r(4.0f) + 0x40;
            *(short *)(random_magnitude_or_effect + 0xA) = scale_game_frames(4);
            random_magnitude_or_effect[8] = random_integer_below(0xFF);
            *(int *)(effect_data + 4) = k2;
            effect_data[0xA] = k7f;
            effect_data[0xB] = random_magnitude_or_effect[0xA];
        }
    } while (--i >= 0);
}
#include "sda.h"

/* State machine: waits for level flags, then turns the moby around and back. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBAA0), where it is exact; names translated to the US level program. */

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern int D_L18_0015F5C4;
extern int D_L18_0016D290_lui __asm__("D_L18_0016D290") __attribute__((section(".data")));

void update_moby_visibility_and_half_turn(struct Moby *moby) __asm__("FUN_L18_002fa640");

void update_moby_visibility_and_half_turn(struct Moby *moby) __asm__("FUN_L18_002fa640");
void update_moby_visibility_and_half_turn(struct Moby *moby) {
    enum {
        INITIALIZE_STATE_MACHINE = 0,
        WAIT_TO_HIDE_AND_TURN = 1,
        HIDDEN_WAIT_TO_SHOW = 2
    };
    switch (moby->state) {
    case INITIALIZE_STATE_MACHINE:
        moby->state = 1;
        break;
    case WAIT_TO_HIDE_AND_TURN:
        if (D_L18_0015F5C4 == 2 && D_L18_0016D290_lui == 3) {
            moby->flags |= 1;
            moby->visible = 0;
            moby->rot.x = fast_add_rotations(moby->rot.x, 3.1415927f);
            moby->state = 2;
        }
        break;
    case HIDDEN_WAIT_TO_SHOW:
        if (D_L18_0015F5C4 != 2) {
            unsigned short fl = moby->flags;
            moby->state = 3;
            fl &= 0xFFFE;
            moby->visible = 1;
            moby->flags = fl;
        }
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* updates a moby that drags a child moby along with it */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBB88), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_00250df8(void *);
extern void update_entity_matrix(char *a, void *b, void *c, void *d) __asm__("FUN_L00_00260738");
extern char *func_0020D348_m(int) __asm__("FUN_0020c4f8");

void update_moby_transform_and_child(char *moby) __asm__("FUN_L18_002fa728");

void update_moby_transform_and_child(char *moby) {
    enum {
        INITIALIZE_TRANSFORM_HISTORY = 0,
        UPDATE_TRANSFORM_FROM_HISTORY = 1
    };
    char *data = *(char **)(moby + 0x78);
    char position_delta[16];
    char *child;
    switch (*(unsigned char *)(moby + 0x20)) {
    case INITIALIZE_TRANSFORM_HISTORY:
        moby[0x20] = 1;
        break;
    case UPDATE_TRANSFORM_FROM_HISTORY:
        fast_vec_sub(position_delta, moby + 0x10, data + 0x60);
        update_entity_matrix(data + 0x20, position_delta, data + 0x70, moby + 0x40);
        break;
    }
    if (*(unsigned char *)(moby + 0xBC) != 0 && *(int *)(data + 0x80) != 0 &&
        *(int *)(data + 0x84) == 0) {
        char *new_child = func_0020D348_m(0x764);
        *(char **)(data + 0x84) = new_child;
        if (new_child != 0) {
            *(unsigned short *)(new_child + 0x32) = *(unsigned short *)(moby + 0x32);
            (*(char **)(data + 0x84))[0x31] = 1;
            *(long *)(*(char **)(data + 0x84) + 0x38) = *(long *)(moby + 0x38);
            *(float *)(*(char **)(data + 0x84) + 0x2C) *=
                *(float *)(moby + 0x2C) / *(float *)(*(char **)(moby + 0x24) + 0x24);
        }
    }
    qcopy(data + 0x60, moby + 0x10);
    qcopy(data + 0x70, moby + 0x40);
    child = *(char **)(data + 0x84);
    if (child != 0) {
        qcopy(child + 0x10, moby + 0x10);
        qcopy(child + 0x40, moby + 0x40);
        FUN_L00_00250df8(child);
    }
}
extern int D_L18_001AC240[];
extern int D_L18_0015FFD8;
/* 0x002fa888, 88 bytes.
 * Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c:
 * func_L18_002FBCE8), where it is exact; names translated to the US level
 * program. */
void set_group_class_0630_moby_byte_bc(int group_index, int value) __asm__("FUN_L18_002fa888");

void set_group_class_0630_moby_byte_bc(int group_index, int value) {
    unsigned short *table = (unsigned short *)D_L18_001AC240[group_index];

    if (table != 0) {
        unsigned short *group_moby_entry = table;
        do {
            unsigned char *moby = (unsigned char *)(((group_moby_entry[0] & 0x7FFF) << 8) + D_L18_0015FFD8);

            if (*(short *)(moby + 0xA6) == 0x630) {
                moby[0xBC] = value;
            }
        } while ((short)*group_moby_entry++ >= 0);
    }
}

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

#define NOT_SDA

#define MACRO_ADDR

/* Remove a vendor moby and any child it owns. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBD40), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x84];
    void *child;
} L18VD_a8e0;

typedef struct {
    char pad0[0x78];
    L18VD_a8e0 *data;
    char pad7C[0x2A];
    short class_id;
} L18VM_a8e0;

void mark_class_0630_moby_and_child_for_removal(L18VM_a8e0 *moby) __asm__("FUN_L18_002fa8e0");

void mark_class_0630_moby_and_child_for_removal(L18VM_a8e0 *moby) {
    if (moby->class_id == 0x630) {
        void *child = moby->data->child;
        if (child != 0) {
            mark_moby_for_removal(child);
        }
        mark_moby_for_removal(moby);
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FC188), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x30];
    unsigned int n;
    char pad2[0x178 - 0x34];
    char *arr[5];
} ML;

typedef struct {
    char pad[0x20];
    unsigned char state;
    char pad2[0xF];
    unsigned char v;
} M2;

extern ML D_L18_0016D260;
extern int D_L18_0015F5C4;
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void draw_trail(void *, void *, void *, float, float) __asm__("FUN_L00_00263b70");

extern short D_L18_00162578 __attribute__((sda));
extern short D_L18_0016257C __attribute__((sda));
extern short D_L18_00162580 __attribute__((sda));
void update_selected_moby_joint_trail(M2 *moby) __asm__("FUN_L18_002fad28");

void update_selected_moby_joint_trail(M2 *moby) {
    enum {
        INITIALIZE_TRAIL_EFFECT = 0,
        UPDATE_TRAIL_EFFECT = 1
    };
    char joint_eight_world_position[16];
    char joint_nine_world_position[16];
    char trail_velocity[16];
    int trail_source_moby_index;
    char *trail_source_moby;
    switch (moby->state) {
    case INITIALIZE_TRAIL_EFFECT:
        moby->v = 0xFF;
        moby->state = 1;
        break;
    case UPDATE_TRAIL_EFFECT:
        if (D_L18_0015F5C4 == 2) {
            if (D_L18_0016D260.n < 4) {
                trail_source_moby_index = 0;
                if (D_L18_0016D260.n == 0 || D_L18_0016D260.n == D_L18_0015F5C4 ||
                    D_L18_0016D260.n == 3)
                    trail_source_moby_index = 3;
                if (D_L18_0016D260.n == 1)
                    trail_source_moby_index = 4;
                trail_source_moby = D_L18_0016D260.arr[trail_source_moby_index];
                moby_point_position(trail_source_moby, 8, joint_eight_world_position);
                moby_point_position(trail_source_moby, 9, joint_nine_world_position);
                fast_vec_sub(trail_velocity, joint_eight_world_position, joint_nine_world_position);
                fast_vec_normalize(trail_velocity, trail_velocity, *(float *)&D_L18_00162578);
                draw_trail(joint_eight_world_position, joint_eight_world_position, trail_velocity, *(float *)&D_L18_0016257C,
                                 *(float *)&D_L18_00162580);
            }
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FD058), where it is exact; names translated to the US level program. */

typedef unsigned int QVd058 __attribute__((mode(TI), aligned(16)));

extern char D_L18_001625F0[4] __attribute__((sda));
extern char D_L18_00162608 __attribute__((sda));
extern float vector_distance_xy(float *, float *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int spawn_explosion() __asm__("FUN_L00_0025e450");
extern int FUN_L00_002dc9f0(void *, void *);
extern int is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int random_integer_below(int);
extern int scale_game_frames(int);
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_data/gadgebot_update.h"
extern void collect_mobies_in_sphere_at_moby_point(void *, float, float, float, int, int, int, int, int) __asm__("FUN_L00_002599e8");
extern void update_gadgebot_damage_target_and_glow(struct Moby *) __asm__("FUN_L18_002fc100");
void draw_gadgebot_launch_glow_quad(struct Moby *m) __asm__("FUN_L18_002fc860");

void draw_gadgebot_launch_glow_quad(struct Moby *m) __asm__("FUN_L18_002fc860");
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void scale_vector_xyz(void *, void *, float);
extern void vu_euler_rotation_basis(void *, void *);
float move_gadgebot_toward_target(char *self, float *src) __asm__("FUN_L18_002fc4e0");
extern float FUN_L18_002fc4e0_c(void *, void *) __asm__("FUN_L18_002fc4e0");
extern float f9d48_i(void *, void *) __asm__("FUN_001f9b80");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");

void gadgebot_update(unsigned char *m) __asm__("FUN_L18_002fbb98");
void gadgebot_update(unsigned char *moby) {
    enum {
        GADGEBOT_INITIALIZE = 0,
        GADGEBOT_LAUNCH_FLIGHT = 1,
        GADGEBOT_WAIT_TO_APPROACH = 2,
        GADGEBOT_APPROACH_TARGET = 3,
        GADGEBOT_FACE_TARGET = 4,
        GADGEBOT_DEATH_FALL = 5,
        GADGEBOT_EXPLODE = 6
    };
    char position_scratch[16];
    unsigned char *moby_vars = *(unsigned char **)(moby + 0x78);
    unsigned char *moby_transform_ptr;
    update_gadgebot_damage_target_and_glow((struct Moby *)moby);
    switch (moby[0x20]) {
    case GADGEBOT_INITIALIZE: {
        unsigned short moby_flags;
        moby[0x20] = 8;
        if (random_integer_below(2))
            *(unsigned short *)(moby + 0x34) |= 0x8000;
        else
            *(unsigned short *)(moby + 0x34) &= 0x7FFF;
        *(unsigned short *)(moby + 0x34) |= 0x100;
        moby[0x73] = D_L18_00162608;
        *(void **)(moby_vars + 0x140) = &D_L18_001625F0;
        *(float *)(moby_vars + 0x20) = 1.0f;
        moby_vars[0x58] = 8;
        *(short *)(moby_vars + 0x24) = 1;
        moby_vars[0x5C] = 1;
        *(float *)(moby_vars + 0x30) = 0.5f;
        *(float *)(moby_vars + 0x1F8) = random_angle_radians();
        moby[0x30] = 0xFF;
        moby_flags = *(unsigned short *)(moby + 0x34);
        *(short *)(moby + 0x32) = 0xFF;
        moby_flags |= 1;
        moby[0x31] = 0;
        moby_flags &= 0xEFFF;
        *(int *)(moby + 0x94) = 0;
        *(unsigned short *)(moby + 0x34) = moby_flags;
        break;
    }
    case GADGEBOT_LAUNCH_FLIGHT: {
        float launch_gravity_and_minimum_z = 10.0f;
        float r;
        moby_transform_ptr = moby + 0x10;
        *(float *)(moby_vars + 0x1D8) = *(float *)(moby_vars + 0x1D8) - frame_time_sq * launch_gravity_and_minimum_z;
        add_vector_xyz(moby_transform_ptr, moby_transform_ptr, moby_vars + 0x1D0);
        qcopy(position_scratch, moby_transform_ptr);
        ((float *)position_scratch)[2] += 1.0f;
        r = probe_ground_height(position_scratch, 0, 0.5f);
        if (*(float *)(moby + 0x18) < r) {
            *(float *)(moby + 0x18) = r;
            moby[0x20] = 2;
            if (moby[0x53])
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (*(float *)(moby + 0x18) < launch_gravity_and_minimum_z) {
            moby[0x20] = 6;
        }
        break;
    }
    case 7:
        if (FUN_L00_002dc9f0(moby, moby_vars + 0x70)) {
            moby[0x20] = 2;
            *(short *)(moby_vars + 0x138) = 0;
        }
        break;
    case GADGEBOT_WAIT_TO_APPROACH:
        if (*(int *)(moby_vars + 0x1C4) == 2) {
            if (!(10.0f < f9d48_i(moby + 0x10, moby_vars + 0x180)))
                break;
        }
        moby[0x20] = 3;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        break;
    case GADGEBOT_APPROACH_TARGET: {
        float target_distance;
        *(QVd058 *)position_scratch = *(QVd058 *)(moby_vars + 0x180);
        target_distance = FUN_L18_002fc4e0_c(moby, position_scratch);
        if (*(int *)(moby_vars + 0x1C4) == 2 && target_distance < 5.0f) {
            moby[0x20] = 2;
            if (moby[0x53])
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            break;
        }
        if (target_distance < 1.5f) {
            moby[0x20] = 4;
            if (moby[0x53] != 2) {
                int animation_frame_index = (int)gadgebot_sequence_2_start_frame_gp;
                blend_moby_animation(moby, 2, animation_frame_index, scale_game_frames(5));
            }
        }
        break;
    }
    case GADGEBOT_FACE_TARGET: {
        float r, k, b, max_turn_step;
        moby_transform_ptr = moby + 0x48;
        r = fast_arc_tan(*(float *)(moby_vars + 0x180) - *(float *)(moby + 0x10),
                         *(float *)(moby_vars + 0x184) - *(float *)(moby + 0x14));
        k = 6.2831855f;
        b = frame_time_sq * k;
        max_turn_step = frame_time * k;
        approach_angle((float *)moby_transform_ptr, (float *)(moby_vars + 0x1E8), r, b, b, max_turn_step);
        if (is_value_within_interpolated_window((char *)moby, 14.0f))
            collect_mobies_in_sphere_at_moby_point(moby, 0.5f, 1.0f, 1.0f, 1, 1, 0, 1, 0);
        if (moby[0x70] & 2) {
            if (*(int *)(moby_vars + 0x1C4) == 2) {
                moby[0x20] = 2;
                if (moby[0x53])
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                break;
            }
            if (!(2.5f < f9d48_i(moby + 0x10, moby_vars + 0x180)))
                break;
            moby[0x20] = 3;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            break;
        }
        break;
    }
    case GADGEBOT_DEATH_FALL:
        if (!(mine_fall(moby, moby_vars + 0x70) & 0x140))
            break;
    case GADGEBOT_EXPLODE:
        qcopy(position_scratch, moby + 0x10);
        ((float *)position_scratch)[2] += 0.5f;
        func_L00_0025F4A8_alt(moby, moby_vars + 0x40, position_scratch, 0.0f, 0.0f, 5, 2, 4, 1.0f, 0.5f, 9.0f, 0.5f, 4,
                              0.0f, 0, 1, -1, 0);
        moby[0x20] = 8;
        if (moby[0x53])
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        *(int *)(moby + 0x94) = 0;
        *(unsigned short *)(moby + 0x34) = (*(unsigned short *)(moby + 0x34) | 0x41) & 0xEFFF;
        break;
    }
    moby_transform_ptr = moby + 0xC0;
    vu_euler_rotation_basis(moby_transform_ptr, moby + 0x40);
    if (*(float *)(moby_vars + 0x1F4) != 0.0f) {
        float g;
        approach_value((float *)(moby_vars + 0x1F4), 0.0f, frame_time * 4.0f);
        g = 1.0f;
        scale_vector_xyz(moby + 0xE0, moby + 0xE0, *(float *)(moby_vars + 0x1F4) + g);
        g = g - *(float *)(moby_vars + 0x1F4);
        scale_vector_xyz(moby_transform_ptr, moby_transform_ptr, g);
        scale_vector_xyz(moby + 0xD0, moby + 0xD0, g);
        enqueue_callback_list_1(draw_gadgebot_launch_glow_quad, moby);
    }
}
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
extern int game_frame __asm__("D_L18_0015F5CC");
extern int D_L18_0015F5C4;
extern int *D_L18_001B0EB0[] __asm__("D_L18_001B0EB0");
void draw_group_class_0772_joint_glows(struct Moby *moby) __asm__("FUN_L18_002fca98");

extern void draw_group_class_0772_joint_glows(struct Moby *moby) __asm__("FUN_L18_002fca98");
extern float fast_add_rotations_c100(float, float) __asm__("FUN_001fa580");
extern float fast_sin_c100(float) __asm__("FUN_001f9de0");
extern int tween_color_c100(int, int, float) __asm__("FUN_001fa6e0");
extern void blend_moby_animation_c100(void *, int, int, int) __asm__("FUN_00212f90");
extern int scale_game_frames_c100(int) __asm__("FUN_001f96f8");
extern char *find_moby_hit_c100(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction_c100(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_00213928");
extern int random_integer_below_c100(int) __asm__("FUN_00213260");
extern void knockback_c100(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void start_fall_c100(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void anim_reset_c100(void *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_c100(void *, void *) __asm__("FUN_L00_0025d538");
extern int FUN_L00_0025ff38_c100(float, void *, void *, int, int, int *, int) __asm__("FUN_L00_0025ff38");

typedef struct {
    char pad0[0x20];
    float health;              /* 0x20 */
    char pad24[0x3C];
    char anim[0x7];            /* 0x60 */
    unsigned char anim_flags;  /* 0x67 */
    char pad68[8];
    char fall[0x10];           /* 0x70 */
    float unk80;
    float unk84;
    float unk88;
    float unk8C;
    int unk90;
    int unk94;
    float unk98;
    char pad9C[0x11];
    unsigned char unkAD;       /* 0xAD */
    char padAE[0xE];
    float unkBC;
    float unkC0;
    float unkC4;
    char padC8[0xB8];
    u128 target;               /* 0x180 */
    char pad190[0x30];
    struct Moby *target_moby;  /* 0x1C0 */
    int target_mode;           /* 0x1C4 */
    char pad1C8[0x18];
    int path;                  /* 0x1E0 */
    char pad1E4[0x14];
    float spin;                /* 0x1F8 */
} SentryVars_c100;

extern int gadgebot_joint_glows_last_queued_frame_gp __asm__("D_L18_00162604") __attribute__((sda));
extern int gadgebot_glow_tint_start_gp __asm__("D_L18_0016260C") __attribute__((sda));
extern int gadgebot_glow_tint_end_gp __asm__("D_L18_00162610") __attribute__((sda));
extern float D_L18_001625FC __attribute__((sda));
extern float gadgebot_glow_default_phase_speed_degrees_per_second_gp __asm__("D_L18_00162614") __attribute__((sda));
extern float gadgebot_glow_state_4_or_5_phase_speed_degrees_per_second_gp __asm__("D_L18_00162618") __attribute__((sda));
#include "rnc/overlay/l18/gameplay/entities/deplanetizer_gun_data/update_gadgebot_damage_target_and_glow.h"
/* Sentry update: spins and tints the moby, takes hits (knockback, then a fall), and follows its path toward the hero. */
void update_gadgebot_damage_target_and_glow(struct Moby *moby) {
    SentryVars_c100 *sentry_vars = (SentryVars_c100 *)moby->pvars;
    union { u128 q; float f[4]; } v;
    int hit;
    float dmg;
    float angle;
    float speed;
    char *coll;
    int reaction_value_or_path_address;                      /* the reaction kind, later the path */
    int fall_setup_value_or_path_slot_address;                      /* the reaction flags, later the path's slot in the table */

    if (moby->state == 0 || moby->state == 8) {
        return;
    }
    if (moby->state != 7 && game_frame != gadgebot_joint_glows_last_queued_frame_gp) {
        gadgebot_joint_glows_last_queued_frame_gp = game_frame;
        enqueue_callback_list_1(draw_group_class_0772_joint_glows, moby);
    }
    speed = gadgebot_glow_default_phase_speed_degrees_per_second_gp;
    if (moby->state == 4 || moby->state == 5) {
        speed = gadgebot_glow_state_4_or_5_phase_speed_degrees_per_second_gp;
    }
    sentry_vars->spin = fast_add_rotations_c100(sentry_vars->spin, speed * DEG_TO_RAD * frame_time);
    moby->glow_color = tween_color_c100(gadgebot_glow_tint_start_gp, gadgebot_glow_tint_end_gp,
                                fast_sin_c100(sentry_vars->spin) * 0.5f + 0.5f);
    if (moby->pos.z < 100.0f && moby->state != 7) {
        moby->state = 6;
    }
    moby->scale = moby->pclass->scale * D_L18_001625FC;
    if (hero.state.current == 0x72 || D_L18_0015F5C4 == 2) {
        if (moby->state != 7) {
            moby->state = 8;
            if (moby->prev_seq != 0) {
                blend_moby_animation_c100(moby, 0, 0, scale_game_frames_c100(10));
            }
            moby->collision = 0;
            moby->flags = (moby->flags | 0x41) & ~0x1000;
            return;
        }
    }
    dmg = 0.0f;
    coll = find_moby_hit_c100(moby, 0x330000, 0);
    hit_reaction_c100(moby, coll, &sentry_vars->health, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && moby->state != 6 && dmg != 0.0f) {
        sentry_vars->health -= dmg;
        if (sentry_vars->health <= 0.0f) {
            if (random_integer_below_c100(2) != 0) {
                moby->state = 6;
            } else {
                fall_setup_value_or_path_slot_address = 0x200;
                sentry_vars->unk90 = fall_setup_value_or_path_slot_address;
                sentry_vars->unk98 = 0.5f;
                sentry_vars->unk80 = gadgebot_death_fall_gravity_gp * frame_time_sq;
                sentry_vars->unk84 = gadgebot_death_fall_horizontal_speed_approach_rate_gp * frame_time_sq;
                sentry_vars->unk88 = gadgebot_death_fall_base_horizontal_speed_gp * frame_time;
                sentry_vars->unk8C = gadgebot_death_fall_base_vertical_speed_gp * frame_time;
                reaction_value_or_path_address = 0x29;
                sentry_vars->unk94 = reaction_value_or_path_address;
                sentry_vars->unkAD = 0;
                sentry_vars->unkBC = frame_time + frame_time;
                moby->flags &= ~0x1000;
                v.q = *(u128 *)(coll + 0x10);
                knockback_c100(&v, &angle, &sentry_vars->unk88, &sentry_vars->unk8C);
                start_fall_c100(moby, sentry_vars->fall, 3, 1, 0, angle);
                sentry_vars->unkC0 = 7.5f;
                sentry_vars->unkC4 = 15.0f;
                moby->state = 5;
                moby->collision = 0;
                sentry_vars->anim_flags = 0x78;
                anim_reset_c100(moby, sentry_vars->anim);
            }
        }
    }
    moby->hit_slot = 0xFF;
    anim_update_c100(moby, sentry_vars->anim);
    fall_setup_value_or_path_slot_address = (int)&D_L18_001B0EB0[sentry_vars->path];
    reaction_value_or_path_address = *(int *)fall_setup_value_or_path_slot_address;
    FUN_L00_0025ff38_c100(512.0f, moby, &sentry_vars->target, 0, 0, (int *)(reaction_value_or_path_address + 0x10), *(int *)reaction_value_or_path_address);
    if (sentry_vars->target_mode == 2) {
        if (hero.state.control_mode == 0xF) {
            int *list = (int *)hero.rail_spline;
            qcopy(&sentry_vars->target, (char *)list + *list * 16);
        } else {
            qcopy(&sentry_vars->target, &hero.motion.pos);
        }
    }
    if (sentry_vars->target_moby == 0) {
        sentry_vars->target_moby = hero.moby;
    }
}
#include "eetypes.h"

/* Turns and accelerates the moby toward a target point and returns the remaining distance. */
typedef struct {
    float v[4];
} __attribute__((aligned(16))) QVec;

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float vector_distance_xy(float *, float *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern void FUN_L00_00258830(int, int, float, float, float, int);
extern void resolve_point_path_segment_xy_clearance(float, void *, void *, void *) __asm__("FUN_L00_00261d78");

float move_gadgebot_toward_target(char *self, float *target_position_source) {
    QVec target_position;
    float v[4];
    float movement_distance_scratch[4];
    float z = 0.0f;
    float movement_clearance = 0.5f;
    char *moby_vars = *(char **)(self + 0x78);
    float initial_target_distance_xy;
    *(u128 *)&target_position = *(u128 *)target_position_source;
    {
        float d = fast_add_rotations(
            fast_arc_tan(target_position.v[0] - *(float *)(self + 0x10), target_position.v[1] - *(float *)(self + 0x14)),
            *(float *)(moby_vars + 0x1EC));
        approach_angle((float *)(self + 0x48), (float *)(moby_vars + 0x1E8), d, frame_time_sq * 6.2831855f,
                         frame_time_sq * 6.2831855f, frame_time * 6.2831855f);
    }
    movement_distance_scratch[0] = z;
    initial_target_distance_xy = vector_distance_xy((float *)(self + 0x10), target_position.v);
    advance_accelerated_scalar(movement_distance_scratch, (float *)(moby_vars + 0x1F0), initial_target_distance_xy, frame_time_sq * 6.0f, frame_time_sq * 6.0f,
                               frame_time * 5.0f);
    v[0] = fast_cos(*(float *)(self + 0x48)) * *(float *)(moby_vars + 0x1F0);
    v[1] = fast_sin(*(float *)(self + 0x48)) * *(float *)(moby_vars + 0x1F0);
    v[2] = -(frame_time + frame_time);
    FUN_L00_00258830((int)self, (int)v, movement_clearance, movement_clearance, z, 0);
    resolve_point_path_segment_xy_clearance(movement_clearance, *(void **)(moby_vars + 0x1E0), self + 0x10, self + 0x10);
    return initial_target_distance_xy;
}
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"
extern int D_L18_001AC240[];
extern struct Moby *moby_table __asm__("D_L18_0015FFD8");

/* Pvars fields set when a group moby is launched. */
typedef struct {
    u8 pad0[0x1D0];
    f32 velocity[4];   /* 0x1D0: launch direction scaled to speed; [2] replaced by the arc speed */
    u8 pad1E0[0xC];
    f32 spin;          /* 0x1EC: random 15..45 degrees, in radians */
    u8 pad1F0[4];
    f32 unk1F4;        /* 0x1F4: set to 1.0 on launch */
} LaunchedMobyVars;
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_L00_0025abf0(void *, void *, float, float, int);
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");

/* Launches the first idle (state 8) class-0x772 moby of group `group` from `from` towards `to` at `speed`.
   Returns 1 when one was launched. */
int launch_first_idle_group_class_0772_moby(int group, void *from, void *to, float speed) __asm__("FUN_L18_002fc668");

int launch_first_idle_group_class_0772_moby(int group, void *from, void *to, float speed) {
    unsigned short *group_moby_entry = (unsigned short *)D_L18_001AC240[group];
    struct Moby *table;
    struct Moby *moby;

    if (group_moby_entry == 0)
        return 0;
    table = moby_table;
    for (;;) {
        moby = &table[*group_moby_entry & 0x7FFF];
        if (moby->oclass == 0x772) {
            if (moby->state == 8) {
                LaunchedMobyVars *launched_moby_vars = (LaunchedMobyVars *)moby->pvars;
                moby->flags &= ~0x41;
                moby->collision = moby->pclass->collision;
                moby->flags |= 0x1000;
                qcopy(&moby->pos, from);
                moby->state = 1;
                launched_moby_vars->unk1F4 = 1.0f;
                fast_vec_sub(launched_moby_vars->velocity, to, from);
                launched_moby_vars->velocity[2] = 0.0f;
                fast_vec_normalize(launched_moby_vars->velocity, launched_moby_vars->velocity, speed);
                launched_moby_vars->velocity[2] = FUN_L00_0025abf0(from, to, speed, -(frame_time_sq * 10.0f), 0);
                launched_moby_vars->spin = randf_sym(15.0f, 45.0f) * 0.017453292f;
                return 1;
            }
        }
        if ((short)*group_moby_entry++ < 0)
            return 0;
    }
}
/* Counts the listed mobys of class 0x772 that are not in state 8. */

extern int D_L18_001AC240[];
extern int D_L18_0015FFD8;

short count_group_class_0772_mobys_not_in_state_8(int group_index) __asm__("FUN_L18_002fc7e0");

short count_group_class_0772_mobys_not_in_state_8(int group_index) {
    unsigned short *group_moby_entry = (unsigned short *)D_L18_001AC240[group_index];
    short n = 0;
    int base;
    if (group_moby_entry == 0)
        return 0;
    base = D_L18_0015FFD8;
    while (1) {
        unsigned char *moby = (unsigned char *)(base + ((group_moby_entry[0] & 0x7FFF) << 8));
        if (*(short *)(moby + 0xA6) == 0x772 && moby[0x20] != 8)
            n++;
        if ((short)*group_moby_entry++ < 0)
            return n;
    }
    return n;
}
/* Draws the moby's glow as a camera-facing quad sized by its charge, coloured between two tints. */
#include "qcopy.h"
typedef struct {
    float m[4][4];
    int col[4];
    float uv[4][2];
    unsigned long a, b, c, d;
} Pk_c860;
extern char D_L18_001677C0_g[] __asm__("D_L18_001677C0");
extern void clear_g(void *) __asm__("FUN_001f99f8");
extern void sub_g(void *, void *, void *) __asm__("FUN_001f9a28");
extern void cross_g(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void norm_g(void *, void *, float) __asm__("FUN_001f9bf8");
extern unsigned long tex_g(int) __asm__("FUN_001f44b8");
extern void draw_g(void *, void *, int) __asm__("FUN_001f7d30");

extern int gadgebot_launch_glow_blend_minuend_selector_gp __asm__("D_L18_0016262C") __attribute__((sda));
extern int gadgebot_launch_glow_blend_subtrahend_selector_gp __asm__("D_L18_00162630") __attribute__((sda));
extern int gadgebot_launch_glow_blend_alpha_factor_selector_gp __asm__("D_L18_00162634") __attribute__((sda));
extern int gadgebot_launch_glow_blend_addend_selector_gp __asm__("D_L18_00162638") __attribute__((sda));
extern int gadgebot_launch_glow_tint_start_gp __asm__("D_L18_0016263C") __attribute__((sda));
extern int gadgebot_launch_glow_tint_end_gp __asm__("D_L18_00162640") __attribute__((sda));
extern int gadgebot_launch_glow_texture_index_gp __asm__("D_L18_00162644") __attribute__((sda));
extern float gadgebot_launch_glow_z_offset_gp __asm__("D_L18_00162648") __attribute__((sda));
extern float gadgebot_launch_glow_vertical_scale_adjustment_gp __asm__("D_L18_0016264C") __attribute__((sda));
extern float gadgebot_launch_glow_horizontal_scale_adjustment_gp __asm__("D_L18_00162650") __attribute__((sda));
extern float D_L18_001EFCD0[4][2];
void draw_gadgebot_launch_glow_quad(struct Moby *m) {
    char *d = (char *)m->pvars;
    Pk_c860 pk;
    float quad_transform[4][4];
    float horizontal_axis_scale;
    float vertical_axis_scale;
    int c;
    int i;
    float *quad_vertex;
    vertical_axis_scale = *(float *)(d + 0x1F4) + 1.0f + gadgebot_launch_glow_vertical_scale_adjustment_gp;
    horizontal_axis_scale = 1.0f - *(float *)(d + 0x1F4) + gadgebot_launch_glow_horizontal_scale_adjustment_gp;
    qcopy(quad_transform[3], &m->pos);
    quad_transform[3][2] -= gadgebot_launch_glow_z_offset_gp;
    clear_g(quad_transform[2]);
    quad_transform[2][2] = vertical_axis_scale;
    sub_g(quad_transform[0], D_L18_001677C0_g, &m->pos);
    cross_g(quad_transform[1], quad_transform[0], quad_transform[2]);
    cross_g(quad_transform[0], quad_transform[1], quad_transform[2]);
    norm_g(quad_transform[0], quad_transform[0], horizontal_axis_scale);
    norm_g(quad_transform[1], quad_transform[1], horizontal_axis_scale);
    pk.b = tex_g(gadgebot_launch_glow_texture_index_gp);
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 0;
    pk.d = (unsigned long)gadgebot_launch_glow_blend_minuend_selector_gp | ((unsigned long)gadgebot_launch_glow_blend_subtrahend_selector_gp << 2) | ((unsigned long)gadgebot_launch_glow_blend_alpha_factor_selector_gp << 4) |
           ((unsigned long)gadgebot_launch_glow_blend_addend_selector_gp << 6) | 0x8000000000ULL;
    c = fast_tween_color(gadgebot_launch_glow_tint_start_gp, gadgebot_launch_glow_tint_end_gp, *(float *)(d + 0x1F4));
    for (i = 0; i < 4; i++) {
        pk.uv[i][0] = D_L18_001EFCD0[i][0];
        pk.uv[i][1] = D_L18_001EFCD0[i][1];
        quad_vertex = pk.m[i];
        clear_g(quad_vertex);
        if (i & 1)
            quad_vertex[2] = 1.0f;
        else
            quad_vertex[2] = 0.0f;
        if (i < 2)
            quad_vertex[1] = 1.0f;
        else
            quad_vertex[1] = -1.0f;
        quad_vertex[3] = 1.0f;
        pk.col[i] = c;
    }
    draw_g(&pk, quad_transform, 0);
}
#include "sda.h"

/* For each listed moby of matching type, spawns effects at five points. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FDF58), where it is exact; names translated to the US level program. */

extern int D_L18_0015FFD8;
extern short D_L18_00162654;
extern unsigned short *D_L18_001AC240_c[] __asm__("D_L18_001AC240");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

void draw_group_class_0772_joint_glows(struct Moby *moby) {
    unsigned short *group_moby_entry = D_L18_001AC240_c[moby->group];
    if (group_moby_entry != 0) {
        while (1) {
            unsigned char *group_moby = D_L18_0015FFD8 + ((group_moby_entry[0] & 0x7FFF) << 8);
            unsigned short *next = group_moby_entry + 1;
            if (*(short *)(group_moby + 0xA6) == 0x772 && group_moby[0x20] != 0 && group_moby[0x20] < 6) {
                int i;
                int col = (*(int *)(group_moby + 0x90) & 0xFFFFFF) + (*(int *)&D_L18_00162654 << 24);
                float joint_world_position[4];
                for (i = 2; i < 5; i++) {
                    moby_point_position(group_moby, i, joint_world_position);
                    draw_offset_textured_effect_quad(joint_world_position, col, 0.175f, 0.08f);
                }
            }
            if (*(short *)group_moby_entry < 0)
                break;
            group_moby_entry = next;
        }
    }
}
