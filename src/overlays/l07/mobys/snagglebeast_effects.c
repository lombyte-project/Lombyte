/* Level 07 snagglebeast effect helpers: timed point light, rumble, joint emission burst and clipped beam (snagglebeast.c declares them with other types, so they stay apart). */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/rendering/effect_texture.h"
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E820), where it is exact; names translated to the US level program. */
extern char D_L07_00166E40[];
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003134B8), where it is exact; names translated to the US level program. */
void rumble_by_distance(char *moby) __asm__("FUN_L07_003120d8");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313530), where it is exact; names translated to the US level program. */
extern char point_light_slot_table[] __asm__("D_L07_00180340");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");
void        update_timed_point_light_color(char *light_effect_state) __asm__("FUN_L07_00312150");

/* steps a hit-effect emitter: orient, spawn a puff, and reseed the timer */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313690), where it is exact; names translated to the US level program. */
extern int  FUN_0022da68(int, int, int);
extern int  FUN_L07_0030e370_c() __asm__("FUN_L07_0030e370");
extern void FUN_L00_001ff600(float, void *, void *);
extern void configure_timed_point_light(
    float, void *, void *, short, void *, short, unsigned int, int) __asm__("FUN_L07_00311eb8");
int          update_alternating_joint_emission_burst(void  *mp,
                                                     char  *emitter_state,
                                                     float *target_position) __asm__("FUN_L07_003122b0");
extern void  draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern int   rand(void) __asm__("FUN_001160d8");
extern s32   rand(void);
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313298), where it is exact; names translated to the US level program. */
extern int
allocate_packed_color_point_light_slot(float *, float, float, int) __asm__("FUN_L00_0023e5e0");

typedef struct EffData {
    char  pad0[0x140];
    float f140[4];
    float f150[3];
    float f15C;
} EffData;

typedef struct Packet {
    float        a[4];
    float        b[4];
    float        c[4];
    float        d[4];
    unsigned int col[4];
    float        uv[8];
    long         x120;
    long         x128;
    long         x130;
    long         x138;
} Packet;

extern int   D_L07_0015F5CC_lui __asm__("D_L07_0015F5CC");
extern int   FUN_001efa68_2cd2b0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void *FUN_L00_002729c8_2cd2b0(float        *pos,
                                     unsigned char a1,
                                     unsigned char a2,
                                     unsigned char a3,
                                     unsigned char a4,
                                     unsigned char a5,
                                     void         *a6,
                                     float         f0,
                                     float         f1) __asm__("FUN_L00_002729c8");
extern char  D_L07_00167050_2cd2b0[] __asm__("D_L07_00167050");
extern char  D_L07_00173EE0_2cd2b0[] __asm__("D_L07_00173EE0");
void draw_collision_clipped_moby_beam(struct Moby *effect_owner_moby) __asm__("FUN_L07_00312420");

void configure_timed_point_light(float        radius,
                                 void        *moby_,
                                 void        *obj_,
                                 short        pa,
                                 void        *effect_position_override,
                                 short        duration_frames,
                                 unsigned int color,

                                 int ca) {
    char *moby = moby_;
    char *light_effect_state = obj_;
    float effect_position[4];
    char *effect_slot_entry;
    int   y;
    if (effect_position_override == 0) {
        qcopy(effect_position, moby + 0x10);
    } else {
        qcopy(effect_position, effect_position_override);
    }
    if (*(short *)(light_effect_state + 0x280) == -1) {
        if (duration_frames > 0) {
            if (radius == -1.0f) {
                radius = 15.0f;
            }
            if ((int)color == -1) {
                color = 0x80004080U;
            }
            if (ca == -1) {
                ca = 0;
            }
            *(short *)(light_effect_state + 0x280) =
                allocate_packed_color_point_light_slot(effect_position, radius, 0.0f, color);
            *(short *)(light_effect_state + 0x284) = 0;
        }
    }
    if (*(short *)(light_effect_state + 0x280) != -1) {
        effect_slot_entry = point_light_slot_table + (*(short *)(light_effect_state + 0x280) << 5);
        if (*(short *)(light_effect_state + 0x282) == pa &&
                (pa == -1 || *(short *)(light_effect_state + 0x284) == 0) ||
            *(short *)(light_effect_state + 0x282) != pa) {
            if (duration_frames != -1) {
                *(short *)(light_effect_state + 0x284) = duration_frames;
                *(short *)(light_effect_state + 0x286) = duration_frames;
            }
            if (radius != -1.0f) {
                *(float *)(effect_slot_entry + 0x1C) = radius;
            }
        } else {
            if (duration_frames != -1) {
                y = *(short *)(light_effect_state + 0x286) * 7 / 8;
                if (!(y < *(short *)(light_effect_state + 0x284))) {
                    *(short *)(light_effect_state + 0x284) = y;
                }
                if (radius != -1.0f) {
                    *(float *)(effect_slot_entry + 0x1C) = radius;
                }
            } else if (radius != -1.0f) {
                *(float *)(effect_slot_entry + 0x1C) = radius;
            }
        }
        if ((int)color != -1) {
            *(int *)(light_effect_state + 0x288) = color;
            *(float *)effect_slot_entry = convert_integer_to_float(color & 0xFF) * 0.0078125f;
            *(float *)(effect_slot_entry + 4) =
                convert_integer_to_float(((int)color >> 8) & 0xFF) * 0.0078125f;
            *(float *)(effect_slot_entry + 8) =
                convert_integer_to_float(((int)color >> 16) & 0xFF) * 0.0078125f;
        }
        if (ca != -1) {
            *(int *)(light_effect_state + 0x28C) = ca;
        }
        if (effect_position_override != 0) {
            qcopy(effect_slot_entry + 0x10, effect_position_override);
        }
        if (pa != -1) {
            *(short *)(light_effect_state + 0x282) = pa;
        }
    }
}

void rumble_by_distance(char *moby) {
    float distance = fast_vec_dist(moby + 0x10, D_L07_00166E40);
    char *state;
    if (distance > 80.0f)
        distance = 80.0f;
    state = D_L07_00166E40 - 0x140;
    *(int *)(state + 0x168) = 0x14;
    *(float *)(state + 0x160) = ((80.0f - distance) / 80.0f) * 0.1f;
}

void update_timed_point_light_color(char *light_effect_state) {
    int    color_phase_threshold;
    float  phase_remaining_fraction;
    int    interpolated_packed_color;
    float *rgb_destination;
    if (fast_dec_timer_s16(light_effect_state + 0x284)) {
        if (*(short *)(light_effect_state + 0x280) != -1) {
            release_point_light_slot(*(short *)(light_effect_state + 0x280));
            *(short *)(light_effect_state + 0x280) = -1;
            *(short *)(light_effect_state + 0x282) = -1;
        }
    } else if (*(short *)(light_effect_state + 0x280) != -1) {
        color_phase_threshold = (*(short *)(light_effect_state + 0x286) * 7) / 8;
        if (color_phase_threshold < *(short *)(light_effect_state + 0x284)) {
            phase_remaining_fraction = convert_integer_to_float(
                *(short *)(light_effect_state + 0x284) - color_phase_threshold);
            phase_remaining_fraction /= convert_integer_to_float(
                *(short *)(light_effect_state + 0x286) - color_phase_threshold);
            interpolated_packed_color = fast_tween_color(
                0, *(int *)(light_effect_state + 0x288), 1.0f - phase_remaining_fraction);
        } else {
            phase_remaining_fraction =
                convert_integer_to_float(*(short *)(light_effect_state + 0x284));
            phase_remaining_fraction /= convert_integer_to_float(color_phase_threshold);
            interpolated_packed_color = fast_tween_color(*(int *)(light_effect_state + 0x288),
                                                         *(int *)(light_effect_state + 0x28C),
                                                         1.0f - phase_remaining_fraction);
        }
        rgb_destination =
            (float *)(point_light_slot_table + *(short *)(light_effect_state + 0x280) * 32);
        rgb_destination[0] =
            convert_integer_to_float(interpolated_packed_color & 0xFF) * 0.0078125f;
        rgb_destination[1] =
            convert_integer_to_float((interpolated_packed_color >> 8) & 0xFF) * 0.0078125f;
        rgb_destination[2] =
            convert_integer_to_float((interpolated_packed_color >> 16) & 0xFF) * 0.0078125f;
    }
}

int update_alternating_joint_emission_burst(void *mp, char *emitter_state, float *target_position) {
    struct Moby *moby = mp;
    float        A[4];
    float        B[4];
    if (fast_dec_timer((int *)(emitter_state + 0x160))) {
        moby_point_position(moby, *(int *)(emitter_state + 0x164) & 1, A);
        fast_vec_sub(B, target_position, A);
        FUN_L00_001ff600(10.7f, B, B);
        B[2] += 0.5f;
        if (B[2] > 0.0f)
            B[2] = 0.0f;
        fast_vec_normalize(B, B, frame_time * 9.0f);
        FUN_L07_0030e370_c(A, B, moby);
        FUN_0022da68(0, 0, (int)moby);
        configure_timed_point_light(
            10.0f, moby, emitter_state, 0, A, scale_game_frames(0x1E), 0x800060FF, 0);
        *(int *)(emitter_state + 0x160) = scale_game_frames(9);
        if (++*(int *)(emitter_state + 0x164) >= 0x15) {
            *(short *)(emitter_state + 0x198) = scale_game_frames(0x23);
            *(int *)(emitter_state + 0x160) = scale_game_frames(0x1E);
            return 1;
        }
    }
    return moby->state;
}

void draw_collision_clipped_moby_beam(struct Moby *effect_owner_moby) {
    float line_endpoint[4], endpoint_work_vector[4], quad_side_offset[4], attachment_position[4],
        endpoint_to_attachment[4], oscillating_reference_position[4], effect_sample_position[4],
        effect_sample_step[4], sample_probe_offset[4], ii[4], jj[4];
    float    frame_phase_work_value, oscillation_work_value;
    int      flag = 1;
    EffData *moby_effect_data = ((EffData *)effect_owner_moby->pvars);
    int      k;
    Packet   pkt;

    frame_phase_work_value = convert_integer_to_float(D_L07_0015F5CC_lui);
    qcopy(oscillating_reference_position, moby_effect_data->f140);
    oscillation_work_value = fast_cos(normalize_angle(frame_phase_work_value / 11.0f));
    frame_phase_work_value = frame_phase_work_value / 10.0f;
    oscillation_work_value =
        normalize_angle(frame_phase_work_value + oscillation_work_value * 1.5707964f);
    oscillating_reference_position[0] =
        oscillating_reference_position[0] + fast_sin(oscillation_work_value);
    moby_point_position(effect_owner_moby, 2, attachment_position);
    fast_vec_sub(endpoint_work_vector, oscillating_reference_position, attachment_position);
    fast_vec_normalize(endpoint_work_vector, endpoint_work_vector, 30.0f);
    fast_vec_add(line_endpoint, oscillating_reference_position, endpoint_work_vector);
    if (FUN_001efa68_2cd2b0(attachment_position, line_endpoint, 2, effect_owner_moby, 0)) {
        qcopy(line_endpoint, D_L07_00173EE0_2cd2b0);
    } else {
        flag = 0;
    }
    fast_vec_sub(endpoint_to_attachment, attachment_position, line_endpoint);
    fast_vec_cross(quad_side_offset, D_L07_00167050_2cd2b0, endpoint_to_attachment);
    fast_vec_normalize(quad_side_offset, quad_side_offset, 0.25f);
    fast_vec_add(endpoint_work_vector, quad_side_offset, line_endpoint);
    if (flag) {
        FUN_L00_002729c8_2cd2b0(line_endpoint,
                                0,
                                scale_game_frames((rand() & 1) + 0x2D),
                                scale_game_frames((rand() & 7) + 0x37),
                                1,
                                scale_game_frames((rand() & 3) + 0xD),
                                effect_owner_moby,
                                1.0f,
                                1.0f);
        if (moby_effect_data->f15C != 0.0f) {
            fast_vec_sub(effect_sample_step, line_endpoint, moby_effect_data->f150);
            fast_vec_normalize(effect_sample_step, effect_sample_step, 0.2f);
            qcopy(effect_sample_position, moby_effect_data->f150);
            for (k = 0; k < 4; k++) {
                fast_vec_add(effect_sample_position, effect_sample_position, effect_sample_step);
                fast_vec_sub(sample_probe_offset, effect_sample_position, attachment_position);
                fast_vec_normalize(sample_probe_offset, sample_probe_offset, 5.0f);
                fast_vec_sub(ii, effect_sample_position, sample_probe_offset);
                fast_vec_add(jj, effect_sample_position, sample_probe_offset);
                if (FUN_001efa68_2cd2b0(ii, jj, 2, effect_owner_moby, 0)) {
                    qcopy(effect_sample_position, D_L07_00173EE0_2cd2b0);
                }
                if (k == 2) {
                    FUN_L00_002729c8_2cd2b0(effect_sample_position,
                                            0,
                                            scale_game_frames((rand() & 1) + 0x2D),
                                            scale_game_frames((rand() & 7) + 0x37),
                                            1,
                                            scale_game_frames((rand() & 3) + 0xD),
                                            effect_owner_moby,
                                            1.0f,
                                            1.0f);
                } else {
                    FUN_L00_002729c8_2cd2b0(effect_sample_position,
                                            1,
                                            scale_game_frames(0x2D),
                                            scale_game_frames(0x37),
                                            1,
                                            scale_game_frames(0xD),
                                            effect_owner_moby,
                                            1.0f,
                                            1.0f);
                }
            }
        }
        qcopy(moby_effect_data->f150, line_endpoint);
        moby_effect_data->f15C = 1.0f;
    } else {
        moby_effect_data->f15C = 0.0f;
    }
    pkt.x128 = get_effect_texture(0x3B);
    pkt.col[0] = pkt.col[1] = pkt.col[2] = pkt.col[3] = 0x80808080;
    pkt.x130 = 0xFF9000000260;
    pkt.x120 = 4;
    pkt.x138 = 0x8000000048;
    pkt.uv[4] = pkt.uv[0] = 0.0f;
    pkt.uv[6] = pkt.uv[2] = 1.0f;
    pkt.uv[3] = pkt.uv[1] = 0.0f;
    pkt.uv[7] = pkt.uv[5] = 1.0f;
    qcopy_nc(pkt.a, attachment_position);
    fast_vec_add(pkt.b, pkt.a, quad_side_offset);
    qcopy(pkt.c, line_endpoint);
    qcopy(pkt.d, endpoint_work_vector);
    draw_geometry_quad(&pkt, 0, 1);
}
