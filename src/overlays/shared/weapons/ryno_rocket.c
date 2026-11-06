/* Shared moby class rocket (0x1C9): the R.Y.N.O. missile. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/sdk/libc/stdlib.h"

typedef struct RynoRocketVars {
    u8    pad0[0x10];
    u8    path_position[0x10];
    char *owner;
    s32   life_timer;
    char *target;
    f32   target_height_offset;
    f32   speed;
    s32   yaw_velocity;
    s32   pitch_velocity;
    u8    pad3C[0x8];
    f32   wobble_radius;
    f32   wobble_angle;
    f32   wobble_phase;
    f32   wobble_angle_rate;
    f32   wobble_phase_rate;
    s32   target_class;
} RynoRocketVars;

typedef union {
    u128 q;
    f32  f[4];
} V4;

typedef struct {
    u8  pad00[0x28];
    f32 f28;
    u8  pad2C[0x40 - 0x2C];
    V4  v40;
} Data;

typedef int u128_cd __attribute__((mode(TI)));

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V4_cd;

typedef struct {
    float          x, y, z, w;
    char          *p;
    int            c;
    unsigned char  b18;
    unsigned char  b19;
    unsigned short s;
    float          e;
    int            i;
} RocketHit;

extern float D_0015ED6C MACRO_ADDR; /* frame-rate scale */
extern float D_0015ED70 MACRO_ADDR;
extern void             fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void             reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern Data             D_L00_00173E40;
extern float            D_0015ED6C;
extern int              collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void             add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void             fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern float            D_0015ED60;

extern unsigned char *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;

extern float D_0015ED70;
extern float FUN_L00_001ff430(void *);
extern float FUN_L00_00258110(float *vel, float cur, float target, float k, float d, float max);
extern s32   scale_game_frames_c(s32) __asm__("func_001F96F8");
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
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

/* Rocket moby update: steers the rocket toward its target, moves it with collision retries, then checks hits and range. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CE6A0), where it is exact; names translated to the US level program. */

void ryno_rocket_update(char *m) __asm__("FUN_L00_002cd1f0");

/* Moby class 457 */
void ryno_rocket_update(char *rocket_moby) {
    RynoRocketVars *rocket_vars;
    char           *target_moby;
    char           *p;
    char           *gg;
    V4_cd           previous_position;
    V4_cd           explosion_direction;
    V4_cd           z;
    V4_cd           forward_step;
    union {
        struct {
            V4_cd a;
            V4_cd b;
            V4_cd c;
        } v;
        RocketHit h;
    } u;
    V4_cd         intercept_direction;
    V4_cd         va0[4];
    int           steering_timer_threshold;
    unsigned char flag;
    float         selected_intercept_root;
    float         s;
    float         k;
    float         k60;
    float         a;
    float         intercept_angle_scratch;
    float         sp;
    float         zf;

    if (rocket_moby == 0)
        return;
    rocket_vars = (RynoRocketVars *)(*(char **)(rocket_moby + 0x78));
    if (rocket_vars == 0)
        return;

    a = *(float *)(rocket_moby + 0x2C);
    k = D_0015ED6C;
    *(u128_cd *)&z = 0;
    z.f[2] = k * 8.0f;
    *(u128_cd *)&explosion_direction = *(u128_cd *)&z;
    k60 = D_0015ED60;
    {
        float k64 = frame_scale_sq;
        *(float *)(rocket_moby + 0x2C) =
            a + (*(float *)(*(char **)(rocket_moby + 0x24) + 0x24) - a) * (k60 * 0.05f);
        {
            float e = rocket_vars->speed;
            rocket_vars->speed = e + (k * 20.0f - e) * (k60 * 0.1f) * k64;
        }
    }
    steering_timer_threshold = scale_game_frames_c(250);
    steering_timer_threshold -= scale_game_frames_c(5);
    if (rocket_vars->life_timer < steering_timer_threshold) {
        target_moby = rocket_vars->target;
        if (target_moby == 0 || *(short *)(target_moby + 0xA6) != rocket_vars->target_class ||
            *(unsigned char *)(target_moby + 0x20) == 0xFE ||
            *(unsigned char *)(target_moby + 0x20) == 0xFD) {
            rocket_vars->wobble_radius = rocket_vars->wobble_radius + D_0015ED60 * 0.1f;
            *(int *)&rocket_vars->target = 0;
            steering_timer_threshold = scale_game_frames_c(250);
            steering_timer_threshold -= scale_game_frames_c(100);
            if (rocket_vars->life_timer < steering_timer_threshold) {
                *(float *)(rocket_moby + 0x44) =
                    fast_add_rotations(*(float *)(rocket_moby + 0x44),
                                       fast_sin(rocket_vars->wobble_angle) * (D_0015ED60 * 0.04f));
            }
            *(float *)(rocket_moby + 0x48) =
                fast_add_rotations(*(float *)(rocket_moby + 0x48),
                                   fast_cos(rocket_vars->wobble_angle) * (D_0015ED60 * 0.05f));
        } else {
            qcopy(&u.v.c, target_moby + 0x10);
            u.v.c.f[2] = u.v.c.f[2] + rocket_vars->target_height_offset;
            fast_vec_sub(&u.v.b, &u.v.c, rocket_vars);
            fast_vec_sub(&u.v.a, &u.v.c, &rocket_vars->path_position);
            qcopy(rocket_vars, &u.v.c);
            {
                float target_displacement_length_squared = FUN_L00_001ff430(&u.v.b);
                float fB =
                    (u.v.b.f[0] * u.v.a.f[0] + u.v.b.f[1] * u.v.a.f[1] + u.v.b.f[2] * u.v.a.f[2]) *
                    -2.0f;
                float fA =
                    rocket_vars->speed * rocket_vars->speed - target_displacement_length_squared;
                float r2f = FUN_L00_001ff430(&u.v.a);
                float sq = square_root(fB * fB - (4.0f * fA) * (-r2f));
                float intercept_root_plus = (-fB + sq) / (fA + fA);
                float intercept_root_minus = (-fB - sq) / (fA + fA);
                if (0.0f < intercept_root_plus && 0.0f < intercept_root_minus) {
                    selected_intercept_root = (intercept_root_minus < intercept_root_plus)
                                                  ? intercept_root_plus
                                                  : intercept_root_minus;
                } else if (0.0f < intercept_root_plus) {
                    selected_intercept_root = intercept_root_plus;
                } else if (0.0f < intercept_root_minus) {
                    selected_intercept_root = intercept_root_minus;
                } else {
                    selected_intercept_root = -1.0f;
                }
            }
            if (0.0f < selected_intercept_root) {
                fast_vec_scale(&intercept_direction, &u.v.b, selected_intercept_root);
                add_vector_xyz(&intercept_direction, &intercept_direction, &u.v.a);
                intercept_angle_scratch =
                    fast_arc_tan(intercept_direction.f[0], intercept_direction.f[1]);
                *(float *)(rocket_moby + 0x48) =
                    FUN_L00_00258110((float *)(&rocket_vars->yaw_velocity),
                                     *(float *)(rocket_moby + 0x48),
                                     intercept_angle_scratch,
                                     D_0015ED70 * 188.49556f,
                                     0.0f,
                                     0.0f);
                intercept_angle_scratch =
                    fast_arc_tan(vector_length_xy(&intercept_direction), intercept_direction.f[2]);
                *(float *)(rocket_moby + 0x44) =
                    FUN_L00_00258110((float *)(&rocket_vars->pitch_velocity),
                                     *(float *)(rocket_moby + 0x44),
                                     -intercept_angle_scratch,
                                     D_0015ED70 * 188.49556f,
                                     0.0f,
                                     0.0f);
            }
            s = fast_vec_length(&u.v.a);
            if (s < 6.0f) {
                rocket_vars->wobble_radius = rocket_vars->wobble_radius * (s / 6.0f);
            } else {
                rocket_vars->wobble_radius = rocket_vars->wobble_radius + D_0015ED60 * 0.1f;
            }
        }
    }

    build_spherical_offset(&forward_step,
                           rocket_vars->speed,
                           *(float *)(rocket_moby + 0x48),
                           -*(float *)(rocket_moby + 0x44));
    *(u128_cd *)&u.v.b = 0;
    u.v.b.f[0] = random_float_between(-1.0f, 1.0f);
    u.v.b.f[1] = random_float_between(-1.0f, 1.0f);
    u.v.b.f[2] = random_float_between(-1.0f, 1.0f);
    *(u128_cd *)&u.v.a = *(u128_cd *)&u.v.b;
    fast_vec_normalize(&u.v.a, &u.v.a, random_float_between(0.1f, 0.2f) * D_0015ED6C);
    fast_vec_normalize(&z, &forward_step, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
    add_vector_xyz(&u.v.a, &u.v.a, &z);
    fast_vec_normalize(&z, &forward_step, random_float_between(0.0f, 1.0f) * rocket_vars->speed);
    add_vector_xyz(&z, &z, rocket_moby + 0x10);
    {
        int particle_duration_frames = scale_game_frames_c(random_int_between(0x1C, 0x28));
        spawn_effect(&z,
                     &u.v.a,
                     0x6F00AFFF,
                     0x100000FF,
                     particle_duration_frames,
                     0x28,
                     random_int_between(0x28, 0x46),
                     1);
    }
    qcopy(&previous_position, rocket_moby + 0x10);
    add_vector_xyz(&rocket_vars->path_position, &rocket_vars->path_position, &forward_step);
    rocket_vars->wobble_angle =
        fast_add_rotations(rocket_vars->wobble_angle, rocket_vars->wobble_angle_rate);
    rocket_vars->wobble_phase =
        fast_add_rotations(rocket_vars->wobble_phase, rocket_vars->wobble_phase_rate);
    rocket_vars->wobble_angle =
        fast_add_rotations(rocket_vars->wobble_angle, fast_sin(rocket_vars->wobble_phase) * 0.15f);
    if (0.9f < rocket_vars->wobble_radius)
        rocket_vars->wobble_radius = 0.9f;

    flag = 0;
    goto body;
    for (;;) {
        float offset_trig_factor;
        if (!(0.1f < sp))
            goto done;
    body:
        offset_trig_factor = fast_sin(rocket_vars->wobble_angle);
        u.v.a.f[1] =
            rocket_vars->wobble_radius * offset_trig_factor * fast_cos(rocket_vars->wobble_phase);
        offset_trig_factor = fast_cos(rocket_vars->wobble_angle);
        u.v.a.f[2] =
            rocket_vars->wobble_radius * offset_trig_factor * fast_cos(rocket_vars->wobble_phase);
        zf = 0.0f;
        u.v.a.f[3] = zf;
        u.v.a.f[0] = zf;
        euler_rotation_matrix(va0, rocket_moby + 0x40);
        transform_vector(&u.v.a, &u.v.a, va0);
        add_vector_xyz(rocket_moby + 0x10, &rocket_vars->path_position, &u.v.a);
        if (collision_line(&previous_position, rocket_moby + 0x10, 6, (int)rocket_moby, 0) == 0)
            goto done;
        goto hit;
    top:;
    }
hit:
    flag = 1;
    sp = rocket_vars->wobble_radius * 0.7f;
    rocket_vars->wobble_radius = sp;
    goto top;
done:

    if (flag) {
        *(float *)(rocket_moby + 0x48) =
            fast_arc_tan(*(float *)(rocket_moby + 0x10) - previous_position.f[0],
                         *(float *)(rocket_moby + 0x14) - previous_position.f[1]);
        *(float *)(rocket_moby + 0x44) =
            -fast_arc_tan(vector_distance_xy(&previous_position, rocket_moby + 0x10),
                          *(float *)(rocket_moby + 0x18) - previous_position.f[2]);
        rocket_vars->yaw_velocity = 0;
        rocket_vars->pitch_velocity = 0;
    }

    if (*(float *)(rocket_moby + 0x10) < 0.0f || *(float *)(rocket_moby + 0x14) < 0.0f ||
        *(float *)(rocket_moby + 0x18) < 0.0f)
        goto del;
    u.h.c = 0x830000;
    u.h.e = 3.0f;
    u.h.p = rocket_moby;
    u.h.i = 1;
    qcopy(&u.v.a, &forward_step);
    set_vector_length_xy(&u.v.a, &u.v.a, 1.0f);
    u.h.z = 1.0f;
    u.h.b18 = 3;
    u.h.w = 5627.925f;
    u.h.b19 = 3;
    u.h.s = *(unsigned short *)(rocket_moby + 0xA6);
    if (collision_line(&previous_position, rocket_moby + 0x10, 0, (int)rocket_moby, (int)&u.v.a)) {
        gg = (char *)((int *)&D_L00_00173E40);
        p = *(char **)(gg + 0x18);
        if (p != 0) {
            if (p == (char *)D_001413D0_lui || p == rocket_vars->owner ||
                *(short *)(p + 0xA6) == 0x1C9 || *(short *)(p + 0xA6) == 0)
                goto keep;
            rocket_moby[0xBC] = 2;
            qcopy(rocket_moby + 0x10, gg + 0x20);
            reflect_vector(&explosion_direction, &forward_step, gg + 0x40);
            fast_vec_normalize(&explosion_direction, &explosion_direction, D_0015ED6C + D_0015ED6C);
            func_L00_0025F4A8_alt(rocket_moby,
                                  &explosion_direction,
                                  0,
                                  0.0f,
                                  0.0f,
                                  10,
                                  3,
                                  9,
                                  4.0f,
                                  2.0f,
                                  9.0f,
                                  1.0f,
                                  0,
                                  15.0f,
                                  1,
                                  1,
                                  -1,
                                  0);
            mark_moby_for_removal(rocket_moby);
            return;
        } else {
            if (*(int *)(gg + 0x1C) <= 0)
                goto keep;
            rocket_moby[0xBC] = 1;
            qcopy(rocket_moby + 0x10, gg + 0x20);
            reflect_vector(&explosion_direction, &forward_step, gg + 0x40);
            fast_vec_normalize(&explosion_direction, &explosion_direction, D_0015ED6C + D_0015ED6C);
            func_L00_0025F4A8_alt(rocket_moby,
                                  &explosion_direction,
                                  0,
                                  0.0f,
                                  1.0f,
                                  10,
                                  3,
                                  9,
                                  4.0f,
                                  2.0f,
                                  9.0f,
                                  1.0f,
                                  0,
                                  15.0f,
                                  1,
                                  1,
                                  -1,
                                  0);
            mark_moby_for_removal(rocket_moby);
            return;
        }
    } else {
        if (fast_dec_timer((int *)(&rocket_vars->life_timer)) == 0 &&
            !(200.0f < vector_distance_xy(rocket_moby + 0x10, (char *)D_001413D0_lui + 0x10)))
            goto keep;
    }
del:
    mark_moby_for_removal(rocket_moby);
    return;
keep:
    if (rocket_vars->life_timer % scale_game_frames_c(0x20) != 0)
        return;
    if (RandomNext() & 1) {
        rocket_vars->wobble_angle_rate =
            rocket_vars->wobble_angle_rate * random_float_between(-0.7f, -1.5f);
    }
    if (RandomNext() & 1) {
        rocket_vars->wobble_phase_rate =
            rocket_vars->wobble_phase_rate * random_float_between(-0.6f, -1.6f);
    }
}
