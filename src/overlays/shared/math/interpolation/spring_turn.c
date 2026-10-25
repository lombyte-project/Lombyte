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
float interpolate_wrapped_angle_cosine_eased(float x, float y, float z) __asm__("FUN_L00_00257ef0");

float interpolate_wrapped_angle_cosine_eased(float x, float y, float z) {
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

void interpolate_vector_xyz_cosine_eased(void *interpolated_vector_output, void *start_vector, void *target_vector, float interpolation_fraction) __asm__("FUN_L00_00257f70");

void interpolate_vector_xyz_cosine_eased(void *interpolated_vector_output, void *start_vector, void *target_vector, float interpolation_fraction) {
    float interpolation_delta[4];
    float ease;

    if (interpolation_fraction == 0.0f) {
        qcopy(interpolated_vector_output, start_vector);
        return;
    }
    if (interpolation_fraction == 1.0f) {
        qcopy(interpolated_vector_output, target_vector);
        return;
    }
    ease = (1.0f - fast_cos(interpolation_fraction * 3.14159274f)) * 0.5f;
    subtract_vector_xyz(interpolation_delta, target_vector, start_vector);
    scale_vector_xyz(interpolation_delta, interpolation_delta, ease);
    add_vector_xyz(interpolated_vector_output, start_vector, interpolation_delta);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00259088), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");

/* Blends three angles from a toward b with a cosine ease; ang 0 gives a, 1 gives b. */
void interpolate_wrapped_rotations_cosine_eased(float *out, float *start_rotations, float *target_rotations, float interpolation_fraction) __asm__("FUN_L00_00258050");

void interpolate_wrapped_rotations_cosine_eased(float *out, float *start_rotations, float *target_rotations, float interpolation_fraction) {
    float cosine_ease_weight = (1.0f - fast_cos(interpolation_fraction * 3.1415927f)) * 0.5f;
    out[0] = fast_add_rotations(start_rotations[0], fast_subtract_rotations(target_rotations[0], start_rotations[0]) * cosine_ease_weight);
    out[1] = fast_add_rotations(start_rotations[1], fast_subtract_rotations(target_rotations[1], start_rotations[1]) * cosine_ease_weight);
    out[2] = fast_add_rotations(start_rotations[2], fast_subtract_rotations(target_rotations[2], start_rotations[2]) * cosine_ease_weight);
}
float FUN_001fa5c8_00258110(float, float) __asm__("FUN_001fa5c8");
float FUN_001f99c0_00258110(float) __asm__("FUN_001f99c0");
float FUN_001fa580_00258110(float, float) __asm__("FUN_001fa580");
float spring_turn(float *p, float a, float b, float c, float d, float lim) __asm__("FUN_L00_00258110");
float spring_turn(float *turn_step, float current_angle, float target_angle, float spring_strength, float damping_factor, float max_turn_step) {
    float angle_delta = FUN_001fa5c8_00258110(target_angle, current_angle);
    float k = angle_delta / 0.15707964f;
    if (k > 1.0f)
        k = 1.0f;
    else if (k < -1.0f)
        k = -1.0f;
    *turn_step = *turn_step + (spring_strength * k - damping_factor * *turn_step);
    if (max_turn_step != 0.0f) {
        if (*turn_step > max_turn_step)
            *turn_step = max_turn_step;
        else if (*turn_step < -max_turn_step)
            *turn_step = -max_turn_step;
    }
    if (*turn_step > FUN_001f99c0_00258110(angle_delta))
        *turn_step = FUN_001f99c0_00258110(angle_delta);
    else if (*turn_step < -FUN_001f99c0_00258110(angle_delta))
        *turn_step = -FUN_001f99c0_00258110(angle_delta);
    return FUN_001fa580_00258110(current_angle, *turn_step);
}
extern float FUN_001f99c0(float);

void FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void FUN_L00_00258278(unsigned char *moby, float *turn_step, float target_heading, float spring_strength, float damping_factor, float limit) {
    float wrapped_heading_delta = fast_subtract_rotations(target_heading, *(float *)(moby + 0x48));
    float k = wrapped_heading_delta / 0.15707964f;
    float unclamped_turn_step;
    if (1.0f < k)
        k = 1.0f;
    else if (k < -1.0f)
        k = -1.0f;
    unclamped_turn_step = *turn_step + (spring_strength * k - damping_factor * (*turn_step));
    *turn_step = unclamped_turn_step;
    if (limit != 0.0f) {
        if (limit < unclamped_turn_step) {
            *turn_step = limit;
        } else if (unclamped_turn_step < -limit) {
            *turn_step = -limit;
        }
    }
    if (FUN_001f99c0(wrapped_heading_delta) < *turn_step) {
        *turn_step = FUN_001f99c0(wrapped_heading_delta);
    } else if (*turn_step < -FUN_001f99c0(wrapped_heading_delta)) {
        *turn_step = -FUN_001f99c0(wrapped_heading_delta);
    }
    *(float *)(moby + 0x48) = fast_add_rotations(*(float *)(moby + 0x48), *turn_step);
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
s32 probe_vertical_ground_collision(void *probe_base_position, s32 moby, s32 flag, f32 up, f32 down) __asm__("FUN_L00_002583f0");

s32 probe_vertical_ground_collision(void *probe_base_position, s32 moby, s32 flag, f32 up, f32 down) {
    V002583f0 raised_pos, lowered_pos;
    s32 collision_result;
    qcopy(&raised_pos, probe_base_position);
    raised_pos.f[2] += up;
    qcopy(&lowered_pos, probe_base_position);
    lowered_pos.f[2] -= down;
    if (lowered_pos.f[2] < 0.0f)
        lowered_pos.f[2] = 0.0f;
    collision_result = collision_line(&raised_pos, &lowered_pos, 0x22, moby, 0);
    if (D_L00_00173E40_u.x1c < 0)
        collision_result = 0;
    if (flag && D_L00_00173E40_u.x18 != 0)
        collision_result = 0;
    return collision_result;
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
int FUN_L00_00258830_c(char *moving_object, char *movement_displacement, int flags, float a, float b, float ground_probe_offset) {
    Vs original_position;
    Vs raised_position;
    Vs probe_end;
    Vs scaled_displacement;
    int r = 0;
    int fl = (flags * 2) & 0x20;
    char *movement_position = moving_object + 0x10;
    int i;
    char *collision_position;

    qcopy(&original_position, movement_position);
    add_vector_xyz(movement_position, movement_position, movement_displacement);
    if (b < vector_length_xyz(movement_displacement)) {
        qcopy(&raised_position, &original_position);
        raised_position.a[2] += a;
        normalize_vector_xyz(&scaled_displacement, movement_displacement, b);
        add_vector_xyz(&probe_end, &scaled_displacement, movement_position);
        probe_end.a[2] += a;
        if (collision_line(&raised_position, &probe_end, fl, (int)moving_object, 0)) {
            subtract_vector_xyz(movement_position, (char *)&D_94C8_60, &scaled_displacement);
            r = 1;
            *(float *)(moving_object + 0x18) -= a;
        }
    }
    for (i = 0, collision_position = moving_object + 0x10; i < 6; i++) {
        qcopy(&raised_position, collision_position);
        raised_position.a[2] += a;
        if (func_L00_001F10E0_l(&raised_position, b, fl | 4, moving_object)) {
            qcopy(collision_position, &D_L00_00173E70);
            r |= 1;
            *(float *)(moving_object + 0x18) -= a;
        } else {
            break;
        }
    }
    b = *(float *)(moving_object + 0x18) - ground_probe_offset;
    qcopy(&raised_position, collision_position);
    qcopy(&probe_end, collision_position);
    raised_position.a[2] += a;
    probe_end.a[2] = b - 0.05f;
    if (collision_line(&raised_position, &probe_end, fl | 2, (int)moving_object, 0)) {
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
                *(float *)(moving_object + 0x18) += (g - b) * 0.3f;
            }
        }
    }
    subtract_vector_xyz(movement_displacement, collision_position, &original_position);
    return r;
}
float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
void FUN_L00_00258830(int, int, float, float, float, int);
/* Calls FUN_L00_00258830 with c converted to float and divided by 1024. */
void move_moby_with_collision_integer_radius(int moby, int movement_displacement, float x, float y, int collision_radius_fixed, int collision_flags) __asm__("FUN_L00_00258ad0");

void move_moby_with_collision_integer_radius(int moby, int movement_displacement, float x, float y, int collision_radius_fixed, int collision_flags) {
    FUN_L00_00258830(moby, movement_displacement, x, convert_integer_to_float(collision_radius_fixed) * (1.0f / 1024.0f), y, collision_flags);
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
s32 FUN_L00_00258b50(Obj *moby, Ctl *movement_control, void *target_position, Vec4 *out, f32 f) {
    Vec4 save;
    Vec4 displacement;
    Vec4 probe_or_direction;
    Vec4 q;
    Glob *collision_probe_result;
    s32 mask = 0;
    s32 movement_collision_flags;
    f32 target_distance_xy;
    f32 sp;
    f32 horizontal_step_cap_or_probe_height_threshold;
    f32 speed_increment;
    f32 displacement_probe_hit_height;
    f32 lookahead_probe_hit_height;
    f32 ground_height;
    f32 normal_elevation_angle;
    f32 intended_displacement_length_xy;

    sp = movement_control->f1C * f;
    horizontal_step_cap_or_probe_height_threshold = movement_control->f24 * f;
    fast_vec_sub(&displacement, target_position, &moby->pos);
    target_distance_xy = vector_length_xy(&displacement);
    save.q = moby->pos.q;
    speed_increment = sp * target_distance_xy;
    if (sp < speed_increment)
        speed_increment = sp;
    movement_control->f10 = movement_control->f10 + speed_increment;
    if (target_distance_xy < movement_control->f10 * 10.0f)
        movement_control->f10 = movement_control->f10 - movement_control->f20 * movement_control->f10;
    if (horizontal_step_cap_or_probe_height_threshold < movement_control->f10)
        movement_control->f10 = horizontal_step_cap_or_probe_height_threshold;
    else if (movement_control->f10 < -horizontal_step_cap_or_probe_height_threshold)
        movement_control->f10 = -horizontal_step_cap_or_probe_height_threshold;
    if (movement_control->i38 & 4) {
        displacement.f[0] = FUN_001f9dc8_c(movement_control->f4C) * movement_control->f10;
        displacement.f[1] = fast_sin(movement_control->f4C) * movement_control->f10;
    } else {
        displacement.f[0] = FUN_001f9dc8_c(moby->f48) * movement_control->f10;
        displacement.f[1] = fast_sin(moby->f48) * movement_control->f10;
    }
    if (movement_control->i38 & 1) {
        displacement.f[2] = 0.0f;
    } else {
        displacement.f[2] = movement_control->f18;
        movement_control->f18 = movement_control->f18 - frame_time_sq_gp * 9.0f;
    }
    horizontal_step_cap_or_probe_height_threshold = moby->pos.f[2] + 0.04f;
    fast_vec_add(&probe_or_direction, &moby->pos, &displacement);
    probe_or_direction.f[2] = moby->pos.f[2] - movement_control->f30;
    if (FUN_L00_002583f0_c(&probe_or_direction, moby, movement_control->i38 & 0x20, movement_control->f08, movement_control->f0C)) {
        collision_probe_result = &D_L00_00173E40_c;
        normal_elevation_angle = fast_arc_tan(collision_probe_result->v40.f[2], vector_length_xy(&collision_probe_result->v40));
        displacement_probe_hit_height = collision_probe_result->f28;
        if (0.5f < normal_elevation_angle && 0.1f < vector_length_xy(&displacement)) {
            {
                fast_vec_normalize(&q, &displacement, 0.57f);
                fast_vec_add(&q, &q, &moby->pos);
                q.f[2] = moby->pos.f[2] - movement_control->f30;
                if (FUN_L00_002583f0_c(&q, moby, movement_control->i38 & 0x20, movement_control->f08, movement_control->f0C)) {
                    lookahead_probe_hit_height = collision_probe_result->f28;
                    if (!(0.5f <= fast_arc_tan(collision_probe_result->v40.f[2], vector_length_xy(&collision_probe_result->v40))))
                        goto done;
                    mask = 0x22;
                    if (!(movement_control->i38 & 8))
                        goto done;
                    if (!(lookahead_probe_hit_height < horizontal_step_cap_or_probe_height_threshold))
                        goto done;
                } else {
                    mask = 0x22;
                    if (!(movement_control->i38 & 8))
                        goto done;
                }
                if (displacement_probe_hit_height < horizontal_step_cap_or_probe_height_threshold)
                    mask = 8;
            }
        }
    } else {
        mask = (movement_control->i38 & 8) ? 8 : 0x22;
    }
done:
    if (!(mask & 2)) {
        intended_displacement_length_xy = vector_length_xy(&displacement);
        FUN_001f9a68_c(&probe_or_direction, &displacement, 1.0f / intended_displacement_length_xy);
        movement_collision_flags = FUN_L00_00258ad0_c(moby, &displacement, movement_control->f04, movement_control->i00, movement_control->f30, movement_control->i38);
        if (movement_collision_flags & 2)
            movement_control->f18 = frame_scale * -0.01f;
        if (movement_collision_flags & 1) {
            mask |= 1;
            if (displacement.f[0] * probe_or_direction.f[0] + displacement.f[1] * probe_or_direction.f[1] < intended_displacement_length_xy * 0.2f)
                mask |= 2;
            ground_height = probe_ground_height(&moby->pos, 0, 0.5f);
            if (movement_control->f08 < save.f[2] - ground_height || movement_control->f0C < ground_height - save.f[2]) {
                mask |= 0x22;
                moby->pos.q = save.q;
            }
        }
        qcopy(out, &displacement);
    } else {
        if ((movement_control->i38 ^ 1) & 1)
            movement_control->f18 = frame_scale * -0.01f;
    }
    if (target_distance_xy < movement_control->f2C)
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
int FUN_L00_00259028(unsigned char *moby, char *movement_control, float *target_position, void *out) {
    int navigation_result_flags = 0;
    float target_heading, heading_error;
    FUN_001f99f8_259028(out);
    if (FUN_001f9b48_259028(moby + 0x10, target_position) < *(float *)(movement_control + 0x2C))
        navigation_result_flags = 4;
    target_heading = FUN_001f9e90_259028(target_position[0] - *(float *)(moby + 0x10), target_position[1] - *(float *)(moby + 0x14));
    FUN_L00_00258278_259028(moby, (float *)(movement_control + 0x14), target_heading, *(float *)(movement_control + 0x3C), *(float *)(movement_control + 0x40),
                            *(float *)(movement_control + 0x44));
    heading_error = FUN_001fa688_259028(target_heading, *(float *)(moby + 0x48));
    if (*(float *)(movement_control + 0x28) <= heading_error)
        navigation_result_flags |= 0x10;
    FUN_001f9740_259028(movement_control + 0x48);
    if (heading_error < *(float *)(movement_control + 0x28) && *(int *)(movement_control + 0x48) < FUN_001f96f8_259028(4)) {
        *(int *)(movement_control + 0x48) = FUN_001f96f8_259028(4);
    }
    if (*(int *)(movement_control + 0x48) != 0) {
        navigation_result_flags |= FUN_L00_00258b50_259028(moby, movement_control, target_position, out,
                                     (*(float *)(movement_control + 0x28) - heading_error) / *(float *)(movement_control + 0x28));
        if (0.1f < heading_error)
            navigation_result_flags &= ~2;
    }
    return navigation_result_flags;
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
int get_first_moby_in_group(char **out, int moby_group_index, int include_sign_bit_set_entries, int require_sign_bit_set_entries) __asm__("FUN_L00_002591d0");

int get_first_moby_in_group(char **out, int moby_group_index, int include_sign_bit_set_entries, int require_sign_bit_set_entries) {
    unsigned short *entry_index_list;
    char *entry_address;
    int entry_sign_bit_set;
    int entry_index;
    if (moby_group_index < 0 || D_L00_0015FFF4 < moby_group_index) {
        *out = 0;
        return -1;
    }
    *out = 0;
    D_L00_00160114 = 0;
    entry_index_list = D_L00_001AB840_2591d0[moby_group_index];
    D_L00_0016010C_2591d0 = entry_index_list;
    if (entry_index_list == 0) return -1;
    entry_index = *entry_index_list & 0x7FFF;
    D_L00_00160110 = entry_index;
    entry_address = D_L00_0015FFD8_2591d0 + (entry_index << 8);
    D_L00_00160114 = entry_address;
    *out = entry_address;
    entry_sign_bit_set = (unsigned)(*(char *volatile *)&D_L00_00160114)[0x20] >> 31;
    if (!include_sign_bit_set_entries) {
        if (require_sign_bit_set_entries || entry_sign_bit_set) return FUN_L00_002592b8_c(out, D_L00_00160114);
    } else if (require_sign_bit_set_entries && !entry_sign_bit_set) return FUN_L00_002592b8_c(out, D_L00_00160114);
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
int get_next_moby_in_group(char **out, char *target, int include_sign_bit_set_entries, int require_sign_bit_set_entries) __asm__("FUN_L00_002592b8");

int get_next_moby_in_group(char **out, char *target, int include_sign_bit_set_entries, int require_sign_bit_set_entries) {
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
        if ((include_sign_bit_set_entries == 0 && require_sign_bit_set_entries == 0 && entry_sign_bit_set == 0) || (include_sign_bit_set_entries != 0 && (require_sign_bit_set_entries == 0 || entry_sign_bit_set != 0))) {
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
int decrement_packed_countdown(int *packed_countdown, int decrement_amount) __asm__("FUN_L00_00259430");

int decrement_packed_countdown(int *packed_countdown, int decrement_amount) {
    int original_packed_word = *packed_countdown;
    int remaining_countdown = (original_packed_word >> 24) - decrement_amount;
    if (remaining_countdown < 0)
        remaining_countdown = 0;
    *packed_countdown = (original_packed_word & 0xFFFFFF) | (remaining_countdown << 24);
    return remaining_countdown == 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A4A0), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");

float calculate_triangular_corrected_scalar(float *base_values, float *b, int *out, float triangular_correction_coefficient) __asm__("FUN_L00_00259468");

float calculate_triangular_corrected_scalar(float *base_values, float *linear_term_values, int *out, float triangular_correction_coefficient) {
    int truncated_ratio = truncate_float_to_s32(linear_term_values[2] / triangular_correction_coefficient);
    float truncated_ratio_float = convert_integer_to_float(truncated_ratio);
    float corrected_scalar_value = base_values[2] + linear_term_values[2] * truncated_ratio_float - convert_integer_to_float((truncated_ratio * truncated_ratio + truncated_ratio) >> 1) * triangular_correction_coefficient;
    if (out) {
        *out = truncated_ratio;
    }
    return corrected_scalar_value;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A540), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");

void predict_position_with_vertical_drop(void *destination, void *base_position, void *velocity, int frame_count, float vertical_drop_coefficient) __asm__("FUN_L00_00259508");

void predict_position_with_vertical_drop(void *destination, void *base_position, void *velocity, int frame_count, float vertical_drop_coefficient) {
    float displacement[4];
    fast_vec_scale(displacement, velocity, convert_integer_to_float(frame_count));
    displacement[2] -= convert_integer_to_float((frame_count * frame_count + frame_count) >> 1) * vertical_drop_coefficient;
    fast_vec_add(destination, base_position, displacement);
}
float FUN_001f99c0(float);
float square_root(float) __asm__("FUN_001f9988");
int solve_quadratic_equation(float *larger_root_output, float *smaller_root_output, float quadratic_coefficient, float linear_coefficient, float constant_coefficient) __asm__("FUN_L00_002595a0");

int solve_quadratic_equation(float *larger_root_output, float *smaller_root_output, float quadratic_coefficient, float linear_coefficient, float constant_coefficient) {
    float discriminant = linear_coefficient * linear_coefficient - quadratic_coefficient * 4.0f * constant_coefficient;
    float sqrt_abs_discriminant, root_swap_temp;
    if (discriminant == 0.0f) {
        *larger_root_output = -linear_coefficient / (quadratic_coefficient + quadratic_coefficient);
        return 1;
    }
    sqrt_abs_discriminant = square_root(FUN_001f99c0(discriminant));
    *larger_root_output = (-linear_coefficient + sqrt_abs_discriminant) / (quadratic_coefficient + quadratic_coefficient);
    *smaller_root_output = (-linear_coefficient - sqrt_abs_discriminant) / (quadratic_coefficient + quadratic_coefficient);
    if (*larger_root_output < *smaller_root_output) {
        root_swap_temp = *larger_root_output;
        *smaller_root_output = root_swap_temp;
        *larger_root_output = (-linear_coefficient - sqrt_abs_discriminant) / (quadratic_coefficient + quadratic_coefficient);
    }
    {
        int real_root_count = 0;
        if (0.0f < discriminant)
            real_root_count = 2;
        return real_root_count;
    }
}
