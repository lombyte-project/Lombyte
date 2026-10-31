/* Level 09 moby classes 0x107, 0x108, 0x109, 0x10a, 0x10b: asteroid. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/vendor/vendor_volume.h"
#include "rnc/overlay/level_path.h"

extern float D_L09_00166F40[];

#include "qcopy.h"

/* spawns a debris moby with randomized velocity toward a point */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EB808), where it is exact; names translated to the US level program. */
char *spawn_fading_asteroid_debris(struct Moby *owner,
                                   float       *inherited_velocity) __asm__("FUN_L09_002ea528");

/* spawns a debris moby at a random point of a ring above the arena, falling toward the centre */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EBA58), where it is exact; names translated to the US level program. */
char *spawn_falling_asteroid(void) __asm__("FUN_L09_002ea778");

/* Variables of the pebble mobys the l09 rockfall spawner creates (moby->pvars). */
typedef struct {
    f32          vel[3]; /* 0x00: sideways drift */
    f32          speed;  /* 0x0C */
    s32          path;   /* 0x10 */
    s32          unk14;  /* 0x14 */
    s32          count;  /* 0x18: pebbles spawned (on the spawner) */
    u8           pad1C[4];
    Vec4         start;  /* 0x20 */
    Vec4         offset; /* 0x30 */
    f32          spin_x; /* 0x40 */
    f32          spin_y; /* 0x44 */
    s32          unk48;  /* 0x48 */
    u8           pad4C[4];
    struct Moby *owner; /* 0x50 */
} PebbleVars;

typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 pts[1]; /* 0x10: count + 1 points, pts[-1] is this header */
} PebblePath;

extern char D_L09_00166F40_2eaa50[] __asm__("D_L09_00166F40");

/* Spawns one of four pebble classes at the spawner, scattered sideways off the end of its path. */
struct Moby *spawn_path_following_asteroid(struct Moby *owner) __asm__("FUN_L09_002eaa50");

/* ticks a fading drifting moby: decays its velocity, follows it and eases its rotation */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EC010), where it is exact; names translated to the US level program. */
void update_fading_asteroid_debris(unsigned char *moby) __asm__("FUN_L09_002ead30");

#include "sda.h"

/* ticks a falling rock: moves it toward its target, sheds sparks and debris on impact, shakes the camera and removes it */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EC188), where it is exact; names translated to the US level program. */
typedef struct Moby Moby;

typedef struct {
    int v[6];
} V6;

typedef struct {
    float pos[4];
    char  pad10[0x10];
    float target[4];
    char  pad30[0x10];
    float f40;
    float f44;
    char  pad48[8];
    float f50;
    float f54;
    int   f58;
} Data;

typedef struct {
    char pad0[0x2084];
    int  f2084;
} Hero_2EC188;

typedef struct {
    char  pad0[0x140];
    float pos[4];
    char  pad150[0x10];
    float f160;
    int   f164;
    int   f168;
} Cam;

char *FUN_L09_002ea528_c(struct Moby *owner, float *vec) __asm__("FUN_L09_002ea528");

V6 D_L09_00208B00 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L09_00208B18 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

extern char *FUN_L09_002efe48_c(void *, void *, void *, float, float) __asm__("FUN_L09_002efe48");
extern float D_L09_00173FE8 NOT_SDA;
extern float D_0015ED6C     MACRO_ADDR;
extern int                  FUN_001efa68(void *, void *, int, int, int);
extern int                  FUN_L00_00257b90(int, int);
extern int                  trooper_play_sound(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern unsigned char        D_0013F350[];
extern void spawn_particle(void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern f32  frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
void        update_falling_asteroid(Moby *falling_rock) __asm__("FUN_L09_002eaea8");

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002ECC50), where it is exact; names translated to the US level program. */
typedef struct {
    float        pos[3];    /* 0x00 */
    float        speed;     /* 0x0C */
    int          path;      /* 0x10 */
    int          node;      /* 0x14 */
    int          count;     /* 0x18 */
    int          timer;     /* 0x1C */
    float        target[4]; /* 0x20 */
    float        centre[4]; /* 0x30 */
    float        f40;       /* 0x40 */
    float        f44;       /* 0x44 */
    int          init;      /* 0x48 */
    int          mode;      /* 0x4C */
    struct Moby *owner;     /* 0x50 */
} Data_2ECC50;

typedef struct {
    int   n;
    int   pad[3];
    float pts[1][4]; /* 0x10 */
} Path;

extern float D_0015ED6C_c __asm__("D_0015ED6C");
extern void  update_fading_asteroid_debris(unsigned char *m) __asm__("FUN_L09_002ead30");
extern void  scale_vector_xyz_c2(void *, void *, float) __asm__("FUN_001f9a68");
s32          compute_advanced_moby_path_position(
             struct Moby *m, void *path, void *out, s32 i, s32 dir, f32 dist) __asm__("FUN_L09_00294ae0");
void  asteroid_update(struct Moby *m) __asm__("FUN_L09_002eb970");
char *spawn_radially_scattered_moby(void  *unused,
                                    float  lower_radius,
                                    float  upper_radius,
                                    void  *origin,
                                    float *direction) __asm__("FUN_L09_002efe48");

char *spawn_fading_asteroid_debris(struct Moby *debris_source_asteroid, float *inherited_velocity) {
    char *moby = create_moby(random_integer_below(4) + 0x107);
    if (moby != 0) {
        char *spawned_moby_vars = ((char *)MOBY(moby)->pvars);
        char *owner_vars = (char *)debris_source_asteroid->pvars;
        char *dv;
        float reference_position_distance;
        *(float *)spawned_moby_vars = random_float_between(-2.0f, 2.0f);
        *(float *)(spawned_moby_vars + 4) = random_float_between(-2.0f, 2.0f);
        *(float *)(spawned_moby_vars + 8) = random_float_between(-2.0f, 2.0f);
        dv = spawned_moby_vars + 0x20;
        fast_vec_add(dv, spawned_moby_vars, owner_vars);
        fast_vec_normalize(spawned_moby_vars, spawned_moby_vars, frame_time * 50.0f);
        fast_vec_add(spawned_moby_vars, spawned_moby_vars, inherited_velocity);
        fast_vec_scale(spawned_moby_vars, spawned_moby_vars, frame_scale * -0.7f + 1.0f);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        *(int *)(spawned_moby_vars + 0x4C) = 2;
        moby[0x23] = 0x7F;
        {
            float spawned_scale =
                debris_source_asteroid->scale * random_float_between(0.125f, 0.25f);
            MOBY(moby)->scale = spawned_scale;
            *(float *)(spawned_moby_vars + 0x50) = spawned_scale;
        }
        *(float *)(spawned_moby_vars + 0x40) = random_float_between(-0.05235988f, 0.05235988f);
        *(float *)(spawned_moby_vars + 0x44) = random_float_between(-0.05235988f, 0.05235988f);
        fast_vec_sub(moby + 0x10, dv, D_L09_00166F40);
        reference_position_distance = fast_vec_length(moby + 0x10);
        if (100.0f < reference_position_distance) {
            fast_vec_normalize(moby + 0x10, moby + 0x10, 100.0f);
            fast_vec_add(moby + 0x10, moby + 0x10, D_L09_00166F40);
            MOBY(moby)->scale =
                *(float *)(spawned_moby_vars + 0x50) * (100.0f / reference_position_distance);
        } else {
            qcopy(moby + 0x10, dv);
            MOBY(moby)->scale = *(float *)(spawned_moby_vars + 0x50);
        }
    }
    return moby;
}

char *spawn_falling_asteroid(void) {
    float target_position[4];
    char *moby = create_moby(random_integer_below(4) + 0x107);
    if (moby != 0) {
        char *spawned_moby_position = moby + 0x10;
        char *moby_vars = ((char *)MOBY(moby)->pvars);
        float radius = random_float_between(0.0f, 250.0f);
        float angle_radians = random_float_between(-3.1415927f, 3.1415927f);
        float reference_position_distance;
        target_position[0] = fast_cos(angle_radians) * radius;
        target_position[1] = fast_sin(angle_radians) * radius;
        target_position[0] += 256.0f;
        target_position[1] += 256.0f;
        target_position[2] = 100.0f;
        MOBY(moby)->pos.x = 1000.0f;
        MOBY(moby)->pos.y = 500.0f;
        MOBY(moby)->pos.z = random_float_between(200.0f, 250.0f);
        fast_vec_add(spawned_moby_position, spawned_moby_position, target_position);
        qcopy(moby_vars, spawned_moby_position);
        MOBY(moby)->scale *= random_float_between(0.5f, 1.0f);
        *(float *)(moby_vars + 0x40) = random_float_between(-0.015707964f, 0.015707964f);
        *(float *)(moby_vars + 0x44) = random_float_between(-0.015707964f, 0.015707964f);
        target_position[2] = probe_ground_height(target_position, 0, 0.5f);
        if (target_position[2] < 25.0f)
            target_position[2] = 25.0f;
        qcopy(moby_vars + 0x20, target_position);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        *(int *)(moby_vars + 0x4C) = 1;
        *(float *)(moby_vars + 0x54) = random_float_between(110.0f, 130.0f) * frame_time;
        *(float *)(moby_vars + 0x50) = MOBY(moby)->scale;
        *(float *)moby_vars += random_float_between(-100.0f, 100.0f);
        *(float *)(moby_vars + 4) += random_float_between(-100.0f, 100.0f);
        fast_vec_sub(spawned_moby_position, moby_vars, D_L09_00166F40);
        reference_position_distance = fast_vec_length(spawned_moby_position);
        if (100.0f < reference_position_distance) {
            fast_vec_normalize(spawned_moby_position, spawned_moby_position, 100.0f);
            fast_vec_add(spawned_moby_position, spawned_moby_position, D_L09_00166F40);
            MOBY(moby)->scale =
                *(float *)(moby_vars + 0x50) * (100.0f / reference_position_distance);
        } else {
            qcopy(spawned_moby_position, moby_vars);
            MOBY(moby)->scale = *(float *)(moby_vars + 0x50);
        }
        *(int *)(moby_vars + 0x58) = -1;
        update_entity(moby);
    }
    return moby;
}

struct Moby *spawn_path_following_asteroid(struct Moby *owner) {
    struct Moby *spawned_moby;
    PebbleVars  *ovars;
    PebbleVars  *spawned_asteroid_vars;
    PebblePath  *path;
    f32          lateral_scatter_distance;
    Vec4         path_displacement;
    Vec4         path_scatter_vector;

    spawned_moby = create_moby(random_integer_below(4) + 0x107);
    if (spawned_moby != 0) {
        ovars = (PebbleVars *)owner->pvars;
        spawned_asteroid_vars = (PebbleVars *)spawned_moby->pvars;
        qcopy(&spawned_moby->pos, &owner->pos);
        ovars->count++;
        spawned_asteroid_vars->owner = owner;
        spawned_asteroid_vars->path = ovars->path;
        spawned_asteroid_vars->unk48 = 1;
        spawned_asteroid_vars->unk14 = 0;
        spawned_moby->command = 1;
        spawned_moby->visible = 1;
        spawned_moby->update_dist = 0xFF;
        spawned_moby->draw_dist = 0xFF;
        set_moby_color(spawned_moby, 0x80, 0x80, 0x80);
        spawned_moby->alpha = 0;
        spawned_moby->scale *= random_float_between(0.4f, 0.85f);
        spawned_asteroid_vars->spin_x = random_float_between(-0.0052359877f, 0.0052359877f);
        spawned_asteroid_vars->spin_y = random_float_between(-0.0052359877f, 0.0052359877f);
        if (spawned_asteroid_vars->path >= 0) {
            path = ((PebblePath *)l09_paths[spawned_asteroid_vars->path]);
            lateral_scatter_distance = random_float_between(-30.0f, 30.0f);
            fast_vec_sub(
                &path_displacement, &path->pts[path->count - 1], &path->pts[path->count - 2]);
            fast_vec_sub(&path_scatter_vector, &path->pts[1], &path->pts[0]);
            fast_vec_cross(&path_scatter_vector, &path_scatter_vector, &path_displacement);
            path_scatter_vector.f[2] = 0.0f;
            fast_vec_normalize(
                spawned_asteroid_vars, &path_scatter_vector, lateral_scatter_distance);
            fast_vec_sub(&path_displacement, &path->pts[path->count - 1], &path->pts[0]);
            fast_vec_cross(&path_scatter_vector, &path_scatter_vector, &path_displacement);
            fast_vec_normalize(
                &path_scatter_vector, &path_scatter_vector, random_float_between(0.0f, 5.0f));
            fast_vec_add(spawned_asteroid_vars, spawned_asteroid_vars, &path_scatter_vector);
            qcopy(&spawned_asteroid_vars->start, &path->pts[0]);
            qcopy(&spawned_asteroid_vars->offset, &ovars->offset);
            fast_vec_add(&spawned_moby->pos, &path->pts[0], spawned_asteroid_vars);
            fast_vec_add(&spawned_moby->pos, &spawned_moby->pos, D_L09_00166F40_2eaa50);
            fast_vec_sub(&spawned_moby->pos, &spawned_moby->pos, &spawned_asteroid_vars->offset);
            spawned_asteroid_vars->speed = lateral_scatter_distance / 20.0f + 10.0f;
            spawned_moby->scale *= lateral_scatter_distance / 100.0f + 1.0f;
            spawned_asteroid_vars->speed *= frame_time;
        }
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

void update_fading_asteroid_debris(unsigned char *moby) {
    if (moby[0x23] < 3) {
        mark_moby_for_removal(moby);
    } else {
        char *moby_vars;
        float reference_position_distance, updated_rotation_x;
        moby[0x23] -= 2;
        moby_vars = ((char *)MOBY(moby)->pvars);
        fast_vec_scale(moby_vars, moby_vars, frame_scale * -0.0199999809f + 1.0f);
        *(float *)(moby_vars + 8) -= frame_time_sq * 20.0f;
        fast_vec_add(moby_vars + 0x20, moby_vars + 0x20, moby_vars);
        fast_vec_sub(moby + 0x10, moby_vars + 0x20, D_L09_00166F40);
        reference_position_distance = fast_vec_length(moby + 0x10);
        if (reference_position_distance > 100.0f) {
            fast_vec_normalize(moby + 0x10, moby + 0x10, 100.0f);
            fast_vec_add(moby + 0x10, moby + 0x10, D_L09_00166F40);
            MOBY(moby)->scale =
                *(float *)(moby_vars + 0x50) * (100.0f / reference_position_distance);
        } else {
            qcopy(moby + 0x10, moby_vars + 0x20);
            MOBY(moby)->scale = *(float *)(moby_vars + 0x50);
        }
        updated_rotation_x = fast_add_rotations(MOBY(moby)->rot.x, *(float *)(moby_vars + 0x40));
        MOBY(moby)->rot.x = updated_rotation_x;
        MOBY(moby)->rot.y = fast_add_rotations(updated_rotation_x, *(float *)(moby_vars + 0x44));
    }
}

void update_falling_asteroid(Moby *falling_rock) {
    float movement_step[4];
    float collision_probe_offset[4];
    float position_or_movement_step[4];
    float offset_position[4];
    Data *moby_vars = (*(Data **)&falling_rock->pvars);
    float reference_position_distance;
    float updated_rotation_x;

    fast_vec_sub(movement_step, moby_vars->target, moby_vars->pos);
    fast_vec_normalize(collision_probe_offset, movement_step, 25.0f);
    fast_vec_normalize(movement_step, movement_step, moby_vars->f54);
    fast_vec_add(moby_vars->pos, moby_vars->pos, movement_step);
    fast_vec_sub(((f32 *)&falling_rock->pos), moby_vars->pos, D_L09_00166F40);
    reference_position_distance = fast_vec_length(((f32 *)&falling_rock->pos));
    if (reference_position_distance > 100.0f && moby_vars->pos[2] > 27.0f) {
        fast_vec_normalize(((f32 *)&falling_rock->pos), ((f32 *)&falling_rock->pos), 100.0f);
        fast_vec_add(((f32 *)&falling_rock->pos), ((f32 *)&falling_rock->pos), D_L09_00166F40);
        falling_rock->scale = moby_vars->f50 * (100.0f / reference_position_distance);
    } else {
        qcopy(((f32 *)&falling_rock->pos), moby_vars->pos);
        falling_rock->scale = moby_vars->f50;
    }
    updated_rotation_x = fast_add_rotations(falling_rock->rot.x, moby_vars->f40);
    falling_rock->rot.x = updated_rotation_x;
    falling_rock->rot.y = fast_add_rotations(updated_rotation_x, moby_vars->f44);
    if (falling_rock->state == 0) {
        if (moby_vars->pos[2] < 100.0f && moby_vars->pos[0] < 500.0f &&
            moby_vars->pos[1] < 500.0f) {
            position_or_movement_step[0] = random_float_between(-5.0f, 5.0f);
            position_or_movement_step[1] = random_float_between(-5.0f, 5.0f);
            position_or_movement_step[2] = random_float_between(-5.0f, 5.0f);
            fast_vec_add(position_or_movement_step, position_or_movement_step, moby_vars->pos);
            fast_vec_add(offset_position, position_or_movement_step, collision_probe_offset);
            if (FUN_001efa68(position_or_movement_step, offset_position, 4, (int)falling_rock, 0) !=
                    0 &&
                D_L09_00173FE8 > 26.5f) {
                float scaled_movement_step[4];
                V6    A;
                V6    B;
                float extra;
                int   i;
                int   h;
                extra = 0.0f;
                if (reference_position_distance < 7.0f)
                    extra = 7.0f - reference_position_distance;
                scale_vector_xyz_c(scaled_movement_step, movement_step, 0.1f);
                for (i = 7; i >= 0; i--) {
                    float s;
                    float sc;
                    int  *pa;
                    int  *pb;
                    int   a;
                    int   b;
                    s = random_float_between(8.0f, 10.0f);
                    A = D_L09_00208B00;
                    B = D_L09_00208B18;
                    sc = s * D_0015ED6C - extra * D_0015ED6C;
                    pa = &A.v[random_integer_below(6)];
                    pb = &B.v[random_integer_below(6)];
                    {
                        int lo = scale_game_frames(15), hi = scale_game_frames(20);
                        a = FUN_L00_00257b90(lo, hi);
                    }
                    {
                        int lo = scale_game_frames(25), hi = scale_game_frames(30);
                        b = FUN_L00_00257b90(lo, hi);
                    }
                    spawn_particle(
                        moby_vars->pos, scaled_movement_step, *pa, *pb, a, b, 0, 0, 400000.0f, sc);
                }
                for (i = 9; i >= 0; i--) {
                    FUN_L09_002ea528_c((char *)falling_rock, movement_step);
                }
                moby_vars->f58 = h = trooper_play_sound(6, 0, (int)falling_rock, 0xC1);
                if (h != 0) {
                    vendor_volume(h, 0x800);
                }
                if (((Hero_2EC188 *)(D_0013F350))->f2084 != 0x32) {
                    float dist = fast_vec_dist(moby_vars->pos, D_L09_00166F40);
                    if (dist < 15.0f) {
                        Cam *c = (Cam *)((char *)D_L09_00166F40 - 0x140);
                        c->f160 = (64.0f - dist) * 0.01f;
                        c->f168 = scale_game_frames(10);
                    }
                }
                falling_rock->state++;
                return;
            }
        }
        if (moby_vars->target[2] == 25.0f) {
            if (moby_vars->pos[2] < 27.0f) {
                int j;
                qcopy(position_or_movement_step, moby_vars->pos);
                position_or_movement_step[2] = 25.0f;
                for (j = 4; j >= 0; j--) {
                    FUN_L09_002efe48_c(falling_rock,
                                       position_or_movement_step,
                                       movement_step,
                                       moby_vars->f50 * 4.0f,
                                       moby_vars->f50 * 12.0f);
                }
                moby_vars->f54 = moby_vars->f54 * (frame_scale_macro * -0.100000024f + 1.0f);
                if (moby_vars->f58 <= 0) {
                    moby_vars->f58 = trooper_play_sound(0, 0, (int)falling_rock, 0x107);
                }
            }
        }
        if (moby_vars->pos[2] < moby_vars->target[2]) {
            if (moby_vars->target[2] > 26.5f) {
                V6    A;
                V6    B;
                float extra;
                int   i;
                int   h;
                extra = 0.0f;
                if (reference_position_distance < 7.0f)
                    extra = 7.0f - reference_position_distance;
                scale_vector_xyz_c(position_or_movement_step, movement_step, 0.5f);
                for (i = 0; i < 8; i++) {
                    float s;
                    float sc;
                    int  *pa;
                    int  *pb;
                    int   a;
                    int   b;
                    s = random_float_between(8.0f, 10.0f);
                    A = D_L09_00208B00;
                    B = D_L09_00208B18;
                    sc = s * D_0015ED6C - extra * D_0015ED6C;
                    offset_position[0] = random_float_between(-3.0f, 3.0f);
                    offset_position[1] = random_float_between(-3.0f, 3.0f);
                    offset_position[2] = random_float_between(-3.0f, 3.0f);
                    fast_vec_add(offset_position, offset_position, moby_vars->pos);
                    pa = &A.v[random_integer_below(6)];
                    pb = &B.v[random_integer_below(6)];
                    {
                        int lo = scale_game_frames(15), hi = scale_game_frames(20);
                        a = FUN_L00_00257b90(lo, hi);
                    }
                    {
                        int lo = scale_game_frames(25), hi = scale_game_frames(30);
                        b = FUN_L00_00257b90(lo, hi);
                    }
                    spawn_particle(offset_position,
                                   position_or_movement_step,
                                   *pa,
                                   *pb,
                                   a,
                                   b,
                                   0,
                                   0,
                                   1200000.0f,
                                   sc);
                }
                for (i = 9; i >= 0; i--) {
                    FUN_L09_002ea528_c((char *)falling_rock, movement_step);
                }
                moby_vars->f58 = h = trooper_play_sound(6, 0, (int)falling_rock, 0xC1);
                if (h != 0) {
                    vendor_volume(h, 0x800);
                }
                if (((Hero_2EC188 *)(D_0013F350))->f2084 != 0x32) {
                    float dist = fast_vec_dist(moby_vars->pos, D_L09_00166F40);
                    if (dist < 15.0f) {
                        Cam *c = (Cam *)((char *)D_L09_00166F40 - 0x140);
                        c->f160 = (64.0f - dist) * 0.01f;
                        c->f168 = scale_game_frames(10);
                    }
                }
            }
            falling_rock->state++;
        }
    } else {
        falling_rock->state++;
        if (moby_vars->target[2] == 25.0f) {
            int j;
            falling_rock->draw_dist = 0;
            falling_rock->visible = 0;
            qcopy(position_or_movement_step, moby_vars->pos);
            position_or_movement_step[2] = 25.0f;
            for (j = 1; j >= 0; j--) {
                FUN_L09_002efe48_c(falling_rock,
                                   position_or_movement_step,
                                   movement_step,
                                   moby_vars->f50 * 4.0f,
                                   moby_vars->f50 * 12.0f);
            }
            if (falling_rock->state > scale_game_frames(10)) {
                mark_moby_for_removal(falling_rock);
            }
        } else {
            V6    A;
            V6    B;
            float extra;
            int   k;
            extra = 0.0f;
            if (reference_position_distance < 7.0f)
                extra = 7.0f - reference_position_distance;
            scale_vector_xyz_c(position_or_movement_step, movement_step, 0.25f);
            for (k = 0; k < 3; k++) {
                float s;
                float sc;
                int  *pa;
                int  *pb;
                int   a;
                int   b;
                s = random_float_between(8.0f, 10.0f);
                A = D_L09_00208B00;
                B = D_L09_00208B18;
                sc = s * D_0015ED6C - extra * D_0015ED6C;
                offset_position[0] = random_float_between(-1.0f, 1.0f);
                offset_position[1] = random_float_between(-1.0f, 1.0f);
                offset_position[2] = random_float_between(-1.0f, 1.0f);
                fast_vec_add(offset_position, offset_position, moby_vars->pos);
                pa = &A.v[random_integer_below(6)];
                pb = &B.v[random_integer_below(6)];
                {
                    int lo = scale_game_frames(15), hi = scale_game_frames(20);
                    a = FUN_L00_00257b90(lo, hi);
                }
                {
                    int lo = scale_game_frames(25), hi = scale_game_frames(30);
                    b = FUN_L00_00257b90(lo, hi);
                }
                spawn_particle(offset_position,
                               position_or_movement_step,
                               *pa,
                               *pb,
                               a,
                               b,
                               0,
                               0,
                               1200000.0f,
                               sc);
            }
            if (falling_rock->state >= 3) {
                mark_moby_for_removal(falling_rock);
            }
        }
    }
}

/* Moby classes 263-267 */
void asteroid_update(struct Moby *asteroid_moby) {
    Data_2ECC50 *moby_vars = (*(Data_2ECC50 **)&asteroid_moby->pvars);
    Path        *path;
    float        displacement_vector[4];
    float        endpoint_offset[4];
    float        endpoint_segment_length;
    float        dot;
    int          i;

    if (moby_vars->mode == 2) {
        update_fading_asteroid_debris((unsigned char *)asteroid_moby);
        return;
    }
    if (moby_vars->mode == 1) {
        update_falling_asteroid((void *)asteroid_moby);
        return;
    }
    if (asteroid_moby->command == 0 && moby_vars->init == 0) {
        moby_vars->speed = D_0015ED6C_c * 10.0f;
        moby_vars->centre[0] = moby_vars->centre[1] = moby_vars->centre[2] = 0.0f;
        if (moby_vars->path >= 0) {
            Path *ring = ((Path *)l09_paths[moby_vars->path]);
            for (i = 0; i < ring->n; i++) {
                fast_vec_add(moby_vars->centre, moby_vars->centre, ring->pts[i]);
            }
            scale_vector_xyz_c2(moby_vars->centre, moby_vars->centre, 1.0f / (float)ring->n);
            moby_vars->centre[2] = ring->pts[0][2] - 10.0f;
            for (i = 0; i < 23; i++) {
                struct Moby *q = spawn_falling_asteroid();
                if (q != 0) {
                    Data_2ECC50 *e = (*(Data_2ECC50 **)&q->pvars);
                    fast_vec_sub(displacement_vector, e->target, e);
                    fast_vec_normalize(
                        displacement_vector,
                        displacement_vector,
                        random_float_between(0.0f, fast_vec_length(displacement_vector) - 32.0f));
                    fast_vec_add(e, e, displacement_vector);
                }
            }
            for (i = 0; i < 50; i++) {
                struct Moby *q = spawn_path_following_asteroid(asteroid_moby);
                if (q != 0) {
                    Data_2ECC50 *e = (*(Data_2ECC50 **)&q->pvars);
                    int          n = random_integer_below(ring->n - 2);
                    float        segment_fraction = random_float_between(0.0f, 1.0f);
                    e->node = n;
                    fast_vec_sub(displacement_vector, ring->pts[n + 1], ring->pts[n]);
                    scale_vector_xyz_c2(displacement_vector, displacement_vector, segment_fraction);
                    fast_vec_add(e->target, displacement_vector, ring->pts[n]);
                    qcopy(((f32 *)&q->pos), e->target);
                    fast_vec_add(((f32 *)&q->pos), ring->pts[0], e);
                    fast_vec_add(((f32 *)&q->pos), ((f32 *)&q->pos), D_L09_00166F40);
                    fast_vec_sub(((f32 *)&q->pos), ((f32 *)&q->pos), e->centre);
                }
            }
        }
        moby_vars->init = 1;
    }
    if (asteroid_moby->command == 0) {
        if (random_integer_below(scale_game_frames(40)) == 0) {
            spawn_falling_asteroid();
        }
    }
    asteroid_moby->update_dist = 0xFF;
    asteroid_moby->draw_dist = 0xFF;
    if (moby_vars->path < 0) {
        return;
    }
    qcopy_nc(((f32 *)&asteroid_moby->pos), moby_vars->target);
    path = ((Path *)l09_paths[moby_vars->path]);
    if (asteroid_moby->command == 0 && moby_vars->count < 50 && fast_dec_timer(&moby_vars->timer)) {
        moby_vars->timer = scale_game_frames(15) + random_integer_below(scale_game_frames(30));
        spawn_path_following_asteroid(asteroid_moby);
    }
    asteroid_moby->rot.x = fast_add_rotations(asteroid_moby->rot.x, moby_vars->f40);
    asteroid_moby->rot.y = fast_add_rotations(asteroid_moby->rot.y, moby_vars->f44);
    moby_vars->node = compute_advanced_moby_path_position(
        asteroid_moby, path, ((f32 *)&asteroid_moby->pos), moby_vars->node, 1, moby_vars->speed);
    if (moby_vars->node == path->n - 1) {
        if (asteroid_moby->command != 0) {
            (*(Data_2ECC50 **)&(*(Data_2ECC50 **)&asteroid_moby->pvars)->owner->pvars)->count--;
            mark_moby_for_removal(asteroid_moby);
        } else {
            moby_vars->node = 0;
            qcopy(((f32 *)&asteroid_moby->pos), path->pts[0]);
        }
    }
    if (moby_vars->node == 0) {
        fast_vec_sub(displacement_vector, path->pts[1], path->pts[0]);
        endpoint_segment_length = fast_vec_length(displacement_vector);
        fast_vec_normalize(displacement_vector, displacement_vector, 1.0f);
        fast_vec_sub(endpoint_offset, ((f32 *)&asteroid_moby->pos), path->pts[0]);
        dot = fast_vec_dot(endpoint_offset, displacement_vector);
        if (dot < 0.0f) {
            dot = 0.0f;
        }
        asteroid_moby->alpha = (int)(dot * 128.0f / endpoint_segment_length);
        if (asteroid_moby->alpha > 0x80) {
            asteroid_moby->alpha = 0x80;
        }
    } else if (moby_vars->node == path->n - 2) {
        fast_vec_sub(displacement_vector, path->pts[path->n - 2], path->pts[path->n - 1]);
        endpoint_segment_length = fast_vec_length(displacement_vector);
        fast_vec_normalize(displacement_vector, displacement_vector, 1.0f);
        fast_vec_sub(endpoint_offset, ((f32 *)&asteroid_moby->pos), path->pts[path->n - 1]);
        dot = fast_vec_dot(endpoint_offset, displacement_vector);
        if (dot < 0.0f) {
            dot = 0.0f;
        }
        asteroid_moby->alpha = (int)(dot * 128.0f / endpoint_segment_length);
        if (asteroid_moby->alpha > 0x80) {
            asteroid_moby->alpha = 0x80;
        }
    } else {
        asteroid_moby->alpha = 0x80;
    }
    qcopy(moby_vars->target, ((f32 *)&asteroid_moby->pos));
    fast_vec_add(((f32 *)&asteroid_moby->pos), ((f32 *)&asteroid_moby->pos), moby_vars);
    fast_vec_add(((f32 *)&asteroid_moby->pos), ((f32 *)&asteroid_moby->pos), D_L09_00166F40);
    fast_vec_sub(((f32 *)&asteroid_moby->pos), ((f32 *)&asteroid_moby->pos), moby_vars->centre);
}

char *spawn_radially_scattered_moby(

    void *unused, float lower_radius, float upper_radius, void *origin, float *direction) {
    float offset[4];
    char *moby = (char *)create_moby(0x1A1);
    if (moby != 0) {
        float pi = 3.1415927f;
        char *spawned_velocity = (char *)MOBY(moby)->pvars;
        float angle = random_float_between(-3.1415927f, 3.1415927f);
        float radius = random_float_between(lower_radius, upper_radius);
        float heading = fast_arc_tan(direction[0], direction[1]);
        float difference = fast_difference_between_rotations(heading, angle);
        float gap;

        MOBY(moby)->pos.x = fast_cos(angle) * radius;
        {
            float sine = fast_sin(angle);
            float speed = frame_time;
            MOBY(moby)->pos.y = sine * radius;
            MOBY(moby)->pos.z = 0.0f;
            fast_vec_normalize(spawned_velocity, &MOBY(moby)->pos, difference * speed);
        }
        gap = pi - difference;
        fast_vec_normalize(offset, direction, gap * gap * 0.25f * frame_time);
        fast_vec_add(spawned_velocity, spawned_velocity, offset);
        *(int *)(spawned_velocity + 8) = 0;
        fast_vec_add(&MOBY(moby)->pos, &MOBY(moby)->pos, origin);
        fast_vec_add(&MOBY(moby)->pos, &MOBY(moby)->pos, direction);
        fast_vec_scale(spawned_velocity, spawned_velocity, 0.35f);
        MOBY(moby)->anim_speed *= 0.5f;
        MOBY(moby)->rot.z =
            fast_arc_tan(*(float *)spawned_velocity, *(float *)(spawned_velocity + 4));
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        MOBY(moby)->visible = 1;
        pi -= difference;
        MOBY(moby)->scale *= (3.1415927f - difference) + (3.1415927f - difference) + 1.0f;
        update_entity(moby);
    }
    return moby;
}
