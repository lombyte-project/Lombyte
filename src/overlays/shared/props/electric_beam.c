/* Electric beam (class 0x4C8, Oltanis, Quartu and Drek's fleet): lightning that pulses on a timer along a cuboid's axis, branches, and hurts and pushes back what it crosses. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/random.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/world/collision/collision_line.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00302A58), where it is exact; names translated to the US level program. */

typedef struct {
    int   w0;
    short s4, s6, s8, sA;
    int   wC;
    short s10, s12;
    char  pad14[8];
    float radius; /* 0x1C: beam half width */
    char  pad20[0x604];
    float f624;
    char  pad628[8];
    Vec4  end_a; /* 0x630: end of the line along +axis 1 */
    Vec4  end_b; /* 0x640: end of the line along -axis 1 */
} D1224;

#include "rnc/overlay/cuboid.h"

extern int  D_L14_001620C8;   /* no foreign declaration */
extern int  D_L14_001620D8_t; /* no foreign declaration */
extern void start_electric_beam_pulse(char *) __asm__("FUN_L14_003017f8");
void        update_electric_beam_animation(char *moby) __asm__("FUN_L14_00301de8");

extern void update_electric_beam_animation(char *) __asm__("FUN_L14_00301de8");
extern void draw_electric_beam(char *) __asm__("FUN_L14_00301fa8");
void        apply_electric_beam_damage(char *self) __asm__("FUN_L14_00302538");

extern void apply_electric_beam_damage(char *) __asm__("FUN_L14_00302538");
extern int  D_L14_001620CC_gp __asm__("D_L14_001620CC") __attribute__((sda));
extern int  D_L14_001620B8_gp __asm__("D_L14_001620B8") __attribute__((sda));
extern int  D_L14_001620D0_gp __asm__("D_L14_001620D0") __attribute__((sda));
extern s32  D_L14_001620C8_gp __asm__("D_L14_001620C8") __attribute__((sda));
extern int  D_L14_001620D4_gp __asm__("D_L14_001620D4") __attribute__((sda));

extern int electric_beam_damage_disabled_gp __asm__("D_L14_001620D8") __attribute__((sda));
void       update_electric_beam(unsigned char *moby) __asm__("FUN_L14_003015d0");

void update_electric_beam(unsigned char *moby) {
    enum {
        BEAM_STATE_INITIALIZE = 0,
        BEAM_STATE_COUNTER_TRIGGERED_PULSES = 1,
        BEAM_STATE_WAIT_FOR_TIMED_PULSE = 2,
        BEAM_STATE_REPEAT_PULSES = 3
    };
    D1224 *beam_vars = ((D1224 *)MOBY(moby)->pvars);
    float  beam_culling_sphere[4] __attribute__((aligned(16)));
    int    beam_state;
    int    phase_duration_frames;
    if (moby[0x20] != 0) {
        qcopy(beam_culling_sphere, moby + 0x10);
        beam_culling_sphere[3] = beam_vars->f624;
        if (fast_bsphere_check(beam_culling_sphere, 52.0f) < 0) {
            return;
        }
    }
    if (D_L14_001620CC_gp != 0) {
        return;
    }
    beam_state = moby[0x20];
    switch (beam_state) {
    case BEAM_STATE_INITIALIZE:
        start_electric_beam_pulse((char *)moby);
        beam_vars->s10 = scale_game_frames(beam_vars->wC);
        moby[0x20] = 2;
        beam_vars->f624 = fast_vec_length(&l14_cuboids[beam_vars->w0].axes[1]);
        beam_vars->s8 = 1;
        beam_vars->sA = 1;
        break;
    case BEAM_STATE_COUNTER_TRIGGERED_PULSES:
        if (D_L14_001620B8_gp != 0) {
            if (beam_vars->s12 == 0) {
                D_L14_001620B8_gp = D_L14_001620B8_gp - 1;
                start_electric_beam_pulse((char *)moby);
            }
        }
        break;
    case BEAM_STATE_WAIT_FOR_TIMED_PULSE:
        if (fast_dec_timer_s16(&beam_vars->s10) != 0) {
            moby[0x20] = 3;
            beam_vars->s10 = scale_game_frames(beam_vars->s4);
            start_electric_beam_pulse((char *)moby);
        }
        break;
    case BEAM_STATE_REPEAT_PULSES:
        if (fast_dec_timer_s16(&beam_vars->s10) != 0) {
            if (beam_vars->s6 != 0) {
                moby[0x20] = 2;
                phase_duration_frames = beam_vars->s6;
            } else {
                phase_duration_frames = beam_vars->s4;
            }
            beam_vars->s10 = scale_game_frames(phase_duration_frames);
        } else if (beam_vars->s12 == 0) {
            start_electric_beam_pulse((char *)moby);
        }
        break;
    }
    if (electric_beam_damage_disabled_gp == 0) {
        if (vector_distance_xy(moby + 0x10, &hero.motion.pos) < 10.0f) {
            apply_electric_beam_damage((char *)moby);
        }
    }
    if (beam_vars->s12 != 0) {
        if (D_L14_001620D0_gp == 0) {
            update_electric_beam_animation((char *)moby);
        }
        if (*(int *)&D_L14_001620C8_gp < beam_vars->s12) {
            if (D_L14_001620D4_gp != 0) {
                return;
            }
            add_draw_callback(draw_electric_beam, moby);
        }
    }
}

extern int electric_beam_min_pulse_duration_frames_gp __asm__("D_L14_001620BC")
    __attribute__((sda));
extern int electric_beam_max_pulse_duration_frames_gp __asm__("D_L14_001620C0")
    __attribute__((sda));
extern float electric_beam_displacement_radius_gp __asm__("D_L14_001620FC") __attribute__((sda));
extern float electric_beam_max_point_speed_gp __asm__("D_L14_00162100") __attribute__((sda));
extern int   electric_beam_spark_initial_word_gp __asm__("D_L14_00162108") __attribute__((sda));
extern int   electric_beam_spark_second_word_gp __asm__("D_L14_0016210C") __attribute__((sda));

#include "rnc/overlay/shared/props/electric_beam_data/start_electric_beam_pulse_2.h"

extern int D_L14_0016211C __attribute__((sda));

#include "rnc/overlay/shared/props/electric_beam_data/start_electric_beam_pulse.h"

extern char D_L14_001674C0[];

typedef struct {
    int  frame; /* 0x0: l14_cuboids index */
    u8   pad4[0xE];
    s16  countdown;       /* 0x12 */
    int  color;           /* 0x14 */
    u8   branch_start[4]; /* 0x18: main line point each branch leaves from */
    u8   pad1c[4];
    Vec4 points[32];         /* 0x20 */
    Vec4 point_steps[32];    /* 0x220 */
    Vec4 branches[4][4];     /* 0x420: [point][branch] */
    Vec4 branch_steps[4][4]; /* 0x520: [point][branch] */
    int  duration;           /* 0x620 */
} ElectricBeamVars;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00302C80). */
void start_electric_beam_pulse(char *moby) {
    ElectricBeamVars *beam = ((ElectricBeamVars *)MOBY(moby)->pvars);
    Vec4              path[32];
    Vec4              path_steps[32];
    Vec4              work, delta;
    Vec4              matrix[4];
    LevelCuboid      *frame;
    float             length;
    int               low, high, duration;
    int               i, j, k, branch;

    frame = (LevelCuboid *)((beam->frame << 7) + (int)l14_cuboids);
    {
        int ticks = electric_beam_min_pulse_duration_frames_gp;
        frame->pos.w = 1.0f;
        low = scale_game_frames(ticks);
    }
    high = scale_game_frames(electric_beam_max_pulse_duration_frames_gp);
    duration = random_int_between(low, high);
    beam->countdown = (short)duration;
    beam->duration = (short)duration;
    length = fast_vec_length(&l14_cuboids[beam->frame].axes[1]);
    length = length + length;

    for (i = 0; i < 32; i++) {
        float y_offset = convert_integer_to_float(i);
        float last = convert_integer_to_float(0x1F);
        float y;
        y_offset = y_offset / last;
        y_offset = (0.5f - y_offset) * length;
        if (i < 3 || i > 28) {
            clear_u64_value(&path[i]);
            clear_u64_value(&path_steps[i]);
        } else {
            random_vector(&path[i], 0.0f, electric_beam_displacement_radius_gp);
            random_vector(&path_steps[i], 0.0f, electric_beam_max_point_speed_gp * frame_time);
        }
        y = path[i].f[1];
        path[i].f[3] = 1.0f;
        path[i].f[1] = y + y_offset;
    }

    for (j = 0; j < 32; j++) {
        if (j == 0 || j == 31) {
            qcopy(&beam->points[j], &path[j]);
            qcopy(&beam->point_steps[j], &path_steps[j]);
        } else {
            fast_vec_add(&work, &path[j - 1], &path[j]);
            fast_vec_add(&work, &work, &path[j + 1]);
            fast_vec_scale(&beam->points[j], &work, 0.333f);
            fast_vec_add(&work, &path_steps[j - 1], &path_steps[j]);
            fast_vec_add(&work, &work, &path_steps[j + 1]);
            fast_vec_scale(&beam->point_steps[j], &work, 0.333f);
        }
    }

    for (branch = 0; branch < 4; branch++) {
        int   start;
        float spread = electric_beam_displacement_radius_gp;
        random_vector(&work, spread, spread);
        start = truncate_float_to_s32(
                    random_float_between((float)branch * 6.0f, (float)(branch + 1) * 6.0f)) +
                4;
        beam->branch_start[branch] = (unsigned char)start;
        qcopy(&beam->branches[0][branch], &beam->points[start]);
        qcopy(&beam->branch_steps[0][branch], &beam->point_steps[start]);
        for (k = 1; k < 4; k++) {
            random_vector(&delta, 0.1f, 0.1f);
            random_vector(&beam->branch_steps[k][branch],
                          0.0f,
                          electric_beam_max_point_speed_gp * frame_time);
            fast_vec_add(&beam->branches[k][branch], &beam->branches[k - 1][branch], &work);
            fast_vec_add(&beam->branches[k][branch], &beam->branches[k][branch], &delta);
        }
    }

    if (vector_distance_xy(&l14_cuboids[beam->frame].pos, D_L14_001674C0) >= 4.0f) {
        int color;
        int point;
        int never = 0;
        euler_rotation_matrix(matrix, &l14_cuboids[beam->frame].rot);
        qcopy(&matrix[3], &l14_cuboids[beam->frame].pos);
        for (point = 0; point < 32; point += 8) {
            int frames;
            transform_vector(&work, &beam->points[point], matrix);
            qcopy(&delta, &beam->point_steps[point]);
            delta.f[3] = electric_beam_spark_step_w_gp;
            color =
                random_integer_below(2) ? D_L14_0016211C : electric_beam_branch_end_spark_color_gp;
            frames = scale_game_frames(random_integer_below(10) + 20);
            spawn_spark(&work,
                        &delta,
                        &delta,
                        electric_beam_spark_initial_word_gp,
                        electric_beam_spark_second_word_gp,
                        point >> 1,
                        15,
                        frames,
                        color);
        }
        for (k = 0; k < 2; k++) {
            int frames, unused_color;
            transform_vector(&work, &beam->branches[3][k], matrix);
            qcopy(&delta, &beam->branch_steps[3][k]);
            delta.f[3] = electric_beam_spark_step_w_gp;
            unused_color =
                random_integer_below(2) ? D_L14_0016211C : electric_beam_branch_end_spark_color_gp;
            frames = scale_game_frames(random_integer_below(10) + 20);
            /* Fakematch: a never-taken exit keeps the spark arguments in their own block until
             * gcse (retail's dead color pick did), so &delta is reused from the spilled copy. */
            if (never)
                continue;
            spawn_spark(&work,
                        &delta,
                        &delta,
                        electric_beam_spark_initial_word_gp,
                        electric_beam_spark_second_word_gp,
                        beam->branch_start[k] >> 1,
                        15,
                        frames,
                        electric_beam_branch_end_spark_color_gp);
        }
    }
}

extern int electric_beam_color_transition_countdown_gp __asm__("D_L14_001620C4")
    __attribute__((sda));
extern int electric_beam_initial_color_gp __asm__("D_L14_001620F0") __attribute__((sda));
extern int electric_beam_transition_color_gp __asm__("D_L14_001620F4") __attribute__((sda));
extern int electric_beam_final_color_gp __asm__("D_L14_001620F8") __attribute__((sda));
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

void update_electric_beam_animation(char *moby) {
    char *vars = ((char *)MOBY(moby)->pvars);
    char *beam_point, *src;
    u8   *indices;
    int   i, j, k, copy_i, next_i;
    float fraction;

    if (*(s16 *)(vars + 0x12) != 0) {
        *(s16 *)(vars + 0x12) = *(u16 *)(vars + 0x12) - 1;
        indices = (u8 *)(vars + 0x18);
        src = vars + 0x260;
        beam_point = vars + 0x60;
        for (j = 22; j >= 0; j--) {
            fast_vec_add(beam_point, beam_point, src);
            beam_point += 0x10;
            src += 0x10;
        }
        i = 0;
        do {
            next_i = i + 1;
            for (k = 2; k >= 0; k--) {
                char *row = vars + (i << 4) + (2 - k) * 0x40;
                fast_vec_add(row + 0x460, row + 0x460, row + 0x560);
            }
            i = next_i;
        } while ((float)i < 4.0f);
        {
            char *src = (char *)indices;
            indices = 0; /* End the saved index-base lifetime after the transfer. */
            copy_i = 0;
            do {
                qcopy(vars + 0x420 + (copy_i << 4),
                      (char *)(((u8)src[copy_i] << 4) + (int)vars) + 0x20);
                copy_i++;
            } while (copy_i < 4);
        }
    }
    if (*(s16 *)(vars + 0x12) > electric_beam_color_transition_countdown_gp) {
        fraction = convert_integer_to_float(*(s16 *)(vars + 0x12) -
                                            electric_beam_color_transition_countdown_gp) /
                   convert_integer_to_float(*(int *)(vars + 0x620) -
                                            electric_beam_color_transition_countdown_gp);
        *(int *)(vars + 0x14) = fast_tween_color(
            electric_beam_transition_color_gp, electric_beam_initial_color_gp, fraction);
    } else if (*(s16 *)(vars + 0x12) > D_L14_001620C8) {
        fraction =
            convert_integer_to_float(*(s16 *)(vars + 0x12) - D_L14_001620C8) /
            convert_integer_to_float(electric_beam_color_transition_countdown_gp - D_L14_001620C8);
        *(int *)(vars + 0x14) = fast_tween_color(
            electric_beam_final_color_gp, electric_beam_transition_color_gp, fraction);
    } else {
        *(int *)(vars + 0x14) = 0;
    }
}

typedef struct {
    u128  vertices[4];
    int   colors[4];
    float uv[4][2];
    u64   tag;
    u64   texture;
    u64   texture_state;
    u64   gs_state;
} ElectricBeamStripPacket;

extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

extern int electric_beam_effect_texture_index_gp __asm__("D_L14_001620DC") __attribute__((sda));

#include "rnc/overlay/shared/props/electric_beam_data/draw_electric_beam.h"

extern float electric_beam_base_uv[4][2] __asm__("D_L14_001E5FD0");

/* Fakematch: an empty scope with a declaration adds two block notes (see the corner loops). */
#define EMPTY_SCOPES_4   \
    { typedef int pad; } \
    { typedef int pad; } \
    { typedef int pad; } \
    { typedef int pad; }

/* Ported from rac1-decomp (src/overlays/shared/vendor_00303430.c: func_L14_00303430). */
void draw_electric_beam(char *moby) {
    ElectricBeamVars       *beam = ((ElectricBeamVars *)MOBY(moby)->pvars);
    ElectricBeamStripPacket packet[3];
    float                   world[16] __attribute__((aligned(16)));
    float                   inverse[16] __attribute__((aligned(16)));
    float                   camera[4] __attribute__((aligned(16)));
    float                   prev_offset[4] __attribute__((aligned(16)));
    float                   offset[4] __attribute__((aligned(16)));
    float                   view[4] __attribute__((aligned(16)));
    float                   tangent[4] __attribute__((aligned(16)));
    u64                     texture;
    u64                     gs_state;
    int                     branch, node, corner;
    int                     start_point;
    int                     k;

    euler_rotation_matrix(world, &l14_cuboids[beam->frame].rot);
    transpose_rotation_matrix(inverse, world);
    qcopy_nc(&world[12], &l14_cuboids[beam->frame].pos);
    fast_vec_sub(camera, D_L14_001674C0, &l14_cuboids[beam->frame].pos);
    transform_vector_by_basis(camera, camera, inverse);
    texture = get_effect_texture(electric_beam_effect_texture_index_gp);
    gs_state = (u64)electric_beam_blend_minuend_selector_gp |
               ((u64)electric_beam_blend_subtrahend_selector_gp << 2) |
               ((u64)electric_beam_blend_alpha_factor_selector_gp << 4) |
               ((u64)electric_beam_blend_addend_selector_gp << 6) | (0x8000ULL << 24);
    packet[2].texture_state = 0xFF9000000260ULL;
    packet[1].texture_state = 0xFF9000000260ULL;
    packet[0].texture_state = 0xFF9000000260ULL;
    packet[2].texture = texture;
    packet[1].texture = texture;
    packet[0].texture = texture;
    packet[2].tag = 0;
    packet[1].tag = 0;
    packet[0].tag = 0;
    packet[2].gs_state = gs_state;
    packet[1].gs_state = gs_state;
    packet[0].gs_state = gs_state;
    for (k = 0; k < 4; k++) {
        float v_coord = electric_beam_base_uv[k][1];
        int   color;
        packet[2].uv[k][1] = v_coord;
        packet[1].uv[k][1] = v_coord;
        packet[0].uv[k][1] = v_coord;
        color = beam->color;
        packet[2].colors[k] = color;
        packet[1].colors[k] = color;
        packet[0].colors[k] = color;
    }
    clear_u64_value(offset);
    {
        int point;
        for (point = 0; point < 31; point++) {
            qcopy(prev_offset, offset);
            if (point < 30) {
                fast_vec_sub(tangent, &beam->points[point], &beam->points[point + 2]);
                fast_vec_sub(view, &beam->points[point + 1], camera);
                fast_vec_cross(offset, view, tangent);
                fast_vec_normalize(offset, offset, 0.1f);
            } else {
                clear_u64_value(offset);
            }
            for (corner = 0; corner < 4; corner++) {
                if (corner < 2) {
                    qcopy(&packet[0].vertices[corner], &beam->points[point]);
                    if (corner & 1)
                        fast_vec_add(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], prev_offset);
                    else
                        fast_vec_sub(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], prev_offset);
                } else {
                    qcopy(&packet[0].vertices[corner], &beam->points[point + 1]);
                    if (corner & 1)
                        fast_vec_add(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], offset);
                    else
                        fast_vec_sub(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], offset);
                }
                /* Fakematch: 34 block notes push this loop past haifa-sched's 100-insn region limit,
                 * so sched1 moves nothing between its blocks, as in retail. */
                EMPTY_SCOPES_4 EMPTY_SCOPES_4 EMPTY_SCOPES_4 EMPTY_SCOPES_4 {
                    typedef int pad;
                }
                packet[0].uv[corner][0] = electric_beam_base_uv[corner][0] +
                                          convert_integer_to_float(point - point / 4 * 4) * 0.25f;
            }
            draw_geometry_quad(&packet[0], world, 0);
        }
    }
    for (branch = 0; branch < 4; branch++) {
        clear_u64_value(offset);
        start_point = beam->branch_start[branch];
        if (start_point >= 0x1F)
            continue;
        if (start_point <= -1)
            continue;
        for (node = 0; node < 3; node++) {
            qcopy(prev_offset, offset);
            if (node < 2) {
                fast_vec_sub(
                    tangent, &beam->branches[node][branch], &beam->branches[node + 2][branch]);
                fast_vec_sub(view, &beam->branches[node + 1][branch], camera);
                fast_vec_cross(offset, view, tangent);
                fast_vec_normalize(offset, offset, 0.1f);
            } else {
                clear_u64_value(offset);
            }
            if (node == 0)
                qcopy(prev_offset, offset);
            for (corner = 0; corner < 4; corner++) {
                if (corner < 2) {
                    qcopy(&packet[0].vertices[corner], &beam->branches[node][branch]);
                    if (corner & 1)
                        fast_vec_add(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], prev_offset);
                    else
                        fast_vec_sub(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], prev_offset);
                } else {
                    qcopy(&packet[0].vertices[corner], &beam->branches[node + 1][branch]);
                    if (corner & 1)
                        fast_vec_add(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], offset);
                    else
                        fast_vec_sub(
                            &packet[0].vertices[corner], &packet[0].vertices[corner], offset);
                }
                /* Fakematch: 32 block notes push this loop past haifa-sched's 100-insn region limit,
                 * so sched1 moves nothing between its blocks, as in retail. */
                EMPTY_SCOPES_4 EMPTY_SCOPES_4 EMPTY_SCOPES_4 EMPTY_SCOPES_4 packet[0]
                    .uv[corner][0] = electric_beam_base_uv[corner][0] +
                                     convert_integer_to_float(node - node / 4 * 4) * 0.25f;
            }
            draw_geometry_quad(&packet[0], world, 0);
        }
    }
}

/* Damage request passed to the collision line test (coll_line_fix). */
typedef struct {
    f32          v[4];   /* 0x0: push direction xy; w 5627.925 */
    struct Moby *moby;   /* 0x10: source */
    s32          flags;  /* 0x14 */
    s8           a;      /* 0x18 */
    s8           b;      /* 0x19 */
    s16          oclass; /* 0x1A: source class */
    f32          range;  /* 0x1C */
    s32          one;    /* 0x20 */
} ElectricBeamReq;

extern u8 D_L14_00174560[];

/* Sweeps the beam's two end lines; anything they cross takes damage pushed away from the beam. */
void apply_electric_beam_damage(char *self) {
    struct Moby    *beam_moby = (struct Moby *)self;
    D1224          *beam_vars = (D1224 *)beam_moby->pvars;
    ElectricBeamReq req;
    Vec4 damage_push_direction, to_hero, base, end_a, end_b, from, to, beam_radius_offset;
    f32  dot, collision_offset_length;

    if (beam_vars->radius == 0.0f)
        return;
    req.flags = 0x10001;
    req.range = 1.0f;
    req.one = 1;
    req.moby = beam_moby;
    clear_u64_value(&req);
    req.v[2] = 1.0f;
    req.b = 1;
    req.v[3] = 5627.925f;
    req.a = 3;
    req.oclass = beam_moby->oclass;
    damage_push_direction.f[0] = fast_cos(beam_moby->rot.z);
    damage_push_direction.f[1] = fast_sin(beam_moby->rot.z);
    damage_push_direction.f[2] = 0.0f;
    fast_vec_sub(&to_hero, &hero.motion.pos, &beam_moby->pos);
    to_hero.f[2] = 0.0f;
    dot = fast_vec_dot(&damage_push_direction, &to_hero);
    if (fast_vec_length(&to_hero) != 0.0f) {
        if (dot < 0.0f)
            fast_vec_scale(&damage_push_direction, &damage_push_direction, -1.0f);
        req.v[0] = damage_push_direction.f[0];
        req.v[1] = damage_push_direction.f[1];
    }
    qcopy_nc(&base, &l14_cuboids[beam_vars->w0].pos);
    if (beam_vars->s8) {
        fast_vec_add(&end_a, &base, &l14_cuboids[beam_vars->w0].axes[1]);
        if (collision_line(&base, &end_a, 0, beam_moby, 0)) {
            fast_vec_sub(&from, D_L14_00174560, &base);
            collision_offset_length = fast_vec_length(&from);
            fast_vec_scale(&from, &from, collision_offset_length * 0.98f / collision_offset_length);
            fast_vec_add(&beam_vars->end_a, &base, &from);
            beam_vars->s8 = 0;
        } else {
            qcopy(&beam_vars->end_a, &end_a);
        }
    }
    if (beam_vars->sA) {
        fast_vec_sub(&end_b, &base, &l14_cuboids[beam_vars->w0].axes[1]);
        if (collision_line(&base, &end_b, 0, beam_moby, 0)) {
            fast_vec_sub(&from, D_L14_00174560, &base);
            collision_offset_length = fast_vec_length(&from);
            fast_vec_scale(&from, &from, collision_offset_length * 0.98f / collision_offset_length);
            fast_vec_add(&beam_vars->end_b, &base, &from);
            beam_vars->sA = 0;
        } else {
            qcopy(&beam_vars->end_b, &end_b);
        }
    }
    qcopy(&end_a, &beam_vars->end_a);
    qcopy(&end_b, &beam_vars->end_b);
    fast_vec_normalize(&beam_radius_offset, &l14_cuboids[beam_vars->w0].axes[0], beam_vars->radius);
    fast_vec_add(&from, &end_a, &beam_radius_offset);
    fast_vec_add(&to, &end_b, &beam_radius_offset);
    collision_line(&from, &to, 1, beam_moby, &req);
    fast_vec_sub(&from, &end_a, &beam_radius_offset);
    fast_vec_sub(&to, &end_b, &beam_radius_offset);
    collision_line(&from, &to, 1, beam_moby, &req);
    fast_vec_normalize(&beam_radius_offset, &l14_cuboids[beam_vars->w0].axes[2], beam_vars->radius);
    fast_vec_add(&from, &end_a, &beam_radius_offset);
    fast_vec_add(&to, &end_b, &beam_radius_offset);
    collision_line(&from, &to, 1, beam_moby, &req);
    fast_vec_sub(&from, &end_a, &beam_radius_offset);
    fast_vec_sub(&to, &end_b, &beam_radius_offset);
    collision_line(&from, &to, 1, beam_moby, &req);
}

int   D_L15_00161F54 = 0;
int   D_L15_00161F58 = 0;
int   D_L15_00161F44 = 0;
int   D_L15_00161F5C = 0;
int   D_L15_00161F60 = 0;
int   D_L15_00161F64 = 0;
int   D_L15_00161F48 = 0xa;
int   D_L15_00161F4C = 0x28;
float D_L15_00161F88 = 0.3f;
float D_L15_00161F8C = 1.0f;
int   D_L15_00161F94 = 0x10806080;
int   D_L15_00161F98 = 0x10806060;
int   D_L15_00161FA8 = 0x18;
int   D_L15_00161F50 = 0xf;
int   D_L15_00161F7C = 0x401040;
int   D_L15_00161F80 = 0x80403040;
int   D_L15_00161F84 = 0x400000;
int   D_L15_00161F68 = 0xe;

float D_L15_001D94F0[4][2] = {
    { 0.0f, 0.0f},
    { 0.0f, 1.0f},
    {0.25f, 0.0f},
    {0.25f, 1.0f},
};

int   D_L17_00161F34 = 0;
int   D_L17_00161F38 = 0;
int   D_L17_00161F24 = 0;
int   D_L17_00161F3C = 0;
int   D_L17_00161F40 = 0;
int   D_L17_00161F44 = 0;
int   D_L17_00161F28 = 0xa;
int   D_L17_00161F2C = 0x28;
float D_L17_00161F68 = 0.3f;
float D_L17_00161F6C = 1.0f;
int   D_L17_00161F74 = 0x10806080;
int   D_L17_00161F78 = 0x10806060;
int   D_L17_00161F88 = 0x18;
int   D_L17_00161F30 = 0xf;
int   D_L17_00161F5C = 0x401040;
int   D_L17_00161F60 = 0x80403040;
int   D_L17_00161F64 = 0x400000;
int   D_L17_00161F48 = 0xe;

float D_L17_001D9860[4][2] = {
    { 0.0f, 0.0f},
    { 0.0f, 1.0f},
    {0.25f, 0.0f},
    {0.25f, 1.0f},
};

int D_L14_001620D0_gp __asm__("D_L14_001620D0") = 0;

s32 D_L14_001620C8_gp __asm__("D_L14_001620C8") = 0;

int D_L14_001620D4_gp __asm__("D_L14_001620D4") = 0;

int electric_beam_initial_color_gp __asm__("D_L14_001620F0") = 0x401040;

int electric_beam_effect_texture_index_gp __asm__("D_L14_001620DC") = 0xe;

float electric_beam_base_uv[4][2] __asm__("D_L14_001E5FD0") = {
    { 0.0f, 0.0f},
    { 0.0f, 1.0f},
    {0.25f, 0.0f},
    {0.25f, 1.0f},
};
