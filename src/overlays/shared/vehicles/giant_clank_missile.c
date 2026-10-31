/* Giant Clank's missile (class 0x100, Quartu and Veldin 2): homes on its target, leading it; and the function that spawns it. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/spherical_offset.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "sda.h"
#include "rnc/overlay/shared/vehicles/giant_clank_missile_data/update_giant_clank_missile_2.h"
#include "rnc/overlay/shared/vehicles/giant_clank_missile_data/giant_clank_missile_update.h"

typedef float V[4] __attribute__((aligned(16)));

typedef int Q_2aa998 __attribute__((mode(TI)));

typedef union {
    Q_2aa998 q;
    float    f[4];
} BoltVector;

typedef struct {
    char       pad[0x10];
    BoltVector position;
} BoltOwner;

extern float D_0015ED6C;
extern f32   D_0015ED6C;
extern s32   FUN_001efa68(void *, void *, s32, s32, s32);

extern void *D_001413D0 NOT_SDA;

extern char *get_moby_target_info(void *) __asm__("FUN_002141f8");
extern int   FUN_001efa68(void *, void *, int, int, int);

extern V     D_L00_00173E60;
extern char *select_auto_aim_target_moby(
    void *, void *, void *, float *, float *, float *, float) __asm__("FUN_L00_002630c8");
extern char  D_0013F490[];
extern void *D_001413D0;
extern float D_L00_001613C4 __attribute__((sda));
extern short D_L00_001B0770[];
extern void  append_unique_moby_index(void *, short *, int) __asm__("FUN_L00_0025f780");

/* Spawns a homing bolt (class 0x100) from owner at pos, aimed at tgt or at whatever lies along its heading. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AA998), where it is exact; names translated to the US level program. */

char *spawn_giant_clank_missile(void *owner,
                                void *pos,
                                float a,
                                float b,
                                float c,
                                char *tgt,
                                float rz,
                                float ry) __asm__("FUN_L00_002a96f8");

char *spawn_giant_clank_missile(

    void *owner, void *pos, float a, float b, float c, char *tgt, float rz, float ry) {
    char      *m;
    char      *d;
    BoltVector t;
    BoltVector p;
    m = (char *)create_moby(0x100);
    if (m != 0) {
        ((unsigned char *)m)[0x30] = 0xFF;
        m[0x31] = 1;
        m[0x20] = 0;
        *(float *)(m + 0x2C) *= D_L00_001613C4;
        *(short *)(m + 0x32) = 0xFF;
        d = *(char **)(m + 0x78);
        *(float *)(m + 0x48) = rz;
        *(float *)(m + 0x44) = ry;
        if (tgt == 0) {
            build_spherical_offset(&t, D_0015ED6C * 20.0f, rz, ry);
            p.q = *(Q_2aa998 *)pos;
            tgt = select_auto_aim_target_moby(owner, &p, &t, &a, &b, &c, 0.34906584f);
        }
        *(void **)(d + 0x10) = owner;
        qcopy(m + 0x10, pos);
        *(float *)(d + 0x34) = D_0015ED6C * 100.0f;
        *(int *)(d + 0x38) = 1;
        *(int *)(d + 0x24) = 0;
        *(int *)(d + 0x28) = 0;
        *(int *)(d + 0x20) = 0;
        *(char **)(d + 0x18) = tgt;
        if (tgt != 0) {
            unsigned char *e = (unsigned char *)get_moby_target_info(tgt);
            if (e != 0) {
                *(unsigned short *)(e + 0x1E) |= 0x80;
                if (e[0xB] != 0)
                    *(int *)(d + 0x38) = 0;
                if (e[0xC] != 0)
                    *(float *)(d + 0x34) = (float)e[0xC] * D_0015ED6C;
            }
        }
        *(float *)(d + 0x2C) = a;
        *(float *)(d + 0x30) = b;
        *(int *)(d + 0x14) = scale_game_frames(0x96);
        if (tgt != 0) {
            qcopy(d, tgt + 0x10);
            *(float *)(d + 8) += c;
            *(float *)(d + 0x1C) = c;
        }
        {
            int r;
            if (fast_vec_length((float *)(D_0013F490)) > D_0015ED6C * 0.1f) {
                r = scale_game_frames(0x3C);
                *(short *)(d + 0x3E) = r;
                *(short *)(d + 0x3C) = r;
            } else {
                r = scale_game_frames(1);
                *(short *)(d + 0x3E) = r;
                *(short *)(d + 0x3C) = r;
            }
        }
        t.q = ((BoltOwner *)owner)->position.q;
        t.f[2] = ((float *)pos)[2];
        if (FUN_001efa68(&t, pos, 0, *(int *)(((char *)&D_001413D0)), 0) != 0) {
            m[0xBC] = 1;
            qcopy(m + 0x10, &D_L00_00173E60);
        }
        update_entity(m);
        append_unique_moby_index(m, D_L00_001B0770, 0x1F);
        *(float *)(m + 0x2C) += *(float *)(m + 0x2C);
    }
    return m;
}

short D_L01_001B0BF0 NOT_SDA = 0;
short D_L02_001B0D70 NOT_SDA = 0;
short D_L03_001B0870 NOT_SDA = 0;
float                D_L04_00161398 = 3.0f;
short D_L04_001B08F0 NOT_SDA = 0;
short D_L05_001B0BF0 NOT_SDA = 0;
short D_L06_001B0EF0 NOT_SDA = 0;
float                D_L07_00161398 = 3.0f;
short D_L07_001B07F0 NOT_SDA = 0;
short D_L08_001B0F70 NOT_SDA = 0;
float                D_L09_00161420 = 3.0f;
short D_L09_001B08F0 NOT_SDA = 0;
float                D_L10_0016139C = 3.0f;
short D_L10_001B0BF0 NOT_SDA = 0;
short D_L11_001B1170 NOT_SDA = 0;
short D_L12_001B0BF0 NOT_SDA = 0;
float                D_L13_00161460 = 3.0f;
short D_L13_001B0A70 NOT_SDA = 0;
short D_L14_001B0E70 NOT_SDA = 0;
float                D_L15_00161480 = 3.0f;
short D_L16_001B0BF0 NOT_SDA = 0;
short D_L17_001B1070 NOT_SDA = 0;
float                D_L18_00161438 = 3.0f;
short D_L18_001B1170 NOT_SDA = 0;

typedef int u128_29E8A0 __attribute__((mode(TI)));

typedef struct {
    int w[20];
} __attribute__((aligned(16))) Tbl_29E8A0;

typedef struct {
    int c[6];
} Cols;

typedef struct {
    char          pad0[2];
    unsigned char f2;
    unsigned char f3;
} Part;

typedef struct {
    float          v[4];
    char          *moby;
    int            flags;
    char           kind;
    char           one;
    unsigned short id;
    float          radius;
    int            count;
} Coll;

typedef struct {
    float            v[4];
    char            *owner;
    int              timer;
    struct M_29E8A0 *target;
    float            zoff;
    float            speed;
    float            yawvel;
    float            pitchvel;
    float            yaw0;
    float            pitch0;
    float            speed_end;
    int              f38;
    short            f3C;
    short            f3E;
} D_29E8A0;

typedef struct M_29E8A0 {
    char           pad0[0x10];
    float          pos[4];
    signed char    state;
    char           pad21[0x1F];
    float          rot[4];
    char           pad50[0x28];
    D_29E8A0      *data;
    char           pad7C[0x2A];
    unsigned short id;
    char           padA8[0x14];
    unsigned char  bc;
    char           padBD[3];
    float          mtx[16];
} M_29E8A0;

extern float D_L15_001673C0[4];
extern float fast_sin(float);
extern float vector_length_xy(void *);
extern int   random_integer_below(int);
extern int   scale_game_frames(int);

extern Cols       l15_missile_burst_primary_color_palette __asm__("D_L15_001E2660");
extern Cols       l15_missile_burst_secondary_color_palette __asm__("D_L15_001E2678");
extern Tbl_29E8A0 l15_missile_camera_blast_parameter_template __asm__("D_L15_001E2610");
extern int        D_L15_00178500[];
extern char      *spawn_debris(char *, char *, int, int, int) __asm__("FUN_L00_002af450");
extern char       D_L15_00174440[];
extern char       missile_drop_spawn_parameters[] __asm__("D_L15_001B0D70");
extern float      D_0015ED64;
extern float      D_0015ED70;

extern float FUN_L00_001ff430(void *);
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float fast_vec_dist(void *, void *);

extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int FUN_L00_00257b90(int, int);
extern unsigned char *D_L15_001B26DC NOT_SDA;
extern unsigned char                *FUN_L00_002712b8(void  *pos,
                                                      float *vec,
                                                      int    s,
                                                      int    a,
                                                      int    col,
                                                      int    n,
                                                      float  x,
                                                      float  y,
                                                      float  z,
                                                      float  w,
                                                      float  pw);
extern void *
FUN_L00_0026bb70_c(void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern void suck_draw_reticle(void *, int, float, float, void *, int, int, float, int) __asm__("FUN_L00_001edf60");
extern void FUN_L00_001ff2c0(void *, void *, void *);
extern void FUN_L00_001ffd48(void *, void *, float);
extern void FUN_L00_001ffe20(void *, void *, void *);
extern void FUN_L00_0025a9f8(void *, void *, void *, int, int, int, int, int, float, float, float);
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void FUN_L00_0026a4c8(void *pos, void *vel, int color, int tag, int life, float size);
extern void spawn_particle(void *, void *, int, int, float, int, int, float, int, int) __asm__("FUN_L00_0026a9f0");
extern void spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern void FUN_L00_002ac910(char *, char *, int, int, float, int, int, int, int);
extern void mine_camera_blast(void *, void *, int, int) __asm__("FUN_L00_002d3838");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern char D_0013F490[];
extern char D_0013F350[];

/* update of moby class 256: a homing missile that leads its target, trails smoke and explodes on contact */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029E8A0), where it is exact; names translated to the US level program. */

void giant_clank_missile_update(M_29E8A0 *missile_moby) __asm__("FUN_L15_0029d6c0");

/* Moby class 256 */
void giant_clank_missile_update(M_29E8A0 *missile_moby) {
    Tbl_29E8A0 camera_blast_parameters;
    float      effect_bias_velocity[4];
    float      previous_position[4];
    float      burst_drift_velocity[4];
    float      burst_rotation_axis[4];
    float      trail_offset_or_position[4];
    float      missile_movement_delta[4];
    float      vB0[4];
    float      up[4];
    char      *owner;
    D_29E8A0  *missile_data;
    M_29E8A0  *target_moby;
    char      *hero;
    float      yaw;
    float      pitch;
    int        k;

    camera_blast_parameters = l15_missile_camera_blast_parameter_template;
    owner = 0;
    *(u128_29E8A0 *)burst_rotation_axis = 0;
    burst_rotation_axis[2] = D_0015ED6C * 8.0f;
    *(u128_29E8A0 *)burst_drift_velocity = *(u128_29E8A0 *)burst_rotation_axis;
    *(u128_29E8A0 *)burst_rotation_axis = 0;
    burst_rotation_axis[2] = 1.0f;
    missile_data = missile_moby->data;
    target_moby = missile_data->target;
    missile_data->speed =
        missile_data->speed + (missile_data->speed_end - missile_data->speed) / 10.0f * D_0015ED64;
    fast_dec_timer_s16(&missile_data->f3C);
    {
        float a = convert_integer_to_float(missile_data->f3C);
        float b = convert_integer_to_float(missile_data->f3E);
        fast_vec_scale(up, ((char *)&D_0013F490), a / b);
    }
    {
        float delta[4];
        float dv[4];
        float tp[4];
        float ip[4];
        if (missile_target_reticle_enabled != 0 && target_moby != 0) {
            float scaled_target_distance = fast_vec_dist(D_L15_001673C0, target_moby->pos) / 50.0f;
            qcopy(delta, target_moby->pos);
            delta[2] = delta[2] + missile_data->zoff;
            suck_draw_reticle(missile_moby,
                              0xFF0FFFFF,
                              absolute_float(0.5f - scaled_target_distance * 0.35f),
                              0.0f,
                              delta,
                              0xD,
                              -1,
                              90.0f,
                              4);
        }
        if (target_moby != 0 &&
            missile_data->timer < scale_game_frames(0x96) - scale_game_frames(0) &&
            target_moby->state >= 0) {
            float intercept_time = 0.0f;
            qcopy(tp, target_moby->pos);
            tp[2] = tp[2] + missile_data->zoff;
            fast_vec_sub(delta, tp, missile_moby->pos);
            if (missile_data->f38 != 0) {
                float a, b, c, intercept_time_plus_root, intercept_time_minus_root,
                    discriminant_sqrt;
                fast_vec_sub(dv, tp, missile_data->v);
                FUN_L00_001ff2c0(ip, dv, up);
                qcopy(missile_data->v, tp);
                c = FUN_L00_001ff430(dv);
                b = (dv[0] * delta[0] + dv[1] * delta[1] + dv[2] * delta[2]) * -2.0f;
                a = missile_data->speed * missile_data->speed - c;
                c = -FUN_L00_001ff430(delta);
                discriminant_sqrt = square_root(b * b - a * 4.0f * c);
                intercept_time_plus_root = (-b + discriminant_sqrt) / (a + a);
                intercept_time_minus_root = (-b - discriminant_sqrt) / (a + a);
                if (intercept_time_plus_root > 0.0f && intercept_time_minus_root > 0.0f) {
                    if (intercept_time_minus_root < intercept_time_plus_root)
                        intercept_time = intercept_time_plus_root;
                    else
                        intercept_time = intercept_time_minus_root;
                } else if (intercept_time_plus_root > 0.0f) {
                    intercept_time = intercept_time_plus_root;
                } else if (intercept_time_minus_root > 0.0f) {
                    intercept_time = intercept_time_minus_root;
                } else {
                    intercept_time = -1.0f;
                }
            }
            if (intercept_time > 0.0f && intercept_time < (float)scale_game_frames(0x96)) {
                fast_vec_scale(ip, dv, intercept_time);
                fast_vec_add(ip, ip, delta);
                yaw = fast_arc_tan(ip[0], ip[1]);
                pitch = -fast_arc_tan(vector_length_xy(ip), ip[2]);
            } else {
                yaw = fast_arc_tan(delta[0], delta[1]);
                pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
            }
        } else {
            if (missile_data->timer < scale_game_frames(0x96) - scale_game_frames(0)) {
                yaw = missile_data->yaw0;
                pitch = missile_data->pitch0;
            } else {
                yaw = missile_moby->rot[2];
                pitch = missile_moby->rot[1];
            }
        }
        if (target_moby != 0) {
            missile_moby->rot[2] = spring_turn(&missile_data->yawvel,
                                               missile_moby->rot[2],
                                               yaw,
                                               D_0015ED70 * 6.2831855f,
                                               D_0015ED70 * 3.1415927f,
                                               D_0015ED6C * 6.2831855f);
            missile_moby->rot[1] = spring_turn(&missile_data->pitchvel,
                                               missile_moby->rot[1],
                                               pitch,
                                               D_0015ED70 * 6.2831855f,
                                               D_0015ED70 * 3.1415927f,
                                               D_0015ED6C * 6.2831855f);
        } else {
            missile_moby->rot[2] = spring_turn(&missile_data->yawvel,
                                               missile_moby->rot[2],
                                               yaw,
                                               D_0015ED70 * 6.2831855f,
                                               D_0015ED70 * 3.1415927f,
                                               D_0015ED6C * 4.712389f);
            missile_moby->rot[1] = spring_turn(&missile_data->pitchvel,
                                               missile_moby->rot[1],
                                               pitch,
                                               D_0015ED70 * 6.2831855f,
                                               D_0015ED70 * 3.1415927f,
                                               D_0015ED6C * 4.712389f);
        }
        {
            Part *trail_particle;
            build_spherical_offset(missile_movement_delta,
                                   missile_data->speed,
                                   missile_moby->rot[2],
                                   -missile_moby->rot[1]);
            *(u128_29E8A0 *)dv = 0;
            dv[0] = random_float_between(-1.0f, 1.0f);
            dv[1] = random_float_between(-1.0f, 1.0f);
            dv[2] = random_float_between(-1.0f, 1.0f);
            *(u128_29E8A0 *)delta = *(u128_29E8A0 *)dv;
            fast_vec_normalize(delta, delta, random_float_between(0.1f, 0.2f) * D_0015ED6C);
            fast_vec_normalize(trail_offset_or_position,
                               missile_movement_delta,
                               -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
            fast_vec_add(delta, delta, trail_offset_or_position);
            fast_vec_normalize(trail_offset_or_position,
                               missile_movement_delta,
                               random_float_between(0.0f, 1.0f) * missile_data->speed);
            fast_vec_add(trail_offset_or_position, trail_offset_or_position, missile_moby->pos);
            trail_particle = (Part *)FUN_L00_002712b8(trail_offset_or_position,
                                                      delta,
                                                      scale_game_frames(0x3C),
                                                      0x7F,
                                                      0x606060,
                                                      3,
                                                      40000.0f,
                                                      1000.0f,
                                                      1.0f,
                                                      -0.0002f,
                                                      0.0f);
            if (trail_particle != 0) {
                trail_particle->f2 = *D_L15_001B26DC;
                trail_particle->f3 = 0x44;
            }
            FUN_L00_002712b8(missile_moby->pos,
                             delta,
                             scale_game_frames(6),
                             0x7F,
                             0xB0B0B0,
                             3,
                             40000.0f,
                             1000.0f,
                             1.0f,
                             -0.0004f,
                             0.0f);
            *(u128_29E8A0 *)dv = 0;
            dv[2] = 0.02f;
            transform_vector_by_basis(dv, dv, missile_moby->mtx);
            build_look_at_matrix(dv, dv, missile_moby->mtx, random_angle_radians());
            spawn_burst(trail_offset_or_position,
                        dv,
                        0x4F007FFF,
                        0x1FFFFFFF,
                        scale_game_frames(5),
                        1,
                        20000.0f);
            qcopy(previous_position, missile_moby->pos);
            add_vectors(delta, missile_movement_delta, up);
        }
    }
    fast_vec_add(missile_moby->pos, missile_moby->pos, missile_movement_delta);
    if (missile_moby->pos[0] < 0.0f || missile_moby->pos[1] < 0.0f || missile_moby->pos[2] < 0.0f) {
        mine_spawn_drops(missile_moby, missile_drop_spawn_parameters);
        mark_moby_for_removal(missile_moby);
        return;
    }
    {
        Coll coll;
        coll.flags = 0x830000;
        coll.radius = 3.0f;
        coll.moby = (char *)missile_moby;
        coll.count = 1;
        qcopy(coll.v, missile_movement_delta);
        set_vector_length_xy(coll.v, coll.v, 1.0f);
        hero = ((char *)&D_0013F350);
        coll.v[3] = 5627.9248f;
        coll.kind = 3;
        coll.id = missile_moby->id;
        coll.v[2] = 1.0f;
        coll.one = 1;
        if (collision_line(
                previous_position, missile_moby->pos, 0, *(char **)(hero + 0x2080), &coll) != 0) {
            char *hit = D_L15_00174440;
            char *hm = *(char **)(hit + 0x18);
            if (hm != 0) {
                if (hm != *(char **)(hero + 0x2080) && hm != missile_data->owner) {
                    missile_moby->bc = 2;
                    qcopy(missile_moby->pos, hit + 0x20);
                    fast_vec_sub(vB0, hit + 0x50, hit + 0x60);
                    owner = (char *)missile_moby;
                    fast_vec_normalize(vB0, vB0, 1.0f);
                    reflect_vector(burst_drift_velocity, missile_movement_delta, hit + 0x40);
                    fast_vec_normalize(
                        burst_drift_velocity, burst_drift_velocity, D_0015ED6C + D_0015ED6C);
                    fast_vec_normalize(burst_rotation_axis, hit + 0x40, 1.0f);
                }
            } else if (*(int *)(hit + 0x1C) > 0) {
                missile_moby->bc = 1;
                qcopy(missile_moby->pos, hit + 0x20);
                fast_vec_sub(vB0, hit + 0x50, hit + 0x60);
                fast_vec_normalize(vB0, vB0, 1.0f);
                reflect_vector(burst_drift_velocity, missile_movement_delta, hit + 0x40);
                fast_vec_normalize(
                    burst_drift_velocity, burst_drift_velocity, D_0015ED6C + D_0015ED6C);
                fast_vec_normalize(burst_rotation_axis, hit + 0x40, 1.0f);
            }
        } else if (fast_dec_timer(&missile_data->timer) != 0 ||
                   vector_distance_xy(missile_moby->pos, (float *)(hero + 0x80)) > 260.0f) {
            mine_spawn_drops(missile_moby, missile_drop_spawn_parameters);
            mark_moby_for_removal(missile_moby);
            return;
        }
        if (missile_moby->bc != 0) {
            float burst_position_or_velocity[4];
            float effect_reference_distance;
            char *blast_attenuation_view;
            int   i, j;
            int   r = collect_mobies_in_sphere(missile_moby->pos, 2.0f, 0x10, missile_moby, 0);
            *(u128_29E8A0 *)burst_position_or_velocity = *(u128_29E8A0 *)missile_moby->pos;
            FUN_L00_0025a9f8(missile_moby,
                             burst_position_or_velocity,
                             ((char **)D_L15_00178500),
                             r,
                             (int)owner,
                             0x810000,
                             3,
                             1,
                             3.0f,
                             1.0f,
                             1.0f);
            {
                float K[16];
                float K2[16];
                float M3[16];
                float M2[16];
                float M1[16];
                float ny;
                K[4] = -burst_rotation_axis[2];
                K[1] = burst_rotation_axis[2];
                K[0] = 0.0f;
                K[8] = burst_rotation_axis[1];
                ny = -burst_rotation_axis[1];
                K[9] = -burst_rotation_axis[0];
                K[2] = ny;
                K[6] = burst_rotation_axis[0];
                K[12] = 0.0f;
                K[5] = 0.0f;
                K[13] = 0.0f;
                K[10] = 0.0f;
                K[14] = 0.0f;
                K[3] = 0.0f;
                K[7] = 0.0f;
                K[11] = 0.0f;
                K[15] = 0.0f;
                multiply_matrix(K2, K, K);
                for (k = 0; k < 10; k++) {
                    if (missile_moby->bc == 1) {
                        float burst_rotation_angle = random_angle_radians();
                        FUN_L00_001ffd48(M1, K2, 1.0f - fast_cos(burst_rotation_angle));
                        FUN_L00_001ffd48(M2, K, fast_sin(burst_rotation_angle));
                        FUN_L00_001ffe20(M2, M2, M1);
                        set_identity_matrix(M1);
                        FUN_L00_001ffe20(M3, M2, M1);
                        transform_vector(burst_position_or_velocity, vB0, M3);
                        fast_vec_normalize(burst_position_or_velocity,
                                           burst_position_or_velocity,
                                           random_float_between(8.5f, 16.5f) * D_0015ED6C);
                    } else {
                        float sp = random_float_between(8.5f, 16.5f) * D_0015ED6C;
                        float random_yaw = random_angle_radians();
                        build_spherical_offset(
                            burst_position_or_velocity, sp, random_yaw, random_angle_radians());
                    }
                    burst_position_or_velocity[2] =
                        burst_position_or_velocity[2] + D_0015ED6C * 5.0f;
                    fast_vec_add(burst_position_or_velocity,
                                 burst_position_or_velocity,
                                 burst_drift_velocity);
                    FUN_L00_0026bb70_c(
                        missile_moby->pos,
                        burst_position_or_velocity,
                        0x4F007FFF,
                        0x1F00007F,
                        40000.0f,
                        FUN_L00_00257b90(scale_game_frames(0x3C), scale_game_frames(0x78)),
                        1,
                        -1,
                        -1);
                }
                {
                    float random_bias_direction[4];
                    {
                        *(u128_29E8A0 *)random_bias_direction = 0;
                        random_bias_direction[0] = random_float_between(-1.0f, 1.0f);
                        random_bias_direction[1] = random_float_between(-1.0f, 1.0f);
                        random_bias_direction[2] = random_float_between(-1.0f, 1.0f);
                        *(u128_29E8A0 *)burst_position_or_velocity =
                            *(u128_29E8A0 *)random_bias_direction;
                    }
                }
                fast_vec_sub(effect_bias_velocity, D_L15_001673C0, missile_moby->pos);
                effect_reference_distance = fast_vec_length(effect_bias_velocity);
                effect_bias_velocity[2] =
                    effect_bias_velocity[2] + effect_reference_distance * 0.5f;
                fast_vec_normalize(burst_position_or_velocity,
                                   burst_position_or_velocity,
                                   effect_reference_distance / 5.0f * D_0015ED6C);
                fast_vec_normalize(effect_bias_velocity,
                                   effect_bias_velocity,
                                   (effect_reference_distance + effect_reference_distance) *
                                       D_0015ED6C);
                fast_vec_add(
                    burst_position_or_velocity, burst_position_or_velocity, effect_bias_velocity);
                clamp_vector_length(
                    burst_position_or_velocity, burst_position_or_velocity, D_0015ED6C * 10.0f);
                spawn_debris((char *)missile_moby->pos,
                             (char *)burst_position_or_velocity,
                             FUN_L00_00257b90(scale_game_frames(0x3C), scale_game_frames(0x5A)),
                             0,
                             0);
                {
                    int   burst_effect_pair_count;
                    float under;
                    if (effect_reference_distance < 6.0f) {
                        burst_effect_pair_count =
                            truncate_float_to_s32(effect_reference_distance) / 2;
                    } else {
                        burst_effect_pair_count = 3;
                    }
                    under = 0.0f;
                    if (effect_reference_distance < 7.0f)
                        under = 7.0f - effect_reference_distance;
                    i = 0;
                    while (i < burst_effect_pair_count) {
                        float sz =
                            random_float_between(8.0f, 10.0f) * D_0015ED6C - under * D_0015ED6C;
                        Cols c1 = l15_missile_burst_primary_color_palette;
                        Cols c2 = l15_missile_burst_secondary_color_palette;
                        int *p1 = &c1.c[random_integer_below(6)];
                        int *p2 = &c2.c[random_integer_below(6)];
                        int  colored_burst_effect_frame_count =
                            FUN_L00_00257b90(scale_game_frames(0xF), scale_game_frames(0x14));
                        int colored_burst_effect_secondary_frame_count =
                            FUN_L00_00257b90(scale_game_frames(0x19), scale_game_frames(0x1E));
                        int white_burst_effect_frame_count,
                            white_burst_effect_secondary_frame_count;
                        spawn_particle(missile_moby->pos,
                                       burst_drift_velocity,
                                       *p1,
                                       *p2,
                                       400000.0f,
                                       colored_burst_effect_frame_count,
                                       colored_burst_effect_secondary_frame_count,
                                       sz,
                                       0,
                                       0);
                        white_burst_effect_frame_count =
                            FUN_L00_00257b90(scale_game_frames(5), scale_game_frames(0xA));
                        white_burst_effect_secondary_frame_count =
                            FUN_L00_00257b90(scale_game_frames(0xF), scale_game_frames(0x14));
                        spawn_particle(missile_moby->pos,
                                       burst_drift_velocity,
                                       0x7FFFFFFF,
                                       0xFFFFFF,
                                       400000.0f,
                                       white_burst_effect_frame_count,
                                       white_burst_effect_secondary_frame_count,
                                       sz * 0.5f,
                                       0,
                                       0);
                        i++;
                    }
                }
                for (j = 0; j < 10; j++) {
                    Cols  c1 = l15_missile_burst_primary_color_palette;
                    Cols  c2 = l15_missile_burst_secondary_color_palette;
                    float burst_particle_velocity[4];
                    {
                        float random_burst_direction[4];
                        *(u128_29E8A0 *)random_burst_direction = 0;
                        random_burst_direction[0] = random_float_between(-1.0f, 1.0f);
                        random_burst_direction[1] = random_float_between(-1.0f, 1.0f);
                        random_burst_direction[2] = random_float_between(-1.0f, 1.0f);
                        *(u128_29E8A0 *)burst_particle_velocity =
                            *(u128_29E8A0 *)random_burst_direction;
                    }
                    fast_vec_normalize(burst_particle_velocity,
                                       burst_particle_velocity,
                                       random_float_between(0.0f, 3.0f) * D_0015ED6C);
                    {
                        int *p1 = &c1.c[random_integer_below(6)];
                        int *p2 = &c2.c[random_integer_below(6)];
                        FUN_L00_0026a4c8(
                            missile_moby->pos,
                            burst_particle_velocity,
                            *p1,
                            *p2,
                            FUN_L00_00257b90(scale_game_frames(0x14), scale_game_frames(0x23)),
                            200000.0f);
                    }
                }
                if (missile_distant_gray_glow_gate < 0.95f && 9.0f < effect_reference_distance) {
                    ((void (*)(void *, void *, void *, float, int, int, int, int, int))
                         FUN_L00_002ac910)(missile_moby,
                                           missile_moby->pos,
                                           burst_drift_velocity,
                                           4.0f,
                                           scale_game_frames(0xF),
                                           0x7F,
                                           0x7F,
                                           0x7F,
                                           0x20);
                    ((void (*)(void *, void *, void *, float, int, int, int, int, int))
                         FUN_L00_002ac910)(missile_moby,
                                           missile_moby->pos,
                                           burst_drift_velocity,
                                           4.0f,
                                           scale_game_frames(0x18),
                                           0x7F,
                                           0x7F,
                                           0x7F,
                                           0x20);
                }
                ((void (*)(void *, void *, void *, float, int, int, int, int, int))
                     FUN_L00_002ac910)(missile_moby,
                                       missile_moby->pos,
                                       burst_drift_velocity,
                                       4.0f,
                                       scale_game_frames(0x14),
                                       0x7F,
                                       0x7F,
                                       0,
                                       0x30);
                ((void (*)(void *, void *, void *, float, int, int, int, int, int))
                     FUN_L00_002ac910)(missile_moby,
                                       missile_moby->pos,
                                       burst_drift_velocity,
                                       2.0f,
                                       scale_game_frames(0x13),
                                       0xFF,
                                       0xFF,
                                       0xFF,
                                       0x20);
                missile_movement_delta[0] = 0.0f;
                blast_attenuation_view = (char *)&l15_camera;
                missile_moby->bc = 0;
                if (effect_reference_distance < 20.0f) {
                    *(float *)(blast_attenuation_view + 0x160) =
                        0.4f - effect_reference_distance * 0.0175f;
                } else {
                    *(float *)(blast_attenuation_view + 0x160) = 0.050000012f;
                }
                {
                    int   sh = scale_game_frames(0x19);
                    char *blast_timing_view = (char *)&l15_camera;
                    *(int *)(blast_timing_view + 0x168) = sh;
                }
                allocate_voice_for_target_entry(0, 0, (int)missile_moby);
                mine_camera_blast(&camera_blast_parameters, missile_moby->pos, 0, 0);
                mine_spawn_drops(missile_moby, missile_drop_spawn_parameters);
                mark_moby_for_removal(missile_moby);
            }
        }
    }
}

Cols D_L18_001F1F30 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Cols D_L18_001F1F48 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

Tbl_29E8A0 D_L18_001F1EE0 = {
    {0,         0,          0x3f800000, 0,          0xff0000,   0xff000980, 0xc8000,
     0xd800064, 0x41a00000, 0x41a00000, 0x41a00000, 0x3c23d70a, 0x3c23d70a, 0x46,
     0x80000,   0xff,       0,          0,          0x300,      0}
};

Cols l15_missile_burst_primary_color_palette __asm__("D_L15_001E2660") = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Cols l15_missile_burst_secondary_color_palette __asm__("D_L15_001E2678") = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

Tbl_29E8A0 l15_missile_camera_blast_parameter_template __asm__("D_L15_001E2610") = {
    {0,         0,          0x3f800000, 0,          0xff0000,   0xff000980, 0xc8000,
     0xd800064, 0x41a00000, 0x41a00000, 0x41a00000, 0x3c23d70a, 0x3c23d70a, 0x46,
     0x80000,   0xff,       0,          0,          0x300,      0}
};
