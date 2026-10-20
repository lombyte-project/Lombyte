/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00258F28), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");

/* Cosine-eased rotation from x toward y at z. */
float FUN_L00_00257ef0(float x, float y, float z) {
    float wrapped_angle_delta = fast_subtract_rotations(y, x);
    float phase_cosine = fast_cos(z * 3.1415927f);
    return fast_add_rotations(x, wrapped_angle_delta * ((1.0f - phase_cosine) * 0.5f));
}
#include "qcopy.h"
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

/* Vector cosine interpolation: dst = a + (b - a) * ((1 - cos(t * pi)) * 0.5).
   Scalar analogue matched at src/game/mobyutil.c:func_00214220. */
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00258FA8), where it is exact; names translated to the US level program. */

void FUN_L00_00257f70(void *dst, void *start_vector, void *b, float interpolation_fraction) {
    float interpolation_delta[4];
    float ease;

    if (interpolation_fraction == 0.0f) {
        qcopy(dst, start_vector);
        return;
    }
    if (interpolation_fraction == 1.0f) {
        qcopy(dst, b);
        return;
    }
    ease = (1.0f - fast_cos(interpolation_fraction * 3.14159274f)) * 0.5f;
    subtract_vector_xyz(interpolation_delta, b, start_vector);
    scale_vector_xyz(interpolation_delta, interpolation_delta, ease);
    add_vector_xyz(dst, start_vector, interpolation_delta);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00259088), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");

/* Blends three angles from a toward b with a cosine ease; ang 0 gives a, 1 gives b. */
void FUN_L00_00258050(float *out, float *start_rotations, float *b, float ang) {
    float cosine_ease_weight = (1.0f - fast_cos(ang * 3.1415927f)) * 0.5f;
    out[0] = fast_add_rotations(start_rotations[0], fast_subtract_rotations(b[0], start_rotations[0]) * cosine_ease_weight);
    out[1] = fast_add_rotations(start_rotations[1], fast_subtract_rotations(b[1], start_rotations[1]) * cosine_ease_weight);
    out[2] = fast_add_rotations(start_rotations[2], fast_subtract_rotations(b[2], start_rotations[2]) * cosine_ease_weight);
}
float FUN_001fa5c8_00258110(float, float) __asm__("FUN_001fa5c8");
float FUN_001f99c0_00258110(float) __asm__("FUN_001f99c0");
float FUN_001fa580_00258110(float, float) __asm__("FUN_001fa580");
float spring_turn(float *p, float a, float b, float c, float d, float lim) __asm__("FUN_L00_00258110");
float spring_turn(float *turn_step, float current_angle, float b, float spring_strength, float damping_factor, float lim) {
    float angle_delta = FUN_001fa5c8_00258110(b, current_angle);
    float k = angle_delta / 0.15707964f;
    if (k > 1.0f)
        k = 1.0f;
    else if (k < -1.0f)
        k = -1.0f;
    *turn_step = *turn_step + (spring_strength * k - damping_factor * *turn_step);
    if (lim != 0.0f) {
        if (*turn_step > lim)
            *turn_step = lim;
        else if (*turn_step < -lim)
            *turn_step = -lim;
    }
    if (*turn_step > FUN_001f99c0_00258110(angle_delta))
        *turn_step = FUN_001f99c0_00258110(angle_delta);
    else if (*turn_step < -FUN_001f99c0_00258110(angle_delta))
        *turn_step = -FUN_001f99c0_00258110(angle_delta);
    return FUN_001fa580_00258110(current_angle, *turn_step);
}
extern float FUN_001f99c0(float);

void FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void FUN_L00_00258278(unsigned char *moby, float *ptr, float target_heading, float b, float damping_factor, float limit) {
    float r = fast_subtract_rotations(target_heading, *(float *)(moby + 0x48));
    float k = r / 0.15707964f;
    float v;
    if (1.0f < k)
        k = 1.0f;
    else if (k < -1.0f)
        k = -1.0f;
    v = *ptr + (b * k - damping_factor * (*ptr));
    *ptr = v;
    if (limit != 0.0f) {
        if (limit < v) {
            *ptr = limit;
        } else if (v < -limit) {
            *ptr = -limit;
        }
    }
    if (FUN_001f99c0(r) < *ptr) {
        *ptr = FUN_001f99c0(r);
    } else if (*ptr < -FUN_001f99c0(r)) {
        *ptr = -FUN_001f99c0(r);
    }
    *(float *)(moby + 0x48) = fast_add_rotations(*(float *)(moby + 0x48), *ptr);
}
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
typedef union {
    u128 q;
    f32 f[4];
} V002583f0;
typedef struct {
    u8 pad[0x18];
    s32 x18;
    s32 x1c;
} G002583f0;
extern G002583f0 D_L00_00173E40_u __asm__("D_L00_00173E40");
s32 collision_line(void *, void *, s32, s32, s32) __asm__("FUN_001efa68");
s32 FUN_L00_002583f0(void *pos, s32 moby, s32 flag, f32 up, f32 down) {
    V002583f0 raised_pos, lowered_pos;
    s32 r;
    qcopy(&raised_pos, pos);
    raised_pos.f[2] += up;
    qcopy(&lowered_pos, pos);
    lowered_pos.f[2] -= down;
    if (lowered_pos.f[2] < 0.0f)
        lowered_pos.f[2] = 0.0f;
    r = collision_line(&raised_pos, &lowered_pos, 0x22, moby, 0);
    if (D_L00_00173E40_u.x1c < 0)
        r = 0;
    if (flag && D_L00_00173E40_u.x18 != 0)
        r = 0;
    return r;
}
typedef union {
    OvlQuad q;
    float f[4];
} V_258490;
extern unsigned char D_L00_00173E40_258490[] __asm__("D_L00_00173E40")
    __attribute__((section(".data")));
extern V_258490 D_L00_00173E60_258490 __asm__("D_L00_00173E60") __attribute__((section(".data")));
extern V_258490 D_L00_00173E70_258490 __asm__("D_L00_00173E70") __attribute__((section(".data")));
extern float FUN_L00_00259710_258490(void *) __asm__("FUN_L00_00259710");
extern float FUN_001f99c0_258490(float) __asm__("FUN_001f99c0");
extern void FUN_001f9a28_258490(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9a10_258490(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a68_258490(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_001f9bf8_258490(void *, void *, float) __asm__("FUN_001f9bf8");
extern int FUN_001efa68_258490(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern float FUN_001f9b20_258490(void *) __asm__("FUN_001f9b20");
extern float FUN_001f9b80_258490(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_258490(float, float) __asm__("FUN_001f9e90");
extern void FUN_00125180_258490(void *, void *) __asm__("FUN_00125180");
extern int FUN_L00_001f0d60_258490(void *, float, int, void *) __asm__("FUN_L00_001f0d60");

int FUN_L00_00258490(void *ign, V_258490 *to, V_258490 *pos, int flags, float h, float r, float lim,
                     float slope) {
    V_258490 d;
    V_258490 n;
    V_258490 a;
    V_258490 b;
    V_258490 up;
    V_258490 t;
    int ok;
    int i;
    int m2;
    float gz;
    float zz;
    float ang;
    unsigned char *c;
    float *nrm;
    unsigned char *q60;
    unsigned char *q40;

    ok = 1;
    m2 = flags & 2;
    pos->f[2] += h;
    gz = FUN_L00_00259710_258490(pos);
    pos->f[2] -= h;
    if (lim < FUN_001f99c0_258490(gz - to->f[2])) {
        if ((((flags ^ 1) & 1) && gz < to->f[2]) || to->f[2] < gz) {
            qcopy(pos, to);
            ok = 0;
        }
    }
    FUN_001f9a28_258490(&d, pos, to);
    FUN_001f9bf8_258490(&n, &d, r * 1.2f);
    up.q = 0;
    up.f[2] = h;
    FUN_001f9a10_258490(&a, to, &up);
    FUN_001f9a10_258490(&b, &a, &n);
    if (ok && FUN_001efa68_258490(&a, &b, m2 | 0x24, ign, 0)) {
        c = D_L00_00173E40_258490;
        if (*(int *)(c + 0x1C) > 0) {
            nrm = (float *)(c + 0x40);
            if (slope <= FUN_001f9e90_258490(*(float *)(c + 0x48), FUN_001f9b20_258490(nrm))) {
                *(int *)(c + 0x48) = 0;
                FUN_00125180_258490(nrm, nrm);
                {
                    float k = -d.f[0] * *(float *)(c + 0x40) - d.f[1] * *(float *)(c + 0x44);
                    if (0.0f < k) {
                        FUN_001f9a68_258490(nrm, nrm, k);
                        ok = 0;
                        FUN_001f9a10_258490(&d, &d, nrm);
                    }
                }
            }
            FUN_001f9a10_258490(pos, to, &d);
        }
    }
    if (!(flags & 2)) {
        zz = h + r;
        q60 = (unsigned char *)&D_L00_00173E60_258490;
        q40 = q60 - 0x20;
        i = 0;
        do {
            qcopy(&t, pos);
            t.f[2] += zz;
            if (!FUN_L00_001f0d60_258490(&t, r, 0x24, ign)) {
                break;
            }
            ang = FUN_001f9e90_258490(FUN_001f9b80_258490(pos, q60),
                                      *(float *)(q40 + 0x28) - pos->f[2]);
            if (*(int *)(q40 + 0x18) != 0 || slope < ang) {
                qcopy(pos, &D_L00_00173E70_258490);
                ok = 0;
                pos->f[2] -= zz;
            }
            i++;
        } while (i < 6);
    }
    pos->f[2] += h;
    gz = FUN_L00_00259710_258490(pos);
    pos->f[2] -= h;
    if (lim < FUN_001f99c0_258490(gz - to->f[2])) {
        if ((((flags ^ 1) & 1) && gz < to->f[2]) || to->f[2] < gz) {
            qcopy(pos, to);
            ok = 0;
        }
    }
    to->f[0] = pos->f[0];
    to->f[1] = pos->f[1];
    return ok;
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00259868), where it is exact; names translated to the US level program. */

typedef struct {
    float a[4];
} Vs __attribute__((aligned(16)));


typedef union {
    OvlQuad q;
    float f[4];
} V_94C8;

extern V_94C8 D_L00_00173E70;
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float vector_length_xy(void *);
extern float vector_length_xyz(void *);
extern int D_L00_00173E40[];
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int coll_sphere() __asm__("FUN_L00_001f0d60");
extern void add_vector_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
extern V_94C8 D_94C8_60 __asm__("D_L00_00173E60");
extern int func_L00_001F10E0_l(void *, float, int, void *) __asm__("FUN_L00_001f0d60");

int FUN_L00_00258830_c(char *m, char *v, int flags, float a, float b,
                       float c) __asm__("FUN_L00_00258830");
int FUN_L00_00258830_c(char *m, char *v, int flags, float a, float b, float ground_probe_offset) {
    Vs original_position;
    Vs raised_position;
    Vs probe_end;
    Vs scaled_displacement;
    int r = 0;
    int fl = (flags * 2) & 0x20;
    char *movement_position = m + 0x10;
    int i;
    char *collision_position;

    qcopy(&original_position, movement_position);
    add_vector_xyz(movement_position, movement_position, v);
    if (b < vector_length_xyz(v)) {
        qcopy(&raised_position, &original_position);
        raised_position.a[2] += a;
        normalize_vector_xyz(&scaled_displacement, v, b);
        add_vector_xyz(&probe_end, &scaled_displacement, movement_position);
        probe_end.a[2] += a;
        if (collision_line(&raised_position, &probe_end, fl, (int)m, 0)) {
            subtract_vector_xyz(movement_position, (char *)&D_94C8_60, &scaled_displacement);
            r = 1;
            *(float *)(m + 0x18) -= a;
        }
    }
    for (i = 0, collision_position = m + 0x10; i < 6; i++) {
        qcopy(&raised_position, collision_position);
        raised_position.a[2] += a;
        if (func_L00_001F10E0_l(&raised_position, b, fl | 4, m)) {
            qcopy(collision_position, &D_L00_00173E70);
            r |= 1;
            *(float *)(m + 0x18) -= a;
        } else {
            break;
        }
    }
    b = *(float *)(m + 0x18) - ground_probe_offset;
    qcopy(&raised_position, collision_position);
    qcopy(&probe_end, collision_position);
    raised_position.a[2] += a;
    probe_end.a[2] = b - 0.05f;
    if (collision_line(&raised_position, &probe_end, fl | 2, (int)m, 0)) {
        if (D_L00_00173E40[7] > 0) {
            float g;
            r |= 2;
            g = fast_arc_tan(*(float *)((char *)D_L00_00173E40 + 0x48),
                             vector_length_xy((char *)D_L00_00173E40 + 0x40));
            if (g > 0.5f) {
                r |= 4;
            }
            g = *(float *)((char *)D_L00_00173E40 + 0x28);
            if (b < g) {
                *(float *)(m + 0x18) += (g - b) * 0.3f;
            }
        }
    }
    subtract_vector_xyz(v, collision_position, &original_position);
    return r;
}
float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
void FUN_L00_00258830(int, int, float, float, float, int);
/* Calls FUN_L00_00258830 with c converted to float and divided by 1024. */
void FUN_L00_00258ad0(int moby, int b, float x, float y, int collision_radius_fixed, int collision_flags) {
    FUN_L00_00258830(moby, b, x, convert_integer_to_float(collision_radius_fixed) * (1.0f / 1024.0f), y, collision_flags);
}
#include "rnc/math/vector.h"

typedef struct {
    s32 i00;
    f32 f04;
    f32 f08;
    f32 f0C;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
    f32 f30;
    f32 f34;
    s32 i38;
    u8 pad3C[0x4C - 0x3C];
    f32 f4C;
} Ctl;

typedef struct {
    u8 pad00[0x10];
    Vec4 pos;
    u8 pad20[0x48 - 0x20];
    f32 f48;
} Obj;

typedef struct {
    u8 pad00[0x28];
    f32 f28;
    u8 pad2C[0x40 - 0x2C];
    Vec4 v40;
    f32 f48;
} Glob;

extern Glob D_L00_00173E40_c __asm__("D_L00_00173E40");

extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 FUN_001f9dc8_c(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern s32 FUN_L00_002583f0_c(void *, void *, s32, f32, f32) __asm__("FUN_L00_002583f0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void FUN_001f9a68_c(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32 FUN_L00_00258ad0_c(void *, void *, f32, s32, f32, s32) __asm__("FUN_L00_00258ad0");
extern f32 probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");

extern f32 frame_time_sq_gp __asm__("D_0015ED70") __attribute__((sda));
extern f32 frame_scale __asm__("D_0015ED60") __attribute__((sda));
s32 FUN_L00_00258b50(Obj *m, Ctl *c, void *v, Vec4 *out, f32 f);
s32 FUN_L00_00258b50(Obj *m, Ctl *c, void *v, Vec4 *out, f32 f) {
    Vec4 save;
    Vec4 displacement;
    Vec4 p;
    Vec4 q;
    Glob *g;
    s32 mask = 0;
    s32 r;
    f32 len;
    f32 sp;
    f32 lim;
    f32 speed_increment;
    f32 h1;
    f32 h2;
    f32 ground_height;
    f32 normal_elevation_angle;
    f32 l2;

    sp = c->f1C * f;
    lim = c->f24 * f;
    fast_vec_sub(&displacement, v, &m->pos);
    len = vector_length_xy(&displacement);
    save.q = m->pos.q;
    speed_increment = sp * len;
    if (sp < speed_increment)
        speed_increment = sp;
    c->f10 = c->f10 + speed_increment;
    if (len < c->f10 * 10.0f)
        c->f10 = c->f10 - c->f20 * c->f10;
    if (lim < c->f10)
        c->f10 = lim;
    else if (c->f10 < -lim)
        c->f10 = -lim;
    if (c->i38 & 4) {
        displacement.f[0] = FUN_001f9dc8_c(c->f4C) * c->f10;
        displacement.f[1] = fast_sin(c->f4C) * c->f10;
    } else {
        displacement.f[0] = FUN_001f9dc8_c(m->f48) * c->f10;
        displacement.f[1] = fast_sin(m->f48) * c->f10;
    }
    if (c->i38 & 1) {
        displacement.f[2] = 0.0f;
    } else {
        displacement.f[2] = c->f18;
        c->f18 = c->f18 - frame_time_sq_gp * 9.0f;
    }
    lim = m->pos.f[2] + 0.04f;
    fast_vec_add(&p, &m->pos, &displacement);
    p.f[2] = m->pos.f[2] - c->f30;
    if (FUN_L00_002583f0_c(&p, m, c->i38 & 0x20, c->f08, c->f0C)) {
        g = &D_L00_00173E40_c;
        normal_elevation_angle = fast_arc_tan(g->v40.f[2], vector_length_xy(&g->v40));
        h1 = g->f28;
        if (0.5f < normal_elevation_angle && 0.1f < vector_length_xy(&displacement)) {
            {
                fast_vec_normalize(&q, &displacement, 0.57f);
                fast_vec_add(&q, &q, &m->pos);
                q.f[2] = m->pos.f[2] - c->f30;
                if (FUN_L00_002583f0_c(&q, m, c->i38 & 0x20, c->f08, c->f0C)) {
                    h2 = g->f28;
                    if (!(0.5f <= fast_arc_tan(g->v40.f[2], vector_length_xy(&g->v40))))
                        goto done;
                    mask = 0x22;
                    if (!(c->i38 & 8))
                        goto done;
                    if (!(h2 < lim))
                        goto done;
                } else {
                    mask = 0x22;
                    if (!(c->i38 & 8))
                        goto done;
                }
                if (h1 < lim)
                    mask = 8;
            }
        }
    } else {
        mask = (c->i38 & 8) ? 8 : 0x22;
    }
done:
    if (!(mask & 2)) {
        l2 = vector_length_xy(&displacement);
        FUN_001f9a68_c(&p, &displacement, 1.0f / l2);
        r = FUN_L00_00258ad0_c(m, &displacement, c->f04, c->i00, c->f30, c->i38);
        if (r & 2)
            c->f18 = frame_scale * -0.01f;
        if (r & 1) {
            mask |= 1;
            if (displacement.f[0] * p.f[0] + displacement.f[1] * p.f[1] < l2 * 0.2f)
                mask |= 2;
            ground_height = probe_ground_height(&m->pos, 0, 0.5f);
            if (c->f08 < save.f[2] - ground_height || c->f0C < ground_height - save.f[2]) {
                mask |= 0x22;
                m->pos.q = save.q;
            }
        }
        qcopy(out, &displacement);
    } else {
        if ((c->i38 ^ 1) & 1)
            c->f18 = frame_scale * -0.01f;
    }
    if (len < c->f2C)
        mask |= 4;
    return mask;
}
void FUN_001f99f8_259028(void *) __asm__("FUN_001f99f8");
float FUN_001f9b48_259028(void *, void *) __asm__("FUN_001f9b48");
float FUN_001f9e90_259028(float, float) __asm__("FUN_001f9e90");
void FUN_L00_00258278_259028(unsigned char *, float *, float, float, float,
                             float) __asm__("FUN_L00_00258278");
float FUN_001fa688_259028(float, float) __asm__("FUN_001fa688");
int FUN_001f9740_259028(void *) __asm__("FUN_001f9740");
int FUN_001f96f8_259028(int) __asm__("FUN_001f96f8");
int FUN_L00_00258b50_259028(unsigned char *, char *, float *, void *,
                            float) __asm__("FUN_L00_00258b50");
int FUN_L00_00259028(unsigned char *m, char *c, float *tgt, void *out);
int FUN_L00_00259028(unsigned char *m, char *movement_control, float *tgt, void *out) {
    int r = 0;
    float target_heading, d;
    FUN_001f99f8_259028(out);
    if (FUN_001f9b48_259028(m + 0x10, tgt) < *(float *)(movement_control + 0x2C))
        r = 4;
    target_heading = FUN_001f9e90_259028(tgt[0] - *(float *)(m + 0x10), tgt[1] - *(float *)(m + 0x14));
    FUN_L00_00258278_259028(m, (float *)(movement_control + 0x14), target_heading, *(float *)(movement_control + 0x3C), *(float *)(movement_control + 0x40),
                            *(float *)(movement_control + 0x44));
    d = FUN_001fa688_259028(target_heading, *(float *)(m + 0x48));
    if (*(float *)(movement_control + 0x28) <= d)
        r |= 0x10;
    FUN_001f9740_259028(movement_control + 0x48);
    if (d < *(float *)(movement_control + 0x28) && *(int *)(movement_control + 0x48) < FUN_001f96f8_259028(4)) {
        *(int *)(movement_control + 0x48) = FUN_001f96f8_259028(4);
    }
    if (*(int *)(movement_control + 0x48) != 0) {
        r |= FUN_L00_00258b50_259028(m, movement_control, tgt, out,
                                     (*(float *)(movement_control + 0x28) - d) / *(float *)(movement_control + 0x28));
        if (0.1f < d)
            r &= ~2;
    }
    return r;
}
extern int D_L00_0015FFF4;
extern unsigned short *D_L00_001AB840_2591d0[] __asm__("D_L00_001AB840");
extern char *D_L00_00160114;
extern unsigned short *D_L00_0016010C_2591d0 __asm__("D_L00_0016010C");
extern short D_L00_00160110;
extern char *D_L00_0015FFD8_2591d0 __asm__("D_L00_0015FFD8");
int FUN_L00_002592b8_c(char **, char *) __asm__("FUN_L00_002592b8");
/* Looks up table entry idx and makes it current; the flag test reads the
 * current entry back, as the original did. */
int FUN_L00_002591d0(char **out, int idx, int a, int b) {
    unsigned short *v;
    char *entry_address;
    int entry_sign_bit_set;
    int entry_index;
    if (idx < 0 || D_L00_0015FFF4 < idx) {
        *out = 0;
        return -1;
    }
    *out = 0;
    D_L00_00160114 = 0;
    v = D_L00_001AB840_2591d0[idx];
    D_L00_0016010C_2591d0 = v;
    if (v == 0) return -1;
    entry_index = *v & 0x7FFF;
    D_L00_00160110 = entry_index;
    entry_address = D_L00_0015FFD8_2591d0 + (entry_index << 8);
    D_L00_00160114 = entry_address;
    *out = entry_address;
    entry_sign_bit_set = (unsigned)(*(char *volatile *)&D_L00_00160114)[0x20] >> 31;
    if (!a) {
        if (b || entry_sign_bit_set) return FUN_L00_002592b8_c(out, D_L00_00160114);
    } else if (b && !entry_sign_bit_set) return FUN_L00_002592b8_c(out, D_L00_00160114);
    return 0;
}

extern unsigned short *D_L00_0016010C;
extern short D_L00_00160110;
extern char *D_L00_00160114;
extern int D_L00_0015FFF4;
extern char *D_L00_0015FFD8;
extern unsigned short *D_L00_001AB840[];

/* Iterator over the mobies of target's group: finds the next one whose
 * visibility matches the two flags; 0 when found, -1 at the end. */
int FUN_L00_002592b8(char **out, char *target, int include_sign_bit_set_entries, int b) {
    int entry_sign_bit_set;

    *out = 0;
    if (target != D_L00_00160114) {
        if (D_L00_0015FFF4 < *(unsigned char *)(target + 0x21)) {
            goto fail;
        }
        D_L00_00160114 = 0;
        D_L00_0016010C = D_L00_001AB840[*(unsigned char *)(target + 0x21)];
        if (D_L00_0016010C == 0) {
        fail:
            return -1;
        }
        D_L00_0016010C--;
        do {
            D_L00_0016010C++;
            D_L00_00160110 = *D_L00_0016010C & 0x7FFF;
            D_L00_00160114 = D_L00_0015FFD8 + (D_L00_00160110 << 8);
            if ((short)*D_L00_0016010C < 0) {
                return -1;
            }
        } while (target != D_L00_00160114);
    } else if ((short)*D_L00_0016010C < 0) {
        goto fail;
    }
    do {
        D_L00_0016010C++;
        D_L00_00160110 = *D_L00_0016010C & 0x7FFF;
        D_L00_00160114 = D_L00_0015FFD8 + (D_L00_00160110 << 8);
        *out = D_L00_00160114;
        entry_sign_bit_set = *(signed char *)(D_L00_00160114 + 0x20) < 0;
        if ((include_sign_bit_set_entries == 0 && b == 0 && entry_sign_bit_set == 0) || (include_sign_bit_set_entries != 0 && (b == 0 || entry_sign_bit_set != 0))) {
            return 0;
        }
    } while ((short)*D_L00_0016010C >= 0);
    *out = 0;
    return -1;
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A468), where it is exact; names translated to the US level program. */

/* Lowers the top byte of *p by b (not below 0); true once it reaches 0. */
int FUN_L00_00259430(int *packed_countdown, int b) {
    int w = *packed_countdown;
    int v = (w >> 24) - b;
    if (v < 0)
        v = 0;
    *packed_countdown = (w & 0xFFFFFF) | (v << 24);
    return v == 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A4A0), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");

float FUN_L00_00259468(float *a, float *b, int *out, float f) {
    int truncated_ratio = truncate_float_to_s32(b[2] / f);
    float truncated_ratio_float = convert_integer_to_float(truncated_ratio);
    float r = a[2] + b[2] * truncated_ratio_float - convert_integer_to_float((truncated_ratio * truncated_ratio + truncated_ratio) >> 1) * f;
    if (out) {
        *out = truncated_ratio;
    }
    return r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A540), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");

void FUN_L00_00259508(void *destination, void *b, void *velocity, int frame_count, float f) {
    float displacement[4];
    fast_vec_scale(displacement, velocity, convert_integer_to_float(frame_count));
    displacement[2] -= convert_integer_to_float((frame_count * frame_count + frame_count) >> 1) * f;
    fast_vec_add(destination, b, displacement);
}
float FUN_001f99c0(float);
float square_root(float) __asm__("FUN_001f9988");
int FUN_L00_002595a0(float *r0, float *r1, float a, float b, float c) {
    float discriminant = b * b - a * 4.0f * c;
    float sqrt_abs_discriminant, root_swap_temp;
    if (discriminant == 0.0f) {
        *r0 = -b / (a + a);
        return 1;
    }
    sqrt_abs_discriminant = square_root(FUN_001f99c0(discriminant));
    *r0 = (-b + sqrt_abs_discriminant) / (a + a);
    *r1 = (-b - sqrt_abs_discriminant) / (a + a);
    if (*r0 < *r1) {
        root_swap_temp = *r0;
        *r1 = root_swap_temp;
        *r0 = (-b - sqrt_abs_discriminant) / (a + a);
    }
    {
        int r = 0;
        if (0.0f < discriminant)
            r = 2;
        return r;
    }
}
