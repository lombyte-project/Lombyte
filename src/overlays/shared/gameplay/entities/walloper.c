/* Shared moby classes: walloper, morphoray. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "rnc/gameplay/entities/moby.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/beam_vector.h"
extern float D_L00_001615AC_gp __asm__("D_L00_001615AC") __attribute__((sda));
extern float D_L00_001615B4_gp __asm__("D_L00_001615B4") __attribute__((sda));
extern float D_L00_001615C8_gp __asm__("D_L00_001615C8") __attribute__((sda));
extern char D_0013E533[];
#include "rnc/data_labels.h"
extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern float g_debug_camera[] __asm__("D_L00_00166DC0");
extern float player_camera[] __asm__("D_L00_00166C80");
extern u8 D_L00_001DB5C0[];
#include "rnc/overlay/collision.h"
extern CollisionHit coll_hit __asm__("D_L00_00173E40");
extern float D_L00_00161594 __attribute__((sda));
extern float D_L00_00161598 __attribute__((sda));
extern float D_L00_0016159C __attribute__((sda));
extern float D_L00_001615A0 __attribute__((sda));
extern float D_L00_001615A4 __attribute__((sda));
extern float D_L00_001615A8 __attribute__((sda));
extern float D_L00_001615B0 __attribute__((sda));
extern float D_L00_001615B8 __attribute__((sda));
extern float D_L00_001615BC __attribute__((sda));
extern float D_L00_001615C0 __attribute__((sda));
extern float D_L00_001615C4 __attribute__((sda));
extern float D_L00_001615CC __attribute__((sda));
extern float D_L00_001615D0 __attribute__((sda));
extern float D_L00_001615E4 __attribute__((sda));
extern float D_L00_001615E8 __attribute__((sda));
extern int D_L00_001615F8 __attribute__((sda));
extern float D_L00_00161628 __attribute__((sda));
extern float D_L00_0016162C __attribute__((sda));
extern short D_L00_00161658[] __attribute__((section(".sdata")));
extern short D_L00_00161668[] __attribute__((section(".sdata")));
extern short D_L00_00161670[] __attribute__((section(".sdata")));
extern s16 D_L00_00161660;
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void clear_u64_value() __asm__("FUN_001f99f8");
extern void blend_vector_2b94d0(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern float fast_cos(float), fast_sin(float), fast_arc_tan(float, float);
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer_s16() __asm__("FUN_001f9770");
extern int suck_get_aim_height() __asm__("FUN_002141f8");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
/* The arc points one slot before each arc row. */
extern Vec4 D_L00_001DBA10[4][5];
extern Vec4 arc_points[4][5] __asm__("D_L00_001DBA20");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int FUN_L00_00257b90(int, int), FUN_00213260(int);
extern void build_look_at_matrix(void *, void *, void *, float) __asm__("FUN_00214890");
extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern int test_moby_linked_record_word8(void *) __asm__("FUN_L00_00259830");
extern int collision_line(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern void collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_0022da68(int, int, void *);
extern char D_L00_0015F580_lui[] __asm__("D_L00_0015F580");
extern char D_L00_0015F580_macro[] __asm__("D_L00_0015F580") __attribute__((section(".sdata")));
extern unsigned char *FUN_L00_00272060(void *, int, float, float, int, int, int, float *, float) __asm__("FUN_L00_00272060");

typedef struct { Vec4 direction; int words[2]; u8 kind, mode; short id; float amount; int flag; } HitDescriptor;
typedef struct {
    Vec4 origin;
    Vec4 tip;
    struct Moby *target;
    u8 pad24[0x14];
    struct Moby *second_target;
    u8 pad3c[2];
    short count3e;
    short target_timer;
    short second_timer;
    short unk44;
    short unk46;
} WalloperBeam;

void build_beam_and_arc_points(u8 *o) __asm__("FUN_L00_002b94d0");
void build_beam_and_arc_points(u8 *o) {
    Vec4 target0, target1;
    struct { Vec4 direction; int words[2]; u8 kind, mode; short id; float amount; int flag; } hit;
    Vec4 camera_direction, direction, delta, blend, step, offset;
    struct Moby *collided[128];
    Vec4 target_delta;
    union { struct { Vec4 origin; HitDescriptor hit0; } first; HitDescriptor hit1; } effects;
    /* Retail uses a five-vector scratch span with differences beginning at slot 1. */
    Vec4 differences[5];
    WalloperBeam *v = *(WalloperBeam **)(o + 0x78);
    int i;
    struct Moby *best = 0, *second = 0, *candidate, **list;
    char *data;
    float best_score = 0.0f, second_score = 0.0f;
    int enabled = (u8)D_0013E533[0];
    float spacing = D_L00_00161594 / 20.0f;
    float distance, angle, score, n, amplitude, wave, length, factor, phase, side_wave;
    float side_phase;
    float x_s, x_c, x_a, x_m;
    struct Hero *player;
    float *camera;
    struct Hero *aim_hero;
    float primary_distance, sign;
    float distance_w17;
    float distance_w16;
    float factor_w15;
    float factor_w13;
    float length_w4;
    float length_w3;
    int k;
    int m;
    int mirror;
    int seg;
    int j, hit_result, collided_count, spin;
    char *previous, *side;
    Vec4 *beam;
    float *side_waves;
    D_L00_001615AC_gp = fast_add_rotations(D_L00_001615AC_gp, D_L00_001615B0);
    D_L00_001615B4_gp = fast_subtract_rotations(D_L00_001615B4_gp, D_L00_001615B8);
    D_L00_001615C8_gp = fast_add_rotations(D_L00_001615C8_gp, D_L00_001615CC);
    if (!v->target || (enabled && !v->second_target)) {
        if (v->second_target) {
            v->target = v->second_target;
            v->target_timer = v->second_timer;
            v->second_target = 0;
            v->second_timer = 0;
        } else {
            for (k = 0; (candidate = ((struct Moby **)active_mobies)[k]) != 0; k++) {
                camera = player_camera;
                aim_hero = &hero;
                if (candidate == v->target || candidate == v->second_target ||
                    !candidate || !candidate->pclass ||
                    *(short *)((char *)candidate->pclass + 0x46) != 5) continue;
                if (candidate->state == 0xfe) goto next_candidate;
                if (candidate->state == 0xfd) goto next_candidate;
                if (!suck_get_aim_height(candidate)) continue;
                fast_vec_sub(&target0, &candidate->pos, v);
                distance_w16 = vector_length_xy(&target0);
                if (D_L00_00161594 < distance_w16) continue;
                length_w3 = fast_difference_between_rotations(camera[0x56],
                    fast_arc_tan(candidate->pos.x - camera[0x50], candidate->pos.y - camera[0x51]));
                if (D_L00_0016159C < length_w3) continue;
                score = fast_difference_between_rotations(
                    fast_arc_tan(candidate->pos.x - aim_hero->motion.pos.f[0], candidate->pos.y - aim_hero->motion.pos.f[1]),
                    aim_hero->motion.rot.f[2]);
                if (D_L00_00161598 < score) continue;
                score = ((D_L00_0016159C - length_w3) / D_L00_0016159C) * D_L00_0016162C +
                        (D_L00_00161594 - distance_w16) / D_L00_00161594;
                if (best_score < score) {
                    second_score = best_score; second = best;
                    best_score = score; best = candidate;
                } else if (second_score < score) { second_score = score; second = candidate; }
next_candidate:;
            }
        }
    }
    if (best) {
        if (v->target) second = best;
        else {
            v->target = best;
            v->target_timer = 0;
            FUN_0022da68(1, 0, o);
        }
    }
    if (second && enabled) {
        distance_w17 = fast_vec_dist(v, &best->pos);
        if (distance_w17 < D_L00_00161594) {
            angle = fast_vec_dist(&best->pos, &second->pos);
            if (distance_w17 + angle < D_L00_00161594) {
                v->second_target = second;
                v->second_timer = 0;
            }
        }
    }
    primary_distance = 0.0f;
    if (v->target) {
        data = (char *)suck_get_aim_height(v->target);
        qcopy(&target0, &v->target->pos);
        target0.f[2] += *(float *)(data + 0x10);
        primary_distance = fast_vec_dist(v, &target0);
        spacing = primary_distance * 0.05f;
    }
    if (v->second_target) {
        data = (char *)suck_get_aim_height(v->second_target);
        qcopy(&target1, &v->second_target->pos);
        target1.f[2] += *(float *)(data + 0x10);
        spacing = (fast_vec_dist(&target0, &target1) + primary_distance) * 0.05f;
    }
    if (enabled) D_L00_001615F8 = 0x207f20;
    else D_L00_001615F8 = 0x7f2020;
    clear_u64_value(&camera_direction);
    make_hit_descriptor(&hit, o, 0x210000, &camera_direction, enabled + 2.0f);
    hit.kind = 5; hit.mode = 1; hit.id = *(short *)(o + 0xa6);
    v->count3e = 20;
    player = &hero;
    collect_mobies_in_sphere(0.3f, v, 0x10, player->moby, &hit);
    fast_vec_sub(&direction, &v->tip, v);
    fast_vec_normalize(&direction, &direction, 1.0f);
    if (v->target || v->second_target) {
        fast_vec_cross(&delta, &direction, &player->gravity_dir);
        build_look_at_matrix(&direction, &direction, &delta, 0.17453292f);
    }
    hit_result = 0;
    collided_count = 0;
    fast_vec_scale(spacing, &step, &direction);
    qcopy(D_L00_001DB5C0, v); qcopy(D_L00_001DB5C0 + 0x140, v);
    beam = (Vec4 *)D_L00_001DB5C0;
    side_waves = (float *)D_L00_001DB9D0;
    for (seg = 1; seg < 20; seg++) {
        previous = (char *)&beam[seg - 1];
        side = (char *)&beam[seg + 20];
        n = (float)seg;
        factor = D_L00_001615A4 + (D_L00_001615A8 - D_L00_001615A4) *
                    (convert_integer_to_float(seg) * 0.05f);
        phase = normalize_angle(D_L00_001615AC_gp + n * D_L00_001615A0);
        wave = factor * fast_sin(phase);
        wave += randf_sym(D_L00_001615E4, D_L00_001615E8);

        beam[seg].f[2] -= ((float *)D_L00_001DB840)[seg];
        ((float *)D_L00_001DB840)[seg] = wave;
        fast_vec_sub(&delta, &beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16);
        blend_vector_2b94d0(0.85f, &blend, &delta, &step);
        {
            float blend_length = fast_vec_length(&blend);
            if (blend_length == 0.0f) qcopy(&beam[seg], previous);
            else {
                fast_vec_scale(spacing / blend_length, &blend, &blend);
                fast_vec_add(&beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16, &blend); qcopy(&step, &blend);
            }
        }
        beam[seg].f[2] += wave;
        fast_vec_sub(&delta, &beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16);
        if (v->target) {
            if (!enabled || !v->second_target) {
                fast_vec_sub(&target_delta, &target0, &beam[seg]);
                length_w4 = fast_vec_length(&target_delta);
                if (length_w4 != 0.0f) {
                    if (seg == 19) { factor_w13 = 0.5f; spacing = length_w4; }
                    else if (seg >= 16) { factor_w13 = 0.5f; spacing = length_w4 / convert_integer_to_float(19 - seg); }
                    else factor_w13 = 0.1f;
                    fast_vec_scale(spacing / length_w4, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor_w13, &step, &step, &target_delta);
                }
            } else {
                if (seg >= 10) fast_vec_sub(&target_delta, &target1, &beam[seg]);
                else fast_vec_sub(&target_delta, &target0, &beam[seg]);
                length_w4 = fast_vec_length(&target_delta);
                if (length_w4 != 0.0f) {
                    if (seg == 9 || seg == 19) { factor_w13 = 0.7f; spacing = length_w4; }
                    else if ((unsigned)(seg - 5) < 5) { factor_w13 = 0.7f; spacing = length_w4 / convert_integer_to_float(9 - seg); }
                    else if (seg > 14) { factor_w13 = 0.7f; spacing = length_w4 / convert_integer_to_float(19 - seg); }
                    else factor_w13 = 0.2f;
                    fast_vec_scale(spacing / length_w4, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor_w13, &step, &step, &target_delta);
                }
            }
        }
        if (seg < 10) {
            side_phase = normalize_angle(D_L00_001615B4_gp + (float)seg * D_L00_001615BC);
            angle = normalize_angle(D_L00_001615C8_gp + (float)seg * D_L00_001615D0);
            factor = D_L00_001615C0 * fast_sin(angle);
            qcopy(side, &beam[seg]);
            x_s = fast_sin(side_phase);
            side_wave = randf_sym(0.0f, 0.1f);
            x_c = factor * x_s + side_wave;
            side_waves[seg] = x_c;
            *(float *)((char *)&beam[seg] + 0x148) += x_c;
            x_a = fast_add_rotations(fast_arc_tan(delta.f[0], delta.f[1]), 1.5707964f);
            x_m = D_L00_001615C4 * fast_cos(side_phase);
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
            struct Hero *h = &hero;
            fast_vec_scale(0.5f, &effects.first.origin, (char *)h->moby + 0xe0);
            fast_vec_add(&target_delta, &h->motion.pos, &effects.first.origin);
            hit_result = collision_line((Vec4f *)&target_delta, (Vec4f *)v, 0x10, h->moby, 0);
        } else if (seg == 19 || !(seg & 1)) {
            hit_result = collision_line((Vec4f *)((char *)D_L00_001DB5C0 - 32 + seg * 16), (Vec4f *)&beam[seg], 0x10, o, 0);
        } else continue;
        fast_vec_sub(&camera_direction, &beam[seg], g_debug_camera);
        camera_direction.f[2] = 0.0f;
        length = fast_vec_length(&camera_direction);
        if (length == 0.0f) camera_direction.f[2] = 1.0f;
        else fast_vec_scale(1.0f / length, &camera_direction, &camera_direction);
        camera_direction.f[3] = 5627.9248046875f;
        qcopy(&hit.direction, &camera_direction);
        if (hit_result) {
            candidate = (struct Moby *)coll_hit.moby;
            if (candidate) {
                if (!suck_get_aim_height(candidate) || !candidate->pclass ||
                    *(short *)((char *)candidate->pclass + 0x46) != 5)
                    record_moby_hit(candidate, &hit);
                else {
                    hit_result = 0; collided[collided_count++] = candidate;
                    candidate->collision = 0;
                }
            }
            if (hit_result) {
                qcopy(&beam[seg], &coll_hit.point);
                v->count3e = seg;
                qcopy(&v->tip, &coll_hit.point);
                if (!coll_hit.moby || test_moby_linked_record_word8(coll_hit.moby)) {
                    v->target = 0; v->target_timer = 0;
                }
                break;
            }
        }
    }
    for (k = 0; k < collided_count; k++)
        collided[k]->collision = *(int *)((char *)collided[k]->pclass + 0x10);
    if (!hit_result) qcopy(&v->tip, D_L00_001DB5C0 + 0x130);
    if (v->target && !(*(unsigned long *)&v->target_timer & 0xffff0000ffffUL)) {
        fast_vec_sub(&target_delta, &v->target->pos, g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(&effects.first.hit0, o, 0x210000, &target_delta, enabled + 2.0f);
        effects.first.hit0.kind = 5; effects.first.hit0.mode = 3; effects.first.hit0.id = *(short *)(o + 0xa6);
        record_moby_hit(v->target, &effects.first.hit0);
        {
            char *aim;
            int frames = 5;
            aim = (char *)suck_get_aim_height(v->target);
            if (aim && *(short *)(aim + 4) == 1) frames = 1;
            v->unk44 = scale_game_frames(frames);
        }
    }
    if (v->second_target && !(*(unsigned long *)&v->target_timer & 0xffff0000ffff0000UL)) {
        fast_vec_sub(&target_delta, &v->second_target->pos, g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(&effects.hit1, o, 0x210000, &target_delta, enabled + 2.0f);
        effects.hit1.kind = 5; effects.hit1.mode = 3; effects.hit1.id = *(short *)(o + 0xa6);
        record_moby_hit(v->second_target, &effects.hit1);
        {
            char *aim;
            int frames = 5;
            aim = (char *)suck_get_aim_height(v->second_target);
            if (aim && *(short *)(aim + 4) == 1) frames = 1;
            v->unk46 = scale_game_frames(frames);
        }
    }
    for (i = 0; i < 4; i++) {
        Vec4 *rows;
        if (i < 2) { qcopy(&effects.first.origin, v); factor_w15 = 0.4f; }
        else { qcopy(&effects.first.origin, &v->tip); factor_w15 = 0.5f; }
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
            for (m = 2; m < 5; m++) {
                fast_vec_sub(&effects.first.hit0, &arc_points[i][-1] + m, g_debug_camera);
                phase = random_float_between(0.17453292f, 0.9599311f) * sign;
                sign = -sign;
                build_look_at_matrix(&target_delta, &target_delta, &effects.first.hit0, phase);
                fast_vec_add(&arc_points[i][m], &arc_points[i][-1] + m, &target_delta);
            }
        } else {
            int d, n0;
            for (j = 1; j < 5; j++) {
                fast_vec_sub(&differences[j], &arc_points[i][j], &arc_points[i][-1] + j);
                differences[j].f[0] += randf_sym(0.0f, 0.1f);
                differences[j].f[1] += randf_sym(0.0f, 0.1f);
                differences[j].f[2] += randf_sym(0.0f, 0.1f);
            }
            qcopy(&arc_points[i][0], &effects.first.origin);
            for (j = 1; j < 5; j++)
                fast_vec_add(&arc_points[i][j], &arc_points[i][-1] + j, &differences[j]);
            d = D_L00_00161670[i]; n0 = D_L00_00161658[i];
            if (d < n0) d = n0 - d; else d = d - n0;
            distance = convert_integer_to_float(d) / convert_integer_to_float(scale_game_frames(15));
            (&D_L00_00161660)[i] = truncate_float_to_s32((1.0f - distance) * 32.0f);
        }
    }
    angle = randf_sym(0.5235988f, 1.0471976f);
    phase = random_float_between(-0.5235988f, 1.0471976f);
    fast_vec_scale(D_L00_00161628 * frame_scale, &target_delta, &direction);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5d0, phase);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5e0, angle);
    spin = FUN_L00_00257b90(1, 8); if (FUN_00213260(2)) spin = -spin;
    amplitude = random_float_between(0.2f, 0.6f);
    FUN_L00_00272060(v, scale_game_frames(15), amplitude * 0.3f, amplitude, D_L00_001615F8 | 0x7f000000, 0, spin, (float *)&target_delta, 0.0f);
    spin = -spin;
    FUN_L00_00272060(v, scale_game_frames(15), amplitude * 0.15f, amplitude * 0.5f, 0x307f7f7f, 0, spin, (float *)&target_delta, 0.0f);
    spin = FUN_L00_00257b90(1, 8); if (FUN_00213260(2)) spin = -spin;
    amplitude = random_float_between(0.15f, 3.0f);
    {
        /* Fakematch: a never-taken branch (cprop proves dead == 0, then jump deletes it). Retail
           had about 30 more insns before gcse that vanish later; they size the PRE hash table,
           which orders the &target_delta and &effects spill slots. */
        int dead = 0;
        if (amplitude < 0.0f) dead = 0;
        if (dead) {
            FUN_L00_00272060(v, scale_game_frames(15), amplitude, amplitude, 0, 0, spin, (float *)&target_delta, 0.0f);
            FUN_L00_00272060(v, scale_game_frames(15), amplitude, amplitude, 0, 0, spin, (float *)&target_delta, 0.0f);
            FUN_L00_00272060(v, scale_game_frames(15), amplitude, amplitude, 0, 0, spin, (float *)&target_delta, 0.0f);
        }
    }
    FUN_L00_00272060(&v->tip, scale_game_frames(12), amplitude * 0.1f, amplitude, D_L00_001615F8 | 0x7f000000, 0, spin, (float *)D_L00_0015F580_macro, 0.0f);
}
typedef struct {
    u8 pad[0x3E];
    s16 h3E;
} S2ba970;
#include "rnc/overlay/shared/gameplay/entities/walloper_globals.h"
extern f32 beam_first_pass_texture_u_offset_gp __asm__("D_L00_001615EC") __attribute__((sda));
extern f32 beam_first_pass_texture_u_scroll_speed_gp __asm__("D_L00_001615F0") __attribute__((sda));
extern s32 primary_beam_first_pass_alpha_gp __asm__("D_L00_001615D4") __attribute__((sda));
extern s32 primary_beam_second_pass_alpha_gp __asm__("D_L00_001615DC") __attribute__((sda));
extern f32 beam_and_arc_first_pass_half_width_gp __asm__("D_L00_001615FC") __attribute__((sda));
extern f32 primary_beam_second_pass_half_width_gp __asm__("D_L00_00161600") __attribute__((sda));
extern f32 primary_beam_second_pass_segment_extension_factor_gp __asm__("D_L00_00161630") __attribute__((sda));
extern s32 secondary_beam_first_pass_alpha_gp __asm__("D_L00_00161604") __attribute__((sda));
extern s32 secondary_beam_second_pass_alpha_gp __asm__("D_L00_00161608") __attribute__((sda));
extern f32 D_L00_0016160C __attribute__((sda));
extern f32 secondary_beam_and_arc_second_pass_half_width_gp __asm__("D_L00_00161610") __attribute__((sda));
extern f32 secondary_beam_second_pass_segment_extension_factor_gp __asm__("D_L00_00161634") __attribute__((sda));
extern s16 D_L00_00161660;
extern u8 D_L00_001DB5C0[];
extern Vec4 arc_points[4][5] __asm__("D_L00_001DBA20");
void draw_beam_and_arc_two_pass_quads(void *, s32, s32, s32, f32, f32, f32) __asm__("FUN_L00_002baae0");
void draw_beam_anchor_camera_facing_quads(void *mv) __asm__("FUN_L00_002bb0b0");

void draw_beam_anchor_camera_facing_quads(void *) __asm__("FUN_L00_002bb0b0");
void draw_beam_and_arc_effects(u8 *moby) __asm__("FUN_L00_002ba970");

void draw_beam_and_arc_effects(u8 *moby) {
    S2ba970 *moby_vars = *(S2ba970 **)(moby + 0x78);
    beam_first_pass_texture_u_offset_gp -= beam_first_pass_texture_u_scroll_speed_gp * frame_scale;
    if (beam_first_pass_texture_u_offset_gp <= -8.0f) {
        beam_first_pass_texture_u_offset_gp += 8.0f;
    }
    draw_beam_and_arc_two_pass_quads(D_L00_001DB5C0, primary_beam_first_pass_alpha_gp, primary_beam_second_pass_alpha_gp, moby_vars->h3E, beam_and_arc_first_pass_half_width_gp,
                     primary_beam_second_pass_half_width_gp, primary_beam_second_pass_segment_extension_factor_gp);
    draw_beam_and_arc_two_pass_quads(D_L00_001DB5C0 + 0x140, secondary_beam_first_pass_alpha_gp, secondary_beam_second_pass_alpha_gp, moby_vars->h3E, D_L00_0016160C,
                     secondary_beam_and_arc_second_pass_half_width_gp, secondary_beam_second_pass_segment_extension_factor_gp);
    draw_beam_and_arc_two_pass_quads(arc_points[0], D_L00_00161660, D_L00_00161660, 5, D_L00_0016160C,
                     secondary_beam_and_arc_second_pass_half_width_gp, 0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[1], beam_arc_second_row_alpha, beam_arc_second_row_alpha, 5, D_L00_0016160C,
                     secondary_beam_and_arc_second_pass_half_width_gp, 0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[2], beam_arc_third_row_alpha, beam_arc_third_row_alpha, 5, D_L00_0016160C,
                     secondary_beam_and_arc_second_pass_half_width_gp, 0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[3], beam_arc_fourth_row_alpha, beam_arc_fourth_row_alpha, 5, D_L00_0016160C,
                     secondary_beam_and_arc_second_pass_half_width_gp, 0.2f);
    draw_beam_anchor_camera_facing_quads(moby);
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

typedef float RW_2baae0[4] __attribute__((aligned(16)));

typedef struct {
    int c[4];
    float f[8];
    long z4, q0, q1, q2;
} RT_2baae0;

extern float g_debug_camera[] __asm__("D_L00_00166DC0");
#include "rnc/overlay/shared/gameplay/entities/walloper_data/draw_beam_and_arc_two_pass_quads.h"
extern int D_L00_001615F8 __attribute__((sda));
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern long tex_2baae0(int) __asm__("FUN_001f44b8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern void vu1_add_g_sregister(int, long) __asm__("FUN_00233980");

void draw_beam_and_arc_two_pass_quads(void *mv, int first_pass_alpha, int second_pass_alpha, int point_count, float sx, float sy, float sz) {
    char *point_bytes = mv;
    RW_2baae0 *v = mv;
    RW_2baae0 first_pass_corners[4];
    RT_2baae0 first_pass_render_attributes;
    RW_2baae0 q2[4];
    RT_2baae0 u;
    RW_2baae0 camera_facing_side_vector, second_pass_width_offset, first_pass_point_camera_offset, segment_delta, adjusted_segment_start, adjusted_segment_end, segment_start_camera_offset, segment_end_camera_offset, s1A0, s1B0;
    int i, col0, col1;
    float first_pass_side_normalization_factor, initial_first_pass_side_normalization_factor, segment_start_camera_distance, segment_end_camera_distance, second_pass_side_normalization_factor;
    first_pass_render_attributes.q0 = tex_2baae0(beam_and_arc_first_pass_texture_index_gp);
    col0 = beam_and_arc_first_pass_base_vertex_color_gp | (first_pass_alpha << 24);
    first_pass_render_attributes.q2 = 0x8000000048L;
    first_pass_render_attributes.q1 = 0xFF9000000260L;
    first_pass_render_attributes.z4 = 0;
    first_pass_render_attributes.c[3] = col0;
    first_pass_render_attributes.c[2] = col0;
    first_pass_render_attributes.c[1] = col0;
    first_pass_render_attributes.c[0] = col0;
    u.q0 = tex_2baae0(beam_and_arc_second_pass_texture_index_gp);
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
        initial_first_pass_side_normalization_factor = 1.0f / initial_first_pass_side_normalization_factor;
    fast_vec_scale(beam_and_arc_first_pass_half_width_gp * initial_first_pass_side_normalization_factor, camera_facing_side_vector, camera_facing_side_vector);
    fast_vec_add(first_pass_corners[0], point_bytes, camera_facing_side_vector);
    fast_vec_sub(first_pass_corners[1], point_bytes, camera_facing_side_vector);
    for (i = 1; i < point_count - 1; i++) {
        fast_vec_sub(first_pass_point_camera_offset, v[i], g_debug_camera);
        fast_vec_sub(segment_delta, v[i + 1], v[i]);
        fast_vec_cross(camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
        first_pass_side_normalization_factor = fast_vec_length(camera_facing_side_vector);
        if (first_pass_side_normalization_factor != 0)
            first_pass_side_normalization_factor = 1.0f / first_pass_side_normalization_factor;
        fast_vec_scale(beam_and_arc_first_pass_half_width_gp * first_pass_side_normalization_factor, camera_facing_side_vector, camera_facing_side_vector);
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
                fast_vec_scale(segment_start_camera_distance / segment_end_camera_distance, segment_end_camera_offset, segment_end_camera_offset);
        } else {
            if (segment_start_camera_distance == 0)
                qcopy(segment_start_camera_offset, segment_end_camera_offset);
            else
                fast_vec_scale(segment_end_camera_distance / segment_start_camera_distance, segment_start_camera_offset, segment_start_camera_offset);
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
        fast_vec_scale(sy * second_pass_side_normalization_factor, second_pass_width_offset, camera_facing_side_vector);
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
} UV_2bb0b0;
typedef struct {
    V_2bb0b0 corner[4];
    unsigned int color[4];
    UV_2bb0b0 uv[4];
    long unk70, tex, unk80, unk88;
} Q_2bb0b0;
extern unsigned int D_L00_001615F8_2bb0b0 __asm__("D_L00_001615F8") __attribute__((sda));
extern float g_debug_camera[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern char D_0013F5E0[];
void vsub_2bb0b0(void *, void *, void *) __asm__("FUN_001f9a28");
void vadd_2bb0b0(void *, void *, void *) __asm__("FUN_001f9a10");
void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void cross_2bb0b0(void *, void *, void *) __asm__("FUN_001f9ad8");
void vmul_2bb0b0(float, void *, void *) __asm__("FUN_001f9a68");
void xform_2bb0b0(void *, void *, void *) __asm__("FUN_001f9d20");
long tex_2bb0b0(int) __asm__("FUN_001f44b8");
float rnd_2bb0b0(float, float) __asm__("FUN_L00_00257c48");
void draw_2bb0b0(void *, void *, int) __asm__("FUN_001f7d30");
extern unsigned int D_L00_00161640 __attribute__((sda));
extern float beam_anchor_first_pass_base_corner_scale_gp __asm__("D_L00_00161648") __attribute__((sda));
extern float beam_anchor_second_pass_base_corner_scale_gp __asm__("D_L00_0016164C") __attribute__((sda));
extern V_2bb0b0 D_L00_001DB560[] __attribute__((section(".data")));
extern UV_2bb0b0 D_L00_001DB5A0[] __attribute__((section(".data")));
void draw_beam_anchor_camera_facing_quads(void *mv) {
    Q_2bb0b0 quad;
    V_2bb0b0 camera_facing_transform[4];
    V_2bb0b0 *quad_anchor_position = *(V_2bb0b0 **)((char *)mv + 0x78);
    unsigned int quad_color;
    float corner_scale;
    int i;
    quad.tex = tex_2bb0b0(0xB);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    vsub_2bb0b0(&camera_facing_transform[3], &(*(V_2bb0b0 *)g_debug_camera), quad_anchor_position);
    fast_vec_normalize(&camera_facing_transform[3], &camera_facing_transform[3], 0.3f);
    vadd_2bb0b0(&camera_facing_transform[3], &camera_facing_transform[3], quad_anchor_position);
    camera_facing_transform[3].f[3] = 1.0f;
    vsub_2bb0b0(&camera_facing_transform[0], &(*(V_2bb0b0 *)g_debug_camera), &camera_facing_transform[3]);
    fast_vec_normalize(&camera_facing_transform[0], &camera_facing_transform[0], 1.0f);
    cross_2bb0b0(&camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2bb0b0 *)D_0013F5E0));
    fast_vec_normalize(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    cross_2bb0b0(&camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    quad_color = D_L00_00161640;
    quad.color[3] = quad_color;
    quad.color[2] = quad_color;
    quad.color[1] = quad_color;
    quad.color[0] = quad_color;
    corner_scale = beam_anchor_first_pass_base_corner_scale_gp + rnd_2bb0b0(0.0f, 0.025f);
    for (i = 0; i < 4; i++) {
        vmul_2bb0b0(corner_scale, &quad.corner[i], &D_L00_001DB560[i]);
        xform_2bb0b0(&quad.corner[i], &quad.corner[i], &camera_facing_transform[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_2bb0b0(&quad, 0, 0);
    quad_color = D_L00_001615F8_2bb0b0;
    quad.color[3] = quad_color;
    quad.color[2] = quad_color;
    quad.color[1] = quad_color;
    quad.color[0] = quad_color;
    corner_scale = beam_anchor_second_pass_base_corner_scale_gp + rnd_2bb0b0(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        vmul_2bb0b0(corner_scale, &quad.corner[i], &D_L00_001DB560[i]);
        xform_2bb0b0(&quad.corner[i], &quad.corner[i], &camera_facing_transform[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_2bb0b0(&quad, 0, 0);
}
u8 *create_moby(s32) __asm__("FUN_0020c4f8");
void clear_u64_value() __asm__("FUN_001f99f8");
void refresh_moby_spatial_bounds_from_basis() __asm__("FUN_0020e098");
void FUN_L00_00250df8();
u8 *spawn_linked_moby_from_basis(u8 *basis_source_moby) __asm__("FUN_L00_002bb360");

u8 *spawn_linked_moby_from_basis(u8 *basis_source_moby) {
    u8 *spawned_moby = create_moby(0xB3);
    if (spawned_moby) {
        u8 *spawned_moby_vars = *(u8 **)(spawned_moby + 0x78);
        spawned_moby[0x30] = 0xFF;
        *(s16 *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        *(s16 *)(spawned_moby + 0x34) = 0x204;
        spawned_moby[0x23] = 0x50;
        spawned_moby[0x20] = 0;
        *(u8 **)(spawned_moby_vars + 8) = basis_source_moby;
        clear_u64_value(spawned_moby + 0x40);
        *(OvlQuad *)(spawned_moby + 0xC0) = *(OvlQuad *)(basis_source_moby + 0xC0);
        *(OvlQuad *)(spawned_moby + 0xD0) = *(OvlQuad *)(basis_source_moby + 0xD0);
        *(OvlQuad *)(spawned_moby + 0xE0) = *(OvlQuad *)(basis_source_moby + 0xE0);
        refresh_moby_spatial_bounds_from_basis(spawned_moby);
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
typedef struct {
    char pad[0x10B0];
    int i10B0;
} G_2bb400;
extern void FUN_0020c828_2bb400(void *) __asm__("FUN_0020c828");
extern int FUN_001f96f8_2bb400(int) __asm__("FUN_001f96f8");
extern void offset_moby_class_point_pairs(void *, int, int) __asm__("FUN_L00_0024efb0");
extern float FUN_002132a8_2bb400(float, float) __asm__("FUN_002132a8");
extern float FUN_L00_0025e310_2bb400(float) __asm__("FUN_L00_0025e310");
extern float FUN_001fa580_2bb400(float, float) __asm__("FUN_001fa580");
extern void FUN_0020e098_2bb400(void *) __asm__("FUN_0020e098");

typedef struct {
    int pad;
    int ang;
    unsigned char *o;
    float fC;
} V_2bb400;

void update_linked_moby_scale_and_rotation(unsigned char *moby) __asm__("FUN_L00_002bb400");

void update_linked_moby_scale_and_rotation(unsigned char *moby) {
    V_2bb400 *moby_vars = *(V_2bb400 **)(moby + 0x78);
    unsigned char *o = moby_vars->o;
    int old, unwrapped_angle, angle_delta;
    if (o[0x20] == 0 || (o[0x20] & 0xF0) == 0xF0) {
        FUN_0020c828_2bb400(moby);
        return;
    }
    if (o != 0 && (*(u16 *)(moby + 0x34) & 1) != (*(u16 *)(o + 0x34) & 1)) {
        *(u16 *)(moby + 0x34) &= ~1;
        *(u16 *)(moby + 0x34) |= *(u16 *)(moby_vars->o + 0x34) & 1;
    }
    if (hero.items[0].ready_frames < FUN_001f96f8_2bb400(0x19)) {
        *(u16 *)(moby + 0x34) |= 0x41;
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
    offset_moby_class_point_pairs(*(void **)(moby + 0x24), 0, angle_delta);
    *(float *)(moby + 0x2C) =
        *(float *)(*(char **)(moby + 0x24) + 0x24) * moby_vars->fC * FUN_002132a8_2bb400(0.5f, 1.0f);
    *(float *)(moby + 0x44) =
        FUN_001fa580_2bb400(*(float *)(moby + 0x44), FUN_L00_0025e310_2bb400(moby_vars->fC));
    FUN_0020e098_2bb400(moby);
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BC860), where it is exact; names translated to the US level program. */

typedef struct {
    float previous[4];
    float point[4][4];
} SparkleRowC860;

extern char D_0013E533[];
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
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
#include "rnc/overlay/shared/gameplay/entities/walloper_data/walloper_update_2.h"
extern float D_L00_00166DC0[];
extern float D_L00_0017AEBC __attribute__((section(".data")));
extern float FUN_L00_00257c48(float lo, float hi);
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int D_L00_001616CC;
#include "rnc/overlay/shared/gameplay/entities/walloper_data/walloper_update.h"
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32();
extern float D_L00_001616A4 __attribute__((sda));
extern short D_L00_00161660_e[] __asm__("D_L00_00161660") __attribute__((section(".sdata")));
short D_L00_001DBED8[10] = {0};
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
extern char D_L00_001616A8[4] __attribute__((sda));
extern char D_L00_001616AC[4] __attribute__((sda));
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
extern void random_spherical_offset(void *, float, float) __asm__("FUN_00213358");
extern void FUN_L00_002bbb00_c(int) __asm__("FUN_L00_002bbb00");
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void build_look_at_matrix(void *dst, void *vec, void *axis,
                                 float angle) __asm__("FUN_00214890");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
extern int func_001F9908_r(void *) __asm__("FUN_001f9740");

extern SparkleRowC860 D_L00_001DBB90[];
void walloper_update(char *m) __asm__("FUN_L00_002bb558");
void walloper_update(char *walloper_moby) {
    enum {
        WALLOPER_STATE_INITIALIZE = 0
    };
    int flag = 0;
    float arc_origin[4] __attribute__((aligned(16)));
    float arc_step[4] __attribute__((aligned(16)));
    float arc_point_camera_offset[4] __attribute__((aligned(16)));
    float arc_point_offsets[5][4] __attribute__((aligned(16)));
    struct Hero *walloper_hero_state = &hero;
    int i;
    int next_arc_index;
    int j;
    int arc_init_index;
    int any;

    if (*(unsigned char *)(((char *)walloper_hero_state->moby) + 0x53) != 0x82) {
        D_L00_0017AEBC = 0.1f;
    }
    switch (*(unsigned char *)(walloper_moby + 0x20)) {
    case WALLOPER_STATE_INITIALIZE:
        walloper_moby[0x20] = 2;
        for (arc_init_index = 0; arc_init_index < 10; arc_init_index++) {
            D_L00_001DBEC0[arc_init_index] = 0;
            D_L00_001DBEF0[arc_init_index] = 2;
            D_L00_001DBF08[arc_init_index] = 1;
        }
        D_L00_001616CC = scale_game_frames(*(int *)&D_L00_001616A8);
        walloper_effect_fade_out_countdown = scale_game_frames(*(int *)&D_L00_001616AC);
        break;
    case 2:
        if (walloper_hero_state->state.current == 0x20) {
            float interpolated_record_value = compute_interpolated_record_value(walloper_moby);
            if (6.0f <= interpolated_record_value) {
                flag = 1;
                if (func_001F9908_r(&D_L00_00161660_e[54]) == 0) {
                    float a = convert_integer_to_float(D_L00_001616CC);
                    a = a / convert_integer_to_float(scale_game_frames(*(int *)&D_L00_001616A8));
                    walloper_effect_alpha_scale = 1.0f - a;
                } else if (12.0f <= interpolated_record_value) {
                    if (func_001F9908_r(&D_L00_00161660_e[56]) == 0) {
                        float a = convert_integer_to_float(walloper_effect_fade_out_countdown);
                        a = a / convert_integer_to_float(scale_game_frames(*(int *)&D_L00_001616AC));
                        walloper_effect_alpha_scale = a;
                    } else {
                        walloper_effect_alpha_scale = 0.0f;
                    }
                } else {
                    walloper_effect_alpha_scale = 1.0f;
                }
            }
        } else {
            D_L00_001616CC = scale_game_frames(*(int *)&D_L00_001616A8);
            walloper_effect_fade_out_countdown = scale_game_frames(*(int *)&D_L00_001616AC);
            blend_moby_animation(walloper_moby, 1, 0, scale_game_frames(12));
        }
    }
    scale_vector_xyz(arc_origin, walloper_moby + 0xD0, -0.5f);
    add_vector_xyz(arc_origin, arc_origin, walloper_moby + 0x10);
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
                int k;
                D_L00_001DBEC0[i] = 2;
                D_L00_001DBED8[i] = 0x40;
                random_spherical_offset(arc_step, D_L00_001616A4, D_L00_001616A4);
                qcopy(D_L00_001DBBA0[i].point[0], arc_origin);
                next_arc_index = i + 1;
                sign = 1.0f;
                for (k = 1; k < 5; k++) {
                    float bend_angle;
                    subtract_vector_xyz(arc_point_camera_offset, D_L00_001DBB90[i].point[k - 1], D_L00_00166DC0);
                    bend_angle = random_float_between(0.17453292f, 0.7853982f) * sign;
                    sign = -sign;
                    build_look_at_matrix(arc_step, arc_step, arc_point_camera_offset, bend_angle);
                    add_vector_xyz(D_L00_001DBBA0[i].point[k],
                                   D_L00_001DBB90[i].point[k - 1], arc_step);
                }
            } else {
                next_arc_index = i + 1;
            }
        } else {
            int k;
            float(*workspace)[4] = arc_point_offsets;
            for (k = 1; k < 5; k++) {
                float *point_offset = workspace[k];
                subtract_vector_xyz(point_offset, D_L00_001DBBA0[i].point[k],
                                    D_L00_001DBB90[i].point[k - 1]);
                point_offset[0] += FUN_L00_00257c48(0.0f, 0.1f);
                point_offset[1] += FUN_L00_00257c48(0.0f, 0.1f);
                point_offset[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            next_arc_index = i + 1;
            qcopy(D_L00_001DBBA0[i].point[0], arc_origin);
            for (k = 1; k < 5; k++) {
                add_vector_xyz(D_L00_001DBBA0[i].point[k],
                               D_L00_001DBB90[i].point[k - 1], workspace[k]);
            }
            if (D_L00_001DBEC0[i] > 0)
                D_L00_001DBED8[i] = 0x20;
            else
                D_L00_001DBED8[i] = 0;
        }
    }
    enqueue_callback_list_1(FUN_L00_002bbb00_c, walloper_moby);
}
extern f32 D_L00_00161660_f[] __asm__("D_L00_00161660") __attribute__((section(".sdata")));
extern f32 walloper_arc_first_pass_half_width_gp __asm__("D_L00_00161690") __attribute__((sda));
extern f32 walloper_arc_second_pass_half_width_gp __asm__("D_L00_00161694") __attribute__((sda));
f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
void draw_walloper_arc_two_pass_quads(void *, s32, s32, s32, f32, f32, f32) __asm__("FUN_L00_002bbbc8");
void draw_walloper_camera_facing_quad(void *mv) __asm__("FUN_L00_002bc1a0");

void draw_walloper_camera_facing_quad(void *) __asm__("FUN_L00_002bc1a0");
void draw_walloper_arc_and_quad_effects(void *moby) __asm__("FUN_L00_002bbb00");

void draw_walloper_arc_and_quad_effects(void *moby) {
    s32 i;
    s32 arc_alpha;
    for (i = 0; i < 10; i++) {
        if (D_L00_001DBEC0[i] != 0) {
            arc_alpha = truncate_float_to_s32(convert_integer_to_float(D_L00_001DBED8[i]) * D_L00_00161660_f[29]);
            draw_walloper_arc_two_pass_quads(&D_L00_001DBBA0[i], arc_alpha, arc_alpha, 5, walloper_arc_first_pass_half_width_gp, walloper_arc_second_pass_half_width_gp, 0.2f);
        }
    }
    draw_walloper_camera_facing_quad(moby);
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BCED0), where it is exact; names translated to the US level program. */

typedef float RW[4] __attribute__((aligned(16)));

typedef struct {
    int c[4];
    float f[8];
    long z4, q0, q1, q2;
} RT;

extern float D_L00_00166DC0[];
extern float vector_length_xyz(void *);
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
extern char D_L00_00161680[4] __attribute__((sda));
extern char D_L00_00161684[4] __attribute__((sda));
extern char D_L00_001616B0[4] __attribute__((sda));
extern char D_L00_001616B4[4] __attribute__((sda));
extern void add_vector_xyz(void *, void *, void *);
extern void cross_vectors_xyz(void *, void *, void *);
extern void draw_geometry_quad(void *, void *, int);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
extern void vu1_add_g_sregister(int, long) __asm__("FUN_00233980");

void draw_walloper_arc_two_pass_quads(void *mv, int first_pass_alpha, int second_pass_alpha, int point_count, float sx, float sy, float sz) {
    char *point_bytes = mv;
    RW first_pass_corners[4];
    RT first_pass_render_attributes;
    RW q2[4];
    RT second_pass_render_attributes;
    RW arc_camera_facing_side_vector, second_pass_width_offset, first_pass_point_camera_offset, segment_delta, adjusted_segment_start, adjusted_segment_end, segment_start_camera_offset, segment_end_camera_offset, s1A0, s1B0;
    int i, col0, col1;
    float side_vector_normalization_factor, initial_side_vector_normalization_factor, segment_start_camera_distance, segment_end_camera_distance;
    first_pass_render_attributes.q0 = get_effect_texture(*(int *)&D_L00_001616B0);
    col0 = *(int *)&D_L00_00161680 | (first_pass_alpha << 24);
    first_pass_render_attributes.q2 = 0x8000000048L;
    first_pass_render_attributes.q1 = 0xFF9000000260L;
    first_pass_render_attributes.z4 = 0;
    first_pass_render_attributes.c[3] = col0;
    first_pass_render_attributes.c[2] = col0;
    first_pass_render_attributes.c[1] = col0;
    first_pass_render_attributes.c[0] = col0;
    second_pass_render_attributes.q0 = get_effect_texture(*(int *)&D_L00_001616B4);
    col1 = *(int *)&D_L00_00161684 | (second_pass_alpha << 24);
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
    subtract_vector_xyz(first_pass_point_camera_offset, point_bytes, D_L00_00166DC0);
    subtract_vector_xyz(segment_delta, point_bytes + 0x10, point_bytes);
    cross_vectors_xyz(arc_camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
    initial_side_vector_normalization_factor = vector_length_xyz(arc_camera_facing_side_vector);
    if (initial_side_vector_normalization_factor != 0)
        initial_side_vector_normalization_factor = 1.0f / initial_side_vector_normalization_factor;
    scale_vector_xyz(arc_camera_facing_side_vector, arc_camera_facing_side_vector, sx * initial_side_vector_normalization_factor);
    add_vector_xyz(first_pass_corners[0], point_bytes, arc_camera_facing_side_vector);
    subtract_vector_xyz(first_pass_corners[1], point_bytes, arc_camera_facing_side_vector);
    for (i = 1; i < point_count - 1; i++) {
        subtract_vector_xyz(first_pass_point_camera_offset, point_bytes + i * 16, D_L00_00166DC0);
        subtract_vector_xyz(segment_delta, point_bytes + i * 16 + 16, point_bytes + i * 16);
        cross_vectors_xyz(arc_camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
        side_vector_normalization_factor = vector_length_xyz(arc_camera_facing_side_vector);
        if (side_vector_normalization_factor != 0)
            side_vector_normalization_factor = 1.0f / side_vector_normalization_factor;
        scale_vector_xyz(arc_camera_facing_side_vector, arc_camera_facing_side_vector, sx * side_vector_normalization_factor);
        add_vector_xyz(first_pass_corners[2], point_bytes + i * 16, arc_camera_facing_side_vector);
        subtract_vector_xyz(first_pass_corners[3], point_bytes + i * 16, arc_camera_facing_side_vector);
        if (i == 1) {
            first_pass_render_attributes.c[1] = *(int *)&D_L00_00161680;
            first_pass_render_attributes.c[0] = *(int *)&D_L00_00161680;
        } else if (i == point_count - 2) {
            first_pass_render_attributes.c[3] = *(int *)&D_L00_00161680;
            first_pass_render_attributes.c[2] = *(int *)&D_L00_00161680;
        } else if (i == 2) {
            first_pass_render_attributes.c[1] = col0;
            first_pass_render_attributes.c[0] = col0;
        }
        draw_geometry_quad(first_pass_corners, 0, 0);
        qcopy(first_pass_corners[0], first_pass_corners[2]);
        qcopy(first_pass_corners[1], first_pass_corners[3]);
    }
    for (i = 0; i < point_count - 1; i++) {
        subtract_vector_xyz(segment_start_camera_offset, point_bytes + i * 16, D_L00_00166DC0);
        subtract_vector_xyz(segment_end_camera_offset, point_bytes + i * 16 + 16, D_L00_00166DC0);
        segment_start_camera_distance = vector_length_xyz(segment_start_camera_offset);
        segment_end_camera_distance = vector_length_xyz(segment_end_camera_offset);
        if (segment_end_camera_distance < segment_start_camera_distance) {
            if (segment_end_camera_distance == 0)
                qcopy(segment_end_camera_offset, segment_start_camera_offset);
            else
                scale_vector_xyz(segment_end_camera_offset, segment_end_camera_offset, segment_start_camera_distance / segment_end_camera_distance);
        } else {
            if (segment_start_camera_distance == 0)
                qcopy(segment_start_camera_offset, segment_end_camera_offset);
            else
                scale_vector_xyz(segment_start_camera_offset, segment_start_camera_offset, segment_end_camera_distance / segment_start_camera_distance);
        }
        add_vector_xyz(adjusted_segment_start, D_L00_00166DC0, segment_start_camera_offset);
        add_vector_xyz(adjusted_segment_end, D_L00_00166DC0, segment_end_camera_offset);
        subtract_vector_xyz(segment_delta, adjusted_segment_end, adjusted_segment_start);
        scale_vector_xyz(s1B0, segment_delta, sz);
        subtract_vector_xyz(adjusted_segment_start, adjusted_segment_start, s1B0);
        add_vector_xyz(adjusted_segment_end, adjusted_segment_end, s1B0);
        subtract_vector_xyz(s1A0, D_L00_00166DC0, adjusted_segment_start);
        cross_vectors_xyz(arc_camera_facing_side_vector, s1A0, segment_delta);
        side_vector_normalization_factor = vector_length_xyz(arc_camera_facing_side_vector);
        if (side_vector_normalization_factor != 0)
            side_vector_normalization_factor = 1.0f / side_vector_normalization_factor;
        scale_vector_xyz(second_pass_width_offset, arc_camera_facing_side_vector, sy * side_vector_normalization_factor);
        add_vector_xyz(q2[0], adjusted_segment_start, second_pass_width_offset);
        subtract_vector_xyz(q2[1], adjusted_segment_start, second_pass_width_offset);
        add_vector_xyz(q2[2], adjusted_segment_end, second_pass_width_offset);
        subtract_vector_xyz(q2[3], adjusted_segment_end, second_pass_width_offset);
        if (i == 0) {
            second_pass_render_attributes.c[1] = *(int *)&D_L00_00161684;
            second_pass_render_attributes.c[0] = *(int *)&D_L00_00161684;
        } else if (i == point_count - 2) {
            second_pass_render_attributes.c[3] = *(int *)&D_L00_00161684;
            second_pass_render_attributes.c[2] = *(int *)&D_L00_00161684;
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
    V_2bc1a0 corner[4];
    unsigned int color[4];
    int unk50, unk54, unk58;
    float unk5C, unk60;
    int unk64;
    float unk68, unk6C;
    long unk70, tex, unk80, unk88;
} Q_2bc1a0;
extern float g_debug_camera[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern char D_0013F5E0[];
void FUN_001f9a68_2bc1a0(float, void *, void *) __asm__("FUN_001f9a68");
void FUN_001f9a10_2bc1a0(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9a28_2bc1a0(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_001f9bf8_2bc1a0(void *, void *, float) __asm__("FUN_001f9bf8");
void FUN_001f9ad8_2bc1a0(void *, void *, void *) __asm__("FUN_001f9ad8");
void FUN_001f9d20_2bc1a0(void *, void *, void *) __asm__("FUN_001f9d20");
long FUN_001f44b8_2bc1a0(int) __asm__("FUN_001f44b8");
int FUN_00213260_2bc1a0(int) __asm__("FUN_00213260");
float FUN_001fa6c0_2bc1a0(int) __asm__("FUN_001fa6c0");
int FUN_001fa6d0_2bc1a0(float) __asm__("FUN_001fa6d0");
void FUN_001f7d30_2bc1a0(void *, void *, int) __asm__("FUN_001f7d30");
extern float walloper_quad_basis_offset_scale_gp __asm__("D_L00_001616BC") __attribute__((sda));
extern float walloper_quad_corner_scale_gp __asm__("D_L00_001616B8") __attribute__((sda));
extern int walloper_quad_texture_index_gp __asm__("D_L00_001616C8") __attribute__((sda));
extern unsigned int D_L00_001616C0 __attribute__((sda));
extern unsigned int D_L00_001616C4 __attribute__((sda));
extern V_2bc1a0 D_L00_001DBB60[] __attribute__((section(".data")));
void draw_walloper_camera_facing_quad(void *mv) {
    char *moby_bytes = mv;
    Q_2bc1a0 quad;
    V_2bc1a0 camera_facing_transform[4];
    V_2bc1a0 scaled_corner;
    V_2bc1a0 scaled_basis_offset;
    V_2bc1a0 camera_facing_offset;
    unsigned int uniform_vertex_color;
    int i;
    FUN_001f9a68_2bc1a0(walloper_quad_basis_offset_scale_gp, &scaled_basis_offset, moby_bytes + 0xD0);
    FUN_001f9a10_2bc1a0(&camera_facing_transform[3], &scaled_basis_offset, moby_bytes + 0x10);
    quad.tex = FUN_001f44b8_2bc1a0(walloper_quad_texture_index_gp);
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
    if (!FUN_00213260_2bc1a0(4))
        uniform_vertex_color = D_L00_001616C4;
    else
        uniform_vertex_color = D_L00_001616C0;
    uniform_vertex_color = (uniform_vertex_color & 0xFFFFFF) |
        (FUN_001fa6d0_2bc1a0(FUN_001fa6c0_2bc1a0((int)uniform_vertex_color >> 24) * D_L00_00161660_f[29]) << 24);
    quad.color[3] = uniform_vertex_color;
    quad.color[2] = uniform_vertex_color;
    quad.color[1] = uniform_vertex_color;
    quad.color[0] = uniform_vertex_color;
    FUN_001f9a28_2bc1a0(&camera_facing_offset, &(*(V_2bc1a0 *)g_debug_camera), &camera_facing_transform[3]);
    FUN_001f9bf8_2bc1a0(&camera_facing_transform[0], &camera_facing_offset, 1.0f);
    FUN_001f9ad8_2bc1a0(&camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2bc1a0 *)D_0013F5E0));
    FUN_001f9bf8_2bc1a0(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    FUN_001f9ad8_2bc1a0(&camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    FUN_001f9a68_2bc1a0(0.2f, &camera_facing_offset, &camera_facing_transform[0]);
    FUN_001f9a10_2bc1a0(&camera_facing_transform[3], &camera_facing_transform[3], &camera_facing_offset);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2bc1a0(walloper_quad_corner_scale_gp, &scaled_corner, &D_L00_001DBB60[3 - i]);
        FUN_001f9d20_2bc1a0(&quad.corner[3 - i], &scaled_corner, &camera_facing_transform[0]);
    }
    FUN_001f7d30_2bc1a0(&quad, 0, 0);
}
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BD6B8), where it is exact; names translated to the US level program. */

typedef u32 VecQuad __attribute__((mode(TI), aligned(16)));

extern Vec4 collision_hit_point __asm__("D_L00_00173E60");
extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_sin(f32) __asm__("func_001F9DF8");
extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int D_L00_00173E40[];
extern int collision_line(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern int fun_002141f8(char *) __asm__("FUN_002141f8");
extern float D_L00_001616DC __attribute__((sda));
#include "rnc/overlay/shared/gameplay/entities/walloper_data/select_morphoray_target_and_adjust_aim.h"
extern void add_vector_xyz_c4(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
float absolute_float(float input) __asm__("func_001F99C0");

char *select_morphoray_target_and_adjust_aim(char *self, float *position, float *heading, float *pitch) __asm__("FUN_L00_002bc3b0");

char *select_morphoray_target_and_adjust_aim(char *self, float *position, float *heading, float *pitch) {
    float start[4] __attribute__((aligned(16)));
    float end[4] __attribute__((aligned(16)));
    float target[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    char *candidate;
    char *data;
    char *moby;
    int collision;
    int a0s[1];
    int selected_target[1];
    float distance, angle, elevation, headingError, limit, apparentAngle, radius, reduction;
    int i;
    char *hero_base;
    char *hero_moby;

    limit = D_L00_001616DC;
    a0s[0] = (int)self;
    *(VecQuad *)start = *(VecQuad *)position;
    hero_base = ((char *)&hero);
    hero_moby = *(char **)(hero_base + 0x2080);
    *(VecQuad *)end = *(VecQuad *)(hero_moby + 0x10);
    position = start;
    end[2] = position[2];
    collision = collision_line((Vec4f *)end, (Vec4f *)position, 6, hero_moby, 0);
    selected_target[0] = 0;
    if (collision) {
        qcopy(position, &collision_hit_point);
        return 0;
    }
    i = 0;
    if ((candidate = active_mobies[i]) != 0) do {
        if (*(signed char *)(candidate + 0x20) < 0 || *(short *)(candidate + 0xA6) == 0x58E ||
            *(short *)(candidate + 0xA6) == 0x452)
            continue;
        qcopy(target, candidate + 0x10);
        data = (char *)fun_002141f8(candidate);
        if (!data)
            continue;
        target[2] += *(float *)(data + 0x10);
        if (!candidate || !*(char **)(candidate + 0x24) ||
            *(short *)(*(char **)(candidate + 0x24) + 0x46) != 5)
            continue;
        angle = fast_arc_tan_c(target[0] - position[0], target[1] - position[1]);
        distance = vector_distance_xy_alt(position, target);
        elevation = fast_arc_tan_c(distance, target[2] - position[2]);
        headingError = fast_difference_between_rotations_c(*heading, angle);
        distance = fast_vec_dist(position, target);
        if (distance < 2.5f) {
            float toward;
            char *close_range_hero_base = ((char *)&hero);
            moby = *(char **)(close_range_hero_base + 0x2080);
            toward = fast_arc_tan_c(*(float *)(candidate + 0x10) - *(float *)(close_range_hero_base + 0x80),
                                   *(float *)(candidate + 0x14) - *(float *)(close_range_hero_base + 0x84));
            if (fast_difference_between_rotations_c(*(float *)(moby + 0x48), toward) < 1.0471976f &&
                absolute_float(position[2] - target[2]) < 2.0f &&
                !collision_line((Vec4f *)position, (Vec4f *)target, 6, *(char **)(close_range_hero_base + 0x2080), 0)) {
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
            build_spherical_offset(offset, distance, *heading, *pitch + *(float *)(aim_offset_hero_base + 0x2E4) * 0.5f);
        }
        add_vector_xyz_c4(offset, offset, position);
        apparentAngle = fast_arc_sin(fast_vec_dist(offset, target) / (distance + distance));
        apparentAngle += apparentAngle;
        if (morphoray_target_angle_threshold_degrees_gp * 0.017453292f < apparentAngle) {
            data = (char *)fun_002141f8(candidate);
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
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/hud.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BDAF0), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef int u128_2BDAF0 __attribute__((mode(TI)));

typedef struct Vq_2BDAF0 { float x, y, z, w; } __attribute__((aligned(16))) Vq_2BDAF0;

typedef struct Ray_2BDAF0 {
    float pos[4];
    Moby *target;
    float total;
    float left;
    short timer;
    short seen;
    char p0[0x34 - 0x20];
    int snd;
    char p1[0x44 - 0x38];
    int delay;
    float yaw;
    float pitch;
} Ray_2BDAF0;

typedef struct Stats_2BDAF0 {
    char p0[0xA8];
    unsigned short kills;
    unsigned short best;
    int levels;
} Stats_2BDAF0;

typedef struct Skill_2BDAF0 {
    char p0[8];
    unsigned char f8;
} Skill_2BDAF0;

typedef struct Light_2BDAF0 {
    float col[4];
    float pos[3];
    float radius;
} Light_2BDAF0;

typedef struct Voice_2BDAF0 {
    char p0[0x74];
    unsigned char on;
    char p1[0x88 - 0x75];
    Moby *owner;
} Voice_2BDAF0;

char *FUN_L00_002bc3b0_u(char *self, float *position, float *heading, float *pitch) __asm__("FUN_L00_002bc3b0");
extern unsigned char D_00141680_r[] __asm__("D_00141680");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C;
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int D_00141680;
extern int D_0015EEA4;
extern float morphoray_base_drain_rate_gp __asm__("D_L00_001616E0") __attribute__((sda));
extern int D_L00_001616EC __attribute__((sda));
extern int D_L00_001617BC;
extern int D_L00_001617C8;
extern int fast_dec_timer_s16() __asm__("FUN_001f9770");
extern int transpose_rotation_matrix() __asm__("FUN_001fa2d8");
extern int suck_get_aim_height() __asm__("FUN_002141f8");
extern int FUN_L00_002223f8();
extern int draw_uniform_size_hud_stars() __asm__("FUN_L00_00236e50");
extern int FUN_L00_0024f7c8();
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern s32 queue_animation_update() __asm__("func_001FF308");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void add_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void tick_countdown_32_c(s32 *) __asm__("func_001F9740");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
f32 compute_interpolated_record_value_c(void *anim) __asm__("FUN_0020c9e0");
int allocate_packed_color_point_light_slot(void *v, float a, float b, int col) __asm__("FUN_L00_0023e5e0");
int voice_is_playing(int a, int i) __asm__("FUN_L00_0028d8c0");
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");
s32 remove_hud_item(s32 item) __asm__("FUN_001ff480");
struct Moby *spawn_morphoray_chicken(void *a) __asm__("FUN_L00_002c9348");
void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
void hud_init_particles(HudElem *e) __asm__("FUN_L00_00236268");
void hud_update_particles(void) __asm__("FUN_L00_00236458");
void release_point_light_slot(int i) __asm__("FUN_L00_0023e838");
void FUN_L00_00263d40(int a, int b);
extern char D_0013A5E0_2BDAF0[] __asm__("D_0013CAE0");
extern char item_unlocked[] __asm__("D_0013D408");
extern unsigned char D_0013E633_2BDAF0[] __asm__("D_0013E533");
extern char voice_pool[] __asm__("D_0013E550");
extern char D_L00_001803C0_2BDAF0[] __asm__("D_L00_001802C0");
extern char D_L00_00166C80[];
extern float func_0020D830_2BDAF0(void *) __asm__("FUN_0020c9e0");
extern int func_001F9938_2BDAF0(void *) __asm__("FUN_001f9770");
extern void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern int func_001F9850_2BDAF0(int) __asm__("FUN_001f96f8");
extern void func_L00_00250800_2BDAF0(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void func_L00_00222B80_2BDAF0(int, int) __asm__("FUN_L00_002223f8");
extern void func_001FA480_2BDAF0(void *, void *) __asm__("FUN_001fa2b8");
extern void func_001FA4A0_2BDAF0(void *, void *) __asm__("FUN_001fa2d8");
extern void func_001F9EE8_2BDAF0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void func_001F9C30_2BDAF0(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9EC0_2BDAF0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float func_L00_001FF860_2BDAF0(float, float) __asm__("FUN_001f9e90");
extern float func_001F9CE8_2BDAF0(void *) __asm__("FUN_001f9b20");
extern Moby *func_L00_002BD6B8_2BDAF0(void *, void *, void *, void *) __asm__("FUN_L00_002bc3b0");
extern int func_001F9908_2BDAF0(void *) __asm__("FUN_001f9740");
extern float *func_L00_0025D390_2BDAF0(Moby *) __asm__("FUN_002141f8");
extern float func_001FA888_2BDAF0(int) __asm__("FUN_001fa6c0");
extern int func_001FFB38_2BDAF0(int, int, void *, void *, void *, int *, int) __asm__("FUN_001ff308");
extern int func_001FA898_2BDAF0(float) __asm__("FUN_001fa6d0");
extern int func_0022EE28_2BDAF0(int, int, int) __asm__("FUN_0022db10");
extern void func_L00_00264DB8_2BDAF0(int, int) __asm__("FUN_L00_00263d40");
extern void func_L00_002CA728_2BDAF0(void *) __asm__("FUN_L00_002c9348");
extern void func_L00_002BE3A8_2BDAF0(void *) __asm__("FUN_L00_002bd0a0");
extern int func_L00_0023EF78_2BDAF0(float *pos, float radius, float intensity, int color) __asm__("FUN_L00_0023e5e0");
extern void func_L00_001FF4B0_2BDAF0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_2BDAF0(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_L00_0028EB98_2BDAF0(void *, int) __asm__("FUN_L00_0028d8c0");
extern int func_0022ED80_2BDAF0(int, int, void *) __asm__("FUN_0022da68");
extern int func_001FFCB0_2BDAF0(int) __asm__("FUN_001ff480");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");
extern void func_L00_0028EBF0_2BDAF0(int) __asm__("FUN_0022d798");
extern void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
extern void func_L00_00236BF8_2BDAF0(void) __asm__("FUN_L00_00236268");
extern void func_L00_00236DE8_2BDAF0(void) __asm__("FUN_L00_00236458");
extern void func_L00_002377E0_2BDAF0(void) __asm__("FUN_L00_00236e50");

void morphoray_update(Moby *moby) __asm__("FUN_L00_002bc7e8");
void morphoray_update(Moby *moby) {
    Vq_2BDAF0 direction;
    Vq_2BDAF0 points[3];
    Vq_2BDAF0 matrix[4];
    Ray_2BDAF0 *data;
    int busy;
    int in_range;
    Moby *hit;
    int now;

    busy = 0;
    if (((Hero *)((char *)&hero))->items[0].state == 3) {
        busy = 1;
    }
    data = (struct Ray_2BDAF0 *)moby->pvars;
    if (moby->state == 0) {
        moby->state = 1;
        morphoray_point_light_slot = -1;
        D_L00_001617C8 = -1;
        morphoray_light_fade_countdown = 0;
    }
    in_range = 1;
    if (moby->prev_seq == 0 && func_0020D830_2BDAF0(moby) < 6.0f) {
        in_range = 0;
    }
    func_001F9938_2BDAF0(&data->timer);
    if ((((unsigned int)((Hero *)((char *)&hero))->state.control_mode < 2 || ((Hero *)((char *)&hero))->state.control_mode == 12
          || ((Hero *)((char *)&hero))->state.control_mode == 4 || ((Hero *)((char *)&hero))->state.control_mode == 5))
        && (*(int *)D_0013A5E0_2BDAF0 & ((Hero *)((char *)&hero))->items[0].button_mask) != 0
        && !busy && in_range) {
        if (((Hero *)((char *)&hero))->item_anim_active == 0) {
            start_equipped_item_animation();
        }
        if (data->seen == 0) {
            data->seen = 1;
            if (((Stats_2BDAF0 *)(D_00141680_r))->kills <= 0xFFFE) {
                ((Stats_2BDAF0 *)(D_00141680_r))->kills++;
            }
            now = func_001F9850_2BDAF0(D_0015EEA4) / 600;
            if (((Stats_2BDAF0 *)(D_00141680_r))->best < now) {
                ((Stats_2BDAF0 *)(D_00141680_r))->best = func_001F9850_2BDAF0(D_0015EEA4) / 600;
            }
            ((Stats_2BDAF0 *)(D_00141680_r))->levels = ((Stats_2BDAF0 *)(D_00141680_r))->levels | (1 << current_level_index) | 0x80000000;
        }
        if (data->timer == 0) {
            data->timer = func_001F9850_2BDAF0(15);
        }
        func_L00_00250800_2BDAF0(moby, 0, data);
        if (((Hero *)((char *)&hero))->state.current == 1) {
            func_L00_00222B80_2BDAF0(30, 1);
        }
        *(u128_2BDAF0 *)&direction = 0;
        if (((Hero *)((char *)&hero))->state.current == 30
            && (*(unsigned short *)((char *)((Hero *)((char *)&hero))->moby + 0x34) & 1)) {
            func_001FA480_2BDAF0(points, D_L00_00166C80);
            func_001FA4A0_2BDAF0(matrix, points);
            direction.z = 1.0f;
            func_001F9EE8_2BDAF0(&direction, &direction, matrix);
        } else if (((Hero *)((char *)&hero))->unk22C8 == 1) {
            func_001F9C30_2BDAF0(&direction, &moby->rot_row1, -1.0f);
        } else {
            direction.x = 1.0f;
            func_001F9EC0_2BDAF0(&direction, &direction, ((Hero *)((char *)&hero))->basis[0].f);
        }
        data->yaw = func_L00_001FF860_2BDAF0(direction.x, direction.y);
        data->pitch = func_L00_001FF860_2BDAF0(func_001F9CE8_2BDAF0(&direction), direction.z);
        *(u128_2BDAF0 *)&points[0] = *(u128_2BDAF0 *)data->pos;
        hit = func_L00_002BD6B8_2BDAF0(moby, &points[0], &data->yaw, &data->pitch);
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
            float *target_health = func_L00_0025D390_2BDAF0(hit);
            if (hit != data->target && data->delay == 0) {
                data->target = hit;
                data->total = func_001FA888_2BDAF0(((short *)target_health)[2]);
                data->left = target_health[0];
                data->delay = func_001F9850_2BDAF0(30);
                if (data->total <= 1.0f) {
                    data->left = 0.0f;
                }
                data->snd = func_001FFB38_2BDAF0(4, 0x7533, func_L00_00236BF8_2BDAF0, func_L00_00236DE8_2BDAF0,
                                              func_L00_002377E0_2BDAF0, &D_L00_001616EC, 10000);
            }
        }
            {
                float level_bonus = func_001FA888_2BDAF0(D_0013E633_2BDAF0[2]) * 0.33f + 1.0f;
                float remaining = data->left - morphoray_base_drain_rate_gp * D_0015ED6C * level_bonus;
                data->left = remaining;
                if (remaining < 0.0f) {
                    data->left = 0.0f;
                }
            }
            D_L00_001616EC = 10000 - func_001FA898_2BDAF0(data->left * 10000.0f / data->total);
        }
        if (data->target != 0 && data->left <= 0.0f) {
            if (current_level_index == 5 && data->target->oclass == 0x271 && ((Skill_2BDAF0 *)item_unlocked)->f8 == 0) {
                ((Skill_2BDAF0 *)item_unlocked)->f8 = 1;
                func_0022EE28_2BDAF0(1, 0, 0);
                func_L00_00264DB8_2BDAF0(0x53D6, -1);
            }
            func_L00_002CA728_2BDAF0(data->target);
            data->target = 0;
        }
        func_L00_002BE3A8_2BDAF0(moby);
        if (morphoray_point_light_slot == -1) {
            morphoray_point_light_slot = func_L00_0023EF78_2BDAF0((float *)&moby->pos, 7.5f, 0.0f, 0x207F7F);
        }
        if (morphoray_point_light_slot >= 0) {
            morphoray_light_fade_countdown = func_001F9850_2BDAF0(20);
            func_L00_001FF4B0_2BDAF0(&points[0], &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[1], data, &points[0]);
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].pos[0] = points[1].x;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].pos[1] = points[1].y;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].pos[2] = points[1].z;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].radius = 7.5f;
        }
        if (func_L00_0028EB98_2BDAF0(moby, D_L00_001617C8) == 0) {
            D_L00_001617C8 = func_0022ED80_2BDAF0(0, 4, moby);
        }
    } else {
        data->target = 0;
        data->seen = 0;
        data->delay = 0;
        if (data->snd != -1) {
            func_001FFCB0_2BDAF0(data->snd);
            data->snd = -1;
        }
        D_L00_001617BC = 1;
        if (morphoray_point_light_slot >= 0) {
            float brightness;
            func_L00_001FF4B0_2BDAF0(&direction, &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[0], data, &direction);
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].pos[0] = points[0].x;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].pos[1] = points[0].y;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].pos[2] = points[0].z;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].radius = 7.5f;
            brightness = func_001FA888_2BDAF0(morphoray_light_fade_countdown) * 0.05f;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].col[0] = brightness;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].col[1] = brightness;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[morphoray_point_light_slot].col[2] = brightness * 0.25f;
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
            if (D_L00_001617C8 != -1 && ((Voice_2BDAF0 *)(voice_pool + D_L00_001617C8 * 0x70))->owner == moby
                && ((Voice_2BDAF0 *)(voice_pool + D_L00_001617C8 * 0x70))->on != 0) {
                func_L00_0028EBF0_2BDAF0(D_L00_001617C8);
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
extern f32 D_L00_001616DC_gp __asm__("D_L00_001616DC") __attribute__((sda));
extern f32 D_L00_001617CC_macro __asm__("D_L00_001617CC") __attribute__((section(".sdata")));
extern s32 D_L00_00161718_i __asm__("D_L00_00161718") __attribute__((sda));
extern s32 D_L00_00161720_i __asm__("D_L00_00161720") __attribute__((sda));
extern s32 D_L00_00161724_i __asm__("D_L00_00161724") __attribute__((sda));
extern float D_L00_001617D0 __attribute__((section(".sdata")));
extern s32 D_L00_00161824;
extern s32 D_L00_00161818;
extern s32 D_L00_001DBF50 __attribute__((section(".data")));
extern u128 D_L00_001DBF60_2bcf98 __asm__("D_L00_001DBF60") __attribute__((section(".data")));
extern s32 D_L00_001DD0D0 __attribute__((section(".data")));
extern void scale_vector_2bcf98(float, void *, void *) __asm__("FUN_001f9a68");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");

void reset_beam_chain_and_pulse_state(u8 *moby) __asm__("FUN_L00_002bcf98");

void reset_beam_chain_and_pulse_state(u8 *moby) {
    u128 step;
    u128 *source = *(u128 **)(moby + 0x78);
    s32 i;
    s32 j;
    f32 scale;

    if (D_L00_001617BC != 0) {
        D_L00_001617BC = 0;
        scale = D_L00_001616DC_gp / convert_integer_to_float(12);
        D_L00_001617CC_macro = scale;
        scale_vector_2bcf98(-scale, &step, moby + 0xD0);
        qcopy(&D_L00_001DBF60_2bcf98, source);
        for (i = 1; i < 12; i++) {
            fast_vec_add(&D_L00_001DBF60_2bcf98 + i, (u128 *)&D_L00_001DBF50 + i, &step);
        }
        D_L00_00161718_i = 0;
        D_L00_00161720_i = 0;
        D_L00_00161724_i = 0;
        D_L00_001617D0 = 0.0f;
        D_L00_00161824 = 0;
        for (j = 0; j < 3; j++) {
            (&D_L00_00161818)[j] = 0;
            qcopy((u128 *)&D_L00_001DD0D0 + j, &step);
        }
    }
}
/* Tesla claw arc: bends the twelve-point beam toward the target, builds the two side rails and the ring of
 * twenty points around each node, queues the draw callback, sparks at the tip, and runs the three travelling
 * pulses along the beam. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BE3A8), where it is exact; names translated to the US level program. */

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Vec_2BE3A8;

typedef struct {
    Vec_2BE3A8 pos;
    struct Moby *target;
    char pad14[0x34];
    float f48;
    float f4C;
} Vars_2BE3A8;

typedef struct {
    char pad0[0x10];
    float height;
} Info_2BE3A8;

typedef struct { int c[5]; } Tab_2BE3A8;

typedef struct { float f[4]; } __attribute__((aligned(16))) V_t;

extern unsigned char D_0013F5E0_r[] __asm__("D_0013F5E0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz_c(void *) __asm__("FUN_001f9af0");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float D_L00_001616DC_gp __asm__("D_L00_001616DC") __attribute__((sda));
extern signed char D_L00_00161763;
extern float D_L00_001617CC;
extern int D_L00_001617E8;
extern int D_L00_00161808;
extern int D_L00_00161818;
extern int D_L00_00161824;
extern int D_L00_001DBF50;
extern int D_L00_001DBF70;
extern int D_L00_001DD0A0;
extern int D_L00_001DD0D0;
extern int FUN_001efa68_c_c(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern int lerp_vector() __asm__("FUN_001f9a40");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern void add_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_look_at_matrix_c(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void cross_vectors_xyz_c(void *, void *, void *) __asm__("func_001F9AD8");
extern void scale_vector_xyz_c3(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
unsigned char *FUN_L00_00272060(void *pos, int s, float x, float y, int color, int mode, int b, float *v, float z);
unsigned char *spawn_proximity_corrected_size_alpha_tween_particle(void *pos, int s, int col, int mode, int b, float *vec, int h, float x, float y) __asm__("FUN_L00_00275910");
void reset_beam_chain_and_pulse_state(u8 *moby) __asm__("FUN_L00_002bcf98");
void draw_beam_ring_and_rail_effects(void *o) __asm__("FUN_L00_002bdc50");
extern float D_L00_001616DC_gp __asm__("D_L00_001616DC") __attribute__((sda));
extern float D_L00_001617C0_2BE3A8 __asm__("D_L00_001616F0") __attribute__((sda));
extern int D_L00_001617C4_2BE3A8 __asm__("D_L00_001616F4") __attribute__((sda));
extern float D_L00_001617C8_2BE3A8 __asm__("D_L00_001616F8") __attribute__((sda));
extern float D_L00_001617CC_2BE3A8 __asm__("D_L00_001616FC") __attribute__((sda));
extern float D_L00_001617D4_2BE3A8 __asm__("D_L00_00161704") __attribute__((sda));
extern float D_L00_001617D8_2BE3A8 __asm__("D_L00_00161708") __attribute__((sda));
extern float D_L00_001617DC_2BE3A8 __asm__("D_L00_0016170C") __attribute__((sda));
extern float D_L00_001617FC_2BE3A8 __asm__("D_L00_0016172C") __attribute__((sda));
extern float D_L00_00161800_2BE3A8 __asm__("D_L00_00161730") __attribute__((sda));
extern float D_L00_00161804_2BE3A8 __asm__("D_L00_00161734") __attribute__((sda));
extern float D_L00_0016180C_2BE3A8 __asm__("D_L00_0016173C") __attribute__((sda));
extern float D_L00_00161810_2BE3A8 __asm__("D_L00_00161740") __attribute__((sda));
extern float D_L00_00161818_2BE3A8 __asm__("D_L00_00161748") __attribute__((sda));
extern int D_L00_0016181C_2BE3A8 __asm__("D_L00_0016174C") __attribute__((sda));
extern int D_L00_00161820_2BE3A8 __asm__("D_L00_00161750") __attribute__((sda));
extern float D_L00_00161828_2BE3A8 __asm__("D_L00_00161758") __attribute__((sda));
extern float D_L00_0016182C_2BE3A8 __asm__("D_L00_0016175C") __attribute__((sda));
extern float D_L00_00161838_2BE3A8 __asm__("D_L00_00161768") __attribute__((sda));
extern signed char D_L00_00161833_2BE3A8[] __asm__("D_L00_00161763") __attribute__((section(".sdata")));
extern float D_L00_001617CC_macro __asm__("D_L00_001617CC") __attribute__((section(".sdata")));
extern int D_L00_00161824_macro __asm__("D_L00_00161824") __attribute__((section(".sdata")));
extern float frame_scale __asm__("D_0015ED60") __attribute__((section(".sdata")));
extern float D_0015ED6C_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern int D_L00_001618A8_2BE3A8[3] __asm__("D_L00_001617D8") __attribute__((section(".sdata")));
extern float D_L00_001618B8_2BE3A8[3] __asm__("D_L00_001617E8") __attribute__((section(".sdata")));
extern float D_L00_001618C8_2BE3A8[3] __asm__("D_L00_001617F8") __attribute__((section(".sdata")));
extern float D_L00_001618D8_2BE3A8[3] __asm__("D_L00_00161808") __attribute__((section(".sdata")));
extern int D_L00_001618E8_2BE3A8[3] __asm__("D_L00_00161818") __attribute__((section(".sdata")));
extern Vec_2BE3A8 D_L00_001DC250_2BE3A8[12] __asm__("D_L00_001DBF50");
extern Vec_2BE3A8 D_L00_001DC260_2BE3A8[12] __asm__("D_L00_001DBF60");
extern Vec_2BE3A8 D_L00_001DC320_2BE3A8[12] __asm__("D_L00_001DC020");
extern Vec_2BE3A8 D_L00_001DC3E0_2BE3A8[12] __asm__("D_L00_001DC0E0");
Vec_2BE3A8 D_L00_001DC1A0_2BE3A8[12][20] __asm__("D_L00_001DC1A0") = {0};
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
extern Vec_2BE3A8 D_L00_001DD3A0_2BE3A8[3] __asm__("D_L00_001DD0A0");
extern Vec_2BE3A8 D_L00_001DD3D0_2BE3A8[3] __asm__("D_L00_001DD0D0");
extern Vec_2BE3A8 D_L00_00173F60_2BE3A8 __asm__("D_L00_00173E60");
extern char D_L00_0015F660_2BE3A8[] __asm__("D_L00_0015F580") __attribute__((section(".sdata")));
#include "rnc/data_labels.h"
extern void func_L00_002BE2A0_2BE3A8(void) __asm__("FUN_L00_002bcf98");
extern float func_L00_0025F368_2BE3A8(float) __asm__("FUN_L00_0025e310");
extern void func_00215C00_2BE3A8(void *, float, float, float) __asm__("FUN_00214db0");
extern Info_2BE3A8 *func_L00_0025D390_2BE3A8(struct Moby *) __asm__("FUN_002141f8");
extern void func_001F9BD8_2BE3A8(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_L00_001EFFF0_2BE3A8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern float func_001F9D48_2BE3A8(void *, void *) __asm__("FUN_001f9b80");
extern void func_001F9BF0_2BE3A8(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_001FA888_2BE3A8(int) __asm__("FUN_001fa6c0");
extern void func_L00_001FF4B0_2BE3A8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9C08_2BE3A8(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float func_001F9CB8_2BE3A8(void *) __asm__("FUN_001f9af0");
extern void func_001F9C30_2BE3A8(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9CA0_2BE3A8(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void func_002156E0_2BE3A8(void *, void *, void *, float) __asm__("FUN_00214890");
extern float func_001F9FA8_2BE3A8(float) __asm__("FUN_001f9de0");
extern float func_001F9F90_2BE3A8(float) __asm__("FUN_001f9dc8");
extern void func_001F9EE8_2BE3A8(void *, void *, void *) __asm__("FUN_001f9d20");
extern void func_001F49B0_2BE3A8(void *, struct Moby *) __asm__("FUN_001f4600");
extern void draw_beam_ring_and_rail_effects(void *) __asm__("FUN_L00_002bdc50");
extern float func_002140F8_2BE3A8(float, float) __asm__("FUN_002132a8");
extern int func_002140B0_2BE3A8(int) __asm__("FUN_00213260");
extern int func_001F9850_2BE3A8(int) __asm__("FUN_001f96f8");
extern void func_L00_00272F00_2BE3A8(void *, int, int, int, int, void *, float, float, float) __asm__("FUN_L00_00272060");
extern float func_L00_00258C80_2BE3A8(float, float) __asm__("FUN_L00_00257c48");
extern int func_001FA898_2BE3A8(float) __asm__("FUN_001fa6d0");
extern void func_L00_002767B0_2BE3A8(void *, int, int, int, int, void *, struct Moby *, float, float) __asm__("FUN_L00_00275910");
extern int func_001F9908_2BE3A8(int *) __asm__("FUN_001f9740");

#include "rnc/overlay/shared/gameplay/entities/walloper_data/update_beam_ring_rail_and_pulse_effects.h"
extern float D_L00_001617D0 __attribute__((section(".sdata")));
extern Tab_2BE3A8 D_L00_001E9AB0;
void update_beam_ring_rail_and_pulse_effects(struct Moby *moby) __asm__("FUN_L00_002bd0a0");

void update_beam_ring_rail_and_pulse_effects(struct Moby *moby) {
    Vec_2BE3A8 aim;
    Vec_2BE3A8 segment;
    Vec_2BE3A8 wave_offset;
    Vec_2BE3A8 beam_end;
    Vec_2BE3A8 point;
    Vec_2BE3A8 beam_segment_direction;
    Vec_2BE3A8 side;
    Vars_2BE3A8 *data;
    Info_2BE3A8 *info;
    int ring_index;
    int slot;
    int pulse;
    int sign;
    int node_index;
    int color;
    float beam_length;
    float progress;
    float factor;
    float wave_width;
    float spark_size;

    data = (Vars_2BE3A8 *)moby->pvars;
    wave_offset.w = 1.0f;
    wave_offset.x = 0.0f;
    func_L00_002BE2A0_2BE3A8();
    D_L00_001617D0 = func_L00_0025F368_2BE3A8(D_L00_001617D0 + D_L00_001617C8_2BE3A8);
    func_00215C00_2BE3A8(&aim, D_L00_001617CC_macro, data->f48, data->f4C);
    if (data->target != 0 && (info = func_L00_0025D390_2BE3A8(data->target)) != 0) {
        qcopy(&beam_end, &data->target->pos);
        beam_end.z = beam_end.z + info->height;
    } else {
        func_00215C00_2BE3A8(&beam_end, D_L00_001616DC_gp, data->f48, data->f4C);
        func_001F9BD8_2BE3A8(&beam_end, &beam_end, data);
    }
    beam_length = D_L00_001616DC_gp;
    if (data->target == 0) {
        if (func_L00_001EFFF0_2BE3A8(data, &beam_end, 20, *(void **)((char *)&D_001413D0), 0) != 0) {
            beam_length = func_001F9D48_2BE3A8(&D_L00_00173F60_2BE3A8, data);
        }
    } else {
        beam_length = func_001F9D48_2BE3A8(&data->target->pos, data);
    }
    {
        int i;

        D_L00_001617CC_macro = beam_length * D_L00_001617C0_2BE3A8;
        qcopy(&D_L00_001DC260_2BE3A8[0], data);
        qcopy(&D_L00_001DC320_2BE3A8[0], data);
        qcopy(&D_L00_001DC3E0_2BE3A8[0], data);
        for (i = 1; i < 12; i++) {
            float seg_length;

            func_001F9BF0_2BE3A8(&segment, &D_L00_001DC260_2BE3A8[i], &D_L00_001DC260_2BE3A8[i - 1]);
            progress = func_001FA888_2BE3A8(i) * D_L00_001617C0_2BE3A8;
            factor = progress * -0.49999997f + 0.9f;
            if (D_L00_001617C4_2BE3A8 < i && data->target != 0) {
                func_001F9BF0_2BE3A8(&point, &data->target->pos, &D_L00_001DC260_2BE3A8[i]);
                func_L00_001FF4B0_2BE3A8(&point, &point, D_L00_001617CC_macro);
                func_001F9C08_2BE3A8(&aim, &aim, &point, 0.2f);
            }
            func_001F9C08_2BE3A8(&point, &segment, &aim, factor);
            seg_length = func_001F9CB8_2BE3A8(&point);
            if (seg_length == 0.0f) {
                qcopy(&D_L00_001DC260_2BE3A8[i], &D_L00_001DC260_2BE3A8[i - 1]);
            } else {
                func_001F9C30_2BE3A8(&point, &point, D_L00_001617CC_macro / seg_length);
                func_001F9BD8_2BE3A8(&D_L00_001DC260_2BE3A8[i], &D_L00_001DC260_2BE3A8[i - 1], &point);
            }
            func_001F9BF0_2BE3A8(&beam_segment_direction, &D_L00_001DC260_2BE3A8[i], &D_L00_001DC260_2BE3A8[i - 1]);
            func_001F9CA0_2BE3A8(&side, &beam_segment_direction, D_0013F5E0_r);
            func_L00_001FF4B0_2BE3A8(&side, &side, D_L00_001617D8_2BE3A8 + (D_L00_001617DC_2BE3A8 - D_L00_001617D8_2BE3A8) * progress);
            {
                Vec_2BE3A8 ring;
                Vec_2BE3A8 fwd;
                Vec_2BE3A8 out;
                Vec_2BE3A8 up;
                Vec_2BE3A8 node;

                for (ring_index = 0; ring_index < 20; ring_index++) {
                    func_002156E0_2BE3A8(&ring, &side, &beam_segment_direction, (float)ring_index * D_L00_001617D4_2BE3A8);
                    func_001F9BD8_2BE3A8(&D_L00_001DC1A0_2BE3A8[i][ring_index], &ring, &D_L00_001DC260_2BE3A8[i]);
                }
                qcopy(&node, &D_L00_001DC260_2BE3A8[i]);
                func_L00_001FF4B0_2BE3A8(&fwd, &beam_segment_direction, 1.0f);
                func_L00_001FF4B0_2BE3A8(&out, &side, 1.0f);
                func_001F9CA0_2BE3A8(&up, &out, &fwd);
                wave_width = D_L00_001617FC_2BE3A8;
                wave_width = wave_width + (D_L00_00161800_2BE3A8 - wave_width) * progress;
                factor = func_L00_0025F368_2BE3A8(D_L00_001617D0 + D_L00_001617CC_2BE3A8 * func_001FA888_2BE3A8(i));
                wave_offset.y = wave_width * func_001F9FA8_2BE3A8(factor);
                wave_width = wave_width * func_001F9F90_2BE3A8(factor);
                wave_offset.z = wave_width;
                func_001F9EE8_2BE3A8(&D_L00_001DC320_2BE3A8[i], &wave_offset, &fwd);
                wave_offset.z = -wave_offset.z;
                func_001F9EE8_2BE3A8(&D_L00_001DC3E0_2BE3A8[i], &wave_offset, &fwd);
            }
        }
    }
    func_001F49B0_2BE3A8(draw_beam_ring_and_rail_effects, moby);
    spark_size = func_002140F8_2BE3A8(0.1f, 2.0f);
    sign = func_002140B0_2BE3A8(2);
    if (sign == 0) {
        sign = sign - 1;
    }
    if (func_002140B0_2BE3A8(2) == 0) {
        qcopy(&point, &D_L00_001DC320_2BE3A8[11]);
        func_L00_00272F00_2BE3A8(&point, func_001F9850_2BE3A8(30), 0x7F7F2020, 0, sign, D_L00_0015F660_2BE3A8, spark_size * 0.1f, spark_size, 0.003f);
        sign = -sign;
        func_L00_00272F00_2BE3A8(&point, func_001F9850_2BE3A8(30), 0x7F7F7F7F, 1, sign, D_L00_0015F660_2BE3A8, spark_size * 0.07f, spark_size * 0.7f, 0.003f);
    }
    if (func_002140B0_2BE3A8(2) == 0) {
        qcopy(&point, &D_L00_001DC3E0_2BE3A8[11]);
        func_L00_00272F00_2BE3A8(&point, func_001F9850_2BE3A8(30), 0x7F20207F, 0, sign, D_L00_0015F660_2BE3A8, spark_size * 0.1f, spark_size, 0.003f);
        sign = -sign;
        func_L00_00272F00_2BE3A8(&point, func_001F9850_2BE3A8(30), 0x7F7F7F7F, 1, sign, D_L00_0015F660_2BE3A8, spark_size * 0.07f, spark_size * 0.7f, 0.003f);
    }
    func_001F9BF0_2BE3A8(&beam_segment_direction, &D_L00_001DC260_2BE3A8[1], &D_L00_001DC260_2BE3A8[0]);
    func_L00_001FF4B0_2BE3A8(&beam_segment_direction, &beam_segment_direction, D_L00_00161804_2BE3A8 * frame_scale);
    if (func_002140B0_2BE3A8(2) == 0) {
        func_L00_001FF4B0_2BE3A8(&side, &moby->rot_row1, D_L00_00161810_2BE3A8);
        func_001F9BD8_2BE3A8(&point, &D_L00_001DC260_2BE3A8[0], &side);
        {
            Tab_2BE3A8 colors;
            Vec_2BE3A8 out;
            Vec_2BE3A8 up;

            colors = D_L00_001E9AB0;
            color = colors.c[func_002140B0_2BE3A8(5)];
            wave_width = func_L00_00258C80_2BE3A8(0.0f, D_L00_0016180C_2BE3A8);
            func_002156E0_2BE3A8(&out, &beam_segment_direction, D_0013F5E0_r, func_L00_00258C80_2BE3A8(0.0f, D_L00_0016180C_2BE3A8));
            func_001F9CA0_2BE3A8(&up, &beam_segment_direction, D_0013F5E0_r);
            func_002156E0_2BE3A8(&out, &out, &up, wave_width);
            node_index = func_001FA898_2BE3A8(beam_length * 1.2f / (D_L00_00161804_2BE3A8 * frame_scale));
            if (data->target != 0) {
                node_index = node_index * 3;
            }
            func_L00_002767B0_2BE3A8(&point, node_index, color, 0, sign, &out, data->target, spark_size * 0.1f, spark_size);
            func_L00_002767B0_2BE3A8(&point, node_index, 0x7F7F7F7F, 1, -sign, &out, data->target, spark_size * 0.07f, spark_size * 0.7f);
        }
    }
    if (func_001F9908_2BE3A8(&D_L00_00161824_macro) != 0) {
        D_L00_00161824_macro = func_001F9850_2BE3A8(beam_pulse_spawn_interval_frames_gp);
        for (slot = 0; slot < 3; slot++) {
            if (D_L00_001618E8_2BE3A8[slot] == 0) {
                D_L00_001618E8_2BE3A8[slot] = 1;
                D_L00_001618A8_2BE3A8[slot] = func_001F9850_2BE3A8(D_L00_0016181C_2BE3A8);
                D_L00_001618C8_2BE3A8[slot] = 0.0f;
                D_L00_001618D8_2BE3A8[slot] = D_L00_0016182C_2BE3A8;
                D_L00_001618B8_2BE3A8[slot] = func_001FA888_2BE3A8(D_L00_00161833_2BE3A8[0]);
                qcopy(&D_L00_001DD3A0_2BE3A8[slot], data);
                func_001F9BF0_2BE3A8(&side, &D_L00_001DC260_2BE3A8[1], &D_L00_001DC260_2BE3A8[0]);
                func_L00_001FF4B0_2BE3A8(&D_L00_001DD3D0_2BE3A8[slot], &side, 1.0f);
                break;
            }
        }
    }
    for (pulse = 0; pulse < 3; pulse++) {
        Vec_2BE3A8 step;
        float dt;
        float travelled;
        float max_length;
        float ratio;
        float remainder;
        float inv_length;

        if (D_L00_001618E8_2BE3A8[pulse] == 0) {
            continue;
        }
        if (func_001F9908_2BE3A8(&D_L00_001618A8_2BE3A8[pulse]) != 0) {
            D_L00_001618E8_2BE3A8[pulse] = 0;
            continue;
        }
        if (D_L00_001618A8_2BE3A8[pulse] < D_L00_00161820_2BE3A8) {
            D_L00_001618B8_2BE3A8[pulse] = D_L00_001618B8_2BE3A8[pulse] - D_L00_00161838_2BE3A8;
        }
        dt = D_L00_00161818_2BE3A8 * D_0015ED6C_gp;
        travelled = D_L00_001618C8_2BE3A8[pulse] + dt;
        max_length = D_L00_001616DC_gp;
        ratio = travelled / max_length;
        D_L00_001618C8_2BE3A8[pulse] = travelled;
        if (1.0f < ratio) {
            ratio = 1.0f;
        }
        D_L00_001618D8_2BE3A8[pulse] = D_L00_0016182C_2BE3A8 + (D_L00_00161828_2BE3A8 - D_L00_0016182C_2BE3A8) * ratio;
        if (max_length <= travelled) {
            func_001F9C30_2BE3A8(&side, &D_L00_001DD3D0_2BE3A8[pulse], dt);
            func_001F9BD8_2BE3A8(&D_L00_001DD3A0_2BE3A8[pulse], &D_L00_001DD3A0_2BE3A8[pulse], &side);
            continue;
        }
        node_index = func_001FA898_2BE3A8(travelled / D_L00_001617CC_macro);
        remainder = D_L00_001618C8_2BE3A8[pulse] - func_001FA888_2BE3A8(node_index) * D_L00_001617CC_macro;
        qcopy(&D_L00_001DD3A0_2BE3A8[pulse], &D_L00_001DC260_2BE3A8[node_index]);
        if (node_index == 11) {
            func_001F9BF0_2BE3A8(&side, &D_L00_001DC260_2BE3A8[node_index], &D_L00_001DC250_2BE3A8[node_index]);
        } else {
            func_001F9BF0_2BE3A8(&side, &D_L00_001DC260_2BE3A8[node_index + 1], &D_L00_001DC260_2BE3A8[node_index]);
        }
        inv_length = func_001F9CB8_2BE3A8(&side);
        if (inv_length == 0.0f) {
            continue;
        }
        inv_length = 1.0f / inv_length;
        func_001F9C30_2BE3A8(&step, &side, remainder * inv_length);
        func_001F9BD8_2BE3A8(&D_L00_001DD3A0_2BE3A8[pulse], &D_L00_001DD3A0_2BE3A8[pulse], &step);
        func_L00_001FF4B0_2BE3A8(&step, &side, inv_length);
        func_001F9C08_2BE3A8(&D_L00_001DD3D0_2BE3A8[pulse], &D_L00_001DD3D0_2BE3A8[pulse], &step, 0.1f);
        func_L00_001FF4B0_2BE3A8(&D_L00_001DD3D0_2BE3A8[pulse], &D_L00_001DD3D0_2BE3A8[pulse], 1.0f);
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
typedef struct {
    OvlQuad corner[4];
    int color[4];
    float uv[8];
    long unk70, tex, unk80, unk88;
} Quad;
extern u8 D_0013E535 __attribute__((section(".data")));
#include "rnc/overlay/shared/gameplay/entities/walloper_data/draw_beam_ring_and_rail_effects_4.h"
extern float D_L00_00161718 __attribute__((sda));
#include "rnc/overlay/shared/gameplay/entities/walloper_data/draw_beam_ring_and_rail_effects_3.h"
extern float D_L00_00161720 __attribute__((sda));
extern float D_L00_00161724 __attribute__((sda));
#include "rnc/overlay/shared/gameplay/entities/walloper_data/draw_beam_ring_and_rail_effects_2.h"
extern int D_L00_00161798 __attribute__((sda));
#include "rnc/overlay/shared/gameplay/entities/walloper_data/draw_beam_ring_and_rail_effects.h"
extern float g_debug_camera[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern OvlVec4 D_L00_001DC020[] __attribute__((section(".data")));
extern OvlVec4 D_L00_001DC0E0[] __attribute__((section(".data")));
extern OvlQuad D_L00_001DC060[][20] __attribute__((section(".data")));
extern OvlQuad D_L00_001DC1A0[][20] __attribute__((section(".data")));
void draw_scrolling_ribbon_colored(void *, int, int, float, float, float, int, int) __asm__("FUN_L00_002be078");
void draw_beam_pulse_quads(void *unused) __asm__("FUN_L00_002be650");

void draw_beam_pulse_quads(void *) __asm__("FUN_L00_002be650");
void draw_beam_origin_camera_facing_quads(void *mv) __asm__("FUN_L00_002be890");

void draw_beam_origin_camera_facing_quads(void *) __asm__("FUN_L00_002be890");
long FUN_001f44b8(int);
void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
void fast_draw_quad_real(void *, void *, int) __asm__("FUN_001f7d30");
/* scrolls the two texture-offset accumulators and draws the 10x20 band of
   textured quads between the D_L00_001DC060/001DC1A0 rings, skipping
   back-facing ones */
void draw_beam_ring_and_rail_effects(void *moby) {
    Quad quad;
    OvlVec4 edge_to_corner0, edge_to_corner2, face_normal, camera_to_corner1;
    float y, first_pass_v, second_pass_v, texture_scroll_step;
    int c, i, j, k, off, jo;

    beam_rail_first_pass_texture_u_offset_gp = beam_rail_first_pass_texture_u_offset_gp - beam_rail_first_pass_texture_u_scroll_speed_gp * frame_scale;
    if (beam_rail_first_pass_texture_u_offset_gp <= -8.0f) beam_rail_first_pass_texture_u_offset_gp += 8.0f;
    draw_scrolling_ribbon_colored(D_L00_001DC020, beam_rail_first_pass_alpha_gp, beam_rail_second_pass_alpha_gp, beam_rail_first_pass_half_width_gp, beam_rail_second_pass_half_width_gp, beam_rail_second_pass_segment_extension_factor_gp, 12, beam_first_rail_second_pass_base_vertex_color_gp);
    draw_scrolling_ribbon_colored(D_L00_001DC0E0, beam_rail_first_pass_alpha_gp, beam_rail_second_pass_alpha_gp, beam_rail_first_pass_half_width_gp, beam_rail_second_pass_half_width_gp, beam_rail_second_pass_segment_extension_factor_gp, 12, beam_second_rail_second_pass_base_vertex_color_gp);
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
    if (D_L00_00161718 <= -8.0f) D_L00_00161718 += 8.0f;
    texture_scroll_step = beam_ring_texture_v_scroll_speed_gp * frame_scale;
    D_L00_00161720 = D_L00_00161720 - texture_scroll_step;
    if (D_L00_00161720 <= -7.0f) D_L00_00161720 += 7.0f;
    D_L00_00161724 = D_L00_00161724 + texture_scroll_step;
    if (D_L00_00161724 >= 5.0f) D_L00_00161724 -= 5.0f;
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
                fast_draw_quad_real(&quad, 0, 0);
                quad.uv[0] = y;
                quad.uv[1] = second_pass_v;
                quad.uv[2] = y;
                quad.uv[3] = second_pass_v + beam_ring_face_texture_v_step_gp;
                quad.uv[5] = second_pass_v;
                quad.uv[4] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[6] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[7] = second_pass_v + beam_ring_face_texture_v_step_gp;
                second_pass_v = second_pass_v + beam_ring_face_texture_v_step_gp;
                fast_draw_quad_real(&quad, 0, 0);
            }
        }
        y += beam_ring_band_texture_u_step_gp;
    }
}
extern int D_L00_00161780 __attribute__((sda));
extern int D_L00_001617A4 __attribute__((sda));
extern int D_L00_001617A8 __attribute__((sda));
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF380). */
void draw_scrolling_ribbon_colored(void *mv, int a, int b, float sx, float sy, float sz, int n, int col) {
    RW_2baae0 *m = mv;
    RW_2baae0 q[4];
    RT_2baae0 t;
    RW_2baae0 q2[4];
    RT_2baae0 u;
    RW_2baae0 w120, w130, w140, w150, r160, r170, a180, b190, s1A0, s1B0;
    int i, col0, col1;
    float len, len0, len2, l0, l1;
    t.q0 = tex_2baae0(D_L00_001617A4);
    col0 = D_L00_00161780 | (a << 24);
    t.q2 = 0x8000000048L;
    t.q1 = 0xFF9000000260L;
    t.z4 = 0;
    t.c[3] = col0;
    t.c[2] = col0;
    t.c[1] = col0;
    t.c[0] = col0;
    u.q0 = tex_2baae0(D_L00_001617A8);
    col1 = col | (b << 24);
    u.q2 = t.q2;
    u.q1 = t.q1;
    u.z4 = t.z4;
    u.c[3] = col1;
    u.c[2] = col1;
    u.c[1] = col1;
    u.c[0] = col1;
    u.f[0] = 0; u.f[1] = 0; u.f[2] = 0; u.f[3] = 1.0f; u.f[4] = 1.0f; u.f[5] = 0; u.f[6] = 1.0f; u.f[7] = 1.0f;
    t.f[0] = u.f[0] + beam_rail_first_pass_texture_u_offset_gp;
    t.f[1] = u.f[1];
    t.f[2] = u.f[2] + beam_rail_first_pass_texture_u_offset_gp;
    t.f[3] = u.f[3];
    t.f[4] = u.f[4] + beam_rail_first_pass_texture_u_offset_gp;
    t.f[5] = u.f[5];
    t.f[6] = u.f[6] + beam_rail_first_pass_texture_u_offset_gp;
    t.f[7] = u.f[7];
    fast_vec_sub(w140, m, g_debug_camera);
    fast_vec_sub(w150, &m[1], m);
    fast_vec_cross(w120, w150, w140);
    len0 = fast_vec_length(w120);
    if (len0 != 0) len0 = 1.0f / len0;
    fast_vec_scale(beam_rail_first_pass_half_width_gp * len0, w120, w120);
    fast_vec_add(q[0], m, w120);
    fast_vec_sub(q[1], m, w120);
    for (i = 1; i < n - 1; i++) {
        fast_vec_sub(w140, &m[i], g_debug_camera);
        fast_vec_sub(w150, &m[i + 1], &m[i]);
        fast_vec_cross(w120, w150, w140);
        len = fast_vec_length(w120);
        if (len != 0) len = 1.0f / len;
        fast_vec_scale(beam_rail_first_pass_half_width_gp * len, w120, w120);
        fast_vec_add(q[2], &m[i], w120);
        fast_vec_sub(q[3], &m[i], w120);
        if (i == 1) {
            t.c[1] = D_L00_00161780;
            t.c[0] = D_L00_00161780;
        } else if (i == n - 2) {
            t.c[3] = D_L00_00161780;
            t.c[2] = D_L00_00161780;
        } else if (i == 2) {
            t.c[1] = col0;
            t.c[0] = col0;
        }
        draw_geometry_quad(q, 0, 0);
        qcopy(q[0], q[2]);
        qcopy(q[1], q[3]);
    }
    for (i = 0; i < n - 1; i++) {
        fast_vec_sub(a180, &m[i], g_debug_camera);
        fast_vec_sub(b190, &m[i + 1], g_debug_camera);
        l0 = fast_vec_length(a180);
        l1 = fast_vec_length(b190);
        if (l1 < l0) {
            if (l1 == 0) qcopy(b190, a180);
            else fast_vec_scale(l0 / l1, b190, b190);
        } else {
            if (l0 == 0) qcopy(a180, b190);
            else fast_vec_scale(l1 / l0, a180, a180);
        }
        fast_vec_add(r160, g_debug_camera, a180);
        fast_vec_add(r170, g_debug_camera, b190);
        fast_vec_sub(w150, r170, r160);
        fast_vec_scale(sz, s1B0, w150);
        fast_vec_sub(r160, r160, s1B0);
        fast_vec_add(r170, r170, s1B0);
        fast_vec_sub(s1A0, g_debug_camera, r160);
        fast_vec_cross(w120, s1A0, w150);
        len2 = fast_vec_length(w120);
        if (len2 != 0) len2 = 1.0f / len2;
        fast_vec_scale(sy * len2, w130, w120);
        fast_vec_add(q2[0], r160, w130);
        fast_vec_sub(q2[1], r160, w130);
        fast_vec_add(q2[2], r170, w130);
        fast_vec_sub(q2[3], r170, w130);
        if (i == 0) {
            u.c[1] = col;
            u.c[0] = col;
        } else if (i == n - 2) {
            u.c[3] = col;
            u.c[2] = col;
        } else if (i == 1) {
            u.c[1] = col1;
            u.c[0] = col1;
        }
        draw_geometry_quad(q2, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF958). */
#include "rnc/rendering/geometry_quad.h"
extern int beam_pulse_default_rgb_gp __asm__("D_L00_00161760") __attribute__((sda));
extern int D_L00_00161764 __attribute__((sda));
extern float D_L00_001617E8_macro[3] __asm__("D_L00_001617E8") __attribute__((section(".sdata")));
extern float D_L00_00161808_macro[3] __asm__("D_L00_00161808") __attribute__((section(".sdata")));
extern int D_L00_00161818_macro[3] __asm__("D_L00_00161818") __attribute__((section(".sdata")));
extern OvlVec4 D_L00_001DBF20[] __attribute__((section(".data")));

void draw_beam_pulse_quads(void *unused) {
    struct GeometryQuad quad;
    Vec4 basis[3], position, corner;
    int color;
    Vec4 *origin;
    int i;
    int j;

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
            qcopy(origin, &D_L00_001DD3A0_2BE3A8[i]);
            qcopy(&basis[0], &D_L00_001DD3D0_2BE3A8[i]);
            fast_vec_scale(-1.0f, &basis[2], D_0013F5E0);
            fast_vec_cross(&basis[1], &basis[0], &basis[2]);
            fast_vec_normalize(&basis[1], &basis[1], 1.0f);
            fast_vec_cross(&basis[2], &basis[1], &basis[0]);
            for (j = 0; j < 4; j++) {
                fast_vec_scale(D_L00_00161808_macro[i], &corner, &D_L00_001DBF20[j]);
                transform_vector(&quad.positions[j], &corner, &basis[0]);
            }
            fast_draw_quad_real(&quad, 0, 0);
        }
    }
}
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2be890;
typedef struct {
    V_2be890 corner[4];
    int color[4];
    int unk50, unk54, unk58;
    float unk5C, unk60;
    int unk64;
    float unk68, unk6C;
    long unk70, tex, unk80, unk88;
} Q_2be890;
extern float g_debug_camera[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern V_2be890 D_L00_001DBF60_2be890 __asm__("D_L00_001DBF60") __attribute__((section(".data")));
extern char D_0013F5E0[];
void FUN_001f9a68_2be890(float, void *, void *) __asm__("FUN_001f9a68");
void FUN_001f9a10_2be890(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9a28_2be890(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_001f9bf8_2be890(void *, void *, float) __asm__("FUN_001f9bf8");
void FUN_001f9ad8_2be890(void *, void *, void *) __asm__("FUN_001f9ad8");
void FUN_001f9d20_2be890(void *, void *, void *) __asm__("FUN_001f9d20");
long FUN_001f44b8_2be890(int) __asm__("FUN_001f44b8");
void FUN_001f7d30_2be890(void *, void *, int) __asm__("FUN_001f7d30");
extern float beam_origin_quad_basis_offset_length_gp __asm__("D_L00_001617B4") __attribute__((sda));
extern float beam_origin_quad_first_pass_corner_scale_gp __asm__("D_L00_001617AC") __attribute__((sda));
extern float beam_origin_quad_second_pass_corner_scale_gp __asm__("D_L00_001617B0") __attribute__((sda));
extern int beam_origin_quad_first_pass_vertex_color_gp __asm__("D_L00_001617B8") __attribute__((sda));
void draw_beam_origin_camera_facing_quads(void *mv) {
    char *moby_bytes = mv;
    Q_2be890 quad;
    V_2be890 camera_facing_transform[4];
    V_2be890 scaled_corner;
    V_2be890 offset_vector;
    int first_pass_vertex_color;
    int i, j;
    quad.tex = FUN_001f44b8_2be890(11);
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
    FUN_001f9a10_2be890(&camera_facing_transform[3], &D_L00_001DBF60_2be890, &offset_vector);
    FUN_001f9a28_2be890(&offset_vector, &(*(V_2be890 *)g_debug_camera), &camera_facing_transform[3]);
    FUN_001f9bf8_2be890(&camera_facing_transform[0], &offset_vector, 1.0f);
    FUN_001f9ad8_2be890(&camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2be890 *)D_0013F5E0));
    FUN_001f9bf8_2be890(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    FUN_001f9ad8_2be890(&camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2be890(beam_origin_quad_first_pass_corner_scale_gp, &scaled_corner, &D_L00_001DBF20[3 - i]);
        FUN_001f9d20_2be890(&quad.corner[3 - i], &scaled_corner, &camera_facing_transform[0]);
    }
    FUN_001f7d30_2be890(&quad, 0, 0);
    quad.color[3] = -1;
    quad.color[2] = -1;
    quad.color[1] = -1;
    quad.color[0] = -1;
    for (j = 3; j >= 0; j--) {
        FUN_001f9a68_2be890(beam_origin_quad_second_pass_corner_scale_gp, &scaled_corner, &D_L00_001DBF20[3 - j]);
        FUN_001f9d20_2be890(&quad.corner[3 - j], &scaled_corner, &camera_facing_transform[0]);
    }
    FUN_001f7d30_2be890(&quad, 0, 0);
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Nudge a position and ask the level helper about it, lowering a global on success. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFDB8), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Vx __attribute__((aligned(16)));

extern char D_0013E533[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern int D_L00_00173E40[];
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

int probe_moby_raised_sphere_collision(char *moby, int b, Vx *position_override) __asm__("FUN_L00_002beab0");

int probe_moby_raised_sphere_collision(char *moby, int b, Vx *position_override) {
    Vx probe_position;
    float probe_scale;
    float vertical_offset;
    int r;
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
#define NOT_SDA

#define MACRO_ADDR

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
#define NOT_SDA

#define MACRO_ADDR

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
#define NOT_SDA

#define MACRO_ADDR

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
    u128 q;
    float f[4];
} Vec4_2bec80;
typedef struct {
    u8 pad[0x18];
    char *o18;
    u8 pad2[0xC];
    float f28;
} S173E40_2bec80;
extern int D_L00_00173E40[] __attribute__((section(".data")));
int FUN_001efa68_002bec80(Vec4_2bec80 *, Vec4_2bec80 *, int, void *, int) __asm__("FUN_001efa68");
unsigned int FUN_001f0b58_002bec80(void) __asm__("FUN_001f0b58");
float probe_moby_landing_height(Vec4_2bec80 *probe_position, void *probing_moby) __asm__("FUN_L00_002bec80");

float probe_moby_landing_height(Vec4_2bec80 *probe_position, void *probing_moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    Vec4_2bec80 lower_probe_point, upper_probe_point;
    float f, unclamped_lower_z;
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
        char *hit_moby = (*(S173E40_2bec80 *)D_L00_00173E40).o18;
        if (hit_moby && *(char **)(hit_moby + 0x24) && *(short *)(*(char **)(hit_moby + 0x24) + 0x46) == 5) {
            return (*(S173E40_2bec80 *)D_L00_00173E40).f28;
        }
        switch (FUN_001f0b58_002bec80()) {
        case COLLISION_MATERIAL_WATER:
        case 1:
        case 3:
        case 8:
        case 11:
        case 12:
        case 13:
            break;
        default:
            return (*(S173E40_2bec80 *)D_L00_00173E40).f28;
        }
    }
    return 0.0f;
}
extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern char D_0013F3D0[] __attribute__((section(".data")));
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float FUN_001f99c0(float);
extern int FUN_001efa68_2bed90(void *, void *, int, void *, int) __asm__("FUN_001efa68");

char *select_class5_target_or_hero(char *self, float *position, float *angles, int unused, int mode) __asm__("FUN_L00_002bed90");

char *select_class5_target_or_hero(char *self, float *position, float *angles, int unused, int mode) {
    char *candidate;
    char *best = 0;
    float target[4] __attribute__((aligned(16)));
    float bestScore = 1000000000.0f;
    float distance, score, angleError, heightError, bonus;
    int checkCollision = 1;
    int valid;
    int i;
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
        angleError = fast_difference_between_rotations(angles[2], fast_arc_tan(target[0] - position[0], target[1] - position[1]));
        angleError *= angleError;
        heightError = fast_difference_between_rotations(angles[1], fast_arc_tan(distance, target[2] - position[2]));
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
        if (vector_distance_xy(self + 0x10, playerPosition) > 3.2f)
            best = *(char **)(playerPosition + 0x2000);
    }
    return best;
}
#include "sda.h"

typedef int V4 __attribute__((mode(TI), aligned(16)));
typedef union {
    V4 q;
    f32 f[4];
} VU;

typedef struct {
    u8 pad0[0x46];
    s16 h46;
} Cls;
typedef struct {
    u8 pad0[0x24];
    Cls *p24;
} Hit;
typedef struct {
    u8 pad0[0x38];
    Hit *p38;
} Ext;
typedef struct {
    u8 pad0[0x10];
    f32 f10;
    f32 f14;
    u8 pad18[0x30];
    f32 f48;
    u8 pad4C[0x2C];
    Ext *p78;
} Mob;
typedef struct {
    u8 pad0[0x18];
    Hit *unk18;
} XD;

extern Hit *D_L00_00173E58_bf __asm__("D_L00_00173E58") NOT_SDA;
extern int D_L00_00173E40[];
extern VU D_L00_00173E60;

extern s32 FUN_001fa6d0_c(f32) __asm__("FUN_001fa6d0");
extern f32 FUN_001f9e90_c(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern s32 FUN_001efa68_c(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern f32 FUN_L00_002bec80_c(void *, void *) __asm__("FUN_L00_002bec80");

f32 select_moby_heading_by_collision_probes(Mob *moby, VU *probe_origin, f32 *target_position, f32 *out, f32 ang, f32 arc, f32 step, f32 probe_height_offset, f32 horizontal_probe_distance,
                     f32 probe_start_horizontal_offset) __asm__("FUN_L00_002bf050");

f32 select_moby_heading_by_collision_probes(Mob *moby, VU *probe_origin, f32 *target_position, f32 *out, f32 probe_heading, f32 arc, f32 step, f32 probe_height_offset, f32 horizontal_probe_distance,
                     f32 probe_start_horizontal_offset) {
    VU probe_start;
    VU probe_end;
    f32 da;
    s32 sweep_probe_count;
    s32 i;
    f32 best;
    f32 bestDist;
    f32 bestDiff;
    f32 preferred_heading;
    f32 a;
    f32 h2;
    f32 hit_distance;
    f32 sweep_heading_error;
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
        preferred_heading = FUN_001f9e90_c(target_position[0] - moby->f10, target_position[1] - moby->f14);
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
        if (FUN_L00_002bec80_c(&probe_end, moby) != 0.0f) {
            best = preferred_heading;
            bestDist = horizontal_probe_distance;
            *out = horizontal_probe_distance;
        }
    } else {
        if (D_L00_00173E58_bf == x->p38 || (D_L00_00173E58_bf != 0 && D_L00_00173E58_bf->p24 != 0 &&
                                            D_L00_00173E58_bf->p24->h46 == 5)) {
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
            if (FUN_L00_002bec80_c(&probe_end, moby) != 0.0f) {
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
        if (D_L00_00173E58_bf != x->p38 &&
            !(D_L00_00173E58_bf != 0 && D_L00_00173E58_bf->p24 != 0 &&
              D_L00_00173E58_bf->p24->h46 == 5)) {
            hit_distance = fast_vec_dist(&probe_start, &D_L00_00173E60);
            best = fast_add_rotations(best, (horizontal_probe_distance - hit_distance) * 3.1415927f / (horizontal_probe_distance * 3.0f));
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
        if (D_L00_00173E58_bf != x->p38 &&
            !(D_L00_00173E58_bf != 0 && D_L00_00173E58_bf->p24 != 0 &&
              D_L00_00173E58_bf->p24->h46 == 5)) {
            hit_distance = fast_vec_dist(&probe_start, &D_L00_00173E60);
            best = fast_subtract_rotations(best, (horizontal_probe_distance - hit_distance) * 3.1415927f / (horizontal_probe_distance * 3.0f));
        }
    }
    if (2.5132742f < fast_difference_between_rotations(best, moby->f48)) {
        best = fast_add_rotations(moby->f48, fast_subtract_rotations(best, moby->f48) * 0.5f);
    }
    return best;
}
/* Steps a vector by n repeated additions of the normalised difference. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002C0B18), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9a68(void *, void *, float);

void accumulate_local_vector_steps(char *start_vector, char *end_vector, int step_count) __asm__("FUN_L00_002bf810");

void accumulate_local_vector_steps(char *start_vector, char *end_vector, int step_count) {
    float step_vector[4];
    float accumulated_vector[4];
    int i;
    fast_vec_sub(step_vector, end_vector, start_vector);
    FUN_001f9a68(step_vector, step_vector, 1.0f / convert_integer_to_float(step_count));
    qcopy(accumulated_vector, start_vector);
    for (i = 0; i < step_count; i++) {
        fast_vec_add(accumulated_vector, accumulated_vector, step_vector);
    }
}
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f99c0(float);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025abf0(void *, void *, int, float, float);

void initialize_moby_hop_trajectory(unsigned char *m, unsigned char *o, u128 *src) __asm__("FUN_L00_002bf8b0");

void initialize_moby_hop_trajectory(unsigned char *m, unsigned char *o, u128 *src) {
    float s, t, r, lim;
#define F2BF(p, x) (*(float *)((p) + (x)))
    *(int *)(o + 0x30) = 0;
    qcopy(o, src);
    s = vector_distance_xy(m + 0x10, o);
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
    char pad0[0x24];
    float unk24;                  /* 0x24 */
} Cls_2C0CF8;

typedef struct Mob_2C0CF8 {
    char pad0[0x10];
    float pos[4];                 /* 0x10 */
    char pad20[4];
    Cls_2C0CF8 *p24;              /* 0x24 */
    char pad28[4];
    float f2C;                    /* 0x2C */
    char pad30[0x8C];
    unsigned char bBC;            /* 0xBC */
} Mob_2C0CF8;

typedef struct Blk_2C0CF8 {
    char pad0[0x38];
    void *p38;                    /* 0x38 */
    char pad3C[0x14];
    float f50;                    /* 0x50 */
} Blk_2C0CF8;

typedef struct {
    char pad[0x46];
    s16 field46;
} SubStruct;

typedef struct {
    char pad[0x24];
    SubStruct *sub;
} TopStruct;

extern int FUN_001efa68_c2(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern int FUN_L00_001ff600();
extern void add_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz_c2(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
float calculate_triangular_corrected_scalar(float *a, float *b, int *out, float f) __asm__("FUN_L00_00259468");
float FUN_L00_002bec80_c2(void *p, void *q) __asm__("FUN_L00_002bec80");
float absolute_float(float input) __asm__("func_001F99C0");
int FUN_L00_002beab0_c(char *a, int b, void *c) __asm__("FUN_L00_002beab0");
void FUN_L00_002bec00_c(char *a) __asm__("FUN_L00_002bec00");
void FUN_L00_002bec48_c(char *a) __asm__("FUN_L00_002bec48");
void FUN_L00_002bf810_c(char *a, char *b, int n) __asm__("FUN_L00_002bf810");
void FUN_L00_002bf8b0_c(unsigned char *m, unsigned char *o, void *src) __asm__("FUN_L00_002bf8b0");
extern void func_001F9BF0_2C0CF8(void *, void *, void *) __asm__("FUN_001f9a28");
extern int func_L00_001FF5B0_2C0CF8(float, void *, void *) __asm__("FUN_L00_001ff600");
extern void func_001F9BD8_2C0CF8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_001F9C30_2C0CF8(void *, void *, float) __asm__("FUN_001f9a68");
extern float func_001F9CE8_2C0CF8(void *) __asm__("FUN_001f9b20");
extern float func_L00_002BFF88_2C0CF8(void *, void *) __asm__("FUN_L00_002bec80");
extern void func_L00_001FF4B0_2C0CF8(void *, void *, float) __asm__("FUN_001f9bf8");
extern float func_001F9B88_2C0CF8(float) __asm__("FUN_001f99c0");
extern int func_L00_002BFDB8_2C0CF8(void *, int, void *) __asm__("FUN_L00_002beab0");
extern void func_L00_002BFF08_2C0CF8(void *a) __asm__("FUN_L00_002bec00");
extern void func_L00_002BFF50_2C0CF8(void *a) __asm__("FUN_L00_002bec48");
extern void func_L00_002C0BB8_2C0CF8(void *m, void *o, void *src) __asm__("FUN_L00_002bf8b0");
extern float func_L00_0025A4A0_2C0CF8(void *a, void *b, int *out, float f) __asm__("FUN_L00_00259468");
extern void func_L00_002C0B18_2C0CF8(void *a, void *b, int n, int c, int d, int e, float g, int f) __asm__("FUN_L00_002bf810");
extern int func_L00_001EFFF0_2C0CF8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern char D_L00_00173F80_2C0CF8[] __asm__("D_L00_00173E80");
extern void *D_L00_00173F58_2C0CF8[] __asm__("D_L00_00173E58");
extern char D_0013E633_2C0CF8[] __asm__("D_00141260");

int try_start_moby_hop(Mob_2C0CF8 *moby, Blk_2C0CF8 *block, void *target, int check_line) __asm__("FUN_L00_002bf9f0");

int try_start_moby_hop(Mob_2C0CF8 *moby, Blk_2C0CF8 *block, void *target, int check_line) {
    float jump[4];
    float land[4];
    float from[4];
    float to[4];
    float half[4];
    float mid[4];
    float up[4];
    int can_halve;
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
    land[2] = func_L00_002BFF88_2C0CF8(land, moby);
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
    qcopy(mid, D_L00_00173F80_2C0CF8);
    func_L00_001FF4B0_2C0CF8(mid, mid, 1.0f);
    sum = func_001F9B88_2C0CF8(mid[0]);
    sum = sum + func_001F9B88_2C0CF8(mid[1]);
    if (func_001F9B88_2C0CF8(mid[2]) < sum) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    land[2] = land[2] + moby->f2C * 0.025f / moby->p24->unk24;
    if (func_L00_002BFDB8_2C0CF8(moby, 6, land)) {
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
    func_L00_002BFF08_2C0CF8(block);
    func_L00_002C0BB8_2C0CF8(moby, D_0013E633_2C0CF8, land);
    func_L00_002BFF50_2C0CF8(block);
    func_001F9C30_2C0CF8(mid, jump, 0.5f);
    func_001F9BD8_2C0CF8(mid, mid, moby->pos);
    qzero(up);
    up[2] = block->f50;
    mid[2] = func_L00_0025A4A0_2C0CF8(moby->pos, up, 0, *(float *)(D_0013E633_2C0CF8 + 0x1C));
    func_L00_002C0B18_2C0CF8(from, mid, 10, 0, 32, 32, 40000.0f, 60);
    func_L00_002C0B18_2C0CF8(to, mid, 10, 0, 32, 32, 40000.0f, 60);
    if (func_L00_001EFFF0_2C0CF8(from, mid, 6, moby, 0) != 0 && D_L00_00173F58_2C0CF8[0] != block->p38) {
        if (can_halve) {
            return try_start_moby_hop(moby, block, half, 1);
        }
        return 0;
    }
    if (func_L00_001EFFF0_2C0CF8(mid, to, 6, moby, 0) != 0 && D_L00_00173F58_2C0CF8[0] != block->p38) {
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
unsigned char *create_moby(int) __asm__("FUN_0020c4f8");
void FUN_L00_00250df8(unsigned char *);
void copy_hero_moby_field_38(unsigned char *) __asm__("FUN_L00_0025d1b8");
unsigned char *spawn_doom_bot(int a, u128 *spawn_position, u128 *initial_vector) __asm__("FUN_L00_002bfe08");

unsigned char *spawn_doom_bot(int linked_moby, u128 *spawn_position, u128 *initial_vector) {
    unsigned char *spawned_moby = create_moby(0xba);
    if (spawned_moby) {
        char *moby_vars;
        spawned_moby[0x30] = 0xff;
        *(short *)(spawned_moby + 0x32) = 0xff;
        spawned_moby[0x31] = 1;
        moby_vars = *(char **)(spawned_moby + 0x78);
        spawned_moby[0x20] = 0;
        spawned_moby[0xbc] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(moby_vars + 0x10, initial_vector);
        *(int *)(moby_vars + 0x60) = linked_moby;
        FUN_L00_00250df8(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
        *(float *)(spawned_moby + 0x2c) = *(float *)(*(char **)(spawned_moby + 0x24) + 0x24) * 0.1f;
    }
    return spawned_moby;
}
#include "rnc/math/vector.h"
extern char *D_L00_00173E58 __attribute__((section(".data")));
int FUN_001efa68(Vec4 *, Vec4 *, int, void *, int);
unsigned int collision_hit_material(void) __asm__("FUN_001f0b58");
int compute_moby_state_from_vertical_probe(unsigned char *moby) __asm__("FUN_L00_002bfec8");

int compute_moby_state_from_vertical_probe(unsigned char *moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    Vec4 lower_probe_point, upper_probe_point;
    int resulting_moby_state = moby[0x20];
    qcopy(&lower_probe_point, moby + 0x10);
    qcopy(&upper_probe_point, moby + 0x10);
    lower_probe_point.f[2] -= 0.3f;
    upper_probe_point.f[2] += 1.0f;
    if (FUN_001efa68(&upper_probe_point, &lower_probe_point, 6, moby, 0)) {
        char *hit_moby = D_L00_00173E58;
        if (hit_moby && *(char **)(hit_moby + 0x24) && *(short *)(*(char **)(hit_moby + 0x24) + 0x46) == 5) {
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
