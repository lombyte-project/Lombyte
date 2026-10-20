/* Shared moby classes: walloper, morphoray. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002b94d0.s", FUN_L00_002b94d0);
#else
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/beam_vector.h"
extern int D_L00_001615AC_gp __asm__("D_L00_001615AC") __attribute__((sda));
extern int D_L00_001615B4_gp __asm__("D_L00_001615B4") __attribute__((sda));
extern int D_L00_001615C8_gp __asm__("D_L00_001615C8") __attribute__((sda));
extern char D_0013E533[];
#include "rnc/data_labels.h"
extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern float g_debug_camera[] __asm__("D_L00_00166DC0");
extern u8 D_L00_001DB5C0[], D_L00_001DBA20[];
extern char *D_L00_00173E58 __attribute__((section(".data")));
extern Vec4 collision_hit_point __asm__("D_L00_00173E60");
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
extern short D_L00_00161658[], D_L00_00161668[], D_L00_00161670[];
extern short D_L00_00161660;
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
extern float FUN_L00_00257c48(float, float);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
/* The arc points one slot before each arc row. */
extern Vec4 D_L00_001DBA10[4][5];
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int FUN_L00_00257b90(int, int), FUN_00213260(int);
extern void build_look_at_matrix(void *, void *, void *, float) __asm__("FUN_00214890");
extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void FUN_L00_00259a88(void *, void *);
extern int FUN_L00_00259830(void *);
extern int FUN_001efa68(Vec4 *, Vec4 *, int, void *, int);
extern void collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_0022da68(int, int, void *);
extern char D_L00_0015F580_lui[] __asm__("D_L00_0015F580");
extern unsigned char *FUN_L00_00272060(void *, int, int, int, int, float *, float, float, float);

typedef struct { Vec4 direction; int words[2]; u8 kind, mode; short id; float amount; int flag; } HitDescriptor_2b94d0;
void FUN_L00_002b94d0(u8 *o) {
    Vec4 target0, target1;
    struct { Vec4 direction; int words[2]; u8 kind, mode; short id; float amount; int flag; } hit;
    Vec4 camera_direction, direction, delta, blend, step, offset;
    char *collided[128];
    Vec4 target_delta;
    union { struct { Vec4 origin; HitDescriptor_2b94d0 hit0; } first; HitDescriptor_2b94d0 hit1; } effects;
    /* Retail uses a five-vector scratch span with differences beginning at slot 1. */
    Vec4 differences[5];
    u8 *v = *(u8 **)(o + 0x78);
    char *best = 0, *second = 0, *candidate, **list, *data;
    float best_score = 0.0f, second_score = 0.0f;
    unsigned int enabled = (u8)D_0013E533[0];
    float spacing = D_L00_00161594 / 20.0f;
    float distance, angle, score, n, amplitude, wave, length, factor, phase, side_wave;
    struct Hero *player;
    char *camera_base;
    char *hero_base;
    float primary_distance, sign;
    float enabled_float;
    int i, j, hit_result, collided_count, spin, timer, difference;
    char *point, *previous, *side;
    char *collision_previous;
    (*(float *)&D_L00_001615AC_gp) = fast_add_rotations((*(float *)&D_L00_001615AC_gp), D_L00_001615B0);
    (*(float *)&D_L00_001615B4_gp) = fast_subtract_rotations((*(float *)&D_L00_001615B4_gp), D_L00_001615B8);
    (*(float *)&D_L00_001615C8_gp) = fast_add_rotations((*(float *)&D_L00_001615C8_gp), D_L00_001615CC);
    if (!*(char **)(v + 0x20) || (enabled && !*(char **)(v + 0x38))) {
        if (*(char **)(v + 0x38)) {
            *(char **)(v + 0x20) = *(char **)(v + 0x38);
            *(short *)(v + 0x40) = *(short *)(v + 0x42);
            *(char **)(v + 0x38) = 0;
            *(short *)(v + 0x42) = 0;
        } else {
            for (list = active_mobies; (candidate = *list) != 0; list++) {
                camera_base = (char *)g_debug_camera - 0x140;
                hero_base = D_0013F3D0 - 0x80;
                if (candidate == *(char **)(v + 0x20) || candidate == *(char **)(v + 0x38) ||
                    !candidate || !*(char **)(candidate + 0x24) ||
                    *(short *)(*(char **)(candidate + 0x24) + 0x46) != 5) continue;
                if ((u8)candidate[0x20] == 0xfe) goto next_candidate;
                if ((u8)candidate[0x20] == 0xfd) goto next_candidate;
                if (!suck_get_aim_height(candidate)) continue;
                fast_vec_sub(&target0, candidate + 0x10, v);
                distance = vector_length_xy(&target0);
                if (D_L00_00161594 < distance) continue;
                length = fast_difference_between_rotations(*(float *)(camera_base + 0x158),
                    fast_arc_tan(*(float *)(candidate + 0x10) - *(float *)(camera_base + 0x140),
                                  *(float *)(candidate + 0x14) - *(float *)(camera_base + 0x144)));
                if (D_L00_0016159C < length) continue;
                score = fast_difference_between_rotations(
                    fast_arc_tan(*(float *)(candidate + 0x10) - *(float *)(hero_base + 0x80),
                                  *(float *)(candidate + 0x14) - *(float *)(hero_base + 0x84)),
                    *(float *)(hero_base + 0x98));
                if (D_L00_00161598 < score) continue;
                score = ((D_L00_0016159C - length) / D_L00_0016159C) * D_L00_0016162C +
                        (D_L00_00161594 - distance) / D_L00_00161594;
                if (best_score < score) {
                    second_score = best_score; second = best;
                    best_score = score; best = candidate;
                } else if (second_score < score) { second_score = score; second = candidate; }
next_candidate:;
            }
        }
    }
    enabled_float = (float)(int)enabled;
    if (best) {
        if (*(char **)(v + 0x20)) second = best;
        else {
            *(char **)(v + 0x20) = best;
            *(short *)(v + 0x40) = 0;
            FUN_0022da68(1, 0, o);
        }
    }
    if (second && enabled) {
        distance = fast_vec_dist(v, best + 0x10);
        if (distance < D_L00_00161594) {
            angle = fast_vec_dist(best + 0x10, second + 0x10);
            if (distance + angle < D_L00_00161594) {
                *(char **)(v + 0x38) = second;
                *(short *)(v + 0x42) = 0;
            }
        }
    }
    primary_distance = 0.0f;
    if (*(char **)(v + 0x20)) {
        data = (char *)suck_get_aim_height(*(char **)(v + 0x20));
        qcopy(&target0, *(char **)(v + 0x20) + 0x10);
        target0.f[2] += *(float *)(data + 0x10);
        primary_distance = fast_vec_dist(v, &target0);
        spacing = primary_distance * 0.05f;
    }
    if (*(char **)(v + 0x38)) {
        data = (char *)suck_get_aim_height(*(char **)(v + 0x38));
        qcopy(&target1, *(char **)(v + 0x38) + 0x10);
        target1.f[2] += *(float *)(data + 0x10);
        spacing = (fast_vec_dist(&target0, &target1) + primary_distance) * 0.05f;
    }
    if (enabled) D_L00_001615F8 = 0x207f20;
    else D_L00_001615F8 = 0x7f2020;
    clear_u64_value(&camera_direction);
    make_hit_descriptor(&hit, o, 0x210000, &camera_direction, enabled_float + 2.0f);
    hit.id = *(short *)(o + 0xa6); hit.mode = 1; hit.kind = 5;
    *(short *)(v + 0x3e) = 20;
    player = &hero;
    collect_mobies_in_sphere(0.3f, v, 0x10, player->moby, &hit);
    fast_vec_sub(&direction, v + 0x10, v);
    fast_vec_normalize(&direction, &direction, 1.0f);
    if (*(char **)(v + 0x20) || *(char **)(v + 0x38)) {
        fast_vec_cross(&delta, &direction, &player->gravity_dir);
        build_look_at_matrix(&direction, &direction, &delta, 0.17453292f);
    }
    hit_result = 0;
    collided_count = 0;
    fast_vec_scale(spacing, &step, &direction);
    qcopy(D_L00_001DB5C0, v); qcopy(D_L00_001DB5C0 + 0x140, v);
    difference = 16;
    point = (char *)D_L00_001DB5C0 + 16;
    previous = (char *)D_L00_001DB5C0;
    side = (char *)D_L00_001DB5C0 + 0x150;
    for (i = 1; i < 20; i++, difference += 16, point += 16, side += 16, previous += 16) {
        n = (float)i;
        factor = D_L00_001615A4 + (D_L00_001615A8 - D_L00_001615A4) *
                    (convert_integer_to_float(i) * 0.05f);
        phase = normalize_angle((*(float *)&D_L00_001615AC_gp) + n * D_L00_001615A0);
        wave = factor * fast_sin(phase);
        wave += FUN_L00_00257c48(D_L00_001615E4, D_L00_001615E8);

        score = *(float *)(point + 8) - ((float *)D_L00_001DB840)[i];
        *(float *)(point + 8) = score;
        ((float *)D_L00_001DB840)[i] = wave;
        fast_vec_sub(&delta, point, (char *)D_L00_001DB5C0 - 16 + difference);
        blend_vector_2b94d0(0.85f, &blend, &delta, &step);
        score = fast_vec_length(&blend);
        if (score == 0.0f) qcopy(point, previous);
        else {
            fast_vec_scale(spacing / score, &blend, &blend);
            fast_vec_add(point, (char *)D_L00_001DB5C0 - 16 + difference, &blend); qcopy(&step, &blend);
        }
        *(float *)(point + 8) += wave;
        fast_vec_sub(&delta, point, (char *)D_L00_001DB5C0 - 16 + difference);
        if (*(char **)(v + 0x20)) {
            if (!enabled || !*(char **)(v + 0x38)) {
                fast_vec_sub(&target_delta, &target0, point);
                length = fast_vec_length(&target_delta);
                if (length != 0.0f) {
                    if (i == 19) { factor = 0.5f; spacing = length; }
                    else if (i >= 16) { factor = 0.5f; spacing = length / convert_integer_to_float(19 - i); }
                    else factor = 0.1f;
                    fast_vec_scale(spacing / length, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor, &step, &step, &target_delta);
                }
            } else {
                if (i < 10) fast_vec_sub(&target_delta, &target0, point);
                else fast_vec_sub(&target_delta, &target1, point);
                length = fast_vec_length(&target_delta);
                if (length != 0.0f) {
                    if (i == 9 || i == 19) { factor = 0.7f; spacing = length; }
                    else if ((unsigned)(i - 5) < 5 || i > 14) {
                        factor = 0.7f; spacing = length / convert_integer_to_float((unsigned)(i - 5) < 5 ? 9 - i : 19 - i);
                    } else factor = 0.2f;
                    fast_vec_scale(spacing / length, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor, &step, &step, &target_delta);
                }
            }
        }
        if (i < 10) {
            phase = normalize_angle((*(float *)&D_L00_001615B4_gp) + (float)i * D_L00_001615BC);
            angle = normalize_angle((*(float *)&D_L00_001615C8_gp) + (float)i * D_L00_001615D0);
            factor = D_L00_001615C0 * fast_sin(angle);
            qcopy(side, point);
            wave = fast_sin(phase);
            side_wave = FUN_L00_00257c48(0.0f, 0.1f);
            wave = factor * wave + side_wave;
            ((float *)D_L00_001DB9D0)[i] = wave;
            *(float *)(point + 0x148) += wave;
            angle = fast_add_rotations(fast_arc_tan(delta.f[0], delta.f[1]), 1.5707964f);
            wave = D_L00_001615C4 * fast_cos(phase);
            offset.f[0] = fast_cos(angle) * wave;
            offset.f[1] = fast_sin(angle) * wave;
            offset.f[2] = 0.0f;
            qcopy(&D_L00_001DB890[i], &offset);
            fast_vec_add(side, side, &offset);
        } else {
            qcopy(side, point);
            *(float *)(point + 0x148) += ((float *)D_L00_001DB9D0)[21 - i];
            fast_vec_add(side, side, &D_L00_001DB890[21 - i]);
            qcopy(&offset, &D_L00_001DB890[i]);
        }
        if (i == 1) {
            struct Hero *h = &hero;
            fast_vec_scale(0.5f, &effects.first.origin, (char *)h->moby + 0xe0);
            fast_vec_add(&target_delta, &h->motion.pos, &effects.first.origin);
            hit_result = FUN_001efa68(&target_delta, (Vec4 *)v, 0x10, h->moby, 0);
        } else if (i == 19 || !(i & 1)) {
            hit_result = FUN_001efa68((Vec4 *)((char *)D_L00_001DB5C0 - 32 + difference), (Vec4 *)point, 0x10, o, 0);
        } else continue;
        fast_vec_sub(&camera_direction, point, g_debug_camera);
        camera_direction.f[2] = 0.0f;
        length = fast_vec_length(&camera_direction);
        if (length == 0.0f) camera_direction.f[2] = 1.0f;
        else fast_vec_scale(1.0f / length, &camera_direction, &camera_direction);
        camera_direction.f[3] = 5627.9248046875f;
        qcopy(&hit.direction, &camera_direction);
        if (hit_result) {
            candidate = D_L00_00173E58;
            if (candidate) {
                if (!suck_get_aim_height(candidate) || !*(char **)(candidate + 0x24) ||
                    *(short *)(*(char **)(candidate + 0x24) + 0x46) != 5)
                    FUN_L00_00259a88(candidate, &hit);
                else {
                    hit_result = 0; collided[collided_count++] = candidate;
                    *(int *)(candidate + 0x94) = 0;
                }
            }
            if (hit_result) {
                qcopy(point, &collision_hit_point);
                *(short *)(v + 0x3e) = i;
                qcopy(v + 0x10, &collision_hit_point);
                if (!D_L00_00173E58 || FUN_L00_00259830(D_L00_00173E58)) {
                    *(char **)(v + 0x20) = 0; *(short *)(v + 0x40) = 0;
                }
                break;
            }
        }
    }
    for (i = 0; i < collided_count; i++) {
        candidate = collided[i];
        *(int *)(candidate + 0x94) = *(int *)(*(char **)(candidate + 0x24) + 0x10);
    }
    if (!hit_result) qcopy(v + 0x10, D_L00_001DB5C0 + 0x130);
    if (*(char **)(v + 0x20) && !(*(unsigned long *)(v + 0x40) & 0xffff0000ffffUL)) {
        fast_vec_sub(&target_delta, *(char **)(v + 0x20) + 0x10, g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(&effects.first.hit0, o, 0x210000, &target_delta, enabled_float + 2.0f);
        effects.first.hit0.id = *(short *)(o + 0xa6); effects.first.hit0.mode = 3; effects.first.hit0.kind = 5;
        FUN_L00_00259a88(*(char **)(v + 0x20), &effects.first.hit0);
        timer = 5;
        data = (char *)suck_get_aim_height(*(char **)(v + 0x20));
        if (data && *(short *)(data + 4) == 1) timer = 1;
        *(short *)(v + 0x44) = scale_game_frames(timer);
    }
    if (*(char **)(v + 0x38) && !(*(unsigned long *)(v + 0x40) & 0xffff0000ffff0000UL)) {
        fast_vec_sub(&target_delta, *(char **)(v + 0x38) + 0x10, g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(&effects.hit1, o, 0x210000, &target_delta, enabled_float + 2.0f);
        effects.hit1.id = *(short *)(o + 0xa6); effects.hit1.kind = 5; effects.hit1.mode = 3;
        FUN_L00_00259a88(*(char **)(v + 0x38), &effects.hit1);
        timer = 5;
        data = (char *)suck_get_aim_height(*(char **)(v + 0x38));
        if (data && *(short *)(data + 4) == 1) timer = 1;
        *(short *)(v + 0x46) = scale_game_frames(timer);
    }
    for (i = 0; i < 4; i++) {
        Vec4 *rows;
        char *arcpoint, *arcprev;
        Vec4 *diffp;
        if (i < 2) { qcopy(&effects.first.origin, v); factor = 0.4f; }
        else { qcopy(&effects.first.origin, v + 0x10); factor = 0.5f; }
        if (fast_dec_timer_s16(&D_L00_00161658[i])) {
            D_L00_00161658[i] = scale_game_frames(D_L00_00161668[i]);
            (&D_L00_00161660)[i] = 0;
            angle = FUN_L00_00257c48(0.5235988f, 1.0471976f);
            phase = random_float_between(-1.0471976f, 0.17453292f);
            fast_vec_scale(factor, &target_delta, &direction);
            rows = (Vec4 *)(D_0013F3D0 + 0x5d0);
            build_look_at_matrix(&target_delta, &target_delta, rows, phase);
            build_look_at_matrix(&target_delta, &target_delta, rows + 1, angle);
            qcopy(&((Vec4 (*)[5])D_L00_001DBA20)[i][0], &effects.first.origin);
            fast_vec_add(&((Vec4 (*)[5])D_L00_001DBA20)[i][1], &effects.first.origin, &target_delta);
            sign = 1.0f;
            for (j = 2; j < 5; j++) {
                fast_vec_sub(&effects.first.hit0, &D_L00_001DBA10[i][j], g_debug_camera);
                phase = random_float_between(0.17453292f, 0.9599311f) * sign;
                sign = -sign;
                build_look_at_matrix(&target_delta, &target_delta, &effects.first.hit0, phase);
                fast_vec_add(&((Vec4 (*)[5])D_L00_001DBA20)[i][j], &D_L00_001DBA10[i][j], &target_delta);
            }
        } else {
            int d, n0;
            for (j = 1; j < 5; j++) {
                fast_vec_sub(&differences[j], &((Vec4 (*)[5])D_L00_001DBA20)[i][j], &D_L00_001DBA10[i][j]);
                differences[j].f[0] += FUN_L00_00257c48(0.0f, 0.1f);
                differences[j].f[1] += FUN_L00_00257c48(0.0f, 0.1f);
                differences[j].f[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            qcopy(&((Vec4 (*)[5])D_L00_001DBA20)[i][0], &effects.first.origin);
            for (j = 1; j < 5; j++)
                fast_vec_add(&((Vec4 (*)[5])D_L00_001DBA20)[i][j], &D_L00_001DBA10[i][j], &differences[j]);
            d = D_L00_00161670[i]; n0 = D_L00_00161658[i];
            if (d < n0) d = n0 - d; else d = d - n0;
            distance = convert_integer_to_float(d) / convert_integer_to_float(scale_game_frames(15));
            (&D_L00_00161660)[i] = truncate_float_to_s32((1.0f - distance) * 32.0f);
        }
    }
    angle = FUN_L00_00257c48(0.5235988f, 1.0471976f);
    phase = random_float_between(-0.5235988f, 1.0471976f);
    fast_vec_scale(D_L00_00161628 * frame_scale, &target_delta, &direction);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5d0, phase);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5e0, angle);
    spin = FUN_L00_00257b90(1, 8); if (FUN_00213260(2)) spin = -spin;
    amplitude = random_float_between(0.2f, 0.6f);
    FUN_L00_00272060(v, scale_game_frames(15), D_L00_001615F8 | 0x7f000000, 0, spin,
                    (float *)&target_delta, amplitude * 0.3f, amplitude, 0.0f);
    spin = -spin;
    FUN_L00_00272060(v, scale_game_frames(15), 0x307f7f7f, 0, spin,
                    (float *)&target_delta, amplitude * 0.15f, amplitude * 0.5f, 0.0f);
    spin = FUN_L00_00257b90(1, 8); if (FUN_00213260(2)) spin = -spin;
    amplitude = random_float_between(0.15f, 3.0f);
    FUN_L00_00272060(v + 0x10, scale_game_frames(12), D_L00_001615F8 | 0x7f000000, 0, spin,
                    (float *)D_L00_0015F580_lui, amplitude * 0.1f, amplitude, 0.0f);
}
#endif
typedef struct {
    u8 pad[0x3E];
    s16 h3E;
} S2ba970;
extern f32 D_L00_001615EC __attribute__((sda));
extern f32 D_L00_001615F0 __attribute__((sda));
extern s32 D_L00_001615D4 __attribute__((sda));
extern s32 D_L00_001615DC __attribute__((sda));
extern f32 D_L00_001615FC __attribute__((sda));
extern f32 D_L00_00161600 __attribute__((sda));
extern f32 D_L00_00161630 __attribute__((sda));
extern s32 D_L00_00161604 __attribute__((sda));
extern s32 D_L00_00161608 __attribute__((sda));
extern f32 D_L00_0016160C __attribute__((sda));
extern f32 D_L00_00161610 __attribute__((sda));
extern f32 D_L00_00161634 __attribute__((sda));
extern s16 D_L00_00161660;
extern s16 D_L00_00161662;
extern s16 D_L00_00161664;
extern s16 D_L00_00161666;
extern u8 D_L00_001DB5C0[];
extern u8 D_L00_001DBA20[];
void FUN_L00_002baae0(void *, s32, s32, s32, f32, f32, f32);
void FUN_L00_002bb0b0(void *);
void FUN_L00_002ba970(u8 *o) {
    S2ba970 *v = *(S2ba970 **)(o + 0x78);
    D_L00_001615EC -= D_L00_001615F0 * frame_scale;
    if (D_L00_001615EC <= -8.0f) {
        D_L00_001615EC += 8.0f;
    }
    FUN_L00_002baae0(D_L00_001DB5C0, D_L00_001615D4, D_L00_001615DC, v->h3E, D_L00_001615FC,
                     D_L00_00161600, D_L00_00161630);
    FUN_L00_002baae0(D_L00_001DB5C0 + 0x140, D_L00_00161604, D_L00_00161608, v->h3E, D_L00_0016160C,
                     D_L00_00161610, D_L00_00161634);
    FUN_L00_002baae0(D_L00_001DBA20, D_L00_00161660, D_L00_00161660, 5, D_L00_0016160C,
                     D_L00_00161610, 0.2f);
    FUN_L00_002baae0(D_L00_001DBA20 + 0x50, D_L00_00161662, D_L00_00161662, 5, D_L00_0016160C,
                     D_L00_00161610, 0.2f);
    FUN_L00_002baae0(D_L00_001DBA20 + 0xA0, D_L00_00161664, D_L00_00161664, 5, D_L00_0016160C,
                     D_L00_00161610, 0.2f);
    FUN_L00_002baae0(D_L00_001DBA20 + 0xF0, D_L00_00161666, D_L00_00161666, 5, D_L00_0016160C,
                     D_L00_00161610, 0.2f);
    FUN_L00_002bb0b0(o);
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
extern int D_L00_00161638 __attribute__((sda));
extern int D_L00_0016163C __attribute__((sda));
extern int D_L00_001615F4 __attribute__((sda));
extern int D_L00_001615F8 __attribute__((sda));
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern long tex_2baae0(int) __asm__("FUN_001f44b8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
extern void draw_2baae0(void *, void *, int) __asm__("FUN_001f7d30");
extern void gsreg_2baae0(int, long) __asm__("FUN_00233980");

void FUN_L00_002baae0(void *mv, int a, int b, int n, float sx, float sy, float sz) {
    char *m = mv;
    RW_2baae0 *v = mv;
    RW_2baae0 q[4];
    RT_2baae0 t;
    RW_2baae0 q2[4];
    RT_2baae0 u;
    RW_2baae0 w120, w130, w140, w150, r160, r170, segment_start_camera_offset, b190, s1A0, s1B0;
    int i, col0, col1;
    float len, len0, l0, l1, l2;
    t.q0 = tex_2baae0(D_L00_00161638);
    col0 = D_L00_001615F4 | (a << 24);
    t.q2 = 0x8000000048L;
    t.q1 = 0xFF9000000260L;
    t.z4 = 0;
    t.c[3] = col0;
    t.c[2] = col0;
    t.c[1] = col0;
    t.c[0] = col0;
    u.q0 = tex_2baae0(D_L00_0016163C);
    col1 = D_L00_001615F8 | (b << 24);
    u.q2 = t.q2;
    u.q1 = t.q1;
    u.z4 = t.z4;
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
    t.f[0] = D_L00_001615EC + 0;
    t.f[1] = 0;
    t.f[2] = D_L00_001615EC + 0;
    t.f[3] = 1.0f;
    t.f[4] = D_L00_001615EC + 1.0f;
    t.f[5] = 0;
    t.f[6] = D_L00_001615EC + 1.0f;
    t.f[7] = 1.0f;
    fast_vec_sub(w140, m, g_debug_camera);
    fast_vec_sub(w150, m + 0x10, m);
    fast_vec_cross(w120, w150, w140);
    len0 = fast_vec_length(w120);
    if (len0 != 0)
        len0 = 1.0f / len0;
    fast_vec_scale(D_L00_001615FC * len0, w120, w120);
    fast_vec_add(q[0], m, w120);
    fast_vec_sub(q[1], m, w120);
    for (i = 1; i < n - 1; i++) {
        fast_vec_sub(w140, v[i], g_debug_camera);
        fast_vec_sub(w150, v[i + 1], v[i]);
        fast_vec_cross(w120, w150, w140);
        len = fast_vec_length(w120);
        if (len != 0)
            len = 1.0f / len;
        fast_vec_scale(D_L00_001615FC * len, w120, w120);
        fast_vec_add(q[2], v[i], w120);
        fast_vec_sub(q[3], v[i], w120);
        if (i == 1) {
            t.c[1] = D_L00_001615F4;
            t.c[0] = D_L00_001615F4;
        } else if (i == n - 2) {
            t.c[3] = D_L00_001615F4;
            t.c[2] = D_L00_001615F4;
        } else if (i == 2) {
            t.c[1] = col0;
            t.c[0] = col0;
        }
        draw_2baae0(q, 0, 0);
        qcopy(q[0], q[2]);
        qcopy(q[1], q[3]);
    }
    for (i = 0; i < n - 1; i++) {
        fast_vec_sub(segment_start_camera_offset, m + i * 16, g_debug_camera);
        fast_vec_sub(b190, m + i * 16 + 16, g_debug_camera);
        l0 = fast_vec_length(segment_start_camera_offset);
        l1 = fast_vec_length(b190);
        if (l1 < l0) {
            if (l1 == 0)
                qcopy(b190, segment_start_camera_offset);
            else
                fast_vec_scale(l0 / l1, b190, b190);
        } else {
            if (l0 == 0)
                qcopy(segment_start_camera_offset, b190);
            else
                fast_vec_scale(l1 / l0, segment_start_camera_offset, segment_start_camera_offset);
        }
        fast_vec_add(r160, g_debug_camera, segment_start_camera_offset);
        fast_vec_add(r170, g_debug_camera, b190);
        fast_vec_sub(w150, r170, r160);
        fast_vec_scale(sz, s1B0, w150);
        fast_vec_sub(r160, r160, s1B0);
        fast_vec_add(r170, r170, s1B0);
        fast_vec_sub(s1A0, g_debug_camera, r160);
        fast_vec_cross(w120, s1A0, w150);
        l2 = fast_vec_length(w120);
        if (l2 != 0)
            l2 = 1.0f / l2;
        fast_vec_scale(sy * l2, w130, w120);
        fast_vec_add(q2[0], r160, w130);
        fast_vec_sub(q2[1], r160, w130);
        fast_vec_add(q2[2], r170, w130);
        fast_vec_sub(q2[3], r170, w130);
        if (i == 0) {
            u.c[1] = D_L00_001615F8;
            u.c[0] = D_L00_001615F8;
        } else if (i == n - 2) {
            u.c[3] = D_L00_001615F8;
            u.c[2] = D_L00_001615F8;
        } else if (i == 1) {
            u.c[1] = col1;
            u.c[0] = col1;
        }
        draw_2baae0(q2, 0, 0);
    }
    gsreg_2baae0(0x47, 0x5360B);
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
extern float D_L00_00161648 __attribute__((sda));
extern float D_L00_0016164C __attribute__((sda));
extern V_2bb0b0 D_L00_001DB560[] __attribute__((section(".data")));
extern UV_2bb0b0 D_L00_001DB5A0[] __attribute__((section(".data")));
void FUN_L00_002bb0b0(void *mv) {
    Q_2bb0b0 quad;
    V_2bb0b0 mat[4];
    V_2bb0b0 *pos = *(V_2bb0b0 **)((char *)mv + 0x78);
    unsigned int c;
    float s;
    int i;
    quad.tex = tex_2bb0b0(0xB);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    vsub_2bb0b0(&mat[3], &(*(V_2bb0b0 *)g_debug_camera), pos);
    fast_vec_normalize(&mat[3], &mat[3], 0.3f);
    vadd_2bb0b0(&mat[3], &mat[3], pos);
    mat[3].f[3] = 1.0f;
    vsub_2bb0b0(&mat[0], &(*(V_2bb0b0 *)g_debug_camera), &mat[3]);
    fast_vec_normalize(&mat[0], &mat[0], 1.0f);
    cross_2bb0b0(&mat[1], &mat[0], &(*(V_2bb0b0 *)D_0013F5E0));
    fast_vec_normalize(&mat[1], &mat[1], -1.0f);
    cross_2bb0b0(&mat[2], &mat[1], &mat[0]);
    c = D_L00_00161640;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    s = D_L00_00161648 + rnd_2bb0b0(0.0f, 0.025f);
    for (i = 0; i < 4; i++) {
        vmul_2bb0b0(s, &quad.corner[i], &D_L00_001DB560[i]);
        xform_2bb0b0(&quad.corner[i], &quad.corner[i], &mat[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_2bb0b0(&quad, 0, 0);
    c = D_L00_001615F8_2bb0b0;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    s = D_L00_0016164C + rnd_2bb0b0(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        vmul_2bb0b0(s, &quad.corner[i], &D_L00_001DB560[i]);
        xform_2bb0b0(&quad.corner[i], &quad.corner[i], &mat[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_2bb0b0(&quad, 0, 0);
}
u8 *create_moby(s32) __asm__("FUN_0020c4f8");
void clear_u64_value() __asm__("FUN_001f99f8");
void refresh_moby_spatial_bounds_from_basis() __asm__("FUN_0020e098");
void FUN_L00_00250df8();
u8 *FUN_L00_002bb360(u8 *src) {
    u8 *m = create_moby(0xB3);
    if (m) {
        u8 *v = *(u8 **)(m + 0x78);
        m[0x30] = 0xFF;
        *(s16 *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        *(s16 *)(m + 0x34) = 0x204;
        m[0x23] = 0x50;
        m[0x20] = 0;
        *(u8 **)(v + 8) = src;
        clear_u64_value(m + 0x40);
        *(OvlQuad *)(m + 0xC0) = *(OvlQuad *)(src + 0xC0);
        *(OvlQuad *)(m + 0xD0) = *(OvlQuad *)(src + 0xD0);
        *(OvlQuad *)(m + 0xE0) = *(OvlQuad *)(src + 0xE0);
        refresh_moby_spatial_bounds_from_basis(m);
        FUN_L00_00250df8(m);
    }
    return m;
}
typedef struct {
    char pad[0x10B0];
    int i10B0;
} G_2bb400;
extern void FUN_0020c828_2bb400(void *) __asm__("FUN_0020c828");
extern int FUN_001f96f8_2bb400(int) __asm__("FUN_001f96f8");
extern void FUN_L00_0024efb0_2bb400(void *, int, int) __asm__("FUN_L00_0024efb0");
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

void FUN_L00_002bb400(unsigned char *m) {
    V_2bb400 *v = *(V_2bb400 **)(m + 0x78);
    unsigned char *o = v->o;
    int old, n, d;
    if (o[0x20] == 0 || (o[0x20] & 0xF0) == 0xF0) {
        FUN_0020c828_2bb400(m);
        return;
    }
    if (o != 0 && (*(u16 *)(m + 0x34) & 1) != (*(u16 *)(o + 0x34) & 1)) {
        *(u16 *)(m + 0x34) &= ~1;
        *(u16 *)(m + 0x34) |= *(u16 *)(v->o + 0x34) & 1;
    }
    if (hero.items[0].unk20 < FUN_001f96f8_2bb400(0x19)) {
        *(u16 *)(m + 0x34) |= 0x41;
    }
    old = v->ang;
    d = -0xC0;
    n = old + d;
    v->ang = n;
    if (n > 0x1000) {
        d = -0x10C0;
        v->ang = old + d;
    } else if (n < 0) {
        d = 0xF40;
        v->ang = old + d;
    }
    FUN_L00_0024efb0_2bb400(*(void **)(m + 0x24), 0, d);
    *(float *)(m + 0x2C) =
        *(float *)(*(char **)(m + 0x24) + 0x24) * v->fC * FUN_002132a8_2bb400(0.5f, 1.0f);
    *(float *)(m + 0x44) =
        FUN_001fa580_2bb400(*(float *)(m + 0x44), FUN_L00_0025e310_2bb400(v->fC));
    FUN_0020e098_2bb400(m);
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
extern float D_L00_001616D4;
extern float D_L00_00166DC0[];
extern float D_L00_0017AEBC __attribute__((section(".data")));
extern float FUN_L00_00257c48(float lo, float hi);
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int D_L00_001616CC;
extern int D_L00_001616D0;
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
void walloper_update(char *m) {
    int flag = 0;
    float vec0[4] __attribute__((aligned(16)));
    float vec1[4] __attribute__((aligned(16)));
    float t[4] __attribute__((aligned(16)));
    float w[5][4] __attribute__((aligned(16)));
    struct Hero *g = &hero;
    int i;
    int n;
    int j;
    int q;
    int any;

    if (*(unsigned char *)(((char *)g->moby) + 0x53) != 0x82) {
        D_L00_0017AEBC = 0.1f;
    }
    switch (*(unsigned char *)(m + 0x20)) {
    case 0:
        m[0x20] = 2;
        for (q = 0; q < 10; q++) {
            D_L00_001DBEC0[q] = 0;
            D_L00_001DBEF0[q] = 2;
            D_L00_001DBF08[q] = 1;
        }
        D_L00_001616CC = scale_game_frames(*(int *)&D_L00_001616A8);
        D_L00_001616D0 = scale_game_frames(*(int *)&D_L00_001616AC);
        break;
    case 2:
        if (g->state.current == 0x20) {
            float f = compute_interpolated_record_value(m);
            if (6.0f <= f) {
                flag = 1;
                if (func_001F9908_r(&D_L00_00161660_e[54]) == 0) {
                    float a = convert_integer_to_float(D_L00_001616CC);
                    a = a / convert_integer_to_float(scale_game_frames(*(int *)&D_L00_001616A8));
                    D_L00_001616D4 = 1.0f - a;
                } else if (12.0f <= f) {
                    if (func_001F9908_r(&D_L00_00161660_e[56]) == 0) {
                        float a = convert_integer_to_float(D_L00_001616D0);
                        a = a / convert_integer_to_float(scale_game_frames(*(int *)&D_L00_001616AC));
                        D_L00_001616D4 = a;
                    } else {
                        D_L00_001616D4 = 0.0f;
                    }
                } else {
                    D_L00_001616D4 = 1.0f;
                }
            }
        } else {
            D_L00_001616CC = scale_game_frames(*(int *)&D_L00_001616A8);
            D_L00_001616D0 = scale_game_frames(*(int *)&D_L00_001616AC);
            blend_moby_animation(m, 1, 0, scale_game_frames(12));
        }
    }
    scale_vector_xyz(vec0, m + 0xD0, -0.5f);
    add_vector_xyz(vec0, vec0, m + 0x10);
    any = 0;
    for (j = 0; j < 10; j++) {
        if (D_L00_001DBEC0[j] != 0) {
            any = 1;
            break;
        }
    }
    if (!flag && !any)
        return;
    for (i = 0; i < 10; i = n) {
        D_L00_001DBEC0[i]--;
        if (D_L00_001DBEC0[i] < 0)
            D_L00_001DBEC0[i] = 0;
        if (D_L00_001DBEC0[i] == 0) {
            if (flag) {
                float sign;
                int k;
                D_L00_001DBEC0[i] = 2;
                D_L00_001DBED8[i] = 0x40;
                random_spherical_offset(vec1, D_L00_001616A4, D_L00_001616A4);
                qcopy(D_L00_001DBBA0[i].point[0], vec0);
                n = i + 1;
                sign = 1.0f;
                for (k = 1; k < 5; k++) {
                    float ang;
                    subtract_vector_xyz(t, D_L00_001DBB90[i].point[k - 1], D_L00_00166DC0);
                    ang = random_float_between(0.17453292f, 0.7853982f) * sign;
                    sign = -sign;
                    build_look_at_matrix(vec1, vec1, t, ang);
                    add_vector_xyz(D_L00_001DBBA0[i].point[k],
                                   D_L00_001DBB90[i].point[k - 1], vec1);
                }
            } else {
                n = i + 1;
            }
        } else {
            int k;
            float(*workspace)[4] = w;
            for (k = 1; k < 5; k++) {
                float *v = workspace[k];
                subtract_vector_xyz(v, D_L00_001DBBA0[i].point[k],
                                    D_L00_001DBB90[i].point[k - 1]);
                v[0] += FUN_L00_00257c48(0.0f, 0.1f);
                v[1] += FUN_L00_00257c48(0.0f, 0.1f);
                v[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            n = i + 1;
            qcopy(D_L00_001DBBA0[i].point[0], vec0);
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
    enqueue_callback_list_1(FUN_L00_002bbb00_c, m);
}
extern f32 D_L00_00161660_f[] __asm__("D_L00_00161660") __attribute__((section(".sdata")));
extern f32 D_L00_00161690 __attribute__((sda));
extern f32 D_L00_00161694 __attribute__((sda));
f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
void FUN_L00_002bbbc8(void *, s32, s32, s32, f32, f32, f32);
void FUN_L00_002bc1a0(void *);
void FUN_L00_002bbb00(void *o) {
    s32 i;
    s32 r;
    for (i = 0; i < 10; i++) {
        if (D_L00_001DBEC0[i] != 0) {
            r = truncate_float_to_s32(convert_integer_to_float(D_L00_001DBED8[i]) * D_L00_00161660_f[29]);
            FUN_L00_002bbbc8(&D_L00_001DBBA0[i], r, r, 5, D_L00_00161690, D_L00_00161694, 0.2f);
        }
    }
    FUN_L00_002bc1a0(o);
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

void FUN_L00_002bbbc8(void *mv, int a, int b, int n, float sx, float sy, float sz) {
    char *m = mv;
    RW q[4];
    RT t;
    RW q2[4];
    RT u;
    RW w120, w130, w140, w150, r160, r170, segment_start_camera_offset, b190, s1A0, s1B0;
    int i, col0, col1;
    float len, len0, l0, l1;
    t.q0 = get_effect_texture(*(int *)&D_L00_001616B0);
    col0 = *(int *)&D_L00_00161680 | (a << 24);
    t.q2 = 0x8000000048L;
    t.q1 = 0xFF9000000260L;
    t.z4 = 0;
    t.c[3] = col0;
    t.c[2] = col0;
    t.c[1] = col0;
    t.c[0] = col0;
    u.q0 = get_effect_texture(*(int *)&D_L00_001616B4);
    col1 = *(int *)&D_L00_00161684 | (b << 24);
    u.q2 = t.q2;
    u.q1 = t.q1;
    u.z4 = t.z4;
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
    t.f[0] = 0;
    t.f[1] = 0;
    t.f[2] = 0;
    t.f[3] = 1.0f;
    t.f[4] = 1.0f;
    t.f[5] = 0;
    t.f[6] = 1.0f;
    t.f[7] = 1.0f;
    subtract_vector_xyz(w140, m, D_L00_00166DC0);
    subtract_vector_xyz(w150, m + 0x10, m);
    cross_vectors_xyz(w120, w150, w140);
    len0 = vector_length_xyz(w120);
    if (len0 != 0)
        len0 = 1.0f / len0;
    scale_vector_xyz(w120, w120, sx * len0);
    add_vector_xyz(q[0], m, w120);
    subtract_vector_xyz(q[1], m, w120);
    for (i = 1; i < n - 1; i++) {
        subtract_vector_xyz(w140, m + i * 16, D_L00_00166DC0);
        subtract_vector_xyz(w150, m + i * 16 + 16, m + i * 16);
        cross_vectors_xyz(w120, w150, w140);
        len = vector_length_xyz(w120);
        if (len != 0)
            len = 1.0f / len;
        scale_vector_xyz(w120, w120, sx * len);
        add_vector_xyz(q[2], m + i * 16, w120);
        subtract_vector_xyz(q[3], m + i * 16, w120);
        if (i == 1) {
            t.c[1] = *(int *)&D_L00_00161680;
            t.c[0] = *(int *)&D_L00_00161680;
        } else if (i == n - 2) {
            t.c[3] = *(int *)&D_L00_00161680;
            t.c[2] = *(int *)&D_L00_00161680;
        } else if (i == 2) {
            t.c[1] = col0;
            t.c[0] = col0;
        }
        draw_geometry_quad(q, 0, 0);
        qcopy(q[0], q[2]);
        qcopy(q[1], q[3]);
    }
    for (i = 0; i < n - 1; i++) {
        subtract_vector_xyz(segment_start_camera_offset, m + i * 16, D_L00_00166DC0);
        subtract_vector_xyz(b190, m + i * 16 + 16, D_L00_00166DC0);
        l0 = vector_length_xyz(segment_start_camera_offset);
        l1 = vector_length_xyz(b190);
        if (l1 < l0) {
            if (l1 == 0)
                qcopy(b190, segment_start_camera_offset);
            else
                scale_vector_xyz(b190, b190, l0 / l1);
        } else {
            if (l0 == 0)
                qcopy(segment_start_camera_offset, b190);
            else
                scale_vector_xyz(segment_start_camera_offset, segment_start_camera_offset, l1 / l0);
        }
        add_vector_xyz(r160, D_L00_00166DC0, segment_start_camera_offset);
        add_vector_xyz(r170, D_L00_00166DC0, b190);
        subtract_vector_xyz(w150, r170, r160);
        scale_vector_xyz(s1B0, w150, sz);
        subtract_vector_xyz(r160, r160, s1B0);
        add_vector_xyz(r170, r170, s1B0);
        subtract_vector_xyz(s1A0, D_L00_00166DC0, r160);
        cross_vectors_xyz(w120, s1A0, w150);
        len = vector_length_xyz(w120);
        if (len != 0)
            len = 1.0f / len;
        scale_vector_xyz(w130, w120, sy * len);
        add_vector_xyz(q2[0], r160, w130);
        subtract_vector_xyz(q2[1], r160, w130);
        add_vector_xyz(q2[2], r170, w130);
        subtract_vector_xyz(q2[3], r170, w130);
        if (i == 0) {
            u.c[1] = *(int *)&D_L00_00161684;
            u.c[0] = *(int *)&D_L00_00161684;
        } else if (i == n - 2) {
            u.c[3] = *(int *)&D_L00_00161684;
            u.c[2] = *(int *)&D_L00_00161684;
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
extern float D_L00_001616BC __attribute__((sda));
extern float D_L00_001616B8 __attribute__((sda));
extern int D_L00_001616C8 __attribute__((sda));
extern unsigned int D_L00_001616C0 __attribute__((sda));
extern unsigned int D_L00_001616C4 __attribute__((sda));
extern V_2bc1a0 D_L00_001DBB60[] __attribute__((section(".data")));
void FUN_L00_002bc1a0(void *mv) {
    char *m = mv;
    Q_2bc1a0 quad;
    V_2bc1a0 mat[4];
    V_2bc1a0 t;
    V_2bc1a0 d;
    V_2bc1a0 e;
    unsigned int c;
    int i;
    FUN_001f9a68_2bc1a0(D_L00_001616BC, &d, m + 0xD0);
    FUN_001f9a10_2bc1a0(&mat[3], &d, m + 0x10);
    quad.tex = FUN_001f44b8_2bc1a0(D_L00_001616C8);
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
        c = D_L00_001616C4;
    else
        c = D_L00_001616C0;
    c = (c & 0xFFFFFF) |
        (FUN_001fa6d0_2bc1a0(FUN_001fa6c0_2bc1a0((int)c >> 24) * D_L00_00161660_f[29]) << 24);
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    FUN_001f9a28_2bc1a0(&e, &(*(V_2bc1a0 *)g_debug_camera), &mat[3]);
    FUN_001f9bf8_2bc1a0(&mat[0], &e, 1.0f);
    FUN_001f9ad8_2bc1a0(&mat[1], &mat[0], &(*(V_2bc1a0 *)D_0013F5E0));
    FUN_001f9bf8_2bc1a0(&mat[1], &mat[1], -1.0f);
    FUN_001f9ad8_2bc1a0(&mat[2], &mat[1], &mat[0]);
    FUN_001f9a68_2bc1a0(0.2f, &e, &mat[0]);
    FUN_001f9a10_2bc1a0(&mat[3], &mat[3], &e);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2bc1a0(D_L00_001616B8, &t, &D_L00_001DBB60[3 - i]);
        FUN_001f9d20_2bc1a0(&quad.corner[3 - i], &t, &mat[0]);
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
extern float D_L00_001616D8 __attribute__((sda));
extern void add_vector_xyz_c4(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
float absolute_float(float input) __asm__("func_001F99C0");

char *FUN_L00_002bc3b0(char *self, float *position, float *heading, float *pitch) {
    float start[4] __attribute__((aligned(16)));
    float end[4] __attribute__((aligned(16)));
    float target[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    char *candidate;
    char *data;
    char *moby;
    int collision;
    int a0s[1];
    int res[1];
    float distance, angle, elevation, headingError, limit, apparentAngle, radius, reduction;
    int i;
    char *g;
    char *m0;

    limit = D_L00_001616DC;
    a0s[0] = (int)self;
    *(VecQuad *)start = *(VecQuad *)position;
    g = ((char *)&hero);
    m0 = *(char **)(g + 0x2080);
    *(VecQuad *)end = *(VecQuad *)(m0 + 0x10);
    position = start;
    end[2] = position[2];
    collision = collision_line((Vec4f *)end, (Vec4f *)position, 6, m0, 0);
    res[0] = 0;
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
            char *g1 = ((char *)&hero);
            moby = *(char **)(g1 + 0x2080);
            toward = fast_arc_tan_c(*(float *)(candidate + 0x10) - *(float *)(g1 + 0x80),
                                   *(float *)(candidate + 0x14) - *(float *)(g1 + 0x84));
            if (fast_difference_between_rotations_c(*(float *)(moby + 0x48), toward) < 1.0471976f &&
                absolute_float(position[2] - target[2]) < 2.0f &&
                !collision_line((Vec4f *)position, (Vec4f *)target, 6, *(char **)(g1 + 0x2080), 0)) {
                res[0] = (int)candidate;
                *pitch = -elevation;
                *heading = angle;
                goto done;
            }
        }
        if (distance > limit)
            continue;
        {
            char *g2 = ((char *)&hero);
            build_spherical_offset(offset, distance, *heading, *pitch + *(float *)(g2 + 0x2E4) * 0.5f);
        }
        add_vector_xyz_c4(offset, offset, position);
        apparentAngle = fast_arc_sin(fast_vec_dist(offset, target) / (distance + distance));
        apparentAngle += apparentAngle;
        if (D_L00_001616D8 * 0.017453292f < apparentAngle) {
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
        if (apparentAngle < D_L00_001616D8 * 0.017453292f) {
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
                res[0] = (int)candidate;
            }
        }
    } while ((candidate = active_mobies[++i]) != 0);
done:
    return (char *)res[0];
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
extern float D_L00_001616E0 __attribute__((sda));
extern int D_L00_001616EC __attribute__((sda));
extern int D_L00_001617BC;
extern int D_L00_001617C0;
extern int D_L00_001617C4;
extern int D_L00_001617C8;
extern int fast_dec_timer_s16() __asm__("FUN_001f9770");
extern int transpose_rotation_matrix() __asm__("FUN_001fa2d8");
extern int suck_get_aim_height() __asm__("FUN_002141f8");
extern int FUN_L00_002223f8();
extern int FUN_L00_00236e50();
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
int FUN_L00_0023e5e0(void *v, float a, float b, int col);
int voice_is_playing(int a, int i) __asm__("FUN_L00_0028d8c0");
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");
s32 remove_hud_item(s32 item) __asm__("FUN_001ff480");
struct Moby *FUN_L00_002c9348(void *a);
void FUN_L00_0020e4c8(void);
void FUN_L00_0020e698(void);
void FUN_L00_00236268(HudElem *e);
void FUN_L00_00236458(void);
void FUN_L00_0023e838(int i);
void FUN_L00_00263d40(int a, int b);
extern char D_0013A5E0_2BDAF0[] __asm__("D_0013CAE0");
extern char item_unlocked[] __asm__("D_0013D408");
extern unsigned char D_0013E633_2BDAF0[] __asm__("D_0013E533");
extern char voice_pool[] __asm__("D_0013E550");
extern char D_L00_001803C0_2BDAF0[] __asm__("D_L00_001802C0");
extern char D_L00_00166C80[];
extern float func_0020D830_2BDAF0(void *) __asm__("FUN_0020c9e0");
extern int func_001F9938_2BDAF0(void *) __asm__("FUN_001f9770");
extern void func_L00_0020EB60_2BDAF0(void) __asm__("FUN_L00_0020e4c8");
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
extern void func_L00_0023F1D0_2BDAF0(int) __asm__("FUN_L00_0023e838");
extern void func_L00_0028EBF0_2BDAF0(int) __asm__("FUN_0022d798");
extern void func_L00_0020ED30_2BDAF0(void) __asm__("FUN_L00_0020e698");
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
        D_L00_001617C0 = -1;
        D_L00_001617C8 = -1;
        D_L00_001617C4 = 0;
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
        if (((Hero *)((char *)&hero))->unk20A8 == 0) {
            func_L00_0020EB60_2BDAF0();
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
                float remaining = data->left - D_L00_001616E0 * D_0015ED6C * level_bonus;
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
        if (D_L00_001617C0 == -1) {
            D_L00_001617C0 = func_L00_0023EF78_2BDAF0((float *)&moby->pos, 7.5f, 0.0f, 0x207F7F);
        }
        if (D_L00_001617C0 >= 0) {
            D_L00_001617C4 = func_001F9850_2BDAF0(20);
            func_L00_001FF4B0_2BDAF0(&points[0], &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[1], data, &points[0]);
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].pos[0] = points[1].x;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].pos[1] = points[1].y;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].pos[2] = points[1].z;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].radius = 7.5f;
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
        if (D_L00_001617C0 >= 0) {
            float brightness;
            func_L00_001FF4B0_2BDAF0(&direction, &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[0], data, &direction);
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].pos[0] = points[0].x;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].pos[1] = points[0].y;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].pos[2] = points[0].z;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].radius = 7.5f;
            brightness = func_001FA888_2BDAF0(D_L00_001617C4) * 0.05f;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].col[0] = brightness;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].col[1] = brightness;
            ((Light_2BDAF0 *)D_L00_001803C0_2BDAF0)[D_L00_001617C0].col[2] = brightness * 0.25f;
            if (func_001F9908_2BDAF0(&D_L00_001617C4) != 0) {
                func_L00_0023F1D0_2BDAF0(D_L00_001617C0);
                D_L00_001617C0 = -1;
            }
        }
        if (D_L00_001617C0 >= 0 && D_L00_001617C4 == 0) {
            func_L00_0023F1D0_2BDAF0(D_L00_001617C0);
            D_L00_001617C0 = -1;
        }
        if (D_L00_001617C8 >= 0) {
            if (D_L00_001617C8 != -1 && ((Voice_2BDAF0 *)(voice_pool + D_L00_001617C8 * 0x70))->owner == moby
                && ((Voice_2BDAF0 *)(voice_pool + D_L00_001617C8 * 0x70))->on != 0) {
                func_L00_0028EBF0_2BDAF0(D_L00_001617C8);
            }
            D_L00_001617C8 = -1;
        }
        func_L00_0020ED30_2BDAF0();
    }
    if (busy && D_L00_001617C0 >= 0) {
        func_L00_0023F1D0_2BDAF0(D_L00_001617C0);
        D_L00_001617C0 = -1;
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

void FUN_L00_002bcf98(u8 *moby) {
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
unsigned char *FUN_L00_00272060(void *pos, int s, int color, int mode, int b, float *v, float x, float y, float z);
unsigned char *FUN_L00_00275910(void *pos, int s, int col, int mode, int b, float *vec, int h, float x, float y);
void FUN_L00_002bcf98(u8 *moby);
void FUN_L00_002bdc50_c(void *o) __asm__("FUN_L00_002bdc50");
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
extern void func_L00_002BEF58_2BE3A8(void *) __asm__("FUN_L00_002bdc50");
extern float func_002140F8_2BE3A8(float, float) __asm__("FUN_002132a8");
extern int func_002140B0_2BE3A8(int) __asm__("FUN_00213260");
extern int func_001F9850_2BE3A8(int) __asm__("FUN_001f96f8");
extern void func_L00_00272F00_2BE3A8(void *, int, int, int, int, void *, float, float, float) __asm__("FUN_L00_00272060");
extern float func_L00_00258C80_2BE3A8(float, float) __asm__("FUN_L00_00257c48");
extern int func_001FA898_2BE3A8(float) __asm__("FUN_001fa6d0");
extern void func_L00_002767B0_2BE3A8(void *, int, int, int, int, void *, struct Moby *, float, float) __asm__("FUN_L00_00275910");
extern int func_001F9908_2BE3A8(int *) __asm__("FUN_001f9740");

extern int D_L00_00161744 __attribute__((sda));
extern float D_L00_001617D0 __attribute__((section(".sdata")));
extern Tab_2BE3A8 D_L00_001E9AB0;
void FUN_L00_002bd0a0(struct Moby *moby) {
    Vec_2BE3A8 aim;
    Vec_2BE3A8 segment;
    Vec_2BE3A8 wave_offset;
    Vec_2BE3A8 beam_end;
    Vec_2BE3A8 point;
    Vec_2BE3A8 dir;
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
            func_001F9BF0_2BE3A8(&dir, &D_L00_001DC260_2BE3A8[i], &D_L00_001DC260_2BE3A8[i - 1]);
            func_001F9CA0_2BE3A8(&side, &dir, D_0013F5E0_r);
            func_L00_001FF4B0_2BE3A8(&side, &side, D_L00_001617D8_2BE3A8 + (D_L00_001617DC_2BE3A8 - D_L00_001617D8_2BE3A8) * progress);
            {
                Vec_2BE3A8 ring;
                Vec_2BE3A8 fwd;
                Vec_2BE3A8 out;
                Vec_2BE3A8 up;
                Vec_2BE3A8 node;

                for (ring_index = 0; ring_index < 20; ring_index++) {
                    func_002156E0_2BE3A8(&ring, &side, &dir, (float)ring_index * D_L00_001617D4_2BE3A8);
                    func_001F9BD8_2BE3A8(&D_L00_001DC1A0_2BE3A8[i][ring_index], &ring, &D_L00_001DC260_2BE3A8[i]);
                }
                qcopy(&node, &D_L00_001DC260_2BE3A8[i]);
                func_L00_001FF4B0_2BE3A8(&fwd, &dir, 1.0f);
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
    func_001F49B0_2BE3A8(func_L00_002BEF58_2BE3A8, moby);
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
    func_001F9BF0_2BE3A8(&dir, &D_L00_001DC260_2BE3A8[1], &D_L00_001DC260_2BE3A8[0]);
    func_L00_001FF4B0_2BE3A8(&dir, &dir, D_L00_00161804_2BE3A8 * frame_scale);
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
            func_002156E0_2BE3A8(&out, &dir, D_0013F5E0_r, func_L00_00258C80_2BE3A8(0.0f, D_L00_0016180C_2BE3A8));
            func_001F9CA0_2BE3A8(&up, &dir, D_0013F5E0_r);
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
        D_L00_00161824_macro = func_001F9850_2BE3A8(D_L00_00161744);
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
extern float D_L00_00161710 __attribute__((sda));
extern float D_L00_00161714 __attribute__((sda));
extern float D_L00_00161718 __attribute__((sda));
extern float D_L00_0016171C __attribute__((sda));
extern float D_L00_00161720 __attribute__((sda));
extern float D_L00_00161724 __attribute__((sda));
extern float D_L00_00161728 __attribute__((sda));
extern int D_L00_0016176C __attribute__((sda));
extern int D_L00_00161770 __attribute__((sda));
extern float D_L00_00161774 __attribute__((sda));
extern float D_L00_00161778 __attribute__((sda));
extern float D_L00_0016177C __attribute__((sda));
extern int D_L00_00161784 __attribute__((sda));
extern int D_L00_00161788 __attribute__((sda));
extern float D_L00_0016178C __attribute__((sda));
extern float D_L00_00161790 __attribute__((sda));
extern int D_L00_00161794 __attribute__((sda));
extern int D_L00_00161798 __attribute__((sda));
extern int D_L00_0016179C __attribute__((sda));
extern float g_debug_camera[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern OvlVec4 D_L00_001DC020[] __attribute__((section(".data")));
extern OvlVec4 D_L00_001DC0E0[] __attribute__((section(".data")));
extern OvlQuad D_L00_001DC060[][20] __attribute__((section(".data")));
extern OvlQuad D_L00_001DC1A0[][20] __attribute__((section(".data")));
void FUN_L00_002be078(void *, int, int, int, float, float, float, int);
void FUN_L00_002be650(void *);
void FUN_L00_002be890(void *);
long FUN_001f44b8(int);
void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
void fast_draw_quad_real(void *, void *, int) __asm__("FUN_001f7d30");
/* scrolls the two texture-offset accumulators and draws the 10x20 band of
   textured quads between the D_L00_001DC060/001DC1A0 rings, skipping
   back-facing ones */
void FUN_L00_002bdc50(void *o) {
    Quad quad;
    OvlVec4 d1, d2, n, e;
    float y, a, b, t;
    int c, i, j, k, off, jo;

    D_L00_0016178C = D_L00_0016178C - D_L00_00161790 * frame_scale;
    if (D_L00_0016178C <= -8.0f) D_L00_0016178C += 8.0f;
    FUN_L00_002be078(D_L00_001DC020, D_L00_0016176C, D_L00_00161770, 12, D_L00_00161774, D_L00_00161778, D_L00_0016177C, D_L00_00161784);
    FUN_L00_002be078(D_L00_001DC0E0, D_L00_0016176C, D_L00_00161770, 12, D_L00_00161774, D_L00_00161778, D_L00_0016177C, D_L00_00161788);
    FUN_L00_002be650(o);
    FUN_L00_002be890(o);
    k = D_0013E535;
    c = k ? D_L00_00161798 : D_L00_00161794;
    quad.tex = FUN_001f44b8(D_L00_0016179C);
    quad.unk80 = 0xFF9000000260L;
    quad.unk88 = 0x8000000048L;
    quad.unk70 = 0;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    y = D_L00_00161718;
    D_L00_00161718 = y - D_L00_0016171C * frame_scale;
    if (D_L00_00161718 <= -8.0f) D_L00_00161718 += 8.0f;
    t = D_L00_00161728 * frame_scale;
    D_L00_00161720 = D_L00_00161720 - t;
    if (D_L00_00161720 <= -7.0f) D_L00_00161720 += 7.0f;
    D_L00_00161724 = D_L00_00161724 + t;
    if (D_L00_00161724 >= 5.0f) D_L00_00161724 -= 5.0f;
    for (i = 2; i < 12; i++) {
        a = D_L00_00161720;
        b = D_L00_00161724;
        for (j = 0; j < 20; j++) {
            qcopy(&quad.corner[0], &D_L00_001DC1A0[i][j]);
            qcopy(&quad.corner[1], &D_L00_001DC1A0[i][(j + 1) % 20]);
            qcopy(&quad.corner[2], &D_L00_001DC060[i][j]);
            qcopy(&quad.corner[3], &D_L00_001DC060[i][(j + 1) % 20]);
            fast_vec_sub(&d1, &quad.corner[0], &quad.corner[1]);
            fast_vec_sub(&d2, &quad.corner[2], &quad.corner[1]);
            fast_vec_cross(&n, &d2, &d1);
            fast_vec_sub(&e, &quad.corner[1], &(*(OvlVec4 *)g_debug_camera));
            if (fast_vec_dot(&n, &e) >= 0.0f) {
                a += D_L00_00161714;
                b += D_L00_00161714;
            } else {
                if (i == 11) {
                    quad.color[0] = quad.color[1] = D_L00_00161794 & 0xFFFFFF;
                }
                quad.uv[5] = a;
                quad.uv[1] = a;
                quad.uv[3] = a + D_L00_00161714;
                quad.uv[4] = y - D_L00_00161710;
                quad.uv[0] = y;
                quad.uv[2] = y;
                quad.uv[7] = a + D_L00_00161714;
                quad.uv[6] = y - D_L00_00161710;
                a = a + D_L00_00161714;
                fast_draw_quad_real(&quad, 0, 0);
                quad.uv[0] = y;
                quad.uv[1] = b;
                quad.uv[2] = y;
                quad.uv[3] = b + D_L00_00161714;
                quad.uv[5] = b;
                quad.uv[4] = y - D_L00_00161710;
                quad.uv[6] = y - D_L00_00161710;
                quad.uv[7] = b + D_L00_00161714;
                b = b + D_L00_00161714;
                fast_draw_quad_real(&quad, 0, 0);
            }
        }
        y += D_L00_00161710;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002be078.s", FUN_L00_002be078);
#else
extern int D_L00_00161780 __attribute__((sda));
extern int D_L00_001617A4 __attribute__((sda));
extern int D_L00_001617A8 __attribute__((sda));
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF380). */
void FUN_L00_002be078(void *mv, int a, int b, int n, float sx, float sy, float sz, int col) {
    RW_2baae0 *m = mv;
    RW_2baae0 q[4];
    RT_2baae0 t;
    RW_2baae0 q2[4];
    RT_2baae0 u;
    RW_2baae0 w120, w130, w140, w150, r160, r170, a180, b190, s1A0, s1B0;
    int i, col0, col1, x;
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
    x = col | (b << 24);
    /* Fakematch: the zero terms ((unsigned char)v >> 8) make reload copy the colour
       through $v0 as retail does. */
    col |= (unsigned char)col >> 8;
    col1 = x | ((unsigned char)x >> 8);
    u.q2 = t.q2;
    u.q1 = t.q1;
    u.z4 = t.z4;
    u.c[3] = col1;
    u.c[2] = col1;
    u.c[1] = col1;
    u.c[0] = col1;
    u.f[0] = 0; u.f[1] = 0; u.f[2] = 0; u.f[3] = 1.0f; u.f[4] = 1.0f; u.f[5] = 0; u.f[6] = 1.0f; u.f[7] = 1.0f;
    t.f[0] = u.f[0] + D_L00_0016178C;
    t.f[1] = u.f[1];
    t.f[2] = u.f[2] + D_L00_0016178C;
    t.f[3] = u.f[3];
    t.f[4] = u.f[4] + D_L00_0016178C;
    t.f[5] = u.f[5];
    t.f[6] = u.f[6] + D_L00_0016178C;
    t.f[7] = u.f[7];
    fast_vec_sub(w140, m, g_debug_camera);
    fast_vec_sub(w150, &m[1], m);
    fast_vec_cross(w120, w150, w140);
    len0 = fast_vec_length(w120);
    if (len0 != 0) len0 = 1.0f / len0;
    fast_vec_scale(D_L00_00161774 * len0, w120, w120);
    fast_vec_add(q[0], m, w120);
    fast_vec_sub(q[1], m, w120);
    for (i = 1; i < n - 1; i++) {
        fast_vec_sub(w140, &m[i], g_debug_camera);
        fast_vec_sub(w150, &m[i + 1], &m[i]);
        fast_vec_cross(w120, w150, w140);
        len = fast_vec_length(w120);
        if (len != 0) len = 1.0f / len;
        fast_vec_scale(D_L00_00161774 * len, w120, w120);
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
        draw_2baae0(q, 0, 0);
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
        draw_2baae0(q2, 0, 0);
    }
    gsreg_2baae0(0x47, 0x5360B);
}
#endif
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF958). */
#include "rnc/rendering/geometry_quad.h"
extern int D_L00_00161760 __attribute__((sda));
extern int D_L00_00161764 __attribute__((sda));
extern float D_L00_001617E8_macro[3] __asm__("D_L00_001617E8") __attribute__((section(".sdata")));
extern float D_L00_00161808_macro[3] __asm__("D_L00_00161808") __attribute__((section(".sdata")));
extern int D_L00_00161818_macro[3] __asm__("D_L00_00161818") __attribute__((section(".sdata")));
extern OvlVec4 D_L00_001DBF20[] __attribute__((section(".data")));

void FUN_L00_002be650(void *unused) {
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
        int c = D_0013E535 != 0 ? D_L00_00161764 : D_L00_00161760;
        color = c & 0xFFFFFF;
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
extern float D_L00_001617B4 __attribute__((sda));
extern float D_L00_001617AC __attribute__((sda));
extern float D_L00_001617B0 __attribute__((sda));
extern int D_L00_001617B8 __attribute__((sda));
void FUN_L00_002be890(void *mv) {
    char *m = mv;
    Q_2be890 quad;
    V_2be890 mat[4];
    V_2be890 t;
    V_2be890 e;
    int c;
    int i, j;
    quad.tex = FUN_001f44b8_2be890(11);
    c = D_L00_001617B8;
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    quad.unk50 = 0;
    quad.unk54 = 0;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    quad.unk58 = 0;
    quad.unk5C = 1.0f;
    quad.unk60 = 1.0f;
    quad.unk64 = 0;
    quad.unk68 = 1.0f;
    quad.unk6C = 1.0f;
    FUN_001f9bf8_2be890(&e, m + 0xD0, D_L00_001617B4);
    FUN_001f9a10_2be890(&mat[3], &D_L00_001DBF60_2be890, &e);
    FUN_001f9a28_2be890(&e, &(*(V_2be890 *)g_debug_camera), &mat[3]);
    FUN_001f9bf8_2be890(&mat[0], &e, 1.0f);
    FUN_001f9ad8_2be890(&mat[1], &mat[0], &(*(V_2be890 *)D_0013F5E0));
    FUN_001f9bf8_2be890(&mat[1], &mat[1], -1.0f);
    FUN_001f9ad8_2be890(&mat[2], &mat[1], &mat[0]);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2be890(D_L00_001617AC, &t, &D_L00_001DBF20[3 - i]);
        FUN_001f9d20_2be890(&quad.corner[3 - i], &t, &mat[0]);
    }
    FUN_001f7d30_2be890(&quad, 0, 0);
    quad.color[3] = -1;
    quad.color[2] = -1;
    quad.color[1] = -1;
    quad.color[0] = -1;
    for (j = 3; j >= 0; j--) {
        FUN_001f9a68_2be890(D_L00_001617B0, &t, &D_L00_001DBF20[3 - j]);
        FUN_001f9d20_2be890(&quad.corner[3 - j], &t, &mat[0]);
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

int FUN_L00_002beab0(char *a, int b, Vx *c) {
    Vx v;
    float s;
    float t;
    int r;
    s = convert_integer_to_float((unsigned char)D_0013E533[1]) * 0.5f + 1.0f;
    if (c != 0) {
        qcopy(&v, c);
    } else {
        qcopy(&v, a + 0x10);
    }
    t = s * 0.2f;
    v.z = v.z + t;
    r = coll_sphere(s * 0.1999f, &v, b, a);
    if (r != 0) {
        float *g = (float *)D_L00_00173E40;
        g[14] -= t;
    }
    return r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFE98), where it is exact; names translated to the US level program. */


/* Saves the moby's position, word 0x4C and float 0x5C into ((char *)&hero) at 0x1ED0. */
void FUN_L00_002beb90(char *a) {
    struct Hero *g = &hero;
    g->unk1ED0 = *(float *)(a + 0x40);
    g->unk1ED4 = *(float *)(a + 0x44);
    g->unk1ED8 = *(float *)(a + 0x48);
    g->unk1F08 = *(int *)(a + 0x4C);
    g->unk1EE4 = *(float *)(a + 0x5C);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFED0), where it is exact; names translated to the US level program. */


/* Restores the moby's position, word 0x4C and float 0x5C from ((char *)&hero) at 0x1ED0. */
void FUN_L00_002bebc8(char *a) {
    struct Hero *g = &hero;
    *(float *)(a + 0x40) = g->unk1ED0;
    *(float *)(a + 0x44) = g->unk1ED4;
    *(float *)(a + 0x48) = g->unk1ED8;
    *(int *)(a + 0x4C) = g->unk1F08;
    *(float *)(a + 0x5C) = g->unk1EE4;
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFF08), where it is exact; names translated to the US level program. */

/* Saves moby fields 0x44..0x68 and its vector into ((char *)&hero) at 0x1F10..0x1F40. */
void FUN_L00_002bec00(char *a) {
    struct Hero *g = &hero;
    g->unk1F20 = *(float *)(a + 0x50);
    g->unk1F24 = *(float *)(a + 0x54);
    g->unk1F28 = *(float *)(a + 0x58);
    g->unk1F3F = *(char *)(a + 0x68);
    g->unk1F40 = *(float *)(a + 0x44);
    qcopy(&g->unk1F10, a);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFF50), where it is exact; names translated to the US level program. */


/* Copies fields from ((char *)&hero) at 0x1F20..0x1F40 into the moby. */
void FUN_L00_002bec48(char *a) {
    struct Hero *g = &hero;
    *(float *)(a + 0x50) = g->unk1F20;
    *(float *)(a + 0x54) = g->unk1F24;
    *(float *)(a + 0x58) = g->unk1F28;
    *(char *)(a + 0x68) = g->unk1F3F;
    *(float *)(a + 0x44) = g->unk1F40;
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
float FUN_L00_002bec80(Vec4_2bec80 *p, void *q) {
    Vec4_2bec80 a, b;
    float f, t;
    qcopy(&a, p);
    qcopy(&b, p);
    t = a.f[2] - 5.0f;
    if (t < 0.1f)
        f = 0.1f;
    else
        f = t;
    a.f[2] = f;
    b.f[2] = f + 6.0f;
    if (FUN_001efa68_002bec80(&b, &a, 6, q, 0)) {
        char *o = (*(S173E40_2bec80 *)D_L00_00173E40).o18;
        if (o && *(char **)(o + 0x24) && *(short *)(*(char **)(o + 0x24) + 0x46) == 5) {
            return (*(S173E40_2bec80 *)D_L00_00173E40).f28;
        }
        switch (FUN_001f0b58_002bec80()) {
        case 0:
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

char *FUN_L00_002bed90(char *self, float *position, float *angles, int unused, int mode) {
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

f32 FUN_L00_002bf050(Mob *m, VU *pos, f32 *tgt, f32 *out, f32 ang, f32 arc, f32 step, f32 h, f32 r2,
                     f32 r1) {
    VU probe_start;
    VU probe_end;
    f32 da;
    s32 n;
    s32 i;
    f32 best;
    f32 bestDist;
    f32 bestDiff;
    f32 cur;
    f32 a;
    f32 h2;
    f32 d;
    f32 diff;
    Ext *x;
    Hit *t;

    da = arc / step;
    best = ang;
    n = FUN_001fa6d0_c(step);
    bestDist = 0.0f;
    x = m->p78;
    bestDiff = 6.2831855f;
    *out = bestDist;
    if (tgt != 0) {
        cur = FUN_001f9e90_c(tgt[0] - m->f10, tgt[1] - m->f14);
    } else {
        cur = ang;
    }
    arc *= 0.5f;
    fast_add_rotations(ang, arc);
    arc = fast_subtract_rotations(ang, arc);

    qcopy(&probe_start, pos);
    probe_start.f[1] += fast_sin(cur) * r1;
    probe_start.f[0] += fast_cos(cur) * r1;
    probe_start.f[2] += h;
    qcopy(&probe_end, pos);
    probe_end.f[1] += fast_sin(cur) * r2;
    probe_end.f[0] += fast_cos(cur) * r2;
    probe_end.f[2] += h + h;
    if (FUN_001efa68_c(&probe_start, &probe_end, 4, m, 0) == 0) {
        if (FUN_L00_002bec80_c(&probe_end, m) != 0.0f) {
            best = cur;
            bestDist = r2;
            *out = r2;
        }
    } else {
        if (D_L00_00173E58_bf == x->p38 || (D_L00_00173E58_bf != 0 && D_L00_00173E58_bf->p24 != 0 &&
                                            D_L00_00173E58_bf->p24->h46 == 5)) {
            *out = 100.0f;
            return cur;
        }
        best = cur;
        bestDist = fast_vec_dist(&probe_start, &D_L00_00173E60);
        bestDiff = 0.0f;
        if (*out < bestDist) {
            *out = bestDist;
        }
    }

    for (i = 0; i < n; i++) {
        qcopy(&probe_start, pos);
        probe_start.f[1] += fast_sin(arc) * r1;
        probe_start.f[0] += fast_cos(arc) * r1;
        probe_start.f[2] += h;
        qcopy(&probe_end, pos);
        probe_end.f[1] += fast_sin(arc) * r2;
        probe_end.f[0] += fast_cos(arc) * r2;
        probe_end.f[2] += h + h;
        if (FUN_001efa68_c(&probe_start, &probe_end, 4, m, 0) == 0) {
            if (FUN_L00_002bec80_c(&probe_end, m) != 0.0f) {
                diff = fast_difference_between_rotations(arc, cur);
                if (diff < bestDiff || bestDist < r2) {
                    bestDiff = diff;
                    best = arc;
                    bestDist = r2;
                    if (*out < r2) {
                        *out = r2;
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
            d = fast_vec_dist(&probe_start, &D_L00_00173E60);
            if (bestDist < d) {
                best = arc;
                bestDist = d;
                bestDiff = fast_difference_between_rotations(best, cur);
                if (*out < d) {
                    *out = d;
                }
            } else if (d == bestDist) {
                diff = fast_difference_between_rotations(arc, cur);
                if (diff < bestDiff) {
                    bestDiff = diff;
                    best = arc;
                    bestDist = d;
                    if (*out < d) {
                        *out = d;
                    }
                }
            }
        }
        arc = fast_add_rotations(arc, da);
    }

    ang = fast_subtract_rotations(best, 1.5707964f);
    qcopy(&probe_start, pos);
    probe_start.f[1] += fast_sin(ang) * r1;
    probe_start.f[0] += fast_cos(ang) * r1;
    probe_start.f[2] += h * 0.125f;
    qcopy(&probe_end, &probe_start);
    probe_end.f[1] += fast_sin(best) * r2;
    probe_end.f[0] += fast_cos(best) * r2;
    probe_end.f[2] += 0.12f;
    if (FUN_001efa68_c(&probe_start, &probe_end, 4, m, 0) != 0) {
        if (D_L00_00173E58_bf != x->p38 &&
            !(D_L00_00173E58_bf != 0 && D_L00_00173E58_bf->p24 != 0 &&
              D_L00_00173E58_bf->p24->h46 == 5)) {
            d = fast_vec_dist(&probe_start, &D_L00_00173E60);
            best = fast_add_rotations(best, (r2 - d) * 3.1415927f / (r2 * 3.0f));
        }
    }
    ang = fast_add_rotations(ang, 3.1415927f);
    qcopy(&probe_start, pos);
    probe_start.f[1] += fast_sin(ang) * r1;
    probe_start.f[0] += fast_cos(ang) * r1;
    probe_start.f[2] += h * 0.125f;
    qcopy(&probe_end, &probe_start);
    probe_end.f[1] += fast_sin(best) * r2;
    probe_end.f[0] += fast_cos(best) * r2;
    probe_end.f[2] += 0.12f;
    if (FUN_001efa68_c(&probe_start, &probe_end, 4, m, 0) != 0) {
        if (D_L00_00173E58_bf != x->p38 &&
            !(D_L00_00173E58_bf != 0 && D_L00_00173E58_bf->p24 != 0 &&
              D_L00_00173E58_bf->p24->h46 == 5)) {
            d = fast_vec_dist(&probe_start, &D_L00_00173E60);
            best = fast_subtract_rotations(best, (r2 - d) * 3.1415927f / (r2 * 3.0f));
        }
    }
    if (2.5132742f < fast_difference_between_rotations(best, m->f48)) {
        best = fast_add_rotations(m->f48, fast_subtract_rotations(best, m->f48) * 0.5f);
    }
    return best;
}
/* Steps a vector by n repeated additions of the normalised difference. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002C0B18), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9a68(void *, void *, float);

void FUN_L00_002bf810(char *a, char *b, int n) {
    float d[4];
    float t[4];
    int i;
    fast_vec_sub(d, b, a);
    FUN_001f9a68(d, d, 1.0f / convert_integer_to_float(n));
    qcopy(t, a);
    for (i = 0; i < n; i++) {
        fast_vec_add(t, t, d);
    }
}
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f99c0(float);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025abf0(void *, void *, int, float, float);

void FUN_L00_002bf8b0(unsigned char *m, unsigned char *o, u128 *src) {
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
float FUN_L00_00259468(float *a, float *b, int *out, float f);
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

int FUN_L00_002bf9f0(Mob_2C0CF8 *moby, Blk_2C0CF8 *block, void *target, int check_line) {
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
            return FUN_L00_002bf9f0(moby, block, half, 1);
        }
        return 0;
    }
    if (moby->f2C * 3.0f / moby->p24->unk24 < jump[2]) {
        if (can_halve) {
            return FUN_L00_002bf9f0(moby, block, half, 1);
        }
        return 0;
    }
    qcopy(mid, D_L00_00173F80_2C0CF8);
    func_L00_001FF4B0_2C0CF8(mid, mid, 1.0f);
    sum = func_001F9B88_2C0CF8(mid[0]);
    sum = sum + func_001F9B88_2C0CF8(mid[1]);
    if (func_001F9B88_2C0CF8(mid[2]) < sum) {
        if (can_halve) {
            return FUN_L00_002bf9f0(moby, block, half, 1);
        }
        return 0;
    }
    land[2] = land[2] + moby->f2C * 0.025f / moby->p24->unk24;
    if (func_L00_002BFDB8_2C0CF8(moby, 6, land)) {
        if (can_halve) {
            return FUN_L00_002bf9f0(moby, block, half, 1);
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
                return FUN_L00_002bf9f0(moby, block, half, 1);
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
            return FUN_L00_002bf9f0(moby, block, half, 1);
        }
        return 0;
    }
    if (func_L00_001EFFF0_2C0CF8(mid, to, 6, moby, 0) != 0 && D_L00_00173F58_2C0CF8[0] != block->p38) {
        if (can_halve) {
            return FUN_L00_002bf9f0(moby, block, half, 1);
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
void FUN_L00_0025d1b8(unsigned char *);
unsigned char *FUN_L00_002bfe08(int a, u128 *b, u128 *c) {
    unsigned char *m = create_moby(0xba);
    if (m) {
        char *q;
        m[0x30] = 0xff;
        *(short *)(m + 0x32) = 0xff;
        m[0x31] = 1;
        q = *(char **)(m + 0x78);
        m[0x20] = 0;
        m[0xbc] = 0;
        qcopy(m + 0x10, b);
        qcopy(q + 0x10, c);
        *(int *)(q + 0x60) = a;
        FUN_L00_00250df8(m);
        FUN_L00_0025d1b8(m);
        *(float *)(m + 0x2c) = *(float *)(*(char **)(m + 0x24) + 0x24) * 0.1f;
    }
    return m;
}
#include "rnc/math/vector.h"
extern char *D_L00_00173E58 __attribute__((section(".data")));
int FUN_001efa68(Vec4 *, Vec4 *, int, void *, int);
unsigned int collision_hit_material(void) __asm__("FUN_001f0b58");
int FUN_L00_002bfec8(unsigned char *p) {
    Vec4 a, b;
    int r = p[0x20];
    qcopy(&a, p + 0x10);
    qcopy(&b, p + 0x10);
    a.f[2] -= 0.3f;
    b.f[2] += 1.0f;
    if (FUN_001efa68(&b, &a, 6, p, 0)) {
        char *o = D_L00_00173E58;
        if (o && *(char **)(o + 0x24) && *(short *)(*(char **)(o + 0x24) + 0x46) == 5) {
            r = 11;
        } else {
            switch (collision_hit_material()) {
            case 0:
            case 1:
            case 3:
            case 8:
            case 11:
            case 12:
            case 13:
                r = 11;
                break;
            }
        }
    }
    return r;
}
