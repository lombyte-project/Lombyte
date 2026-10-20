/* Shared moby classes: rocket, ocean_pokitaru. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"

extern float FUN_001f9b48_b7(void *, void *) __asm__("FUN_001f9b48");
extern int FUN_001f96f8_b7(int) __asm__("FUN_001f96f8");
extern int FUN_00116248_b7(void *, void *) __asm__("FUN_00116248");
extern void FUN_0020c828_b7(void *) __asm__("FUN_0020c828");

extern unsigned char D_0013D389[];
extern char D_L11_00179B18[];
extern char D_L11_0021A900[];
extern int D_L11_0015F640;
void FUN_L11_002cb790(char *moby) {
    if (FUN_001f9b48_b7(moby + 0x10, &hero.motion.pos) < 2.0f) {
        D_0013D389[0] = 1;
        FUN_00116248_b7(D_L11_00179B18, D_L11_0021A900);
        D_L11_0015F640 = FUN_001f96f8_b7(0xB4);
        FUN_0020c828_b7(moby);
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00309A40), where it is exact; names translated to the US level program. */

extern char *D_L11_0015FFE4;
extern float square_root(float) __asm__("FUN_001f9988");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");

char *FUN_L11_00308670(float *pos, float *aim, float max_yaw_error, float max_pitch_error, float maxd) {
    char *best = 0;
    char *candidate_position;
    char *candidate_moby = D_L11_0015FFE4;
    float bestd = 10.0f;
    while (candidate_moby != 0) {
        if ((*(unsigned short *)(candidate_moby + 0x34) & 0x1000) && candidate_moby != 0 && *(char **)(candidate_moby + 0x24) != 0 &&
            *(short *)(*(char **)(candidate_moby + 0x24) + 0x46) == 5) {
            float distance_xy;
            candidate_position = candidate_moby + 0x10;
            distance_xy = vector_distance_xy(pos, candidate_position);
            if (distance_xy < maxd) {
                float adjusted_target_position[4];
                float ra, rb, y, score;
                qcopy(adjusted_target_position, candidate_position);
                adjusted_target_position[2] = adjusted_target_position[2] + 0.4f;
                ra = fast_difference_between_rotations(
                    aim[2], fast_arc_tan(adjusted_target_position[0] - pos[0], adjusted_target_position[1] - pos[1]));
                ra = ra * ra;
                if (ra < max_yaw_error * max_yaw_error) {
                    y = fast_difference_between_rotations(aim[1], fast_arc_tan(distance_xy, adjusted_target_position[2] - pos[2]));
                    rb = y * y;
                    if (rb < max_pitch_error * max_pitch_error) {
                        score = ra * rb * 30.0f;
                        score = score + square_root(fast_vec_dist(pos, adjusted_target_position));
                        if (score < bestd) {
                            bestd = score;
                            best = candidate_moby;
                        }
                    }
                }
            }
        }
        candidate_moby = *(char **)(candidate_moby + 0x28);
    }
    return best;
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* spawns a moby of class 0x3F1 aimed along a direction */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00309C18), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void FUN_L00_00250df8(void *);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

unsigned char *FUN_L11_00308848(char *owner, float *dir, float *pos, float travel_distance, float b) {
    unsigned char *spawned_moby = func_0020D348_m(0x3F1);
    if (spawned_moby) {
        char *moby_vars = *(char **)(spawned_moby + 0x78);
        float direction_length_xy;
        *(char **)(moby_vars + 0x1C) = owner;
        spawned_moby[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = func_001FA898_r(travel_distance);
        *(int *)(spawned_moby + 0x40) = 0;
        spawned_moby[0x31] = 1;
        direction_length_xy = vector_length_xy(dir);
        *(float *)(spawned_moby + 0x44) = -fast_arc_tan(direction_length_xy, dir[2]);
        *(float *)(spawned_moby + 0x48) = fast_arc_tan(dir[0], dir[1]);
        *(float *)(moby_vars + 0x10) = 1.0f;
        if (b > 0.0f) {
            fast_vec_normalize(moby_vars, dir, b);
        } else {
            qcopy(moby_vars, dir);
            b = fast_vec_length(dir);
        }
        qcopy(spawned_moby + 0x10, pos);
        *(int *)(moby_vars + 0x18) = *(int *)(moby_vars + 0x14) = func_001FA898_r(travel_distance / b);
        *(short *)(spawned_moby + 0x34) = 0x200;
        *(float *)(spawned_moby + 0x2C) =
            *(float *)(*(char **)(spawned_moby + 0x24) + 0x24) *
            (*(float *)(owner + 0x2C) / *(float *)(*(char **)(owner + 0x24) + 0x24));
        spawned_moby[0x23] = func_001FA898_r(*(float *)(moby_vars + 0x10) * 24.0f);
        set_moby_color(spawned_moby, 0xFF, 0xFF, 0xFF);
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
#include "rnc/overlay/quad.h"
#include "rnc/gameplay/entities/moby.h"

typedef struct {
    float x, y, z;
} Vec3_089d0;

typedef struct {
    float vel[4];
    float size;
    int unk14;
    int timer;
    struct Moby *owner;
    float target_pos[3];
    struct Moby *target;
    float *aim;
    float *src;
} MissileVars_089d0;

typedef struct {
    u8 pad0[0x1A];
    s16 oclass;
    u8 pad1C[0x14];
} MissileHit_089d0;

extern char *D_L11_0015FFD8_089d0 __asm__("D_L11_0015FFD8");
extern char D_L11_00174880[];
extern int ftoi_089d0(float) __asm__("FUN_001fa6d0");
extern float distance_089d0(void *, void *) __asm__("FUN_001f9b48");
extern float vector_length_089d0(void *) __asm__("FUN_001f9af0");
extern float angle_xy_089d0(float, float) __asm__("FUN_001f9e90");
extern float subtract_rotations_089d0(float, float) __asm__("FUN_001fa5c8");
extern float add_rotations_089d0(float, float) __asm__("FUN_001fa580");
extern float absolute_float_089d0(float) __asm__("FUN_001f99c0");
extern float distance_xy_089d0(void *, void *) __asm__("FUN_001f9b80");
extern void build_spherical_offset_089d0(void *, float, float, float) __asm__("FUN_00214db0");
extern void no_op_089d0(void *, void *) __asm__("FUN_001e93e8");
extern void add_vector_089d0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_089d0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_00259858(float, void *, struct Moby *, int);
extern int collision_line_089d0(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern float random_float_between_089d0(float, float) __asm__("FUN_002132a8");
extern void reflect_089d0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int scale_game_frames_089d0(int) __asm__("FUN_001f96f8");
extern int random_int_between_089d0(int, int) __asm__("FUN_L00_00257b90");
extern char *FUN_L00_0026dd20_c(void *, void *, int, int, float) __asm__("FUN_L00_0026dd20");
extern void remove_moby_089d0(void *) __asm__("FUN_0020c828");
extern int tick_countdown_089d0(int *) __asm__("FUN_001f9740");
char *FUN_L11_00308670(float *pos, float *aim, float r1, float r2, float maxd);

static inline void copy_xyz_089d0(float *dst, float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

/* Homing missile: eases its scale, re-picks a target every sixth frame, turns toward it within a cone and flies on;
   on impact it bursts into five sparks, otherwise it shrinks away once its timer runs out. */
void FUN_L11_003089d0(struct Moby *moby) {
    MissileVars_089d0 *vars = (MissileVars_089d0 *)moby->pvars;
    float old[4];
    float look[4];
    MissileHit_089d0 hit;
    float spark_velocity[4];
    float r[4];
    struct Moby *target;
    float speed, yaw, yaw_off, pitch, pitch_off, max_yaw, max_pitch;
    int i;

    moby->scale += (moby->pclass->scale - moby->scale) * 0.1f;
    qcopy(old, &moby->pos);
    if (level_frame % 6 == (((char *)moby - D_L11_0015FFD8_089d0) >> 8) % 6)
        vars->target = 0;
    moby->alpha = ftoi_089d0(vars->size * 24.0f);
    copy_xyz_089d0(look, vars->target_pos);
    ((int *)look)[3] = 0;
    target = vars->target;
    if (target == 0 || target->state == 0xFE || target->state == 0xFD ||
        distance_089d0(&target->pos, look) > 3.0f)
        vars->target = (struct Moby *)FUN_L11_00308670((float *)&moby->pos, (float *)&moby->rot, 0.2617994f, 0.2617994f, 60.0f);
    if (vars->target != 0) {
        copy_xyz_089d0(vars->target_pos, (float *)&vars->target->pos);
        speed = vector_length_089d0(vars);
        yaw_off = subtract_rotations_089d0(angle_xy_089d0(vars->target->pos.x - moby->pos.x, vars->target->pos.y - moby->pos.y), moby->rot.z);
        yaw = absolute_float_089d0(yaw_off);
        pitch_off = subtract_rotations_089d0(-angle_xy_089d0(distance_xy_089d0(&moby->pos, &vars->target->pos), vars->target->pos.z - moby->pos.z), moby->rot.y);
        pitch = absolute_float_089d0(pitch_off);
        if (yaw < 0.2617994f && pitch < 0.2617994f) {
            max_yaw = frame_time * 0.87266463f;
            if (max_yaw < yaw) {
                yaw_off /= yaw;
                yaw_off *= max_yaw;
            }
            moby->rot.z = add_rotations_089d0(moby->rot.z, yaw_off);
            max_pitch = frame_time * 0.87266463f;
            if (max_pitch < pitch) {
                pitch_off /= pitch;
                pitch_off *= max_pitch;
            }
            moby->rot.y = add_rotations_089d0(moby->rot.y, pitch_off);
            build_spherical_offset_089d0(vars, speed, moby->rot.z, -moby->rot.y);
            no_op_089d0(&moby->pos, &vars->target->pos);
        } else {
            vars->target = 0;
        }
    }
    add_vector_089d0(&moby->pos, &moby->pos, vars);
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
        moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        remove_moby_089d0(moby);
        return;
    }
    FUN_L00_00259858(1.0f, &hit, moby, 0x10000);
    hit.oclass = moby->oclass;
    if (collision_line_089d0(old, &moby->pos, 0, vars->owner, &hit)) {
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)r = 0;
            r[0] = random_float_between_089d0(-1.0f, 1.0f);
            r[1] = random_float_between_089d0(-1.0f, 1.0f);
            r[2] = random_float_between_089d0(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)r;
            reflect_089d0(r, vars, D_L11_00174880);
            normalize_vector_089d0(spark_velocity, spark_velocity, vector_length_089d0(r) * 0.5f);
            add_vector_089d0(spark_velocity, r, spark_velocity);
            normalize_vector_089d0(spark_velocity, spark_velocity, random_float_between_089d0(frame_time * 5.0f, frame_time * 9.0f));
            FUN_L00_0026dd20_c(&moby->pos, spark_velocity, 0x7F2F4F6F, random_int_between_089d0(scale_game_frames_089d0(10), scale_game_frames_089d0(15)), 90000.0f);
        }
        remove_moby_089d0(moby);
        return;
    }
    if (tick_countdown_089d0(&vars->timer)) {
        vars->size *= 0.74f;
        if (vars->size < 0.02f)
            remove_moby_089d0(moby);
    }
}
/* spawns a streak moby aimed along a direction with the given size and colour */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030A318), where it is exact; names translated to the US level program. */

char *FUN_L11_00308f48(int owner, float *dir, float *pos, float size, float len, float z) {
    char *spawned_moby = func_0020D348_m(0x3F9);
    if (spawned_moby != 0) {
        char *moby_vars = *(char **)(spawned_moby + 0x78);
        int r;
        *(int *)(spawned_moby + 0x94) = 0;
        *(int *)(moby_vars + 0x1C) = owner;
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = truncate_float_to_s32(size);
        *(int *)(spawned_moby + 0x40) = 0;
        spawned_moby[0x31] = 1;
        *(float *)(spawned_moby + 0x44) = -fast_arc_tan(vector_length_xy(dir), dir[2]);
        *(float *)(spawned_moby + 0x48) = fast_arc_tan(dir[0], dir[1]);
        *(float *)(moby_vars + 0x10) = 1.0f;
        if (len > 0.0f) {
            fast_vec_normalize(moby_vars, dir, len);
        } else {
            qcopy(moby_vars, dir);
            len = fast_vec_length(dir);
        }
        qcopy(spawned_moby + 0x10, pos);
        r = truncate_float_to_s32(size / len);
        *(int *)(moby_vars + 0x14) = r;
        *(float *)(moby_vars + 0x20) = z;
        *(int *)(moby_vars + 0x18) = r - 13;
        *(short *)(spawned_moby + 0x34) = 0x200;
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030A468), where it is exact; names translated to the US level program. */
extern char *FUN_L00_0026dd20(char *pos, char *vel, int c, int d, float f);
extern char D_L11_00174880[];
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern int D_L11_00161DC0 __attribute__((sda));
extern int D_L11_00161DC4 __attribute__((sda));
extern int D_L11_00161DC8 __attribute__((sda));
extern int D_L11_00161DCC __attribute__((sda));
extern float D_L11_00161DD0 __attribute__((sda));
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
/* Per-frame update of a projectile moby: moves it by its velocity and leaves a trail particle; on impact it bursts into
   five sparks and removes itself, otherwise it shrinks when its timer runs out and is removed once tiny. */
void FUN_L11_00309098(char *moby) {
    float old[4];
    float trail_position[4];
    float v[4];
    float hit[4];
    float pad[4][2];
    float spark_velocity[4];
    float r[4];
    char *data;
    char *pos;
    int trail_color;
    if (moby == 0) return;
    data = *(char **)(moby + 0x78);
    if (data == 0) return;
    pos = moby + 0x10;
    qcopy(old, pos);
    trail_color = fast_tween_color(D_L11_00161DC0, D_L11_00161DC4, random_float_between(0.0f, 1.0f));
    random_vector(trail_position, 0.0f, 0.1f);
    add_vector_xyz(trail_position, trail_position, pos);
    scale_vector_xyz(v, data, D_L11_00161DD0);
    FUN_L00_0026dd20((char *)trail_position, (char *)v, trail_color, scale_game_frames(D_L11_00161DC8), (float)D_L11_00161DCC);
    add_vector_xyz(pos, pos, data);
    make_hit_descriptor(hit, moby, 0x10000, *(float *)(data + 0x20), data);
    if (collision_line(old, pos, 0, *(void **)(data + 0x1C), hit)) {
        int i;
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)r = 0;
            r[0] = random_float_between(-1.0f, 1.0f);
            r[1] = random_float_between(-1.0f, 1.0f);
            r[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)spark_velocity = *(OvlQuad *)r;
            FUN_L00_001ff660(r, data, D_L11_00174880);
            normalize_vector_xyz(spark_velocity, spark_velocity, vector_length_xyz(r) * 0.5f);
            add_vector_xyz(spark_velocity, r, spark_velocity);
            normalize_vector_xyz(spark_velocity, spark_velocity, random_float_between(frame_time * 5.0f, frame_time * 9.0f));
            FUN_L00_0026dd20(moby + 0x10, (char *)spark_velocity, 0x7F2F4F6F, random_int_between(scale_game_frames(10), scale_game_frames(15)), 90000.0f);
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (tick_countdown_32_alt((int *)(data + 0x18))) {
        *(float *)(data + 0x10) *= 0.74f;
        if (*(float *)(data + 0x10) < 0.02f) {
            mark_moby_for_removal(moby);
        }
    }
}
typedef struct {
    Vec4 step;             /* moved by this each frame; its length is speed */
    Vec4 target_pos;       /* 0x10 */
    void *collision;       /* 0x20 */
    struct Moby *target;   /* 0x24 */
    s32 timer;             /* 0x28 */
    f32 speed;             /* 0x2C */
    f32 turn_speed;        /* 0x30 */
} Missile_94f8;

extern float random_angle_radians_94f8(void) __asm__("FUN_00213308");
extern float distance_xyz_94f8(void *, void *) __asm__("FUN_001f9b48");
extern float distance_xy_94f8(void *, void *) __asm__("FUN_001f9b80");
extern float angle_xy_94f8(float, float) __asm__("FUN_001f9e90");
extern void transform_vector_by_basis_94f8(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void build_look_at_matrix_94f8(void *, void *, void *, float) __asm__("FUN_00214890");
extern void FUN_L00_0026cbb0_94f8(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern unsigned char *FUN_L00_002712b8_94f8(void *, void *, int, int, int, int,
                                              float, float, float, float, float) __asm__("FUN_L00_002712b8");
extern float FUN_L00_0025be00_94f8(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void build_spherical_offset_94f8(void *, float, float, float) __asm__("FUN_00214db0");
extern void moby_point_position_94f8(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_0025e450_94f8(void *, void *, void *, float, float, int, int, int,
                                    float, float, float, float, int, float, int, int, int, int)
    __asm__("FUN_L00_0025e450");

extern char D_L11_001B2A80[];
extern char D_L11_00174860[];
/* Homing missile: turns toward its target while the target lives and stays within 3 units of
   where it was last frame, leaves a smoke trail, and bursts on a hit, when its timer runs out
   or outside the level box. */
void rocket_update(struct Moby *moby) __asm__("FUN_L11_003094f8");
void rocket_update(struct Moby *moby) {
    Missile_94f8 *data = (Missile_94f8 *)moby->pvars;
    Vec4f *pos;
    Vec4 previous_pos, spray, offset, velocity, random;
    float fraction;
    struct Moby *target;
    char *texture;

    qcopy(&previous_pos, &moby->pos);
    moby->scale += (moby->pclass->scale - moby->scale) * 0.1f;
    random.q = 0;
    random.f[0] = random_float_between(-1.0f, 1.0f);
    random.f[1] = random_float_between(-1.0f, 1.0f);
    random.f[2] = random_float_between(-1.0f, 1.0f);
    velocity.q = random.q;
    normalize_vector_xyz(&velocity, &velocity, random_float_between(0.1f, 0.2f) * frame_time);
    normalize_vector_xyz(&offset, &data->step, -random_float_between(frame_time * 0.1f, frame_time));
    add_vector_xyz(&velocity, &velocity, &offset);
    FUN_L00_002712b8_94f8(&moby->pos, &velocity, scale_game_frames(6), 0x7f, 0xb0b0b0,
                           3, 40000.0f, 1000.0f, 1.0f, -0.0004f, 0.0f);
    texture = D_L11_001B2A80;
    for (fraction = 0.0f; fraction < 1.0f; fraction += 0.33333334f) {
        unsigned char *particle;
        normalize_vector_xyz(&offset, &data->step, random_float_between(fraction, fraction + 0.33333334f) * data->speed);
        add_vector_xyz(&offset, &offset, &moby->pos);
        particle = FUN_L00_002712b8_94f8(&offset, &velocity, scale_game_frames(60), 0x7f, 0x606060, 3, 40000.0f,
                                          1000.0f, 1.0f, -0.0002f, 0.0f);
        if (particle != 0) {
            unsigned char *texture_pixels = *(unsigned char **)(texture + 0x5c);
            unsigned char texture_pixel = texture_pixels[0];
            particle[3] = 0x44;
            particle[2] = texture_pixel;
        }
        random.q = 0;
        random.f[2] = 0.02f;
        transform_vector_by_basis_94f8(&random, &random, &moby->rot_row0);
        build_look_at_matrix_94f8(&random, &random, &moby->rot_row0, random_angle_radians_94f8());
        FUN_L00_0026cbb0_94f8(&offset, &random, 0x4f007fff, 0x1fffffff, scale_game_frames(5), 1, 20000.0f);
    }
    pos = &moby->pos;
    target = data->target;
    if (target != 0 && target->state != 0xfe && target->state != 0xfd &&
        distance_xyz_94f8(&data->target_pos, &target->pos) < 3.0f) {
        moby->rot.z = angle_xy_94f8(data->target->pos.x - moby->pos.x, data->target->pos.y - moby->pos.y);
        moby->rot.y = -angle_xy_94f8(distance_xy_94f8(pos, &data->target->pos), data->target->pos.z - moby->pos.z);
        qcopy(&data->target_pos, &data->target->pos);
    } else {
        FUN_L00_0025be00_94f8(&moby->rot.z, &data->turn_speed, moby->rot.z, frame_time_sq * 251.32741f,
                               frame_time_sq * 251.32741f, frame_time * 2513.2742f);
        FUN_L00_0025be00_94f8(&moby->rot.y, &data->turn_speed, moby->rot.y, frame_time_sq * 251.32741f,
                               frame_time_sq * 251.32741f, frame_time * 2513.2742f);
        data->target = 0;
    }
    build_spherical_offset_94f8(&data->step, data->speed, moby->rot.z, -moby->rot.y);
    add_vector_xyz(pos, pos, &data->step);
    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f || 1021.0f < moby->pos.y ||
        moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    moby_point_position_94f8(moby, 1, &spray);
    if (collision_line(&previous_pos, &spray, 0, data->collision, 0)) {
        FUN_L00_0025e450_94f8(moby, data, D_L11_00174860, 2.0f, 10.0f, 10, 3, 0x10, 4.0f, 2.0f, 9.0f, 1.0f,
                               -1, 20.0f, 0, 1, -1, 0);
        mark_moby_for_removal(moby);
        return;
    }
    if (tick_countdown_32_alt(&data->timer))
        mark_moby_for_removal(moby);
}
extern void FUN_001f97b0_30a908(void) __asm__("FUN_001f97b0");
extern void FUN_001f9a10_30a908(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a68_30a908(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f9b20_30a908(void *) __asm__("FUN_001f9b20");
extern void FUN_001f9bf8_30a908(void *, void *, float) __asm__("FUN_001f9bf8");
extern int FUN_001fa6e0_30a908(int, int, float) __asm__("FUN_001fa6e0");
typedef struct { float u, v; } UV_30a908;
typedef struct { float x, y, z; } Pos_30a908;
typedef struct {
    Vec4f v[4];
    int col[4];
    UV_30a908 uv[4];
    long z70;
    long z78;
    long q80;
    long q88;
} Quad_30a908;
typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    float f14;
    float f18;
    int f1C;
    int f20;
    float f24;
    float f28;
    float f2C;
} Data_30a908;
typedef struct { char pad[0x140]; float x, y, z; } Cam_30a908;
typedef struct { char pad[0x228]; float f228, f22C; } Fog_30a908;

extern void FUN_00233980_30a908(int, long) __asm__("FUN_00233980");
extern int FUN_001f44b8_30a908(int) __asm__("FUN_001f44b8");
extern void FUN_001f76a0_30a908(void) __asm__("FUN_001f76a0");
extern void FUN_001f9cf8_30a908(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_001f9a28_30a908(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001fa6c0_30a908(int) __asm__("FUN_001fa6c0");
extern void func_L00_001FDE48_30a908(int, void *, void *, void *, int) __asm__("FUN_L00_001fde98");
extern void FUN_001f7d30_30a908(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L11_001677C0_30a908[] __asm__("D_L11_001677C0");
extern UV_30a908 D_L11_00161E08_30a908[] __asm__("D_L11_00161E08") __attribute__((section(".sdata")));

extern char D_L11_0016D440[];
extern char D_L11_001679D0[];
extern Pos_30a908 D_L11_001D9B08[];
extern int D_L11_001D9E08[];
extern UV_30a908 D_L11_001DA310[];
extern Pos_30a908 D_L11_001D9F08[];
extern int D_L11_001DA208[];
extern UV_30a908 D_L11_001DA510[];
extern float wave_speed[2] __asm__("D_L11_00161DF8") __attribute__((sda));
extern float tide_level_macro __asm__("D_L11_00161E18") __attribute__((section(".sdata")));
/* Draws the level's water sheet: a fading edge strip, a far quad, and two scrolling layers of grid cells around the camera. */
void FUN_L11_0030a908(struct Moby *moby) {
    Vec4f scratch_vector;
    Vec4f side_offset;
    Vec4f a;
    Vec4f b;
    Vec4f strip_corners[4];
    Vec4f first_strip_edge_delta;
    Vec4f second_strip_edge_delta;
    Quad_30a908 far_quad;
    Data_30a908 *ocean_vars = (Data_30a908 *)moby->pvars;
    float saved_fog_228 = ((Fog_30a908 *)D_L11_0016D440)->f228;
    float s22C = ((Fog_30a908 *)D_L11_0016D440)->f22C;
    int i;
    int layer;
    int colA;
    int colB;
    int vertex_count;
    float t;
    float x;
    float y;

    if (hero.state.current != 0x1D && D_L11_0016D180 == 0) {
        ((Fog_30a908 *)D_L11_0016D440)->f228 = 255.0f;
        ((Fog_30a908 *)D_L11_0016D440)->f22C = 249.9f;
    }
    FUN_00233980_30a908(0x42, 0x3200000044L);
    FUN_00233980_30a908(8, 0);
    FUN_00233980_30a908(0x14, 0xFF9000000260L);
    FUN_00233980_30a908(0x47, 0x513F1);
    FUN_00233980_30a908(6, FUN_001f44b8_30a908(0x16));
    FUN_001f76a0_30a908();
    a.x = 1.0f;
    a.y = 0.0f;
    a.z = 0.0f;
    a.w = 1.0f;
    FUN_001f9cf8_30a908(&a, &a, D_L11_001679D0);
    b.x = 0.0f;
    b.y = 1.0f;
    b.z = 0.0f;
    b.w = 1.0f;
    FUN_001f9cf8_30a908(&b, &b, D_L11_001679D0);
    a.z = 0.0f;
    FUN_001f9bf8_30a908(&a, &a, 1.0f);
    b.z = 0.0f;
    FUN_001f9bf8_30a908(&b, &b, 1.0f);
    FUN_001f9bf8_30a908(&scratch_vector, &a, -75.0f);
    FUN_001f9bf8_30a908(&side_offset, &b, -87.0f);
    FUN_001f9a10_30a908(&strip_corners[0], &scratch_vector, &side_offset);
    FUN_001f9bf8_30a908(&side_offset, &b, 87.0f);
    FUN_001f9a10_30a908(&strip_corners[2], &scratch_vector, &side_offset);
    FUN_001f9bf8_30a908(&scratch_vector, &a, ocean_vars->f28);
    FUN_001f9bf8_30a908(&side_offset, &b, -750.0f);
    FUN_001f9a10_30a908(&strip_corners[1], &scratch_vector, &side_offset);
    FUN_001f9bf8_30a908(&side_offset, &b, 750.0f);
    FUN_001f9a10_30a908(&strip_corners[3], &scratch_vector, &side_offset);
    for (i = 0; i < 4; i++) {
        Vec4f *strip_corner = &strip_corners[i];
        FUN_001f9a10_30a908(strip_corner, strip_corner, D_L11_001677C0_30a908);
        strip_corner->z = tide_level_macro;
    }
    FUN_001f9a28_30a908(&first_strip_edge_delta, &strip_corners[1], &strip_corners[0]);
    FUN_001f9a28_30a908(&second_strip_edge_delta, &strip_corners[3], &strip_corners[2]);
    t = 10.0f;
    for (i = 0; i < 12; i += 2) {
        float strip_fraction = FUN_001fa6c0_30a908(i) / 10.0f;
        int col = FUN_001fa6e0_30a908(ocean_vars->f0, ocean_vars->f4, FUN_001fa6c0_30a908(i) / t);
        FUN_001f9a68_30a908(&scratch_vector, &first_strip_edge_delta, strip_fraction);
        D_L11_001D9E08[i] = col;
        D_L11_001DA310[i].u = 0.0f;
        D_L11_001DA310[i].v = 0.0f;
        FUN_001f9a10_30a908(&scratch_vector, &scratch_vector, &strip_corners[0]);
        D_L11_001D9B08[i].x = scratch_vector.x;
        D_L11_001D9B08[i].y = scratch_vector.y;
        D_L11_001D9B08[i].z = scratch_vector.z;
        FUN_001f9a68_30a908(&scratch_vector, &second_strip_edge_delta, strip_fraction);
        D_L11_001D9E08[i + 1] = col;
        D_L11_001DA310[i + 1].u = 0.0f;
        D_L11_001DA310[i + 1].v = 0.0f;
        FUN_001f9a10_30a908(&scratch_vector, &scratch_vector, &strip_corners[2]);
        D_L11_001D9B08[i + 1].x = scratch_vector.x;
        D_L11_001D9B08[i + 1].y = scratch_vector.y;
        D_L11_001D9B08[i + 1].z = scratch_vector.z;
    }
    func_L00_001FDE48_30a908(0xC, D_L11_001D9B08, D_L11_001D9E08, D_L11_001DA310, 1);
    far_quad.z78 = FUN_001f44b8_30a908(0x16);
    far_quad.q80 = 0xFF9000000260L;
    far_quad.q88 = 0x8000000044L;
    far_quad.z70 = 0;
    if (ocean_vars->f14 < ((Cam_30a908 *)D_L11_00167680)->z - ocean_vars->f24 &&
        hero.state.current != 0x1D && D_L11_0016D180 == 0) {
        far_quad.uv[0].u = 0.0f;
        far_quad.uv[0].v = 0.0f;
        far_quad.uv[1].u = 0.0f;
        far_quad.uv[1].v = 0.0f;
        far_quad.uv[2].u = 0.0f;
        far_quad.uv[2].v = 0.0f;
        far_quad.uv[3].u = 0.0f;
        far_quad.uv[3].v = 0.0f;
        far_quad.col[0] = ocean_vars->f4;
        far_quad.col[1] = ocean_vars->f8;
        far_quad.col[2] = far_quad.col[0];
        far_quad.col[3] = far_quad.col[1];
        *(u128 *)&far_quad.v[0] = *(u128 *)&strip_corners[1];
        *(u128 *)&far_quad.v[1] = *(u128 *)&strip_corners[1];
        far_quad.v[1].z = ((Cam_30a908 *)D_L11_00167680)->z - ocean_vars->f24;
        *(u128 *)&far_quad.v[2] = *(u128 *)&strip_corners[3];
        *(u128 *)&far_quad.v[3] = *(u128 *)&strip_corners[3];
        far_quad.v[3].z = ((Cam_30a908 *)D_L11_00167680)->z - ocean_vars->f24;
        FUN_001f7d30_30a908(&far_quad, 0, 0);
    }
    FUN_00233980_30a908(0x42, 0x8000000048L);
    FUN_00233980_30a908(8, 0);
    FUN_00233980_30a908(0x14, 0xFF9000000260L);
    FUN_00233980_30a908(0x47, 0x513F1);
    colA = ocean_vars->fC;
    colB = ocean_vars->f10;
    t = 1.0f - (((Cam_30a908 *)D_L11_00167680)->z - tide_level_macro) / ocean_vars->f2C;
    if (t > 1.0f) {
        t = 1.0f;
    }
    if (t >= 0.0f) {
        colA = FUN_001fa6e0_30a908(0, colA, t);
        colB = FUN_001fa6e0_30a908(0, colB, t);
        for (layer = 0; layer < 2; layer++) {
            int id = layer + 0x28;
            FUN_00233980_30a908(6, FUN_001f44b8_30a908(ocean_vars->f1C + id));
            for (y = ocean_vars->f18 * -2.0f; y < ocean_vars->f18 * 2.0f; y += ocean_vars->f18) {
                vertex_count = 0;
                for (x = ocean_vars->f18 * -2.0f; x < ocean_vars->f18 * 2.0f; x += ocean_vars->f18) {
                    D_L11_001DA510[vertex_count].u = x / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].u;
                    D_L11_001DA510[vertex_count].v = y / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].v;
                    D_L11_001D9F08[vertex_count].x = x + ((Cam_30a908 *)D_L11_00167680)->x + 1.0f;
                    D_L11_001D9F08[vertex_count].y = y + ((Cam_30a908 *)D_L11_00167680)->y + 1.0f;
                    D_L11_001D9F08[vertex_count].z = tide_level_macro;
                    i = vertex_count + 1;
                    D_L11_001DA510[i].u = x / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].u;
                    D_L11_001DA510[i].v = (y + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].v;
                    D_L11_001D9F08[i].x = x + ((Cam_30a908 *)D_L11_00167680)->x + 1.0f;
                    D_L11_001D9F08[i].y = y + ocean_vars->f18 + ((Cam_30a908 *)D_L11_00167680)->y + 1.0f;
                    D_L11_001D9F08[i].z = tide_level_macro;
                    i = vertex_count + 2;
                    D_L11_001DA510[i].u = (x + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].u;
                    D_L11_001DA510[i].v = y / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].v;
                    D_L11_001D9F08[i].x = x + ocean_vars->f18 + ((Cam_30a908 *)D_L11_00167680)->x + 1.0f;
                    D_L11_001D9F08[i].y = y + ((Cam_30a908 *)D_L11_00167680)->y + 1.0f;
                    D_L11_001D9F08[i].z = tide_level_macro;
                    i = vertex_count + 3;
                    D_L11_001DA510[i].u = (x + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].u;
                    D_L11_001DA510[i].v = (y + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + D_L11_00161E08_30a908[layer].v;
                    D_L11_001D9F08[i].x = x + ocean_vars->f18 + ((Cam_30a908 *)D_L11_00167680)->x + 1.0f;
                    D_L11_001D9F08[i].y = y + ocean_vars->f18 + ((Cam_30a908 *)D_L11_00167680)->y + 1.0f;
                    D_L11_001D9F08[i].z = tide_level_macro;
                    for (i = vertex_count; i < vertex_count + 4; i++) {
                        scratch_vector.x = D_L11_001D9F08[i].x - ((Cam_30a908 *)D_L11_00167680)->x;
                        scratch_vector.y = D_L11_001D9F08[i].y - ((Cam_30a908 *)D_L11_00167680)->y;
                        scratch_vector.z = D_L11_001D9F08[i].z - ((Cam_30a908 *)D_L11_00167680)->z;
                        scratch_vector.w = 1.0f;
                        t = FUN_001f9b20_30a908(&scratch_vector) / (ocean_vars->f18 * 2.0f);
                        if (t > 1.0f) {
                            t = 1.0f;
                        }
                        D_L11_001DA208[i] = FUN_001fa6e0_30a908(colA, colB, t);
                    }
                    vertex_count += 4;
                    if (vertex_count >= 0x40) {
                        FUN_001f97b0_30a908();
                    }
                }
                func_L00_001FDE48_30a908(vertex_count, D_L11_001D9F08, D_L11_001DA208, D_L11_001DA510, 1);
            }
        }
    }
    ((Fog_30a908 *)D_L11_0016D440)->f228 = saved_fog_228;
    ((Fog_30a908 *)D_L11_0016D440)->f22C = s22C;
    FUN_00233980_30a908(0x47, 0x5360B);
}
/* Ocean update: scrolls the two wave layers with the camera and the time, keeps their offsets in -1..1 and sets the level's tide values. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030C728), where it is exact; names translated to the US level program. */
typedef struct { float u, v; } UV_30b358;
typedef float V4_30b358[4] __attribute__((aligned(16)));

typedef struct {
    char pad00[0x14];
    float level;            /* 0x14 */
    float scale;            /* 0x18 */
    char pad1C[0x4];
    float limit;            /* 0x20 */
} OceanVars_30b358;

typedef struct {
    char pad00[0x20];
    unsigned char state;    /* 0x20 */
    char pad21[0xF];
    unsigned char f30;      /* 0x30 */
    char pad31[0x47];
    OceanVars_30b358 *vars; /* 0x78 */
} OceanMoby_30b358;

extern void subtract_vector_b358(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_b358(void *, void *, float) __asm__("FUN_001f9a68");
extern int float_to_int_b358(float) __asm__("FUN_001fa6d0");
extern float int_to_float_b358(int) __asm__("FUN_001fa6c0");
extern float fast_sin_b358(float) __asm__("FUN_001f9de0");
extern void enqueue_callback_b358(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L11_0030a908_b358(void) __asm__("FUN_L11_0030a908");
extern UV_30b358 wave_offset[] __asm__("D_L11_00161E08") __attribute__((section(".sdata")));
extern float camera_pos_b358[] __asm__("D_L11_001677C0");
extern char last_camera_pos[4] __asm__("D_L11_00162840") __attribute__((sda)); /* a 16-byte vector; declared small so it is reached through $gp */
extern float ocean_level __asm__("D_L11_00161E1C");
extern UV_30b358 wave_drift[] __asm__("D_L11_001D9AF0");
extern float frame_delta_time __asm__("D_0015ED7C");

void ocean_pokitaru_update(OceanMoby_30b358 *moby) __asm__("FUN_L11_0030b358");
void ocean_pokitaru_update(OceanMoby_30b358 *moby)
{
    OceanVars_30b358 *ocean_vars = moby->vars;
    float src[4];
    float out[4];
    int k;

    if (moby->state == 0) {
        moby->state = 1;
        moby->f30 = 0xFF;
        for (k = 0; k < 2; k++) {
            wave_offset[k].u = 0.0f;
            wave_offset[k].v = 0.0f;
        }
        qcopy(last_camera_pos, camera_pos_b358);
        ocean_level = ocean_vars->level;
        return;
    }
    subtract_vector_b358(src, camera_pos_b358, last_camera_pos);
    qcopy(last_camera_pos, camera_pos_b358);
    if (!(ocean_vars->limit < camera_pos_b358[2])) {
        return;
    }
    for (k = 0; k < 2; k++) {
        scale_vector_b358(out, src, (wave_speed[k] + wave_speed[k]) / (ocean_vars->scale * 5.0f));
        if (1.0f < out[0] || out[0] < 1.0f) {
            out[0] = out[0] - int_to_float_b358(float_to_int_b358(out[0]));
        }
        if (1.0f < out[1] || out[1] < 1.0f) {
            out[1] = out[1] - int_to_float_b358(float_to_int_b358(out[1]));
        }
        wave_offset[k].u = wave_offset[k].u + out[0];
        wave_offset[k].v = wave_offset[k].v + out[1];
        if (1.0f < wave_offset[k].u) {
            wave_offset[k].u = wave_offset[k].u - 1.0f;
        }
        if (wave_offset[k].u < -1.0f) {
            wave_offset[k].u = wave_offset[k].u + 1.0f;
        }
        if (1.0f < wave_offset[k].v) {
            wave_offset[k].v = wave_offset[k].v - 1.0f;
        }
        if (wave_offset[k].v < -1.0f) {
            wave_offset[k].v = wave_offset[k].v + 1.0f;
        }
    }
    tide_level = ocean_level + fast_sin_b358(int_to_float_b358(level_frame % 360) * 0.017444445f - 3.14f) * 0.25f;
    for (k = 0; k < 2; k++) {
        float sum = wave_offset[k].u + wave_drift[k].u * frame_delta_time;
        wave_offset[k].u = sum;
        if (1.0f < sum) {
            wave_offset[k].u = sum - 1.0f;
        }
        if (wave_offset[k].u < -1.0f) {
            wave_offset[k].u = wave_offset[k].u + 1.0f;
        }
        sum = wave_offset[k].v + wave_drift[k].v * frame_delta_time;
        wave_offset[k].v = sum;
        if (1.0f < sum) {
            wave_offset[k].v = sum - 1.0f;
        }
        if (wave_offset[k].v < -1.0f) {
            wave_offset[k].v = wave_offset[k].v + 1.0f;
        }
    }
    enqueue_callback_b358((void *)FUN_L11_0030a908_b358, moby);
}

f32 D_L16_001D8A28[1] __attribute__((section(".data"))) = {0};
f32 D_L16_001D8A2C[1] __attribute__((section(".data"))) = {0};
f32 D_L16_001D8A30[1] __attribute__((section(".data"))) = {0};
f32 D_L16_001D8A34[1] __attribute__((section(".data"))) = {0};
f32 D_L16_001D8A38[1] __attribute__((section(".data"))) = {0};
f32 D_L16_001D8A3C[1] __attribute__((section(".data"))) = {0};
s32 D_L16_001D8D28[1] __attribute__((section(".data"))) = {0};
s32 D_L16_001D8D2C[1] __attribute__((section(".data"))) = {0};
s32 D_L16_001D9230[1] __attribute__((section(".data"))) = {0};
s32 D_L16_001D9234[1] __attribute__((section(".data"))) = {0};
s32 D_L16_001D9238[1] __attribute__((section(".data"))) = {0};
s32 D_L16_001D923C[1] __attribute__((section(".data"))) = {0};
