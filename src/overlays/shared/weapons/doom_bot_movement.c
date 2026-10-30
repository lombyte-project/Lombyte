/* Doom bot movement helpers: collision and landing probes, heading and target selection, the hop trajectory, and saving and restoring the hero state around them. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/overlay/collision.h"
#include "qzero.h"

typedef struct {
    float x, y, z, w;
} Vx __attribute__((aligned(16)));

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

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern float fast_vec_dist(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
float        absolute_float(float input) __asm__("FUN_001f99c0");
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
int FUN_001efa68_002bec80(Vec4_2bec80 *, Vec4_2bec80 *, int, void *, int) __asm__("FUN_001efa68");
extern float FUN_001f99c0(float);
extern int   FUN_001efa68_2bed90(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern s32   FUN_001fa6d0_c(f32) __asm__("FUN_001fa6d0");
extern f32   FUN_001f9e90_c(f32, f32) __asm__("FUN_001f9e90");
extern s32   FUN_001efa68_c(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void  FUN_001f9a68(void *, void *, float);
extern float FUN_L00_0025abf0(void *, void *, int, float, float);
float        calculate_triangular_corrected_scalar(float *a,
                                                   float *b,
                                                   int   *out,
                                                   float  f) __asm__("FUN_L00_00259468");
extern void  func_001F9BF0_2C0CF8(void *, void *, void *) __asm__("FUN_001f9a28");
extern int   func_L00_001FF5B0_2C0CF8(float, void *, void *) __asm__("FUN_L00_001ff600");
extern void  func_001F9BD8_2C0CF8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  func_001F9C30_2C0CF8(void *, void *, float) __asm__("FUN_001f9a68");
extern float func_001F9CE8_2C0CF8(void *) __asm__("FUN_001f9b20");
extern void  func_L00_001FF4B0_2C0CF8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  func_L00_002C0B18_2C0CF8(
     void *a, void *b, int n, int c, int d, int e, float g, int f) __asm__("FUN_L00_002bf810");
extern int func_L00_001EFFF0_2C0CF8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
int        FUN_001efa68(Vec4 *, Vec4 *, int, void *, int);

extern char  D_0013E533[];
extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern int   D_L00_00173E40[];
extern int   D_L00_00173E40[] NOT_SDA;
extern char  D_0013F3D0[] NOT_SDA;
extern VU    D_L00_00173E60;
extern char  D_0013E633_2C0CF8[] __asm__("D_00141260");

/* Nudge a position and ask the level helper about it, lowering a global on success. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BFDB8), where it is exact; names translated to the US level program. */
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

/* Tries a hop of the moby toward q: probes the ground at the landing point and the path to it, and sets the jump
 * state on success. When blocked and the hop is long enough it tries again with half the distance. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002C0CF8), where it is exact; names translated to the US level program. */
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
