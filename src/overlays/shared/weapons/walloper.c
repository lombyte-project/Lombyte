/* Shared moby classes: walloper, morphoray. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/point_light.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/beam_vector.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_check_damage.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/math/random/random_spherical_offset.h"
#include "rnc/ui/hud/remove_hud_item.h"

extern float D_L00_001615AC_gp __asm__("D_L00_001615AC") __attribute__((sda));
extern float D_L00_001615B4_gp __asm__("D_L00_001615B4") __attribute__((sda));
extern float D_L00_001615C8_gp __asm__("D_L00_001615C8") __attribute__((sda));
extern char  D_0013E533[];

#include "rnc/data_labels.h"

extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern float g_debug_camera[] __asm__("D_L00_00166DC0");

#include "rnc/overlay/level_camera.h"

extern u8 D_L00_001DB5C0[];

#include "rnc/overlay/collision.h"

extern CollisionHit coll_hit __asm__("D_L00_00173E40");
extern float        D_L00_00161594 __attribute__((sda));
extern float beam_target_hero_heading_error_limit __asm__("D_L00_00161598") __attribute__((sda));
extern float beam_target_camera_heading_error_limit __asm__("D_L00_0016159C") __attribute__((sda));
extern float D_L00_001615A0 __attribute__((sda));
extern float primary_beam_wave_start_amplitude __asm__("D_L00_001615A4") __attribute__((sda));
extern float primary_beam_wave_end_amplitude __asm__("D_L00_001615A8") __attribute__((sda));
extern float primary_beam_wave_phase_advance __asm__("D_L00_001615B0") __attribute__((sda));
extern float secondary_beam_wave_phase_retreat __asm__("D_L00_001615B8") __attribute__((sda));
extern float D_L00_001615BC __attribute__((sda));
extern float secondary_beam_vertical_wave_envelope_amplitude __asm__("D_L00_001615C0")
    __attribute__((sda));
extern float secondary_beam_horizontal_wave_amplitude __asm__("D_L00_001615C4")
    __attribute__((sda));
extern float secondary_beam_vertical_envelope_phase_advance __asm__("D_L00_001615CC")
    __attribute__((sda));
extern float D_L00_001615D0 __attribute__((sda));
extern float primary_beam_vertical_jitter_range_start __asm__("D_L00_001615E4")
    __attribute__((sda));
extern float primary_beam_vertical_jitter_range_end __asm__("D_L00_001615E8") __attribute__((sda));
extern int   D_L00_001615F8 __attribute__((sda));
extern float beam_origin_particle_direction_scale __asm__("D_L00_00161628") __attribute__((sda));
extern float beam_target_camera_alignment_score_weight __asm__("D_L00_0016162C")
    __attribute__((sda));
extern short D_L00_00161658[] MACRO_ADDR;
extern short D_L00_00161668[] MACRO_ADDR;
extern short D_L00_00161670[] MACRO_ADDR;
extern s16   D_L00_00161660;
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void  fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
extern void  fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  clear_u64_value() __asm__("FUN_001f99f8");
extern void  blend_vector_2b94d0(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int   truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern int   fast_dec_timer_s16() __asm__("FUN_001f9770");
extern int   suck_get_aim_height() __asm__("FUN_002141f8");
/* The arc points one slot before each arc row. */
extern Vec4  D_L00_001DBA10[4][5];
extern Vec4  arc_points[4][5] __asm__("D_L00_001DBA20");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int   FUN_L00_00257b90(int, int), FUN_00213260(int);
extern void  make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void  record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern int   test_moby_linked_record_word8(void *) __asm__("FUN_L00_00259830");
extern int   collision_line(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern void
collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void           FUN_0022da68(int, int, void *);
extern Vec4           zero_vector __asm__("D_L00_0015F580");
extern Vec4           zero_vector_macro __asm__("D_L00_0015F580") MACRO_ADDR;
extern unsigned char *FUN_L00_00272060(
    void *, int, float, float, int, int, int, float *, float) __asm__("FUN_L00_00272060");

typedef struct {
    Vec4  direction;
    int   words[2];
    u8    kind, mode;
    short id;
    float amount;
    int   flag;
} HitDescriptor;

typedef struct {
    Vec4         origin;
    Vec4         tip;
    struct Moby *target;
    u8           pad24[0x14];
    struct Moby *second_target;
    u8           pad3c[2];
    short        count3e;
    short        target_timer;
    short        second_timer;
    short        unk44;
    short        unk46;
} WalloperBeam;

void build_beam_and_arc_points(struct Moby *o) __asm__("FUN_L00_002b94d0");

void build_beam_and_arc_points(struct Moby *beam_owner_moby) {
    Vec4 target0, target1;
    struct {
        Vec4  direction;
        int   words[2];
        u8    kind, mode;
        short id;
        float amount;
        int   flag;
    } hit;
    Vec4         camera_direction, direction, delta, blend, step, offset;
    struct Moby *collided[128];
    Vec4         target_delta;
    union {
        struct {
            Vec4          origin;
            HitDescriptor hit0;
        } first;
        HitDescriptor hit1;
    } effects;
    /* Retail uses a five-vector scratch span with differences beginning at slot 1. */
    Vec4          differences[5];
    WalloperBeam *beam_state = ((WalloperBeam *)beam_owner_moby->pvars);
    int           i;
    struct Moby  *best = 0, *second = 0, *candidate, **list;
    char         *data;
    float         best_score = 0.0f, second_score = 0.0f;
    int           enabled = (u8)D_0013E533[0];
    float         spacing = D_L00_00161594 / 20.0f;
    float distance, angle, score, beam_wave_sample_index, amplitude, wave, length, factor, phase,
        side_wave;
    float        side_phase;
    float        x_s, x_c, x_a, x_m;
    struct Hero *player;
    LevelCamera *camera;
    struct Hero *aim_hero;
    float        primary_distance, sign;
    float        distance_w17;
    float        distance_w16;
    float        factor_w15;
    float        factor_w13;
    float        length_w4;
    float        length_w3;
    int          k;
    int          arc_point_index;
    int          mirror;
    int          seg;
    int          j, hit_result, collided_count, spin;
    char        *previous, *side;
    Vec4        *beam;
    float       *side_waves;
    D_L00_001615AC_gp = fast_add_rotations(D_L00_001615AC_gp, primary_beam_wave_phase_advance);
    D_L00_001615B4_gp =
        fast_subtract_rotations(D_L00_001615B4_gp, secondary_beam_wave_phase_retreat);
    D_L00_001615C8_gp =
        fast_add_rotations(D_L00_001615C8_gp, secondary_beam_vertical_envelope_phase_advance);
    if (!beam_state->target || (enabled && !beam_state->second_target)) {
        if (beam_state->second_target) {
            beam_state->target = beam_state->second_target;
            beam_state->target_timer = beam_state->second_timer;
            beam_state->second_target = 0;
            beam_state->second_timer = 0;
        } else {
            for (k = 0; (candidate = ((struct Moby **)active_mobies)[k]) != 0; k++) {
                camera = &l00_camera;
                aim_hero = &hero;
                if (candidate == beam_state->target || candidate == beam_state->second_target ||
                    !candidate || !candidate->pclass ||
                    *(short *)((char *)candidate->pclass + 0x46) != 5)
                    continue;
                if (candidate->state == 0xfe)
                    goto next_candidate;
                if (candidate->state == 0xfd)
                    goto next_candidate;
                if (!suck_get_aim_height(candidate))
                    continue;
                fast_vec_sub(&target0, &candidate->pos, beam_state);
                distance_w16 = vector_length_xy(&target0);
                if (D_L00_00161594 < distance_w16)
                    continue;
                length_w3 = fast_difference_between_rotations(
                    camera->yaw,
                    fast_arc_tan(candidate->pos.x - camera->pos.f[0],
                                 candidate->pos.y - camera->pos.f[1]));
                if (beam_target_camera_heading_error_limit < length_w3)
                    continue;
                score = fast_difference_between_rotations(
                    fast_arc_tan(candidate->pos.x - aim_hero->motion.pos.f[0],
                                 candidate->pos.y - aim_hero->motion.pos.f[1]),
                    aim_hero->motion.rot.f[2]);
                if (beam_target_hero_heading_error_limit < score)
                    continue;
                score = ((beam_target_camera_heading_error_limit - length_w3) /
                         beam_target_camera_heading_error_limit) *
                            beam_target_camera_alignment_score_weight +
                        (D_L00_00161594 - distance_w16) / D_L00_00161594;
                if (best_score < score) {
                    second_score = best_score;
                    second = best;
                    best_score = score;
                    best = candidate;
                } else if (second_score < score) {
                    second_score = score;
                    second = candidate;
                }
            next_candidate:;
            }
        }
    }
    if (best) {
        if (beam_state->target)
            second = best;
        else {
            beam_state->target = best;
            beam_state->target_timer = 0;
            FUN_0022da68(1, 0, beam_owner_moby);
        }
    }
    if (second && enabled) {
        distance_w17 = fast_vec_dist(beam_state, &best->pos);
        if (distance_w17 < D_L00_00161594) {
            angle = fast_vec_dist(&best->pos, &second->pos);
            if (distance_w17 + angle < D_L00_00161594) {
                beam_state->second_target = second;
                beam_state->second_timer = 0;
            }
        }
    }
    primary_distance = 0.0f;
    if (beam_state->target) {
        data = (char *)suck_get_aim_height(beam_state->target);
        qcopy(&target0, &beam_state->target->pos);
        target0.f[2] += *(float *)(data + 0x10);
        primary_distance = fast_vec_dist(beam_state, &target0);
        spacing = primary_distance * 0.05f;
    }
    if (beam_state->second_target) {
        data = (char *)suck_get_aim_height(beam_state->second_target);
        qcopy(&target1, &beam_state->second_target->pos);
        target1.f[2] += *(float *)(data + 0x10);
        spacing = (fast_vec_dist(&target0, &target1) + primary_distance) * 0.05f;
    }
    if (enabled)
        D_L00_001615F8 = 0x207f20;
    else
        D_L00_001615F8 = 0x7f2020;
    clear_u64_value(&camera_direction);
    make_hit_descriptor(&hit, beam_owner_moby, 0x210000, &camera_direction, enabled + 2.0f);
    hit.kind = 5;
    hit.mode = 1;
    hit.id = beam_owner_moby->oclass;
    beam_state->count3e = 20;
    player = &hero;
    collect_mobies_in_sphere(0.3f, beam_state, 0x10, player->moby, &hit);
    fast_vec_sub(&direction, &beam_state->tip, beam_state);
    fast_vec_normalize(&direction, &direction, 1.0f);
    if (beam_state->target || beam_state->second_target) {
        fast_vec_cross(&delta, &direction, &player->gravity_dir);
        build_look_at_matrix(&direction, &direction, &delta, 0.17453292f);
    }
    hit_result = 0;
    collided_count = 0;
    fast_vec_scale(spacing, &step, &direction);
    qcopy(D_L00_001DB5C0, beam_state);
    qcopy(D_L00_001DB5C0 + 0x140, beam_state);
    beam = (Vec4 *)D_L00_001DB5C0;
    side_waves = (float *)D_L00_001DB9D0;
    for (seg = 1; seg < 20; seg++) {
        previous = (char *)&beam[seg - 1];
        side = (char *)&beam[seg + 20];
        beam_wave_sample_index = (float)seg;
        factor = primary_beam_wave_start_amplitude +
                 (primary_beam_wave_end_amplitude - primary_beam_wave_start_amplitude) *
                     (convert_integer_to_float(seg) * 0.05f);
        phase = normalize_angle(D_L00_001615AC_gp + beam_wave_sample_index * D_L00_001615A0);
        wave = factor * fast_sin(phase);
        wave += randf_sym(primary_beam_vertical_jitter_range_start,
                          primary_beam_vertical_jitter_range_end);

        beam[seg].f[2] -= ((float *)D_L00_001DB840)[seg];
        ((float *)D_L00_001DB840)[seg] = wave;
        fast_vec_sub(&delta, &beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16);
        blend_vector_2b94d0(0.85f, &blend, &delta, &step);
        {
            float blend_length = fast_vec_length(&blend);
            if (blend_length == 0.0f)
                qcopy(&beam[seg], previous);
            else {
                fast_vec_scale(spacing / blend_length, &blend, &blend);
                fast_vec_add(&beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16, &blend);
                qcopy(&step, &blend);
            }
        }
        beam[seg].f[2] += wave;
        fast_vec_sub(&delta, &beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16);
        if (beam_state->target) {
            if (!enabled || !beam_state->second_target) {
                fast_vec_sub(&target_delta, &target0, &beam[seg]);
                length_w4 = fast_vec_length(&target_delta);
                if (length_w4 != 0.0f) {
                    if (seg == 19) {
                        factor_w13 = 0.5f;
                        spacing = length_w4;
                    } else if (seg >= 16) {
                        factor_w13 = 0.5f;
                        spacing = length_w4 / convert_integer_to_float(19 - seg);
                    } else
                        factor_w13 = 0.1f;
                    fast_vec_scale(spacing / length_w4, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor_w13, &step, &step, &target_delta);
                }
            } else {
                if (seg >= 10)
                    fast_vec_sub(&target_delta, &target1, &beam[seg]);
                else
                    fast_vec_sub(&target_delta, &target0, &beam[seg]);
                length_w4 = fast_vec_length(&target_delta);
                if (length_w4 != 0.0f) {
                    if (seg == 9 || seg == 19) {
                        factor_w13 = 0.7f;
                        spacing = length_w4;
                    } else if ((unsigned)(seg - 5) < 5) {
                        factor_w13 = 0.7f;
                        spacing = length_w4 / convert_integer_to_float(9 - seg);
                    } else if (seg > 14) {
                        factor_w13 = 0.7f;
                        spacing = length_w4 / convert_integer_to_float(19 - seg);
                    } else
                        factor_w13 = 0.2f;
                    fast_vec_scale(spacing / length_w4, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor_w13, &step, &step, &target_delta);
                }
            }
        }
        if (seg < 10) {
            side_phase = normalize_angle(D_L00_001615B4_gp + (float)seg * D_L00_001615BC);
            angle = normalize_angle(D_L00_001615C8_gp + (float)seg * D_L00_001615D0);
            factor = secondary_beam_vertical_wave_envelope_amplitude * fast_sin(angle);
            qcopy(side, &beam[seg]);
            x_s = fast_sin(side_phase);
            side_wave = randf_sym(0.0f, 0.1f);
            x_c = factor * x_s + side_wave;
            side_waves[seg] = x_c;
            *(float *)((char *)&beam[seg] + 0x148) += x_c;
            x_a = fast_add_rotations(fast_arc_tan(delta.f[0], delta.f[1]), 1.5707964f);
            x_m = secondary_beam_horizontal_wave_amplitude * fast_cos(side_phase);
            offset.f[0] = fast_cos(x_a) * x_m;
            offset.f[1] = fast_sin(x_a) * x_m;
            offset.f[2] = 0.0f;
            qcopy_nc(&D_L00_001DB890[seg], &offset);
            fast_vec_add(side, side, &offset);
        } else {
            mirror = 21 - seg;
            qcopy(side, &beam[seg]);
            *(float *)((char *)&beam[seg] + 0x148) += side_waves[mirror];
            fast_vec_add(side, side, &D_L00_001DB890[mirror]);
            qcopy(&offset, &D_L00_001DB890[seg]);
        }
        if (seg == 1) {
            struct Hero *collision_source_hero = &hero;
            fast_vec_scale(0.5f, &effects.first.origin, (char *)collision_source_hero->moby + 0xe0);
            fast_vec_add(&target_delta, &collision_source_hero->motion.pos, &effects.first.origin);
            hit_result = collision_line(
                (Vec4f *)&target_delta, (Vec4f *)beam_state, 0x10, collision_source_hero->moby, 0);
        } else if (seg == 19 || !(seg & 1)) {
            hit_result = collision_line((Vec4f *)((char *)D_L00_001DB5C0 - 32 + seg * 16),
                                        (Vec4f *)&beam[seg],
                                        0x10,
                                        beam_owner_moby,
                                        0);
        } else
            continue;
        fast_vec_sub(&camera_direction, &beam[seg], g_debug_camera);
        camera_direction.f[2] = 0.0f;
        length = fast_vec_length(&camera_direction);
        if (length == 0.0f)
            camera_direction.f[2] = 1.0f;
        else
            fast_vec_scale(1.0f / length, &camera_direction, &camera_direction);
        camera_direction.f[3] = 5627.9248046875f;
        qcopy(&hit.direction, &camera_direction);
        if (hit_result) {
            candidate = (struct Moby *)coll_hit.moby;
            if (candidate) {
                if (!suck_get_aim_height(candidate) || !candidate->pclass ||
                    *(short *)((char *)candidate->pclass + 0x46) != 5)
                    record_moby_hit(candidate, &hit);
                else {
                    hit_result = 0;
                    collided[collided_count++] = candidate;
                    candidate->collision = 0;
                }
            }
            if (hit_result) {
                qcopy(&beam[seg], &coll_hit.point);
                beam_state->count3e = seg;
                qcopy(&beam_state->tip, &coll_hit.point);
                if (!coll_hit.moby || test_moby_linked_record_word8(coll_hit.moby)) {
                    beam_state->target = 0;
                    beam_state->target_timer = 0;
                }
                break;
            }
        }
    }
    for (k = 0; k < collided_count; k++)
        collided[k]->collision = *(int *)((char *)collided[k]->pclass + 0x10);
    if (!hit_result)
        qcopy(&beam_state->tip, D_L00_001DB5C0 + 0x130);
    if (beam_state->target && !(*(unsigned long *)&beam_state->target_timer & 0xffff0000ffffUL)) {
        fast_vec_sub(&target_delta, &beam_state->target->pos, g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(
            &effects.first.hit0, beam_owner_moby, 0x210000, &target_delta, enabled + 2.0f);
        effects.first.hit0.kind = 5;
        effects.first.hit0.mode = 3;
        effects.first.hit0.id = beam_owner_moby->oclass;
        record_moby_hit(beam_state->target, &effects.first.hit0);
        {
            char *aim;
            int   frames = 5;
            aim = (char *)suck_get_aim_height(beam_state->target);
            if (aim && *(short *)(aim + 4) == 1)
                frames = 1;
            beam_state->unk44 = scale_game_frames(frames);
        }
    }
    if (beam_state->second_target &&
        !(*(unsigned long *)&beam_state->target_timer & 0xffff0000ffff0000UL)) {
        fast_vec_sub(&target_delta, &beam_state->second_target->pos, g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(
            &effects.hit1, beam_owner_moby, 0x210000, &target_delta, enabled + 2.0f);
        effects.hit1.kind = 5;
        effects.hit1.mode = 3;
        effects.hit1.id = beam_owner_moby->oclass;
        record_moby_hit(beam_state->second_target, &effects.hit1);
        {
            char *aim;
            int   frames = 5;
            aim = (char *)suck_get_aim_height(beam_state->second_target);
            if (aim && *(short *)(aim + 4) == 1)
                frames = 1;
            beam_state->unk46 = scale_game_frames(frames);
        }
    }
    for (i = 0; i < 4; i++) {
        Vec4 *rows;
        if (i < 2) {
            qcopy(&effects.first.origin, beam_state);
            factor_w15 = 0.4f;
        } else {
            qcopy(&effects.first.origin, &beam_state->tip);
            factor_w15 = 0.5f;
        }
        if (fast_dec_timer_s16(&D_L00_00161658[i])) {
            D_L00_00161658[i] = scale_game_frames(D_L00_00161668[i]);
            (&D_L00_00161660)[i] = 0;
            angle = randf_sym(0.5235988f, 1.0471976f);
            phase = random_float_between(-1.0471976f, 0.17453292f);
            fast_vec_scale(factor_w15, &target_delta, &direction);
            rows = (Vec4 *)(D_0013F3D0 + 0x5d0);
            build_look_at_matrix(&target_delta, &target_delta, rows, phase);
            build_look_at_matrix(&target_delta, &target_delta, rows + 1, angle);
            qcopy(&arc_points[i][0], &effects.first.origin);
            fast_vec_add(&arc_points[i][1], &effects.first.origin, &target_delta);
            sign = 1.0f;
            for (arc_point_index = 2; arc_point_index < 5; arc_point_index++) {
                fast_vec_sub(
                    &effects.first.hit0, &arc_points[i][-1] + arc_point_index, g_debug_camera);
                phase = random_float_between(0.17453292f, 0.9599311f) * sign;
                sign = -sign;
                build_look_at_matrix(&target_delta, &target_delta, &effects.first.hit0, phase);
                fast_vec_add(&arc_points[i][arc_point_index],
                             &arc_points[i][-1] + arc_point_index,
                             &target_delta);
            }
        } else {
            int arc_alpha_timer_delta, arc_countdown_frames;
            for (j = 1; j < 5; j++) {
                fast_vec_sub(&differences[j], &arc_points[i][j], &arc_points[i][-1] + j);
                differences[j].f[0] += randf_sym(0.0f, 0.1f);
                differences[j].f[1] += randf_sym(0.0f, 0.1f);
                differences[j].f[2] += randf_sym(0.0f, 0.1f);
            }
            qcopy(&arc_points[i][0], &effects.first.origin);
            for (j = 1; j < 5; j++)
                fast_vec_add(&arc_points[i][j], &arc_points[i][-1] + j, &differences[j]);
            arc_alpha_timer_delta = D_L00_00161670[i];
            arc_countdown_frames = D_L00_00161658[i];
            if (arc_alpha_timer_delta < arc_countdown_frames)
                arc_alpha_timer_delta = arc_countdown_frames - arc_alpha_timer_delta;
            else
                arc_alpha_timer_delta = arc_alpha_timer_delta - arc_countdown_frames;
            distance = convert_integer_to_float(arc_alpha_timer_delta) /
                       convert_integer_to_float(scale_game_frames(15));
            (&D_L00_00161660)[i] = truncate_float_to_s32((1.0f - distance) * 32.0f);
        }
    }
    angle = randf_sym(0.5235988f, 1.0471976f);
    phase = random_float_between(-0.5235988f, 1.0471976f);
    fast_vec_scale(beam_origin_particle_direction_scale * frame_scale, &target_delta, &direction);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5d0, phase);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5e0, angle);
    spin = FUN_L00_00257b90(1, 8);
    if (FUN_00213260(2))
        spin = -spin;
    amplitude = random_float_between(0.2f, 0.6f);
    FUN_L00_00272060(beam_state,
                     scale_game_frames(15),
                     amplitude * 0.3f,
                     amplitude,
                     D_L00_001615F8 | 0x7f000000,
                     0,
                     spin,
                     (float *)&target_delta,
                     0.0f);
    spin = -spin;
    FUN_L00_00272060(beam_state,
                     scale_game_frames(15),
                     amplitude * 0.15f,
                     amplitude * 0.5f,
                     0x307f7f7f,
                     0,
                     spin,
                     (float *)&target_delta,
                     0.0f);
    spin = FUN_L00_00257b90(1, 8);
    if (FUN_00213260(2))
        spin = -spin;
    amplitude = random_float_between(0.15f, 3.0f);
    {
        /* Fakematch: a never-taken branch (cprop proves dead == 0, then jump deletes it). Retail
           had about 30 more insns before gcse that vanish later; they size the PRE hash table,
           which orders the &target_delta and &effects spill slots. */
        int dead = 0;
        if (amplitude < 0.0f)
            dead = 0;
        if (dead) {
            FUN_L00_00272060(beam_state,
                             scale_game_frames(15),
                             amplitude,
                             amplitude,
                             0,
                             0,
                             spin,
                             (float *)&target_delta,
                             0.0f);
            FUN_L00_00272060(beam_state,
                             scale_game_frames(15),
                             amplitude,
                             amplitude,
                             0,
                             0,
                             spin,
                             (float *)&target_delta,
                             0.0f);
            FUN_L00_00272060(beam_state,
                             scale_game_frames(15),
                             amplitude,
                             amplitude,
                             0,
                             0,
                             spin,
                             (float *)&target_delta,
                             0.0f);
        }
    }
    FUN_L00_00272060(&beam_state->tip,
                     scale_game_frames(12),
                     amplitude * 0.1f,
                     amplitude,
                     D_L00_001615F8 | 0x7f000000,
                     0,
                     spin,
                     zero_vector_macro.f,
                     0.0f);
}

typedef struct {
    u8  pad[0x3E];
    s16 h3E;
} S2ba970;

#include "rnc/overlay/shared/weapons/walloper_globals.h"

extern f32 beam_first_pass_texture_u_offset_gp __asm__("D_L00_001615EC") __attribute__((sda));
extern f32 beam_first_pass_texture_u_scroll_speed_gp __asm__("D_L00_001615F0") __attribute__((sda));
extern s32 primary_beam_first_pass_alpha_gp __asm__("D_L00_001615D4") __attribute__((sda));
extern s32 primary_beam_second_pass_alpha_gp __asm__("D_L00_001615DC") __attribute__((sda));
extern f32 beam_and_arc_first_pass_half_width_gp __asm__("D_L00_001615FC") __attribute__((sda));
extern f32 primary_beam_second_pass_half_width_gp __asm__("D_L00_00161600") __attribute__((sda));
extern f32 primary_beam_second_pass_segment_extension_factor_gp __asm__("D_L00_00161630")
    __attribute__((sda));
extern s32 secondary_beam_first_pass_alpha_gp __asm__("D_L00_00161604") __attribute__((sda));
extern s32 secondary_beam_second_pass_alpha_gp __asm__("D_L00_00161608") __attribute__((sda));
extern f32 D_L00_0016160C __attribute__((sda));
extern f32 secondary_beam_and_arc_second_pass_half_width_gp __asm__("D_L00_00161610")
    __attribute__((sda));
extern f32 secondary_beam_second_pass_segment_extension_factor_gp __asm__("D_L00_00161634")
    __attribute__((sda));
void draw_beam_and_arc_two_pass_quads(void *, s32, s32, s32, f32, f32, f32) __asm__("FUN_L00_002baae0");
void draw_beam_anchor_camera_facing_quads(void *mv) __asm__("FUN_L00_002bb0b0");

void draw_beam_anchor_camera_facing_quads(void *) __asm__("FUN_L00_002bb0b0");
void draw_beam_and_arc_effects(struct Moby *moby) __asm__("FUN_L00_002ba970");

void draw_beam_and_arc_effects(struct Moby *moby) {
    S2ba970 *moby_vars = ((S2ba970 *)moby->pvars);
    beam_first_pass_texture_u_offset_gp -= beam_first_pass_texture_u_scroll_speed_gp * frame_scale;
    if (beam_first_pass_texture_u_offset_gp <= -8.0f) {
        beam_first_pass_texture_u_offset_gp += 8.0f;
    }
    draw_beam_and_arc_two_pass_quads(D_L00_001DB5C0,
                                     primary_beam_first_pass_alpha_gp,
                                     primary_beam_second_pass_alpha_gp,
                                     moby_vars->h3E,
                                     beam_and_arc_first_pass_half_width_gp,
                                     primary_beam_second_pass_half_width_gp,
                                     primary_beam_second_pass_segment_extension_factor_gp);
    draw_beam_and_arc_two_pass_quads(D_L00_001DB5C0 + 0x140,
                                     secondary_beam_first_pass_alpha_gp,
                                     secondary_beam_second_pass_alpha_gp,
                                     moby_vars->h3E,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     secondary_beam_second_pass_segment_extension_factor_gp);
    draw_beam_and_arc_two_pass_quads(arc_points[0],
                                     D_L00_00161660,
                                     D_L00_00161660,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[1],
                                     beam_arc_second_row_alpha,
                                     beam_arc_second_row_alpha,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[2],
                                     beam_arc_third_row_alpha,
                                     beam_arc_third_row_alpha,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[3],
                                     beam_arc_fourth_row_alpha,
                                     beam_arc_fourth_row_alpha,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_anchor_camera_facing_quads(moby);
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

typedef float RW_2baae0[4] __attribute__((aligned(16)));

typedef struct {
    int   c[4];
    float f[8];
    long  z4, q0, q1, q2;
} RT_2baae0;

#include "rnc/overlay/shared/weapons/walloper_data/draw_beam_and_arc_two_pass_quads.h"

extern long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_beam_and_arc_two_pass_quads(void *mv,
                                      int   first_pass_alpha,
                                      int   second_pass_alpha,
                                      int   point_count,
                                      float sx,
                                      float sy,

                                      float sz) {
    char      *point_bytes = mv;
    RW_2baae0 *v = mv;
    RW_2baae0  first_pass_corners[4];
    RT_2baae0  first_pass_render_attributes;
    RW_2baae0  q2[4];
    RT_2baae0  u;
    RW_2baae0  camera_facing_side_vector, second_pass_width_offset, first_pass_point_camera_offset,
        segment_delta, adjusted_segment_start, adjusted_segment_end, segment_start_camera_offset,
        segment_end_camera_offset, s1A0, s1B0;
    int   i, col0, col1;
    float first_pass_side_normalization_factor, initial_first_pass_side_normalization_factor,
        segment_start_camera_distance, segment_end_camera_distance,
        second_pass_side_normalization_factor;
    first_pass_render_attributes.q0 = get_effect_texture(beam_and_arc_first_pass_texture_index_gp);
    col0 = beam_and_arc_first_pass_base_vertex_color_gp | (first_pass_alpha << 24);
    first_pass_render_attributes.q2 = 0x8000000048L;
    first_pass_render_attributes.q1 = 0xFF9000000260L;
    first_pass_render_attributes.z4 = 0;
    first_pass_render_attributes.c[3] = col0;
    first_pass_render_attributes.c[2] = col0;
    first_pass_render_attributes.c[1] = col0;
    first_pass_render_attributes.c[0] = col0;
    u.q0 = get_effect_texture(beam_and_arc_second_pass_texture_index_gp);
    col1 = D_L00_001615F8 | (second_pass_alpha << 24);
    u.q2 = first_pass_render_attributes.q2;
    u.q1 = first_pass_render_attributes.q1;
    u.z4 = first_pass_render_attributes.z4;
    u.c[3] = col1;
    u.c[2] = col1;
    u.c[1] = col1;
    u.c[0] = col1;
    u.f[0] = 0;
    u.f[1] = 0;
    u.f[2] = 0;
    u.f[3] = 1.0f;
    u.f[4] = 1.0f;
    u.f[5] = 0;
    u.f[6] = 1.0f;
    u.f[7] = 1.0f;
    first_pass_render_attributes.f[0] = beam_first_pass_texture_u_offset_gp + 0;
    first_pass_render_attributes.f[1] = 0;
    first_pass_render_attributes.f[2] = beam_first_pass_texture_u_offset_gp + 0;
    first_pass_render_attributes.f[3] = 1.0f;
    first_pass_render_attributes.f[4] = beam_first_pass_texture_u_offset_gp + 1.0f;
    first_pass_render_attributes.f[5] = 0;
    first_pass_render_attributes.f[6] = beam_first_pass_texture_u_offset_gp + 1.0f;
    first_pass_render_attributes.f[7] = 1.0f;
    fast_vec_sub(first_pass_point_camera_offset, point_bytes, g_debug_camera);
    fast_vec_sub(segment_delta, point_bytes + 0x10, point_bytes);
    fast_vec_cross(camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
    initial_first_pass_side_normalization_factor = fast_vec_length(camera_facing_side_vector);
    if (initial_first_pass_side_normalization_factor != 0)
        initial_first_pass_side_normalization_factor =
            1.0f / initial_first_pass_side_normalization_factor;
    fast_vec_scale(beam_and_arc_first_pass_half_width_gp *
                       initial_first_pass_side_normalization_factor,
                   camera_facing_side_vector,
                   camera_facing_side_vector);
    fast_vec_add(first_pass_corners[0], point_bytes, camera_facing_side_vector);
    fast_vec_sub(first_pass_corners[1], point_bytes, camera_facing_side_vector);
    for (i = 1; i < point_count - 1; i++) {
        fast_vec_sub(first_pass_point_camera_offset, v[i], g_debug_camera);
        fast_vec_sub(segment_delta, v[i + 1], v[i]);
        fast_vec_cross(camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
        first_pass_side_normalization_factor = fast_vec_length(camera_facing_side_vector);
        if (first_pass_side_normalization_factor != 0)
            first_pass_side_normalization_factor = 1.0f / first_pass_side_normalization_factor;
        fast_vec_scale(beam_and_arc_first_pass_half_width_gp * first_pass_side_normalization_factor,
                       camera_facing_side_vector,
                       camera_facing_side_vector);
        fast_vec_add(first_pass_corners[2], v[i], camera_facing_side_vector);
        fast_vec_sub(first_pass_corners[3], v[i], camera_facing_side_vector);
        if (i == 1) {
            first_pass_render_attributes.c[1] = beam_and_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[0] = beam_and_arc_first_pass_base_vertex_color_gp;
        } else if (i == point_count - 2) {
            first_pass_render_attributes.c[3] = beam_and_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[2] = beam_and_arc_first_pass_base_vertex_color_gp;
        } else if (i == 2) {
            first_pass_render_attributes.c[1] = col0;
            first_pass_render_attributes.c[0] = col0;
        }
        draw_geometry_quad(first_pass_corners, 0, 0);
        qcopy(first_pass_corners[0], first_pass_corners[2]);
        qcopy(first_pass_corners[1], first_pass_corners[3]);
    }
    for (i = 0; i < point_count - 1; i++) {
        fast_vec_sub(segment_start_camera_offset, point_bytes + i * 16, g_debug_camera);
        fast_vec_sub(segment_end_camera_offset, point_bytes + i * 16 + 16, g_debug_camera);
        segment_start_camera_distance = fast_vec_length(segment_start_camera_offset);
        segment_end_camera_distance = fast_vec_length(segment_end_camera_offset);
        if (segment_end_camera_distance < segment_start_camera_distance) {
            if (segment_end_camera_distance == 0)
                qcopy(segment_end_camera_offset, segment_start_camera_offset);
            else
                fast_vec_scale(segment_start_camera_distance / segment_end_camera_distance,
                               segment_end_camera_offset,
                               segment_end_camera_offset);
        } else {
            if (segment_start_camera_distance == 0)
                qcopy(segment_start_camera_offset, segment_end_camera_offset);
            else
                fast_vec_scale(segment_end_camera_distance / segment_start_camera_distance,
                               segment_start_camera_offset,
                               segment_start_camera_offset);
        }
        fast_vec_add(adjusted_segment_start, g_debug_camera, segment_start_camera_offset);
        fast_vec_add(adjusted_segment_end, g_debug_camera, segment_end_camera_offset);
        fast_vec_sub(segment_delta, adjusted_segment_end, adjusted_segment_start);
        fast_vec_scale(sz, s1B0, segment_delta);
        fast_vec_sub(adjusted_segment_start, adjusted_segment_start, s1B0);
        fast_vec_add(adjusted_segment_end, adjusted_segment_end, s1B0);
        fast_vec_sub(s1A0, g_debug_camera, adjusted_segment_start);
        fast_vec_cross(camera_facing_side_vector, s1A0, segment_delta);
        second_pass_side_normalization_factor = fast_vec_length(camera_facing_side_vector);
        if (second_pass_side_normalization_factor != 0)
            second_pass_side_normalization_factor = 1.0f / second_pass_side_normalization_factor;
        fast_vec_scale(sy * second_pass_side_normalization_factor,
                       second_pass_width_offset,
                       camera_facing_side_vector);
        fast_vec_add(q2[0], adjusted_segment_start, second_pass_width_offset);
        fast_vec_sub(q2[1], adjusted_segment_start, second_pass_width_offset);
        fast_vec_add(q2[2], adjusted_segment_end, second_pass_width_offset);
        fast_vec_sub(q2[3], adjusted_segment_end, second_pass_width_offset);
        if (i == 0) {
            u.c[1] = D_L00_001615F8;
            u.c[0] = D_L00_001615F8;
        } else if (i == point_count - 2) {
            u.c[3] = D_L00_001615F8;
            u.c[2] = D_L00_001615F8;
        } else if (i == 1) {
            u.c[1] = col1;
            u.c[0] = col1;
        }
        draw_geometry_quad(q2, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2bb0b0;

typedef struct {
    float u, v;
} UV;

typedef struct {
    V_2bb0b0     corner[4];
    unsigned int color[4];
    UV           uv[4];
    long         unk70, tex, unk80, unk88;
} Q_2bb0b0;

extern unsigned int D_L00_001615F8_gp __asm__("D_L00_001615F8") __attribute__((sda));
extern float        g_debug_camera[] __asm__("D_L00_00166DC0") NOT_SDA;
extern char         D_0013F5E0[];
void                fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
void                fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
void                fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void                fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
void                fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
void                transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
void                draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern unsigned int l00_beam_anchor_first_pass_vertex_color __asm__("D_L00_00161640")
    __attribute__((sda));
extern float beam_anchor_first_pass_base_corner_scale_gp __asm__("D_L00_00161648")
    __attribute__((sda));
extern float beam_anchor_second_pass_base_corner_scale_gp __asm__("D_L00_0016164C")
    __attribute__((sda));
extern V_2bb0b0 D_L00_001DB560[] NOT_SDA;
extern UV       D_L00_001DB5A0[] NOT_SDA;

void draw_beam_anchor_camera_facing_quads(void *mv) {
    Q_2bb0b0     quad;
    V_2bb0b0     camera_facing_transform[4];
    V_2bb0b0    *quad_anchor_position = *(V_2bb0b0 **)((char *)mv + 0x78);
    unsigned int quad_color;
    float        corner_scale;
    int          i;
    quad.tex = get_effect_texture(0xB);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    fast_vec_sub(&camera_facing_transform[3], &(*(V_2bb0b0 *)g_debug_camera), quad_anchor_position);
    fast_vec_normalize(&camera_facing_transform[3], &camera_facing_transform[3], 0.3f);
    fast_vec_add(&camera_facing_transform[3], &camera_facing_transform[3], quad_anchor_position);
    camera_facing_transform[3].f[3] = 1.0f;
    fast_vec_sub(
        &camera_facing_transform[0], &(*(V_2bb0b0 *)g_debug_camera), &camera_facing_transform[3]);
    fast_vec_normalize(&camera_facing_transform[0], &camera_facing_transform[0], 1.0f);
    fast_vec_cross(
        &camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2bb0b0 *)D_0013F5E0));
    fast_vec_normalize(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    fast_vec_cross(
        &camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    quad_color = l00_beam_anchor_first_pass_vertex_color;
    quad.color[3] = quad_color;
    quad.color[2] = quad_color;
    quad.color[1] = quad_color;
    quad.color[0] = quad_color;
    corner_scale = beam_anchor_first_pass_base_corner_scale_gp + randf_sym(0.0f, 0.025f);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(corner_scale, &quad.corner[i], &D_L00_001DB560[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &camera_facing_transform[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_geometry_quad(&quad, 0, 0);
    quad_color = D_L00_001615F8_gp;
    quad.color[3] = quad_color;
    quad.color[2] = quad_color;
    quad.color[1] = quad_color;
    quad.color[0] = quad_color;
    corner_scale = beam_anchor_second_pass_base_corner_scale_gp + randf_sym(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(corner_scale, &quad.corner[i], &D_L00_001DB560[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &camera_facing_transform[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_geometry_quad(&quad, 0, 0);
}

void clear_u64_value() __asm__("FUN_001f99f8");
void refresh_moby_spatial_bounds_from_basis() __asm__("FUN_0020e098");
void update_entity() __asm__("FUN_L00_00250df8");
u8  *spawn_linked_moby_from_basis(u8 *basis_source_moby) __asm__("FUN_L00_002bb360");

u8 *spawn_linked_moby_from_basis(u8 *basis_source_moby) {
    u8 *spawned_moby = create_moby(0xB3);
    if (spawned_moby) {
        u8 *spawned_moby_vars = ((u8 *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        MOBY(spawned_moby)->flags = 0x204;
        spawned_moby[0x23] = 0x50;
        spawned_moby[0x20] = 0;
        *(u8 **)(spawned_moby_vars + 8) = basis_source_moby;
        clear_u64_value(spawned_moby + 0x40);
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row0 = *(OvlQuad *)&MOBY(basis_source_moby)->rot_row0;
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row1 = *(OvlQuad *)&MOBY(basis_source_moby)->rot_row1;
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row2 = *(OvlQuad *)&MOBY(basis_source_moby)->rot_row2;
        refresh_moby_spatial_bounds_from_basis(spawned_moby);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

typedef struct {
    char pad[0x10B0];
    int  i10B0;
} G_2bb400;

extern void  offset_moby_class_point_pairs(void *, int, int) __asm__("FUN_L00_0024efb0");
extern float FUN_001fa580_2bb400(float, float) __asm__("FUN_001fa580");

typedef struct {
    int            pad;
    int            ang;
    unsigned char *o;
    float          fC;
} V_2bb400;

void update_linked_moby_scale_and_rotation(struct Moby *moby) __asm__("FUN_L00_002bb400");

void update_linked_moby_scale_and_rotation(struct Moby *moby) {
    V_2bb400      *moby_vars = ((V_2bb400 *)moby->pvars);
    unsigned char *o = moby_vars->o;
    int            old, unwrapped_angle, angle_delta;
    if (o[0x20] == 0 || (o[0x20] & 0xF0) == 0xF0) {
        mark_moby_for_removal(moby);
        return;
    }
    if (o != 0 && (moby->flags & 1) != (*(u16 *)(o + 0x34) & 1)) {
        moby->flags &= ~1;
        moby->flags |= *(u16 *)(moby_vars->o + 0x34) & 1;
    }
    if (hero.items[0].ready_frames < scale_game_frames(0x19)) {
        moby->flags |= 0x41;
    }
    old = moby_vars->ang;
    angle_delta = -0xC0;
    unwrapped_angle = old + angle_delta;
    moby_vars->ang = unwrapped_angle;
    if (unwrapped_angle > 0x1000) {
        angle_delta = -0x10C0;
        moby_vars->ang = old + angle_delta;
    } else if (unwrapped_angle < 0) {
        angle_delta = 0xF40;
        moby_vars->ang = old + angle_delta;
    }
    offset_moby_class_point_pairs(((void *)moby->pclass), 0, angle_delta);
    moby->scale = *(float *)(((char *)moby->pclass) + 0x24) * moby_vars->fC *
                  random_float_between(0.5f, 1.0f);
    moby->rot.y = FUN_001fa580_2bb400(moby->rot.y, normalize_angle(moby_vars->fC));
    refresh_moby_spatial_bounds_from_basis(moby);
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BC860), where it is exact; names translated to the US level program. */

typedef struct {
    float previous[4];
    float point[4][4];
} SparkleRowC860;

/* One arc: its start point and four bent points toward the D_L00_001DBB90 row. */
typedef struct {
    float point[5][4];
} SparkleArc;

SparkleArc D_L00_001DBBA0[10] = {0};

SparkleArc D_L01_001DC020[10] = {0};

SparkleArc D_L02_001D0620[10] = {0};

SparkleArc D_L03_001D0120[10] = {0};

SparkleArc D_L04_001D01A0[10] = {0};

SparkleArc D_L05_001D04C0[10] = {0};

SparkleArc D_L06_001D07A0[10] = {0};

SparkleArc D_L07_001D00A0[10] = {0};

SparkleArc D_L08_001D0820[10] = {0};

SparkleArc D_L09_001D01A0[10] = {0};

SparkleArc D_L10_001D04A0[10] = {0};

SparkleArc D_L11_001D0A20[10] = {0};

SparkleArc D_L12_001E30C0[10] = {0};

SparkleArc D_L13_001D0930[10] = {0};

SparkleArc D_L14_001DCFE0[10] = {0};

SparkleArc D_L15_001D0AA0[10] = {0};

SparkleArc D_L16_001D04A0[10] = {0};

SparkleArc D_L17_001D0D20[10] = {0};

SparkleArc D_L18_001D0A20[10] = {0};

#include "rnc/overlay/shared/weapons/walloper_data/walloper_update_2.h"

extern float                D_L00_00166DC0[];
extern float D_L00_0017AEBC NOT_SDA;
extern float                FUN_L00_00257c48(float lo, float hi);
extern int                  D_L00_001616CC;

#include "rnc/overlay/shared/weapons/walloper_data/walloper_update.h"

extern int   fast_dec_timer() __asm__("func_001F9740");
extern float D_L00_001616A4 __attribute__((sda));
extern short D_L00_00161660_macro[] __asm__("D_L00_00161660") MACRO_ADDR;
short        D_L00_001DBED8[10] = {0};

short D_L01_001DC358[10] = {0};

short D_L02_001D0958[10] = {0};

short D_L03_001D0458[10] = {0};

short D_L04_001D04D8[10] = {0};

short D_L05_001D07F8[10] = {0};

short D_L06_001D0AD8[10] = {0};

short D_L07_001D03D8[10] = {0};

short D_L08_001D0B58[10] = {0};

short D_L09_001D04D8[10] = {0};

short D_L10_001D07D8[10] = {0};

short D_L11_001D0D58[10] = {0};

short D_L12_001E33F8[10] = {0};

short D_L13_001D0C68[10] = {0};

short D_L14_001DD318[10] = {0};

short D_L15_001D0DD8[10] = {0};

short D_L16_001D07D8[10] = {0};

short D_L17_001D1058[10] = {0};

short D_L18_001D0D58[10] = {0};

extern char walloper_effect_fade_in_duration_frames[4] __asm__("D_L00_001616A8")
    __attribute__((sda));
extern char walloper_effect_fade_out_duration_frames[4] __asm__("D_L00_001616AC")
    __attribute__((sda));
short D_L00_001DBEC0[10] = {0};

short D_L01_001DC340[10] = {0};

short D_L02_001D0940[10] = {0};

short D_L03_001D0440[10] = {0};

short D_L04_001D04C0[10] = {0};

short D_L05_001D07E0[10] = {0};

short D_L06_001D0AC0[10] = {0};

short D_L07_001D03C0[10] = {0};

short D_L08_001D0B40[10] = {0};

short D_L09_001D04C0[10] = {0};

short D_L10_001D07C0[10] = {0};

short D_L11_001D0D40[10] = {0};

short D_L12_001E33E0[10] = {0};

short D_L13_001D0C50[10] = {0};

short D_L14_001DD300[10] = {0};

short D_L15_001D0DC0[10] = {0};

short D_L16_001D07C0[10] = {0};

short D_L17_001D1040[10] = {0};

short D_L18_001D0D40[10] = {0};

extern short D_L00_001DBEF0[];
extern short D_L00_001DBF08[];
extern void  FUN_L00_002bbb00_c(int) __asm__("FUN_L00_002bbb00");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern int  func_001F9908_r(void *) __asm__("FUN_001f9740");

extern SparkleRowC860 l00_walloper_arc_predecessor_rows[] __asm__("D_L00_001DBB90");
void                  walloper_update(char *m) __asm__("FUN_L00_002bb558");

void walloper_update(char *walloper_moby) {
    enum {
        WALLOPER_STATE_INITIALIZE = 0,
        WALLOPER_STATE_ANIMATION_DRIVEN_EFFECTS = 2
    };
    int          flag = 0;
    float        arc_origin[4] __attribute__((aligned(16)));
    float        arc_step[4] __attribute__((aligned(16)));
    float        arc_point_camera_offset[4] __attribute__((aligned(16)));
    float        arc_point_offsets[5][4] __attribute__((aligned(16)));
    struct Hero *walloper_hero_state = &hero;
    int          i;
    int          next_arc_index;
    int          j;
    int          arc_init_index;
    int          any;

    if (*(unsigned char *)(((char *)walloper_hero_state->moby) + 0x53) != 0x82) {
        D_L00_0017AEBC = 0.1f;
    }
    switch (MOBY(walloper_moby)->state) {
    case WALLOPER_STATE_INITIALIZE:
        walloper_moby[0x20] = 2;
        for (arc_init_index = 0; arc_init_index < 10; arc_init_index++) {
            D_L00_001DBEC0[arc_init_index] = 0;
            D_L00_001DBEF0[arc_init_index] = 2;
            D_L00_001DBF08[arc_init_index] = 1;
        }
        D_L00_001616CC = scale_game_frames(*(int *)&walloper_effect_fade_in_duration_frames);
        walloper_effect_fade_out_countdown =
            scale_game_frames(*(int *)&walloper_effect_fade_out_duration_frames);
        break;
    case WALLOPER_STATE_ANIMATION_DRIVEN_EFFECTS:
        if (walloper_hero_state->state.current == 0x20) {
            float interpolated_record_value = compute_interpolated_record_value(walloper_moby);
            if (6.0f <= interpolated_record_value) {
                flag = 1;
                if (func_001F9908_r(&D_L00_00161660_macro[54]) == 0) {
                    float a = convert_integer_to_float(D_L00_001616CC);
                    a = a / convert_integer_to_float(scale_game_frames(
                                *(int *)&walloper_effect_fade_in_duration_frames));
                    walloper_effect_alpha_scale = 1.0f - a;
                } else if (12.0f <= interpolated_record_value) {
                    if (func_001F9908_r(&D_L00_00161660_macro[56]) == 0) {
                        float a = convert_integer_to_float(walloper_effect_fade_out_countdown);
                        a = a / convert_integer_to_float(scale_game_frames(
                                    *(int *)&walloper_effect_fade_out_duration_frames));
                        walloper_effect_alpha_scale = a;
                    } else {
                        walloper_effect_alpha_scale = 0.0f;
                    }
                } else {
                    walloper_effect_alpha_scale = 1.0f;
                }
            }
        } else {
            D_L00_001616CC = scale_game_frames(*(int *)&walloper_effect_fade_in_duration_frames);
            walloper_effect_fade_out_countdown =
                scale_game_frames(*(int *)&walloper_effect_fade_out_duration_frames);
            blend_moby_animation(walloper_moby, 1, 0, scale_game_frames(12));
        }
    }
    scale_vector_xyz(arc_origin, walloper_moby + 0xD0, -0.5f);
    fast_vec_add(arc_origin, arc_origin, walloper_moby + 0x10);
    any = 0;
    for (j = 0; j < 10; j++) {
        if (D_L00_001DBEC0[j] != 0) {
            any = 1;
            break;
        }
    }
    if (!flag && !any)
        return;
    for (i = 0; i < 10; i = next_arc_index) {
        D_L00_001DBEC0[i]--;
        if (D_L00_001DBEC0[i] < 0)
            D_L00_001DBEC0[i] = 0;
        if (D_L00_001DBEC0[i] == 0) {
            if (flag) {
                float sign;
                int   k;
                D_L00_001DBEC0[i] = 2;
                D_L00_001DBED8[i] = 0x40;
                random_spherical_offset(arc_step, D_L00_001616A4, D_L00_001616A4);
                qcopy(D_L00_001DBBA0[i].point[0], arc_origin);
                next_arc_index = i + 1;
                sign = 1.0f;
                for (k = 1; k < 5; k++) {
                    float bend_angle;
                    fast_vec_sub(arc_point_camera_offset,
                                 l00_walloper_arc_predecessor_rows[i].point[k - 1],
                                 D_L00_00166DC0);
                    bend_angle = random_float_between(0.17453292f, 0.7853982f) * sign;
                    sign = -sign;
                    build_look_at_matrix(arc_step, arc_step, arc_point_camera_offset, bend_angle);
                    fast_vec_add(D_L00_001DBBA0[i].point[k],
                                 l00_walloper_arc_predecessor_rows[i].point[k - 1],
                                 arc_step);
                }
            } else {
                next_arc_index = i + 1;
            }
        } else {
            int k;
            float(*workspace)[4] = arc_point_offsets;
            for (k = 1; k < 5; k++) {
                float *point_offset = workspace[k];
                fast_vec_sub(point_offset,
                             D_L00_001DBBA0[i].point[k],
                             l00_walloper_arc_predecessor_rows[i].point[k - 1]);
                point_offset[0] += FUN_L00_00257c48(0.0f, 0.1f);
                point_offset[1] += FUN_L00_00257c48(0.0f, 0.1f);
                point_offset[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            next_arc_index = i + 1;
            qcopy(D_L00_001DBBA0[i].point[0], arc_origin);
            for (k = 1; k < 5; k++) {
                fast_vec_add(D_L00_001DBBA0[i].point[k],
                             l00_walloper_arc_predecessor_rows[i].point[k - 1],
                             workspace[k]);
            }
            if (D_L00_001DBEC0[i] > 0)
                D_L00_001DBED8[i] = 0x20;
            else
                D_L00_001DBED8[i] = 0;
        }
    }
    add_draw_callback(FUN_L00_002bbb00_c, walloper_moby);
}

extern f32 D_L00_00161660_f[] __asm__("D_L00_00161660") MACRO_ADDR;
extern f32 walloper_arc_first_pass_half_width_gp __asm__("D_L00_00161690") __attribute__((sda));
extern f32 walloper_arc_second_pass_half_width_gp __asm__("D_L00_00161694") __attribute__((sda));
f32        convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
s32        truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
void       draw_walloper_arc_two_pass_quads(void *, s32, s32, s32, f32, f32, f32) __asm__("FUN_L00_002bbbc8");
void draw_walloper_camera_facing_quad(void *mv) __asm__("FUN_L00_002bc1a0");

void draw_walloper_camera_facing_quad(void *) __asm__("FUN_L00_002bc1a0");
void draw_walloper_arc_and_quad_effects(void *moby) __asm__("FUN_L00_002bbb00");

void draw_walloper_arc_and_quad_effects(void *moby) {
    s32 i;
    s32 arc_alpha;
    for (i = 0; i < 10; i++) {
        if (D_L00_001DBEC0[i] != 0) {
            arc_alpha = truncate_float_to_s32(convert_integer_to_float(D_L00_001DBED8[i]) *
                                              D_L00_00161660_f[29]);
            draw_walloper_arc_two_pass_quads(&D_L00_001DBBA0[i],
                                             arc_alpha,
                                             arc_alpha,
                                             5,
                                             walloper_arc_first_pass_half_width_gp,
                                             walloper_arc_second_pass_half_width_gp,
                                             0.2f);
        }
    }
    draw_walloper_camera_facing_quad(moby);
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BCED0), where it is exact; names translated to the US level program. */

typedef float RW[4] __attribute__((aligned(16)));

typedef struct {
    int   c[4];
    float f[8];
    long  z4, q0, q1, q2;
} RT;

extern char walloper_arc_first_pass_base_vertex_color_gp[4] __asm__("D_L00_00161680")
    __attribute__((sda));
extern char walloper_arc_second_pass_base_vertex_color_gp[4] __asm__("D_L00_00161684")
    __attribute__((sda));
extern char walloper_arc_first_pass_texture_index_gp[4] __asm__("D_L00_001616B0")
    __attribute__((sda));
extern char walloper_arc_second_pass_texture_index_gp[4] __asm__("D_L00_001616B4")
    __attribute__((sda));
extern void draw_geometry_quad(void *, void *, int);

void draw_walloper_arc_two_pass_quads(void *mv,
                                      int   first_pass_alpha,
                                      int   second_pass_alpha,
                                      int   point_count,
                                      float sx,
                                      float sy,

                                      float sz) {
    char *point_bytes = mv;
    RW    first_pass_corners[4];
    RT    first_pass_render_attributes;
    RW    q2[4];
    RT    second_pass_render_attributes;
    RW    arc_camera_facing_side_vector, second_pass_width_offset, first_pass_point_camera_offset,
        segment_delta, adjusted_segment_start, adjusted_segment_end, segment_start_camera_offset,
        segment_end_camera_offset, s1A0, s1B0;
    int   i, col0, col1;
    float side_vector_normalization_factor, initial_side_vector_normalization_factor,
        segment_start_camera_distance, segment_end_camera_distance;
    first_pass_render_attributes.q0 =
        get_effect_texture(*(int *)&walloper_arc_first_pass_texture_index_gp);
    col0 = *(int *)&walloper_arc_first_pass_base_vertex_color_gp | (first_pass_alpha << 24);
    first_pass_render_attributes.q2 = 0x8000000048L;
    first_pass_render_attributes.q1 = 0xFF9000000260L;
    first_pass_render_attributes.z4 = 0;
    first_pass_render_attributes.c[3] = col0;
    first_pass_render_attributes.c[2] = col0;
    first_pass_render_attributes.c[1] = col0;
    first_pass_render_attributes.c[0] = col0;
    second_pass_render_attributes.q0 =
        get_effect_texture(*(int *)&walloper_arc_second_pass_texture_index_gp);
    col1 = *(int *)&walloper_arc_second_pass_base_vertex_color_gp | (second_pass_alpha << 24);
    second_pass_render_attributes.q2 = first_pass_render_attributes.q2;
    second_pass_render_attributes.q1 = first_pass_render_attributes.q1;
    second_pass_render_attributes.z4 = first_pass_render_attributes.z4;
    second_pass_render_attributes.c[3] = col1;
    second_pass_render_attributes.c[2] = col1;
    second_pass_render_attributes.c[1] = col1;
    second_pass_render_attributes.c[0] = col1;
    second_pass_render_attributes.f[0] = 0;
    second_pass_render_attributes.f[1] = 0;
    second_pass_render_attributes.f[2] = 0;
    second_pass_render_attributes.f[3] = 1.0f;
    second_pass_render_attributes.f[4] = 1.0f;
    second_pass_render_attributes.f[5] = 0;
    second_pass_render_attributes.f[6] = 1.0f;
    second_pass_render_attributes.f[7] = 1.0f;
    first_pass_render_attributes.f[0] = 0;
    first_pass_render_attributes.f[1] = 0;
    first_pass_render_attributes.f[2] = 0;
    first_pass_render_attributes.f[3] = 1.0f;
    first_pass_render_attributes.f[4] = 1.0f;
    first_pass_render_attributes.f[5] = 0;
    first_pass_render_attributes.f[6] = 1.0f;
    first_pass_render_attributes.f[7] = 1.0f;
    fast_vec_sub(first_pass_point_camera_offset, point_bytes, D_L00_00166DC0);
    fast_vec_sub(segment_delta, point_bytes + 0x10, point_bytes);
    fast_vec_cross(arc_camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
    initial_side_vector_normalization_factor = fast_vec_length(arc_camera_facing_side_vector);
    if (initial_side_vector_normalization_factor != 0)
        initial_side_vector_normalization_factor = 1.0f / initial_side_vector_normalization_factor;
    scale_vector_xyz(arc_camera_facing_side_vector,
                     arc_camera_facing_side_vector,
                     sx * initial_side_vector_normalization_factor);
    fast_vec_add(first_pass_corners[0], point_bytes, arc_camera_facing_side_vector);
    fast_vec_sub(first_pass_corners[1], point_bytes, arc_camera_facing_side_vector);
    for (i = 1; i < point_count - 1; i++) {
        fast_vec_sub(first_pass_point_camera_offset, point_bytes + i * 16, D_L00_00166DC0);
        fast_vec_sub(segment_delta, point_bytes + i * 16 + 16, point_bytes + i * 16);
        fast_vec_cross(
            arc_camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
        side_vector_normalization_factor = fast_vec_length(arc_camera_facing_side_vector);
        if (side_vector_normalization_factor != 0)
            side_vector_normalization_factor = 1.0f / side_vector_normalization_factor;
        scale_vector_xyz(arc_camera_facing_side_vector,
                         arc_camera_facing_side_vector,
                         sx * side_vector_normalization_factor);
        fast_vec_add(first_pass_corners[2], point_bytes + i * 16, arc_camera_facing_side_vector);
        fast_vec_sub(first_pass_corners[3], point_bytes + i * 16, arc_camera_facing_side_vector);
        if (i == 1) {
            first_pass_render_attributes.c[1] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[0] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
        } else if (i == point_count - 2) {
            first_pass_render_attributes.c[3] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[2] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
        } else if (i == 2) {
            first_pass_render_attributes.c[1] = col0;
            first_pass_render_attributes.c[0] = col0;
        }
        draw_geometry_quad(first_pass_corners, 0, 0);
        qcopy(first_pass_corners[0], first_pass_corners[2]);
        qcopy(first_pass_corners[1], first_pass_corners[3]);
    }
    for (i = 0; i < point_count - 1; i++) {
        fast_vec_sub(segment_start_camera_offset, point_bytes + i * 16, D_L00_00166DC0);
        fast_vec_sub(segment_end_camera_offset, point_bytes + i * 16 + 16, D_L00_00166DC0);
        segment_start_camera_distance = fast_vec_length(segment_start_camera_offset);
        segment_end_camera_distance = fast_vec_length(segment_end_camera_offset);
        if (segment_end_camera_distance < segment_start_camera_distance) {
            if (segment_end_camera_distance == 0)
                qcopy(segment_end_camera_offset, segment_start_camera_offset);
            else
                scale_vector_xyz(segment_end_camera_offset,
                                 segment_end_camera_offset,
                                 segment_start_camera_distance / segment_end_camera_distance);
        } else {
            if (segment_start_camera_distance == 0)
                qcopy(segment_start_camera_offset, segment_end_camera_offset);
            else
                scale_vector_xyz(segment_start_camera_offset,
                                 segment_start_camera_offset,
                                 segment_end_camera_distance / segment_start_camera_distance);
        }
        fast_vec_add(adjusted_segment_start, D_L00_00166DC0, segment_start_camera_offset);
        fast_vec_add(adjusted_segment_end, D_L00_00166DC0, segment_end_camera_offset);
        fast_vec_sub(segment_delta, adjusted_segment_end, adjusted_segment_start);
        scale_vector_xyz(s1B0, segment_delta, sz);
        fast_vec_sub(adjusted_segment_start, adjusted_segment_start, s1B0);
        fast_vec_add(adjusted_segment_end, adjusted_segment_end, s1B0);
        fast_vec_sub(s1A0, D_L00_00166DC0, adjusted_segment_start);
        fast_vec_cross(arc_camera_facing_side_vector, s1A0, segment_delta);
        side_vector_normalization_factor = fast_vec_length(arc_camera_facing_side_vector);
        if (side_vector_normalization_factor != 0)
            side_vector_normalization_factor = 1.0f / side_vector_normalization_factor;
        scale_vector_xyz(second_pass_width_offset,
                         arc_camera_facing_side_vector,
                         sy * side_vector_normalization_factor);
        fast_vec_add(q2[0], adjusted_segment_start, second_pass_width_offset);
        fast_vec_sub(q2[1], adjusted_segment_start, second_pass_width_offset);
        fast_vec_add(q2[2], adjusted_segment_end, second_pass_width_offset);
        fast_vec_sub(q2[3], adjusted_segment_end, second_pass_width_offset);
        if (i == 0) {
            second_pass_render_attributes.c[1] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
            second_pass_render_attributes.c[0] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
        } else if (i == point_count - 2) {
            second_pass_render_attributes.c[3] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
            second_pass_render_attributes.c[2] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
        } else if (i == 1) {
            second_pass_render_attributes.c[1] = col1;
            second_pass_render_attributes.c[0] = col1;
        }
        draw_geometry_quad(q2, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2bc1a0;

typedef struct {
    V_2bc1a0     corner[4];
    unsigned int color[4];
    int          unk50, unk54, unk58;
    float        unk5C, unk60;
    int          unk64;
    float        unk68, unk6C;
    long         unk70, tex, unk80, unk88;
} Q_2bc1a0;

void         FUN_001f9a68_2bc1a0(float, void *, void *) __asm__("FUN_001f9a68");
void         FUN_001f9bf8_2bc1a0(void *, void *, float) __asm__("FUN_001f9bf8");
float        FUN_001fa6c0_2bc1a0(int) __asm__("FUN_001fa6c0");
int          FUN_001fa6d0_2bc1a0(float) __asm__("FUN_001fa6d0");
extern float walloper_quad_basis_offset_scale_gp __asm__("D_L00_001616BC") __attribute__((sda));
extern float walloper_quad_corner_scale_gp __asm__("D_L00_001616B8") __attribute__((sda));
extern int   walloper_quad_texture_index_gp __asm__("D_L00_001616C8") __attribute__((sda));
extern unsigned int l00_walloper_quad_common_base_vertex_color __asm__("D_L00_001616C0")
    __attribute__((sda));
extern unsigned int l00_walloper_quad_rare_base_vertex_color __asm__("D_L00_001616C4")
    __attribute__((sda));
extern V_2bc1a0 D_L00_001DBB60[] NOT_SDA;

void draw_walloper_camera_facing_quad(void *mv) {
    char        *moby_bytes = mv;
    Q_2bc1a0     quad;
    V_2bc1a0     camera_facing_transform[4];
    V_2bc1a0     scaled_corner;
    V_2bc1a0     scaled_basis_offset;
    V_2bc1a0     camera_facing_offset;
    unsigned int uniform_vertex_color;
    int          i;
    FUN_001f9a68_2bc1a0(
        walloper_quad_basis_offset_scale_gp, &scaled_basis_offset, moby_bytes + 0xD0);
    fast_vec_add(&camera_facing_transform[3], &scaled_basis_offset, moby_bytes + 0x10);
    quad.tex = get_effect_texture(walloper_quad_texture_index_gp);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    quad.unk50 = 0;
    quad.unk54 = 0;
    quad.unk58 = 0;
    quad.unk5C = 1.0f;
    quad.unk60 = 1.0f;
    quad.unk64 = 0;
    quad.unk68 = 1.0f;
    quad.unk6C = 1.0f;
    if (!random_integer_below(4))
        uniform_vertex_color = l00_walloper_quad_rare_base_vertex_color;
    else
        uniform_vertex_color = l00_walloper_quad_common_base_vertex_color;
    uniform_vertex_color =
        (uniform_vertex_color & 0xFFFFFF) |
        (FUN_001fa6d0_2bc1a0(FUN_001fa6c0_2bc1a0((int)uniform_vertex_color >> 24) *
                             D_L00_00161660_f[29])
         << 24);
    quad.color[3] = uniform_vertex_color;
    quad.color[2] = uniform_vertex_color;
    quad.color[1] = uniform_vertex_color;
    quad.color[0] = uniform_vertex_color;
    fast_vec_sub(
        &camera_facing_offset, &(*(V_2bc1a0 *)g_debug_camera), &camera_facing_transform[3]);
    FUN_001f9bf8_2bc1a0(&camera_facing_transform[0], &camera_facing_offset, 1.0f);
    fast_vec_cross(
        &camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2bc1a0 *)D_0013F5E0));
    FUN_001f9bf8_2bc1a0(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    fast_vec_cross(
        &camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    FUN_001f9a68_2bc1a0(0.2f, &camera_facing_offset, &camera_facing_transform[0]);
    fast_vec_add(&camera_facing_transform[3], &camera_facing_transform[3], &camera_facing_offset);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2bc1a0(walloper_quad_corner_scale_gp, &scaled_corner, &D_L00_001DBB60[3 - i]);
        transform_vector(&quad.corner[3 - i], &scaled_corner, &camera_facing_transform[0]);
    }
    draw_geometry_quad(&quad, 0, 0);
}

#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BD6B8), where it is exact; names translated to the US level program. */

typedef u32 VecQuad __attribute__((mode(TI), aligned(16)));

extern Vec4  collision_hit_point __asm__("D_L00_00173E60");
extern f32   fast_arc_sin(f32) __asm__("func_001F9DF8");
extern float fast_vec_dist(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int   D_L00_00173E40[];
extern float D_L00_001616DC __attribute__((sda));

#include "rnc/overlay/shared/weapons/walloper_data/select_morphoray_target_and_adjust_aim.h"

float absolute_float(float input) __asm__("FUN_001f99c0");

char *select_morphoray_target_and_adjust_aim(char  *self,
                                             float *position,
                                             float *heading,
                                             float *pitch) __asm__("FUN_L00_002bc3b0");

char *

select_morphoray_target_and_adjust_aim(char *self, float *position, float *heading, float *pitch) {
    float        start[4] __attribute__((aligned(16)));
    float        end[4] __attribute__((aligned(16)));
    float        target[4] __attribute__((aligned(16)));
    float        offset[4] __attribute__((aligned(16)));
    char        *candidate;
    char        *data;
    struct Moby *moby;
    int          collision;
    int          a0s[1];
    int          selected_target[1];
    float        distance, angle, elevation, headingError, limit, apparentAngle, radius, reduction;
    int          i;
    char        *hero_base;
    char        *hero_moby;

    limit = D_L00_001616DC;
    a0s[0] = (int)self;
    *(VecQuad *)start = *(VecQuad *)position;
    hero_base = ((char *)&hero);
    hero_moby = *(char **)(hero_base + 0x2080);
    *(VecQuad *)end = *(VecQuad *)&MOBY(hero_moby)->pos;
    position = start;
    end[2] = position[2];
    collision = collision_line((Vec4f *)end, (Vec4f *)position, 6, hero_moby, 0);
    selected_target[0] = 0;
    if (collision) {
        qcopy(position, &collision_hit_point);
        return 0;
    }
    i = 0;
    if ((candidate = active_mobies[i]) != 0)
        do {
            if (*(signed char *)(candidate + 0x20) < 0 || *(short *)(candidate + 0xA6) == 0x58E ||
                *(short *)(candidate + 0xA6) == 0x452)
                continue;
            qcopy(target, candidate + 0x10);
            data = (char *)suck_get_aim_height(candidate);
            if (!data)
                continue;
            target[2] += *(float *)(data + 0x10);
            if (!candidate || !*(char **)(candidate + 0x24) ||
                *(short *)(*(char **)(candidate + 0x24) + 0x46) != 5)
                continue;
            angle = fast_arc_tan(target[0] - position[0], target[1] - position[1]);
            distance = vector_distance(position, target);
            elevation = fast_arc_tan(distance, target[2] - position[2]);
            headingError = fast_difference_between_rotations(*heading, angle);
            distance = fast_vec_dist(position, target);
            if (distance < 2.5f) {
                float toward;
                char *close_range_hero_base = ((char *)&hero);
                moby = *(char **)(close_range_hero_base + 0x2080);
                toward = fast_arc_tan(
                    *(float *)(candidate + 0x10) - *(float *)(close_range_hero_base + 0x80),
                    *(float *)(candidate + 0x14) - *(float *)(close_range_hero_base + 0x84));
                if (fast_difference_between_rotations(moby->rot.z, toward) < 1.0471976f &&
                    absolute_float(position[2] - target[2]) < 2.0f &&
                    !collision_line((Vec4f *)position,
                                    (Vec4f *)target,
                                    6,
                                    *(char **)(close_range_hero_base + 0x2080),
                                    0)) {
                    selected_target[0] = (int)candidate;
                    *pitch = -elevation;
                    *heading = angle;
                    goto done;
                }
            }
            if (distance > limit)
                continue;
            {
                char *aim_offset_hero_base = ((char *)&hero);
                build_spherical_offset(offset,
                                       distance,
                                       *heading,
                                       *pitch + *(float *)(aim_offset_hero_base + 0x2E4) * 0.5f);
            }
            fast_vec_add(offset, offset, position);
            apparentAngle = fast_arc_sin(fast_vec_dist(offset, target) / (distance + distance));
            apparentAngle += apparentAngle;
            if (morphoray_target_angle_threshold_degrees_gp * 0.017453292f < apparentAngle) {
                data = (char *)suck_get_aim_height(candidate);
                if (data) {
                    radius = convert_integer_to_float(*(unsigned char *)(data + 10)) * 0.125f;
                    reduction = fast_arc_sin(radius / distance);
                    if (radius < distance) {
                        apparentAngle -= reduction;
                        if (apparentAngle < 0.0f)
                            apparentAngle = 0.0f;
                    }
                }
            }
            if (apparentAngle < morphoray_target_angle_threshold_degrees_gp * 0.017453292f) {
                int valid = 1;
                if (collision_line((Vec4f *)position, (Vec4f *)target, 6, self, 0)) {
                    if (!D_L00_00173E40[6] || (char *)D_L00_00173E40[6] != candidate)
                        valid = 0;
                }
                if (valid) {
                    *pitch = -elevation;
                    limit = distance;
                    if (headingError < 0.17453292f)
                        *heading = angle;
                    selected_target[0] = (int)candidate;
                }
            }
        } while ((candidate = active_mobies[++i]) != 0);
done:
    return (char *)selected_target[0];
}

#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/hud.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BDAF0), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef int u128_2BDAF0 __attribute__((mode(TI)));

typedef struct Vq {
    float x, y, z, w;
} __attribute__((aligned(16))) Vq;

typedef struct Ray {
    float pos[4];
    Moby *target;
    float total;
    float left;
    short timer;
    short seen;
    char  p0[0x34 - 0x20];
    int   snd;
    char  p1[0x44 - 0x38];
    int   delay;
    float yaw;
    float pitch;
} Ray;

typedef struct Stats {
    char           p0[0xA8];
    unsigned short kills;
    unsigned short best;
    int            levels;
} Stats;

typedef struct Voice {
    char          p0[0x74];
    unsigned char on;
    char          p1[0x88 - 0x75];
    Moby         *owner;
} Voice;

extern unsigned char D_00141680_r[] __asm__("D_00141680");
extern f32           vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float         D_0015ED6C;
extern int           D_00141680;
extern int           D_0015EEA4;
extern float         morphoray_base_drain_rate_gp __asm__("D_L00_001616E0") __attribute__((sda));
extern int           D_L00_001616EC __attribute__((sda));
extern int           D_L00_001617BC;
extern int           D_L00_001617C8;
extern int           transpose_rotation_matrix() __asm__("FUN_001fa2d8");
extern int           FUN_L00_002223f8();
extern int           draw_uniform_size_hud_stars() __asm__("FUN_L00_00236e50");
extern int           FUN_L00_0024f7c8();
extern void          fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void          scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void          tick_countdown_32_c(s32 *) __asm__("func_001F9740");
extern void          transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
int                  allocate_packed_color_point_light_slot(void *v,
                                                            float a,
                                                            float b,
                                                            int   col) __asm__("FUN_L00_0023e5e0");
struct Moby         *spawn_morphoray_chicken(void *a) __asm__("FUN_L00_002c9348");
void                 start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
void                 hud_init_particles(HudElem *e) __asm__("FUN_L00_00236268");
void                 hud_update_particles(void) __asm__("FUN_L00_00236458");
void                 FUN_L00_00263d40(int a, int b);
extern char          D_0013A5E0_2BDAF0[] __asm__("D_0013CAE0");
extern unsigned char D_0013E633_2BDAF0[] __asm__("D_0013E533");
extern char          voice_pool[] __asm__("D_0013E550");
extern int           func_001F9938_2BDAF0(void *) __asm__("FUN_001f9770");
extern void          start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern int           func_001F9850_2BDAF0(int) __asm__("FUN_001f96f8");
extern void          hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void          copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void          func_001FA4A0_2BDAF0(void *, void *) __asm__("FUN_001fa2d8");
extern void          func_001F9EE8_2BDAF0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void          func_001F9C30_2BDAF0(void *, void *, float) __asm__("FUN_001f9a68");
extern void          transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float         func_001F9CE8_2BDAF0(void *) __asm__("FUN_001f9b20");
extern int           func_001F9908_2BDAF0(void *) __asm__("FUN_001f9740");
extern float         func_001FA888_2BDAF0(int) __asm__("FUN_001fa6c0");
extern int
queue_animation_update(int, int, void *, void *, void *, int *, int) __asm__("FUN_001ff308");
extern int  func_001FA898_2BDAF0(float) __asm__("FUN_001fa6d0");
extern void func_L00_002BE3A8_2BDAF0(void *) __asm__("FUN_L00_002bd0a0");
extern void func_L00_001FF4B0_2BDAF0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_2BDAF0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void hud_update_particles(void) __asm__("FUN_L00_00236458");

void morphoray_update(Moby *moby) __asm__("FUN_L00_002bc7e8");

void morphoray_update(Moby *moby) {
    Vq    direction;
    Vq    points[3];
    Vq    matrix[4];
    Ray  *data;
    int   busy;
    int   in_range;
    Moby *hit;
    int   now;

    busy = 0;
    if (((Hero *)((char *)&hero))->items[0].state == 3) {
        busy = 1;
    }
    data = (struct Ray *)moby->pvars;
    if (moby->state == 0) {
        moby->state = 1;
        morphoray_point_light_slot = -1;
        D_L00_001617C8 = -1;
        morphoray_light_fade_countdown = 0;
    }
    in_range = 1;
    if (moby->prev_seq == 0 && compute_interpolated_record_value(moby) < 6.0f) {
        in_range = 0;
    }
    func_001F9938_2BDAF0(&data->timer);
    if ((((unsigned int)((Hero *)((char *)&hero))->state.control_mode < 2 ||
          ((Hero *)((char *)&hero))->state.control_mode == 12 ||
          ((Hero *)((char *)&hero))->state.control_mode == 4 ||
          ((Hero *)((char *)&hero))->state.control_mode == 5)) &&
        (*(int *)D_0013A5E0_2BDAF0 & ((Hero *)((char *)&hero))->items[0].button_mask) != 0 &&
        !busy && in_range) {
        if (((Hero *)((char *)&hero))->item_anim_active == 0) {
            start_equipped_item_animation();
        }
        if (data->seen == 0) {
            data->seen = 1;
            if (((Stats *)(D_00141680_r))->kills <= 0xFFFE) {
                ((Stats *)(D_00141680_r))->kills++;
            }
            now = func_001F9850_2BDAF0(D_0015EEA4) / 600;
            if (((Stats *)(D_00141680_r))->best < now) {
                ((Stats *)(D_00141680_r))->best = func_001F9850_2BDAF0(D_0015EEA4) / 600;
            }
            ((Stats *)(D_00141680_r))->levels =
                ((Stats *)(D_00141680_r))->levels | (1 << current_level_index) | 0x80000000;
        }
        if (data->timer == 0) {
            data->timer = func_001F9850_2BDAF0(15);
        }
        moby_point_position(moby, 0, data);
        if (((Hero *)((char *)&hero))->state.current == 1) {
            hero_set_state(30, 1);
        }
        *(u128_2BDAF0 *)&direction = 0;
        if (((Hero *)((char *)&hero))->state.current == 30 &&
            (*(unsigned short *)((char *)((Hero *)((char *)&hero))->moby + 0x34) & 1)) {
            copy_matrix3x4(points, l00_camera.unk0);
            func_001FA4A0_2BDAF0(matrix, points);
            direction.z = 1.0f;
            func_001F9EE8_2BDAF0(&direction, &direction, matrix);
        } else if (((Hero *)((char *)&hero))->unk22C8 == 1) {
            func_001F9C30_2BDAF0(&direction, &moby->rot_row1, -1.0f);
        } else {
            direction.x = 1.0f;
            transform_vector_by_basis(
                &direction, &direction, ((Hero *)((char *)&hero))->basis[0].f);
        }
        data->yaw = fast_arc_tan(direction.x, direction.y);
        data->pitch = fast_arc_tan(func_001F9CE8_2BDAF0(&direction), direction.z);
        *(u128_2BDAF0 *)&points[0] = *(u128_2BDAF0 *)data->pos;
        hit = select_morphoray_target_and_adjust_aim(moby, &points[0], &data->yaw, &data->pitch);
        func_001F9908_2BDAF0(&data->delay);
        if (data->target != 0) {
            if (data->target->state == 0xFE) {
                data->target = 0;
                data->delay = 0;
            } else if (data->target->state == 0xFD) {
                data->target = 0;
                data->delay = 0;
            }
        }
        if (hit != 0 || data->delay != 0) {
            if (hit != 0) {
                float *target_health = suck_get_aim_height(hit);
                if (hit != data->target && data->delay == 0) {
                    data->target = hit;
                    data->total = func_001FA888_2BDAF0(((short *)target_health)[2]);
                    data->left = target_health[0];
                    data->delay = func_001F9850_2BDAF0(30);
                    if (data->total <= 1.0f) {
                        data->left = 0.0f;
                    }
                    data->snd = queue_animation_update(4,
                                                       0x7533,
                                                       hud_init_particles,
                                                       hud_update_particles,
                                                       draw_uniform_size_hud_stars,
                                                       &D_L00_001616EC,
                                                       10000);
                }
            }
            {
                float level_bonus = func_001FA888_2BDAF0(D_0013E633_2BDAF0[2]) * 0.33f + 1.0f;
                float remaining =
                    data->left - morphoray_base_drain_rate_gp * D_0015ED6C * level_bonus;
                data->left = remaining;
                if (remaining < 0.0f) {
                    data->left = 0.0f;
                }
            }
            D_L00_001616EC = 10000 - func_001FA898_2BDAF0(data->left * 10000.0f / data->total);
        }
        if (data->target != 0 && data->left <= 0.0f) {
            if (current_level_index == 5 && data->target->oclass == 0x271 &&
                item_unlocked[8] == 0) {
                item_unlocked[8] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
            spawn_morphoray_chicken(data->target);
            data->target = 0;
        }
        func_L00_002BE3A8_2BDAF0(moby);
        if (morphoray_point_light_slot == -1) {
            morphoray_point_light_slot =
                allocate_packed_color_point_light_slot((float *)&moby->pos, 7.5f, 0.0f, 0x207F7F);
        }
        if (morphoray_point_light_slot >= 0) {
            morphoray_light_fade_countdown = func_001F9850_2BDAF0(20);
            func_L00_001FF4B0_2BDAF0(&points[0], &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[1], data, &points[0]);
            point_lights[morphoray_point_light_slot].position.f[0] = points[1].x;
            point_lights[morphoray_point_light_slot].position.f[1] = points[1].y;
            point_lights[morphoray_point_light_slot].position.f[2] = points[1].z;
            point_lights[morphoray_point_light_slot].position.f[3] = 7.5f;
        }
        if (voice_is_playing(moby, D_L00_001617C8) == 0) {
            D_L00_001617C8 = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else {
        data->target = 0;
        data->seen = 0;
        data->delay = 0;
        if (data->snd != -1) {
            remove_hud_item(data->snd);
            data->snd = -1;
        }
        D_L00_001617BC = 1;
        if (morphoray_point_light_slot >= 0) {
            float brightness;
            func_L00_001FF4B0_2BDAF0(&direction, &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[0], data, &direction);
            point_lights[morphoray_point_light_slot].position.f[0] = points[0].x;
            point_lights[morphoray_point_light_slot].position.f[1] = points[0].y;
            point_lights[morphoray_point_light_slot].position.f[2] = points[0].z;
            point_lights[morphoray_point_light_slot].position.f[3] = 7.5f;
            brightness = func_001FA888_2BDAF0(morphoray_light_fade_countdown) * 0.05f;
            point_lights[morphoray_point_light_slot].color[0] = brightness;
            point_lights[morphoray_point_light_slot].color[1] = brightness;
            point_lights[morphoray_point_light_slot].color[2] = brightness * 0.25f;
            if (func_001F9908_2BDAF0(&morphoray_light_fade_countdown) != 0) {
                release_point_light_slot(morphoray_point_light_slot);
                morphoray_point_light_slot = -1;
            }
        }
        if (morphoray_point_light_slot >= 0 && morphoray_light_fade_countdown == 0) {
            release_point_light_slot(morphoray_point_light_slot);
            morphoray_point_light_slot = -1;
        }
        if (D_L00_001617C8 >= 0) {
            if (D_L00_001617C8 != -1 &&
                ((Voice *)(voice_pool + D_L00_001617C8 * 0x70))->owner == moby &&
                ((Voice *)(voice_pool + D_L00_001617C8 * 0x70))->on != 0) {
                release_voice_slot(D_L00_001617C8);
            }
            D_L00_001617C8 = -1;
        }
        cancel_hero_item_animation();
    }
    if (busy && morphoray_point_light_slot >= 0) {
        release_point_light_slot(morphoray_point_light_slot);
        morphoray_point_light_slot = -1;
    }
}

extern f32                  D_L00_001616DC_gp __asm__("D_L00_001616DC") __attribute__((sda));
extern f32                  D_L00_001617CC_macro __asm__("D_L00_001617CC") MACRO_ADDR;
extern s32                  D_L00_00161718_gp __asm__("D_L00_00161718") __attribute__((sda));
extern s32                  D_L00_00161720_gp __asm__("D_L00_00161720") __attribute__((sda));
extern s32                  D_L00_00161724_gp __asm__("D_L00_00161724") __attribute__((sda));
extern float D_L00_001617D0 MACRO_ADDR;
extern s32                  D_L00_00161824;
extern s32                  D_L00_00161818;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2BE3A8;

extern Vec_2BE3A8 beam_prev_nodes[12] __asm__("D_L00_001DBF50");
extern Vec_2BE3A8 beam_nodes[] __asm__("D_L00_001DBF60") NOT_SDA;
extern Vec_2BE3A8 beam_pulse_directions[3] __asm__("D_L00_001DD0D0");
extern void       scale_vector_2bcf98(float, void *, void *) __asm__("FUN_001f9a68");

void reset_beam_chain_and_pulse_state(u8 *moby) __asm__("FUN_L00_002bcf98");

void reset_beam_chain_and_pulse_state(u8 *moby) {
    u128  step;
    u128 *source = *(u128 **)(moby + 0x78);
    s32   i;
    s32   j;
    f32   scale;

    if (D_L00_001617BC != 0) {
        D_L00_001617BC = 0;
        scale = D_L00_001616DC_gp / convert_integer_to_float(12);
        D_L00_001617CC_macro = scale;
        scale_vector_2bcf98(-scale, &step, moby + 0xD0);
        qcopy(&beam_nodes[0], source);
        for (i = 1; i < 12; i++) {
            fast_vec_add(&beam_nodes[i], (u128 *)beam_prev_nodes + i, &step);
        }
        D_L00_00161718_gp = 0;
        D_L00_00161720_gp = 0;
        D_L00_00161724_gp = 0;
        D_L00_001617D0 = 0.0f;
        D_L00_00161824 = 0;
        for (j = 0; j < 3; j++) {
            (&D_L00_00161818)[j] = 0;
            qcopy((u128 *)beam_pulse_directions + j, &step);
        }
    }
}
/* Tesla claw arc: bends the twelve-point beam toward the target, builds the two side rails and the ring of
 * twenty points around each node, queues the draw callback, sparks at the tip, and runs the three travelling
 * pulses along the beam. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BE3A8), where it is exact; names translated to the US level program. */

typedef struct {
    Vec_2BE3A8   pos;
    struct Moby *target;
    char         pad14[0x34];
    float        f48;
    float        f4C;
} Vars;

typedef struct {
    char  pad0[0x10];
    float height;
} Info;

typedef struct {
    int c[5];
} Tab;

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_t;

extern unsigned char D_0013F5E0_r[] __asm__("D_0013F5E0");
extern f32           vector_length_xyz_c(void *) __asm__("FUN_001f9af0");
extern float         D_L00_001616DC_gp __asm__("D_L00_001616DC") __attribute__((sda));
extern signed char   D_L00_00161763;
extern float         D_L00_001617CC;
extern int           D_L00_001617E8;
extern int           D_L00_00161808;
extern int           D_L00_00161818;
extern int           D_L00_00161824;
extern int           D_L00_001DBF70;
extern int           FUN_001efa68_c_c(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern int           lerp_vector() __asm__("FUN_001f9a40");
extern void          scale_vector_xyz_c3(void *, void *, float) __asm__("FUN_001f9a68");
unsigned char *
FUN_L00_00272060(void *pos, int s, float x, float y, int color, int mode, int b, float *v, float z);
unsigned char                    *
spawn_proximity_corrected_size_alpha_tween_particle(void  *pos,
                                                                       int    s,
                                                                       int    col,
                                                                       int    mode,
                                                                       int    b,
                                                                       float *vec,
                                                                       int    h,
                                                                       float  x,
                                                                       float  y) __asm__("FUN_L00_00275910");
void               draw_beam_ring_and_rail_effects(void *o) __asm__("FUN_L00_002bdc50");
extern float       D_L00_001616F0_gp __asm__("D_L00_001616F0") __attribute__((sda));
extern int         D_L00_001616F4_gp __asm__("D_L00_001616F4") __attribute__((sda));
extern float       D_L00_001616F8_gp __asm__("D_L00_001616F8") __attribute__((sda));
extern float       D_L00_001616FC_gp __asm__("D_L00_001616FC") __attribute__((sda));
extern float       D_L00_00161704_gp __asm__("D_L00_00161704") __attribute__((sda));
extern float       D_L00_00161708_gp __asm__("D_L00_00161708") __attribute__((sda));
extern float       D_L00_0016170C_gp __asm__("D_L00_0016170C") __attribute__((sda));
extern float       D_L00_0016172C_gp __asm__("D_L00_0016172C") __attribute__((sda));
extern float       D_L00_00161730_gp __asm__("D_L00_00161730") __attribute__((sda));
extern float       D_L00_00161734_gp __asm__("D_L00_00161734") __attribute__((sda));
extern float       D_L00_0016173C_gp __asm__("D_L00_0016173C") __attribute__((sda));
extern float       D_L00_00161740_gp __asm__("D_L00_00161740") __attribute__((sda));
extern float       D_L00_00161748_gp __asm__("D_L00_00161748") __attribute__((sda));
extern int         D_L00_0016174C_gp __asm__("D_L00_0016174C") __attribute__((sda));
extern int         D_L00_00161750_gp __asm__("D_L00_00161750") __attribute__((sda));
extern float       D_L00_00161758_gp __asm__("D_L00_00161758") __attribute__((sda));
extern float       D_L00_0016175C_gp __asm__("D_L00_0016175C") __attribute__((sda));
extern float       D_L00_00161768_gp __asm__("D_L00_00161768") __attribute__((sda));
extern signed char D_L00_00161763_macro[] __asm__("D_L00_00161763") MACRO_ADDR;
extern float       D_L00_001617CC_macro __asm__("D_L00_001617CC") MACRO_ADDR;
extern int         D_L00_00161824_macro __asm__("D_L00_00161824") MACRO_ADDR;
extern float       frame_scale __asm__("D_0015ED60") MACRO_ADDR;
extern float       D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern int         D_L00_001617D8_macro[3] __asm__("D_L00_001617D8") MACRO_ADDR;
extern float       D_L00_001618B8_2BE3A8[3] __asm__("D_L00_001617E8") MACRO_ADDR;
extern float       D_L00_001617F8_macro[3] __asm__("D_L00_001617F8") MACRO_ADDR;
extern float       D_L00_001618D8_2BE3A8[3] __asm__("D_L00_00161808") MACRO_ADDR;
extern int         D_L00_001618E8_2BE3A8[3] __asm__("D_L00_00161818") MACRO_ADDR;
extern Vec_2BE3A8  beam_nodes[] __asm__("D_L00_001DBF60") NOT_SDA;
extern Vec_2BE3A8  beam_rail_a_points[12] __asm__("D_L00_001DC020");
extern Vec_2BE3A8  beam_rail_b_points[12] __asm__("D_L00_001DC0E0");
Vec_2BE3A8         D_L00_001DC1A0_2BE3A8[12][20] __asm__("D_L00_001DC1A0") = {0};

Vec_2BE3A8 D_L01_001DC620[12][20] = {0};

Vec_2BE3A8 D_L02_001D0C20[12][20] = {0};

Vec_2BE3A8 D_L03_001D0720[12][20] = {0};

Vec_2BE3A8 D_L04_001D07A0[12][20] = {0};

Vec_2BE3A8 D_L05_001D0AC0[12][20] = {0};

Vec_2BE3A8 D_L06_001D0DA0[12][20] = {0};

Vec_2BE3A8 D_L07_001D06A0[12][20] = {0};

Vec_2BE3A8 D_L08_001D0E20[12][20] = {0};

Vec_2BE3A8 D_L09_001D07A0[12][20] = {0};

Vec_2BE3A8 D_L10_001D0AA0[12][20] = {0};

Vec_2BE3A8 D_L11_001D1020[12][20] = {0};

Vec_2BE3A8 D_L12_001E36C0[12][20] = {0};

Vec_2BE3A8 D_L13_001D0F30[12][20] = {0};

Vec_2BE3A8 D_L14_001DD5E0[12][20] = {0};

Vec_2BE3A8 D_L15_001D10A0[12][20] = {0};

Vec_2BE3A8 D_L16_001D0AA0[12][20] = {0};

Vec_2BE3A8 D_L17_001D1320[12][20] = {0};

Vec_2BE3A8 D_L18_001D1020[12][20] = {0};

extern Vec_2BE3A8 beam_pulse_positions[3] __asm__("D_L00_001DD0A0");
extern Vec_2BE3A8 D_L00_00173F60_2BE3A8 __asm__("D_L00_00173E60");
extern Vec4       zero_vector_macro __asm__("D_L00_0015F580") MACRO_ADDR;

#include "rnc/data_labels.h"

extern void  func_L00_002BE2A0_2BE3A8(void) __asm__("FUN_L00_002bcf98");
extern void  func_001F9BD8_2BE3A8(void *, void *, void *) __asm__("FUN_001f9a10");
extern int   func_L00_001EFFF0_2BE3A8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void  func_001F9BF0_2BE3A8(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_001FA888_2BE3A8(int) __asm__("FUN_001fa6c0");
extern void  func_L00_001FF4B0_2BE3A8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  func_001F9C08_2BE3A8(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void  func_001F9C30_2BE3A8(void *, void *, float) __asm__("FUN_001f9a68");
extern void  func_001F9CA0_2BE3A8(void *, void *, void *) __asm__("FUN_001f9ad8");
extern float func_001F9FA8_2BE3A8(float) __asm__("FUN_001f9de0");
extern float func_001F9F90_2BE3A8(float) __asm__("FUN_001f9dc8");
extern void  func_001F9EE8_2BE3A8(void *, void *, void *) __asm__("FUN_001f9d20");
extern void  draw_beam_ring_and_rail_effects(void *) __asm__("FUN_L00_002bdc50");
extern int   func_001F9850_2BE3A8(int) __asm__("FUN_001f96f8");
extern void  func_L00_00272F00_2BE3A8(
     void *, int, int, int, int, void *, float, float, float) __asm__("FUN_L00_00272060");
extern int func_001FA898_2BE3A8(float) __asm__("FUN_001fa6d0");
extern int func_001F9908_2BE3A8(int *) __asm__("FUN_001f9740");

#include "rnc/overlay/shared/weapons/walloper_data/update_beam_ring_rail_and_pulse_effects.h"

extern Tab l00_beam_origin_particle_color_palette __asm__("D_L00_001E9AB0");
void       update_beam_ring_rail_and_pulse_effects(struct Moby *moby) __asm__("FUN_L00_002bd0a0");

void update_beam_ring_rail_and_pulse_effects(struct Moby *moby) {
    Vec_2BE3A8 aim;
    Vec_2BE3A8 segment;
    Vec_2BE3A8 wave_offset;
    Vec_2BE3A8 beam_end;
    Vec_2BE3A8 point;
    Vec_2BE3A8 beam_segment_direction;
    Vec_2BE3A8 side;
    Vars      *data;
    Info      *info;
    int        ring_index;
    int        slot;
    int        pulse;
    int        sign;
    int        node_index;
    int        color;
    float      beam_length;
    float      progress;
    float      factor;
    float      wave_width;
    float      spark_size;

    data = (Vars *)moby->pvars;
    wave_offset.w = 1.0f;
    wave_offset.x = 0.0f;
    func_L00_002BE2A0_2BE3A8();
    D_L00_001617D0 = normalize_angle(D_L00_001617D0 + D_L00_001616F8_gp);
    build_spherical_offset(&aim, D_L00_001617CC_macro, data->f48, data->f4C);
    if (data->target != 0 && (info = suck_get_aim_height(data->target)) != 0) {
        qcopy(&beam_end, &data->target->pos);
        beam_end.z = beam_end.z + info->height;
    } else {
        build_spherical_offset(&beam_end, D_L00_001616DC_gp, data->f48, data->f4C);
        func_001F9BD8_2BE3A8(&beam_end, &beam_end, data);
    }
    beam_length = D_L00_001616DC_gp;
    if (data->target == 0) {
        if (func_L00_001EFFF0_2BE3A8(data, &beam_end, 20, *(void **)((char *)&D_001413D0), 0) !=
            0) {
            beam_length = vector_distance(&D_L00_00173F60_2BE3A8, data);
        }
    } else {
        beam_length = vector_distance(&data->target->pos, data);
    }
    {
        int i;

        D_L00_001617CC_macro = beam_length * D_L00_001616F0_gp;
        qcopy(&beam_nodes[0], data);
        qcopy(&beam_rail_a_points[0], data);
        qcopy(&beam_rail_b_points[0], data);
        for (i = 1; i < 12; i++) {
            float seg_length;

            func_001F9BF0_2BE3A8(&segment, &beam_nodes[i], &beam_nodes[i - 1]);
            progress = func_001FA888_2BE3A8(i) * D_L00_001616F0_gp;
            factor = progress * -0.49999997f + 0.9f;
            if (D_L00_001616F4_gp < i && data->target != 0) {
                func_001F9BF0_2BE3A8(&point, &data->target->pos, &beam_nodes[i]);
                func_L00_001FF4B0_2BE3A8(&point, &point, D_L00_001617CC_macro);
                func_001F9C08_2BE3A8(&aim, &aim, &point, 0.2f);
            }
            func_001F9C08_2BE3A8(&point, &segment, &aim, factor);
            seg_length = fast_vec_length(&point);
            if (seg_length == 0.0f) {
                qcopy(&beam_nodes[i], &beam_nodes[i - 1]);
            } else {
                func_001F9C30_2BE3A8(&point, &point, D_L00_001617CC_macro / seg_length);
                func_001F9BD8_2BE3A8(&beam_nodes[i], &beam_nodes[i - 1], &point);
            }
            func_001F9BF0_2BE3A8(&beam_segment_direction, &beam_nodes[i], &beam_nodes[i - 1]);
            func_001F9CA0_2BE3A8(&side, &beam_segment_direction, D_0013F5E0_r);
            func_L00_001FF4B0_2BE3A8(&side,
                                     &side,
                                     D_L00_00161708_gp +
                                         (D_L00_0016170C_gp - D_L00_00161708_gp) * progress);
            {
                Vec_2BE3A8 ring;
                Vec_2BE3A8 fwd;
                Vec_2BE3A8 out;
                Vec_2BE3A8 up;
                Vec_2BE3A8 node;

                for (ring_index = 0; ring_index < 20; ring_index++) {
                    build_look_at_matrix(&ring,
                                         &side,
                                         &beam_segment_direction,
                                         (float)ring_index * D_L00_00161704_gp);
                    func_001F9BD8_2BE3A8(
                        &D_L00_001DC1A0_2BE3A8[i][ring_index], &ring, &beam_nodes[i]);
                }
                qcopy(&node, &beam_nodes[i]);
                func_L00_001FF4B0_2BE3A8(&fwd, &beam_segment_direction, 1.0f);
                func_L00_001FF4B0_2BE3A8(&out, &side, 1.0f);
                func_001F9CA0_2BE3A8(&up, &out, &fwd);
                wave_width = D_L00_0016172C_gp;
                wave_width = wave_width + (D_L00_00161730_gp - wave_width) * progress;
                factor =
                    normalize_angle(D_L00_001617D0 + D_L00_001616FC_gp * func_001FA888_2BE3A8(i));
                wave_offset.y = wave_width * func_001F9FA8_2BE3A8(factor);
                wave_width = wave_width * func_001F9F90_2BE3A8(factor);
                wave_offset.z = wave_width;
                func_001F9EE8_2BE3A8(&beam_rail_a_points[i], &wave_offset, &fwd);
                wave_offset.z = -wave_offset.z;
                func_001F9EE8_2BE3A8(&beam_rail_b_points[i], &wave_offset, &fwd);
            }
        }
    }
    add_draw_callback(draw_beam_ring_and_rail_effects, moby);
    spark_size = random_float_between(0.1f, 2.0f);
    sign = random_integer_below(2);
    if (sign == 0) {
        sign = sign - 1;
    }
    if (random_integer_below(2) == 0) {
        qcopy(&point, &beam_rail_a_points[11]);
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F7F2020,
                                 0,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.1f,
                                 spark_size,
                                 0.003f);
        sign = -sign;
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F7F7F7F,
                                 1,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.07f,
                                 spark_size * 0.7f,
                                 0.003f);
    }
    if (random_integer_below(2) == 0) {
        qcopy(&point, &beam_rail_b_points[11]);
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F20207F,
                                 0,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.1f,
                                 spark_size,
                                 0.003f);
        sign = -sign;
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F7F7F7F,
                                 1,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.07f,
                                 spark_size * 0.7f,
                                 0.003f);
    }
    func_001F9BF0_2BE3A8(&beam_segment_direction, &beam_nodes[1], &beam_nodes[0]);
    func_L00_001FF4B0_2BE3A8(
        &beam_segment_direction, &beam_segment_direction, D_L00_00161734_gp * frame_scale);
    if (random_integer_below(2) == 0) {
        func_L00_001FF4B0_2BE3A8(&side, &moby->rot_row1, D_L00_00161740_gp);
        func_001F9BD8_2BE3A8(&point, &beam_nodes[0], &side);
        {
            Tab        colors;
            Vec_2BE3A8 out;
            Vec_2BE3A8 up;

            colors = l00_beam_origin_particle_color_palette;
            color = colors.c[random_integer_below(5)];
            wave_width = randf_sym(0.0f, D_L00_0016173C_gp);
            build_look_at_matrix(
                &out, &beam_segment_direction, D_0013F5E0_r, randf_sym(0.0f, D_L00_0016173C_gp));
            func_001F9CA0_2BE3A8(&up, &beam_segment_direction, D_0013F5E0_r);
            build_look_at_matrix(&out, &out, &up, wave_width);
            node_index =
                func_001FA898_2BE3A8(beam_length * 1.2f / (D_L00_00161734_gp * frame_scale));
            if (data->target != 0) {
                node_index = node_index * 3;
            }
            spawn_proximity_corrected_size_alpha_tween_particle(&point,
                                                                node_index,
                                                                color,
                                                                0,
                                                                sign,
                                                                &out,
                                                                data->target,
                                                                spark_size * 0.1f,
                                                                spark_size);
            spawn_proximity_corrected_size_alpha_tween_particle(&point,
                                                                node_index,
                                                                0x7F7F7F7F,
                                                                1,
                                                                -sign,
                                                                &out,
                                                                data->target,
                                                                spark_size * 0.07f,
                                                                spark_size * 0.7f);
        }
    }
    if (func_001F9908_2BE3A8(&D_L00_00161824_macro) != 0) {
        D_L00_00161824_macro = func_001F9850_2BE3A8(beam_pulse_spawn_interval_frames_gp);
        for (slot = 0; slot < 3; slot++) {
            if (D_L00_001618E8_2BE3A8[slot] == 0) {
                D_L00_001618E8_2BE3A8[slot] = 1;
                D_L00_001617D8_macro[slot] = func_001F9850_2BE3A8(D_L00_0016174C_gp);
                D_L00_001617F8_macro[slot] = 0.0f;
                D_L00_001618D8_2BE3A8[slot] = D_L00_0016175C_gp;
                D_L00_001618B8_2BE3A8[slot] = func_001FA888_2BE3A8(D_L00_00161763_macro[0]);
                qcopy(&beam_pulse_positions[slot], data);
                func_001F9BF0_2BE3A8(&side, &beam_nodes[1], &beam_nodes[0]);
                func_L00_001FF4B0_2BE3A8(&beam_pulse_directions[slot], &side, 1.0f);
                break;
            }
        }
    }
    for (pulse = 0; pulse < 3; pulse++) {
        Vec_2BE3A8 step;
        float      dt;
        float      travelled;
        float      max_length;
        float      ratio;
        float      remainder;
        float      inv_length;

        if (D_L00_001618E8_2BE3A8[pulse] == 0) {
            continue;
        }
        if (func_001F9908_2BE3A8(&D_L00_001617D8_macro[pulse]) != 0) {
            D_L00_001618E8_2BE3A8[pulse] = 0;
            continue;
        }
        if (D_L00_001617D8_macro[pulse] < D_L00_00161750_gp) {
            D_L00_001618B8_2BE3A8[pulse] = D_L00_001618B8_2BE3A8[pulse] - D_L00_00161768_gp;
        }
        dt = D_L00_00161748_gp * D_0015ED6C_gp;
        travelled = D_L00_001617F8_macro[pulse] + dt;
        max_length = D_L00_001616DC_gp;
        ratio = travelled / max_length;
        D_L00_001617F8_macro[pulse] = travelled;
        if (1.0f < ratio) {
            ratio = 1.0f;
        }
        D_L00_001618D8_2BE3A8[pulse] =
            D_L00_0016175C_gp + (D_L00_00161758_gp - D_L00_0016175C_gp) * ratio;
        if (max_length <= travelled) {
            func_001F9C30_2BE3A8(&side, &beam_pulse_directions[pulse], dt);
            func_001F9BD8_2BE3A8(&beam_pulse_positions[pulse], &beam_pulse_positions[pulse], &side);
            continue;
        }
        node_index = func_001FA898_2BE3A8(travelled / D_L00_001617CC_macro);
        remainder =
            D_L00_001617F8_macro[pulse] - func_001FA888_2BE3A8(node_index) * D_L00_001617CC_macro;
        qcopy(&beam_pulse_positions[pulse], &beam_nodes[node_index]);
        if (node_index == 11) {
            func_001F9BF0_2BE3A8(&side, &beam_nodes[node_index], &beam_prev_nodes[node_index]);
        } else {
            func_001F9BF0_2BE3A8(&side, &beam_nodes[node_index + 1], &beam_nodes[node_index]);
        }
        inv_length = fast_vec_length(&side);
        if (inv_length == 0.0f) {
            continue;
        }
        inv_length = 1.0f / inv_length;
        func_001F9C30_2BE3A8(&step, &side, remainder * inv_length);
        func_001F9BD8_2BE3A8(&beam_pulse_positions[pulse], &beam_pulse_positions[pulse], &step);
        func_L00_001FF4B0_2BE3A8(&step, &side, inv_length);
        func_001F9C08_2BE3A8(
            &beam_pulse_directions[pulse], &beam_pulse_directions[pulse], &step, 0.1f);
        func_L00_001FF4B0_2BE3A8(
            &beam_pulse_directions[pulse], &beam_pulse_directions[pulse], 1.0f);
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

typedef struct {
    OvlQuad corner[4];
    int     color[4];
    float   uv[8];
    long    unk70, tex, unk80, unk88;
} Quad;

extern u8 D_0013E535 NOT_SDA;

#include "rnc/overlay/shared/weapons/walloper_data/draw_beam_ring_and_rail_effects_4.h"

extern float D_L00_00161718 __attribute__((sda));

#include "rnc/overlay/shared/weapons/walloper_data/draw_beam_ring_and_rail_effects_3.h"

extern float D_L00_00161720 __attribute__((sda));
extern float D_L00_00161724 __attribute__((sda));

#include "rnc/overlay/shared/weapons/walloper_data/draw_beam_ring_and_rail_effects_2.h"

extern int D_L00_00161798 __attribute__((sda));

#include "rnc/overlay/shared/weapons/walloper_data/draw_beam_ring_and_rail_effects.h"

extern OvlQuad D_L00_001DC060[][20] NOT_SDA;
extern OvlQuad D_L00_001DC1A0[][20] NOT_SDA;
void draw_scrolling_ribbon_colored(void *, int, int, float, float, float, int, int) __asm__("FUN_L00_002be078");
void draw_beam_pulse_quads(void *unused) __asm__("FUN_L00_002be650");

void draw_beam_pulse_quads(void *) __asm__("FUN_L00_002be650");
void draw_beam_origin_camera_facing_quads(void *mv) __asm__("FUN_L00_002be890");

void  draw_beam_origin_camera_facing_quads(void *) __asm__("FUN_L00_002be890");
long  FUN_001f44b8(int);
float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");

/* scrolls the two texture-offset accumulators and draws the 10x20 band of
   textured quads between the D_L00_001DC060/001DC1A0 rings, skipping
   back-facing ones */
void draw_beam_ring_and_rail_effects(void *moby) {
    Quad    quad;
    OvlVec4 edge_to_corner0, edge_to_corner2, face_normal, camera_to_corner1;
    float   y, first_pass_v, second_pass_v, texture_scroll_step;
    int     c, i, j, k, off, jo;

    beam_rail_first_pass_texture_u_offset_gp =
        beam_rail_first_pass_texture_u_offset_gp -
        beam_rail_first_pass_texture_u_scroll_speed_gp * frame_scale;
    if (beam_rail_first_pass_texture_u_offset_gp <= -8.0f)
        beam_rail_first_pass_texture_u_offset_gp += 8.0f;
    draw_scrolling_ribbon_colored(beam_rail_a_points,
                                  beam_rail_first_pass_alpha_gp,
                                  beam_rail_second_pass_alpha_gp,
                                  beam_rail_first_pass_half_width_gp,
                                  beam_rail_second_pass_half_width_gp,
                                  beam_rail_second_pass_segment_extension_factor_gp,
                                  12,
                                  beam_first_rail_second_pass_base_vertex_color_gp);
    draw_scrolling_ribbon_colored(beam_rail_b_points,
                                  beam_rail_first_pass_alpha_gp,
                                  beam_rail_second_pass_alpha_gp,
                                  beam_rail_first_pass_half_width_gp,
                                  beam_rail_second_pass_half_width_gp,
                                  beam_rail_second_pass_segment_extension_factor_gp,
                                  12,
                                  beam_second_rail_second_pass_base_vertex_color_gp);
    draw_beam_pulse_quads(moby);
    draw_beam_origin_camera_facing_quads(moby);
    k = D_0013E535;
    c = k ? D_L00_00161798 : beam_ring_default_vertex_color_gp;
    quad.tex = FUN_001f44b8(beam_ring_texture_index_gp);
    quad.unk80 = 0xFF9000000260L;
    quad.unk88 = 0x8000000048L;
    quad.unk70 = 0;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    y = D_L00_00161718;
    D_L00_00161718 = y - beam_ring_texture_u_scroll_speed_gp * frame_scale;
    if (D_L00_00161718 <= -8.0f)
        D_L00_00161718 += 8.0f;
    texture_scroll_step = beam_ring_texture_v_scroll_speed_gp * frame_scale;
    D_L00_00161720 = D_L00_00161720 - texture_scroll_step;
    if (D_L00_00161720 <= -7.0f)
        D_L00_00161720 += 7.0f;
    D_L00_00161724 = D_L00_00161724 + texture_scroll_step;
    if (D_L00_00161724 >= 5.0f)
        D_L00_00161724 -= 5.0f;
    for (i = 2; i < 12; i++) {
        first_pass_v = D_L00_00161720;
        second_pass_v = D_L00_00161724;
        for (j = 0; j < 20; j++) {
            qcopy(&quad.corner[0], &D_L00_001DC1A0[i][j]);
            qcopy(&quad.corner[1], &D_L00_001DC1A0[i][(j + 1) % 20]);
            qcopy(&quad.corner[2], &D_L00_001DC060[i][j]);
            qcopy(&quad.corner[3], &D_L00_001DC060[i][(j + 1) % 20]);
            fast_vec_sub(&edge_to_corner0, &quad.corner[0], &quad.corner[1]);
            fast_vec_sub(&edge_to_corner2, &quad.corner[2], &quad.corner[1]);
            fast_vec_cross(&face_normal, &edge_to_corner2, &edge_to_corner0);
            fast_vec_sub(&camera_to_corner1, &quad.corner[1], &(*(OvlVec4 *)g_debug_camera));
            if (fast_vec_dot(&face_normal, &camera_to_corner1) >= 0.0f) {
                first_pass_v += beam_ring_face_texture_v_step_gp;
                second_pass_v += beam_ring_face_texture_v_step_gp;
            } else {
                if (i == 11) {
                    quad.color[0] = quad.color[1] = beam_ring_default_vertex_color_gp & 0xFFFFFF;
                }
                quad.uv[5] = first_pass_v;
                quad.uv[1] = first_pass_v;
                quad.uv[3] = first_pass_v + beam_ring_face_texture_v_step_gp;
                quad.uv[4] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[0] = y;
                quad.uv[2] = y;
                quad.uv[7] = first_pass_v + beam_ring_face_texture_v_step_gp;
                quad.uv[6] = y - beam_ring_band_texture_u_step_gp;
                first_pass_v = first_pass_v + beam_ring_face_texture_v_step_gp;
                draw_geometry_quad(&quad, 0, 0);
                quad.uv[0] = y;
                quad.uv[1] = second_pass_v;
                quad.uv[2] = y;
                quad.uv[3] = second_pass_v + beam_ring_face_texture_v_step_gp;
                quad.uv[5] = second_pass_v;
                quad.uv[4] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[6] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[7] = second_pass_v + beam_ring_face_texture_v_step_gp;
                second_pass_v = second_pass_v + beam_ring_face_texture_v_step_gp;
                draw_geometry_quad(&quad, 0, 0);
            }
        }
        y += beam_ring_band_texture_u_step_gp;
    }
}

extern int beam_rail_first_pass_base_vertex_color_gp __asm__("D_L00_00161780") __attribute__((sda));
extern int beam_rail_first_pass_texture_index_gp __asm__("D_L00_001617A4") __attribute__((sda));
extern int beam_rail_second_pass_texture_index_gp __asm__("D_L00_001617A8") __attribute__((sda));
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF380). */
void draw_scrolling_ribbon_colored(void *mv,
                                   int   ribbon_first_pass_alpha,
                                   int   ribbon_second_pass_alpha,
                                   float sx,
                                   float sy,
                                   float sz,
                                   int   ribbon_point_count,

                                   int col) {
    RW_2baae0 *ribbon_points = mv;
    RW_2baae0  ribbon_first_pass_corners[4];
    RT_2baae0  ribbon_first_pass_render_attributes;
    RW_2baae0  q2[4];
    RT_2baae0  ribbon_second_pass_render_attributes;
    RW_2baae0  ribbon_camera_facing_side_vector, ribbon_second_pass_width_offset,
        ribbon_first_pass_point_camera_offset, ribbon_segment_delta, ribbon_adjusted_segment_start,
        ribbon_adjusted_segment_end, ribbon_segment_start_camera_offset,
        ribbon_segment_end_camera_offset, s1A0, s1B0;
    int   i, col0, col1;
    float ribbon_first_pass_side_normalization_factor, ribbon_initial_side_normalization_factor,
        ribbon_second_pass_side_normalization_factor, ribbon_segment_start_camera_distance,
        ribbon_segment_end_camera_distance;
    ribbon_first_pass_render_attributes.q0 =
        get_effect_texture(beam_rail_first_pass_texture_index_gp);
    col0 = beam_rail_first_pass_base_vertex_color_gp | (ribbon_first_pass_alpha << 24);
    ribbon_first_pass_render_attributes.q2 = 0x8000000048L;
    ribbon_first_pass_render_attributes.q1 = 0xFF9000000260L;
    ribbon_first_pass_render_attributes.z4 = 0;
    ribbon_first_pass_render_attributes.c[3] = col0;
    ribbon_first_pass_render_attributes.c[2] = col0;
    ribbon_first_pass_render_attributes.c[1] = col0;
    ribbon_first_pass_render_attributes.c[0] = col0;
    ribbon_second_pass_render_attributes.q0 =
        get_effect_texture(beam_rail_second_pass_texture_index_gp);
    col1 = col | (ribbon_second_pass_alpha << 24);
    ribbon_second_pass_render_attributes.q2 = ribbon_first_pass_render_attributes.q2;
    ribbon_second_pass_render_attributes.q1 = ribbon_first_pass_render_attributes.q1;
    ribbon_second_pass_render_attributes.z4 = ribbon_first_pass_render_attributes.z4;
    ribbon_second_pass_render_attributes.c[3] = col1;
    ribbon_second_pass_render_attributes.c[2] = col1;
    ribbon_second_pass_render_attributes.c[1] = col1;
    ribbon_second_pass_render_attributes.c[0] = col1;
    ribbon_second_pass_render_attributes.f[0] = 0;
    ribbon_second_pass_render_attributes.f[1] = 0;
    ribbon_second_pass_render_attributes.f[2] = 0;
    ribbon_second_pass_render_attributes.f[3] = 1.0f;
    ribbon_second_pass_render_attributes.f[4] = 1.0f;
    ribbon_second_pass_render_attributes.f[5] = 0;
    ribbon_second_pass_render_attributes.f[6] = 1.0f;
    ribbon_second_pass_render_attributes.f[7] = 1.0f;
    ribbon_first_pass_render_attributes.f[0] =
        ribbon_second_pass_render_attributes.f[0] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[1] = ribbon_second_pass_render_attributes.f[1];
    ribbon_first_pass_render_attributes.f[2] =
        ribbon_second_pass_render_attributes.f[2] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[3] = ribbon_second_pass_render_attributes.f[3];
    ribbon_first_pass_render_attributes.f[4] =
        ribbon_second_pass_render_attributes.f[4] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[5] = ribbon_second_pass_render_attributes.f[5];
    ribbon_first_pass_render_attributes.f[6] =
        ribbon_second_pass_render_attributes.f[6] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[7] = ribbon_second_pass_render_attributes.f[7];
    fast_vec_sub(ribbon_first_pass_point_camera_offset, ribbon_points, g_debug_camera);
    fast_vec_sub(ribbon_segment_delta, &ribbon_points[1], ribbon_points);
    fast_vec_cross(ribbon_camera_facing_side_vector,
                   ribbon_segment_delta,
                   ribbon_first_pass_point_camera_offset);
    ribbon_initial_side_normalization_factor = fast_vec_length(ribbon_camera_facing_side_vector);
    if (ribbon_initial_side_normalization_factor != 0)
        ribbon_initial_side_normalization_factor = 1.0f / ribbon_initial_side_normalization_factor;
    fast_vec_scale(beam_rail_first_pass_half_width_gp * ribbon_initial_side_normalization_factor,
                   ribbon_camera_facing_side_vector,
                   ribbon_camera_facing_side_vector);
    fast_vec_add(ribbon_first_pass_corners[0], ribbon_points, ribbon_camera_facing_side_vector);
    fast_vec_sub(ribbon_first_pass_corners[1], ribbon_points, ribbon_camera_facing_side_vector);
    for (i = 1; i < ribbon_point_count - 1; i++) {
        fast_vec_sub(ribbon_first_pass_point_camera_offset, &ribbon_points[i], g_debug_camera);
        fast_vec_sub(ribbon_segment_delta, &ribbon_points[i + 1], &ribbon_points[i]);
        fast_vec_cross(ribbon_camera_facing_side_vector,
                       ribbon_segment_delta,
                       ribbon_first_pass_point_camera_offset);
        ribbon_first_pass_side_normalization_factor =
            fast_vec_length(ribbon_camera_facing_side_vector);
        if (ribbon_first_pass_side_normalization_factor != 0)
            ribbon_first_pass_side_normalization_factor =
                1.0f / ribbon_first_pass_side_normalization_factor;
        fast_vec_scale(beam_rail_first_pass_half_width_gp *
                           ribbon_first_pass_side_normalization_factor,
                       ribbon_camera_facing_side_vector,
                       ribbon_camera_facing_side_vector);
        fast_vec_add(
            ribbon_first_pass_corners[2], &ribbon_points[i], ribbon_camera_facing_side_vector);
        fast_vec_sub(
            ribbon_first_pass_corners[3], &ribbon_points[i], ribbon_camera_facing_side_vector);
        if (i == 1) {
            ribbon_first_pass_render_attributes.c[1] = beam_rail_first_pass_base_vertex_color_gp;
            ribbon_first_pass_render_attributes.c[0] = beam_rail_first_pass_base_vertex_color_gp;
        } else if (i == ribbon_point_count - 2) {
            ribbon_first_pass_render_attributes.c[3] = beam_rail_first_pass_base_vertex_color_gp;
            ribbon_first_pass_render_attributes.c[2] = beam_rail_first_pass_base_vertex_color_gp;
        } else if (i == 2) {
            ribbon_first_pass_render_attributes.c[1] = col0;
            ribbon_first_pass_render_attributes.c[0] = col0;
        }
        draw_geometry_quad(ribbon_first_pass_corners, 0, 0);
        qcopy(ribbon_first_pass_corners[0], ribbon_first_pass_corners[2]);
        qcopy(ribbon_first_pass_corners[1], ribbon_first_pass_corners[3]);
    }
    for (i = 0; i < ribbon_point_count - 1; i++) {
        fast_vec_sub(ribbon_segment_start_camera_offset, &ribbon_points[i], g_debug_camera);
        fast_vec_sub(ribbon_segment_end_camera_offset, &ribbon_points[i + 1], g_debug_camera);
        ribbon_segment_start_camera_distance = fast_vec_length(ribbon_segment_start_camera_offset);
        ribbon_segment_end_camera_distance = fast_vec_length(ribbon_segment_end_camera_offset);
        if (ribbon_segment_end_camera_distance < ribbon_segment_start_camera_distance) {
            if (ribbon_segment_end_camera_distance == 0)
                qcopy(ribbon_segment_end_camera_offset, ribbon_segment_start_camera_offset);
            else
                fast_vec_scale(ribbon_segment_start_camera_distance /
                                   ribbon_segment_end_camera_distance,
                               ribbon_segment_end_camera_offset,
                               ribbon_segment_end_camera_offset);
        } else {
            if (ribbon_segment_start_camera_distance == 0)
                qcopy(ribbon_segment_start_camera_offset, ribbon_segment_end_camera_offset);
            else
                fast_vec_scale(ribbon_segment_end_camera_distance /
                                   ribbon_segment_start_camera_distance,
                               ribbon_segment_start_camera_offset,
                               ribbon_segment_start_camera_offset);
        }
        fast_vec_add(
            ribbon_adjusted_segment_start, g_debug_camera, ribbon_segment_start_camera_offset);
        fast_vec_add(ribbon_adjusted_segment_end, g_debug_camera, ribbon_segment_end_camera_offset);
        fast_vec_sub(
            ribbon_segment_delta, ribbon_adjusted_segment_end, ribbon_adjusted_segment_start);
        fast_vec_scale(sz, s1B0, ribbon_segment_delta);
        fast_vec_sub(ribbon_adjusted_segment_start, ribbon_adjusted_segment_start, s1B0);
        fast_vec_add(ribbon_adjusted_segment_end, ribbon_adjusted_segment_end, s1B0);
        fast_vec_sub(s1A0, g_debug_camera, ribbon_adjusted_segment_start);
        fast_vec_cross(ribbon_camera_facing_side_vector, s1A0, ribbon_segment_delta);
        ribbon_second_pass_side_normalization_factor =
            fast_vec_length(ribbon_camera_facing_side_vector);
        if (ribbon_second_pass_side_normalization_factor != 0)
            ribbon_second_pass_side_normalization_factor =
                1.0f / ribbon_second_pass_side_normalization_factor;
        fast_vec_scale(sy * ribbon_second_pass_side_normalization_factor,
                       ribbon_second_pass_width_offset,
                       ribbon_camera_facing_side_vector);
        fast_vec_add(q2[0], ribbon_adjusted_segment_start, ribbon_second_pass_width_offset);
        fast_vec_sub(q2[1], ribbon_adjusted_segment_start, ribbon_second_pass_width_offset);
        fast_vec_add(q2[2], ribbon_adjusted_segment_end, ribbon_second_pass_width_offset);
        fast_vec_sub(q2[3], ribbon_adjusted_segment_end, ribbon_second_pass_width_offset);
        if (i == 0) {
            ribbon_second_pass_render_attributes.c[1] = col;
            ribbon_second_pass_render_attributes.c[0] = col;
        } else if (i == ribbon_point_count - 2) {
            ribbon_second_pass_render_attributes.c[3] = col;
            ribbon_second_pass_render_attributes.c[2] = col;
        } else if (i == 1) {
            ribbon_second_pass_render_attributes.c[1] = col1;
            ribbon_second_pass_render_attributes.c[0] = col1;
        }
        draw_geometry_quad(q2, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF958). */
#include "rnc/rendering/geometry_quad.h"

extern int     beam_pulse_default_rgb_gp __asm__("D_L00_00161760") __attribute__((sda));
extern int     D_L00_00161764 __attribute__((sda));
extern float   D_L00_001617E8_macro[3] __asm__("D_L00_001617E8") MACRO_ADDR;
extern float   D_L00_00161808_macro[3] __asm__("D_L00_00161808") MACRO_ADDR;
extern int     D_L00_00161818_macro[3] __asm__("D_L00_00161818") MACRO_ADDR;
extern OvlVec4 D_L00_001DBF20[] NOT_SDA;

void draw_beam_pulse_quads(void *unused) {
    struct GeometryQuad quad;
    Vec4                basis[3], position, corner;
    int                 color;
    Vec4               *origin;
    int                 i;
    int                 j;

    quad.texture = get_effect_texture(8);
    quad.primitive = 0x8000000048;
    quad.texture_state = 0xFF9000000260;
    quad.reserved.value = 0;
    quad.texture_coordinates[0][0] = 0;
    quad.texture_coordinates[0][1] = 0;
    quad.texture_coordinates[1][0] = 0;
    quad.texture_coordinates[1][1] = 1.0f;
    quad.texture_coordinates[2][0] = 1.0f;
    quad.texture_coordinates[2][1] = 0;
    quad.texture_coordinates[3][0] = 1.0f;
    quad.texture_coordinates[3][1] = 1.0f;
    {
        int selected_color = D_0013E535 != 0 ? D_L00_00161764 : beam_pulse_default_rgb_gp;
        color = selected_color & 0xFFFFFF;
    }
    origin = &position;
    for (i = 0; i < 3; i++) {
        int rgba;
        if (D_L00_00161818_macro[i] != 0) {
            rgba = color | (truncate_float_to_s32(D_L00_001617E8_macro[i]) << 24);
            quad.colors[3] = rgba;
            quad.colors[2] = rgba;
            quad.colors[1] = rgba;
            quad.colors[0] = rgba;
            qcopy(origin, &beam_pulse_positions[i]);
            qcopy(&basis[0], &beam_pulse_directions[i]);
            fast_vec_scale(-1.0f, &basis[2], D_0013F5E0);
            fast_vec_cross(&basis[1], &basis[0], &basis[2]);
            fast_vec_normalize(&basis[1], &basis[1], 1.0f);
            fast_vec_cross(&basis[2], &basis[1], &basis[0]);
            for (j = 0; j < 4; j++) {
                fast_vec_scale(D_L00_00161808_macro[i], &corner, &D_L00_001DBF20[j]);
                transform_vector(&quad.positions[j], &corner, &basis[0]);
            }
            draw_geometry_quad(&quad, 0, 0);
        }
    }
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2be890;

typedef struct {
    V_2be890 corner[4];
    int      color[4];
    int      unk50, unk54, unk58;
    float    unk5C, unk60;
    int      unk64;
    float    unk68, unk6C;
    long     unk70, tex, unk80, unk88;
} Q_2be890;

extern Vec_2BE3A8 beam_nodes[] __asm__("D_L00_001DBF60") NOT_SDA;
void              FUN_001f9a68_2be890(float, void *, void *) __asm__("FUN_001f9a68");
void              FUN_001f9a10_2be890(void *, void *, void *) __asm__("FUN_001f9a10");
void              FUN_001f9a28_2be890(void *, void *, void *) __asm__("FUN_001f9a28");
void              FUN_001f9bf8_2be890(void *, void *, float) __asm__("FUN_001f9bf8");
void              FUN_001f9ad8_2be890(void *, void *, void *) __asm__("FUN_001f9ad8");
void              FUN_001f9d20_2be890(void *, void *, void *) __asm__("FUN_001f9d20");
void              FUN_001f7d30_2be890(void *, void *, int) __asm__("FUN_001f7d30");
extern float beam_origin_quad_basis_offset_length_gp __asm__("D_L00_001617B4") __attribute__((sda));
extern float beam_origin_quad_first_pass_corner_scale_gp __asm__("D_L00_001617AC")
    __attribute__((sda));
extern float beam_origin_quad_second_pass_corner_scale_gp __asm__("D_L00_001617B0")
    __attribute__((sda));
extern int beam_origin_quad_first_pass_vertex_color_gp __asm__("D_L00_001617B8")
    __attribute__((sda));

void draw_beam_origin_camera_facing_quads(void *mv) {
    char    *moby_bytes = mv;
    Q_2be890 quad;
    V_2be890 camera_facing_transform[4];
    V_2be890 scaled_corner;
    V_2be890 offset_vector;
    int      first_pass_vertex_color;
    int      i, j;
    quad.tex = get_effect_texture(11);
    first_pass_vertex_color = beam_origin_quad_first_pass_vertex_color_gp;
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    quad.unk50 = 0;
    quad.unk54 = 0;
    quad.color[3] = first_pass_vertex_color;
    quad.color[2] = first_pass_vertex_color;
    quad.color[1] = first_pass_vertex_color;
    quad.color[0] = first_pass_vertex_color;
    quad.unk58 = 0;
    quad.unk5C = 1.0f;
    quad.unk60 = 1.0f;
    quad.unk64 = 0;
    quad.unk68 = 1.0f;
    quad.unk6C = 1.0f;
    FUN_001f9bf8_2be890(&offset_vector, moby_bytes + 0xD0, beam_origin_quad_basis_offset_length_gp);
    FUN_001f9a10_2be890(&camera_facing_transform[3], &beam_nodes[0], &offset_vector);
    FUN_001f9a28_2be890(
        &offset_vector, &(*(V_2be890 *)g_debug_camera), &camera_facing_transform[3]);
    FUN_001f9bf8_2be890(&camera_facing_transform[0], &offset_vector, 1.0f);
    FUN_001f9ad8_2be890(
        &camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2be890 *)D_0013F5E0));
    FUN_001f9bf8_2be890(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    FUN_001f9ad8_2be890(
        &camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2be890(
            beam_origin_quad_first_pass_corner_scale_gp, &scaled_corner, &D_L00_001DBF20[3 - i]);
        FUN_001f9d20_2be890(&quad.corner[3 - i], &scaled_corner, &camera_facing_transform[0]);
    }
    FUN_001f7d30_2be890(&quad, 0, 0);
    quad.color[3] = -1;
    quad.color[2] = -1;
    quad.color[1] = -1;
    quad.color[0] = -1;
    for (j = 3; j >= 0; j--) {
        FUN_001f9a68_2be890(
            beam_origin_quad_second_pass_corner_scale_gp, &scaled_corner, &D_L00_001DBF20[3 - j]);
        FUN_001f9d20_2be890(&quad.corner[3 - j], &scaled_corner, &camera_facing_transform[0]);
    }
    FUN_001f7d30_2be890(&quad, 0, 0);
}

#include "qcopy.h"

/* Nudge a position and ask the level helper about it, lowering a global on success. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFDB8), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Vx __attribute__((aligned(16)));

extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

int probe_moby_raised_sphere_collision(char *moby,
                                       int   b,
                                       Vx   *position_override) __asm__("FUN_L00_002beab0");

int probe_moby_raised_sphere_collision(char *moby, int b, Vx *position_override) {
    Vx    probe_position;
    float probe_scale;
    float vertical_offset;
    int   r;
    probe_scale = convert_integer_to_float((unsigned char)D_0013E533[1]) * 0.5f + 1.0f;
    if (position_override != 0) {
        qcopy(&probe_position, position_override);
    } else {
        qcopy(&probe_position, moby + 0x10);
    }
    vertical_offset = probe_scale * 0.2f;
    probe_position.z = probe_position.z + vertical_offset;
    r = coll_sphere(probe_scale * 0.1999f, &probe_position, b, moby);
    if (r != 0) {
        float *g = (float *)D_L00_00173E40;
        g[14] -= vertical_offset;
    }
    return r;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFE98), where it is exact; names translated to the US level program. */

/* Saves the moby's position, word 0x4C and float 0x5C into ((char *)&hero) at 0x1ED0. */
void save_state_fields_to_hero(char *source_state) __asm__("FUN_L00_002beb90");

void save_state_fields_to_hero(char *source_state) {
    struct Hero *hero_state = &hero;
    hero_state->unk1ED0 = *(float *)(source_state + 0x40);
    hero_state->unk1ED4 = *(float *)(source_state + 0x44);
    hero_state->unk1ED8 = *(float *)(source_state + 0x48);
    hero_state->unk1F08 = *(int *)(source_state + 0x4C);
    hero_state->unk1EE4 = *(float *)(source_state + 0x5C);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFED0), where it is exact; names translated to the US level program. */

/* Restores the moby's position, word 0x4C and float 0x5C from ((char *)&hero) at 0x1ED0. */
void restore_state_fields_from_hero(char *destination_state) __asm__("FUN_L00_002bebc8");

void restore_state_fields_from_hero(char *destination_state) {
    struct Hero *hero_state = &hero;
    *(float *)(destination_state + 0x40) = hero_state->unk1ED0;
    *(float *)(destination_state + 0x44) = hero_state->unk1ED4;
    *(float *)(destination_state + 0x48) = hero_state->unk1ED8;
    *(int *)(destination_state + 0x4C) = hero_state->unk1F08;
    *(float *)(destination_state + 0x5C) = hero_state->unk1EE4;
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFF08), where it is exact; names translated to the US level program. */

/* Saves moby fields 0x44..0x68 and its vector into ((char *)&hero) at 0x1F10..0x1F40. */
void save_state_vector_and_fields_to_hero(char *source_state) __asm__("FUN_L00_002bec00");

void save_state_vector_and_fields_to_hero(char *source_state) {
    struct Hero *hero_state = &hero;
    hero_state->unk1F20 = *(float *)(source_state + 0x50);
    hero_state->unk1F24 = *(float *)(source_state + 0x54);
    hero_state->unk1F28 = *(float *)(source_state + 0x58);
    hero_state->unk1F3F = *(char *)(source_state + 0x68);
    hero_state->unk1F40 = *(float *)(source_state + 0x44);
    qcopy(&hero_state->unk1F10, source_state);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFF50), where it is exact; names translated to the US level program. */

/* Copies fields from ((char *)&hero) at 0x1F20..0x1F40 into the moby. */
void restore_saved_state_fields_from_hero(char *destination_state) __asm__("FUN_L00_002bec48");

void restore_saved_state_fields_from_hero(char *destination_state) {
    struct Hero *hero_state = &hero;
    *(float *)(destination_state + 0x50) = hero_state->unk1F20;
    *(float *)(destination_state + 0x54) = hero_state->unk1F24;
    *(float *)(destination_state + 0x58) = hero_state->unk1F28;
    *(char *)(destination_state + 0x68) = hero_state->unk1F3F;
    *(float *)(destination_state + 0x44) = hero_state->unk1F40;
}

#include "eetypes.h"
#include "qcopy.h"

typedef union {
    u128  q;
    float f[4];
} Vec4_2bec80;

typedef struct {
    u8    pad[0x18];
    char *o18;
    u8    pad2[0xC];
    float f28;
} S173E40;

extern int D_L00_00173E40[] NOT_SDA;
int   FUN_001efa68_002bec80(Vec4_2bec80 *, Vec4_2bec80 *, int, void *, int) __asm__("FUN_001efa68");
float probe_moby_landing_height(Vec4_2bec80 *probe_position,
                                void        *probing_moby) __asm__("FUN_L00_002bec80");

float probe_moby_landing_height(Vec4_2bec80 *probe_position, void *probing_moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    Vec4_2bec80 lower_probe_point, upper_probe_point;
    float       f, unclamped_lower_z;
    qcopy(&lower_probe_point, probe_position);
    qcopy(&upper_probe_point, probe_position);
    unclamped_lower_z = lower_probe_point.f[2] - 5.0f;
    if (unclamped_lower_z < 0.1f)
        f = 0.1f;
    else
        f = unclamped_lower_z;
    lower_probe_point.f[2] = f;
    upper_probe_point.f[2] = f + 6.0f;
    if (FUN_001efa68_002bec80(&upper_probe_point, &lower_probe_point, 6, probing_moby, 0)) {
        char *hit_moby = (*(S173E40 *)D_L00_00173E40).o18;
        if (hit_moby && ((char *)MOBY(hit_moby)->pclass) &&
            *(short *)(((char *)MOBY(hit_moby)->pclass) + 0x46) == 5) {
            return (*(S173E40 *)D_L00_00173E40).f28;
        }
        switch (collision_hit_material()) {
        case COLLISION_MATERIAL_WATER:
        case 1:
        case 3:
        case 8:
        case 11:
        case 12:
        case 13:
            break;
        default:
            return (*(S173E40 *)D_L00_00173E40).f28;
        }
    }
    return 0.0f;
}

extern char  D_0013F3D0[] NOT_SDA;
extern float FUN_001f99c0(float);
extern int   FUN_001efa68_2bed90(void *, void *, int, void *, int) __asm__("FUN_001efa68");

char *select_class5_target_or_hero(
    char *self, float *position, float *angles, int unused, int mode) __asm__("FUN_L00_002bed90");

char *

select_class5_target_or_hero(char *self, float *position, float *angles, int unused, int mode) {
    char *candidate;
    char *best = 0;
    float target[4] __attribute__((aligned(16)));
    float bestScore = 1000000000.0f;
    float distance, score, angleError, heightError, bonus;
    int   checkCollision = 1;
    int   valid;
    int   i;
    (void)unused;
    for (i = 0; (candidate = active_mobies[i]) != 0; i++) {
        char *classData = *(char **)(candidate + 0x24);
        short classId = classData ? *(short *)(classData + 0x46) : 0;
        if (classId != 5)
            continue;
        distance = fast_vec_dist(position, candidate + 0x10);
        if (distance > 40.0f)
            continue;
        if (distance > 30.0f)
            checkCollision = 0;
        qcopy(target, candidate + 0x10);
        target[2] += 0.4f;
        angleError = fast_difference_between_rotations(
            angles[2], fast_arc_tan(target[0] - position[0], target[1] - position[1]));
        angleError *= angleError;
        heightError = fast_difference_between_rotations(
            angles[1], fast_arc_tan(distance, target[2] - position[2]));
        heightError *= heightError;
        if (checkCollision) {
            if (8.0f < FUN_001f99c0(*(float *)(candidate + 0x18) - *(float *)(self + 0x18)))
                valid = 1;
            else
                valid = FUN_001efa68_2bed90(position, target, 6, candidate, 0) != 0;
        } else {
            valid = *(unsigned char *)(candidate + 0x31) == 0;
        }
        if (mode != 0 || !valid) {
            bonus = 0.0f;
            score = fast_vec_dist(position, target);
            if (*(float *)(candidate + 0x18) - *(float *)(self + 0x18) > 3.25f)
                score += 15.0f;
            if (valid)
                bonus = 5.0f;
            {
                float finalScore = score + bonus + heightError + angleError;
                if (finalScore < bestScore) {
                    bestScore = finalScore;
                    best = candidate;
                }
            }
        }
    }
    if (best == 0) {
        char *playerPosition = D_0013F3D0;
        if (vector_distance(self + 0x10, playerPosition) > 3.2f)
            best = *(char **)(playerPosition + 0x2000);
    }
    return best;
}

#include "sda.h"

typedef int V4 __attribute__((mode(TI), aligned(16)));

typedef union {
    V4  q;
    f32 f[4];
} VU;

typedef struct {
    u8  pad0[0x46];
    s16 h46;
} Cls;

typedef struct {
    u8   pad0[0x24];
    Cls *p24;
} Hit;

typedef struct {
    u8   pad0[0x38];
    Hit *p38;
} Ext;

typedef struct {
    u8   pad0[0x10];
    f32  f10;
    f32  f14;
    u8   pad18[0x30];
    f32  f48;
    u8   pad4C[0x2C];
    Ext *p78;
} Mob;

typedef struct {
    u8   pad0[0x18];
    Hit *unk18;
} XD;

extern VU D_L00_00173E60;

extern s32 FUN_001fa6d0_c(f32) __asm__("FUN_001fa6d0");
extern f32 FUN_001f9e90_c(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern s32 FUN_001efa68_c(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");

f32 select_moby_heading_by_collision_probes(
    Mob *moby,
    VU  *probe_origin,
    f32 *target_position,
    f32 *out,
    f32  ang,
    f32  arc,
    f32  step,
    f32  probe_height_offset,
    f32  horizontal_probe_distance,
    f32  probe_start_horizontal_offset) __asm__("FUN_L00_002bf050");

f32 select_moby_heading_by_collision_probes(Mob *moby,
                                            VU  *probe_origin,
                                            f32 *target_position,
                                            f32 *out,
                                            f32  probe_heading,
                                            f32  arc,
                                            f32  step,
                                            f32  probe_height_offset,
                                            f32  horizontal_probe_distance,

                                            f32 probe_start_horizontal_offset) {
    VU   probe_start;
    VU   probe_end;
    f32  da;
    s32  sweep_probe_count;
    s32  i;
    f32  best;
    f32  bestDist;
    f32  bestDiff;
    f32  preferred_heading;
    f32  a;
    f32  h2;
    f32  hit_distance;
    f32  sweep_heading_error;
    Ext *x;
    Hit *t;

    da = arc / step;
    best = probe_heading;
    sweep_probe_count = FUN_001fa6d0_c(step);
    bestDist = 0.0f;
    x = moby->p78;
    bestDiff = 6.2831855f;
    *out = bestDist;
    if (target_position != 0) {
        preferred_heading =
            FUN_001f9e90_c(target_position[0] - moby->f10, target_position[1] - moby->f14);
    } else {
        preferred_heading = probe_heading;
    }
    arc *= 0.5f;
    fast_add_rotations(probe_heading, arc);
    arc = fast_subtract_rotations(probe_heading, arc);

    qcopy(&probe_start, probe_origin);
    probe_start.f[1] += fast_sin(preferred_heading) * probe_start_horizontal_offset;
    probe_start.f[0] += fast_cos(preferred_heading) * probe_start_horizontal_offset;
    probe_start.f[2] += probe_height_offset;
    qcopy(&probe_end, probe_origin);
    probe_end.f[1] += fast_sin(preferred_heading) * horizontal_probe_distance;
    probe_end.f[0] += fast_cos(preferred_heading) * horizontal_probe_distance;
    probe_end.f[2] += probe_height_offset + probe_height_offset;
    if (FUN_001efa68_c(&probe_start, &probe_end, 4, moby, 0) == 0) {
        if (probe_moby_landing_height(&probe_end, moby) != 0.0f) {
            best = preferred_heading;
            bestDist = horizontal_probe_distance;
            *out = horizontal_probe_distance;
        }
    } else {
        if (coll_hit_moby == x->p38 || (coll_hit_moby != 0 && coll_hit_moby->pclass != 0 &&
                                        coll_hit_moby->pclass->type == 5)) {
            *out = 100.0f;
            return preferred_heading;
        }
        best = preferred_heading;
        bestDist = fast_vec_dist(&probe_start, &D_L00_00173E60);
        bestDiff = 0.0f;
        if (*out < bestDist) {
            *out = bestDist;
        }
    }

    for (i = 0; i < sweep_probe_count; i++) {
        qcopy(&probe_start, probe_origin);
        probe_start.f[1] += fast_sin(arc) * probe_start_horizontal_offset;
        probe_start.f[0] += fast_cos(arc) * probe_start_horizontal_offset;
        probe_start.f[2] += probe_height_offset;
        qcopy(&probe_end, probe_origin);
        probe_end.f[1] += fast_sin(arc) * horizontal_probe_distance;
        probe_end.f[0] += fast_cos(arc) * horizontal_probe_distance;
        probe_end.f[2] += probe_height_offset + probe_height_offset;
        if (FUN_001efa68_c(&probe_start, &probe_end, 4, moby, 0) == 0) {
            if (probe_moby_landing_height(&probe_end, moby) != 0.0f) {
                sweep_heading_error = fast_difference_between_rotations(arc, preferred_heading);
                if (sweep_heading_error < bestDiff || bestDist < horizontal_probe_distance) {
                    bestDiff = sweep_heading_error;
                    best = arc;
                    bestDist = horizontal_probe_distance;
                    if (*out < horizontal_probe_distance) {
                        *out = horizontal_probe_distance;
                    }
                }
            }
        } else {
            if ((*(XD *)D_L00_00173E40).unk18 == x->p38 ||
                ((*(XD *)D_L00_00173E40).unk18 != 0 && (*(XD *)D_L00_00173E40).unk18->p24 != 0 &&
                 (*(XD *)D_L00_00173E40).unk18->p24->h46 == 5)) {
                *out = 100.0f;
                return arc;
            }
            hit_distance = fast_vec_dist(&probe_start, &D_L00_00173E60);
            if (bestDist < hit_distance) {
                best = arc;
                bestDist = hit_distance;
                bestDiff = fast_difference_between_rotations(best, preferred_heading);
                if (*out < hit_distance) {
                    *out = hit_distance;
                }
            } else if (hit_distance == bestDist) {
                sweep_heading_error = fast_difference_between_rotations(arc, preferred_heading);
                if (sweep_heading_error < bestDiff) {
                    bestDiff = sweep_heading_error;
                    best = arc;
                    bestDist = hit_distance;
                    if (*out < hit_distance) {
                        *out = hit_distance;
                    }
                }
            }
        }
        arc = fast_add_rotations(arc, da);
    }

    probe_heading = fast_subtract_rotations(best, 1.5707964f);
    qcopy(&probe_start, probe_origin);
    probe_start.f[1] += fast_sin(probe_heading) * probe_start_horizontal_offset;
    probe_start.f[0] += fast_cos(probe_heading) * probe_start_horizontal_offset;
    probe_start.f[2] += probe_height_offset * 0.125f;
    qcopy(&probe_end, &probe_start);
    probe_end.f[1] += fast_sin(best) * horizontal_probe_distance;
    probe_end.f[0] += fast_cos(best) * horizontal_probe_distance;
    probe_end.f[2] += 0.12f;
    if (FUN_001efa68_c(&probe_start, &probe_end, 4, moby, 0) != 0) {
        if (coll_hit_moby != x->p38 && !(coll_hit_moby != 0 && coll_hit_moby->pclass != 0 &&
                                         coll_hit_moby->pclass->type == 5)) {
            hit_distance = fast_vec_dist(&probe_start, &D_L00_00173E60);
            best = fast_add_rotations(best,
                                      (horizontal_probe_distance - hit_distance) * 3.1415927f /
                                          (horizontal_probe_distance * 3.0f));
        }
    }
    probe_heading = fast_add_rotations(probe_heading, 3.1415927f);
    qcopy(&probe_start, probe_origin);
    probe_start.f[1] += fast_sin(probe_heading) * probe_start_horizontal_offset;
    probe_start.f[0] += fast_cos(probe_heading) * probe_start_horizontal_offset;
    probe_start.f[2] += probe_height_offset * 0.125f;
    qcopy(&probe_end, &probe_start);
    probe_end.f[1] += fast_sin(best) * horizontal_probe_distance;
    probe_end.f[0] += fast_cos(best) * horizontal_probe_distance;
    probe_end.f[2] += 0.12f;
    if (FUN_001efa68_c(&probe_start, &probe_end, 4, moby, 0) != 0) {
        if (coll_hit_moby != x->p38 && !(coll_hit_moby != 0 && coll_hit_moby->pclass != 0 &&
                                         coll_hit_moby->pclass->type == 5)) {
            hit_distance = fast_vec_dist(&probe_start, &D_L00_00173E60);
            best = fast_subtract_rotations(best,
                                           (horizontal_probe_distance - hit_distance) * 3.1415927f /
                                               (horizontal_probe_distance * 3.0f));
        }
    }
    if (2.5132742f < fast_difference_between_rotations(best, moby->f48)) {
        best = fast_add_rotations(moby->f48, fast_subtract_rotations(best, moby->f48) * 0.5f);
    }
    return best;
}
/* Steps a vector by n repeated additions of the normalised difference. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002C0B18), where it is exact; names translated to the US level program. */

extern void FUN_001f9a68(void *, void *, float);

void accumulate_local_vector_steps(char *start_vector,
                                   char *end_vector,
                                   int   step_count) __asm__("FUN_L00_002bf810");

void accumulate_local_vector_steps(char *start_vector, char *end_vector, int step_count) {
    float step_vector[4];
    float accumulated_vector[4];
    int   i;
    fast_vec_sub(step_vector, end_vector, start_vector);
    FUN_001f9a68(step_vector, step_vector, 1.0f / convert_integer_to_float(step_count));
    qcopy(accumulated_vector, start_vector);
    for (i = 0; i < step_count; i++) {
        fast_vec_add(accumulated_vector, accumulated_vector, step_vector);
    }
}

extern float FUN_L00_0025abf0(void *, void *, int, float, float);

void initialize_moby_hop_trajectory(unsigned char *m,
                                    unsigned char *o,
                                    u128          *src) __asm__("FUN_L00_002bf8b0");

void initialize_moby_hop_trajectory(unsigned char *m, unsigned char *o, u128 *src) {
    float s, t, r, lim;
#define F2BF(p, x) (*(float *)((p) + (x)))
    *(int *)(o + 0x30) = 0;
    qcopy(o, src);
    s = vector_distance(m + 0x10, o);
    s = s * s;
    t = F2BF(o, 0x1C) * 35.0f * F2BF(o, 0x20) / (FUN_001f99c0(F2BF(o, 8) - F2BF(m, 0x18)) + 1.0f) *
        (s + 0.25f);
    F2BF(o, 0x14) = t;
    if (F2BF(o, 0x20) < t)
        F2BF(o, 0x14) = F2BF(o, 0x20);
    F2BF(o, 0x18) = fast_arc_tan(F2BF(o, 0) - F2BF(m, 0x10), F2BF(o, 4) - F2BF(m, 0x14));
    r = FUN_L00_0025abf0(m + 0x10, o, 0, F2BF(o, 0x14), -F2BF(o, 0x1C));
    lim = frame_time * 7.0f;
    F2BF(o, 0x10) = r;
    if (lim < r)
        F2BF(o, 0x10) = lim;
    o[0x2F] = 0;
    F2BF(o, 0x14) = F2BF(o, 0x14) * (r / F2BF(o, 0x10));
}
#undef F2BF
#include "qzero.h"

/* Tries a hop of the moby toward q: probes the ground at the landing point and the path to it, and sets the jump
 * state on success. When blocked and the hop is long enough it tries again with half the distance. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002C0CF8), where it is exact; names translated to the US level program. */

typedef struct Cls_2C0CF8 {
    char  pad0[0x24];
    float unk24; /* 0x24 */
} Cls_2C0CF8;

typedef struct Mob_2C0CF8 {
    char          pad0[0x10];
    float         pos[4]; /* 0x10 */
    char          pad20[4];
    Cls_2C0CF8   *p24; /* 0x24 */
    char          pad28[4];
    float         f2C; /* 0x2C */
    char          pad30[0x8C];
    unsigned char bBC; /* 0xBC */
} Mob_2C0CF8;

typedef struct Blk {
    char  pad0[0x38];
    void *p38; /* 0x38 */
    char  pad3C[0x14];
    float f50; /* 0x50 */
} Blk;

typedef struct {
    char pad[0x46];
    s16  field46;
} SubStruct;

typedef struct {
    char       pad[0x24];
    SubStruct *sub;
} TopStruct;

extern int   FUN_001efa68_c2(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern int   FUN_L00_001ff600();
extern void  scale_vector_xyz_c2(void *, void *, float) __asm__("FUN_001f9a68");
float        calculate_triangular_corrected_scalar(float *a,
                                                   float *b,
                                                   int   *out,
                                                   float  f) __asm__("FUN_L00_00259468");
void         FUN_L00_002bf810_c(char *a, char *b, int n) __asm__("FUN_L00_002bf810");
extern void  func_001F9BF0_2C0CF8(void *, void *, void *) __asm__("FUN_001f9a28");
extern int   func_L00_001FF5B0_2C0CF8(float, void *, void *) __asm__("FUN_L00_001ff600");
extern void  func_001F9BD8_2C0CF8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  func_001F9C30_2C0CF8(void *, void *, float) __asm__("FUN_001f9a68");
extern float func_001F9CE8_2C0CF8(void *) __asm__("FUN_001f9b20");
extern void  func_L00_001FF4B0_2C0CF8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  func_L00_002C0B18_2C0CF8(
     void *a, void *b, int n, int c, int d, int e, float g, int f) __asm__("FUN_L00_002bf810");
extern int  func_L00_001EFFF0_2C0CF8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern char D_0013E633_2C0CF8[] __asm__("D_00141260");

int try_start_moby_hop(Mob_2C0CF8 *moby,
                       Blk        *block,
                       void       *target,
                       int         check_line) __asm__("FUN_L00_002bf9f0");

int try_start_moby_hop(Mob_2C0CF8 *moby, Blk *block, void *target, int check_line) {
    float jump[4];
    float land[4];
    float from[4];
    float to[4];
    float half[4];
    float mid[4];
    float up[4];
    int   can_halve;
    float half_length;
    float sum;

    can_halve = 1;
    func_001F9BF0_2C0CF8(jump, target, moby->pos);
    func_L00_001FF5B0_2C0CF8(moby->f2C * 5.2f / moby->p24->unk24, jump, jump);
    func_001F9BD8_2C0CF8(land, moby->pos, jump);
    func_001F9C30_2C0CF8(half, jump, 0.5f);
    half_length = func_001F9CE8_2C0CF8(half);
    func_001F9BD8_2C0CF8(half, moby->pos, half);
    if (!(0.5f < half_length)) {
        can_halve = 0;
    }
    land[2] = probe_moby_landing_height(land, moby);
    jump[2] = land[2] - moby->pos[2];
    if (land[2] == 0.0f) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    if (moby->f2C * 3.0f / moby->p24->unk24 < jump[2]) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    qcopy(mid, &coll_hit_normal);
    func_L00_001FF4B0_2C0CF8(mid, mid, 1.0f);
    sum = absolute_float(mid[0]);
    sum = sum + absolute_float(mid[1]);
    if (absolute_float(mid[2]) < sum) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    land[2] = land[2] + moby->f2C * 0.025f / moby->p24->unk24;
    if (probe_moby_raised_sphere_collision(moby, 6, land)) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    land[2] = land[2] - moby->f2C * 0.025f / moby->p24->unk24;
    qcopy_nc(from, moby->pos);
    from[2] = from[2] + moby->f2C * 0.2f / moby->p24->unk24;
    qcopy_nc(to, land);
    to[2] = to[2] + moby->f2C * 0.2f / moby->p24->unk24;
    if (check_line) {
        if (func_L00_001EFFF0_2C0CF8(from, to, 6, moby, 0) == 0) {
            if (can_halve) {
                return try_start_moby_hop(moby, block, half, 1);
            }
            return 0;
        }
    }
    save_state_vector_and_fields_to_hero(block);
    initialize_moby_hop_trajectory(moby, D_0013E633_2C0CF8, land);
    restore_saved_state_fields_from_hero(block);
    func_001F9C30_2C0CF8(mid, jump, 0.5f);
    func_001F9BD8_2C0CF8(mid, mid, moby->pos);
    qzero(up);
    up[2] = block->f50;
    mid[2] = calculate_triangular_corrected_scalar(
        moby->pos, up, 0, *(float *)(D_0013E633_2C0CF8 + 0x1C));
    func_L00_002C0B18_2C0CF8(from, mid, 10, 0, 32, 32, 40000.0f, 60);
    func_L00_002C0B18_2C0CF8(to, mid, 10, 0, 32, 32, 40000.0f, 60);
    if (func_L00_001EFFF0_2C0CF8(from, mid, 6, moby, 0) != 0 && coll_hit_moby != block->p38) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    if (func_L00_001EFFF0_2C0CF8(mid, to, 6, moby, 0) != 0 && coll_hit_moby != block->p38) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    moby->bBC = 0xD;
    return 1;
}

#include "eetypes.h"
#include "qcopy.h"

unsigned char *
spawn_doom_bot(int a, u128 *spawn_position, u128 *initial_vector) __asm__("FUN_L00_002bfe08");

unsigned char *spawn_doom_bot(int linked_moby, u128 *spawn_position, u128 *initial_vector) {
    unsigned char *spawned_moby = create_moby(0xba);
    if (spawned_moby) {
        char *moby_vars;
        spawned_moby[0x30] = 0xff;
        MOBY(spawned_moby)->draw_dist = 0xff;
        spawned_moby[0x31] = 1;
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x20] = 0;
        spawned_moby[0xbc] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(moby_vars + 0x10, initial_vector);
        *(int *)(moby_vars + 0x60) = linked_moby;
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
        MOBY(spawned_moby)->scale = *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24) * 0.1f;
    }
    return spawned_moby;
}

#include "rnc/math/vector.h"

int FUN_001efa68(Vec4 *, Vec4 *, int, void *, int);
int compute_moby_state_from_vertical_probe(unsigned char *moby) __asm__("FUN_L00_002bfec8");

int compute_moby_state_from_vertical_probe(unsigned char *moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    Vec4 lower_probe_point, upper_probe_point;
    int  resulting_moby_state = moby[0x20];
    qcopy(&lower_probe_point, moby + 0x10);
    qcopy(&upper_probe_point, moby + 0x10);
    lower_probe_point.f[2] -= 0.3f;
    upper_probe_point.f[2] += 1.0f;
    if (FUN_001efa68(&upper_probe_point, &lower_probe_point, 6, moby, 0)) {
        char *hit_moby = (char *)coll_hit_moby;
        if (hit_moby && ((char *)MOBY(hit_moby)->pclass) &&
            *(short *)(((char *)MOBY(hit_moby)->pclass) + 0x46) == 5) {
            resulting_moby_state = 11;
        } else {
            switch (collision_hit_material()) {
            case COLLISION_MATERIAL_WATER:
            case 1:
            case 3:
            case 8:
            case 11:
            case 12:
            case 13:
                resulting_moby_state = 11;
                break;
            }
        }
    }
    return resulting_moby_state;
}

float D_L01_001615C8 = 0.7853982f;
float D_L01_001615CC = 0.55850536f;
float D_L01_001615D0 = 0.017453292f;
float D_L01_001615D4 = 0.03f;
float D_L01_001615D8 = 0.03f;
float D_L01_001615E0 = 0.34906584f;
float D_L01_001615E8 = 0.80285144f;
float D_L01_001615EC = 0.99483764f;
float D_L01_001615F0 = 0.8f;
float D_L01_001615F4 = 0.5f;
float D_L01_001615FC = 0.05236f;
float D_L01_00161600 = 0.20943952f;
float D_L01_00161614 = 0;
float D_L01_00161618 = 0.1f;
int   D_L01_00161628 = 0x7f2020;
float D_L01_00161658 = 0.09f;
float D_L01_0016165C = 1.5f;
s16   D_L01_00161690 = 0;
Vec4  D_L01_001DBE90[4][5] = {0};

Vec4 D_L01_001DBEA0[4][5] = {0};

f32          D_L01_0016161C = 0;
f32          D_L01_00161620 = 0.3f;
s32          D_L01_00161604 = 0x80;
s32          D_L01_0016160C = 0x40;
f32          D_L01_0016162C = 0.05f;
f32          D_L01_00161630 = 0.4f;
f32          D_L01_00161660 = 0.9f;
s32          D_L01_00161634 = 0x10;
s32          D_L01_00161638 = 0x20;
f32          D_L01_0016163C = 0.1f;
f32          D_L01_00161640 = 0.3f;
f32          D_L01_00161664 = 0.1f;
unsigned int D_L01_00161670 = 0x307f7f7f;
float        D_L01_00161678 = 0.1f;
float        D_L01_0016167C = 0.35f;

V_2bb0b0 D_L01_001DB9E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L01_001DBA20 NOT_SDA = {0};

int   D_L01_001616FC = 0;
float D_L01_001616D4 = 0.2f;
char  D_L01_001616D8[4] = {5, 0, 0, 0};

char D_L01_001616DC[4] = {0x14, 0, 0, 0};

short D_L01_001DC370 NOT_SDA = 0;
short D_L01_001DC388 NOT_SDA = 0;
f32                  D_L01_001616C0 = 0.05f;
f32                  D_L01_001616C4 = 0.5f;
char                 D_L01_001616B0[4] = {-1, -1, -1, 0};

char D_L01_001616B4[4] = {0x20, 0x20, 0x7f, 0};

char D_L01_001616E0[4] = {0xe, 0, 0, 0};

char D_L01_001616E4[4] = {0x10, 0, 0, 0};

float        D_L01_001616EC = -0.5f;
float        D_L01_001616E8 = 0.75f;
int          D_L01_001616F8 = 8;
unsigned int D_L01_001616F0 = 0x307f4040;
unsigned int D_L01_001616F4 = 0x7f7f4040;

V_2bc1a0 D_L01_001DBFE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L01_00161710 = 3.0f;
int                D_L01_0016171C = 0;
int                D_L01_001617EC = 0;
int                D_L01_001617F8 = 0;
f32                D_L01_001617FC = 0;
s32                D_L01_00161748 = 0;
s32                D_L01_00161750 = 0;
s32                D_L01_00161754 = 0;
float              D_L01_00161800 = 0;
s32                D_L01_00161854 = 0;
s32                D_L01_00161848 = 0;
s32 D_L01_001DC3D0 NOT_SDA = 0;
s32 D_L01_001DD550 NOT_SDA = 0;
signed char        D_L01_00161793 = 0x40;
int                D_L01_00161818 = 0;
int                D_L01_00161838 = 0;
int D_L01_001DC3F0 NOT_SDA = 0;
int D_L01_001DD520 NOT_SDA = 0;
float              D_L01_00161720 = 0.08333f;
int                D_L01_00161724 = 6;
float              D_L01_00161728 = 0.20071286f;
float              D_L01_0016172C = 0.39968038f;
float              D_L01_00161734 = 0.3141592f;
float              D_L01_00161738 = 0.05f;
float              D_L01_0016173C = 1.234f;
float              D_L01_0016175C = 0.01f;
float              D_L01_00161760 = 0.5f;
float              D_L01_00161764 = 0.2f;
float              D_L01_0016176C = 0.2617994f;
float              D_L01_00161770 = -0.25f;
float              D_L01_00161778 = 8.0f;
int                D_L01_0016177C = 0x3c;
int                D_L01_00161780 = 0x1e;
float              D_L01_00161788 = 1.5f;
float              D_L01_0016178C = 0.1f;
float              D_L01_00161798 = 2.13333f;
int                D_L01_00161808[3] = {0};

float D_L01_00161828[3] = {0};

Vec_2BE3A8 D_L01_001DC4A0[12] = {0};

Vec_2BE3A8 D_L01_001DC560[12] = {0};

Tab D_L01_0020AB10 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L01_001617C8 = 0x60007f00;
int   D_L01_001617B0 = 0x7f7f7f;
int   D_L01_001617D4 = 0xe;
int   D_L01_001617D8 = 0x10;
int   D_L01_00161790 = 0x407f207f;
int   D_L01_00161794 = 0x60007f00;
float D_L01_001617E4 = -0.5f;
float D_L01_001617DC = 0.4f;
float D_L01_001617E0 = 0.1f;
int   D_L01_001617E8 = 0x7f207f7f;
float D_L02_00161568 = 0.7853982f;
float D_L02_0016156C = 0.55850536f;
float D_L02_00161570 = 0.017453292f;
float D_L02_00161574 = 0.03f;
float D_L02_00161578 = 0.03f;
float D_L02_00161580 = 0.34906584f;
float D_L02_00161588 = 0.80285144f;
float D_L02_0016158C = 0.99483764f;
float D_L02_00161590 = 0.8f;
float D_L02_00161594 = 0.5f;
float D_L02_0016159C = 0.05236f;
float D_L02_001615A0 = 0.20943952f;
float D_L02_001615B4 = 0;
float D_L02_001615B8 = 0.1f;
int   D_L02_001615C8 = 0x7f2020;
float D_L02_001615F8 = 0.09f;
float D_L02_001615FC = 1.5f;
s16   D_L02_00161630 = 0;
Vec4  D_L02_001D0490[4][5] = {0};

Vec4 D_L02_001D04A0[4][5] = {0};

f32          D_L02_001615BC = 0;
f32          D_L02_001615C0 = 0.3f;
s32          D_L02_001615A4 = 0x80;
s32          D_L02_001615AC = 0x40;
f32          D_L02_001615CC = 0.05f;
f32          D_L02_001615D0 = 0.4f;
f32          D_L02_00161600 = 0.9f;
s32          D_L02_001615D4 = 0x10;
s32          D_L02_001615D8 = 0x20;
f32          D_L02_001615DC = 0.1f;
f32          D_L02_001615E0 = 0.3f;
f32          D_L02_00161604 = 0.1f;
unsigned int D_L02_00161610 = 0x307f7f7f;
float        D_L02_00161618 = 0.1f;
float        D_L02_0016161C = 0.35f;

V_2bb0b0 D_L02_001CFFE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L02_001D0020 NOT_SDA = {0};

int   D_L02_0016169C = 0;
float D_L02_00161674 = 0.2f;
char  D_L02_00161678[4] = {5, 0, 0, 0};

char D_L02_0016167C[4] = {0x14, 0, 0, 0};

short D_L02_001D0970 NOT_SDA = 0;
short D_L02_001D0988 NOT_SDA = 0;
f32                  D_L02_00161660 = 0.05f;
f32                  D_L02_00161664 = 0.5f;
char                 D_L02_00161650[4] = {-1, -1, -1, 0};

char D_L02_00161654[4] = {0x20, 0x20, 0x7f, 0};

char D_L02_00161680[4] = {0xe, 0, 0, 0};

char D_L02_00161684[4] = {0x10, 0, 0, 0};

float        D_L02_0016168C = -0.5f;
float        D_L02_00161688 = 0.75f;
int          D_L02_00161698 = 8;
unsigned int D_L02_00161690 = 0x307f4040;
unsigned int D_L02_00161694 = 0x7f7f4040;

V_2bc1a0 D_L02_001D05E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L02_001616B0 = 3.0f;
int                D_L02_001616BC = 0;
int                D_L02_0016178C = 0;
int                D_L02_00161798 = 0;
f32                D_L02_0016179C = 0;
s32                D_L02_001616E8 = 0;
s32                D_L02_001616F0 = 0;
s32                D_L02_001616F4 = 0;
float              D_L02_001617A0 = 0;
s32                D_L02_001617F4 = 0;
s32                D_L02_001617E8 = 0;
s32 D_L02_001D09D0 NOT_SDA = 0;
s32 D_L02_001D1B50 NOT_SDA = 0;
signed char        D_L02_00161733 = 0x40;
int                D_L02_001617B8 = 0;
int                D_L02_001617D8 = 0;
int D_L02_001D09F0 NOT_SDA = 0;
int D_L02_001D1B20 NOT_SDA = 0;
float              D_L02_001616C0 = 0.08333f;
int                D_L02_001616C4 = 6;
float              D_L02_001616C8 = 0.20071286f;
float              D_L02_001616CC = 0.39968038f;
float              D_L02_001616D4 = 0.3141592f;
float              D_L02_001616D8 = 0.05f;
float              D_L02_001616DC = 1.234f;
float              D_L02_001616FC = 0.01f;
float              D_L02_00161700 = 0.5f;
float              D_L02_00161704 = 0.2f;
float              D_L02_0016170C = 0.2617994f;
float              D_L02_00161710 = -0.25f;
float              D_L02_00161718 = 8.0f;
int                D_L02_0016171C = 0x3c;
int                D_L02_00161720 = 0x1e;
float              D_L02_00161728 = 1.5f;
float              D_L02_0016172C = 0.1f;
float              D_L02_00161738 = 2.13333f;
int                D_L02_001617A8[3] = {0};

float D_L02_001617C8[3] = {0};

Vec_2BE3A8 D_L02_001D0AA0[12] = {0};

Vec_2BE3A8 D_L02_001D0B60[12] = {0};

Tab D_L02_001FA3B0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L02_00161768 = 0x60007f00;
int   D_L02_00161750 = 0x7f7f7f;
int   D_L02_00161774 = 0xe;
int   D_L02_00161778 = 0x10;
int   D_L02_00161730 = 0x407f207f;
int   D_L02_00161734 = 0x60007f00;
float D_L02_00161784 = -0.5f;
float D_L02_0016177C = 0.4f;
float D_L02_00161780 = 0.1f;
int   D_L02_00161788 = 0x7f207f7f;
float D_L03_00161578 = 0.7853982f;
float D_L03_0016157C = 0.55850536f;
float D_L03_00161580 = 0.017453292f;
float D_L03_00161584 = 0.03f;
float D_L03_00161588 = 0.03f;
float D_L03_00161590 = 0.34906584f;
float D_L03_00161598 = 0.80285144f;
float D_L03_0016159C = 0.99483764f;
float D_L03_001615A0 = 0.8f;
float D_L03_001615A4 = 0.5f;
float D_L03_001615AC = 0.05236f;
float D_L03_001615B0 = 0.20943952f;
float D_L03_001615C4 = 0;
float D_L03_001615C8 = 0.1f;
int   D_L03_001615D8 = 0x7f2020;
float D_L03_00161608 = 0.09f;
float D_L03_0016160C = 1.5f;
s16   D_L03_00161640 = 0;
Vec4  D_L03_001CFF90[4][5] = {0};

Vec4 D_L03_001CFFA0[4][5] = {0};

f32          D_L03_001615CC = 0;
f32          D_L03_001615D0 = 0.3f;
s32          D_L03_001615B4 = 0x80;
s32          D_L03_001615BC = 0x40;
f32          D_L03_001615DC = 0.05f;
f32          D_L03_001615E0 = 0.4f;
f32          D_L03_00161610 = 0.9f;
s32          D_L03_001615E4 = 0x10;
s32          D_L03_001615E8 = 0x20;
f32          D_L03_001615EC = 0.1f;
f32          D_L03_001615F0 = 0.3f;
f32          D_L03_00161614 = 0.1f;
unsigned int D_L03_00161620 = 0x307f7f7f;
float        D_L03_00161628 = 0.1f;
float        D_L03_0016162C = 0.35f;

V_2bb0b0 D_L03_001CFAE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L03_001CFB20 NOT_SDA = {0};

int   D_L03_001616AC = 0;
float D_L03_00161684 = 0.2f;
char  D_L03_00161688[4] = {5, 0, 0, 0};

char D_L03_0016168C[4] = {0x14, 0, 0, 0};

short D_L03_001D0470 NOT_SDA = 0;
short D_L03_001D0488 NOT_SDA = 0;
f32                  D_L03_00161670 = 0.05f;
f32                  D_L03_00161674 = 0.5f;
char                 D_L03_00161660[4] = {-1, -1, -1, 0};

char D_L03_00161664[4] = {0x20, 0x20, 0x7f, 0};

char D_L03_00161690[4] = {0xe, 0, 0, 0};

char D_L03_00161694[4] = {0x10, 0, 0, 0};

float        D_L03_0016169C = -0.5f;
float        D_L03_00161698 = 0.75f;
int          D_L03_001616A8 = 8;
unsigned int D_L03_001616A0 = 0x307f4040;
unsigned int D_L03_001616A4 = 0x7f7f4040;

V_2bc1a0 D_L03_001D00E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L03_001616C0 = 3.0f;
int                D_L03_001616CC = 0;
int                D_L03_0016179C = 0;
int                D_L03_001617A8 = 0;
f32                D_L03_001617AC = 0;
s32                D_L03_001616F8 = 0;
s32                D_L03_00161700 = 0;
s32                D_L03_00161704 = 0;
float              D_L03_001617B0 = 0;
s32                D_L03_00161804 = 0;
s32                D_L03_001617F8 = 0;
s32 D_L03_001D04D0 NOT_SDA = 0;
s32 D_L03_001D1650 NOT_SDA = 0;
signed char        D_L03_00161743 = 0x40;
int                D_L03_001617C8 = 0;
int                D_L03_001617E8 = 0;
int D_L03_001D04F0 NOT_SDA = 0;
int D_L03_001D1620 NOT_SDA = 0;
float              D_L03_001616D0 = 0.08333f;
int                D_L03_001616D4 = 6;
float              D_L03_001616D8 = 0.20071286f;
float              D_L03_001616DC = 0.39968038f;
float              D_L03_001616E4 = 0.3141592f;
float              D_L03_001616E8 = 0.05f;
float              D_L03_001616EC = 1.234f;
float              D_L03_0016170C = 0.01f;
float              D_L03_00161710 = 0.5f;
float              D_L03_00161714 = 0.2f;
float              D_L03_0016171C = 0.2617994f;
float              D_L03_00161720 = -0.25f;
float              D_L03_00161728 = 8.0f;
int                D_L03_0016172C = 0x3c;
int                D_L03_00161730 = 0x1e;
float              D_L03_00161738 = 1.5f;
float              D_L03_0016173C = 0.1f;
float              D_L03_00161748 = 2.13333f;
int                D_L03_001617B8[3] = {0};

float D_L03_001617D8[3] = {0};

Vec_2BE3A8 D_L03_001D05A0[12] = {0};

Vec_2BE3A8 D_L03_001D0660[12] = {0};

Tab D_L03_001E2C40 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L03_00161778 = 0x60007f00;
int   D_L03_00161760 = 0x7f7f7f;
int   D_L03_00161784 = 0xe;
int   D_L03_00161788 = 0x10;
int   D_L03_00161740 = 0x407f207f;
int   D_L03_00161744 = 0x60007f00;
float D_L03_00161794 = -0.5f;
float D_L03_0016178C = 0.4f;
float D_L03_00161790 = 0.1f;
int   D_L03_00161798 = 0x7f207f7f;
float D_L04_00161568 = 0.7853982f;
float D_L04_0016156C = 0.55850536f;
float D_L04_00161570 = 0.017453292f;
float D_L04_00161574 = 0.03f;
float D_L04_00161578 = 0.03f;
float D_L04_00161580 = 0.34906584f;
float D_L04_00161588 = 0.80285144f;
float D_L04_0016158C = 0.99483764f;
float D_L04_00161590 = 0.8f;
float D_L04_00161594 = 0.5f;
float D_L04_0016159C = 0.05236f;
float D_L04_001615A0 = 0.20943952f;
float D_L04_001615B4 = 0;
float D_L04_001615B8 = 0.1f;
int   D_L04_001615C8 = 0x7f2020;
float D_L04_001615F8 = 0.09f;
float D_L04_001615FC = 1.5f;
s16   D_L04_00161630 = 0;
Vec4  D_L04_001D0010[4][5] = {0};

Vec4 D_L04_001D0020[4][5] = {0};

f32          D_L04_001615BC = 0;
f32          D_L04_001615C0 = 0.3f;
s32          D_L04_001615A4 = 0x80;
s32          D_L04_001615AC = 0x40;
f32          D_L04_001615CC = 0.05f;
f32          D_L04_001615D0 = 0.4f;
f32          D_L04_00161600 = 0.9f;
s32          D_L04_001615D4 = 0x10;
s32          D_L04_001615D8 = 0x20;
f32          D_L04_001615DC = 0.1f;
f32          D_L04_001615E0 = 0.3f;
f32          D_L04_00161604 = 0.1f;
unsigned int D_L04_00161610 = 0x307f7f7f;
float        D_L04_00161618 = 0.1f;
float        D_L04_0016161C = 0.35f;

V_2bb0b0 D_L04_001CFB60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L04_001CFBA0 NOT_SDA = {0};

int   D_L04_0016169C = 0;
float D_L04_00161674 = 0.2f;
char  D_L04_00161678[4] = {5, 0, 0, 0};

char D_L04_0016167C[4] = {0x14, 0, 0, 0};

short D_L04_001D04F0 NOT_SDA = 0;
short D_L04_001D0508 NOT_SDA = 0;
f32                  D_L04_00161660 = 0.05f;
f32                  D_L04_00161664 = 0.5f;
char                 D_L04_00161650[4] = {-1, -1, -1, 0};

char D_L04_00161654[4] = {0x20, 0x20, 0x7f, 0};

char D_L04_00161680[4] = {0xe, 0, 0, 0};

char D_L04_00161684[4] = {0x10, 0, 0, 0};

float        D_L04_0016168C = -0.5f;
float        D_L04_00161688 = 0.75f;
int          D_L04_00161698 = 8;
unsigned int D_L04_00161690 = 0x307f4040;
unsigned int D_L04_00161694 = 0x7f7f4040;

V_2bc1a0 D_L04_001D0160 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L04_001616B0 = 3.0f;
int                D_L04_001616BC = 0;
int                D_L04_0016178C = 0;
int                D_L04_00161798 = 0;
f32                D_L04_0016179C = 0;
s32                D_L04_001616E8 = 0;
s32                D_L04_001616F0 = 0;
s32                D_L04_001616F4 = 0;
float              D_L04_001617A0 = 0;
s32                D_L04_001617F4 = 0;
s32                D_L04_001617E8 = 0;
s32 D_L04_001D0550 NOT_SDA = 0;
s32 D_L04_001D16D0 NOT_SDA = 0;
signed char        D_L04_00161733 = 0x40;
int                D_L04_001617B8 = 0;
int                D_L04_001617D8 = 0;
int D_L04_001D0570 NOT_SDA = 0;
int D_L04_001D16A0 NOT_SDA = 0;
float              D_L04_001616C0 = 0.08333f;
int                D_L04_001616C4 = 6;
float              D_L04_001616C8 = 0.20071286f;
float              D_L04_001616CC = 0.39968038f;
float              D_L04_001616D4 = 0.3141592f;
float              D_L04_001616D8 = 0.05f;
float              D_L04_001616DC = 1.234f;
float              D_L04_001616FC = 0.01f;
float              D_L04_00161700 = 0.5f;
float              D_L04_00161704 = 0.2f;
float              D_L04_0016170C = 0.2617994f;
float              D_L04_00161710 = -0.25f;
float              D_L04_00161718 = 8.0f;
int                D_L04_0016171C = 0x3c;
int                D_L04_00161720 = 0x1e;
float              D_L04_00161728 = 1.5f;
float              D_L04_0016172C = 0.1f;
float              D_L04_00161738 = 2.13333f;
int                D_L04_001617A8[3] = {0};

float D_L04_001617C8[3] = {0};

Vec_2BE3A8 D_L04_001D0620[12] = {0};

Vec_2BE3A8 D_L04_001D06E0[12] = {0};

Tab D_L04_001DB850 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L04_00161768 = 0x60007f00;
int   D_L04_00161750 = 0x7f7f7f;
int   D_L04_00161774 = 0xe;
int   D_L04_00161778 = 0x10;
int   D_L04_00161730 = 0x407f207f;
int   D_L04_00161734 = 0x60007f00;
float D_L04_00161784 = -0.5f;
float D_L04_0016177C = 0.4f;
float D_L04_00161780 = 0.1f;
int   D_L04_00161788 = 0x7f207f7f;
float D_L05_001615B8 = 0.7853982f;
float D_L05_001615BC = 0.55850536f;
float D_L05_001615C0 = 0.017453292f;
float D_L05_001615C4 = 0.03f;
float D_L05_001615C8 = 0.03f;
float D_L05_001615D0 = 0.34906584f;
float D_L05_001615D8 = 0.80285144f;
float D_L05_001615DC = 0.99483764f;
float D_L05_001615E0 = 0.8f;
float D_L05_001615E4 = 0.5f;
float D_L05_001615EC = 0.05236f;
float D_L05_001615F0 = 0.20943952f;
float D_L05_00161604 = 0;
float D_L05_00161608 = 0.1f;
int   D_L05_00161618 = 0x7f2020;
float D_L05_00161648 = 0.09f;
float D_L05_0016164C = 1.5f;
s16   D_L05_00161680 = 0;
Vec4  D_L05_001D0330[4][5] = {0};

Vec4 D_L05_001D0340[4][5] = {0};

f32          D_L05_0016160C = 0;
f32          D_L05_00161610 = 0.3f;
s32          D_L05_001615F4 = 0x80;
s32          D_L05_001615FC = 0x40;
f32          D_L05_0016161C = 0.05f;
f32          D_L05_00161620 = 0.4f;
f32          D_L05_00161650 = 0.9f;
s32          D_L05_00161624 = 0x10;
s32          D_L05_00161628 = 0x20;
f32          D_L05_0016162C = 0.1f;
f32          D_L05_00161630 = 0.3f;
f32          D_L05_00161654 = 0.1f;
unsigned int D_L05_00161660 = 0x307f7f7f;
float        D_L05_00161668 = 0.1f;
float        D_L05_0016166C = 0.35f;

V_2bb0b0 D_L05_001CFE80 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L05_001CFEC0 NOT_SDA = {0};

int   D_L05_001616EC = 0;
float D_L05_001616C4 = 0.2f;
char  D_L05_001616C8[4] = {5, 0, 0, 0};

char D_L05_001616CC[4] = {0x14, 0, 0, 0};

short D_L05_001D0810 NOT_SDA = 0;
short D_L05_001D0828 NOT_SDA = 0;
f32                  D_L05_001616B0 = 0.05f;
f32                  D_L05_001616B4 = 0.5f;
char                 D_L05_001616A0[4] = {-1, -1, -1, 0};

char D_L05_001616A4[4] = {0x20, 0x20, 0x7f, 0};

char D_L05_001616D0[4] = {0xe, 0, 0, 0};

char D_L05_001616D4[4] = {0x10, 0, 0, 0};

float        D_L05_001616DC = -0.5f;
float        D_L05_001616D8 = 0.75f;
int          D_L05_001616E8 = 8;
unsigned int D_L05_001616E0 = 0x307f4040;
unsigned int D_L05_001616E4 = 0x7f7f4040;

V_2bc1a0 D_L05_001D0480 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L05_00161700 = 3.0f;
int                D_L05_0016170C = 0;
int                D_L05_001617DC = 0;
int                D_L05_001617E8 = 0;
f32                D_L05_001617EC = 0;
s32                D_L05_00161738 = 0;
s32                D_L05_00161740 = 0;
s32                D_L05_00161744 = 0;
float              D_L05_001617F0 = 0;
s32                D_L05_00161844 = 0;
s32                D_L05_00161838 = 0;
s32 D_L05_001D0870 NOT_SDA = 0;
s32 D_L05_001D19F0 NOT_SDA = 0;
signed char        D_L05_00161783 = 0x40;
int                D_L05_00161808 = 0;
int                D_L05_00161828 = 0;
int D_L05_001D0890 NOT_SDA = 0;
int D_L05_001D19C0 NOT_SDA = 0;
float              D_L05_00161710 = 0.08333f;
int                D_L05_00161714 = 6;
float              D_L05_00161718 = 0.20071286f;
float              D_L05_0016171C = 0.39968038f;
float              D_L05_00161724 = 0.3141592f;
float              D_L05_00161728 = 0.05f;
float              D_L05_0016172C = 1.234f;
float              D_L05_0016174C = 0.01f;
float              D_L05_00161750 = 0.5f;
float              D_L05_00161754 = 0.2f;
float              D_L05_0016175C = 0.2617994f;
float              D_L05_00161760 = -0.25f;
float              D_L05_00161768 = 8.0f;
int                D_L05_0016176C = 0x3c;
int                D_L05_00161770 = 0x1e;
float              D_L05_00161778 = 1.5f;
float              D_L05_0016177C = 0.1f;
float              D_L05_00161788 = 2.13333f;
int                D_L05_001617F8[3] = {0};

float D_L05_00161818[3] = {0};

Vec_2BE3A8 D_L05_001D0940[12] = {0};

Vec_2BE3A8 D_L05_001D0A00[12] = {0};

Tab D_L05_002181C0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L05_001617B8 = 0x60007f00;
int   D_L05_001617A0 = 0x7f7f7f;
int   D_L05_001617C4 = 0xe;
int   D_L05_001617C8 = 0x10;
int   D_L05_00161780 = 0x407f207f;
int   D_L05_00161784 = 0x60007f00;
float D_L05_001617D4 = -0.5f;
float D_L05_001617CC = 0.4f;
float D_L05_001617D0 = 0.1f;
int   D_L05_001617D8 = 0x7f207f7f;
float D_L06_00161598 = 0.7853982f;
float D_L06_0016159C = 0.55850536f;
float D_L06_001615A0 = 0.017453292f;
float D_L06_001615A4 = 0.03f;
float D_L06_001615A8 = 0.03f;
float D_L06_001615B0 = 0.34906584f;
float D_L06_001615B8 = 0.80285144f;
float D_L06_001615BC = 0.99483764f;
float D_L06_001615C0 = 0.8f;
float D_L06_001615C4 = 0.5f;
float D_L06_001615CC = 0.05236f;
float D_L06_001615D0 = 0.20943952f;
float D_L06_001615E4 = 0;
float D_L06_001615E8 = 0.1f;
int   D_L06_001615F8 = 0x7f2020;
float D_L06_00161628 = 0.09f;
float D_L06_0016162C = 1.5f;
s16   D_L06_00161660 = 0;
Vec4  D_L06_001D0610[4][5] = {0};

Vec4 D_L06_001D0620[4][5] = {0};

f32          D_L06_001615EC = 0;
f32          D_L06_001615F0 = 0.3f;
s32          D_L06_001615D4 = 0x80;
s32          D_L06_001615DC = 0x40;
f32          D_L06_001615FC = 0.05f;
f32          D_L06_00161600 = 0.4f;
f32          D_L06_00161630 = 0.9f;
s32          D_L06_00161604 = 0x10;
s32          D_L06_00161608 = 0x20;
f32          D_L06_0016160C = 0.1f;
f32          D_L06_00161610 = 0.3f;
f32          D_L06_00161634 = 0.1f;
unsigned int D_L06_00161640 = 0x307f7f7f;
float        D_L06_00161648 = 0.1f;
float        D_L06_0016164C = 0.35f;

V_2bb0b0 D_L06_001D0160 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L06_001D01A0 NOT_SDA = {0};

int   D_L06_001616CC = 0;
float D_L06_001616A4 = 0.2f;
char  D_L06_001616A8[4] = {5, 0, 0, 0};

char D_L06_001616AC[4] = {0x14, 0, 0, 0};

short D_L06_001D0AF0 NOT_SDA = 0;
short D_L06_001D0B08 NOT_SDA = 0;
f32                  D_L06_00161690 = 0.05f;
f32                  D_L06_00161694 = 0.5f;
char                 D_L06_00161680[4] = {-1, -1, -1, 0};

char D_L06_00161684[4] = {0x20, 0x20, 0x7f, 0};

char D_L06_001616B0[4] = {0xe, 0, 0, 0};

char D_L06_001616B4[4] = {0x10, 0, 0, 0};

float        D_L06_001616BC = -0.5f;
float        D_L06_001616B8 = 0.75f;
int          D_L06_001616C8 = 8;
unsigned int D_L06_001616C0 = 0x307f4040;
unsigned int D_L06_001616C4 = 0x7f7f4040;

V_2bc1a0 D_L06_001D0760 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L06_001616E0 = 3.0f;
int                D_L06_001616EC = 0;
int                D_L06_001617BC = 0;
int                D_L06_001617C8 = 0;
f32                D_L06_001617CC = 0;
s32                D_L06_00161718 = 0;
s32                D_L06_00161720 = 0;
s32                D_L06_00161724 = 0;
float              D_L06_001617D0 = 0;
s32                D_L06_00161824 = 0;
s32                D_L06_00161818 = 0;
s32 D_L06_001D0B50 NOT_SDA = 0;
s32 D_L06_001D1CD0 NOT_SDA = 0;
signed char        D_L06_00161763 = 0x40;
int                D_L06_001617E8 = 0;
int                D_L06_00161808 = 0;
int D_L06_001D0B70 NOT_SDA = 0;
int D_L06_001D1CA0 NOT_SDA = 0;
float              D_L06_001616F0 = 0.08333f;
int                D_L06_001616F4 = 6;
float              D_L06_001616F8 = 0.20071286f;
float              D_L06_001616FC = 0.39968038f;
float              D_L06_00161704 = 0.3141592f;
float              D_L06_00161708 = 0.05f;
float              D_L06_0016170C = 1.234f;
float              D_L06_0016172C = 0.01f;
float              D_L06_00161730 = 0.5f;
float              D_L06_00161734 = 0.2f;
float              D_L06_0016173C = 0.2617994f;
float              D_L06_00161740 = -0.25f;
float              D_L06_00161748 = 8.0f;
int                D_L06_0016174C = 0x3c;
int                D_L06_00161750 = 0x1e;
float              D_L06_00161758 = 1.5f;
float              D_L06_0016175C = 0.1f;
float              D_L06_00161768 = 2.13333f;
int                D_L06_001617D8[3] = {0};

float D_L06_001617F8[3] = {0};

Vec_2BE3A8 D_L06_001D0C20[12] = {0};

Vec_2BE3A8 D_L06_001D0CE0[12] = {0};

Tab D_L06_00200EB0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L06_00161798 = 0x60007f00;
int   D_L06_00161780 = 0x7f7f7f;
int   D_L06_001617A4 = 0xe;
int   D_L06_001617A8 = 0x10;
int   D_L06_00161760 = 0x407f207f;
int   D_L06_00161764 = 0x60007f00;
float D_L06_001617B4 = -0.5f;
float D_L06_001617AC = 0.4f;
float D_L06_001617B0 = 0.1f;
int   D_L06_001617B8 = 0x7f207f7f;
float D_L07_00161568 = 0.7853982f;
float D_L07_0016156C = 0.55850536f;
float D_L07_00161570 = 0.017453292f;
float D_L07_00161574 = 0.03f;
float D_L07_00161578 = 0.03f;
float D_L07_00161580 = 0.34906584f;
float D_L07_00161588 = 0.80285144f;
float D_L07_0016158C = 0.99483764f;
float D_L07_00161590 = 0.8f;
float D_L07_00161594 = 0.5f;
float D_L07_0016159C = 0.05236f;
float D_L07_001615A0 = 0.20943952f;
float D_L07_001615B4 = 0;
float D_L07_001615B8 = 0.1f;
int   D_L07_001615C8 = 0x7f2020;
float D_L07_001615F8 = 0.09f;
float D_L07_001615FC = 1.5f;
s16   D_L07_00161630 = 0;
Vec4  D_L07_001CFF10[4][5] = {0};

Vec4 D_L07_001CFF20[4][5] = {0};

f32          D_L07_001615BC = 0;
f32          D_L07_001615C0 = 0.3f;
s32          D_L07_001615A4 = 0x80;
s32          D_L07_001615AC = 0x40;
f32          D_L07_001615CC = 0.05f;
f32          D_L07_001615D0 = 0.4f;
f32          D_L07_00161600 = 0.9f;
s32          D_L07_001615D4 = 0x10;
s32          D_L07_001615D8 = 0x20;
f32          D_L07_001615DC = 0.1f;
f32          D_L07_001615E0 = 0.3f;
f32          D_L07_00161604 = 0.1f;
unsigned int D_L07_00161610 = 0x307f7f7f;
float        D_L07_00161618 = 0.1f;
float        D_L07_0016161C = 0.35f;

V_2bb0b0 D_L07_001CFA60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L07_001CFAA0 NOT_SDA = {0};

int   D_L07_0016169C = 0;
float D_L07_00161674 = 0.2f;
char  D_L07_00161678[4] = {5, 0, 0, 0};

char D_L07_0016167C[4] = {0x14, 0, 0, 0};

short D_L07_001D03F0 NOT_SDA = 0;
short D_L07_001D0408 NOT_SDA = 0;
f32                  D_L07_00161660 = 0.05f;
f32                  D_L07_00161664 = 0.5f;
char                 D_L07_00161650[4] = {-1, -1, -1, 0};

char D_L07_00161654[4] = {0x20, 0x20, 0x7f, 0};

char D_L07_00161680[4] = {0xe, 0, 0, 0};

char D_L07_00161684[4] = {0x10, 0, 0, 0};

float        D_L07_0016168C = -0.5f;
float        D_L07_00161688 = 0.75f;
int          D_L07_00161698 = 8;
unsigned int D_L07_00161690 = 0x307f4040;
unsigned int D_L07_00161694 = 0x7f7f4040;

V_2bc1a0 D_L07_001D0060 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L07_001616B0 = 3.0f;
int                D_L07_001616BC = 0;
int                D_L07_0016178C = 0;
int                D_L07_00161798 = 0;
f32                D_L07_0016179C = 0;
s32                D_L07_001616E8 = 0;
s32                D_L07_001616F0 = 0;
s32                D_L07_001616F4 = 0;
float              D_L07_001617A0 = 0;
s32                D_L07_001617F4 = 0;
s32                D_L07_001617E8 = 0;
s32 D_L07_001D0450 NOT_SDA = 0;
s32 D_L07_001D15D0 NOT_SDA = 0;
signed char        D_L07_00161733 = 0x40;
int                D_L07_001617B8 = 0;
int                D_L07_001617D8 = 0;
int D_L07_001D0470 NOT_SDA = 0;
int D_L07_001D15A0 NOT_SDA = 0;
float              D_L07_001616C0 = 0.08333f;
int                D_L07_001616C4 = 6;
float              D_L07_001616C8 = 0.20071286f;
float              D_L07_001616CC = 0.39968038f;
float              D_L07_001616D4 = 0.3141592f;
float              D_L07_001616D8 = 0.05f;
float              D_L07_001616DC = 1.234f;
float              D_L07_001616FC = 0.01f;
float              D_L07_00161700 = 0.5f;
float              D_L07_00161704 = 0.2f;
float              D_L07_0016170C = 0.2617994f;
float              D_L07_00161710 = -0.25f;
float              D_L07_00161718 = 8.0f;
int                D_L07_0016171C = 0x3c;
int                D_L07_00161720 = 0x1e;
float              D_L07_00161728 = 1.5f;
float              D_L07_0016172C = 0.1f;
float              D_L07_00161738 = 2.13333f;
int                D_L07_001617A8[3] = {0};

float D_L07_001617C8[3] = {0};

Vec_2BE3A8 D_L07_001D0520[12] = {0};

Vec_2BE3A8 D_L07_001D05E0[12] = {0};

Tab D_L07_00210D50 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L07_00161768 = 0x60007f00;
int   D_L07_00161750 = 0x7f7f7f;
int   D_L07_00161774 = 0xe;
int   D_L07_00161778 = 0x10;
int   D_L07_00161730 = 0x407f207f;
int   D_L07_00161734 = 0x60007f00;
float D_L07_00161784 = -0.5f;
float D_L07_0016177C = 0.4f;
float D_L07_00161780 = 0.1f;
int   D_L07_00161788 = 0x7f207f7f;
float D_L08_001615B8 = 0.7853982f;
float D_L08_001615BC = 0.55850536f;
float D_L08_001615C0 = 0.017453292f;
float D_L08_001615C4 = 0.03f;
float D_L08_001615C8 = 0.03f;
float D_L08_001615D0 = 0.34906584f;
float D_L08_001615D8 = 0.80285144f;
float D_L08_001615DC = 0.99483764f;
float D_L08_001615E0 = 0.8f;
float D_L08_001615E4 = 0.5f;
float D_L08_001615EC = 0.05236f;
float D_L08_001615F0 = 0.20943952f;
float D_L08_00161604 = 0;
float D_L08_00161608 = 0.1f;
int   D_L08_00161618 = 0x7f2020;
float D_L08_00161648 = 0.09f;
float D_L08_0016164C = 1.5f;
s16   D_L08_00161680 = 0;
Vec4  D_L08_001D0690[4][5] = {0};

Vec4 D_L08_001D06A0[4][5] = {0};

f32          D_L08_0016160C = 0;
f32          D_L08_00161610 = 0.3f;
s32          D_L08_001615F4 = 0x80;
s32          D_L08_001615FC = 0x40;
f32          D_L08_0016161C = 0.05f;
f32          D_L08_00161620 = 0.4f;
f32          D_L08_00161650 = 0.9f;
s32          D_L08_00161624 = 0x10;
s32          D_L08_00161628 = 0x20;
f32          D_L08_0016162C = 0.1f;
f32          D_L08_00161630 = 0.3f;
f32          D_L08_00161654 = 0.1f;
unsigned int D_L08_00161660 = 0x307f7f7f;
float        D_L08_00161668 = 0.1f;
float        D_L08_0016166C = 0.35f;

V_2bb0b0 D_L08_001D01E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L08_001D0220 NOT_SDA = {0};

int   D_L08_001616EC = 0;
float D_L08_001616C4 = 0.2f;
char  D_L08_001616C8[4] = {5, 0, 0, 0};

char D_L08_001616CC[4] = {0x14, 0, 0, 0};

short D_L08_001D0B70 NOT_SDA = 0;
short D_L08_001D0B88 NOT_SDA = 0;
f32                  D_L08_001616B0 = 0.05f;
f32                  D_L08_001616B4 = 0.5f;
char                 D_L08_001616A0[4] = {-1, -1, -1, 0};

char D_L08_001616A4[4] = {0x20, 0x20, 0x7f, 0};

char D_L08_001616D0[4] = {0xe, 0, 0, 0};

char D_L08_001616D4[4] = {0x10, 0, 0, 0};

float        D_L08_001616DC = -0.5f;
float        D_L08_001616D8 = 0.75f;
int          D_L08_001616E8 = 8;
unsigned int D_L08_001616E0 = 0x307f4040;
unsigned int D_L08_001616E4 = 0x7f7f4040;

V_2bc1a0 D_L08_001D07E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L08_00161700 = 3.0f;
int                D_L08_0016170C = 0;
int                D_L08_001617DC = 0;
int                D_L08_001617E8 = 0;
f32                D_L08_001617EC = 0;
s32                D_L08_00161738 = 0;
s32                D_L08_00161740 = 0;
s32                D_L08_00161744 = 0;
float              D_L08_001617F0 = 0;
s32                D_L08_00161844 = 0;
s32                D_L08_00161838 = 0;
s32 D_L08_001D0BD0 NOT_SDA = 0;
s32 D_L08_001D1D50 NOT_SDA = 0;
signed char        D_L08_00161783 = 0x40;
int                D_L08_00161808 = 0;
int                D_L08_00161828 = 0;
int D_L08_001D0BF0 NOT_SDA = 0;
int D_L08_001D1D20 NOT_SDA = 0;
float              D_L08_00161710 = 0.08333f;
int                D_L08_00161714 = 6;
float              D_L08_00161718 = 0.20071286f;
float              D_L08_0016171C = 0.39968038f;
float              D_L08_00161724 = 0.3141592f;
float              D_L08_00161728 = 0.05f;
float              D_L08_0016172C = 1.234f;
float              D_L08_0016174C = 0.01f;
float              D_L08_00161750 = 0.5f;
float              D_L08_00161754 = 0.2f;
float              D_L08_0016175C = 0.2617994f;
float              D_L08_00161760 = -0.25f;
float              D_L08_00161768 = 8.0f;
int                D_L08_0016176C = 0x3c;
int                D_L08_00161770 = 0x1e;
float              D_L08_00161778 = 1.5f;
float              D_L08_0016177C = 0.1f;
float              D_L08_00161788 = 2.13333f;
int                D_L08_001617F8[3] = {0};

float D_L08_00161818[3] = {0};

Vec_2BE3A8 D_L08_001D0CA0[12] = {0};

Vec_2BE3A8 D_L08_001D0D60[12] = {0};

Tab D_L08_001FBA10 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L08_001617B8 = 0x60007f00;
int   D_L08_001617A0 = 0x7f7f7f;
int   D_L08_001617C4 = 0xe;
int   D_L08_001617C8 = 0x10;
int   D_L08_00161780 = 0x407f207f;
int   D_L08_00161784 = 0x60007f00;
float D_L08_001617D4 = -0.5f;
float D_L08_001617CC = 0.4f;
float D_L08_001617D0 = 0.1f;
int   D_L08_001617D8 = 0x7f207f7f;
float D_L09_00161628 = 0.7853982f;
float D_L09_0016162C = 0.55850536f;
float D_L09_00161630 = 0.017453292f;
float D_L09_00161634 = 0.03f;
float D_L09_00161638 = 0.03f;
float D_L09_00161640 = 0.34906584f;
float D_L09_00161648 = 0.80285144f;
float D_L09_0016164C = 0.99483764f;
float D_L09_00161650 = 0.8f;
float D_L09_00161654 = 0.5f;
float D_L09_0016165C = 0.05236f;
float D_L09_00161660 = 0.20943952f;
float D_L09_00161674 = 0;
float D_L09_00161678 = 0.1f;
int   D_L09_00161688 = 0x7f2020;
float D_L09_001616B8 = 0.09f;
float D_L09_001616BC = 1.5f;
s16   D_L09_001616F0 = 0;
Vec4  D_L09_001D0010[4][5] = {0};

Vec4 D_L09_001D0020[4][5] = {0};

f32          D_L09_0016167C = 0;
f32          D_L09_00161680 = 0.3f;
s32          D_L09_00161664 = 0x80;
s32          D_L09_0016166C = 0x40;
f32          D_L09_0016168C = 0.05f;
f32          D_L09_00161690 = 0.4f;
f32          D_L09_001616C0 = 0.9f;
s32          D_L09_00161694 = 0x10;
s32          D_L09_00161698 = 0x20;
f32          D_L09_0016169C = 0.1f;
f32          D_L09_001616A0 = 0.3f;
f32          D_L09_001616C4 = 0.1f;
unsigned int D_L09_001616D0 = 0x307f7f7f;
float        D_L09_001616D8 = 0.1f;
float        D_L09_001616DC = 0.35f;

V_2bb0b0 D_L09_001CFB60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L09_001CFBA0 NOT_SDA = {0};

int   D_L09_0016175C = 0;
float D_L09_00161734 = 0.2f;
char  D_L09_00161738[4] = {5, 0, 0, 0};

char D_L09_0016173C[4] = {0x14, 0, 0, 0};

short D_L09_001D04F0 NOT_SDA = 0;
short D_L09_001D0508 NOT_SDA = 0;
f32                  D_L09_00161720 = 0.05f;
f32                  D_L09_00161724 = 0.5f;
char                 D_L09_00161710[4] = {-1, -1, -1, 0};

char D_L09_00161714[4] = {0x20, 0x20, 0x7f, 0};

char D_L09_00161740[4] = {0xe, 0, 0, 0};

char D_L09_00161744[4] = {0x10, 0, 0, 0};

float        D_L09_0016174C = -0.5f;
float        D_L09_00161748 = 0.75f;
int          D_L09_00161758 = 8;
unsigned int D_L09_00161750 = 0x307f4040;
unsigned int D_L09_00161754 = 0x7f7f4040;

V_2bc1a0 D_L09_001D0160 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L09_00161770 = 3.0f;
int                D_L09_0016177C = 0;
int                D_L09_0016184C = 0;
int                D_L09_00161858 = 0;
f32                D_L09_0016185C = 0;
s32                D_L09_001617A8 = 0;
s32                D_L09_001617B0 = 0;
s32                D_L09_001617B4 = 0;
float              D_L09_00161860 = 0;
s32                D_L09_001618B4 = 0;
s32                D_L09_001618A8 = 0;
s32 D_L09_001D0550 NOT_SDA = 0;
s32 D_L09_001D16D0 NOT_SDA = 0;
signed char        D_L09_001617F3 = 0x40;
int                D_L09_00161878 = 0;
int                D_L09_00161898 = 0;
int D_L09_001D0570 NOT_SDA = 0;
int D_L09_001D16A0 NOT_SDA = 0;
float              D_L09_00161780 = 0.08333f;
int                D_L09_00161784 = 6;
float              D_L09_00161788 = 0.20071286f;
float              D_L09_0016178C = 0.39968038f;
float              D_L09_00161794 = 0.3141592f;
float              D_L09_00161798 = 0.05f;
float              D_L09_0016179C = 1.234f;
float              D_L09_001617BC = 0.01f;
float              D_L09_001617C0 = 0.5f;
float              D_L09_001617C4 = 0.2f;
float              D_L09_001617CC = 0.2617994f;
float              D_L09_001617D0 = -0.25f;
float              D_L09_001617D8 = 8.0f;
int                D_L09_001617DC = 0x3c;
int                D_L09_001617E0 = 0x1e;
float              D_L09_001617E8 = 1.5f;
float              D_L09_001617EC = 0.1f;
float              D_L09_001617F8 = 2.13333f;
int                D_L09_00161868[3] = {0};

float D_L09_00161888[3] = {0};

Vec_2BE3A8 D_L09_001D0620[12] = {0};

Vec_2BE3A8 D_L09_001D06E0[12] = {0};

Tab D_L09_002086A0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L09_00161828 = 0x60007f00;
int   D_L09_00161810 = 0x7f7f7f;
int   D_L09_00161834 = 0xe;
int   D_L09_00161838 = 0x10;
int   D_L09_001617F0 = 0x407f207f;
int   D_L09_001617F4 = 0x60007f00;
float D_L09_00161844 = -0.5f;
float D_L09_0016183C = 0.4f;
float D_L09_00161840 = 0.1f;
int   D_L09_00161848 = 0x7f207f7f;
float D_L10_00161578 = 0.7853982f;
float D_L10_0016157C = 0.55850536f;
float D_L10_00161580 = 0.017453292f;
float D_L10_00161584 = 0.03f;
float D_L10_00161588 = 0.03f;
float D_L10_00161590 = 0.34906584f;
float D_L10_00161598 = 0.80285144f;
float D_L10_0016159C = 0.99483764f;
float D_L10_001615A0 = 0.8f;
float D_L10_001615A4 = 0.5f;
float D_L10_001615AC = 0.05236f;
float D_L10_001615B0 = 0.20943952f;
float D_L10_001615C4 = 0;
float D_L10_001615C8 = 0.1f;
int   D_L10_001615D8 = 0x7f2020;
float D_L10_00161608 = 0.09f;
float D_L10_0016160C = 1.5f;
s16   D_L10_00161640 = 0;
Vec4  D_L10_001D0310[4][5] = {0};

Vec4 D_L10_001D0320[4][5] = {0};

f32          D_L10_001615CC = 0;
f32          D_L10_001615D0 = 0.3f;
s32          D_L10_001615B4 = 0x80;
s32          D_L10_001615BC = 0x40;
f32          D_L10_001615DC = 0.05f;
f32          D_L10_001615E0 = 0.4f;
f32          D_L10_00161610 = 0.9f;
s32          D_L10_001615E4 = 0x10;
s32          D_L10_001615E8 = 0x20;
f32          D_L10_001615EC = 0.1f;
f32          D_L10_001615F0 = 0.3f;
f32          D_L10_00161614 = 0.1f;
unsigned int D_L10_00161620 = 0x307f7f7f;
float        D_L10_00161628 = 0.1f;
float        D_L10_0016162C = 0.35f;

V_2bb0b0 D_L10_001CFE60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L10_001CFEA0 NOT_SDA = {0};

int   D_L10_001616AC = 0;
float D_L10_00161684 = 0.2f;
char  D_L10_00161688[4] = {5, 0, 0, 0};

char D_L10_0016168C[4] = {0x14, 0, 0, 0};

short D_L10_001D07F0 NOT_SDA = 0;
short D_L10_001D0808 NOT_SDA = 0;
f32                  D_L10_00161670 = 0.05f;
f32                  D_L10_00161674 = 0.5f;
char                 D_L10_00161660[4] = {-1, -1, -1, 0};

char D_L10_00161664[4] = {0x20, 0x20, 0x7f, 0};

char D_L10_00161690[4] = {0xe, 0, 0, 0};

char D_L10_00161694[4] = {0x10, 0, 0, 0};

float        D_L10_0016169C = -0.5f;
float        D_L10_00161698 = 0.75f;
int          D_L10_001616A8 = 8;
unsigned int D_L10_001616A0 = 0x307f4040;
unsigned int D_L10_001616A4 = 0x7f7f4040;

V_2bc1a0 D_L10_001D0460 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L10_001616C0 = 3.0f;
int                D_L10_001616CC = 0;
int                D_L10_0016179C = 0;
int                D_L10_001617A8 = 0;
f32                D_L10_001617AC = 0;
s32                D_L10_001616F8 = 0;
s32                D_L10_00161700 = 0;
s32                D_L10_00161704 = 0;
float              D_L10_001617B0 = 0;
s32                D_L10_00161804 = 0;
s32                D_L10_001617F8 = 0;
s32 D_L10_001D0850 NOT_SDA = 0;
s32 D_L10_001D19D0 NOT_SDA = 0;
signed char        D_L10_00161743 = 0x40;
int                D_L10_001617C8 = 0;
int                D_L10_001617E8 = 0;
int D_L10_001D0870 NOT_SDA = 0;
int D_L10_001D19A0 NOT_SDA = 0;
float              D_L10_001616D0 = 0.08333f;
int                D_L10_001616D4 = 6;
float              D_L10_001616D8 = 0.20071286f;
float              D_L10_001616DC = 0.39968038f;
float              D_L10_001616E4 = 0.3141592f;
float              D_L10_001616E8 = 0.05f;
float              D_L10_001616EC = 1.234f;
float              D_L10_0016170C = 0.01f;
float              D_L10_00161710 = 0.5f;
float              D_L10_00161714 = 0.2f;
float              D_L10_0016171C = 0.2617994f;
float              D_L10_00161720 = -0.25f;
float              D_L10_00161728 = 8.0f;
int                D_L10_0016172C = 0x3c;
int                D_L10_00161730 = 0x1e;
float              D_L10_00161738 = 1.5f;
float              D_L10_0016173C = 0.1f;
float              D_L10_00161748 = 2.13333f;
int                D_L10_001617B8[3] = {0};

float D_L10_001617D8[3] = {0};

Vec_2BE3A8 D_L10_001D0920[12] = {0};

Vec_2BE3A8 D_L10_001D09E0[12] = {0};

Tab D_L10_001DC8A0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L10_00161778 = 0x60007f00;
int   D_L10_00161760 = 0x7f7f7f;
int   D_L10_00161784 = 0xe;
int   D_L10_00161788 = 0x10;
int   D_L10_00161740 = 0x407f207f;
int   D_L10_00161744 = 0x60007f00;
float D_L10_00161794 = -0.5f;
float D_L10_0016178C = 0.4f;
float D_L10_00161790 = 0.1f;
int   D_L10_00161798 = 0x7f207f7f;
float D_L11_001615F8 = 0.7853982f;
float D_L11_001615FC = 0.55850536f;
float D_L11_00161600 = 0.017453292f;
float D_L11_00161604 = 0.03f;
float D_L11_00161608 = 0.03f;
float D_L11_00161610 = 0.34906584f;
float D_L11_00161618 = 0.80285144f;
float D_L11_0016161C = 0.99483764f;
float D_L11_00161620 = 0.8f;
float D_L11_00161624 = 0.5f;
float D_L11_0016162C = 0.05236f;
float D_L11_00161630 = 0.20943952f;
float D_L11_00161644 = 0;
float D_L11_00161648 = 0.1f;
int   D_L11_00161658 = 0x7f2020;
float D_L11_00161688 = 0.09f;
float D_L11_0016168C = 1.5f;
s16   D_L11_001616C0 = 0;
Vec4  D_L11_001D0890[4][5] = {0};

Vec4 D_L11_001D08A0[4][5] = {0};

f32          D_L11_0016164C = 0;
f32          D_L11_00161650 = 0.3f;
s32          D_L11_00161634 = 0x80;
s32          D_L11_0016163C = 0x40;
f32          D_L11_0016165C = 0.05f;
f32          D_L11_00161660 = 0.4f;
f32          D_L11_00161690 = 0.9f;
s32          D_L11_00161664 = 0x10;
s32          D_L11_00161668 = 0x20;
f32          D_L11_0016166C = 0.1f;
f32          D_L11_00161670 = 0.3f;
f32          D_L11_00161694 = 0.1f;
unsigned int D_L11_001616A0 = 0x307f7f7f;
float        D_L11_001616A8 = 0.1f;
float        D_L11_001616AC = 0.35f;

V_2bb0b0 D_L11_001D03E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L11_001D0420 NOT_SDA = {0};

int   D_L11_0016172C = 0;
float D_L11_00161704 = 0.2f;
char  D_L11_00161708[4] = {5, 0, 0, 0};

char D_L11_0016170C[4] = {0x14, 0, 0, 0};

short D_L11_001D0D70 NOT_SDA = 0;
short D_L11_001D0D88 NOT_SDA = 0;
f32                  D_L11_001616F0 = 0.05f;
f32                  D_L11_001616F4 = 0.5f;
char                 D_L11_001616E0[4] = {-1, -1, -1, 0};

char D_L11_001616E4[4] = {0x20, 0x20, 0x7f, 0};

char D_L11_00161710[4] = {0xe, 0, 0, 0};

char D_L11_00161714[4] = {0x10, 0, 0, 0};

float        D_L11_0016171C = -0.5f;
float        D_L11_00161718 = 0.75f;
int          D_L11_00161728 = 8;
unsigned int D_L11_00161720 = 0x307f4040;
unsigned int D_L11_00161724 = 0x7f7f4040;

V_2bc1a0 D_L11_001D09E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L11_00161740 = 3.0f;
int                D_L11_0016174C = 0;
int                D_L11_0016181C = 0;
int                D_L11_00161828 = 0;
f32                D_L11_0016182C = 0;
s32                D_L11_00161778 = 0;
s32                D_L11_00161780 = 0;
s32                D_L11_00161784 = 0;
float              D_L11_00161830 = 0;
s32                D_L11_00161884 = 0;
s32                D_L11_00161878 = 0;
s32 D_L11_001D0DD0 NOT_SDA = 0;
s32 D_L11_001D1F50 NOT_SDA = 0;
signed char        D_L11_001617C3 = 0x40;
int                D_L11_00161848 = 0;
int                D_L11_00161868 = 0;
int D_L11_001D0DF0 NOT_SDA = 0;
int D_L11_001D1F20 NOT_SDA = 0;
float              D_L11_00161750 = 0.08333f;
int                D_L11_00161754 = 6;
float              D_L11_00161758 = 0.20071286f;
float              D_L11_0016175C = 0.39968038f;
float              D_L11_00161764 = 0.3141592f;
float              D_L11_00161768 = 0.05f;
float              D_L11_0016176C = 1.234f;
float              D_L11_0016178C = 0.01f;
float              D_L11_00161790 = 0.5f;
float              D_L11_00161794 = 0.2f;
float              D_L11_0016179C = 0.2617994f;
float              D_L11_001617A0 = -0.25f;
float              D_L11_001617A8 = 8.0f;
int                D_L11_001617AC = 0x3c;
int                D_L11_001617B0 = 0x1e;
float              D_L11_001617B8 = 1.5f;
float              D_L11_001617BC = 0.1f;
float              D_L11_001617C8 = 2.13333f;
int                D_L11_00161838[3] = {0};

float D_L11_00161858[3] = {0};

Vec_2BE3A8 D_L11_001D0EA0[12] = {0};

Vec_2BE3A8 D_L11_001D0F60[12] = {0};

Tab D_L11_0021AD30 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L11_001617F8 = 0x60007f00;
int   D_L11_001617E0 = 0x7f7f7f;
int   D_L11_00161804 = 0xe;
int   D_L11_00161808 = 0x10;
int   D_L11_001617C0 = 0x407f207f;
int   D_L11_001617C4 = 0x60007f00;
float D_L11_00161814 = -0.5f;
float D_L11_0016180C = 0.4f;
float D_L11_00161810 = 0.1f;
int   D_L11_00161818 = 0x7f207f7f;
float D_L12_00161578 = 0.7853982f;
float D_L12_0016157C = 0.55850536f;
float D_L12_00161580 = 0.017453292f;
float D_L12_00161584 = 0.03f;
float D_L12_00161588 = 0.03f;
float D_L12_00161590 = 0.34906584f;
float D_L12_00161598 = 0.80285144f;
float D_L12_0016159C = 0.99483764f;
float D_L12_001615A0 = 0.8f;
float D_L12_001615A4 = 0.5f;
float D_L12_001615AC = 0.05236f;
float D_L12_001615B0 = 0.20943952f;
float D_L12_001615C4 = 0;
float D_L12_001615C8 = 0.1f;
int   D_L12_001615D8 = 0x7f2020;
float D_L12_00161608 = 0.09f;
float D_L12_0016160C = 1.5f;
s16   D_L12_00161640 = 0;
Vec4  D_L12_001E2F30[4][5] = {0};

Vec4 D_L12_001E2F40[4][5] = {0};

f32          D_L12_001615CC = 0;
f32          D_L12_001615D0 = 0.3f;
s32          D_L12_001615B4 = 0x80;
s32          D_L12_001615BC = 0x40;
f32          D_L12_001615DC = 0.05f;
f32          D_L12_001615E0 = 0.4f;
f32          D_L12_00161610 = 0.9f;
s32          D_L12_001615E4 = 0x10;
s32          D_L12_001615E8 = 0x20;
f32          D_L12_001615EC = 0.1f;
f32          D_L12_001615F0 = 0.3f;
f32          D_L12_00161614 = 0.1f;
unsigned int D_L12_00161620 = 0x307f7f7f;
float        D_L12_00161628 = 0.1f;
float        D_L12_0016162C = 0.35f;

V_2bb0b0 D_L12_001E2A80 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L12_001E2AC0 NOT_SDA = {0};

int   D_L12_001616AC = 0;
float D_L12_00161684 = 0.2f;
char  D_L12_00161688[4] = {5, 0, 0, 0};

char D_L12_0016168C[4] = {0x14, 0, 0, 0};

short D_L12_001E3410 NOT_SDA = 0;
short D_L12_001E3428 NOT_SDA = 0;
f32                  D_L12_00161670 = 0.05f;
f32                  D_L12_00161674 = 0.5f;
char                 D_L12_00161660[4] = {-1, -1, -1, 0};

char D_L12_00161664[4] = {0x20, 0x20, 0x7f, 0};

char D_L12_00161690[4] = {0xe, 0, 0, 0};

char D_L12_00161694[4] = {0x10, 0, 0, 0};

float        D_L12_0016169C = -0.5f;
float        D_L12_00161698 = 0.75f;
int          D_L12_001616A8 = 8;
unsigned int D_L12_001616A0 = 0x307f4040;
unsigned int D_L12_001616A4 = 0x7f7f4040;

V_2bc1a0 D_L12_001E3080 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L12_001616C0 = 3.0f;
int                D_L12_001616CC = 0;
int                D_L12_0016179C = 0;
int                D_L12_001617A8 = 0;
f32                D_L12_001617AC = 0;
s32                D_L12_001616F8 = 0;
s32                D_L12_00161700 = 0;
s32                D_L12_00161704 = 0;
float              D_L12_001617B0 = 0;
s32                D_L12_00161804 = 0;
s32                D_L12_001617F8 = 0;
s32 D_L12_001E3470 NOT_SDA = 0;
s32 D_L12_001E45F0 NOT_SDA = 0;
signed char        D_L12_00161743 = 0x40;
int                D_L12_001617C8 = 0;
int                D_L12_001617E8 = 0;
int D_L12_001E3490 NOT_SDA = 0;
int D_L12_001E45C0 NOT_SDA = 0;
float              D_L12_001616D0 = 0.08333f;
int                D_L12_001616D4 = 6;
float              D_L12_001616D8 = 0.20071286f;
float              D_L12_001616DC = 0.39968038f;
float              D_L12_001616E4 = 0.3141592f;
float              D_L12_001616E8 = 0.05f;
float              D_L12_001616EC = 1.234f;
float              D_L12_0016170C = 0.01f;
float              D_L12_00161710 = 0.5f;
float              D_L12_00161714 = 0.2f;
float              D_L12_0016171C = 0.2617994f;
float              D_L12_00161720 = -0.25f;
float              D_L12_00161728 = 8.0f;
int                D_L12_0016172C = 0x3c;
int                D_L12_00161730 = 0x1e;
float              D_L12_00161738 = 1.5f;
float              D_L12_0016173C = 0.1f;
float              D_L12_00161748 = 2.13333f;
int                D_L12_001617B8[3] = {0};

float D_L12_001617D8[3] = {0};

Vec_2BE3A8 D_L12_001E3540[12] = {0};

Vec_2BE3A8 D_L12_001E3600[12] = {0};

Tab D_L12_0020C000 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L12_00161778 = 0x60007f00;
int   D_L12_00161760 = 0x7f7f7f;
int   D_L12_00161784 = 0xe;
int   D_L12_00161788 = 0x10;
int   D_L12_00161740 = 0x407f207f;
int   D_L12_00161744 = 0x60007f00;
float D_L12_00161794 = -0.5f;
float D_L12_0016178C = 0.4f;
float D_L12_00161790 = 0.1f;
int   D_L12_00161798 = 0x7f207f7f;
float D_L13_00161648 = 0.7853982f;
float D_L13_0016164C = 0.55850536f;
float D_L13_00161650 = 0.017453292f;
float D_L13_00161654 = 0.03f;
float D_L13_00161658 = 0.03f;
float D_L13_00161660 = 0.34906584f;
float D_L13_00161668 = 0.80285144f;
float D_L13_0016166C = 0.99483764f;
float D_L13_00161670 = 0.8f;
float D_L13_00161674 = 0.5f;
float D_L13_0016167C = 0.05236f;
float D_L13_00161680 = 0.20943952f;
float D_L13_00161694 = 0;
float D_L13_00161698 = 0.1f;
int   D_L13_001616A8 = 0x7f2020;
float D_L13_001616D8 = 0.09f;
float D_L13_001616DC = 1.5f;
s16   D_L13_00161710 = 0;
Vec4  D_L13_001D07A0[4][5] = {0};

Vec4 D_L13_001D07B0[4][5] = {0};

f32          D_L13_0016169C = 0;
f32          D_L13_001616A0 = 0.3f;
s32          D_L13_00161684 = 0x80;
s32          D_L13_0016168C = 0x40;
f32          D_L13_001616AC = 0.05f;
f32          D_L13_001616B0 = 0.4f;
f32          D_L13_001616E0 = 0.9f;
s32          D_L13_001616B4 = 0x10;
s32          D_L13_001616B8 = 0x20;
f32          D_L13_001616BC = 0.1f;
f32          D_L13_001616C0 = 0.3f;
f32          D_L13_001616E4 = 0.1f;
unsigned int D_L13_001616F0 = 0x307f7f7f;
float        D_L13_001616F8 = 0.1f;
float        D_L13_001616FC = 0.35f;

V_2bb0b0 D_L13_001D02F0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L13_001D0330 NOT_SDA = {0};

int   D_L13_0016177C = 0;
float D_L13_00161754 = 0.2f;
char  D_L13_00161758[4] = {5, 0, 0, 0};

char D_L13_0016175C[4] = {0x14, 0, 0, 0};

short D_L13_001D0C80 NOT_SDA = 0;
short D_L13_001D0C98 NOT_SDA = 0;
f32                  D_L13_00161740 = 0.05f;
f32                  D_L13_00161744 = 0.5f;
char                 D_L13_00161730[4] = {-1, -1, -1, 0};

char D_L13_00161734[4] = {0x20, 0x20, 0x7f, 0};

char D_L13_00161760[4] = {0xe, 0, 0, 0};

char D_L13_00161764[4] = {0x10, 0, 0, 0};

float        D_L13_0016176C = -0.5f;
float        D_L13_00161768 = 0.75f;
int          D_L13_00161778 = 8;
unsigned int D_L13_00161770 = 0x307f4040;
unsigned int D_L13_00161774 = 0x7f7f4040;

V_2bc1a0 D_L13_001D08F0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L13_00161790 = 3.0f;
int                D_L13_0016179C = 0;
int                D_L13_0016186C = 0;
int                D_L13_00161878 = 0;
f32                D_L13_0016187C = 0;
s32                D_L13_001617C8 = 0;
s32                D_L13_001617D0 = 0;
s32                D_L13_001617D4 = 0;
float              D_L13_00161880 = 0;
s32                D_L13_001618D4 = 0;
s32                D_L13_001618C8 = 0;
s32 D_L13_001D0CE0 NOT_SDA = 0;
s32 D_L13_001D1E60 NOT_SDA = 0;
signed char        D_L13_00161813 = 0x40;
int                D_L13_00161898 = 0;
int                D_L13_001618B8 = 0;
int D_L13_001D0D00 NOT_SDA = 0;
int D_L13_001D1E30 NOT_SDA = 0;
float              D_L13_001617A0 = 0.08333f;
int                D_L13_001617A4 = 6;
float              D_L13_001617A8 = 0.20071286f;
float              D_L13_001617AC = 0.39968038f;
float              D_L13_001617B4 = 0.3141592f;
float              D_L13_001617B8 = 0.05f;
float              D_L13_001617BC = 1.234f;
float              D_L13_001617DC = 0.01f;
float              D_L13_001617E0 = 0.5f;
float              D_L13_001617E4 = 0.2f;
float              D_L13_001617EC = 0.2617994f;
float              D_L13_001617F0 = -0.25f;
float              D_L13_001617F8 = 8.0f;
int                D_L13_001617FC = 0x3c;
int                D_L13_00161800 = 0x1e;
float              D_L13_00161808 = 1.5f;
float              D_L13_0016180C = 0.1f;
float              D_L13_00161818 = 2.13333f;
int                D_L13_00161888[3] = {0};

float D_L13_001618A8[3] = {0};

Vec_2BE3A8 D_L13_001D0DB0[12] = {0};

Vec_2BE3A8 D_L13_001D0E70[12] = {0};

Tab D_L13_001F4A50 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L13_00161848 = 0x60007f00;
int   D_L13_00161830 = 0x7f7f7f;
int   D_L13_00161854 = 0xe;
int   D_L13_00161858 = 0x10;
int   D_L13_00161810 = 0x407f207f;
int   D_L13_00161814 = 0x60007f00;
float D_L13_00161864 = -0.5f;
float D_L13_0016185C = 0.4f;
float D_L13_00161860 = 0.1f;
int   D_L13_00161868 = 0x7f207f7f;
float D_L14_00161788 = 0.7853982f;
float D_L14_0016178C = 0.55850536f;
float D_L14_00161790 = 0.017453292f;
float D_L14_00161794 = 0.03f;
float D_L14_00161798 = 0.03f;
float D_L14_001617A0 = 0.34906584f;
float D_L14_001617A8 = 0.80285144f;
float D_L14_001617AC = 0.99483764f;
float D_L14_001617B0 = 0.8f;
float D_L14_001617B4 = 0.5f;
float D_L14_001617BC = 0.05236f;
float D_L14_001617C0 = 0.20943952f;
float D_L14_001617D4 = 0;
float D_L14_001617D8 = 0.1f;
int   D_L14_001617E8 = 0x7f2020;
float D_L14_00161818 = 0.09f;
float D_L14_0016181C = 1.5f;
s16   D_L14_00161850 = 0;
Vec4  D_L14_001DCE50[4][5] = {0};

Vec4 D_L14_001DCE60[4][5] = {0};

f32          D_L14_001617DC = 0;
f32          D_L14_001617E0 = 0.3f;
s32          D_L14_001617C4 = 0x80;
s32          D_L14_001617CC = 0x40;
f32          D_L14_001617EC = 0.05f;
f32          D_L14_001617F0 = 0.4f;
f32          D_L14_00161820 = 0.9f;
s32          D_L14_001617F4 = 0x10;
s32          D_L14_001617F8 = 0x20;
f32          D_L14_001617FC = 0.1f;
f32          D_L14_00161800 = 0.3f;
f32          D_L14_00161824 = 0.1f;
unsigned int D_L14_00161830 = 0x307f7f7f;
float        D_L14_00161838 = 0.1f;
float        D_L14_0016183C = 0.35f;

V_2bb0b0 D_L14_001DC9A0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L14_001DC9E0 NOT_SDA = {0};

int   D_L14_001618BC = 0;
float D_L14_00161894 = 0.2f;
char  D_L14_00161898[4] = {5, 0, 0, 0};

char D_L14_0016189C[4] = {0x14, 0, 0, 0};

short D_L14_001DD330 NOT_SDA = 0;
short D_L14_001DD348 NOT_SDA = 0;
f32                  D_L14_00161880 = 0.05f;
f32                  D_L14_00161884 = 0.5f;
char                 D_L14_00161870[4] = {-1, -1, -1, 0};

char D_L14_00161874[4] = {0x20, 0x20, 0x7f, 0};

char D_L14_001618A0[4] = {0xe, 0, 0, 0};

char D_L14_001618A4[4] = {0x10, 0, 0, 0};

float        D_L14_001618AC = -0.5f;
float        D_L14_001618A8 = 0.75f;
int          D_L14_001618B8 = 8;
unsigned int D_L14_001618B0 = 0x307f4040;
unsigned int D_L14_001618B4 = 0x7f7f4040;

V_2bc1a0 D_L14_001DCFA0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L14_001618D0 = 3.0f;
int                D_L14_001618DC = 0;
int                D_L14_001619AC = 0;
int                D_L14_001619B8 = 0;
f32                D_L14_001619BC = 0;
s32                D_L14_00161908 = 0;
s32                D_L14_00161910 = 0;
s32                D_L14_00161914 = 0;
float              D_L14_001619C0 = 0;
s32                D_L14_00161A14 = 0;
s32                D_L14_00161A08 = 0;
s32 D_L14_001DD390 NOT_SDA = 0;
s32 D_L14_001DE510 NOT_SDA = 0;
signed char        D_L14_00161953 = 0x40;
int                D_L14_001619D8 = 0;
int                D_L14_001619F8 = 0;
int D_L14_001DD3B0 NOT_SDA = 0;
int D_L14_001DE4E0 NOT_SDA = 0;
float              D_L14_001618E0 = 0.08333f;
int                D_L14_001618E4 = 6;
float              D_L14_001618E8 = 0.20071286f;
float              D_L14_001618EC = 0.39968038f;
float              D_L14_001618F4 = 0.3141592f;
float              D_L14_001618F8 = 0.05f;
float              D_L14_001618FC = 1.234f;
float              D_L14_0016191C = 0.01f;
float              D_L14_00161920 = 0.5f;
float              D_L14_00161924 = 0.2f;
float              D_L14_0016192C = 0.2617994f;
float              D_L14_00161930 = -0.25f;
float              D_L14_00161938 = 8.0f;
int                D_L14_0016193C = 0x3c;
int                D_L14_00161940 = 0x1e;
float              D_L14_00161948 = 1.5f;
float              D_L14_0016194C = 0.1f;
float              D_L14_00161958 = 2.13333f;
int                D_L14_001619C8[3] = {0};

float D_L14_001619E8[3] = {0};

Vec_2BE3A8 D_L14_001DD460[12] = {0};

Vec_2BE3A8 D_L14_001DD520[12] = {0};

Tab D_L14_001FC2E0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L14_00161988 = 0x60007f00;
int   D_L14_00161970 = 0x7f7f7f;
int   D_L14_00161994 = 0xe;
int   D_L14_00161998 = 0x10;
int   D_L14_00161950 = 0x407f207f;
int   D_L14_00161954 = 0x60007f00;
float D_L14_001619A4 = -0.5f;
float D_L14_0016199C = 0.4f;
float D_L14_001619A0 = 0.1f;
int   D_L14_001619A8 = 0x7f207f7f;
float D_L15_00161728 = 0.7853982f;
float D_L15_0016172C = 0.55850536f;
float D_L15_00161730 = 0.017453292f;
float D_L15_00161734 = 0.03f;
float D_L15_00161738 = 0.03f;
float D_L15_00161740 = 0.34906584f;
float D_L15_00161748 = 0.80285144f;
float D_L15_0016174C = 0.99483764f;
float D_L15_00161750 = 0.8f;
float D_L15_00161754 = 0.5f;
float D_L15_0016175C = 0.05236f;
float D_L15_00161760 = 0.20943952f;
float D_L15_00161774 = 0;
float D_L15_00161778 = 0.1f;
float D_L15_001617B8 = 0.09f;
float D_L15_001617BC = 1.5f;
s16   D_L15_001617F0 = 0;
Vec4  D_L15_001D0910[4][5] = {0};

Vec4 D_L15_001D0920[4][5] = {0};

f32          D_L15_00161780 = 0.3f;
s32          D_L15_00161764 = 0x80;
s32          D_L15_0016176C = 0x40;
f32          D_L15_00161790 = 0.4f;
f32          D_L15_001617C0 = 0.9f;
s32          D_L15_00161794 = 0x10;
s32          D_L15_00161798 = 0x20;
f32          D_L15_0016179C = 0.1f;
f32          D_L15_001617A0 = 0.3f;
f32          D_L15_001617C4 = 0.1f;
unsigned int D_L15_001617D0 = 0x307f7f7f;
float        D_L15_001617D8 = 0.1f;
float        D_L15_001617DC = 0.35f;

V_2bb0b0 D_L15_001D0460 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L15_001D04A0 NOT_SDA = {0};

int   D_L15_0016185C = 0;
float D_L15_00161834 = 0.2f;
char  D_L15_00161838[4] = {5, 0, 0, 0};

char D_L15_0016183C[4] = {0x14, 0, 0, 0};

short D_L15_001D0DF0 NOT_SDA = 0;
short D_L15_001D0E08 NOT_SDA = 0;
f32                  D_L15_00161820 = 0.05f;
f32                  D_L15_00161824 = 0.5f;
char                 D_L15_00161810[4] = {-1, -1, -1, 0};

char D_L15_00161814[4] = {0x20, 0x20, 0x7f, 0};

char D_L15_00161840[4] = {0xe, 0, 0, 0};

char D_L15_00161844[4] = {0x10, 0, 0, 0};

float        D_L15_0016184C = -0.5f;
float        D_L15_00161848 = 0.75f;
int          D_L15_00161858 = 8;
unsigned int D_L15_00161850 = 0x307f4040;
unsigned int D_L15_00161854 = 0x7f7f4040;

V_2bc1a0 D_L15_001D0A60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L15_00161870 = 3.0f;
int                D_L15_0016187C = 0;
int                D_L15_0016194C = 0;
int                D_L15_00161958 = 0;
f32                D_L15_0016195C = 0;
s32                D_L15_001618A8 = 0;
s32                D_L15_001618B0 = 0;
s32                D_L15_001618B4 = 0;
float              D_L15_00161960 = 0;
s32                D_L15_001619B4 = 0;
s32                D_L15_001619A8 = 0;
s32 D_L15_001D0E50 NOT_SDA = 0;
s32 D_L15_001D1FD0 NOT_SDA = 0;
signed char        D_L15_001618F3 = 0x40;
int                D_L15_00161978 = 0;
int                D_L15_00161998 = 0;
int D_L15_001D0E70 NOT_SDA = 0;
int D_L15_001D1FA0 NOT_SDA = 0;
float              D_L15_00161880 = 0.08333f;
int                D_L15_00161884 = 6;
float              D_L15_00161888 = 0.20071286f;
float              D_L15_0016188C = 0.39968038f;
float              D_L15_00161894 = 0.3141592f;
float              D_L15_00161898 = 0.05f;
float              D_L15_0016189C = 1.234f;
float              D_L15_001618BC = 0.01f;
float              D_L15_001618C0 = 0.5f;
float              D_L15_001618C4 = 0.2f;
float              D_L15_001618CC = 0.2617994f;
float              D_L15_001618D0 = -0.25f;
float              D_L15_001618D8 = 8.0f;
int                D_L15_001618DC = 0x3c;
int                D_L15_001618E0 = 0x1e;
float              D_L15_001618E8 = 1.5f;
float              D_L15_001618EC = 0.1f;
float              D_L15_001618F8 = 2.13333f;
int                D_L15_00161968[3] = {0};

float D_L15_00161988[3] = {0};

Vec_2BE3A8 D_L15_001D0F20[12] = {0};

Vec_2BE3A8 D_L15_001D0FE0[12] = {0};

Tab D_L15_001E2970 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L15_00161928 = 0x60007f00;
int   D_L15_00161910 = 0x7f7f7f;
int   D_L15_00161934 = 0xe;
int   D_L15_00161938 = 0x10;
int   D_L15_001618F0 = 0x407f207f;
int   D_L15_001618F4 = 0x60007f00;
float D_L15_00161944 = -0.5f;
float D_L15_0016193C = 0.4f;
float D_L15_00161940 = 0.1f;
int   D_L15_00161948 = 0x7f207f7f;
float D_L16_00161628 = 0.7853982f;
float D_L16_0016162C = 0.55850536f;
float D_L16_00161630 = 0.017453292f;
float D_L16_00161634 = 0.03f;
float D_L16_00161638 = 0.03f;
float D_L16_00161640 = 0.34906584f;
float D_L16_00161648 = 0.80285144f;
float D_L16_0016164C = 0.99483764f;
float D_L16_00161650 = 0.8f;
float D_L16_00161654 = 0.5f;
float D_L16_0016165C = 0.05236f;
float D_L16_00161660 = 0.20943952f;
float D_L16_00161674 = 0;
float D_L16_00161678 = 0.1f;
int   D_L16_00161688 = 0x7f2020;
float D_L16_001616B8 = 0.09f;
float D_L16_001616BC = 1.5f;
s16   D_L16_001616F0 = 0;
Vec4  D_L16_001D0310[4][5] = {0};

Vec4 D_L16_001D0320[4][5] = {0};

f32          D_L16_0016167C = 0;
f32          D_L16_00161680 = 0.3f;
s32          D_L16_00161664 = 0x80;
s32          D_L16_0016166C = 0x40;
f32          D_L16_0016168C = 0.05f;
f32          D_L16_00161690 = 0.4f;
f32          D_L16_001616C0 = 0.9f;
s32          D_L16_00161694 = 0x10;
s32          D_L16_00161698 = 0x20;
f32          D_L16_0016169C = 0.1f;
f32          D_L16_001616A0 = 0.3f;
f32          D_L16_001616C4 = 0.1f;
unsigned int D_L16_001616D0 = 0x307f7f7f;
float        D_L16_001616D8 = 0.1f;
float        D_L16_001616DC = 0.35f;

V_2bb0b0 D_L16_001CFE60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L16_001CFEA0 NOT_SDA = {0};

int   D_L16_0016175C = 0;
float D_L16_00161734 = 0.2f;
char  D_L16_00161738[4] = {5, 0, 0, 0};

char D_L16_0016173C[4] = {0x14, 0, 0, 0};

short D_L16_001D07F0 NOT_SDA = 0;
short D_L16_001D0808 NOT_SDA = 0;
f32                  D_L16_00161720 = 0.05f;
f32                  D_L16_00161724 = 0.5f;
char                 D_L16_00161710[4] = {-1, -1, -1, 0};

char D_L16_00161714[4] = {0x20, 0x20, 0x7f, 0};

char D_L16_00161740[4] = {0xe, 0, 0, 0};

char D_L16_00161744[4] = {0x10, 0, 0, 0};

float        D_L16_0016174C = -0.5f;
float        D_L16_00161748 = 0.75f;
int          D_L16_00161758 = 8;
unsigned int D_L16_00161750 = 0x307f4040;
unsigned int D_L16_00161754 = 0x7f7f4040;

V_2bc1a0 D_L16_001D0460 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L16_00161770 = 3.0f;
int                D_L16_0016177C = 0;
int                D_L16_0016184C = 0;
int                D_L16_00161858 = 0;
f32                D_L16_0016185C = 0;
s32                D_L16_001617A8 = 0;
s32                D_L16_001617B0 = 0;
s32                D_L16_001617B4 = 0;
float              D_L16_00161860 = 0;
s32                D_L16_001618B4 = 0;
s32                D_L16_001618A8 = 0;
s32 D_L16_001D0850 NOT_SDA = 0;
s32 D_L16_001D19D0 NOT_SDA = 0;
signed char        D_L16_001617F3 = 0x40;
int                D_L16_00161878 = 0;
int                D_L16_00161898 = 0;
int D_L16_001D0870 NOT_SDA = 0;
int D_L16_001D19A0 NOT_SDA = 0;
float              D_L16_00161780 = 0.08333f;
int                D_L16_00161784 = 6;
float              D_L16_00161788 = 0.20071286f;
float              D_L16_0016178C = 0.39968038f;
float              D_L16_00161794 = 0.3141592f;
float              D_L16_00161798 = 0.05f;
float              D_L16_0016179C = 1.234f;
float              D_L16_001617BC = 0.01f;
float              D_L16_001617C0 = 0.5f;
float              D_L16_001617C4 = 0.2f;
float              D_L16_001617CC = 0.2617994f;
float              D_L16_001617D0 = -0.25f;
float              D_L16_001617D8 = 8.0f;
int                D_L16_001617DC = 0x3c;
int                D_L16_001617E0 = 0x1e;
float              D_L16_001617E8 = 1.5f;
float              D_L16_001617EC = 0.1f;
float              D_L16_001617F8 = 2.13333f;
int                D_L16_00161868[3] = {0};

float D_L16_00161888[3] = {0};

Vec_2BE3A8 D_L16_001D0920[12] = {0};

Vec_2BE3A8 D_L16_001D09E0[12] = {0};

Tab D_L16_001E7E00 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L16_00161828 = 0x60007f00;
int   D_L16_00161810 = 0x7f7f7f;
int   D_L16_00161834 = 0xe;
int   D_L16_00161838 = 0x10;
int   D_L16_001617F0 = 0x407f207f;
int   D_L16_001617F4 = 0x60007f00;
float D_L16_00161844 = -0.5f;
float D_L16_0016183C = 0.4f;
float D_L16_00161840 = 0.1f;
int   D_L16_00161848 = 0x7f207f7f;
float D_L17_00161678 = 0.7853982f;
float D_L17_0016167C = 0.55850536f;
float D_L17_00161680 = 0.017453292f;
float D_L17_00161684 = 0.03f;
float D_L17_00161688 = 0.03f;
float D_L17_00161690 = 0.34906584f;
float D_L17_00161698 = 0.80285144f;
float D_L17_0016169C = 0.99483764f;
float D_L17_001616A0 = 0.8f;
float D_L17_001616A4 = 0.5f;
float D_L17_001616AC = 0.05236f;
float D_L17_001616B0 = 0.20943952f;
float D_L17_001616C4 = 0;
float D_L17_001616C8 = 0.1f;
float D_L17_00161708 = 0.09f;
float D_L17_0016170C = 1.5f;
s16   D_L17_00161740 = 0;
Vec4  D_L17_001D0B90[4][5] = {0};

Vec4 D_L17_001D0BA0[4][5] = {0};

f32          D_L17_001616D0 = 0.3f;
s32          D_L17_001616B4 = 0x80;
s32          D_L17_001616BC = 0x40;
f32          D_L17_001616E0 = 0.4f;
f32          D_L17_00161710 = 0.9f;
s32          D_L17_001616E4 = 0x10;
s32          D_L17_001616E8 = 0x20;
f32          D_L17_001616EC = 0.1f;
f32          D_L17_001616F0 = 0.3f;
f32          D_L17_00161714 = 0.1f;
unsigned int D_L17_00161720 = 0x307f7f7f;
float        D_L17_00161728 = 0.1f;
float        D_L17_0016172C = 0.35f;

V_2bb0b0 D_L17_001D06E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L17_001D0720 NOT_SDA = {0};

int   D_L17_001617AC = 0;
float D_L17_00161784 = 0.2f;
char  D_L17_00161788[4] = {5, 0, 0, 0};

char D_L17_0016178C[4] = {0x14, 0, 0, 0};

short D_L17_001D1070 NOT_SDA = 0;
short D_L17_001D1088 NOT_SDA = 0;
f32                  D_L17_00161770 = 0.05f;
f32                  D_L17_00161774 = 0.5f;
char                 D_L17_00161760[4] = {-1, -1, -1, 0};

char D_L17_00161764[4] = {0x20, 0x20, 0x7f, 0};

char D_L17_00161790[4] = {0xe, 0, 0, 0};

char D_L17_00161794[4] = {0x10, 0, 0, 0};

float        D_L17_0016179C = -0.5f;
float        D_L17_00161798 = 0.75f;
int          D_L17_001617A8 = 8;
unsigned int D_L17_001617A0 = 0x307f4040;
unsigned int D_L17_001617A4 = 0x7f7f4040;

V_2bc1a0 D_L17_001D0CE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L17_001617C0 = 3.0f;
int                D_L17_001617CC = 0;
int                D_L17_0016189C = 0;
int                D_L17_001618A8 = 0;
f32                D_L17_001618AC = 0;
s32                D_L17_001617F8 = 0;
s32                D_L17_00161800 = 0;
s32                D_L17_00161804 = 0;
float              D_L17_001618B0 = 0;
s32                D_L17_00161904 = 0;
s32                D_L17_001618F8 = 0;
s32 D_L17_001D10D0 NOT_SDA = 0;
s32 D_L17_001D2250 NOT_SDA = 0;
signed char        D_L17_00161843 = 0x40;
int                D_L17_001618C8 = 0;
int                D_L17_001618E8 = 0;
int D_L17_001D10F0 NOT_SDA = 0;
int D_L17_001D2220 NOT_SDA = 0;
float              D_L17_001617D0 = 0.08333f;
int                D_L17_001617D4 = 6;
float              D_L17_001617D8 = 0.20071286f;
float              D_L17_001617DC = 0.39968038f;
float              D_L17_001617E4 = 0.3141592f;
float              D_L17_001617E8 = 0.05f;
float              D_L17_001617EC = 1.234f;
float              D_L17_0016180C = 0.01f;
float              D_L17_00161810 = 0.5f;
float              D_L17_00161814 = 0.2f;
float              D_L17_0016181C = 0.2617994f;
float              D_L17_00161820 = -0.25f;
float              D_L17_00161828 = 8.0f;
int                D_L17_0016182C = 0x3c;
int                D_L17_00161830 = 0x1e;
float              D_L17_00161838 = 1.5f;
float              D_L17_0016183C = 0.1f;
float              D_L17_00161848 = 2.13333f;
int                D_L17_001618B8[3] = {0};

float D_L17_001618D8[3] = {0};

Vec_2BE3A8 D_L17_001D11A0[12] = {0};

Vec_2BE3A8 D_L17_001D1260[12] = {0};

Tab D_L17_001E6540 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L17_00161878 = 0x60007f00;
int   D_L17_00161860 = 0x7f7f7f;
int   D_L17_00161884 = 0xe;
int   D_L17_00161888 = 0x10;
int   D_L17_00161840 = 0x407f207f;
int   D_L17_00161844 = 0x60007f00;
float D_L17_00161894 = -0.5f;
float D_L17_0016188C = 0.4f;
float D_L17_00161890 = 0.1f;
int   D_L17_00161898 = 0x7f207f7f;
float D_L18_00161608 = 0.7853982f;
float D_L18_0016160C = 0.55850536f;
float D_L18_00161610 = 0.017453292f;
float D_L18_00161614 = 0.03f;
float D_L18_00161618 = 0.03f;
float D_L18_00161620 = 0.34906584f;
float D_L18_00161628 = 0.80285144f;
float D_L18_0016162C = 0.99483764f;
float D_L18_00161630 = 0.8f;
float D_L18_00161634 = 0.5f;
float D_L18_0016163C = 0.05236f;
float D_L18_00161640 = 0.20943952f;
float D_L18_00161654 = 0;
float D_L18_00161658 = 0.1f;
float D_L18_00161698 = 0.09f;
float D_L18_0016169C = 1.5f;
s16   D_L18_001616D0 = 0;
Vec4  D_L18_001D0890[4][5] = {0};

Vec4 D_L18_001D08A0[4][5] = {0};

f32          D_L18_00161660 = 0.3f;
s32          D_L18_00161644 = 0x80;
s32          D_L18_0016164C = 0x40;
f32          D_L18_00161670 = 0.4f;
f32          D_L18_001616A0 = 0.9f;
s32          D_L18_00161674 = 0x10;
s32          D_L18_00161678 = 0x20;
f32          D_L18_0016167C = 0.1f;
f32          D_L18_00161680 = 0.3f;
f32          D_L18_001616A4 = 0.1f;
unsigned int D_L18_001616B0 = 0x307f7f7f;
float        D_L18_001616B8 = 0.1f;
float        D_L18_001616BC = 0.35f;

V_2bb0b0 D_L18_001D03E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L18_001D0420 NOT_SDA = {0};

int   D_L18_0016173C = 0;
float D_L18_00161714 = 0.2f;
char  D_L18_00161718[4] = {5, 0, 0, 0};

char D_L18_0016171C[4] = {0x14, 0, 0, 0};

short D_L18_001D0D70 NOT_SDA = 0;
short D_L18_001D0D88 NOT_SDA = 0;
f32                  D_L18_00161700 = 0.05f;
f32                  D_L18_00161704 = 0.5f;
char                 D_L18_001616F0[4] = {-1, -1, -1, 0};

char D_L18_001616F4[4] = {0x20, 0x20, 0x7f, 0};

char D_L18_00161720[4] = {0xe, 0, 0, 0};

char D_L18_00161724[4] = {0x10, 0, 0, 0};

float        D_L18_0016172C = -0.5f;
float        D_L18_00161728 = 0.75f;
int          D_L18_00161738 = 8;
unsigned int D_L18_00161730 = 0x307f4040;
unsigned int D_L18_00161734 = 0x7f7f4040;

V_2bc1a0 D_L18_001D09E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

float              D_L18_00161750 = 3.0f;
int                D_L18_0016175C = 0;
int                D_L18_0016182C = 0;
int                D_L18_00161838 = 0;
f32                D_L18_0016183C = 0;
s32                D_L18_00161788 = 0;
s32                D_L18_00161790 = 0;
s32                D_L18_00161794 = 0;
float              D_L18_00161840 = 0;
s32                D_L18_00161894 = 0;
s32                D_L18_00161888 = 0;
s32 D_L18_001D0DD0 NOT_SDA = 0;
s32 D_L18_001D1F50 NOT_SDA = 0;
signed char        D_L18_001617D3 = 0x40;
int                D_L18_00161858 = 0;
int                D_L18_00161878 = 0;
int D_L18_001D0DF0 NOT_SDA = 0;
int D_L18_001D1F20 NOT_SDA = 0;
float              D_L18_00161760 = 0.08333f;
int                D_L18_00161764 = 6;
float              D_L18_00161768 = 0.20071286f;
float              D_L18_0016176C = 0.39968038f;
float              D_L18_00161774 = 0.3141592f;
float              D_L18_00161778 = 0.05f;
float              D_L18_0016177C = 1.234f;
float              D_L18_0016179C = 0.01f;
float              D_L18_001617A0 = 0.5f;
float              D_L18_001617A4 = 0.2f;
float              D_L18_001617AC = 0.2617994f;
float              D_L18_001617B0 = -0.25f;
float              D_L18_001617B8 = 8.0f;
int                D_L18_001617BC = 0x3c;
int                D_L18_001617C0 = 0x1e;
float              D_L18_001617C8 = 1.5f;
float              D_L18_001617CC = 0.1f;
float              D_L18_001617D8 = 2.13333f;
int                D_L18_00161848[3] = {0};

float D_L18_00161868[3] = {0};

Vec_2BE3A8 D_L18_001D0EA0[12] = {0};

Vec_2BE3A8 D_L18_001D0F60[12] = {0};

Tab D_L18_001F21E0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int   D_L18_00161808 = 0x60007f00;
int   D_L18_001617F0 = 0x7f7f7f;
int   D_L18_00161814 = 0xe;
int   D_L18_00161818 = 0x10;
int   D_L18_001617D0 = 0x407f207f;
int   D_L18_001617D4 = 0x60007f00;
float D_L18_00161824 = -0.5f;
float D_L18_0016181C = 0.4f;
float D_L18_00161820 = 0.1f;
int   D_L18_00161828 = 0x7f207f7f;

Vec4 D_L00_001DBA10[4][5] = {0};

float D_L00_0017AEBC NOT_SDA = 1.0f;

int D_L00_001616CC = 0;

int walloper_quad_texture_index_gp __asm__("D_L00_001616C8") = 8;

unsigned int l00_walloper_quad_common_base_vertex_color __asm__("D_L00_001616C0") = 0x307f4040;

int D_L00_001617BC = 0;

int D_L00_001617C8 = 0;

float D_L00_001617CC_macro __asm__("D_L00_001617CC") = 0;

s32 D_L00_00161718_gp __asm__("D_L00_00161718") = 0;

s32 D_L00_00161720_gp __asm__("D_L00_00161720") = 0;

s32 D_L00_00161724_gp __asm__("D_L00_00161724") = 0;

float D_L00_001617D0 = 0;

int D_L00_001DBF70 NOT_SDA = 0;

int D_L00_00161824_macro __asm__("D_L00_00161824") = 0;

int D_L00_001617D8_macro[3] __asm__("D_L00_001617D8") = {0};

float D_L00_001617F8_macro[3] __asm__("D_L00_001617F8") = {0};

Vec_2BE3A8 beam_prev_nodes[12] __asm__("D_L00_001DBF50") = {
    {0.0f, 1.0f, -1.0f, 1.0f},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

Vec_2BE3A8 beam_rail_a_points[12] __asm__("D_L00_001DC020") = {0};

Vec_2BE3A8 beam_rail_b_points[12] __asm__("D_L00_001DC0E0") = {0};

Vec_2BE3A8 beam_pulse_positions[3] __asm__("D_L00_001DD0A0") = {0};

Vec_2BE3A8 beam_pulse_directions[3] __asm__("D_L00_001DD0D0") = {0};

Tab l00_beam_origin_particle_color_palette __asm__("D_L00_001E9AB0") = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

float D_L00_001617E8_macro[3] __asm__("D_L00_001617E8") = {0};

float D_L00_00161808_macro[3] __asm__("D_L00_00161808") = {0};

int D_L00_00161818_macro[3] __asm__("D_L00_00161818") = {0};

Vec_2BE3A8 beam_nodes[1] __asm__("D_L00_001DBF60") = {0};
