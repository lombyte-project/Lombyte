/* Level 18 moby class cutscene_effects (0x61b): the deplanetizer gun cutscene sequence, with its camera-facing quads, glows and trails. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/l18/mobys/cutscene_effects_globals.h"

extern int D_L18_0015F5C4;

typedef struct {
    char         pad[0x30];
    int          state;
    int          n;
    char         pad38[0x140];
    struct Moby *objects[8];
} L18SequenceState;

extern L18SequenceState D_L18_0016D260_update __asm__("D_L18_0016D260");
extern int              D_L18_0015F5C4;
extern s32              random_integer_below(s32) __asm__("FUN_00213260");
extern void             enqueue_callback_list_1_overlay(void *, void *) __asm__("FUN_L18_002020c8");
void                    spawn_deplanetizer_gun_trail_effects(void *first_position_source,
                                                             void *second_position_source,
                                                             void *effect_velocity_source,
                                                             float f0,
                                                             float f1) __asm__("FUN_L18_002fa250");

extern void spawn_deplanetizer_gun_trail_effects(void *, void *, void *, float, float) __asm__("FUN_L18_002fa250");
void draw_three_scaled_camera_facing_quads(void) __asm__("FUN_L18_002f9780");

extern void draw_three_scaled_camera_facing_quads(void) __asm__("FUN_L18_002f9780");
void        draw_moby_color_cycling_bone_glows(void *moby) __asm__("FUN_L18_002f9a98");

extern void draw_moby_color_cycling_bone_glows(void *) __asm__("FUN_L18_002f9a98");
void        draw_moby_two_tint_glow_quads(struct Moby *m) __asm__("FUN_L18_002f9c20");

extern void draw_moby_two_tint_glow_quads(struct Moby *) __asm__("FUN_L18_002f9c20");
void        draw_moby_three_scaled_two_tint_quads(struct Moby *m) __asm__("FUN_L18_002f9eb8");

extern void  draw_moby_three_scaled_two_tint_quads(struct Moby *) __asm__("FUN_L18_002f9eb8");
extern void  func_L00_0025F4A8_alt(void *,
                                   void *,
                                   void *,
                                   float,
                                   float,
                                   int,
                                   int,
                                   int,
                                   float,
                                   float,
                                   float,
                                   float,
                                   int,
                                   float,
                                   int,
                                   int,
                                   int,
                                   int) __asm__("FUN_L00_0025e450");
extern char *func_L00_0026DEA0_c(void *, int, void *, float, float, float, float, int) __asm__("FUN_L00_0026d000");
extern struct Moby *l18_moby_list_head __asm__("D_L18_0015FFE4");
extern struct Moby *l18_deplanetizer_class_059b_glow_moby __asm__("D_L18_00162528");

extern float deplanetizer_camera_quad_scale_ramp_start_value_gp __asm__("D_L18_00162490")
    __attribute__((sda));
extern float deplanetizer_camera_quad_scale_ramp_end_value_gp __asm__("D_L18_00162494")
    __attribute__((sda));
extern int deplanetizer_camera_quad_scale_ramp_end_frame_gp __asm__("D_L18_0016249C")
    __attribute__((sda));
extern int deplanetizer_camera_quad_end_frame_gp __asm__("D_L18_001624A0") __attribute__((sda));

#include "rnc/overlay/l18/mobys/cutscene_effects_data/update_deplanetizer_gun_sequence_effects_3.h"

extern int deplanetizer_glow_alpha_ramp_end_frame_gp __asm__("D_L18_001624C0") __attribute__((sda));
extern int deplanetizer_three_quad_scale_ramp_start_frame_gp __asm__("D_L18_001624C4")
    __attribute__((sda));
extern int deplanetizer_three_quad_scale_ramp_end_frame_gp __asm__("D_L18_001624C8")
    __attribute__((sda));

#include "rnc/overlay/l18/mobys/cutscene_effects_data/update_deplanetizer_gun_sequence_effects_2.h"
/* First component of the vector passed at gp - 0x4720. */
#include "rnc/overlay/l18/mobys/cutscene_effects_data/update_deplanetizer_gun_sequence_effects.h"

extern int   D_L18_001617F4, D_L18_001617F8;
extern float D_L18_0015ED60;

extern Vec4 D_L18_00162530 MACRO_ADDR;
extern Vec4 D_L18_00162540 MACRO_ADDR;

typedef struct {
    float radius[5];
} EffectRadii;

extern EffectRadii l18_deplanetizer_object_trail_base_radii __asm__("D_L18_001F2C20");
extern char        D_L18_0015F580[] MACRO_ADDR;
extern void        update_beam_ring_rail_and_pulse_effects(void *) __asm__("FUN_L00_002bd0a0");

extern int deplanetizer_camera_quad_start_frame_gp __asm__("D_L18_00162498") __attribute__((sda));
extern int deplanetizer_glow_alpha_ramp_start_frame_gp __asm__("D_L18_001624BC")
    __attribute__((sda));
extern char deplanetizer_camera_rotation_basis[] __asm__("D_L18_001679D0");
/* Preserve the state-4 frame conversion and the state-7 raw-frame arithmetic. */
void update_deplanetizer_gun_sequence_effects(struct Moby *moby) __asm__("FUN_L18_002f88e8");

void update_deplanetizer_gun_sequence_effects(struct Moby *moby) {
    enum {
        INITIALIZE_EFFECT_TIMING = 0,
        UPDATE_SEQUENCE_EFFECTS = 1
    };
    union {
        EffectRadii radii;
        struct {
            Vec4 pos, velocity;
        } vectors;
    } scratch;
    Vec4           direction, interpolated_joint_zero_position, interpolated_joint_two_position;
    struct Moby   *target;
    float          radius, numerator, denominator, fraction, saved;
    float         *data;
    unsigned char *effect;
    int            i, n;

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
                for (target = l18_moby_list_head; target; target = target->next) {
                    if (target->oclass == 0x59B) {
                        l18_deplanetizer_class_059b_glow_moby = target;
                        break;
                    }
                }
            }
        }
        if (scale_game_frames(500) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n <= scale_game_frames(860)) {
            int i;
            scratch.radii = l18_deplanetizer_object_trail_base_radii;
            for (i = 3; i < 8; i++) {
                if (D_L18_0016D260_update.objects[i]) {
                    moby_point_position(D_L18_0016D260_update.objects[i], 0, &direction);
                    radius = scratch.radii.radius[i - 3];
                    if (i != 3 || D_L18_0016D260_update.n < scale_game_frames(645) ||
                        scale_game_frames(736) < D_L18_0016D260_update.n) {
                        if (i == 5 && scale_game_frames(645) <= D_L18_0016D260_update.n &&
                            D_L18_0016D260_update.n <= scale_game_frames(730))
                            radius = 300000.0f;
                        draw_trail(&direction, &direction, D_L18_0015F580, radius, radius * 0.4f);
                    }
                }
            }
        }
        if (D_L18_0016D260_update.n == scale_game_frames(706)) {
            struct Moby *target = D_L18_0016D260_update.objects[7];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target,
                                      &scratch.vectors.velocity,
                                      &scratch.vectors.pos,
                                      0.0f,
                                      0.0f,
                                      50,
                                      25,
                                      25,
                                      8.0f,
                                      4.0f,
                                      9.0f,
                                      1.0f,
                                      -1,
                                      30.0f,
                                      0,
                                      15,
                                      -1,
                                      0);
            }
        }
        if (D_L18_0016D260_update.n == scale_game_frames(710)) {
            struct Moby *target = D_L18_0016D260_update.objects[4];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target,
                                      &scratch.vectors.velocity,
                                      &scratch.vectors.pos,
                                      0.0f,
                                      0.0f,
                                      50,
                                      25,
                                      25,
                                      8.0f,
                                      4.0f,
                                      9.0f,
                                      1.0f,
                                      -1,
                                      30.0f,
                                      0,
                                      15,
                                      -1,
                                      0);
            }
        }
        if (D_L18_0016D260_update.n == scale_game_frames(720)) {
            struct Moby *target = D_L18_0016D260_update.objects[6];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target,
                                      &scratch.vectors.velocity,
                                      &scratch.vectors.pos,
                                      0.0f,
                                      0.0f,
                                      50,
                                      25,
                                      25,
                                      8.0f,
                                      4.0f,
                                      9.0f,
                                      1.0f,
                                      -1,
                                      30.0f,
                                      0,
                                      15,
                                      -1,
                                      0);
            }
        }
        if (D_L18_0016D260_update.n == scale_game_frames(730)) {
            struct Moby *target = D_L18_0016D260_update.objects[5];
            if (target) {
                moby_point_position(target, 0, &scratch.vectors.pos);
                clear_u64_value(&scratch.vectors.velocity);
                scratch.vectors.velocity.f[2] = 1.0f;
                func_L00_0025F4A8_alt(target,
                                      &scratch.vectors.velocity,
                                      &scratch.vectors.pos,
                                      0.0f,
                                      0.0f,
                                      50,
                                      25,
                                      25,
                                      8.0f,
                                      4.0f,
                                      9.0f,
                                      1.0f,
                                      -1,
                                      30.0f,
                                      0,
                                      15,
                                      -1,
                                      0);
            }
        }
        if (D_L18_0016D260_update.n == scale_game_frames(748)) {
            scratch.vectors.pos.q = 0;
            scratch.vectors.pos.f[0] = 634.556030273f;
            scratch.vectors.pos.f[1] = 454.162994385f;
            scratch.vectors.pos.f[2] = 102.999000549f;
            scratch.vectors.pos.f[3] = 1.0f;
            clear_u64_value(&scratch.vectors.velocity);
            scratch.vectors.velocity.f[2] = 1.0f;
            func_L00_0025F4A8_alt(moby,
                                  &scratch.vectors.velocity,
                                  &scratch.vectors.pos,
                                  0.0f,
                                  0.0f,
                                  300,
                                  200,
                                  200,
                                  12.0f,
                                  6.0f,
                                  9.0f,
                                  1.0f,
                                  -1,
                                  60.0f,
                                  0,
                                  60,
                                  -1,
                                  0);
        }
        if (scale_game_frames(deplanetizer_glow_alpha_ramp_start_frame_gp) <=
                D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n <
                scale_game_frames(deplanetizer_glow_alpha_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(
                D_L18_0016D260_update.n -
                scale_game_frames(deplanetizer_glow_alpha_ramp_start_frame_gp));
            denominator = convert_integer_to_float(
                scale_game_frames(deplanetizer_glow_alpha_ramp_end_frame_gp -
                                  deplanetizer_glow_alpha_ramp_start_frame_gp));
            numerator = numerator / denominator;
            deplanetizer_glow_quad_alpha = truncate_float_to_s32(numerator * 255.0f);
            enqueue_callback_list_1_overlay(draw_moby_two_tint_glow_quads,
                                            l18_deplanetizer_class_059b_glow_moby);
        }
        if (scale_game_frames(deplanetizer_three_quad_scale_ramp_start_frame_gp) <=
                D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n <
                scale_game_frames(deplanetizer_three_quad_scale_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(
                D_L18_0016D260_update.n -
                scale_game_frames(deplanetizer_three_quad_scale_ramp_start_frame_gp));
            denominator = convert_integer_to_float(
                scale_game_frames(deplanetizer_three_quad_scale_ramp_end_frame_gp -
                                  deplanetizer_three_quad_scale_ramp_start_frame_gp));
            numerator = numerator / denominator;
            numerator *= 200.0f;
            numerator += 0.25f;
            deplanetizer_three_quad_local_x_scale = numerator;
            enqueue_callback_list_1_overlay(draw_moby_three_scaled_two_tint_quads,
                                            l18_deplanetizer_class_059b_glow_moby);
        }
        if (scale_game_frames(deplanetizer_camera_quad_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < scale_game_frames(deplanetizer_camera_quad_end_frame_gp)) {
            enqueue_callback_list_1_overlay(draw_three_scaled_camera_facing_quads,
                                            D_L18_0016D260_update.objects[0]);
            if (D_L18_0016D260_update.n >=
                scale_game_frames(deplanetizer_camera_quad_scale_ramp_end_frame_gp)) {
                deplanetizer_camera_quad_base_scale = 10.0f;
            } else {
                numerator = convert_integer_to_float(
                    D_L18_0016D260_update.n -
                    scale_game_frames(deplanetizer_camera_quad_start_frame_gp));
                denominator = convert_integer_to_float(
                    scale_game_frames(deplanetizer_camera_quad_scale_ramp_end_frame_gp -
                                      deplanetizer_camera_quad_start_frame_gp));
                numerator = numerator / denominator;
                deplanetizer_camera_quad_base_scale =
                    deplanetizer_camera_quad_scale_ramp_start_value_gp +
                    (deplanetizer_camera_quad_scale_ramp_end_value_gp -
                     deplanetizer_camera_quad_scale_ramp_start_value_gp) *
                        numerator;
            }
        }
    }
    if (D_L18_0016D260_update.state == 7) {
        if (D_L18_0016D260_update.n == 1) {
            deplanetizer_glow_alpha_ramp_start_frame_gp =
                deplanetizer_state_7_glow_alpha_ramp_start_frame_gp;
            deplanetizer_glow_alpha_ramp_end_frame_gp =
                deplanetizer_state_7_glow_alpha_ramp_end_frame_gp;
            deplanetizer_three_quad_scale_ramp_start_frame_gp =
                deplanetizer_state_7_three_quad_scale_ramp_start_frame_gp;
            deplanetizer_three_quad_scale_ramp_end_frame_gp =
                deplanetizer_state_7_three_quad_scale_ramp_end_frame_gp;
            deplanetizer_camera_quad_start_frame_gp =
                deplanetizer_state_7_camera_quad_start_frame_gp;
            deplanetizer_camera_quad_scale_ramp_end_frame_gp =
                deplanetizer_state_7_camera_quad_scale_ramp_end_frame_gp;
            {
                struct Moby *target;
                for (target = l18_moby_list_head; target; target = target->next) {
                    if (target->oclass == 0x59B) {
                        l18_deplanetizer_class_059b_glow_moby = target;
                        break;
                    }
                }
            }
        }
        if (scale_game_frames(deplanetizer_glow_alpha_ramp_start_frame_gp) <=
                D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n <
                scale_game_frames(deplanetizer_glow_alpha_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n -
                                                 deplanetizer_glow_alpha_ramp_start_frame_gp);
            denominator = convert_integer_to_float(deplanetizer_glow_alpha_ramp_end_frame_gp -
                                                   deplanetizer_glow_alpha_ramp_start_frame_gp);
            numerator = numerator / denominator;
            deplanetizer_glow_quad_alpha = truncate_float_to_s32(numerator * 255.0f);
            enqueue_callback_list_1_overlay(draw_moby_two_tint_glow_quads,
                                            l18_deplanetizer_class_059b_glow_moby);
        }
        if (scale_game_frames(deplanetizer_three_quad_scale_ramp_start_frame_gp) <=
                D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n <
                scale_game_frames(deplanetizer_three_quad_scale_ramp_end_frame_gp)) {
            numerator = convert_integer_to_float(D_L18_0016D260_update.n -
                                                 deplanetizer_three_quad_scale_ramp_start_frame_gp);
            denominator =
                convert_integer_to_float(deplanetizer_three_quad_scale_ramp_end_frame_gp -
                                         deplanetizer_three_quad_scale_ramp_start_frame_gp);
            numerator = numerator / denominator;
            numerator *= 200.0f;
            numerator += 0.25f;
            deplanetizer_three_quad_local_x_scale = numerator;
            enqueue_callback_list_1_overlay(draw_moby_three_scaled_two_tint_quads,
                                            l18_deplanetizer_class_059b_glow_moby);
        }
        if (scale_game_frames(deplanetizer_camera_quad_start_frame_gp) <= D_L18_0016D260_update.n &&
            D_L18_0016D260_update.n < scale_game_frames(deplanetizer_camera_quad_end_frame_gp)) {
            enqueue_callback_list_1_overlay(draw_three_scaled_camera_facing_quads,
                                            D_L18_0016D260_update.objects[0]);
            if (D_L18_0016D260_update.n >= deplanetizer_camera_quad_scale_ramp_end_frame_gp) {
                deplanetizer_camera_quad_base_scale = 10.0f;
            } else {
                numerator = convert_integer_to_float(D_L18_0016D260_update.n -
                                                     deplanetizer_camera_quad_start_frame_gp);
                denominator =
                    convert_integer_to_float(deplanetizer_camera_quad_scale_ramp_end_frame_gp -
                                             deplanetizer_camera_quad_start_frame_gp);
                numerator = numerator / denominator;
                deplanetizer_camera_quad_base_scale =
                    deplanetizer_camera_quad_scale_ramp_start_value_gp +
                    (deplanetizer_camera_quad_scale_ramp_end_value_gp -
                     deplanetizer_camera_quad_scale_ramp_start_value_gp) *
                        numerator;
            }
        }
    }
    if (D_L18_0016D260_update.state == 1 && D_L18_0016D260_update.objects[3])
        enqueue_callback_list_1_overlay(draw_moby_color_cycling_bone_glows,
                                        D_L18_0016D260_update.objects[3]);
    if (D_L18_0016D260_update.state == 5 && scale_game_frames(138) <= D_L18_0016D260_update.n &&
        D_L18_0016D260_update.n <= scale_game_frames(264)) {
        struct Moby *target = D_L18_0016D260_update.objects[2];
        if (target) {
            if (scale_game_frames(140) < D_L18_0016D260_update.n) {
                int i;
                moby_point_position(target, 0, &scratch.vectors.pos);
                moby_point_position(target, 2, &scratch.vectors.velocity);
                for (i = 0; i < 12; i++) {
                    fraction = convert_integer_to_float(i) / 12.0f;
                    lerp_vector(&interpolated_joint_zero_position,
                                &scratch.vectors.pos,
                                &D_L18_00162530,
                                fraction);
                    lerp_vector(&interpolated_joint_two_position,
                                &scratch.vectors.velocity,
                                &D_L18_00162540,
                                fraction);
                    spawn_deplanetizer_gun_trail_effects(&interpolated_joint_zero_position,
                                                         &interpolated_joint_zero_position,
                                                         D_L18_0015F580,
                                                         deplanetizer_joint_trail_base_size_gp,
                                                         deplanetizer_joint_trail_base_size_gp *
                                                             0.4f);
                    spawn_deplanetizer_gun_trail_effects(&interpolated_joint_two_position,
                                                         &interpolated_joint_two_position,
                                                         D_L18_0015F580,
                                                         deplanetizer_joint_trail_base_size_gp,
                                                         deplanetizer_joint_trail_base_size_gp *
                                                             0.4f);
                }
                qcopy(&D_L18_00162530, &scratch.vectors.pos);
                qcopy(&D_L18_00162540, &scratch.vectors.velocity);
            } else {
                moby_point_position(target, 0, &D_L18_00162530);
                moby_point_position(target, 2, &D_L18_00162540);
            }
        }
    }
    if (D_L18_0016D260_update.state == 1 && scale_game_frames(446) <= D_L18_0016D260_update.n &&
        D_L18_0016D260_update.n < scale_game_frames(502)) {
        int          color, color2;
        struct Moby *target = D_L18_0016D260_update.objects[4];
        if (target) {
            data = (float *)moby->pvars;
            data[4] = 0.0f;
            moby_point_position(target, 3, &scratch.vectors.pos);
            moby_point_position(target, 10, &scratch.vectors.velocity);
            fast_vec_sub(&direction, &scratch.vectors.velocity, &scratch.vectors.pos);
            fast_vec_normalize(&moby->rot_row1, &direction, 1.0f);
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
        if (scale_game_frames(586) <= D_L18_0016D260_update.n)
            goto burst;
    }
    if (D_L18_0016D260_update.objects[2])
        enqueue_callback_list_1_overlay(draw_moby_color_cycling_bone_glows,
                                        D_L18_0016D260_update.objects[2]);
burst:
    if (D_L18_0016D260_update.state == 3 && D_L18_0016D260_update.n == scale_game_frames(586)) {
        char *matrix = deplanetizer_camera_rotation_basis;
        transform_vector_by_basis(
            &scratch.vectors.pos, &deplanetizer_burst_local_spawn_offset_x_gp, matrix);
        fast_vec_add(&scratch.vectors.pos, &scratch.vectors.pos, matrix - 0x210);
        qcopy(&scratch.vectors.velocity, matrix + 0x10);
        for (i = 0; i < 32; i++) {
            build_look_at_matrix(&direction,
                                 &scratch.vectors.velocity,
                                 deplanetizer_camera_rotation_basis,
                                 convert_integer_to_float(i) * 0.19634955f);
            fast_vec_normalize(
                &direction, &direction, deplanetizer_burst_velocity_scale_gp * D_L18_0015ED60);
            n = random_integer_below(16);
            if (random_integer_below(2))
                n = -n;
            effect = (unsigned char *)func_L00_0026DEA0_c(&scratch.vectors.pos,
                                                          n,
                                                          &direction,
                                                          0.0f,
                                                          1.0f,
                                                          1.01f,
                                                          deplanetizer_burst_particle_size_gp,
                                                          0x7F7F7F7F);
            if (effect) {
                unsigned char *data = effect + 0x20;
                *(short *)(effect + 10) = scale_game_frames(60);
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
    float         m[4][4];
    int           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk_9780;

typedef struct {
    char pad[0x30];
    int  state;
    int  n;
} ML_9780;

extern ML_9780 D_L18_0016D260_9780 __asm__("D_L18_0016D260");
extern void    draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

#include "rnc/overlay/l18/mobys/cutscene_effects_data/draw_three_scaled_camera_facing_quads_2.h"

extern volatile float l18_deplanetizer_camera_quad_local_offset_y __asm__("D_L18_00162554");
extern volatile float D_L18_00162558;

#include "rnc/overlay/l18/mobys/cutscene_effects_data/draw_three_scaled_camera_facing_quads.h"

extern int deplanetizer_camera_quad_first_color_gp __asm__("D_L18_001624F8") __attribute__((sda));
extern int deplanetizer_camera_quad_smaller_color_gp __asm__("D_L18_001624FC") __attribute__((sda));

float D_L18_001DFB90[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

void draw_three_scaled_camera_facing_quads(void) {
    Pk_9780 pk;
    float   quad_transform[4][4];
    float   up[4];
    int     i;
    int     color;
    float   tilt;
    char   *target;

    if (D_L18_0016D260_9780.n == scale_game_frames(deplanetizer_camera_quad_start_frame_gp + 1)) {
        deplanetizer_camera_quad_local_offset_x = 1.0f;
        l18_deplanetizer_camera_quad_local_offset_y = 0.0f;
        if (D_L18_0016D260_9780.state == 7) {
            tilt = -0.15f;
        } else {
            tilt = 0.15f;
        }
        D_L18_00162558 = tilt;
        deplanetizer_camera_quad_local_offset_w = 1.0f;
    }
    pk.b = get_effect_texture(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    transform_vector_by_basis(quad_transform[3],
                              &deplanetizer_camera_quad_local_offset_x,
                              deplanetizer_camera_rotation_basis);
    target = deplanetizer_camera_rotation_basis;
    target -= 0x210;
    fast_vec_add(quad_transform[3], quad_transform[3], target);
    fast_vec_sub(quad_transform[0], quad_transform[3], target);
    fast_vec_normalize(quad_transform[0], quad_transform[0], 1.0f);
    *(u128_9780 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    fast_vec_cross(quad_transform[1], quad_transform[0], up);
    fast_vec_normalize(quad_transform[1], quad_transform[1], 1.0f);
    fast_vec_cross(quad_transform[2], quad_transform[1], quad_transform[0]);
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
        fast_vec_scale(pk.m[i], D_L18_001DFB90[i], deplanetizer_camera_quad_base_scale);
        transform_vector(pk.m[i], pk.m[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
    color = deplanetizer_camera_quad_smaller_color_gp;
    pk.col[3] = color;
    pk.col[2] = color;
    pk.col[1] = color;
    pk.col[0] = color;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.m[i], D_L18_001DFB90[i], deplanetizer_camera_quad_base_scale * 0.75f);
        transform_vector(pk.m[i], pk.m[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.m[i], D_L18_001DFB90[i], deplanetizer_camera_quad_base_scale * 0.5f);
        transform_vector(pk.m[i], pk.m[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FAEF8), where it is exact; names translated to the US level program. */

extern float D_L18_00162454;
extern float D_L18_00162450;
extern int   D_L18_00162458;
extern int   D_L18_0016245C;
extern int   D_L18_00162460;
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern float D_L18_00162454;
extern float D_L18_00162450;

extern short deplanetizer_bone_glow_offset_scale_gp __asm__("D_L18_00162500") __attribute__((sda));

void draw_moby_color_cycling_bone_glows(void *moby) {
    char  glow_position[16];
    char  bone_transform_rows0_1[32];
    char  bone_transform_row2[16];
    char  bone_world_position[16];
    char  scaled_bone_offset[16];
    int   color_cycle_frames = scale_game_frames(D_L18_00162460);
    float color_cycle_value = convert_integer_to_float(game_frame % color_cycle_frames);
    int   tweened_color;
    color_cycle_value = color_cycle_value / convert_integer_to_float(color_cycle_frames);
    color_cycle_value = color_cycle_value * 6.18318f;
    color_cycle_value = fast_sin(color_cycle_value - 3.14159f);
    tweened_color =
        fast_tween_color(D_L18_00162458, D_L18_0016245C, color_cycle_value * 0.5f + 0.5f);
    build_moby_bone_transform(moby, 1, bone_transform_rows0_1);
    fast_vec_scale(
        scaled_bone_offset, bone_transform_row2, *(float *)&deplanetizer_bone_glow_offset_scale_gp);
    fast_vec_sub(glow_position, bone_world_position, scaled_bone_offset);
    draw_offset_textured_effect_quad(glow_position, tweened_color, D_L18_00162450, D_L18_00162454);
    build_moby_bone_transform(moby, 2, bone_transform_rows0_1);
    fast_vec_scale(
        scaled_bone_offset, bone_transform_row2, *(float *)&deplanetizer_bone_glow_offset_scale_gp);
    fast_vec_sub(glow_position, bone_world_position, scaled_bone_offset);
    draw_offset_textured_effect_quad(glow_position, tweened_color, D_L18_00162450, D_L18_00162454);
}

/* Draws the moby's two glow quads facing the camera, in the inner and outer tints. */
typedef unsigned int u128_9c20 __attribute__((mode(TI)));

typedef struct {
    float         m[4][4];
    int           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk_9c20;

typedef struct {
    char pad[0x34];
    int  n;
} ML_9c20;

extern ML_9c20 D_L18_0016D260_9c20 __asm__("D_L18_0016D260");
extern char    D_L18_001677C0_9c20[] __asm__("D_L18_001677C0");

extern int deplanetizer_glow_first_quad_base_tint_gp __asm__("D_L18_00162504") __attribute__((sda));
extern float deplanetizer_glow_first_quad_scale_gp __asm__("D_L18_00162508") __attribute__((sda));
extern float deplanetizer_glow_second_quad_scale_gp __asm__("D_L18_0016250C") __attribute__((sda));
extern int   deplanetizer_glow_second_quad_base_tint_gp __asm__("D_L18_00162510")
    __attribute__((sda));

void draw_moby_two_tint_glow_quads(struct Moby *m) {
    Pk_9c20 pk;
    float   quad_transform[4][4];
    float   up[4];
    int     c;
    int     second_quad_color;
    int     i;
    if (D_L18_0016D260_9c20.n ==
        scale_game_frames(deplanetizer_glow_alpha_ramp_start_frame_gp + 1)) {
        deplanetizer_glow_local_offset_z = -45.0f;
        deplanetizer_glow_local_offset_w = 1.0f;
        deplanetizer_glow_local_offset_x = 0;
        deplanetizer_glow_local_offset_y = 0;
    }
    pk.b = get_effect_texture(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    transform_vector_by_basis(quad_transform[3], &deplanetizer_glow_local_offset_x, &m->rot_row0);
    fast_vec_add(quad_transform[3], quad_transform[3], &m->pos);
    fast_vec_sub(quad_transform[0], quad_transform[3], D_L18_001677C0_9c20);
    fast_vec_normalize(quad_transform[0], quad_transform[0], 1.0f);
    *(u128_9c20 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    fast_vec_cross(quad_transform[1], quad_transform[0], up);
    fast_vec_normalize(quad_transform[1], quad_transform[1], 1.0f);
    fast_vec_cross(quad_transform[2], quad_transform[1], quad_transform[0]);
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
        fast_vec_scale(pk.m[i], D_L18_001DFB90[i], deplanetizer_glow_first_quad_scale_gp);
        transform_vector(pk.m[i], pk.m[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
    second_quad_color =
        deplanetizer_glow_second_quad_base_tint_gp | (deplanetizer_glow_quad_alpha << 24);
    pk.col[3] = second_quad_color;
    pk.col[2] = second_quad_color;
    pk.col[1] = second_quad_color;
    pk.col[0] = second_quad_color;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.m[i], D_L18_001DFB90[i], deplanetizer_glow_second_quad_scale_gp);
        transform_vector(pk.m[i], pk.m[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
}

extern float deplanetizer_first_quad_local_z_scale_gp __asm__("D_L18_00162514")
    __attribute__((sda));
extern float deplanetizer_second_quad_local_z_scale_gp __asm__("D_L18_00162518")
    __attribute__((sda));
extern float deplanetizer_third_quad_local_z_scale_gp __asm__("D_L18_0016251C")
    __attribute__((sda));
extern float deplanetizer_three_quad_local_vertices[4][4] __asm__("D_L18_001DFBD0");

void draw_moby_three_scaled_two_tint_quads(struct Moby *m) {
    Pk_9c20 pk;
    float   quad_transform[4][4];
    float   up[4];
    Vec4    vertices[4];
    int     i = 0;
    int     color;

    pk.b = get_effect_texture(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    transform_vector_by_basis(quad_transform[3], &deplanetizer_glow_local_offset_x, &m->rot_row0);
    fast_vec_add(quad_transform[3], quad_transform[3], &m->pos);
    *(u128_9c20 *)up = 0;
    up[2] = -1.0f;
    up[3] = 1.0f;
    transform_vector_by_basis(quad_transform[0], up, &m->rot_row0);
    fast_vec_sub(quad_transform[1], quad_transform[3], D_L18_001677C0_9c20);
    fast_vec_cross(quad_transform[2], quad_transform[1], quad_transform[0]);
    fast_vec_normalize(quad_transform[2], quad_transform[2], 1.0f);
    fast_vec_cross(quad_transform[1], quad_transform[0], quad_transform[2]);

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
        qcopy_nc(&vertices[i], deplanetizer_three_quad_local_vertices[i]);
        {
            vertices[i].f[2] *= deplanetizer_first_quad_local_z_scale_gp;
            vertices[i].f[0] *= deplanetizer_three_quad_local_x_scale;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        transform_vector(pk.m[i], &vertices[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);

    pk.col[3] = deplanetizer_glow_second_quad_base_tint_gp | 0x7F000000;
    pk.col[2] = pk.col[3];
    pk.col[1] = pk.col[3];
    pk.col[0] = pk.col[3];
    i = 0;
    do {
        qcopy_nc(&vertices[i], deplanetizer_three_quad_local_vertices[i]);
        {
            vertices[i].f[2] *= deplanetizer_second_quad_local_z_scale_gp;
            vertices[i].f[0] *= deplanetizer_three_quad_local_x_scale;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        transform_vector(pk.m[i], &vertices[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);

    pk.col[3] = deplanetizer_glow_second_quad_base_tint_gp | 0x7F000000;
    pk.col[2] = pk.col[3];
    pk.col[1] = pk.col[3];
    pk.col[0] = pk.col[3];
    i = 0;
    do {
        qcopy_nc(&vertices[i], deplanetizer_three_quad_local_vertices[i]);
        {
            vertices[i].f[2] *= deplanetizer_third_quad_local_z_scale_gp;
            vertices[i].f[0] *= deplanetizer_three_quad_local_x_scale;
        }
        i++;
    } while (i < 4);
    for (i = 0; i < 4; i++) {
        transform_vector(pk.m[i], &vertices[i], quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FB6B0), where it is exact; names translated to the US level program. */

unsigned char *
FUN_L00_0026d000(void *pos, int n, void *vel, int color, float r, float lo, float hi, float size);
extern char *func_L00_0026DEA0_c(void *, int, void *, float, float, float, float, int) __asm__("FUN_L00_0026d000");

void spawn_deplanetizer_gun_trail_effects(void *first_position_source,
                                          void *second_position_source,
                                          void *effect_velocity_source,
                                          float first_effect_size,

                                          float second_effect_size) {
    char  first_effect_position[16];
    char  second_effect_position[16];
    int   i;
    char *random_magnitude_or_effect;
    char *effect_data;
    int   x;
    int   k2, k7f;
    qcopy(first_effect_position, first_position_source);
    qcopy(second_effect_position, second_position_source);
    for (i = 1; i >= 0; i--) {
        random_magnitude_or_effect = (char *)random_integer_below(16);
        random_magnitude_or_effect =
            func_L00_0026DEA0_c(first_effect_position,
                                random_integer_below(2) ? -(int)random_magnitude_or_effect
                                                        : (int)random_magnitude_or_effect,
                                effect_velocity_source,
                                0.0f,
                                1.0f,
                                0.9f,
                                first_effect_size,
                                0x7F204080);
        effect_data = random_magnitude_or_effect + 0x20;
        if (random_magnitude_or_effect) {
            *(short *)(random_magnitude_or_effect + 0xA) = scale_game_frames(15);
            random_magnitude_or_effect[9] = truncate_float_to_s32(4.0f) + 0x40;
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
        random_magnitude_or_effect = func_L00_0026DEA0_c(second_effect_position,
                                                         y,
                                                         effect_velocity_source,
                                                         0.0f,
                                                         1.0f,
                                                         0.97f,
                                                         second_effect_size,
                                                         0x7FFFFFFF);
        effect_data = random_magnitude_or_effect + 0x20;
        if (random_magnitude_or_effect) {
            random_magnitude_or_effect[9] = truncate_float_to_s32(4.0f) + 0x40;
            *(short *)(random_magnitude_or_effect + 0xA) = scale_game_frames(4);
            random_magnitude_or_effect[8] = random_integer_below(0xFF);
            *(int *)(effect_data + 4) = k2;
            effect_data[0xA] = k7f;
            effect_data[0xB] = random_magnitude_or_effect[0xA];
        }
    } while (--i >= 0);
}

struct Moby *l18_deplanetizer_class_059b_glow_moby __asm__("D_L18_00162528") = 0;

Vec4 D_L18_00162530 = {0};

Vec4 D_L18_00162540 = {0};

EffectRadii l18_deplanetizer_object_trail_base_radii __asm__("D_L18_001F2C20") = {
    {1.2e+06f, 5e+05f, 8e+05f, 4e+05f, 2e+05f}
};

float deplanetizer_three_quad_local_vertices[4][4] __asm__("D_L18_001DFBD0") = {
    {-1.0f, 0.0f,  1.0f, 1.0f},
    {-1.0f, 0.0f, -1.0f, 1.0f},
    { 1.0f, 0.0f,  1.0f, 1.0f},
    { 1.0f, 0.0f, -1.0f, 1.0f},
};

int deplanetizer_camera_quad_end_frame_gp __asm__("D_L18_001624A0") = 0x17c;
