/* Shared moby class decoy (also gold_decoy): the inflatable decoy that draws enemies, its throw, landing and destruction, and the function that spawns it. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/world/collision.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/data_labels.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

enum {
    DECOY_INITIALIZE = 0,
    DECOY_HELD = 1,
    DECOY_THROWN_MOVEMENT = 2,
    DECOY_SETTLED_MOVEMENT = 3,
    DECOY_PRE_REMOVAL_DELAY = 4,
    DECOY_WAIT_TO_SHRINK = 5,
    DECOY_SHRINK_FOR_REMOVAL = 6,
    DECOY_RESET_TO_INITIALIZE = 7
};

enum {
    LANDING_REQUIRE_MOBY_PVARS_LINK = 0,
    LANDING_SKIP_LINKED_MOBY_DRAW = 2
};

typedef struct DecoyVars {
    u8    pad0[0x10];
    f32   velocity[4];
    u8    pad20[0x20];
    char *holder;
    s16   timer;
    s16   landing_predicted;
    f32   health;
    f32   target_rot_x;
    f32   target_rot_y;
    s32   frame_count;
    s32   last_hit_source;
    s32   same_source_hit_timer;
    u8    pad60[0xc];
    s16   hit_cooldown_timer;
    s8    hit_cooldown_latched;
    s8    collision_voice_cooldown;
} DecoyVars;

typedef struct {
    u8  pad[0x60];
    f32 f60;
    f32 f64;
    s32 i68;
} O_2c40a0;

typedef struct {
    u8        pad[0x10];
    u8        v10[8];
    f32       f18;
    u8        pad2[0x78 - 0x1C];
    O_2c40a0 *decoy_motion_vars;
} S_2c40a0;

typedef struct {
    u8  pad[0x80];
    u8  hero_position_xy[8];
    f32 hero_position_z;
    u8  pad2[0x1E2 - 0x8C];
    s16 decoy_proximity_response_gate;
} P_2c40a0;

typedef struct {
    char  pad[0x28];
    float height;
    char  pad2[0x14];
    float vector[4];
} S173E40;

/* Vars of a thrown moby (FUN_L00_002c3888 uses the same block). */
typedef struct {
    u8           pad_00[0x10];
    Vec4         vel;     /* 0x10: velocity per step */
    Vec4         landing; /* 0x20: predicted landing point */
    Vec4         normal;  /* 0x30: surface normal there */
    struct Moby *holder;  /* 0x40: moby it was thrown from */
    s16          timer;   /* 0x44 */
    s16          landing_predicted;
} ThrowVars;

extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
int         rand(void) __asm__("FUN_001160d8");
int         FUN_001f96f8_2c3440(int) __asm__("FUN_001f96f8");
void        spawn_particle(float *,
                           unsigned char,
                           unsigned char,
                           unsigned char,
                           float,
                           float,
                           int) __asm__("FUN_L00_00269b70");
void        mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
f32         FUN_001f9b80_2c40a0(void *, void *) __asm__("FUN_001f9b80");
f32         FUN_001f99c0_2c40a0(f32) __asm__("FUN_001f99c0");
void        FUN_001f9a28_2c40a0(void *, void *, void *) __asm__("FUN_001f9a28");
f32         FUN_001f9b20_2c40a0(void *) __asm__("FUN_001f9b20");
f32         FUN_001f9af0_2c40a0(void *) __asm__("FUN_001f9af0");
f32         vendor_spring(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
void        FUN_001f9bf8_2c40a0(void *, void *, f32) __asm__("FUN_001f9bf8");
void        FUN_L00_00258830_2c40a0(void *, void *, f32, f32, f32, int) __asm__("FUN_L00_00258830");
s32         collision_hit_material() __asm__("FUN_001f0b58");
extern int  scale_game_frames(int) __asm__("FUN_001f96f8");
float       fast_vec_length(void *) __asm__("FUN_001f9af0");
void        fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern f32  fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern int  coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern s32  collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void spawn_explosion(float,
                            void *,
                            void *,
                            int,
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
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void           FUN_L00_00260400(void *, int, void *, void *, void *, void *);
extern void           append_unique_moby_index(void *, void *, int) __asm__("FUN_L00_0025f780");
extern f32            square_root(f32) __asm__("FUN_001f9988");
extern void           fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32            scale_ticks(s32) __asm__("FUN_001fa6c0");
extern s32            scale_game_frames_c(s32) __asm__("func_001F96F8");
extern unsigned char *FUN_L00_002603d0(int);
extern void           draw_moby_oscillating_textured_quads(void) __asm__("FUN_L00_002c31c0");
extern float          FUN_00200240_2c4380(int) __asm__("FUN_001fa6c0");
extern int            FUN_L00_0025a420(void *, int, int);
extern int            FUN_L00_00258830(void *, void *, float, float, float, int);
extern void           start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void           update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void           add_random_offset_to_vector_xyz(float, void *) __asm__("FUN_L00_0025f8e0");
extern void  initialize_hit_descriptor(float, void *, void *, int) __asm__("FUN_L00_00259858");
extern int   FUN_001fef20_2c4380(int) __asm__("FUN_001f96f8");
extern int   FUN_001fef68_2c4380(void *) __asm__("FUN_001f9740");
extern int   fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float FUN_001ff3d8_2c4380(void *) __asm__("FUN_001f9af0");
extern float FUN_001ff408_2c4380(void *) __asm__("FUN_001f9b20");
extern float FUN_001ff1e0_2c4380(float) __asm__("FUN_001f9988");
extern void  FUN_001ff2a8_2c4380(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001ff278_2c4380(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  FUN_001ff300_2c4380(float, void *, void *) __asm__("FUN_001f9a68");
extern float FUN_001ff8b0_2c4380(float, float) __asm__("FUN_001f9e90");
extern float FUN_001ff7e8_2c4380(float) __asm__("FUN_001f9dc8");
extern float FUN_001ff800_2c4380(float) __asm__("FUN_001f9de0");
extern float FUN_00200130_2c4380(float, float) __asm__("FUN_001fa5c8");
extern float FUN_002000e8_2c4380(float, float) __asm__("FUN_001fa580");

extern s16            D_L00_001B0830_lui[] __asm__("D_L00_001B0830") NOT_SDA;
extern CollisionHit   coll_hit __asm__("D_L00_00173E40") NOT_SDA;
extern s32            D_L00_0015F5C4;
extern f32 frame_time MACRO_ADDR;
extern f32            frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern CollisionHit   coll_hit __asm__("D_L00_00173E40");
extern Vec4           D_L00_00166DC0;
extern char           D_00173F60_alias[] __asm__("D_L00_00173E60");
/* coll_hit's normal as a whole quadword (CollisionHit names only x/y/z) */
extern Vec4 D_L00_00166DC0 NOT_SDA;
extern float               frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

void destroy_decoy_with_particles(unsigned char *moby) __asm__("FUN_L00_002c3440");

void destroy_decoy_with_particles(unsigned char *moby) {
    float         decoy_particle_position[4] __attribute__((aligned(16)));
    int           i;
    int           r, g;
    unsigned char b;
    float         randomized_scalar;
    for (i = 0; i < 10; i++) {
        qcopy(decoy_particle_position, &MOBY(moby)->pos);
        decoy_particle_position[0] += random_float_between(-0.2f, 0.2f);
        decoy_particle_position[1] += random_float_between(-0.2f, 0.2f);
        decoy_particle_position[2] += random_float_between(-0.2f, 0.2f);
        r = (rand() + 0x30) & 0x3F;
        g = (rand() + 0x20) & 0x3F;
        b = rand() & 0x2F;
        randomized_scalar = random_float_between(100000.0f, 600000.0f);
        spawn_particle(decoy_particle_position,
                       r,
                       g,
                       b,
                       randomized_scalar,
                       0.0f,
                       FUN_001f96f8_2c3440(rand() % 40 + 10));
    }
    mine_spawn_drops(moby, D_L00_001B0830_lui);
    mark_moby_for_removal(moby);
}

u8 *spawn_decoy(s32 value, u128 *position, u128 *other_position) __asm__("FUN_L00_002c35c0");

u8 *spawn_decoy(s32 owner_moby, u128 *position, u128 *other_position) {
    u8 *flags = item_text_variant;
    u8 *spawned_moby;
    u8 *decoy_vars;
    u8 *candidate;
    s32 i;
    s32 best_value;
    s32 current_value;
    f32 speed;

    spawned_moby = create_moby(flags[0x19] ? 0x76c : 0xcb);
    if (spawned_moby != 0) {
        MOBY(spawned_moby)->update_dist = 0xff;
        *(u16 *)(spawned_moby + 0x32) = 0xff;
        MOBY(spawned_moby)->visible = 1;
        decoy_vars = *(u8 **)(spawned_moby + 0x78);
        MOBY(spawned_moby)->state = DECOY_INITIALIZE;
        qcopy(&MOBY(spawned_moby)->pos, position);
        qcopy(decoy_vars + 0x10, other_position);
        speed = frame_scale * -0.5f;
        *(s32 *)(decoy_vars + 0x40) = owner_moby;
        *(u16 *)(decoy_vars + 0x44) = 0;
        *(s32 *)(decoy_vars + 0x54) = 0;
        MOBY(spawned_moby)->scale = *(f32 *)(*(u8 **)(spawned_moby + 0x24) + 0x24) * (speed + 1.0f);
        *(f32 *)(decoy_vars + 0x48) = (scale_ticks(flags[0x19]) + 1.0f) * 4.0f;
        if (MOBY(spawned_moby)->prev_seq != 0)
            blend_moby_animation(spawned_moby, 0, 0, scale_game_frames(0));
        *(s32 *)(spawned_moby + 0x94) = 0;
        MOBY(spawned_moby)->collision_disable = -1;
        MOBY(spawned_moby)->rot.z = random_float_between(-3.14159274f, 3.14159274f);
        *(s32 *)(decoy_vars + 0x58) = 0;
        *(s32 *)(decoy_vars + 0x5c) = 0;
        while (D_L00_001B0830_lui[0] >= 6) {
            candidate = 0;
            best_value = 0;
            for (i = 1; i <= D_L00_001B0830_lui[0]; i++) {
                u8 *listed_moby = (u8 *)(moby_table + (D_L00_001B0830_lui[i] << 8));
                if (listed_moby != 0 &&
                    (MOBY(listed_moby)->oclass == 0xcb || MOBY(listed_moby)->oclass == 0x76c) &&
                    MOBY(listed_moby)->state < DECOY_WAIT_TO_SHRINK) {
                    current_value = *(s32 *)(*(u8 **)(listed_moby + 0x78) + 0x54);
                    if (best_value <= current_value) {
                        candidate = listed_moby;
                        best_value = current_value;
                    }
                }
            }
            if (candidate != 0) {
                mine_spawn_drops(candidate, D_L00_001B0830_lui);
                candidate[0x20] = 6;
            }
        }
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
        *(u16 *)decoy_vars = 0;
        decoy_vars[8] = 0;
        decoy_vars[9] = 0;
        decoy_vars[7] = 0x80;
        *(u16 *)(decoy_vars + 0x0c) = 4;
        *(u16 *)(decoy_vars + 0x0e) = 0xf;
        append_unique_moby_index(spawned_moby, D_L00_001B0830_lui, 0x1f);
        if (hero.hidden != 0 || hero.hand_item_hidden != 0)
            MOBY(spawned_moby)->flags |= 0x41;
    }
    return spawned_moby;
}

/* Tests a movement vector against the player and turns a colliding moby. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C4B90), where it is exact; names translated to the US level program. */
void release_moby_with_collision_check(float *release_position,
                                       char  *moby,
                                       void  *initial_velocity) __asm__("FUN_L00_002c3888");

void release_moby_with_collision_check(float *release_position,
                                       char  *moby,

                                       void *initial_velocity) {
    float        collision_start[4], collision_normal[4], rotated_normal[4];
    char        *state = *(char **)(moby + 0x78);
    struct Hero *camera;
    char        *player;
    qcopy(state + 0x10, initial_velocity);
    {
        struct Hero *view = &hero;
        if (((int)view->ground_moby) && FUN_L00_002603d0(((int)view->ground_moby)))
            fast_vec_add(state + 0x10, state + 0x10, &view->motion.displacement);
    }
    qcopy(&MOBY(moby)->pos, release_position);
    camera = &hero;
    player = ((char *)camera->moby);
    *(OvlQuad *)collision_start = *(OvlQuad *)(player + 0x10);
    collision_start[2] = release_position[2];
    fast_vec_sub(release_position, release_position, collision_start);
    fast_vec_normalize(
        release_position, release_position, fast_vec_length(release_position) + 0.2f);
    fast_vec_add(release_position, release_position, collision_start);
    if (collision_line(collision_start, release_position, 0, ((int)camera->moby), 0)) {
        float smoothing;
        qcopy(&MOBY(moby)->pos, D_00173F60_alias);
        qzero(state + 0x10);
        MOBY(moby)->state = DECOY_PRE_REMOVAL_DELAY;
        *(short *)(state + 0x44) = scale_game_frames_c(30);
        MOBY(moby)->pos.z = ((S173E40 *)D_L00_00173E40)->height;
        qcopy(collision_normal, ((S173E40 *)D_L00_00173E40)->vector);
        {
            float co, si;
            co = fast_cos(MOBY(moby)->rot.z);
            smoothing = 0.1f;
            si = fast_sin(MOBY(moby)->rot.z);
            rotated_normal[0] = collision_normal[0] * co + collision_normal[1] * si;
        }
        {
            float co, si;
            co = fast_cos(MOBY(moby)->rot.z);
            si = fast_sin(MOBY(moby)->rot.z);
            rotated_normal[1] = collision_normal[1] * co - collision_normal[0] * si;
            rotated_normal[2] = collision_normal[2];
        }
        *(float *)(state + 0x4C) = -fast_arc_tan(square_root(rotated_normal[0] * rotated_normal[0] +
                                                             rotated_normal[2] * rotated_normal[2]),
                                                 rotated_normal[1]);
        *(float *)(state + 0x50) = fast_arc_tan(rotated_normal[2], rotated_normal[0]);
        MOBY(moby)->rot.x = fast_add_rotations(
            MOBY(moby)->rot.x,
            fast_subtract_rotations(*(float *)(state + 0x4C), MOBY(moby)->rot.x) *
                (frame_scale * smoothing));
        MOBY(moby)->rot.y = fast_add_rotations(
            MOBY(moby)->rot.y,
            fast_subtract_rotations(*(float *)(state + 0x50), MOBY(moby)->rot.y) *
                (frame_scale * smoothing));
    } else
        MOBY(moby)->state = DECOY_THROWN_MOVEMENT;
}

/*
 * Predicts where a thrown moby lands: steps the velocity (with gravity) from
 * its start until collision_line hits ground (or a moby other than the hero
 * and the holder), for up to `timer` steps. Stores the landing point and
 * normal in vars and pulls the point 5% toward D_L00_00166DC0. mode 0 needs
 * a live hit moby; mode 1 starts from the hero's hand. Returns 1 on a hit.
 */
s32 predict_thrown_moby_landing(ThrowVars   *throw_vars,
                                struct Moby *thrown_moby,
                                s32          mode) __asm__("FUN_L00_002c3af8");

s32 predict_thrown_moby_landing(ThrowVars *throw_vars, struct Moby *thrown_moby, s32 mode) {
    Vec4 predicted_position, predicted_velocity, position_scratch, q, ground_probe_position;
    s32  prediction_countdown;
    s32  found = 0;
    s32  i;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4 != 0)
        return 0;
    qcopy(&position_scratch, &thrown_moby->pos);
    if (thrown_moby->state == DECOY_HELD) {
        predicted_position.f[0] =
            fast_cos(fast_add_rotations(-0.36196801066398620605f,
                                        fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.85903900861740112305f;
        predicted_position.f[1] =
            fast_sin(fast_add_rotations(-0.36196801066398620605f,
                                        fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.85903900861740112305f;
        predicted_position.f[2] = 0.0f;
        fast_vec_add(&predicted_position, &predicted_position, &hero.motion.pos);
        predicted_position.f[2] += 0.39081999659538269043f;
        qcopy(&predicted_velocity, &throw_vars->vel);
        if (hero.state.control_mode == HERO_MODE_GRIND)
            fast_vec_add(&predicted_velocity, &predicted_velocity, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0((int)hero.ground_moby) && mode)
            fast_vec_add(&predicted_velocity, &predicted_velocity, &hero.motion.displacement);
        prediction_countdown = scale_game_frames(300);
    } else {
        u8 *holder_vars = throw_vars->holder->pvars;

        qcopy(&predicted_position, &thrown_moby->pos);
        qcopy(&predicted_velocity, &throw_vars->vel);
        {
            s32 timer = throw_vars->timer;
            s32 flag = *(s32 *)(holder_vars + 0x50);

            prediction_countdown = timer;
            if (flag)
                throw_vars->landing_predicted = 1;
        }
    }
    predicted_velocity.f[2] -= frame_time * 0.5f;
    q.q = ((Vec4 *)&hero.moby->pos)->q;
    q.f[2] = predicted_position.f[2];
    if (collision_line(&q, &predicted_position, 0x10, hero.moby, 0) &&
        (collision_hit_material() || 0.0f < predicted_velocity.f[2])) {
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby != hero.moby &&
                 (struct Moby *)coll_hit.moby != throw_vars->holder) ||
                prediction_countdown < scale_game_frames(300) - scale_game_frames(10)) {
                ground_probe_position.q = 0;
                ground_probe_position.f[2] = 0.5f;
                qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
                found = 1;
                fast_vec_add(&ground_probe_position, &ground_probe_position, &throw_vars->landing);
                probe_ground_height(&ground_probe_position, 0, 0.5f);
            }
        } else if (coll_hit.unk1C > 0) {
            qcopy(&throw_vars->landing, &coll_hit.point);
            found = 1;
        }
    }
    while (!fast_dec_timer(&prediction_countdown) && !found) {
        qcopy(&position_scratch, &predicted_position);
        for (i = 9; i >= 0; i--) {
            fast_vec_add(&predicted_position, &predicted_position, &predicted_velocity);
            predicted_velocity.f[2] -= frame_time_sq_macro * 9.0f;
        }
        if (!collision_line(&position_scratch, &predicted_position, 0x10, thrown_moby, 0))
            continue;
        if (!collision_hit_material() && 0.0f < predicted_velocity.f[2])
            continue;
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby == hero.moby ||
                 (struct Moby *)coll_hit.moby == throw_vars->holder) &&
                !(prediction_countdown < scale_game_frames(300) - scale_game_frames(10)))
                continue;
            ground_probe_position.q = 0;
            ground_probe_position.f[2] = 0.5f;
            qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
            fast_vec_add(&ground_probe_position, &ground_probe_position, &throw_vars->landing);
            probe_ground_height(&ground_probe_position, 0, 0.5f);
            break;
        } else if (coll_hit.unk1C > 0) {
            qcopy(&throw_vars->landing, &coll_hit.point);
            break;
        }
    }
    if (prediction_countdown == 0)
        return 0;
    if (mode == LANDING_REQUIRE_MOBY_PVARS_LINK) {
        if (coll_hit.moby == 0)
            return 0;
        if (FUN_L00_002603d0(coll_hit.moby) == 0)
            goto ret0;
    }
    if (mode != LANDING_SKIP_LINKED_MOBY_DRAW || coll_hit.moby == 0 ||
        !FUN_L00_002603d0(coll_hit.moby))
        add_draw_callback(draw_moby_oscillating_textured_quads, thrown_moby);
    throw_vars->normal.q = coll_hit_normal.q;
    fast_vec_sub(&position_scratch, &throw_vars->landing, &D_L00_00166DC0);
    fast_vec_scale(&position_scratch, &position_scratch, 0.94999998807907104492f);
    fast_vec_add(&throw_vars->landing, &position_scratch, &D_L00_00166DC0);
    return 1;
ret0:
    /* a second return 0 block, as in retail */
    return 0;
}

void update_decoy_proximity_motion(S_2c40a0 *self) __asm__("FUN_L00_002c40a0");

void update_decoy_proximity_motion(S_2c40a0 *decoy_moby) {
    O_2c40a0 *moby_vars = decoy_moby->decoy_motion_vars;
    P_2c40a0 *P = (P_2c40a0 *)&hero;
    s16       proximity_response_gate = P->decoy_proximity_response_gate;
    f32       horizontal_displacement[4];
    f32       horizontal_separation_error, horizontal_separation_distance, ground_height;
    if (moby_vars->i68 == 0) {
        if (proximity_response_gate == 0)
            return;
        if (!(FUN_001f9b80_2c40a0(P->hero_position_xy, decoy_moby->v10) < 1.0f))
            return;
        if (!(FUN_001f99c0_2c40a0(P->hero_position_z - decoy_moby->f18) < 1.3f))
            return;
        moby_vars->i68 = 1;
    }
    FUN_001f9a28_2c40a0(horizontal_displacement, decoy_moby->v10, P->hero_position_xy);
    horizontal_displacement[2] = 0.0f;
    horizontal_separation_error = FUN_001f9b20_2c40a0(horizontal_displacement) - 1.4f;
    if (horizontal_separation_error > 0.0f)
        horizontal_separation_error = 0.0f;
    if (proximity_response_gate == 0)
        horizontal_separation_error = 0.0f;
    vendor_spring(&horizontal_separation_error,
                  0.0f,
                  &moby_vars->f60,
                  frame_scale_sq * 0.02f,
                  frame_scale_sq * 0.3f,
                  frame_time * 4.0f);
    if (horizontal_separation_error == 0.0f && moby_vars->f60 == 0.0f) {
        if (FUN_001f99c0_2c40a0(decoy_moby->f18 - probe_ground_height(decoy_moby->v10, 0, 0.5f)) <
            0.02f)
            moby_vars->i68 = 0;
    }
    horizontal_separation_distance = FUN_001f9af0_2c40a0(horizontal_displacement);
    if (horizontal_separation_distance < moby_vars->f60)
        moby_vars->f60 = horizontal_separation_distance;
    if (moby_vars->f60 < horizontal_separation_distance)
        FUN_001f9bf8_2c40a0(horizontal_displacement, horizontal_displacement, moby_vars->f60);
    FUN_L00_00258830_2c40a0(decoy_moby, horizontal_displacement, 0.5f, 0.4f, 0.0f, 0);
    ground_height = probe_ground_height(decoy_moby->v10, 0, 0.5f);
    if (decoy_moby->f18 < ground_height) {
        decoy_moby->f18 = ground_height;
        moby_vars->f64 = 0.0f;
        return;
    }
    moby_vars->f64 -= frame_time_sq * 24.0f;
    decoy_moby->f18 += moby_vars->f64;
    if (decoy_moby->f18 < ground_height)
        decoy_moby->f18 = ground_height;
}

/* Calls FUN_L00_002c3440 on m for collision_hit_material results 0, 1, 3, 8, 11, 12 and 13. */
void destroy_decoy_on_selected_materials(void *moby) __asm__("FUN_L00_002c4328");

void destroy_decoy_on_selected_materials(void *moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    switch (collision_hit_material()) {
    case COLLISION_MATERIAL_WATER:
    case 1:
    case 3:
    case 8:
    case 11:
    case 12:
    case 13:
        destroy_decoy_with_particles(moby);
        break;
    }
}

void decoy_update(char *moby) __asm__("FUN_L00_002c4380");

/* Moby classes 203, 1900 */
void decoy_update(char *moby) {
    int        active = ((unsigned char *)&D_0013E533)[6];
    DecoyVars *decoy_instance_vars = (DecoyVars *)(*(char **)(moby + 0x78));
    char      *target;
    float      position_effect_and_normal_scratch[4] __attribute__((aligned(16)));
    struct {
        float w[4];
        float x[4];
    } scratch __attribute__((aligned(16)));
    float  rotated_normal[4] __attribute__((aligned(16)));
    float  len, angle, cos_angle, sin_angle, rate;
    int    i, hit, state;
    short *ids;
    float  effect;
    if (MOBY(moby)->oclass == 0xcb)
        active = 0;
    effect = FUN_00200240_2c4380(active);
    if (MOBY(moby)->pos.x < 2.0f || 1021.0f < MOBY(moby)->pos.x || MOBY(moby)->pos.y < 2.0f ||
        1021.0f < MOBY(moby)->pos.y || MOBY(moby)->pos.z < 2.0f || 1021.0f < MOBY(moby)->pos.z) {
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    if ((hero.items[0].moby == 0 || hero.items[0].moby->oclass != 0x232) &&
        (u8)MOBY(moby)->state == DECOY_HELD) {
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    if (hero.vehicle_mode != HERO_VEHICLE_MODE_ON_FOOT && (u8)MOBY(moby)->state == DECOY_HELD) {
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    ++decoy_instance_vars->frame_count;
    if (hero.hidden != 0 && (u8)MOBY(moby)->state < DECOY_THROWN_MOVEMENT)
        MOBY(moby)->flags |= 0x41;
    else
        MOBY(moby)->flags &= 0xffbe;
    if (*(short *)(decoy_instance_vars->holder + 0xa6) == 0x232 &&
        decoy_instance_vars->landing_predicted == 0 &&
        FUN_001ff3d8_2c4380(&decoy_instance_vars->velocity) > 0.0f) {
        if (hero.ground_moby == 0 || !FUN_L00_002603d0((int)hero.ground_moby))
            predict_thrown_moby_landing((ThrowVars *)decoy_instance_vars, (struct Moby *)moby, 1);
        else if (!predict_thrown_moby_landing(
                     (ThrowVars *)decoy_instance_vars, (struct Moby *)moby, 0))
            predict_thrown_moby_landing((ThrowVars *)decoy_instance_vars, (struct Moby *)moby, 2);
    }
    switch ((u8)MOBY(moby)->state) {
    case DECOY_INITIALIZE:
        MOBY(moby)->state = DECOY_HELD;
        decoy_instance_vars->timer = 0;
        MOBY(moby)->collision_disable = -1;
        decoy_instance_vars->hit_cooldown_timer = 0;
        decoy_instance_vars->hit_cooldown_latched = 0;
        break;
    case DECOY_HELD: {
        char *target;
        decoy_instance_vars->timer = 0;
        target = decoy_instance_vars->holder;
        if (target && (((u8)target[0x20] & 0xf0) != 0xf0) && hero.items[0].moby &&
            hero.items[0].moby->oclass == 0x232)
            return;
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    case DECOY_THROWN_MOVEMENT:
        if (MOBY(moby)->scale < *(float *)(*(char **)(moby + 0x24) + 0x24))
            MOBY(moby)->scale += frame_scale * 0.02f;
        qcopy(position_effect_and_normal_scratch, &MOBY(moby)->pos);
        decoy_instance_vars->velocity[2] -= frame_time_sq * 9.0f;
        for (i = 1; i <= D_L00_001B0830_lui[0]; ++i) {
            char *target;
            ids = D_L00_001B0830_lui + i;
            target = (char *)moby_table + *ids * 0x100;
            if (target != moby && (u8)target[0x20] > 1 && target && (u8)target[0x20] != 0xfe &&
                (u8)target[0x20] != 0xfd) {
                float len;
                FUN_001ff2a8_2c4380(scratch.w, &MOBY(moby)->pos, target + 0x10);
                scratch.w[2] = 0.0f;
                len = FUN_001ff3d8_2c4380(scratch.w);
                if (len < 1.8f) {
                    FUN_001ff300_2c4380((1.8f - len) * 0.015f, scratch.w, scratch.w);
                    FUN_001ff278_2c4380(
                        &decoy_instance_vars->velocity, &decoy_instance_vars->velocity, scratch.w);
                }
            }
        }
        if (hero.state.current == 0x1e)
            *(int *)((char *)hero.moby + 0x94) = 0;
        hit = FUN_L00_00258830(moby, &decoy_instance_vars->velocity, 0.2f, 0.2f, 0.0f, 0);
        if (hit & 1) {
            qcopy(scratch.w, &MOBY(moby)->pos);
            scratch.w[2] += 0.2f;
            if (coll_sphere(scratch.w, 0.23f, 0, moby)) {
                if (decoy_instance_vars->velocity[2] < 0.0f) {
                    float len;
                    FUN_001ff2a8_2c4380(scratch.x, &MOBY(moby)->pos, &hero.motion.pos);
                    scratch.x[2] = 0.0f;
                    len = FUN_001ff3d8_2c4380(scratch.x);
                    if (len < 1.8f) {
                        FUN_001ff300_2c4380((1.8f - len) * 0.015f, scratch.x, scratch.x);
                        FUN_001ff278_2c4380(&decoy_instance_vars->velocity,
                                            &decoy_instance_vars->velocity,
                                            scratch.x);
                    }
                }
                reflect_vector(&decoy_instance_vars->velocity,
                               &decoy_instance_vars->velocity,
                               &coll_hit.normal_x);
                if (coll_hit.moby != 0 && coll_hit.unk1C <= 0)
                    FUN_001ff300_2c4380(
                        1.05f, &decoy_instance_vars->velocity, &decoy_instance_vars->velocity);
                else
                    FUN_001ff300_2c4380(
                        0.2f, &decoy_instance_vars->velocity, &decoy_instance_vars->velocity);
                rate = 0.01f;
                len = FUN_001ff3d8_2c4380(&decoy_instance_vars->velocity);
                if (len < frame_scale * rate ||
                    (hero.ground_moby && FUN_L00_002603d0((int)hero.ground_moby) &&
                     FUN_001ff3d8_2c4380(&decoy_instance_vars->velocity) <
                         frame_scale * rate + FUN_001ff3d8_2c4380(&hero.motion.displacement))) {
                    angle = FUN_001ff8b0_2c4380(coll_hit.normal_z,
                                                FUN_001ff408_2c4380(&coll_hit.normal_x));
                    if (angle < 0.61086524f) {
                        ++decoy_instance_vars->timer;
                        if (coll_hit.moby == 0 || coll_hit.unk1C > 0 ||
                            decoy_instance_vars->timer > FUN_001fef20_2c4380(240)) {
                            if (((u8 *)moby)[0x53] != 1)
                                blend_moby_animation(moby, 1, 0, FUN_001fef20_2c4380(10));
                            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
                            MOBY(moby)->collision_disable = 0;
                            qzero(&decoy_instance_vars->velocity);
                            MOBY(moby)->state = DECOY_SETTLED_MOVEMENT;
                            decoy_instance_vars->timer = FUN_001fef20_2c4380(600);
                            MOBY(moby)->pos.z = coll_hit.point.f[2];
                            qcopy(scratch.x, &coll_hit.normal_x);
                            cos_angle = FUN_001ff7e8_2c4380(MOBY(moby)->rot.z);
                            sin_angle = FUN_001ff800_2c4380(MOBY(moby)->rot.z);
                            rotated_normal[0] = scratch.x[0] * cos_angle + scratch.x[1] * sin_angle;
                            cos_angle = FUN_001ff7e8_2c4380(MOBY(moby)->rot.z);
                            sin_angle = FUN_001ff800_2c4380(MOBY(moby)->rot.z);
                            rotated_normal[1] = scratch.x[1] * cos_angle - scratch.x[0] * sin_angle;
                            rotated_normal[2] = scratch.x[2];
                            decoy_instance_vars->target_rot_x = -FUN_001ff8b0_2c4380(
                                FUN_001ff1e0_2c4380(rotated_normal[0] * rotated_normal[0] +
                                                    rotated_normal[2] * rotated_normal[2]),
                                rotated_normal[1]);
                            decoy_instance_vars->target_rot_y =
                                FUN_001ff8b0_2c4380(rotated_normal[2], rotated_normal[0]);
                            rate = 0.1f;
                            MOBY(moby)->rot.x = FUN_002000e8_2c4380(
                                MOBY(moby)->rot.x,
                                FUN_00200130_2c4380(decoy_instance_vars->target_rot_x,
                                                    MOBY(moby)->rot.x) *
                                    (frame_scale * rate));
                            MOBY(moby)->rot.y = FUN_002000e8_2c4380(
                                MOBY(moby)->rot.y,
                                FUN_00200130_2c4380(decoy_instance_vars->target_rot_y,
                                                    MOBY(moby)->rot.y) *
                                    (frame_scale * rate));
                        }
                    }
                }
                destroy_decoy_on_selected_materials(moby);
                if (fast_subtract_rotations_cf(&decoy_instance_vars->collision_voice_cooldown))
                    allocate_voice_for_target_entry(0, 0, moby),
                        decoy_instance_vars->collision_voice_cooldown = FUN_001fef20_2c4380(20);
                else
                    add_random_offset_to_vector_xyz(frame_time, &decoy_instance_vars->velocity);
            }
        }
        if (hero.state.current == 0x1e) {
            target = hero.moby;
            *(int *)(target + 0x94) = *(int *)(*(char **)(target + 0x24) + 0x10);
        }
        break;
    case DECOY_SETTLED_MOVEMENT:
    case DECOY_PRE_REMOVAL_DELAY:
        hit = FUN_L00_0025a420(moby, 0x30001, 0);
        MOBY(moby)->rot.x = FUN_002000e8_2c4380(
            MOBY(moby)->rot.x,
            FUN_00200130_2c4380(decoy_instance_vars->target_rot_x, MOBY(moby)->rot.x) *
                (frame_scale * 0.1f));
        MOBY(moby)->rot.y = FUN_002000e8_2c4380(
            MOBY(moby)->rot.y,
            FUN_00200130_2c4380(decoy_instance_vars->target_rot_y, MOBY(moby)->rot.y) *
                (frame_scale * 0.1f));
        update_decoy_proximity_motion((S_2c40a0 *)moby);
        FUN_001fef68_2c4380(&decoy_instance_vars->same_source_hit_timer);
        fast_dec_timer_s16(&decoy_instance_vars->hit_cooldown_timer);
        if (hit) {
            target = (char *)hit;
            if (*(int *)(target + 0x20) != decoy_instance_vars->last_hit_source ||
                decoy_instance_vars->same_source_hit_timer <= 0) {
                decoy_instance_vars->last_hit_source = *(int *)(target + 0x20);
                decoy_instance_vars->same_source_hit_timer = FUN_001fef20_2c4380(30);
                /* Fakematch: the empty loop keeps the timer read below the cooldown store. */
                do {
                } while (0);
                if (decoy_instance_vars->hit_cooldown_timer == 0) {
                    if (*(u16 *)(target + 0x28) == 0x101 &&
                        ((*(u64 *)((char *)decoy_instance_vars + 0x68) & 0xffffff00000000ULL) ==
                         0)) {
                        decoy_instance_vars->hit_cooldown_timer = FUN_001fef20_2c4380(100);
                        decoy_instance_vars->hit_cooldown_latched = 1;
                    } else
                        decoy_instance_vars->hit_cooldown_latched = 0;
                    allocate_voice_for_target_entry(0, 0, moby);
                    {
                        float len = *(float *)(target + 0x2c);
                        ((u8 *)decoy_instance_vars)[7] = 0xfa;
                        decoy_instance_vars->health -= len;
                    }
                    start_hit_color_flash(moby, decoy_instance_vars);
                    if (decoy_instance_vars->health < 0.0f) {
                        *(OvlQuad *)scratch.w = 0;
                        scratch.w[0] = frame_scale * 0.5f;
                        scratch.w[2] = frame_scale * 0.7f;
                        scratch.w[1] = scratch.w[0];
                        *(OvlQuad *)position_effect_and_normal_scratch = *(OvlQuad *)scratch.w;
                        add_random_offset_to_vector_xyz(scratch.w[0],
                                                        position_effect_and_normal_scratch);
                        spawn_explosion(0,
                                        moby,
                                        position_effect_and_normal_scratch,
                                        0,
                                        0,
                                        4.0f,
                                        0,
                                        0,
                                        0,
                                        3.0f,
                                        9.0f,
                                        1.0f,
                                        12.0f,
                                        2,
                                        1,
                                        1,
                                        -1,
                                        active);
                        if (active) {
                            initialize_hit_descriptor(effect + effect, scratch.w, moby, 0x810000);
                            ((u8 *)scratch.x)[8] = 2;
                            ((u8 *)scratch.x)[9] = 1;
                            *(u16 *)((u8 *)scratch.x + 10) = *(u16 *)(moby + 0xa6);
                            collect_mobies_in_sphere(
                                &MOBY(moby)->pos, effect * 3.0f, 0x10, moby, scratch.w);
                        }
                        mine_spawn_drops(moby, D_L00_001B0830_lui);
                        mark_moby_for_removal(moby);
                        return;
                    }
                }
            }
        }
        ((u8 *)moby)[0xa4] = 0xff;
        update_hit_color_flash(moby, decoy_instance_vars);
        if (MOBY(moby)->anim_flags & 2) {
            if (((u8 *)moby)[0x53] != 2)
                blend_moby_animation(moby, 2, 0, FUN_001fef20_2c4380(10));
            MOBY(moby)->anim_speed = random_float_between(0.7f, 1.05f);
        }
        if ((u8)MOBY(moby)->state != DECOY_PRE_REMOVAL_DELAY) {
            len = probe_ground_height(&MOBY(moby)->pos, 0, 0.5f);
            if (len != 0.0f) {
                destroy_decoy_on_selected_materials(moby);
                if (len + 0.02f < MOBY(moby)->pos.z) {
                    decoy_instance_vars->velocity[2] -= frame_time_sq * 9.8f;
                    FUN_001ff278_2c4380(
                        &MOBY(moby)->pos, &MOBY(moby)->pos, &decoy_instance_vars->velocity);
                    len = probe_ground_height(&MOBY(moby)->pos, 0, 0.5f);
                    if (MOBY(moby)->pos.z < len) {
                        MOBY(moby)->pos.z = len;
                        qzero(&decoy_instance_vars->velocity);
                        qcopy(position_effect_and_normal_scratch, &coll_hit.normal_x);
                        cos_angle = FUN_001ff7e8_2c4380(MOBY(moby)->rot.z);
                        sin_angle = FUN_001ff800_2c4380(MOBY(moby)->rot.z);
                        scratch.w[0] = position_effect_and_normal_scratch[0] * cos_angle +
                                       position_effect_and_normal_scratch[1] * sin_angle;
                        cos_angle = FUN_001ff7e8_2c4380(MOBY(moby)->rot.z);
                        sin_angle = FUN_001ff800_2c4380(MOBY(moby)->rot.z);
                        scratch.w[1] = position_effect_and_normal_scratch[1] * cos_angle -
                                       position_effect_and_normal_scratch[0] * sin_angle;
                        scratch.w[2] = position_effect_and_normal_scratch[2];
                        decoy_instance_vars->target_rot_x =
                            -FUN_001ff8b0_2c4380(FUN_001ff1e0_2c4380(scratch.w[0] * scratch.w[0] +
                                                                     scratch.w[2] * scratch.w[2]),
                                                 scratch.w[1]);
                        decoy_instance_vars->target_rot_y =
                            FUN_001ff8b0_2c4380(scratch.w[2], scratch.w[0]);
                    }
                } else
                    qzero(&decoy_instance_vars->velocity);
                if (coll_hit.moby)
                    FUN_L00_00260400(moby,
                                     coll_hit.moby,
                                     &MOBY(moby)->pos,
                                     moby + 0x40,
                                     &MOBY(moby)->pos,
                                     moby + 0x40);
            }
        } else {
            if (MOBY(moby)->scale < *(float *)(*(char **)(moby + 0x24) + 0x24))
                MOBY(moby)->scale += frame_scale * 0.02f;
            if (fast_dec_timer_s16(&decoy_instance_vars->timer)) {
                if (((u8 *)moby)[0x53])
                    blend_moby_animation(moby, 0, 0, FUN_001fef20_2c4380(60));
                mine_spawn_drops(moby, D_L00_001B0830_lui);
                MOBY(moby)->state = DECOY_WAIT_TO_SHRINK;
            }
        }
        break;
    case DECOY_WAIT_TO_SHRINK:
        len = probe_ground_height(&MOBY(moby)->pos, 0, 0.5f);
        if (len != 0.0f && coll_hit.moby)
            FUN_L00_00260400(
                moby, coll_hit.moby, &MOBY(moby)->pos, moby + 0x40, &MOBY(moby)->pos, moby + 0x40);
        if (MOBY(moby)->anim_flags & 2)
            *(int *)(moby + 0x58) = 0;
        if (fast_dec_timer_s16(&decoy_instance_vars->timer) || (MOBY(moby)->anim_flags & 2))
            MOBY(moby)->state = DECOY_SHRINK_FOR_REMOVAL;
        break;
    case DECOY_SHRINK_FOR_REMOVAL: {
        float len;
        len = probe_ground_height(&MOBY(moby)->pos, 0, 0.5f);
        if (len != 0.0f && coll_hit.moby) {
            FUN_L00_00260400(
                moby, coll_hit.moby, &MOBY(moby)->pos, moby + 0x40, &MOBY(moby)->pos, moby + 0x40);
        }
        len = frame_scale;
        MOBY(moby)->scale -= MOBY(moby)->scale * (len * 0.02f);
        if (!(MOBY(moby)->scale < *(float *)(*(char **)(moby + 0x24) + 0x24) * 0.1f))
            return;
        mark_moby_for_removal(moby);
        break;
    }
    case DECOY_RESET_TO_INITIALIZE:
    default:
        MOBY(moby)->state = DECOY_INITIALIZE;
    }
}
