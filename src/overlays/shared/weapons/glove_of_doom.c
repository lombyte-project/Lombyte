/* Shared moby class glove_of_doom: the glove of doom held by the hero, its throw velocity, and the canister it holds and throws. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/world/collision.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/input/pad_state.h"
#include "rnc/input/pad_globals.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "qzero.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"

typedef union {
    u128 q;
    f32  f[4];
    s32  i[4];
} V2c7320;

typedef unsigned int u128_2c82f0 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2c82f0 q;
    float       f[4];
} Vec4_2c82f0;

/* Vars of the carrier moby that picks up and holds another moby. */
typedef struct {
    u8           pad0[0x40];
    OvlVec4      grip;  /* 0x40: where the held moby is carried */
    struct Moby *held;  /* 0x50 */
    s32          timer; /* 0x54 */
} CarrierVars;

typedef float Vec_2c7548[4] __attribute__((aligned(16)));

typedef float Mat_2c7548[4][4] __attribute__((aligned(16)));

typedef float Mat3_2c7548[3][4] __attribute__((aligned(16)));

/* Counters kept at D_00141680 for the carrier's deliveries. */
typedef struct {
    u8  pad0[0xA0];
    u16 deliveries;                /* 0xA0: counts up to 0xFFFF */
    u16 max_delivery_frame_bucket; /* 0xA2: raised to scale_game_frames(D_0015EEA4) / 600 */
    u32 levels;                    /* 0xA4: bit per level index, plus 0x80000000 */
} DeliveryStats;

unsigned char *spawn_glove_of_doom_held_moby(int   spawning_moby,
                                             u128 *spawn_position,
                                             u128 *initial_velocity) __asm__("FUN_L00_002c8218");
void           release_glove_of_doom_held_moby(Vec4_2c82f0 *release_position,
                                               u8          *released_moby,
                                               void        *initial_velocity) __asm__("FUN_L00_002c82f0");
extern void    fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
int            rand(void) __asm__("FUN_001160d8");
void           spawn_particle(float *,
                              unsigned char,
                              unsigned char,
                              unsigned char,
                              float,
                              float,
                              int) __asm__("FUN_L00_00269b70");
s32            collision_hit_material() __asm__("FUN_001f0b58");
extern int     scale_game_frames(int) __asm__("FUN_001f96f8");
void           fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void    sub_2c7320(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32     fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32     fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32     vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32     fn_2c7320(void *, void *, f32, f32, s32) __asm__("FUN_L00_0025abf0");
void          *FUN_L00_002603d0_002c82f0(void *) __asm__("FUN_L00_002603d0");
void           FUN_001f9a10_002c82f0(void *, void *, void *) __asm__("FUN_001f9a10");
void           FUN_001f9a68_002c82f0(void *, void *, float) __asm__("FUN_001f9a68");
int            FUN_001efa68_002c82f0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
void           FUN_001f9bf8_002c82f0(void *, void *, float) __asm__("FUN_001f9bf8");
extern f32     square_root(f32) __asm__("FUN_001f9988");
extern void    fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void    copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void    transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void    transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void    transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern f32     vector_length_xy(void *) __asm__("FUN_001f9b20");
extern int     collision_line_2c7548(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void    FUN_L00_002c7320_2c7548(float *pos,
                                       float *tgt,
                                       float *out,
                                       float  speed,
                                       float  unused,
                                       float  range) __asm__("FUN_L00_002c7320");

extern char           collision_hit_point[] __asm__("D_L00_00173E60") NOT_SDA;
extern CollisionHit   coll_hit __asm__("D_L00_00173E40") NOT_SDA;
extern f32 frame_time MACRO_ADDR;
extern f32            frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern s32            D_0015EEA4;
extern CollisionHit   coll_hit __asm__("D_L00_00173E40");
extern float          frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern DeliveryStats  D_00141680;

void compute_glove_of_doom_launch_velocity(V2c7320 *origin_position,
                                           V2c7320 *target_position,
                                           f32     *out,
                                           f32      speed,
                                           f32      unused,
                                           f32      range) __asm__("FUN_L00_002c7320");

void compute_glove_of_doom_launch_velocity(V2c7320 *origin_position,
                                           V2c7320 *target_position,
                                           f32     *launch_velocity,
                                           f32      speed,
                                           f32      unused,

                                           f32 range) {
    V2c7320      effective_target_position;
    V2c7320      u;
    f32          launch_heading;
    f32          horizontal_speed;
    f32          k;
    f32          v;
    struct Hero *P;
    if (range < fast_vec_dist(origin_position, target_position)) {
        sub_2c7320(&effective_target_position, target_position, origin_position);
        fast_vec_normalize(&effective_target_position, &effective_target_position, range - 0.01f);
        fast_vec_add(&effective_target_position, &effective_target_position, origin_position);
    } else {
        effective_target_position.q = target_position->q;
    }
    launch_heading = fast_arc_tan(effective_target_position.f[0] - origin_position->f[0],
                                  effective_target_position.f[1] - origin_position->f[1]);
    P = &hero;
    if (fast_difference_between_rotations(P->motion.rot.f[2], launch_heading) > 0.87266463f ||
        vector_distance(origin_position, &effective_target_position) < 1.5f) {
        launch_heading = P->motion.rot.f[2];
        u.f[0] = fast_cos(launch_heading) * 1.5f;
        u.f[1] = fast_sin(P->motion.rot.f[2]) * 1.5f;
        u.i[2] = 0;
        fast_vec_add(&u, &u, origin_position);
        effective_target_position.f[0] = u.f[0];
        effective_target_position.f[1] = u.f[1];
    }
    horizontal_speed = frame_time * 8.5f;
    launch_velocity[0] = fast_cos(launch_heading) * horizontal_speed;
    launch_velocity[1] = fast_sin(launch_heading) * horizontal_speed;
    *(s32 *)&launch_velocity[2] = 0;
    launch_velocity[2] =
        fn_2c7320(origin_position, &effective_target_position, horizontal_speed, -speed, 0);
    if (pad_held_words[0] & 5) {
        k = 3.5f / horizontal_speed;
    } else {
        k = 2.5f / horizontal_speed;
    }
    v = speed * k * 0.5f;
    if (v < launch_velocity[2]) {
        launch_velocity[2] = v;
    }
}

/* Moves the moby the carrier holds toward its throw target: ahead of the hero,
   or along the camera aim (and up to a collision hit) while pad bits 0x5 are held. */
void compute_glove_of_doom_throw_velocity(struct Moby *moby,
                                          CarrierVars *vars,
                                          OvlVec4     *grip) __asm__("FUN_L00_002c7548");

void compute_glove_of_doom_throw_velocity(struct Moby *moby,
                                          CarrierVars *throw_carrier_vars,

                                          OvlVec4 *grip) {
    Vec_2c7548   throw_origin;
    Mat3_2c7548  basis;
    Mat_2c7548   matrix;
    Vec_2c7548   direction, target, arc, saved, probe;
    struct Moby *held;
    float       *held_throw_velocity;
    float        horizontal, radius, angle, vertical, slope, distance, root;
    int          hit;
    float        speed;
    float       *pos = throw_origin;

    *(u128 *)pos = grip->q;
    copy_matrix3x4(basis, l00_camera.view_matrix);
    transpose_rotation_matrix(matrix, basis);
    held = throw_carrier_vars->held;
    if (held == 0)
        return;
    held_throw_velocity = (float *)held->pvars;
    *(u128 *)direction = 0;
    direction[0] = 1.0f;
    transform_vector_by_basis(direction, direction, hero.basis);
    {
        struct Hero *hero_state = &hero;
        pos[0] = fast_cos(fast_add_rotations(
                     -0.361968011f, fast_arc_tan(hero_state->facing_x, hero_state->facing_y))) *
                 0.859039009f;
        pos[1] = fast_sin(fast_add_rotations(
                     -0.361968011f, fast_arc_tan(hero_state->facing_x, hero_state->facing_y))) *
                 0.859039009f;
        pos[2] = 0.0f;
        fast_vec_add(pos, pos, &hero.motion.pos);
        pos[2] = pos[2] + 0.390819997f;
        fast_arc_tan(direction[0], direction[1]);
        speed = (controller_state.held & 5) ? 3.5f : 2.5f;
        fast_vec_scale(target, direction, speed);
        fast_vec_add(target, target, pos);
        if ((controller_state.held & 5) && hero_state->item_use_locked == 0) {
            *(u128 *)arc = 0;
            arc[2] = 1.0f;
            transform_vector(arc, arc, matrix);
            hit = 0;
            fast_subtract_rotations(hero_state->moby->rot.z, fast_arc_tan(arc[0], arc[1]));
            angle = fast_arc_tan(arc[0], arc[1]);
            slope = fast_arc_tan(vector_length_xy(arc), arc[2]);
            if (0.593411922f < slope)
                slope = 0.593411922f;
            if (slope < -1.39626336f)
                slope = -1.39626336f;
            build_spherical_offset(arc, (controller_state.held & 5) ? 3.5f : 2.5f, angle, slope);
            qcopy(saved, &l00_camera.pos);
            distance = vector_length_xy(arc);
            {
                float z = arc[2];
                if (distance != 0.0f) {
                    horizontal = z / distance;
                } else {
                    horizontal = z / 0.00999999978f;
                }
            }
            root = square_root(frame_time_sq_macro * 9.0f *
                               ((controller_state.held & 5) ? 3.5f : 2.5f) * 0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 9.0f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                arc[0] = fast_cos(angle) * radius;
                arc[1] = fast_sin(angle) * radius;
                arc[2] = vertical;
                fast_vec_add(probe, &g_debug_camera, arc);
                if (collision_line_2c7548(&g_debug_camera, probe, 2, (int)moby, 0) &&
                    coll_hit.moby != (s32)hero.moby) {
                    qcopy(target, coll_hit.point.f);
                    hit = 1;
                    angle = fast_arc_tan(target[0] - pos[0], target[1] - pos[1]);
                }
                if (hit == 0) {
                    horizontal = 5.0f;
                    target[0] = fast_cos(angle) * radius * horizontal;
                    target[1] = fast_sin(angle) * radius * horizontal;
                    target[2] = vertical * horizontal;
                    fast_vec_add(target, target, &g_debug_camera);
                }
            }
        } else {
            hero.throw_target = 0;
        }
    }
    FUN_L00_002c7320_2c7548(pos,
                            target,
                            held_throw_velocity,
                            frame_time_sq_macro * 9.0f,
                            0.00000999999975f,
                            (controller_state.held & 5) ? 3.5f : 2.5f);
}

/* Carrier: keeps its grip point in front of it, hands the held moby over when the hero asks, and
   picks a new one up when get_ammo allows. */
void glove_of_doom_update(struct Moby *moby) __asm__("FUN_L00_002c7a58");

/* Moby class 229 */
void glove_of_doom_update(struct Moby *moby) {
    enum {
        GLOVE_INITIALIZE = 0,
        GLOVE_WAIT_FOR_ANIMATION = 1,
        GLOVE_IDLE = 2,
        GLOVE_RELEASE_HELD_MOBY = 3,
        GLOVE_POST_RELEASE_WAIT = 4,
        GLOVE_BEGIN_SKIP_HELD_MOBY_SYNC_AND_SPAWN = 5,
        GLOVE_SKIP_HELD_MOBY_SYNC_AND_SPAWN = 6
    };
    OvlVec4      pos;
    OvlVec4      grip_bone_offset;
    OvlVec4      local;
    OvlVec4      grip_bone_transform[4];
    OvlVec4      grip;
    CarrierVars *glove_carrier_vars = (CarrierVars *)moby->pvars;

    grip_bone_offset.q = 0;
    grip_bone_offset.f[0] = 0.01f;
    grip_bone_offset.f[1] = -0.14f;
    grip_bone_offset.f[2] = -0.05f;
    moby_point_position((u8 *)moby, 0, &pos);
    build_moby_bone_transform(moby, 0, grip_bone_transform);
    transform_vector(&local, &grip_bone_offset, grip_bone_transform);
    fast_vec_add(&glove_carrier_vars->grip, &pos, &local);
    if (glove_carrier_vars->held != 0) {
        glove_carrier_vars->held->collision_disable = 1;
    }
    if (glove_carrier_vars->held == 0 || glove_carrier_vars->held->state == 0xFE ||
        glove_carrier_vars->held->state == 0xFD) {
        glove_carrier_vars->held = 0;
    }
    if (hero.state.current == HERO_STATE_LOOK) {
        hero_set_state(0x1E, 1);
    }
    if (fast_dec_timer(&glove_carrier_vars->timer) && !hero.item_use_locked) {
        if ((hero.item_anim_active && hero.item_anim_frames == scale_game_frames(0x11)) ||
            ((((u32)pad_pressed_lui) & hero.items[0].button_mask) && hero.state.current == 0x1E &&
             get_ammo(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            allocate_hero_voice_for_entry(0x1A, 0);
            use_ammo(-1, 1);
            moby->state = GLOVE_RELEASE_HELD_MOBY;
        }
    }
    grip.q = glove_carrier_vars->grip.q;
    compute_glove_of_doom_throw_velocity(moby, glove_carrier_vars, &grip);
    switch (moby->state) {
    case GLOVE_INITIALIZE:
        glove_carrier_vars->held = 0;
        if (moby->anim_flags & 2) {
            moby->state = GLOVE_IDLE;
        } else {
            moby->state = GLOVE_WAIT_FOR_ANIMATION;
        }
    case GLOVE_WAIT_FOR_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = GLOVE_IDLE;
        }
        break;
    case GLOVE_IDLE:
        break;
    case GLOVE_RELEASE_HELD_MOBY:
        if (glove_carrier_vars->held != 0) {
            if (D_00141680.deliveries < 0xFFFF) {
                D_00141680.deliveries++;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > D_00141680.max_delivery_frame_bucket) {
                D_00141680.max_delivery_frame_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            D_00141680.levels = D_00141680.levels | (1 << current_level_index) | 0x80000000;
            release_glove_of_doom_held_moby((void *)&glove_carrier_vars->grip,
                                            (u8 *)glove_carrier_vars->held,
                                            glove_carrier_vars->held->pvars);
            glove_carrier_vars->held = 0;
        } else {
            allocate_voice_for_target_entry(0, 0, moby);
        }
        moby->state = GLOVE_POST_RELEASE_WAIT;
        glove_carrier_vars->timer = scale_game_frames(0x14);
        break;
    case GLOVE_POST_RELEASE_WAIT:
        if (hero.state.current != 0x23 && !hero.item_anim_active) {
            moby->state = GLOVE_IDLE;
        }
        break;
    case GLOVE_BEGIN_SKIP_HELD_MOBY_SYNC_AND_SPAWN:
        moby->state = GLOVE_SKIP_HELD_MOBY_SYNC_AND_SPAWN;
        break;
    case GLOVE_SKIP_HELD_MOBY_SYNC_AND_SPAWN:
        return;
    }
    if (glove_carrier_vars->held != 0) {
        qcopy(&glove_carrier_vars->held->pos, &glove_carrier_vars->grip);
    } else if (get_ammo(-1)) {
        qzero(&grip);
        glove_carrier_vars->held = (struct Moby *)spawn_glove_of_doom_held_moby(
            (int)moby, (void *)&glove_carrier_vars->grip, (void *)&grip);
    }
}

void spawn_moby_collision_particles(unsigned char *moby) __asm__("FUN_L00_002c8078");

void spawn_moby_collision_particles(unsigned char *moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    float         particle_spawn_position[4] __attribute__((aligned(16)));
    int           i;
    int           r, g;
    unsigned char b;
    float         randomized_particle_scalar;
    switch (collision_hit_material()) {
    case COLLISION_MATERIAL_WATER:
    case 1:
    case 3:
    case 8:
    case 11:
    case 12:
    case 13:
        for (i = 0; i < 20; i++) {
            qcopy(particle_spawn_position, &MOBY(moby)->pos);
            particle_spawn_position[0] += random_float_between(-0.3f, 0.3f);
            particle_spawn_position[1] += random_float_between(-0.3f, 0.3f);
            particle_spawn_position[2] += random_float_between(-0.3f, 0.3f);
            r = (rand() + 0x30) & 0x3F;
            g = (rand() + 0x20) & 0x3F;
            b = rand() & 0x2F;
            randomized_particle_scalar = random_float_between(100000.0f, 800000.0f);
            spawn_particle(particle_spawn_position,
                           r,
                           g,
                           b,
                           randomized_particle_scalar,
                           0.0f,
                           scale_game_frames(rand() % 40 + 10));
        }
        MOBY(moby)->state = 4;
        break;
    }
}

unsigned char *spawn_glove_of_doom_held_moby(int   spawning_moby,
                                             u128 *spawn_position,
                                             u128 *initial_velocity) __asm__("FUN_L00_002c8218");

unsigned char *

spawn_glove_of_doom_held_moby(int spawning_moby, u128 *spawn_position, u128 *initial_velocity) {
    unsigned char *spawned_moby = create_moby(0xe6);
    if (spawned_moby) {
        char *moby_vars;
        moby_vars = *(char **)(spawned_moby + 0x78);
        MOBY(spawned_moby)->update_dist = 0xff;
        MOBY(spawned_moby)->draw_dist = 0xff;
        MOBY(spawned_moby)->visible = 1;
        MOBY(spawned_moby)->state = 0;
        qcopy(&MOBY(spawned_moby)->pos, spawn_position);
        qcopy(moby_vars, initial_velocity);
        *(int *)(moby_vars + 0x30) = spawning_moby;
        *(int *)(moby_vars + 0x34) = 0;
        *(short *)(moby_vars + 0x38) = 0;
        MOBY(spawned_moby)->scale = *(float *)(*(char **)(spawned_moby + 0x24) + 0x24) * 0.01f;
        if (hero.hidden || hero.hand_item_hidden)
            MOBY(spawned_moby)->flags |= 0x41;
    }
    return spawned_moby;
}

void release_glove_of_doom_held_moby(Vec4_2c82f0 *release_position,
                                     u8          *released_moby,
                                     void        *initial_velocity) __asm__("FUN_L00_002c82f0");

void release_glove_of_doom_held_moby(Vec4_2c82f0 *release_position,
                                     u8          *released_moby,

                                     void *initial_velocity) {
    enum {
        GLOVE_HELD_MOBY_RELEASED = 2
    };
    char        *q = *(char **)(released_moby + 0x78);
    struct Hero *P;
    char        *released_moby_position, *collision_reference_moby;
    Vec4_2c82f0  collision_start, collision_position_offset;
    qcopy(q, initial_velocity);
    P = &hero;
    if (P->ground_moby && FUN_L00_002603d0_002c82f0(P->ground_moby)) {
        FUN_001f9a10_002c82f0(q, q, &P->motion.displacement);
    }
    released_moby_position = &MOBY(released_moby)->pos;
    qcopy(released_moby_position, release_position);
    {
        struct Hero *P2 = &hero;
        collision_reference_moby = (char *)P2->moby;
    }
    collision_start.q = ((Vec4_2c82f0 *)&MOBY(collision_reference_moby)->pos)->q;
    collision_start.f[2] = release_position->f[2];
    if (FUN_001efa68_002c82f0(&collision_start, release_position, 0, collision_reference_moby, 0)) {
        char *collision_hit_cursor = collision_hit_point;
        float x;
        qcopy(released_moby_position, collision_hit_cursor);
        x = frame_time_sq;
        collision_hit_cursor += 0x20;
        FUN_001f9bf8_002c82f0(
            &collision_position_offset, collision_hit_cursor, x * 9.0f - x * 0.1f);
        FUN_001f9a10_002c82f0(
            released_moby_position, released_moby_position, &collision_position_offset);
        reflect_vector(q, q, collision_hit_cursor);
        FUN_001f9a68_002c82f0(q, q, 0.6f);
    }
    MOBY(released_moby)->state = GLOVE_HELD_MOBY_RELEASED;
}
