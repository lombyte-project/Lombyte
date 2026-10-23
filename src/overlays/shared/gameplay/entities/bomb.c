/* Shared moby classes: bomb, morphoray_chicken, ryno, rocket, drone, bolt_crate. */
#include "types.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"

#include "rnc/overlay/quad.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/collision.h"
#include "eetypes.h"
#include "qcopy.h"

/* Vars of a thrown moby, as ThrowVars_2c8440 in trespasser.c without its first 0x10 bytes. */
typedef struct {
    Vec4 vel;                      /* 0x00: velocity per step */
    Vec4 landing;                  /* 0x10: predicted landing point */
    Vec4 normal;                   /* 0x20: surface normal there */
    struct Moby *holder;           /* 0x30: moby it was thrown from */
    s32 effect_timer;              /* 0x34 */
    s16 timer;                     /* 0x38 */
    s16 unk46;
    s32 spawn_count;               /* 0x3C */
} ThrowVars_2c8440;

extern s32 D_L00_0015F5C4_2c8440 __asm__("D_L00_0015F5C4");                          /* nonzero: no throws */
extern float D_0015ED6C __attribute__((section(".sdata")));   /* frame-rate scale */
extern float D_0015ED70 __attribute__((section(".sdata")));
extern CollisionHit coll_hit __asm__("D_L00_00173E40") __attribute__((section(".data")));
/* coll_hit_2c8440's normal as a whole quadword (CollisionHit names only x/y/z) */
extern Vec4 coll_hit_normal_2c8440 __asm__("D_L00_00173E80") __attribute__((section(".data")));
extern Vec4 D_L00_00166DC0_2c8440 __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern f32 atan2_f_2c8440(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations_2c8440(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_001f9dc8_2c8440(f32) __asm__("FUN_001f9dc8");                        /* cos */
extern f32 FUN_001f9de0_2c8440(f32) __asm__("FUN_001f9de0");                        /* sin */
extern void add_vector_xyz_2c8440(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_2c8440(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz_2c8440(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32 scale_game_frames_2c8440(s32) __asm__("FUN_001f96f8");
extern s32 collision_line_2c8440(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern s32 tick_countdown_2c8440(s32 *) __asm__("FUN_001f9740");
extern void FUN_00213508_2c8440(void *, s32, f32) __asm__("FUN_00213508");
extern void FUN_001f4600_2c8440(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L00_002c7df8_2c8440(void) __asm__("FUN_L00_002c7df8");
extern unsigned char *FUN_L00_002603d0_2c8440(int) __asm__("FUN_L00_002603d0");
extern s32 FUN_001f0b58_2c8440(void) __asm__("FUN_001f0b58");

/*
 * Predicts where a thrown moby lands: steps the velocity (with gravity) from
 * its start until collision_line_2c8440 hits ground (or a moby other than the hero
 * and the holder), for up to `timer` steps. Stores the landing point and
 * normal in vars and pulls the point 5% toward D_L00_00166DC0_2c8440. mode 0 needs
 * a live hit moby; mode 1 starts from the hero's hand. Returns 1 on a hit.
 */
s32 predict_doom_bot_spawner_landing(ThrowVars_2c8440 *throw_vars, struct Moby *thrown_moby, s32 mode) __asm__("FUN_L00_002c8440");

s32 predict_doom_bot_spawner_landing(ThrowVars_2c8440 *throw_vars, struct Moby *thrown_moby, s32 mode)
{
    Vec4 simulated_position, simulated_velocity, position_scratch, q, landing_probe_position;
    s32 prediction_timer;
    s32 found = 0;
    s32 i;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4_2c8440 != 0)
        return 0;
    qcopy(&position_scratch, &thrown_moby->pos);
    if (thrown_moby->state == 1) {
        simulated_position.f[0] = FUN_001f9dc8_2c8440(fast_add_rotations_2c8440(-0.36196801066398620605f, atan2_f_2c8440(hero.unk670, hero.unk674))) * 0.85903900861740112305f;
        simulated_position.f[1] = FUN_001f9de0_2c8440(fast_add_rotations_2c8440(-0.36196801066398620605f, atan2_f_2c8440(hero.unk670, hero.unk674))) * 0.85903900861740112305f;
        simulated_position.f[2] = 0.0f;
        add_vector_xyz_2c8440(&simulated_position, &simulated_position, &hero.motion.pos);
        simulated_position.f[2] += 0.39081999659538269043f;
        qcopy(&simulated_velocity, &throw_vars->vel);
        if (hero.state.control_mode == 15)
            add_vector_xyz_2c8440(&simulated_velocity, &simulated_velocity, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0_2c8440((int)hero.ground_moby) && mode)
            add_vector_xyz_2c8440(&simulated_velocity, &simulated_velocity, &hero.motion.displacement);
        prediction_timer = scale_game_frames_2c8440(300);
    } else {
        u8 *holder_vars = throw_vars->holder->pvars;

        qcopy(&simulated_position, &thrown_moby->pos);
        qcopy(&simulated_velocity, &throw_vars->vel);
        {
            s32 timer = throw_vars->timer;
            s32 flag = *(s32 *)(holder_vars + 0x50);

            prediction_timer = timer;
            if (flag)
                throw_vars->unk46 = 1;
        }
    }
    simulated_velocity.f[2] -= D_0015ED6C * 0.5f;
    q.q = ((Vec4 *)&hero.moby->pos)->q;
    q.f[2] = simulated_position.f[2];
    if (collision_line_2c8440(&q, &simulated_position, 0x10, hero.moby, 0) && (FUN_001f0b58_2c8440() || 0.0f < simulated_velocity.f[2])) {
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby != hero.moby && (struct Moby *)coll_hit.moby != throw_vars->holder)
                || prediction_timer < scale_game_frames_2c8440(300) - scale_game_frames_2c8440(10)) {
                landing_probe_position.q = 0;
                landing_probe_position.f[2] = 0.5f;
                qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
                found = 1;
                add_vector_xyz_2c8440(&landing_probe_position, &landing_probe_position, &throw_vars->landing);
                FUN_00213508_2c8440(&landing_probe_position, 0, 0.5f);
            }
        } else if (coll_hit.unk1C > 0) {
            qcopy(&throw_vars->landing, &coll_hit.point);
            found = 1;
        }
    }
    while (!tick_countdown_2c8440(&prediction_timer) && !found) {
        qcopy(&position_scratch, &simulated_position);
        for (i = 9; i >= 0; i--) {
            add_vector_xyz_2c8440(&simulated_position, &simulated_position, &simulated_velocity);
            simulated_velocity.f[2] -= D_0015ED70 * 9.0f;
        }
        if (!collision_line_2c8440(&position_scratch, &simulated_position, 0x10, thrown_moby, 0))
            continue;
        if (!FUN_001f0b58_2c8440() && 0.0f < simulated_velocity.f[2])
            continue;
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby == hero.moby || (struct Moby *)coll_hit.moby == throw_vars->holder)
                && !(prediction_timer < scale_game_frames_2c8440(300) - scale_game_frames_2c8440(10)))
                continue;
            landing_probe_position.q = 0;
            landing_probe_position.f[2] = 0.5f;
            qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
            add_vector_xyz_2c8440(&landing_probe_position, &landing_probe_position, &throw_vars->landing);
            FUN_00213508_2c8440(&landing_probe_position, 0, 0.5f);
            break;
        } else if (coll_hit.unk1C > 0) {
            qcopy(&throw_vars->landing, &coll_hit.point);
            break;
        }
    }
    if (prediction_timer == 0)
        return 0;
    if (mode == 0) {
        if (coll_hit.moby == 0)
            return 0;
        if (FUN_L00_002603d0_2c8440(coll_hit.moby) == 0)
            goto ret0;
    }
    if (mode != 2 || coll_hit.moby == 0 || !FUN_L00_002603d0_2c8440(coll_hit.moby))
        FUN_001f4600_2c8440(FUN_L00_002c7df8_2c8440, thrown_moby);
    throw_vars->normal.q = coll_hit_normal_2c8440.q;
    subtract_vector_xyz_2c8440(&position_scratch, &throw_vars->landing, &D_L00_00166DC0_2c8440);
    scale_vector_xyz_2c8440(&position_scratch, &position_scratch, 0.94999998807907104492f);
    add_vector_xyz_2c8440(&throw_vars->landing, &position_scratch, &D_L00_00166DC0_2c8440);
    return 1;
ret0:
    /* a second return 0 block, as in retail */
    return 0;
}
#include "rnc/math/fast_math.h"

extern u8 D_0013E534 __attribute__((section(".data")));
extern int D_L00_0015FFE4;
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern int fast_dec_timer_s16(short *) __asm__("FUN_001f9770");
extern s32 coll_line_fix(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern s32 coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_L00_001ff290(void *, void *, void *);
extern char *FUN_L00_002603d0(char *);
extern void spawn_moby_bound_effect_particle(struct Moby *) __asm__("FUN_L00_0026ed28");
extern void spawn_moby_collision_particles(struct Moby *) __asm__("FUN_L00_002c8078");
extern void spawn_doom_bot(struct Moby *, void *, void *) __asm__("FUN_L00_002bfe08");
extern void FUN_L00_00260400(struct Moby *, s32, void *, void *, void *, void *);
extern f32 probe_ground_height(void *, int, f32) __asm__("FUN_00213508");

/* Six checked switch entries: state 5 shares the default state reset.
 * Case 2/3 shaped after rac1-decomp (nonmatching/shared/func_L00_002C9DC8.c). */
void update_doom_bot_spawner(struct Moby *thrown_moby) __asm__("FUN_L00_002c89e8");

void update_doom_bot_spawner(struct Moby *thrown_moby)
{
    enum {
        THROWN_MOBY_STATE_INITIALIZE = 0,
        THROWN_MOBY_STATE_GROW_TO_QUARTER_SCALE = 1,
        THROWN_MOBY_STATE_MOVE_WITH_COLLISION = 2,
        THROWN_MOBY_STATE_SPAWN_SEQUENCE = 3,
        THROWN_MOBY_STATE_FADE_OUT = 4,
        THROWN_MOBY_STATE_RESET_TO_INITIALIZE = 5
    };
    Vec4 next_pos, start, end, probe;
    ThrowVars_2c8440 *vars;
    struct Moby *other;
    s32 live_count;
    s32 settled;
    s32 settled_frames;
    u8 *collision_point;
    /* Fakematch: hidden zero; the dead `last` keeps the 0xBA count loop's
     * count++ a branch (jump would turn a one-insn then-block into movn). */
    s32 never = 0;
    struct Moby *last;

    if (!thrown_moby) return;
    vars = (ThrowVars_2c8440 *)thrown_moby->pvars;
    if (!vars) return;
    if (thrown_moby->pos.x < 2.0f || 1021.0f < thrown_moby->pos.x ||
        thrown_moby->pos.y < 2.0f || 1021.0f < thrown_moby->pos.y ||
        thrown_moby->pos.z < 2.0f || 1021.0f < thrown_moby->pos.z) goto remove;
    if (hero.unk20A5 && thrown_moby->state < 2)
        thrown_moby->flags |= 0x41;
    else
        thrown_moby->flags &= 0xffbe;
    if (vars->holder->oclass == 0xe5 && vars->unk46 == 0 && fast_vec_length(vars) > 0.0f) {
        if (!hero.ground_moby || !FUN_L00_002603d0((char *)hero.ground_moby))
            predict_doom_bot_spawner_landing(vars, thrown_moby, 1);
        else if (!predict_doom_bot_spawner_landing(vars, thrown_moby, 0))
            predict_doom_bot_spawner_landing(vars, thrown_moby, 2);
    }
    switch (thrown_moby->state) {
    case THROWN_MOBY_STATE_INITIALIZE:
        thrown_moby->state = 1;
        vars->effect_timer = scale_game_frames(4);
        vars->timer = 0;
        vars->spawn_count = 0;
        break;
    case THROWN_MOBY_STATE_GROW_TO_QUARTER_SCALE: {
        vars->timer = 0;
        {
            f32 class_scale = thrown_moby->pclass->scale;
            if (thrown_moby->scale < class_scale * 0.25f)
                thrown_moby->scale = thrown_moby->scale + class_scale * (frame_scale * 0.05f);
        }
        if (fast_dec_timer(&vars->effect_timer)) {
            vars->effect_timer = scale_game_frames(4);
            if (!hero.unk20A5) spawn_moby_bound_effect_particle(thrown_moby);
        }
        if (!hero.vehicle_mode && vars->holder && (vars->holder->state & 0xf0) != 0xf0 &&
            hero.items[0].moby && hero.items[0].moby->oclass == 0xe5) return;
        goto remove;
    }
    case THROWN_MOBY_STATE_MOVE_WITH_COLLISION: {
        f32 size, speed, rest_speed, sink;
        f32 half = 0.5f;
        u8 *carry;
        struct Hero *player;
        size = convert_integer_to_float(D_0013E534) + 1.0f;
        {
            f32 class_scale = thrown_moby->pclass->scale;
            if (thrown_moby->scale < class_scale * half * size)
                thrown_moby->scale = thrown_moby->scale + class_scale * (frame_scale * 0.05f) * size;
        }
        if (fast_dec_timer(&vars->effect_timer)) {
            vars->effect_timer = scale_game_frames(4);
            spawn_moby_bound_effect_particle(thrown_moby);
        }
        vars->vel.f[2] -= frame_time_sq * 9.0f;
        fast_vec_add(&next_pos, &thrown_moby->pos, vars);
        carry = (u8 *)&hero.motion.platform_carry;
        FUN_L00_001ff290(&probe, &thrown_moby->pos, carry);
        qcopy_nc(&start, &thrown_moby->pos);
        sink = frame_scale * 0.15f * thrown_moby->scale;
        start.f[2] -= sink / thrown_moby->pclass->scale;
        qcopy_nc(&end, &next_pos);
        sink = sink / thrown_moby->pclass->scale;
        end.f[2] -= sink;
        qcopy_nc(&probe, &next_pos);
        probe.f[2] += size * 0.2f;
        player = (struct Hero *)(carry - 0x140);
        if (coll_sphere(size * 0.3f, &probe, 4, player->moby)) {
            collision_point = coll_hit.pad30;
            qcopy_nc(&thrown_moby->pos, collision_point);
            thrown_moby->pos.z -= size * 0.2f;
            collision_point += 0x10;
            rest_speed = 0.01f;
            reflect_vector(vars, vars, collision_point);
            fast_vec_scale(vars, vars, half);
            speed = fast_vec_length(vars);
            if (speed < frame_scale * rest_speed) goto settle;
        } else {
            if (!coll_line_fix(&start, &end, 0, player->moby, 0)) {
                goto collision_miss;
            }
            collision_point = (u8 *)&coll_hit.point;
            qcopy_nc(&thrown_moby->pos, collision_point);
            thrown_moby->pos.z += (frame_scale * 0.15f * thrown_moby->scale) / thrown_moby->pclass->scale;
            collision_point += 0x20;
            rest_speed = 0.01f;
            reflect_vector(vars, vars, collision_point);
            fast_vec_scale(vars, vars, half);
            speed = fast_vec_length(vars);
            if (speed < frame_scale * rest_speed) goto settle;
        }
        if (player->ground_moby) {
            if (FUN_L00_002603d0((char *)player->ground_moby)) {
                speed = fast_vec_length(vars);
                if (speed < frame_scale * rest_speed + fast_vec_length(carry - 0x40)) {
settle:
                    settled = (u16)vars->timer + 1;
                    vars->timer = settled;
                    if (!coll_hit.moby || coll_hit.unk1C > 0 ||
                        (settled_frames = (s16)settled, scale_game_frames(240) < settled_frames)) {
                        thrown_moby->state = 3;
                        vars->timer = 0;
                    }
                }
            }
            if (hero.ground_moby && FUN_L00_002603d0((char *)hero.ground_moby))
                fast_vec_add(vars, vars, &hero.motion.displacement);
        }
        fast_vec_add(&thrown_moby->pos, &thrown_moby->pos, vars);
        spawn_moby_collision_particles(thrown_moby);
        break;
collision_miss:
        qcopy_nc(&thrown_moby->pos, &next_pos);
        return;
    }
    case THROWN_MOBY_STATE_SPAWN_SEQUENCE: {
        f32 ground;
        /* Fakematch: reusing case 2's pointer here makes it cross a call, so it gets s0 there. */
        collision_point = (u8 *)&thrown_moby->pos;
        ground = probe_ground_height(collision_point, 0, 0.5f);
        if (ground != 0.0f && coll_hit.moby)
            FUN_L00_00260400(thrown_moby, coll_hit.moby, collision_point, &thrown_moby->rot, collision_point, &thrown_moby->rot);
        if (fast_dec_timer_s16(&vars->timer)) {
            live_count = 0;
            for (other = (struct Moby *)D_L00_0015FFE4; other; other = other->next) {
                if (other->oclass == 0xba) {
                    /* Fakematch: dead `last = other` (see `never`). */
                    if ((other->state & 0xf0) != 0xf0) { live_count++; last = other; }
                }
            }
            /* Fakematch: keeps `last` alive until cse2 folds this to nothing. */
            live_count += (s32)last & never;
            if (live_count >= 8) {
                vars->timer = scale_game_frames(15);
            } else {
                start.f[0] = fast_cos((f32)vars->spawn_count * 6.2831855f * 0.25f);
                start.f[0] = frame_scale * 0.05f * start.f[0];
                start.f[1] = fast_sin((f32)vars->spawn_count * 6.2831855f * 0.25f);
                start.f[2] = frame_scale * 0.02f;
                start.f[1] = frame_scale * 0.05f * start.f[1];
                if (hero.ground_moby && FUN_L00_002603d0((char *)hero.ground_moby))
                    fast_vec_add(&start, &start, &hero.motion.displacement);
                fast_vec_add(&next_pos, &start, &thrown_moby->pos);
                spawn_doom_bot(vars->holder, &next_pos, &start);
                vars->timer = scale_game_frames(8);
                {
                    s32 spawned = vars->spawn_count + 1;
                    vars->spawn_count = spawned;
                    if (spawned > 3) thrown_moby->state = 4;
                }
            }
        }
        if (vars->spawn_count < 2 && fast_dec_timer(&vars->effect_timer)) {
            vars->effect_timer = scale_game_frames(4);
            spawn_moby_bound_effect_particle(thrown_moby);
        }
        break;
    }
    case THROWN_MOBY_STATE_FADE_OUT: {
        f32 ground;
        thrown_moby->alpha -= 4;
        thrown_moby->scale *= 0.92f;
        ground = probe_ground_height(&thrown_moby->pos, 0, 0.5f);
        if (ground != 0.0f && coll_hit.moby)
            FUN_L00_00260400(thrown_moby, coll_hit.moby, &thrown_moby->pos, &thrown_moby->rot, &thrown_moby->pos, &thrown_moby->rot);
        if (thrown_moby->alpha) return;
remove:
        mark_moby_for_removal(thrown_moby);
        break;
    }
    case THROWN_MOBY_STATE_RESET_TO_INITIALIZE:
    default:
        thrown_moby->state = 0;
    }
}
/* 0x002c92d8, 112 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// resets the fields of a record (if non-null) to their defaults
/* Fills the block from 0x78 with fixed defaults and -1 ids. */
void reset_morphoray_chicken_slot_defaults(char *record) __asm__("FUN_L00_002c92d8");

void reset_morphoray_chicken_slot_defaults(char *record) {
    if (record != 0) {
        *(int *)(record + 0x78) = 0;
        *(float *)(record + 0x7C) = 0.05f;
        *(float *)(record + 0x80) = 0.863f;
        *(int *)(record + 0x84) = 4;
        *(int *)(record + 0x88) = 3;
        *(float *)(record + 0x8C) = 0.625f;
        *(short *)(record + 0x9C) = -1;
        *(short *)(record + 0x9E) = -1;
        *(int *)(record + 0xA0) = -1;
        *(int *)(record + 0xA4) = -1;
        *(int *)(record + 0xA8) = -1;
        *(int *)(record + 0xAC) = -1;
    }
}

#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"

typedef union {
    u128 q;
    f32 f[4];
} V4;

typedef struct {
    u8 pad00[0x70];
    void *unk70;
    u8 pad74[0xB0 - 0x74];
} Node;

typedef struct {
    u8 pad00[0x10];
    void *unk10;
    void *unk14;
    u8 pad18[0x64 - 0x18];
    u16 unk64;
    u16 unk66;
    u8 pad68[0x70 - 0x68];
    s32 unk70;
    u32 unk74;
    f32 unk78;
} Sub;

/* Class header view: pclass + 0xE and + 0x10. */
typedef struct {
    u8 pad00[0xE];
    u8 unkE;
    u8 pad0F;
    u32 unk10;
} Part;

typedef struct {
    V4 v;
    f32 t1;
    f32 t2;
    f32 t3;
} Loc;

typedef struct {
    u8 pad00[4];
    u8 f4;
    u8 f5;
    u8 f6;
} Opt;

typedef struct {
    u8 pad00[0xC];
    Opt *unk0C;
} Holder;

typedef struct {
    u8 pad00[0x10];
    V4 pos;
    u8 pad20[0x34 - 0x20];
    u16 flags34;
    u8 pad36[0x38 - 0x36];
    u64 unk38;
    u8 pad40[0x78 - 0x40];
    Holder *unk78;
} State;

typedef struct {
    u8 pad00[0x28];
    f32 f28;
    u8 pad2C[0x40 - 0x2C];
    V4 v40;
} Data;

extern u8 D_0013E520[];
extern Data D_L00_00173E40;
extern struct Moby *D_L00_001DD100[];
extern Node D_L00_001DD150[];
extern u8 D_L00_001DDF10[];
extern s32 D_L00_00161854 __attribute__((sda));
extern s32 D_L00_00161854_gp __asm__("D_L00_00161854") __attribute__((sda));
extern u32 D_L00_00161840[1] __attribute__((sda));

extern void *create_moby(int) __asm__("FUN_0020c4f8");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 probe_ground_height(void *, int, f32) __asm__("FUN_00213508");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 square_root(f32) __asm__("FUN_001f9988");
extern s32 FUN_0022da68_c(s32, s32, void *) __asm__("FUN_0022da68");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 FUN_L00_00257b90(s32, s32);
extern f32 convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern void FUN_L00_00257470(void *, int, int);
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void set_moby_color(void *, u8, u8, u8) __asm__("FUN_L00_002502f0");
extern void FUN_L00_00269b70(void *, int, int, int, int, f32, f32);
extern void fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
extern void FUN_L00_002c92d8_c(void *) __asm__("FUN_L00_002c92d8");
extern void FUN_L00_00250df8(void *);

struct Moby *FUN_L00_002c9348(State *source_state) {
    struct Moby *spawned_moby;
    Sub *moby_vars;
    Loc loc;
    struct Moby *previous_slot_moby;
    void *source_record;
    Opt *color_options;
    f32 ground_height_pitch_and_variant_scratch;
    f32 yaw_cosine_scratch;
    f32 sn;

    spawned_moby = (struct Moby *)create_moby(0x10E);
    if (spawned_moby != NULL) {
        spawned_moby->update_dist = 0x7F;
        spawned_moby->draw_dist = 0x7F;
        spawned_moby->visible = 1;
        moby_vars = (Sub *)spawned_moby->pvars;
        if ((source_state->flags34 & 0x20) != 0) {
            color_options = source_state->unk78->unk0C;
            if (color_options != NULL) {
                set_moby_color(spawned_moby, color_options->f4, color_options->f5, color_options->f6);
            } else {
                spawned_moby->spawn_frame = source_state->unk38;
            }
        } else {
            spawned_moby->spawn_frame = source_state->unk38;
        }
        spawned_moby->anim_speed = random_float_between(0.85f, 1.15f);
        qcopy(&spawned_moby->pos, &source_state->pos);
        clear_u64_value(&spawned_moby->rot);
        spawned_moby->rot.z = fast_arc_tan(spawned_moby->pos.x - hero.motion.pos.f[0], spawned_moby->pos.y - hero.motion.pos.f[1]);
        ground_height_pitch_and_variant_scratch = probe_ground_height(&spawned_moby->pos, 0, 0.5f);
        if (spawned_moby->pos.z <= ground_height_pitch_and_variant_scratch) {
            spawned_moby->pos.z = ground_height_pitch_and_variant_scratch;
            spawned_moby->pos.z = D_L00_00173E40.f28;
            qcopy(&loc.v, (u8 *)&D_L00_00173E40 + 0x40);
            yaw_cosine_scratch = fast_cos(spawned_moby->rot.z);
            sn = fast_sin(spawned_moby->rot.z);
            loc.t1 = loc.v.f[0] * yaw_cosine_scratch + loc.v.f[1] * sn;
            yaw_cosine_scratch = fast_cos(spawned_moby->rot.z);
            sn = fast_sin(spawned_moby->rot.z);
            loc.t2 = loc.v.f[1] * yaw_cosine_scratch - loc.v.f[0] * sn;
            loc.t3 = loc.v.f[2];
            ground_height_pitch_and_variant_scratch = fast_arc_tan(square_root(loc.t1 * loc.t1 + loc.t3 * loc.t3), loc.t2);
            spawned_moby->rot.x = -ground_height_pitch_and_variant_scratch;
            spawned_moby->rot.y = fast_arc_tan(loc.t3, loc.t1);
        }
        spawned_moby->collision = ((Part *)spawned_moby->pclass)->unk10;
        FUN_0022da68_c(1, 0, spawned_moby);
        spawned_moby->state = 1;
        moby_vars->unk66 = 0;
        moby_vars->unk64 = FUN_L00_00257b90(scale_game_frames(0x78), scale_game_frames(0x12C));
        ground_height_pitch_and_variant_scratch = convert_integer_to_float(D_0013E520[0x15]);
        moby_vars->unk70 = -1;
        moby_vars->unk78 = ground_height_pitch_and_variant_scratch * 3.0f + 1.0f;
        moby_vars->unk74 = D_0013E520[0x15];
        spawned_moby->command = moby_vars->unk74;
        FUN_L00_00257470(source_state, 0, -1);
        source_record = suck_get_aim_height(source_state);
        if (source_record != NULL && *(u8 *)((u8 *)source_record + 0xE) == 1) {
            *(u8 *)((u8 *)source_record + 0xE) = 2;
        } else {
            mark_moby_for_removal(source_state);
        }
        yaw_cosine_scratch = 0.0f;
        FUN_L00_00269b70(&spawned_moby->pos, 0x7F, 0x7F, 0x7F, scale_game_frames(0xF), 1000000.0f, yaw_cosine_scratch);
        fill_transfer_words(moby_vars, 0, 0x20);
        previous_slot_moby = D_L00_001DD100[D_L00_00161854];
        if (previous_slot_moby != NULL && previous_slot_moby->oclass == 0x10E && previous_slot_moby->state != 0xFE && previous_slot_moby->state != 0xFD) {
            if (previous_slot_moby->state == 5) {
                moby_vars->unk78 = yaw_cosine_scratch;
                spawned_moby->flags &= ~0x20;
            } else if (!moby_vars->unk74) {
                moby_vars->unk14 = (void *)&D_L00_001DD150[D_L00_00161854];
                moby_vars->unk10 = (void *)(D_L00_001DDF10 + D_L00_00161854 * 0x60);
                spawned_moby->flags |= 0x1020;
                ((Node *)moby_vars->unk14)->unk70 = (void *)D_L00_00161840;
            }
            ((Sub *)D_L00_001DD100[D_L00_00161854]->pvars)->unk70 = (void *)1;
        } else if (!moby_vars->unk74) {
            moby_vars->unk14 = (void *)&D_L00_001DD150[D_L00_00161854];
            moby_vars->unk10 = (void *)(D_L00_001DDF10 + D_L00_00161854 * 0x60);
            spawned_moby->flags |= 0x1020;
            ((Node *)moby_vars->unk14)->unk70 = (void *)D_L00_00161840;
        }
        FUN_L00_002c92d8_c(moby_vars->unk14);
        D_L00_001DD100[D_L00_00161854] = spawned_moby;
        D_L00_00161854_gp = D_L00_00161854 + 1;
        D_L00_00161854 = (D_L00_00161854 + 1) % 20;
        if (moby_vars->unk74) {
            spawned_moby->flags &= ~0x20;
        }
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
#include "rnc/math/vector.h"

/* Update for moby class 270 (morphoray chicken): wanders, pecks and flees from the hero, grows, and bursts into feathers when its health runs out. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CABC0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

/* Four floats aligned to 16 bytes, the layout moved by one lq/sq. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A;

/* find_moby_hit result: hit moby in the low word of 0x20, flags in the high word. */
typedef struct {
    u8 pad00[0x20];
    union {
        u64 bits;
        struct {
            struct Moby *moby;
            u32 flags;
        } s;
    } hit;
    u8 pad28[0x2C - 0x28];
    f32 damage;
} Contact;

/* Vars of a morph-o-ray chicken, set up by FUN_L00_002c9348. */
typedef struct {
    u8 pad0[0x10];
    void *unk10;
    char *unk14;
    u8 pad18[8];
    u8 manipulator[0x44];          /* 0x20: scaled by FUN_L00_00263ac8 */
    short sound_timer;             /* 0x64 */
    short flee_timer;              /* 0x66 */
    float turn_speed;              /* 0x68 */
    float fall_speed;              /* 0x6C */
    int expire_timer;              /* 0x70: -1 keeps the chicken */
    int gold;                      /* 0x74: made by the gold morph-o-ray: grows, explodes when it dies */
    float health;                  /* 0x78 */
    int hit_cooldown;              /* 0x7C */
} ChickenVars;

extern int random_integer_below(int) __asm__("FUN_00213260");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern int allocate_voice_for_target_entry(int, int, char *) __asm__("FUN_0022da68");
extern Contact *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern void blend_moby_animation(Moby *, int, int, int) __asm__("FUN_00212f90");
/* Argument order as the chicken passes them: the order of their definitions changes the code. */
extern void FUN_L00_00263ac8(void *, int, void *, float);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");
extern int FUN_L00_001f2868(void *, float, int, void *, void *);
void spawn_explosion(char *m, void *unused, char *pos, float fA, float fB, int n, int k8, int k9, float fC, float fD, float fE, float fF, int k10, float fG, int k11, int s16, int s18, unsigned char flag) __asm__("FUN_L00_0025e450");
extern void FUN_L00_0025a9f8(void *, void *, void *, int, int, int, int, int, float, float, float);
unsigned char *spawn_feather_moby(int unused, void *pos, float *vel) __asm__("FUN_L00_002cbf68");
extern int FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);
extern void FUN_L00_00258278(void *, float *, float, float, float, float);
extern float FUN_L00_00259710(void *);
extern int FUN_L00_002dc9f0(void *, void *);
extern float D_L00_0015F5D0;
extern float D_L00_0015F5D4;
extern int chicken_manipulator_joint __asm__("D_L00_00161850") __attribute__((sda));
extern float feather_pitch_deg __asm__("D_L00_00161858") __attribute__((sda));
extern float feather_speed_min __asm__("D_L00_0016185C") __attribute__((sda));
extern float feather_speed_max __asm__("D_L00_00161860") __attribute__((sda));
extern float feather_height __asm__("D_L00_00161864") __attribute__((sda));
extern float feather_radius_min __asm__("D_L00_00161868") __attribute__((sda));
extern float feather_radius_max __asm__("D_L00_0016186C") __attribute__((sda));
extern char D_L00_00177F00[];

void update_morphoray_chicken(Moby *chicken_moby) __asm__("FUN_L00_002c97e0");

void update_morphoray_chicken(Moby *chicken_moby) {
    enum {
        CHICKEN_STATE_INITIALIZE = 0,
        CHICKEN_STATE_IDLE = 1,
        CHICKEN_STATE_TURN_AWAY_FROM_HERO = 2,
        CHICKEN_STATE_FLEE_FROM_HERO = 3,
        CHICKEN_STATE_CLOSE_RANGE_FLEE_FROM_HERO = 4,
        CHICKEN_STATE_EMIT_FEATHERS = 5
    };
    Vec4A next_pos;
    Vec4A ahead;
    Vec4A motion_and_normal_scratch;
    Vec4A point;
    ChickenVars *vars;
    struct Hero *player;
    float anim_speed;
    float flee_angle;
    float size;
    float speed;
    float radius;
    float angle;
    int path_clear;
    int i;
    int j;
    int count;
    Contact *hit;

    path_clear = 0;
    player = &hero;
    vars = (ChickenVars *)chicken_moby->pvars;
    anim_speed = chicken_moby->anim_speed;
    flee_angle = fast_arc_tan(chicken_moby->pos.x - player->motion.pos.f[0], chicken_moby->pos.y - player->motion.pos.f[1]);
    if (chicken_moby->state == 0) {
        chicken_moby->state = 1;
        vars->flee_timer = 0;
        vars->expire_timer = -1;
        vars->sound_timer = random_int_between(scale_game_frames(0x78), scale_game_frames(0x12C));
        vars->health = 1.0f;
    }
    if (chicken_moby->state == 6) {
        return;
    }
    if (D_0013E520[0x15] != 0) {
        FUN_L00_00263ac8(chicken_moby, chicken_manipulator_joint, vars->manipulator, 5.0f);
    } else {
        FUN_L00_00263ac8(chicken_moby, chicken_manipulator_joint, vars->manipulator, 7.7f);
    }
    qcopy(&next_pos, &chicken_moby->pos);
    if (vars->expire_timer >= 0 && fast_dec_timer(&vars->expire_timer) != 0 && chicken_moby->visible == 0) {
        mark_moby_for_removal(chicken_moby);
        return;
    }
    if (vars->expire_timer >= 0) {
        if ((0.9f < D_L00_0015F5D4 || 0.9f < D_L00_0015F5D0) && chicken_moby->visible == 0 && vars->expire_timer != -1) {
            if (random_integer_below(0xC7) == 0) {
                mark_moby_for_removal(chicken_moby);
                return;
            }
            vars->expire_timer = random_int_between(0x14, 0x1E);
        }
        if (1.0f < D_L00_0015F5D4 || 1.0f < D_L00_0015F5D0) {
            if (chicken_moby->visible != 0) {
                if (random_integer_below(0x63) == 0) {
                    vars->health = 0.0f;
                } else {
                    vars->expire_timer = random_int_between(0x14, 0x1E);
                }
            } else if (random_integer_below(0x18) == 0) {
                mark_moby_for_removal(chicken_moby);
                return;
            }
        }
    }
    if (vars->gold > 0) {
        chicken_moby->scale += ((convert_integer_to_float(vars->gold) * 3.0f + 1.0f) * chicken_moby->pclass->scale - chicken_moby->scale) * 0.1f;
    }
    size = chicken_moby->scale / chicken_moby->pclass->scale;
    if (fast_dec_timer(&vars->hit_cooldown) != 0) {
        hit = find_moby_hit(chicken_moby, -1, 0);
        if (hit != 0) {
            allocate_voice_for_target_entry(1, 0, (char *)chicken_moby);
            vars->health -= hit->damage;
            vars->hit_cooldown = scale_game_frames(0x19);
        }
    }
    chicken_moby->hit_slot = 0xFF;
    if (vars->health < 1.0f) {
        if (vars->gold == 0) {
            for (i = 0; i < truncate_float_to_s32(size) * 20; i++) {
                radius = random_float_between(0.0f, 0.37f) * size;
                angle = random_angle_radians();
                build_spherical_offset(&point, radius, angle, random_angle_radians());
                point.z += random_float_between(0.25f, 0.5f) * size;
                fast_vec_add(&point, &point, &next_pos);
                radius = random_float_between(frame_time * 0.1f, frame_time) * size;
                angle = random_angle_radians();
                build_spherical_offset(&motion_and_normal_scratch, radius, angle, random_angle_radians());
                spawn_effect_particle_with_target_scalar(&point, &motion_and_normal_scratch, 0x3F7F7F7F, 0x2F2F2F, size * 150000.0f, random_int_between(scale_game_frames(0x1E), scale_game_frames(0x39)));
            }
        } else {
            *(u128 *)&point = 0;
            point.x = frame_scale * 0.5f;
            point.z = frame_scale * 0.7f;
            point.y = frame_scale * 0.5f;
            *(u128 *)&motion_and_normal_scratch = *(u128 *)&point;
            spawn_explosion((char *)chicken_moby, &motion_and_normal_scratch, 0, 0.0f, 2.0f, 0, 0, 0, 4.0f, 3.0f, 9.0f, 1.0f, -1, 12.0f, 1, 1, -1, (unsigned char)vars->gold);
            count = FUN_L00_001f2868(&chicken_moby->pos, 3.0f, 0x10, chicken_moby, 0);
            *(u128 *)&point = *(u128 *)&chicken_moby->pos;
            FUN_L00_0025a9f8(chicken_moby, &point, D_L00_00177F00, count, 0, 0x830000, 2, 1, 1.0f, 1.0f, 1.0f);
        }
        count = random_int_between(5, 8);
        if (0.85f < D_L00_0015F5D0 || 0.85f < D_L00_0015F5D4) {
            if (1.0f < D_L00_0015F5D0 || 1.0f < D_L00_0015F5D4) {
                count = 1;
            } else {
                count /= 2;
            }
        }
        for (j = 0; j < count; j++) {
            radius = random_float_between(feather_radius_min, feather_radius_max) * size;
            angle = random_angle_radians();
            build_spherical_offset(&point, radius, angle, random_angle_radians());
            point.z += feather_height * size;
            fast_vec_add(&point, &point, &next_pos);
            angle = random_float_between(feather_speed_min * frame_time, feather_speed_max * frame_time);
            build_spherical_offset(&motion_and_normal_scratch, angle, random_angle_radians(), feather_pitch_deg * 0.017453292f);
            motion_and_normal_scratch.z = random_float_between(3.5f, 5.5f) * frame_time;
            spawn_feather_moby((int)chicken_moby, &point, &motion_and_normal_scratch.x);
        }
        allocate_voice_for_target_entry(1, 0, (char *)chicken_moby);
        if (vars->expire_timer >= 0) {
            mark_moby_for_removal(chicken_moby);
            return;
        }
        chicken_moby->collision = 0;
        chicken_moby->flags = (chicken_moby->flags & 0xEFFF) | 0x41;
        vars->gold = 0;
        chicken_moby->state = 6;
        chicken_moby->command = 0;
        return;
    }
    if (fast_dec_timer_s16(&vars->sound_timer) != 0) {
        allocate_voice_for_target_entry(random_integer_below(100) >= 75, 0, (char *)chicken_moby);
        vars->sound_timer = random_int_between(scale_game_frames(0x78), scale_game_frames(0x12C));
    }
    if (vars->expire_timer >= 0) {
        speed = (frame_time + frame_time) * size;
    } else {
        speed = 0.333f;
    }
    switch (chicken_moby->state) {
    case CHICKEN_STATE_INITIALIZE:
        break;
    case CHICKEN_STATE_IDLE:
        if (chicken_moby->anim_flags & 2) {
            blend_moby_animation(chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
        }
        if (fast_dec_timer_s16(&vars->flee_timer) != 0 && vector_distance(&chicken_moby->pos, &hero.motion.pos) < size * 5.0f) {
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear != 0 || (fast_difference_between_rotations(chicken_moby->rot.z, fast_arc_tan(next_pos.x - chicken_moby->pos.x, next_pos.y - chicken_moby->pos.y)) < 1.5707964f && fast_vec_dist(&chicken_moby->pos, &next_pos) > size * 0.01f)) {
                chicken_moby->state = 3;
                blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
            } else {
                vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
            }
        } else if (vector_distance(&chicken_moby->pos, &hero.motion.pos) < size * 5.0f && 0.5235988f < fast_difference_between_rotations(flee_angle, chicken_moby->rot.z)) {
            FUN_L00_00258490(chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            chicken_moby->state = 2;
            blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
        } else {
            FUN_L00_00258490(chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
        }
        break;
    case CHICKEN_STATE_TURN_AWAY_FROM_HERO:
        FUN_L00_00258278(chicken_moby, &vars->turn_speed, flee_angle, frame_time_sq * 0.7853982f, frame_time_sq * 1.5707964f, frame_time * 0.7853982f);
        FUN_L00_00258490(chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
        if (fast_dec_timer_s16(&vars->flee_timer) != 0 && vector_distance(&chicken_moby->pos, &hero.motion.pos) < size * 5.0f) {
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            if (absolute_float(probe_ground_height(&ahead, 0, 0.5f) - ahead.z) <= (frame_time + frame_time) * size) {
                chicken_moby->state = 3;
                blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
            }
        } else if (fast_difference_between_rotations(flee_angle, chicken_moby->rot.z) < 0.08726646f || vector_distance(&chicken_moby->pos, &hero.motion.pos) > size * 6.0f) {
            chicken_moby->state = 1;
            vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
            blend_moby_animation(chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
        }
        break;
    case CHICKEN_STATE_FLEE_FROM_HERO:
        if (vector_distance(&chicken_moby->pos, &hero.motion.pos) > size * 6.0f) {
            chicken_moby->state = 1;
            vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
            blend_moby_animation(chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            FUN_L00_00258490(chicken_moby, &next_pos, &next_pos, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
        } else if (vector_distance(&chicken_moby->pos, &hero.motion.pos) < size * 3.0f) {
            FUN_L00_00258278(chicken_moby, &vars->turn_speed, flee_angle, frame_time_sq * 0.7853982f, frame_time_sq * 1.5707964f, frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * (frame_time + frame_time);
            ahead.y = fast_sin(chicken_moby->rot.z) * (frame_time + frame_time);
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 && (1.5707964f < fast_difference_between_rotations(chicken_moby->rot.z, fast_arc_tan(next_pos.x - chicken_moby->pos.x, next_pos.y - chicken_moby->pos.y)) || fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            } else {
                chicken_moby->state = 4;
                blend_moby_animation(chicken_moby, 2, 0, scale_game_frames(10));
            }
        } else {
            FUN_L00_00258278(chicken_moby, &vars->turn_speed, flee_angle, frame_time_sq * 0.7853982f, frame_time_sq * 1.5707964f, frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 && (1.5707964f < fast_difference_between_rotations(chicken_moby->rot.z, fast_arc_tan(next_pos.x - chicken_moby->pos.x, next_pos.y - chicken_moby->pos.y)) || fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            }
        }
        break;
    case CHICKEN_STATE_CLOSE_RANGE_FLEE_FROM_HERO:
        if (fast_dec_timer_s16(&vars->flee_timer) != 0 && vector_distance(&chicken_moby->pos, &hero.motion.pos) > size * 4.0f) {
            FUN_L00_00258278(chicken_moby, &vars->turn_speed, flee_angle, frame_time_sq * 0.7853982f, frame_time_sq * 1.5707964f, frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * frame_time * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * frame_time * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 && (1.5707964f < fast_difference_between_rotations(chicken_moby->rot.z, fast_arc_tan(next_pos.x - chicken_moby->pos.x, next_pos.y - chicken_moby->pos.y)) || fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            } else {
                chicken_moby->state = 3;
                blend_moby_animation(chicken_moby, 3, 0, scale_game_frames(10));
            }
        } else {
            FUN_L00_00258278(chicken_moby, &vars->turn_speed, flee_angle, frame_time_sq * 0.7853982f, frame_time_sq * 1.5707964f, frame_time * 0.7853982f);
            ahead.x = fast_cos(chicken_moby->rot.z) * (frame_time + frame_time) * size;
            ahead.y = fast_sin(chicken_moby->rot.z) * (frame_time + frame_time) * size;
            ahead.z = 0.0f;
            fast_vec_add(&ahead, &ahead, &next_pos);
            path_clear = FUN_L00_00258490(chicken_moby, &next_pos, &ahead, 0, size * 0.2f, size * 0.5f, speed, 0.2617994f);
            if (path_clear == 0 && (1.5707964f < fast_difference_between_rotations(chicken_moby->rot.z, fast_arc_tan(next_pos.x - chicken_moby->pos.x, next_pos.y - chicken_moby->pos.y)) || fast_vec_dist(&chicken_moby->pos, &next_pos) < size * 0.01f)) {
                chicken_moby->state = 1;
                vars->flee_timer = random_int_between(scale_game_frames(0x3C), scale_game_frames(0xB4));
                blend_moby_animation(chicken_moby, random_integer_below(2) == 0, 0, scale_game_frames(5));
            }
        }
        break;
    case CHICKEN_STATE_EMIT_FEATHERS:
        if (FUN_L00_002dc9f0(chicken_moby, vars->unk10) != 0) {
            chicken_moby->state = 0;
            *(short *)(vars->unk14 + 0x68) = 0;
        }
        if (random_integer_below(4) != 0) {
            return;
        }
        {
            Vec4A *spawn_point;
            Vec4A *spawn_velocity;
            spawn_point = &point;
            radius = random_float_between(feather_radius_min, feather_radius_max) * size;
            angle = random_angle_radians();
            build_spherical_offset(spawn_point, radius, angle, random_angle_radians());
            point.z += feather_height * size;
            fast_vec_add(spawn_point, spawn_point, &next_pos);
            spawn_velocity = &motion_and_normal_scratch;
            angle = random_float_between(feather_speed_min * frame_time, feather_speed_max * frame_time);
            build_spherical_offset(spawn_velocity, angle, random_angle_radians(), feather_pitch_deg * 0.017453292f);
            motion_and_normal_scratch.z = random_float_between(3.5f, 5.5f) * frame_time;
            spawn_feather_moby((int)chicken_moby, spawn_point, &spawn_velocity->x);
        }
        return;
    }
    if (path_clear == 0) {
        float movement_distance;
        float max;
        fast_vec_sub(&motion_and_normal_scratch, &next_pos, &chicken_moby->pos);
        movement_distance = fast_vec_length(&motion_and_normal_scratch);
        max = frame_time * size;
        if (max < movement_distance) {
            fast_vec_normalize(&motion_and_normal_scratch, &motion_and_normal_scratch, max);
        }
        fast_vec_add(&next_pos, &chicken_moby->pos, &motion_and_normal_scratch);
        if (absolute_float(FUN_L00_00259710(&next_pos) - chicken_moby->pos.z) <= frame_time * size) {
            qcopy(&chicken_moby->pos, &next_pos);
        }
    } else {
        qcopy(&chicken_moby->pos, &next_pos);
    }
    vars->fall_speed += frame_time_sq * 9.8f;
    chicken_moby->pos.z -= vars->fall_speed;
    {
        float ground = probe_ground_height(&chicken_moby->pos, 0, 0.5f);
        if (chicken_moby->pos.z < ground && ground < chicken_moby->pos.z + size * 0.25f) {
            vars->fall_speed = 0.0f;
            chicken_moby->pos.z = ground;
        }
    }
    if (collision_hit_material() == 0) {
        vars->health = 0.0f;
    }
    if (chicken_moby->state != 5) {
        float pitch;
        float roll;
        chicken_moby->pos.z = coll_hit.point.f[2];
        qcopy(&motion_and_normal_scratch, &coll_hit.normal_x);
        point.x = motion_and_normal_scratch.x * fast_cos(chicken_moby->rot.z) + motion_and_normal_scratch.y * fast_sin(chicken_moby->rot.z);
        point.y = motion_and_normal_scratch.y * fast_cos(chicken_moby->rot.z) - motion_and_normal_scratch.x * fast_sin(chicken_moby->rot.z);
        point.z = motion_and_normal_scratch.z;
        pitch = -fast_arc_tan(square_root(point.x * point.x + point.z * point.z), point.y);
        roll = fast_arc_tan(point.z, point.x);
        chicken_moby->rot.x = fast_add_rotations(chicken_moby->rot.x, fast_subtract_rotations(pitch, chicken_moby->rot.x) * (frame_time * 6.2831855f));
        chicken_moby->rot.y = fast_add_rotations(chicken_moby->rot.y, fast_subtract_rotations(roll, chicken_moby->rot.y) * (frame_time * 6.2831855f));
    }
    if (chicken_moby->pos.z < 0.0f) {
        mark_moby_for_removal(chicken_moby);
    } else {
        chicken_moby->anim_speed = anim_speed;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC210), where it is exact; names translated to the US level program. */

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

/* Moby state 5 while FUN_L00_002db8f8 reports a hit, back to 1 when it stops. */
int FUN_L00_002cadc0(char *a) {
    int r = process_moby_suck_cannon_suction(a);
    if (r == 0) {
        if (*(unsigned char *)(a + 0x20) == 5)
            a[0x20] = 1;
    } else {
        a[0x20] = 5;
    }
    return r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC2B0), where it is exact; names translated to the US level program. */

extern char *get_moby_suck_cannon_record(char *) __asm__("FUN_L00_002db890");
extern int launch_suck_cannon_projectile(char *, int, int, float) __asm__("FUN_L00_002dbc30");
extern int allocate_voice_for_target_entry(int, int, char *) __asm__("FUN_0022da68");

int launch_suck_cannon_projectile_and_update_moby_state(char *moby, int a, int target_moby, float target_height_offset) __asm__("FUN_L00_002cae60");

int launch_suck_cannon_projectile_and_update_moby_state(char *moby, int a, int target_moby, float target_height_offset) {
    char *class_callback_data = get_moby_suck_cannon_record(moby);
    int r = launch_suck_cannon_projectile(moby, a, target_moby, target_height_offset);
    if (r != 0) {
        moby[0x20] = 5;
    } else if ((unsigned char)moby[0x20] == 5) {
        moby[0x20] = 1;
    }
    if (*(short *)(class_callback_data + 0x68) == 6) {
        allocate_voice_for_target_entry(1, 0, moby);
    }
    return r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC360), where it is exact; names translated to the US level program. */

extern int cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");

/* Runs FUN_L00_002dbe20 on the moby and sets its state to 5. */
int cancel_suck_cannon_suction_and_set_moby_state_5(char *moby) __asm__("FUN_L00_002caf10");

int cancel_suck_cannon_suction_and_set_moby_state_5(char *moby) {
    int helper_result = cancel_suck_cannon_suction(moby);
    moby[0x20] = 5;
    return helper_result;
}
/* Returns the word at +0x14 of the moby data, or 0 when either pointer is null. */
s32 FUN_L00_002caf40(s32 moby_address) {
    s32 moby_vars_address;
    if (moby_address != 0) {
        moby_vars_address = *(s32 *)((u8 *)moby_address + 0x78);
        if (moby_vars_address != 0) {
            return *(s32 *)((u8 *)moby_vars_address + 0x14);
        }
        return 0;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC3C0), where it is exact; names translated to the US level program. */

typedef int w128 __attribute__((mode(TI)));

extern char *spawn_source_attached_fading_effect_particle(char *, int, int, int, float) __asm__("FUN_L00_0026daa0");
extern char *func_0020D348_m(int);
extern char D_L00_00173E60_c[] __asm__("D_L00_00173E60");
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_tan_c2(f32, f32) __asm__("func_001F9E90");
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern f32 random_float_between_c2(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C;
extern char D_L00_0015F580[] __attribute__((section(".sdata")));
extern float D_L00_00161890_gp __asm__("D_L00_00161890") __attribute__((sda));
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_L00_00257b90_c2(int, int) __asm__("FUN_L00_00257b90");
extern s32 scale_game_frames_c2(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern float D_L00_00161870 __attribute__((sda));
extern float D_L00_00161874 __attribute__((sda));
extern float D_L00_00161878 __attribute__((sda));
extern float D_L00_00161888 __attribute__((sda));
extern unsigned char *spawn_random_size_rotated_particle(char *a, char *b, int c, float f) __asm__("FUN_L00_00275158");
extern unsigned char D_0013E05A[];
extern unsigned char D_L00_00173E80[];
extern void *FUN_L00_0026dd20(void *, void *, unsigned, int, float);
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_002cbcc0_c(unsigned char *, int) __asm__("FUN_L00_002cbcc0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
#include "rnc/gameplay/state/item_state.h"

char *FUN_L00_002caf70(char *owner, char *spawn_pos, char *target, float vx, float vy, float yaw, float pitch) {
    float start_position[4];
    float probe[4];
    float direction[4];
    float scaled[4];
    float spark[4];
    unsigned char *moby;
    char *vars;
    char *trail;
    unsigned char *variant;
    int result;
    int timer;
    int lifetime;
    int i;
    int sparks;
    float length;
    float blend;
    float index;
    float fraction;
    float *start;

    start = start_position;
    *(w128 *)start_position = *(w128 *)spawn_pos;
    moby = func_0020D348_m(0x131);
    if (moby != 0) {
        vars = *(char **)(moby + 0x78);
        *(char **)(vars + 0x20) = owner;
        moby[0x30] = 0x7F;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x31] = 1;
        *(long *)(moby + 0x38) = *(long *)(owner + 0x38);
        *(float *)(moby + 0x40) = D_L00_00161890_gp;
        *(float *)(moby + 0x44) = fast_add_rotations(pitch, (&D_L00_00161890_gp)[1]);
        *(float *)(moby + 0x48) = yaw;
        qcopy(moby + 0x10, start);
        if (target != 0) {
            int bounds;
            qcopy(vars, target + 0x10);
            bounds = (int)suck_get_aim_height(target);
            if (bounds != 0) {
                *(float *)(vars + 0x8) = *(float *)(vars + 0x8) + *(float *)(bounds + 0x10);
            } else {
                *(float *)(vars + 0x8) = *(float *)(vars + 0x8) + 0.5f;
            }
        } else {
            clear_u64_value_c(vars);
        }
        *(float *)(vars + 0x18) = vx;
        *(float *)(vars + 0x1C) = vy;
        *(int *)(vars + 0x10) = 0;
        *(int *)(vars + 0x14) = 0;
        timer = scale_game_frames_c2(0x23);
        variant = item_text_variant;
        timer = timer + variant[0xF] * 10;
        *(int *)(vars + 0x28) = 0;
        *(short *)(vars + 0x24) = timer;
        *(short *)(vars + 0x26) = timer;
        *(w128 *)probe = *(w128 *)(owner + 0x10);
        probe[2] = start[2];
        result = collision_line(probe, start, 0, (int)owner, 0);
        if (result != 0) {
            build_spherical_offset(direction, D_0015ED6C * 40.0f, *(float *)(moby + 0x48), -*(float *)(moby + 0x44));
            qcopy(moby + 0x10, D_L00_00173E60_c);
            start = scaled;
            sparks = 4;
            do {
                *(w128 *)spark = 0;
                spark[0] = random_float_between_c2(-1.0f, 1.0f);
                sparks--;
                spark[1] = random_float_between_c2(-1.0f, 1.0f);
                spark[2] = random_float_between_c2(-1.0f, 1.0f);
                *(w128 *)scaled = *(w128 *)spark;
                FUN_L00_001ff660(spark, direction, D_L00_00173E80);
                length = fast_vec_length(spark);
                fast_vec_normalize(start, start, length * 0.5f);
                add_vector_xyz(spark, spark, start);
                fast_vec_normalize(spark, spark, random_float_between_c2(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                result = scale_game_frames_c2(10);
                timer = scale_game_frames_c2(15);
                lifetime = FUN_L00_00257b90_c2(result, timer);
                FUN_L00_0026dd20(moby + 0x10, (char *)spark, 0x7F2F4F6F, lifetime, 30000.0f);
            } while (sparks >= 0);
            variant = item_text_variant;
            if (variant[0xF] == 0) {
                FUN_L00_002cbcc0_c(moby, 0);
                mark_moby_for_removal_c2(moby);
                return 0;
            }
            *(int *)(vars + 0x28) = *(int *)(vars + 0x28) + 1;
            if (variant[0xF] + 5 < *(int *)(vars + 0x28)) {
                FUN_L00_002cbcc0_c(moby, 1);
                mark_moby_for_removal_c2(moby);
                return 0;
            }
            FUN_L00_001ff660(direction, direction, D_L00_00173E80);
            *(float *)(moby + 0x48) = fast_arc_tan_c2(direction[0], direction[1]);
            length = vector_length_xy(direction);
            *(float *)(moby + 0x44) = -fast_arc_tan_c2(length, direction[2]);
            return moby;
        } else {
            spawn_source_attached_fading_effect_particle(moby, 0x2F4F7F7F, *(short *)(vars + 0x24), -1, D_L00_00161888);
            for (i = 0; i < 17; i++) {
                trail = (char *)spawn_random_size_rotated_particle(moby + 0x10, D_L00_0015F580, 1, 0.0f);
                if (trail != 0) {
                    *(float *)(trail + 0xC) = random_float_between_c2(D_L00_00161870, D_L00_00161874);
                    index = convert_integer_to_float_c(i);
                    fraction = index / convert_integer_to_float_c(16);
                    blend = cubic_hermite_scalar(-1.5f, 0.0f, 1.0f, 0.0f, fraction);
                    *(float *)(trail + 0xC) = *(float *)(trail + 0xC) + (D_L00_00161878 - *(float *)(trail + 0xC)) * blend;
                    result = truncate_float_to_s32_c(blend * -64.0f + 128.0f);
                    *(int *)(trail + 4) = (result << 24) | 0xFFFFFF;
                    ((int *)(vars + 0x2C))[i] = (int)trail;
                } else {
                    ((int *)(vars + 0x2C))[i] = 0;
                }
            }
        }
    }
    return moby;
}
/* Ported probe rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC868), where it is exact; names translated to the US level program. */



/* Vars of the homing shot FUN_L00_002caf70 spawns. */
typedef struct {
    f32 target_x, target_y, target_z;
    f32 unk0C;
    f32 yaw_rate;
    f32 pitch_rate;
    f32 rest_yaw;
    f32 rest_pitch;
    s32 ignore;
    s16 timer;
    s16 life;
    s32 bounces;
    Moby *follower;
    Moby *trail[16];
} ShotVars_2cb418;

typedef struct {
    Vec4A dir;
    Moby *moby;
    s32 flags;
    u8 b18;
    u8 b19;
    s16 oclass;
    f32 radius;
    s32 i20;
} ShotQuery_2cb418;

extern u8 D_0013E520_2cb418[] __asm__("D_0013E520");
extern unsigned char D_L00_00173E60[];      /* +0x20: hit normal */
extern char D_L00_00166DC0_2cb418[] __asm__("D_L00_00166DC0");
extern char D_L00_00166FF0_2cb418[] __asm__("D_L00_00166FF0");
extern char D_0013F3D0[];
extern Moby *D_001403E0_2cb418 __asm__("D_001403E0") __attribute__((section(".data")));
/* D_L00_00161894 again, through the symbol below it: retail's second load stays lui */
extern float D_0015ED70_lui __asm__("D_0015ED70");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern void FUN_L00_002cbcc0_2cb418(Moby *, s32) __asm__("FUN_L00_002cbcc0");
extern f32 vector_distance_2cb418(void *, void *) __asm__("FUN_001f9b80");
extern s32 random_int_between_2cb418(s32, s32) __asm__("FUN_L00_00257b90");
extern void spawn_spark_2cb418(void *, void *, s32, s32, f32) __asm__("FUN_L00_0026dd20");
extern void FUN_001f9c48_2cb418(void *, void *, f32) __asm__("FUN_001f9c48");
extern void normalize_vector_xyz_2cb418(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 atan2_f_2cb418(f32, f32) __asm__("FUN_001f9e90");
extern f32 turn_toward_2cb418(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern void build_spherical_offset_2cb418(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void reflect_vector_2cb418(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern f32 random_float_between_2cb418(f32, f32) __asm__("FUN_002132a8");
extern void rotate_vector_2cb418(void *, void *, void *, f32) __asm__("FUN_00214890");
extern void scale_vector_xyz_2cb418(void *, void *, f32) __asm__("FUN_001f9a68");
extern void cross_vectors_xyz_2cb418(void *, void *, void *) __asm__("FUN_001f9ad8");
extern f32 vector_length_xy_2cb418(void *) __asm__("FUN_001f9b20");
extern void play_sound_2cb418(s32, s32, Moby *) __asm__("FUN_0022da68");
extern f32 int_to_float_2cb418(s32) __asm__("FUN_001fa6c0");
extern s32 random_integer_below_2cb418(s32) __asm__("FUN_00213260");
extern f32 random_float_2cb418(f32, f32) __asm__("FUN_L00_00257c48");
extern s32 tick_timer_2cb418(void *) __asm__("FUN_001f9770");
extern s32 collide_line_2cb418(void *, void *, s32, s32, void *) __asm__("FUN_001efa68");
extern s32 FUN_001f0b58_2cb418(void) __asm__("FUN_001f0b58");
extern s32 scale_game_frames_2cb418(s32) __asm__("FUN_001f96f8");
extern f32 fast_add_rotations_2cb418(f32, f32) __asm__("FUN_001fa580");
extern f32 vector_length_xyz_2cb418(void *) __asm__("FUN_001f9af0");
extern void add_vector_xyz_2cb418(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_2cb418(void *, void *, void *) __asm__("FUN_001f9a28");
extern void mark_moby_for_removal_2cb418(Moby *) __asm__("FUN_0020c828");

extern f32 D_L00_00161894[2];
extern f32 D_L00_00161890[2];
extern f32 jitter __asm__("D_L00_0016187C") __attribute__((sda));
extern f32 trail_speed __asm__("D_L00_00161880") __attribute__((sda));
extern f32 trail_decay __asm__("D_L00_00161884") __attribute__((sda));
void update_homing_shot(Moby *shot_moby) __asm__("FUN_L00_002cb418");

void update_homing_shot(Moby *shot_moby) {
    Vec4A start;
    Vec4A step;
    ShotQuery_2cb418 query;
    Vec4A vector_scratch;
    Vec4A spark_and_pushout_scratch;
    Vec4A spark_velocity;
    ShotVars_2cb418 *vars;
    float yaw;
    float pitch;
    float speed;
    float spark_direction_length;
    int i;
    int j;
    int left;

    vars = *(ShotVars_2cb418 **)&shot_moby->pvars;
    shot_moby->rot.y = fast_add_rotations_2cb418(shot_moby->rot.y, -D_L00_00161894[0]);
    if (vector_length_xyz_2cb418(vars) < 0.1f) {
        if (vars->timer < scale_game_frames_2cb418(0x23) - scale_game_frames_2cb418(1) && vars->bounces == 0) {
            yaw = vars->rest_yaw;
            pitch = vars->rest_pitch;
        } else {
            yaw = shot_moby->rot.z;
            pitch = shot_moby->rot.y;
        }
    } else {
        yaw = atan2_f_2cb418(vars->target_x - shot_moby->pos.x, vars->target_y - shot_moby->pos.y);
        pitch = -atan2_f_2cb418(vector_distance_2cb418((Vec4A *)&shot_moby->pos, vars), vars->target_z - shot_moby->pos.z);
    }
    if (vector_length_xyz_2cb418(vars) < 0.1f) {
        shot_moby->rot.z = turn_toward_2cb418(&vars->yaw_rate, shot_moby->rot.z, yaw, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 6.2831855f);
        shot_moby->rot.y = turn_toward_2cb418(&vars->pitch_rate, shot_moby->rot.y, pitch, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 6.2831855f);
    } else {
        shot_moby->rot.z = turn_toward_2cb418(&vars->yaw_rate, shot_moby->rot.z, yaw, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 4.712389f);
        shot_moby->rot.y = turn_toward_2cb418(&vars->pitch_rate, shot_moby->rot.y, pitch, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 4.712389f);
    }
    build_spherical_offset_2cb418(&step, D_0015ED6C_lui * 40.0f, shot_moby->rot.z, -shot_moby->rot.y);
    qcopy(&start, (Vec4A *)&shot_moby->pos);
    add_vector_xyz_2cb418((Vec4A *)&shot_moby->pos, (Vec4A *)&shot_moby->pos, &step);
    if (shot_moby->pos.x < 0.0f || shot_moby->pos.y < 0.0f || shot_moby->pos.z < 0.0f
        || vector_distance_2cb418((Vec4A *)&shot_moby->pos, D_L00_00166DC0_2cb418) > 64.0f) {
        FUN_L00_002cbcc0_2cb418(shot_moby, 0);
        mark_moby_for_removal_2cb418(shot_moby);
        return;
    }
    query.flags = 0x10001;
    query.radius = 0.25f;
    query.moby = shot_moby;
    query.i20 = 1;
    subtract_vector_xyz_2cb418(&query, (Vec4A *)&shot_moby->pos, D_0013F3D0);
    FUN_001f9c48_2cb418(&query, &query, 1.0f);
    query.dir.z = 1.0f;
    query.b18 = 1;
    query.b19 = 1;
    query.dir.w = 5627.9248f;
    query.oclass = shot_moby->oclass;
    if (tick_timer_2cb418(&vars->timer) != 0) {
        FUN_L00_002cbcc0_2cb418(shot_moby, 0);
        mark_moby_for_removal_2cb418(shot_moby);
        return;
    }
    if (collide_line_2cb418(&start, (Vec4A *)&shot_moby->pos, 0, vars->ignore, &query) != 0) {
        if (FUN_001f0b58_2cb418() == 0) {
            if (vars->timer > scale_game_frames_2cb418(3)) {
                vars->timer = scale_game_frames_2cb418(3);
            }
        } else {
            qcopy((Vec4A *)&shot_moby->pos, ((char *)D_L00_00173E60));
            for (i = 0; i < 5; i++) {
                *(u128 *)&spark_and_pushout_scratch = 0;
                spark_and_pushout_scratch.x = random_float_between_2cb418(-1.0f, 1.0f);
                spark_and_pushout_scratch.y = random_float_between_2cb418(-1.0f, 1.0f);
                spark_and_pushout_scratch.z = random_float_between_2cb418(-1.0f, 1.0f);
                *(u128 *)&vector_scratch = *(u128 *)&spark_and_pushout_scratch;
                reflect_vector_2cb418(&spark_and_pushout_scratch, &step, ((char *)D_L00_00173E60) + 0x20);
                normalize_vector_xyz_2cb418(&vector_scratch, &vector_scratch, vector_length_xyz_2cb418(&spark_and_pushout_scratch) * 0.5f);
                add_vector_xyz_2cb418(&spark_and_pushout_scratch, &spark_and_pushout_scratch, &vector_scratch);
                normalize_vector_xyz_2cb418(&spark_and_pushout_scratch, &spark_and_pushout_scratch, random_float_between_2cb418(D_0015ED6C_lui * 3.0f, D_0015ED6C_lui * 6.0f));
                spawn_spark_2cb418((Vec4A *)&shot_moby->pos, &spark_and_pushout_scratch, 0x7F2F4F6F, random_int_between_2cb418(scale_game_frames_2cb418(10), scale_game_frames_2cb418(15)), 30000.0f);
            }
            if (D_0013E520_2cb418[15] == 0) {
                FUN_L00_002cbcc0_2cb418(shot_moby, 0);
                mark_moby_for_removal_2cb418(shot_moby);
                return;
            }
            vars->bounces++;
            switch (FUN_001f0b58_2cb418()) {
            case 0:
            case 1:
            case 3:
            case 4:
            case 7:
            case 11:
            case 13:
                vars->bounces = 1000;
                break;
            }
            if (D_0013E520_2cb418[15] + 5 < vars->bounces) {
                FUN_L00_002cbcc0_2cb418(shot_moby, 1);
                mark_moby_for_removal_2cb418(shot_moby);
                return;
            }
            reflect_vector_2cb418(&vector_scratch, &step, ((char *)D_L00_00173E60) + 0x20);
            shot_moby->rot.z = atan2_f_2cb418(vector_scratch.x, vector_scratch.y);
            shot_moby->rot.y = -atan2_f_2cb418(vector_length_xy_2cb418(&vector_scratch), vector_scratch.z);
            vars->yaw_rate = 0.0f;
            vars->pitch_rate = 0.0f;
            normalize_vector_xyz_2cb418(&spark_and_pushout_scratch, ((char *)D_L00_00173E60) + 0x20, 0.05f);
            add_vector_xyz_2cb418((Vec4A *)&shot_moby->pos, ((char *)D_L00_00173E60), &spark_and_pushout_scratch);
            if (D_001403E0_2cb418 != 0 && D_001403E0_2cb418->oclass == 0xA8
                && D_001403E0_2cb418->state != 0xFE && D_001403E0_2cb418->state != 0xFD) {
                play_sound_2cb418(4, 0, D_001403E0_2cb418);
            }
        }
    }
    shot_moby->rot.y = fast_add_rotations_2cb418(shot_moby->rot.y, D_L00_00161890[1]);
    left = vars->life - vars->timer;
    if (left < scale_game_frames_2cb418(13)) {
        speed = int_to_float_2cb418(left);
        speed = speed / int_to_float_2cb418(scale_game_frames_2cb418(13));
        speed = speed * trail_speed;
    } else {
        speed = trail_speed;
    }
    subtract_vector_xyz_2cb418(&step, &start, (Vec4A *)&shot_moby->pos);
    normalize_vector_xyz_2cb418(&step, &step, speed);
    qcopy(&vector_scratch, (Vec4A *)&shot_moby->pos);
    if (vars->follower != 0) {
        qcopy((Vec4A *)&vars->follower->pos, &vector_scratch);
    }
    for (j = 0; j < 16; j++) {
        Moby *trail_moby = vars->trail[j];
        if (trail_moby != 0) {
            add_vector_xyz_2cb418((Vec4A *)&trail_moby->pos, &vector_scratch, &step);
            qcopy(&vector_scratch, (Vec4A *)&trail_moby->pos);
            trail_moby->pos.x = trail_moby->pos.x + random_float_2cb418(0.0f, jitter);
            trail_moby->pos.y = trail_moby->pos.y + random_float_2cb418(0.0f, jitter);
            trail_moby->pos.z = trail_moby->pos.z + random_float_2cb418(0.0f, jitter);
            scale_vector_xyz_2cb418(&step, &step, trail_decay);
        }
    }
    if (random_integer_below_2cb418(5) != 0) {
        return;
    }
    cross_vectors_xyz_2cb418(&spark_and_pushout_scratch, &step, D_L00_00166FF0_2cb418);
    rotate_vector_2cb418(&spark_and_pushout_scratch, &spark_and_pushout_scratch, &step, random_float_2cb418(0.0f, 3.1415927f));
    spark_direction_length = vector_length_xyz_2cb418(&spark_and_pushout_scratch);
    if (spark_direction_length == 0.0f) {
        return;
    }
    scale_vector_xyz_2cb418(&spark_velocity, &spark_and_pushout_scratch, random_float_between_2cb418(D_0015ED6C_lui * 0.5f, D_0015ED6C_lui + D_0015ED6C_lui) / spark_direction_length);
    spawn_spark_2cb418((Vec4A *)&shot_moby->pos, &spark_velocity, 0x7F2F4F6F, random_int_between_2cb418(scale_game_frames_2cb418(10), scale_game_frames_2cb418(15)), 30000.0f);
}
extern void FUN_00267a08_2cbcc0(void *) __asm__("FUN_L00_00267a08");
extern f32 FUN_L00_00257be8_2cbcc0(f32, f32) __asm__("FUN_L00_00257be8");
extern s32 FUN_L00_00257b90_2cbcc0(s32, s32) __asm__("FUN_L00_00257b90");
extern s32 FUN_L00_00257b50_2cbcc0(s32) __asm__("FUN_L00_00257b50");
extern s32 FUN_001fef20_2cbcc0(s32) __asm__("FUN_L00_001fef20");
extern void FUN_001ff500_2cbcc0(void *, void *, f32) __asm__("FUN_L00_001ff500");
extern void *FUN_0026dd20_2cbcc0(void *, void *, u32, s32, f32) __asm__("FUN_L00_0026dd20");
extern u8 *FUN_0026d000_2cbcc0(f32, f32, f32, f32, void *, s32, void *, u32) __asm__("FUN_L00_0026d000");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern float D_0015ED60;

void FUN_L00_002cbcc0(u8 *shot_moby, s32 enabled)
{
    s32 i;
    s32 j;
    void **handle;
    u8 *shot_position;
    V4 vectors[2];
    s32 lifetime;
    s32 color;
    s32 sign;
    u8 *effect;
    s32 alpha;

    handle = (void **)(*(u8 **)(shot_moby + 0x78) + 0x2c);
    for (i = 16; i >= 0; i--) {
        if (*handle != 0)
            FUN_00267a08_2cbcc0(*handle);
        *handle++ = 0;
    }
    shot_position = shot_moby + 0x10;

    if (enabled != 0) {
        V4 *vector = &vectors[1];
        for (j = 4; j >= 0; j--) {
            vectors[1].q = 0;
            vectors[1].f[0] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[1].f[1] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[1].f[2] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[0].q = vectors[1].q;
            FUN_001ff500_2cbcc0(vector, &vectors[0],
                FUN_L00_00257be8_2cbcc0(D_0015ED6C_lui * 3.0f, D_0015ED6C_lui * 6.0f));
            lifetime = FUN_L00_00257b90_2cbcc0(FUN_001fef20_2cbcc0(10), FUN_001fef20_2cbcc0(15));
            FUN_0026dd20_2cbcc0(shot_position, vector, 0x7f2f4f6f, lifetime, 35000.0f);
        }
    }

    {
        s32 j;
        for (j = 2; j >= 0; j--) {
            color = ((u32)FUN_L00_00257b90_2cbcc0(0x20, 0x80) << 24) | 0x606060;
            sign = FUN_L00_00257b50_2cbcc0(2) ? -1 : 1;
            vectors[0].q = 0;
            vectors[0].f[2] = 1.0f;
            vectors[0].f[3] = 1.0f;
            vectors[0].f[2] = FUN_L00_00257be8_2cbcc0(0.01f, 0.025f) * D_0015ED60;
            effect = FUN_0026d000_2cbcc0(0.2f, 1.0f, 1.02f, 140000.0f,
                shot_position, sign, &vectors[0], color);
            if (effect != 0) {
                *(s16 *)(effect + 0xa) = FUN_001fef20_2cbcc0(FUN_L00_00257b90_2cbcc0(50, 80));
                /* The direction is no longer needed; retain the effect field address. */
                i = (s32)(effect + 0x20);
                *(s32 *)(i + 4) = 2;
                ((u8 *)i)[0xa] = color >> 24;
                ((u8 *)i)[0xb] = effect[0xa];
                alpha = FUN_L00_00257b50_2cbcc0(2);
                if (alpha) {
                    /* The fixed low color byte is 0x60, so the alpha byte is 0x44. */
                    alpha = 0x44;
                    effect[3] = alpha;
                }
            }
        }
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern unsigned char D_0013E535_2cbf68 __asm__("D_0013E535") __attribute__((section(".data")));
unsigned char *FUN_0020c4f8_2cbf68(int) __asm__("FUN_0020c4f8");
float rndf_2cbf68(float, float) __asm__("FUN_002132a8");
float rnd_2cbf68(float, float) __asm__("FUN_L00_00257c48");
int FUN_001160d8_2cbf68(void) __asm__("FUN_001160d8");
float rnda_2cbf68(void) __asm__("FUN_00213308");
int rf_2cbf68(float) __asm__("FUN_001fa6d0");
int rndi_2cbf68(int, int) __asm__("FUN_L00_00257b90");
int fr_2cbf68(int) __asm__("FUN_001f96f8");
void FUN_L00_00250df8_2cbf68(void *) __asm__("FUN_L00_00250df8");
void FUN_L00_0025d1b8_2cbf68(void *) __asm__("FUN_L00_0025d1b8");

extern int D_L00_001618A4 __attribute__((sda));
extern float D_L00_001618A0 __attribute__((sda));
unsigned char *spawn_feather_moby(int unused, void *spawn_position, float *initial_velocity) {
    unsigned char *spawned_moby = FUN_0020c4f8_2cbf68(0x1AC);
    if (spawned_moby) {
        unsigned char *moby_vars;
        int lifetime_lower_bound, lifetime_upper_bound;
        spawned_moby[0x30] = 0x40;
        *(unsigned short *)(spawned_moby + 0x34) |= 0x1000;
        *(short *)(spawned_moby + 0x32) = 0x20;
        spawned_moby[0x31] = 1;
        moby_vars = *(unsigned char **)(spawned_moby + 0x78);
        spawned_moby[0x20] = 0;
        spawned_moby[0xBC] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        *(float *)(spawned_moby + 0x18) += rndf_2cbf68(0.2f, 0.7f);
        *(float *)(spawned_moby + 0x10) += initial_velocity[0] * 3.0f;
        *(float *)(spawned_moby + 0x14) += initial_velocity[1] * 3.0f;
        qcopy(moby_vars, initial_velocity);
        *(float *)(spawned_moby + 0x44) = rnd_2cbf68(0.17453292f, 0.5235988f);
        *(float *)(spawned_moby + 0x40) = rndf_2cbf68(0.17453292f, 0.7853982f);
        {
            float x_rotation_step = rndf_2cbf68(200.0f, 370.0f) * DEG_TO_RAD * D_0015ED6C_lui;
            *(int *)(moby_vars + 0x24) = -1;
            *(float *)(moby_vars + 0x2C) = x_rotation_step;
        }
        if (FUN_001160d8_2cbf68() & 1) {
            *(int *)(moby_vars + 0x24) = 1;
            *(float *)(spawned_moby + 0x44) = -*(float *)(spawned_moby + 0x44);
        }
        *(float *)(spawned_moby + 0x48) = rnda_2cbf68();
        *(float *)(moby_vars + 0x28) = -(rndf_2cbf68(200.0f, 550.0f) * DEG_TO_RAD * D_0015ED6C_lui);
        *(float *)(moby_vars + 0x10) = rnda_2cbf68();
        {
            float r = rnda_2cbf68();
            float base_lifetime = (float)D_L00_001618A4;
            *(float *)(moby_vars + 0x14) = r;
            lifetime_lower_bound = rf_2cbf68(base_lifetime * 0.7f);
        }
        lifetime_upper_bound = rf_2cbf68((float)D_L00_001618A4 * 1.2f);
        *(int *)(moby_vars + 0x1C) = fr_2cbf68(rndi_2cbf68(lifetime_lower_bound, lifetime_upper_bound));
        {
            float k = D_L00_001618A0;
            *(float *)(spawned_moby + 0x2C) *= k;
        }
        if (D_0013E535_2cbf68)
            *(float *)(spawned_moby + 0x2C) *= 2.0f;
        *(float *)(moby_vars + 0x20) = rndf_2cbf68(-1.2f, -0.4f) * D_0015ED6C_lui;
        FUN_L00_00250df8_2cbf68(spawned_moby);
        FUN_L00_0025d1b8_2cbf68(spawned_moby);
    }
    return spawned_moby;
}
extern float D_0015ED70_lui __asm__("D_0015ED70");
void FUN_001f9a10_2cc210(void *, void *, void *) __asm__("FUN_001f9a10");
float FUN_001fa580_2cc210(float, float) __asm__("FUN_001fa580");
float FUN_001f99c0_2cc210(float) __asm__("FUN_001f99c0");
int FUN_001f9740_2cc210(void *) __asm__("FUN_001f9740");
void FUN_0020c828_2cc210(void *) __asm__("FUN_0020c828");
int FUN_001f96f8_2cc210(int) __asm__("FUN_001f96f8");
int FUN_L00_001fefc8_2cc210(void *) __asm__("FUN_L00_001fefc8");
extern float D_L00_001618A8 __attribute__((sda));
void update_damped_falling_moby(unsigned char *updated_moby) __asm__("FUN_L00_002cc210");

void update_damped_falling_moby(unsigned char *updated_moby) {
    float *moby_vars = *(float **)(updated_moby + 0x78);
    float sc;
    FUN_001f9a10_2cc210(updated_moby + 0x10, updated_moby + 0x10, moby_vars);
    moby_vars[2] -= D_0015ED70_lui * 15.0f;
    sc = D_L00_001618A8;
    moby_vars[0] *= sc;
    moby_vars[1] *= sc;
    if (moby_vars[2] < moby_vars[8])
        moby_vars[2] = moby_vars[8];
    *(float *)(updated_moby + 0x48) = FUN_001fa580_2cc210(*(float *)(updated_moby + 0x48), moby_vars[10]);
    *(float *)(updated_moby + 0x40) = FUN_001fa580_2cc210(*(float *)(updated_moby + 0x40), moby_vars[11] * (float)((int *)moby_vars)[9]);
    if (0.7853982f < FUN_001f99c0_2cc210(*(float *)(updated_moby + 0x40))) {
        ((int *)moby_vars)[9] = -((int *)moby_vars)[9];
    }
    if (FUN_001f9740_2cc210(&moby_vars[7]) != 0 || updated_moby[0x31] == 0) {
        FUN_0020c828_2cc210(updated_moby);
    } else if (((int *)moby_vars)[7] < FUN_001f96f8_2cc210(0x1E) && FUN_L00_001fefc8_2cc210(updated_moby + 0x23)) {
        FUN_0020c828_2cc210(updated_moby);
    }
}
typedef unsigned int u128_2cc360 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2cc360 q;
    float f[4];
} V_2cc360;
typedef struct {
    char p0[0x46];
    short h46;
} C_2cc360;
typedef struct M_2cc360 {
    char p0[0x10];
    V_2cc360 pos;
    char p20[4];
    C_2cc360 *c;
    struct M_2cc360 *next;
    char p2c[5];
    unsigned char b31;
    short h32;
    unsigned short h34;
} M_2cc360;
extern M_2cc360 *D_L00_0015FFE4_2cc360 __asm__("D_L00_0015FFE4");
extern float *c338_2cc360(void *) __asm__("FUN_002141f8");
extern float dist_2cc360(void *, void *) __asm__("FUN_001f9b80");
extern float atan_2cc360(float, float) __asm__("FUN_001f9e90");
extern float diffrot_2cc360(float, float) __asm__("FUN_001fa688");
extern int coll_2cc360(void *, void *, int, void *, int) __asm__("FUN_001efa68");
M_2cc360 *FUN_L00_002cc360(float *pos, float *aim_rotation, M_2cc360 **excl, float maxYaw, float maxPitch,
                           float maxDist) {
    M_2cc360 *best = 0;
    float bestScore = 1.0e9f;
    M_2cc360 *candidate_moby;
    float *info;
    V_2cc360 aim_point;
    V_2cc360 *pp;
    float candidate_distance, ys, ps, score;
    int i, found;
    for (candidate_moby = D_L00_0015FFE4_2cc360; candidate_moby; candidate_moby = candidate_moby->next) {
        if (candidate_moby->h32 == 0)
            continue;
        if (excl) {
            found = 0;
            for (i = 0; i < 7; i++) {
                if (excl[i] == candidate_moby) {
                    found = 1;
                    break;
                }
            }
            if (found)
                continue;
        } else if (!candidate_moby->b31) {
            continue;
        }
        info = c338_2cc360(candidate_moby);
        if (!info)
            continue;
        if (info[0] < 0.0f)
            continue;
        if (!(candidate_moby->h34 & 0x1000))
            continue;
        if (!candidate_moby)
            continue;
        if (!candidate_moby->c)
            continue;
        if (candidate_moby->c->h46 != 5)
            continue;
        pp = &candidate_moby->pos;
        candidate_distance = dist_2cc360(pos, pp);
        if (!(candidate_distance < maxDist))
            continue;
        qcopy(&aim_point, pp);
        aim_point.f[2] += info[4] + 0.1f;
        ys = diffrot_2cc360(aim_rotation[2], atan_2cc360(aim_point.f[0] - pos[0], aim_point.f[1] - pos[1]));
        ys = ys * ys;
        if (ys < maxYaw * maxYaw) {
            ps = diffrot_2cc360(aim_rotation[1], atan_2cc360(candidate_distance, aim_point.f[2] - pos[2]));
            ps = ps * ps;
            if (!(ps < maxPitch * maxPitch))
                continue;
            if (5.0f < candidate_distance) {
                ys = ys * ps * candidate_distance;
                ys += candidate_distance;
            } else {
                ys = candidate_distance / 5.0f;
            }
            if (ys < bestScore) {
                if (coll_2cc360(pos, &aim_point, 2, candidate_moby, 0))
                    continue;
                bestScore = ys;
                best = candidate_moby;
            }
        }
    }
    return best;
}
#include "rnc/input/pad_state.h"

#include "rnc/gameplay/state/usage_stats.h"
typedef struct { float m[3][4]; } __attribute__((aligned(16))) Mat3_2cc608;
typedef struct { float m[4][4]; } __attribute__((aligned(16))) Mat4_2cc608;
typedef struct {
    struct Moby *target;
    unsigned char shots;
    unsigned char slot;
    short shot_timer;
    int lock_timer;
    float height;
    short track_timer;
    short count;
    float life;
    struct Moby *recent[7];
} Ryno_2cc608;
typedef struct { float radius; char pad4[0xC]; float height; } Bounds_2cc608;

extern char D_L00_00166C80[];
extern Vec4 camera_rot_2cc608 __asm__("D_L00_00166DD0");
extern float camera_pos_2cc608[] __asm__("D_L00_00166DC0");
extern int play_frames_2cc608 __asm__("D_0015EEA4") __attribute__((section(".sdata")));
extern int D_0015ED84 __attribute__((section(".sdata")));
extern struct UsageStats item_usage_2cc608 __asm__("D_00141680");
extern void matcopy_2cc608(void *, void *) __asm__("FUN_001fa2b8");
extern void transpose_2cc608(void *, void *) __asm__("FUN_001fa2d8");
extern int timer32_2cc608(int *) __asm__("FUN_001f9740");
extern int timer16_2cc608(short *) __asm__("FUN_001f9770");
extern void muzzle_2cc608(struct Moby *, int, float *) __asm__("FUN_L00_0024f7c8");
extern struct Moby *FUN_L00_002cc360_c(float *, float *, struct Moby **, float, float, float) __asm__("FUN_L00_002cc360");
extern int ticks_2cc608(int) __asm__("FUN_001f96f8");
extern Bounds_2cc608 *bounds_2cc608(struct Moby *) __asm__("FUN_002141f8");
extern int has_ammo_2cc608(int) __asm__("FUN_L00_00233e98");
extern int use_ammo_2cc608(int, int) __asm__("FUN_L00_00233db8");
extern float distance_2cc608(void *, void *) __asm__("FUN_001f9b48");
extern float fabs_2cc608(float) __asm__("FUN_001f99c0");
extern void reticle_2cc608(struct Moby *, int, float, float, float *, int, int, float, int) __asm__("FUN_L00_001edf60");
extern void hud_2cc608(int, int) __asm__("FUN_L00_002223f8");
extern void f_fun_l00_0020e4c8_227fa0(void) __asm__("FUN_L00_0020e4c8");
extern void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
extern void apply_mat4_2cc608(float *, float *, void *) __asm__("FUN_001f9d20");
extern void apply_mat3_2cc608(float *, float *, void *) __asm__("FUN_001f9cf8");
extern void add_2cc608(float *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_2cc608(float *, float *, float) __asm__("FUN_001f9a68");
extern float angle_2cc608(float, float) __asm__("FUN_001f9e90");
extern float length_2cc608(float *) __asm__("FUN_001f9b20");
extern unsigned char *FUN_L00_002ccee0_c(struct Moby *, float *, struct Moby *, float, float, float) __asm__("FUN_L00_002ccee0");
extern int rand_2cc608(void) __asm__("FUN_001160d8");

/* Ryno (class 454) update: target lock and reticle while held, then the eight-missile salvo state machine. */
void ryno_update(struct Moby *m) __asm__("FUN_L00_002cc608");
void ryno_update(struct Moby *ryno_moby) {
    enum {
        RYNO_STATE_INITIALIZE = 0,
        RYNO_STATE_WAIT_ANIMATION = 1,
        RYNO_STATE_READY_TO_FIRE = 2,
        RYNO_STATE_READY_TO_FIRE_WITH_RETICLE = 3,
        RYNO_STATE_FIRE_SALVO = 4,
        RYNO_STATE_COOLDOWN = 5
    };
    Mat3_2cc608 launch_direction_basis;
    Mat4_2cc608 view;
    Vec4 aim;
    Vec4 muzzle_position_and_launch_direction;
    Vec4 ofs;
    Ryno_2cc608 *ryno_vars = *(Ryno_2cc608 **)&ryno_moby->pvars;
    struct Moby *acquired_target;
    Bounds_2cc608 *target_bounds;
    float yaw, pitch;
    int i;

    matcopy_2cc608(&launch_direction_basis, D_L00_00166C80);
    transpose_2cc608(&view, &launch_direction_basis);
    if ((unsigned int)(ryno_moby->state - 2) < 2) {
        if (timer32_2cc608(&ryno_vars->lock_timer) || ryno_vars->target == 0 || ryno_vars->target->state == 0xFE || ryno_vars->target->state == 0xFD) {
            ryno_vars->target = 0;
            ryno_vars->height = 0;
        }
        qcopy(aim.f, camera_rot_2cc608.f);
        aim.f[1] = -aim.f[1];
        muzzle_2cc608(ryno_moby, ryno_vars->slot, muzzle_position_and_launch_direction.f);
        acquired_target = FUN_L00_002cc360_c(muzzle_position_and_launch_direction.f, aim.f, 0, 1.6981583f, 1.6981583f, 80.0f);
        if ((acquired_target != 0 && ryno_vars->target == 0) || ryno_vars->target == 0 || ryno_vars->target->state == 0xFE || ryno_vars->target->state == 0xFD || ryno_vars->target->visible == 0) {
            ryno_vars->height = 0;
            ryno_vars->target = acquired_target;
            ryno_vars->lock_timer = ticks_2cc608(20);
            ryno_vars->life = 1.0f;
            target_bounds = bounds_2cc608(ryno_vars->target);
            if (target_bounds != 0) {
                ryno_vars->height = target_bounds->height;
                ryno_vars->life = target_bounds->radius;
            }
        }
        if (ryno_vars->target != 0 && timer16_2cc608(&ryno_vars->track_timer) && has_ammo_2cc608(-1)) {
            if (ryno_vars->target == 0 || ryno_vars->target->state == 0xFE || ryno_vars->target->state == 0xFD) {
                ryno_vars->target = 0;
                ryno_vars->height = 0;
            } else {
                float f = distance_2cc608(camera_pos_2cc608, ((float *)&ryno_vars->target->pos)) / 150.0f;
                qcopy(aim.f, ((float *)&ryno_vars->target->pos));
                f = f * 0.65f;
                aim.f[2] = aim.f[2] + ryno_vars->height;
                reticle_2cc608(ryno_moby, 0xFF0FFF0F, fabs_2cc608(0.9f - f), 0.0f, aim.f, 0x23, -1, 90.0f, 4);
            }
        }
    }
    switch (ryno_moby->state) {
    case RYNO_STATE_INITIALIZE:
        ryno_vars->target = 0;
        hero.items[0].unk1C = 0;
        ryno_moby->state = 1;
        ryno_vars->track_timer = 0;
        break;
    case RYNO_STATE_WAIT_ANIMATION:
        if (ryno_moby->anim_flags & 2) ryno_moby->state = 2;
        break;
    case RYNO_STATE_READY_TO_FIRE:
        if (hero.state.current == 1) hud_2cc608(0x1E, 1);
        if ((controller_state.pressed & hero.items[0].button_mask) == 0) return;
        if (hero.unk20AC != 0) return;
        if (use_ammo_2cc608(-1, 1) == 0) return;
        {
            int per;
            struct UsageStats *s = &item_usage_2cc608;
            if (s->stat[23].count <= 0xFFFE) s->stat[23].count++;
            per = 600;
            if (ticks_2cc608(play_frames_2cc608) / per > s->stat[23].unk2) s->stat[23].unk2 = ticks_2cc608(play_frames_2cc608) / 600;
            s->stat[23].level_mask = s->stat[23].level_mask | (1 << D_0015ED84) | 0x80000000;
        }
        hero.items[0].unk1C = 2;
        ryno_moby->state = 4;
        ryno_vars->shot_timer = 0;
        ryno_vars->slot = 0;
        ryno_vars->shots = 0;
        f_fun_l00_0020e4c8_227fa0();
        {
            int k;
            for (k = 6; k >= 0; k--) ryno_vars->recent[k] = 0;
        }
        ryno_vars->count = 0;
        break;
    case RYNO_STATE_READY_TO_FIRE_WITH_RETICLE:
        reticle_2cc608(ryno_moby, 0xFF0F0FFF, 1.0f, 0.0f, 0, 0x23, -1, 90.0f, 4);
        if ((controller_state.held & 5) == 0) {
            ryno_moby->state = 2;
            return;
        }
        if ((controller_state.pressed & hero.items[0].button_mask) == 0) return;
        if (hero.unk20AC != 0) return;
        if (use_ammo_2cc608(-1, 1) == 0) return;
        {
            int per;
            struct UsageStats *s = &item_usage_2cc608;
            if (s->stat[23].count <= 0xFFFE) s->stat[23].count++;
            per = 600;
            if (ticks_2cc608(play_frames_2cc608) / per > s->stat[23].unk2) s->stat[23].unk2 = ticks_2cc608(play_frames_2cc608) / 600;
            s->stat[23].level_mask = s->stat[23].level_mask | (1 << D_0015ED84) | 0x80000000;
        }
        hero.items[0].unk1C = 2;
        ryno_moby->state = 4;
        ryno_vars->shot_timer = 0;
        ryno_vars->slot = 0;
        ryno_vars->shots = 0;
        f_fun_l00_0020e4c8_227fa0();
        {
            int k;
            for (k = 6; k >= 0; k--) ryno_vars->recent[k] = 0;
        }
        ryno_vars->count = 0;
        break;
    case RYNO_STATE_FIRE_SALVO:
        if (timer16_2cc608(&ryno_vars->shot_timer) == 0) return;
        ryno_vars->shot_timer = ticks_2cc608(9);
        ryno_vars->slot++;
        ryno_vars->slot = ryno_vars->slot % 9;
        ryno_vars->shots++;
        muzzle_position_and_launch_direction.q = 0;
        if (hero.state.current == 30) {
            muzzle_position_and_launch_direction.f[2] = 1.0f;
            apply_mat4_2cc608(muzzle_position_and_launch_direction.f, muzzle_position_and_launch_direction.f, &view);
        } else {
            muzzle_position_and_launch_direction.f[0] = 1.0f;
            apply_mat3_2cc608(muzzle_position_and_launch_direction.f, muzzle_position_and_launch_direction.f,  hero.basis);
        }
        add_2cc608(ofs.f, &hero.motion.displacement, &hero.motion.platform_carry);
        scale_2cc608(ofs.f, ofs.f, 3.0f);
        add_2cc608(muzzle_position_and_launch_direction.f, muzzle_position_and_launch_direction.f, ofs.f);
        if (hero.items[6].unk40 != 0) {
            yaw = hero.items[6].unk4C;
        } else {
            yaw = angle_2cc608(muzzle_position_and_launch_direction.f[0], muzzle_position_and_launch_direction.f[1]);
        }
        pitch = -angle_2cc608(length_2cc608(muzzle_position_and_launch_direction.f), muzzle_position_and_launch_direction.f[2]);
        muzzle_2cc608(ryno_moby, ryno_vars->slot, aim.f);
        f_fun_l00_0020e4c8_227fa0();
        FUN_L00_002ccee0_c(ryno_moby, aim.f, ryno_vars->target, ryno_vars->height, yaw, pitch);
        if (ryno_vars->shots >= 7) {
            hero.items[0].unk1C = 0;
            cancel_hero_item_animation();
            ryno_moby->state = 5;
            ryno_vars->shot_timer = ticks_2cc608(60);
            ryno_vars->track_timer = ticks_2cc608(60);
        }
        ryno_vars->life = ryno_vars->life - 1.0f;
        if (ryno_vars->life <= 0.0f || ryno_vars->target == 0 || ryno_vars->target->state == 0xFE || ryno_vars->target->state == 0xFD) {
            ryno_vars->recent[ryno_vars->count] = ryno_vars->target;
            ryno_vars->count++;
            ryno_vars->count = ryno_vars->count % 7;
            qcopy(aim.f, camera_rot_2cc608.f);
            aim.f[1] = -aim.f[1];
            ryno_vars->target = FUN_L00_002cc360_c(((float *)&ryno_moby->pos), aim.f, ryno_vars->recent, 3.1415927f, 3.1415927f, 100.0f);
            if (ryno_vars->target == 0 && ryno_vars->count > 0 && (rand_2cc608() & (ryno_vars->count + 1)) == 0) {
                ryno_vars->target = ryno_vars->recent[rand_2cc608() % ryno_vars->count];
            }
            ryno_vars->height = 0;
            ryno_vars->lock_timer = ticks_2cc608(20);
            ryno_vars->life = 1.0f;
            target_bounds = bounds_2cc608(ryno_vars->target);
            if (target_bounds != 0) {
                ryno_vars->height = target_bounds->height;
                ryno_vars->life = target_bounds->radius;
            }
        }
        break;
    case RYNO_STATE_COOLDOWN:
        if (timer16_2cc608(&ryno_vars->shot_timer) == 0) return;
        ryno_vars->shots = 0;
        ryno_vars->target = 0;
        ryno_moby->state = 2;
        break;
    case 6:
    case 7:
    case 8:
        break;
    }
}
typedef union {
    OvlQuad q;
    float f[4];
} V_2ccee0;
extern float D_0015ED60;
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern unsigned char *D_001413D0_2ccee0 __asm__("D_001413D0") __attribute__((section(".data")));
extern unsigned char D_L00_00173E60_2ccee0[] __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
extern unsigned char *FUN_0020c4f8_2ccee0(int) __asm__("FUN_0020c4f8");
extern unsigned char *FUN_002141f8_2ccee0(void *) __asm__("FUN_002141f8");
extern int FUN_001f96f8_2ccee0(int) __asm__("FUN_001f96f8");
extern float FUN_002132a8_2ccee0(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_00250df8_2ccee0(void *) __asm__("FUN_L00_00250df8");
extern void FUN_0022da68_2ccee0(int, int, void *) __asm__("FUN_0022da68");
extern int FUN_001efa68_2ccee0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void FUN_00214db0_2ccee0(void *, float, float, float) __asm__("FUN_00214db0");
extern void FUN_L00_001ff660_2ccee0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_001f9bf8_2ccee0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_0025e450_2ccee0(void *, void *, void *, float, float, int, int, int, float,
                                    float, float, float, int, float, int, int, int,
                                    int) __asm__("FUN_L00_0025e450");
extern void FUN_0020c828_2ccee0(void *) __asm__("FUN_0020c828");

unsigned char *FUN_L00_002ccee0(int owner_moby, V_2ccee0 *spawn_position, unsigned char *target_moby, float target_height_offset, float yaw,
                                float pitch) {
    V_2ccee0 forward_step;
    V_2ccee0 collision_start;
    V_2ccee0 explosion_direction;
    V_2ccee0 z;
    unsigned char *spawned_moby;
    unsigned char *rocket_vars;
    unsigned char *target_aim_data;
    unsigned char *collision_ignore_moby;
    float random_range_limit;
    float f;

    spawned_moby = FUN_0020c4f8_2ccee0(0x1C9);
    if (spawned_moby != 0) {
        rocket_vars = *(unsigned char **)(spawned_moby + 0x78);
        spawned_moby[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        *(float *)(spawned_moby + 0x48) = yaw;
        *(float *)(spawned_moby + 0x44) = pitch;
        spawned_moby[0x20] = 0;
        *(int *)(rocket_vars + 0x20) = owner_moby;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(rocket_vars + 0x10, spawn_position);
        *(int *)(rocket_vars + 0x34) = 0;
        *(int *)(rocket_vars + 0x38) = 0;
        *(int *)(rocket_vars + 0x30) = 0;
        *(unsigned char **)(rocket_vars + 0x28) = target_moby;
        *(float *)(spawned_moby + 0x2C) = *(float *)(*(unsigned char **)(spawned_moby + 0x24) + 0x24) * 0.25f;
        if (target_moby != 0) {
            *(int *)(rocket_vars + 0x58) = *(short *)(target_moby + 0xA6);
            target_aim_data = FUN_002141f8_2ccee0(target_moby);
            if (target_aim_data != 0) {
                *(unsigned short *)(target_aim_data + 0x1E) |= 0x80;
            }
            qcopy(rocket_vars, target_moby + 0x10);
            *(float *)(rocket_vars + 0x2C) = target_height_offset;
            *(float *)(rocket_vars + 8) += target_height_offset;
        }
        *(int *)(rocket_vars + 0x24) = FUN_001f96f8_2ccee0(0xFA);
        *(int *)(rocket_vars + 0x44) = 0;
        *(float *)(rocket_vars + 0x48) = FUN_002132a8_2ccee0(-3.1415927f, 3.1415927f);
        f = FUN_002132a8_2ccee0(-3.1415927f, 3.1415927f);
        random_range_limit = D_0015ED60;
        random_range_limit *= 0.00967f;
        *(float *)(rocket_vars + 0x4C) = f;
        f = FUN_002132a8_2ccee0(-random_range_limit, random_range_limit);
        random_range_limit = D_0015ED60;
        random_range_limit *= 0.02406f;
        *(float *)(rocket_vars + 0x50) = f;
        *(float *)(rocket_vars + 0x54) = FUN_002132a8_2ccee0(-random_range_limit, random_range_limit);
        FUN_L00_00250df8_2ccee0(spawned_moby);
        FUN_0022da68_2ccee0(1, 0, spawned_moby);
        z.q = 0;
        z.f[2] = D_0015ED6C_lui * 8.0f;
        collision_ignore_moby = D_001413D0_2ccee0;
        explosion_direction.q = z.q;
        collision_start.q = ((V_2ccee0 *)(collision_ignore_moby + 0x10))->q;
        collision_start.f[2] = spawn_position->f[2];
        if (FUN_001efa68_2ccee0(&collision_start, spawn_position, 0, collision_ignore_moby, 0)) {
            spawned_moby[0xBC] = 2;
            qcopy(spawned_moby + 0x10, D_L00_00173E60_2ccee0);
            FUN_00214db0_2ccee0(&forward_step, *(float *)(rocket_vars + 0x30), *(float *)(spawned_moby + 0x48),
                                -*(float *)(spawned_moby + 0x44));
            FUN_L00_001ff660_2ccee0(&explosion_direction, &forward_step, D_L00_00173E60_2ccee0 + 0x20);
            FUN_001f9bf8_2ccee0(&explosion_direction, &explosion_direction, D_0015ED6C_lui + D_0015ED6C_lui);
            FUN_L00_0025e450_2ccee0(spawned_moby, &explosion_direction, 0, 0.0f, 0.0f, 10, 3, 9, 4.0f, 2.0f, 9.0f, 1.0f, 0,
                                    15.0f, 1, 1, -1, 0);
            FUN_0020c828_2ccee0(spawned_moby);
            return 0;
        }
    }
    return spawned_moby;
}
/* Rocket moby update: steers the rocket toward its target, moves it with collision retries, then checks hits and range. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CE6A0), where it is exact; names translated to the US level program. */

typedef int u128_cd __attribute__((mode(TI)));

typedef struct { float f[4]; } __attribute__((aligned(16))) V4_cd;

typedef struct { float x, y, z, w; char *p; int c; unsigned char b18; unsigned char b19; unsigned short s; float e; int i; } RocketHit;

extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED60;
extern float D_0015ED64;
extern float D_0015ED6C;
extern float D_0015ED70;
extern float FUN_L00_001ff430(void *);
extern float FUN_L00_00258110(float *vel, float cur, float target, float k, float d, float max);
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern Data D_L00_00173E40;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_L00_00257b90_c(int, int) __asm__("FUN_L00_00257b90");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 rand(void);
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
void spawn_explosion(char *m, void *unused, char *pos, float fA, float fB, int n, int k8, int k9, float fC, float fD, float fE, float fF, int k10, float fG, int k11, int s16, int s18, unsigned char flag) __asm__("FUN_L00_0025e450");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void rocket_update(char *m) __asm__("FUN_L00_002cd1f0");
void rocket_update(char *rocket_moby) {
    char *rocket_vars;
    char *target_moby;
    char *p;
    char *gg;
    V4_cd previous_position;
    V4_cd explosion_direction;
    V4_cd z;
    V4_cd forward_step;
    union { struct { V4_cd a; V4_cd b; V4_cd c; } v; RocketHit h; } u;
    V4_cd intercept_direction;
    V4_cd va0[4];
    int steering_timer_threshold;
    unsigned char flag;
    float selected_intercept_root;
    float s;
    float k;
    float k60;
    float a;
    float intercept_angle_scratch;
    float sp;
    float zf;

    if (rocket_moby == 0) return;
    rocket_vars = *(char **)(rocket_moby + 0x78);
    if (rocket_vars == 0) return;

    a = *(float *)(rocket_moby + 0x2C);
    k = D_0015ED6C;
    *(u128_cd *)&z = 0;
    z.f[2] = k * 8.0f;
    *(u128_cd *)&explosion_direction = *(u128_cd *)&z;
    k60 = D_0015ED60;
    {
        float k64 = D_0015ED64;
        *(float *)(rocket_moby + 0x2C) = a + (*(float *)(*(char **)(rocket_moby + 0x24) + 0x24) - a) * (k60 * 0.05f);
        {
            float e = *(float *)(rocket_vars + 0x30);
            *(float *)(rocket_vars + 0x30) = e + (k * 20.0f - e) * (k60 * 0.1f) * k64;
        }
    }
    steering_timer_threshold = scale_game_frames_c(250);
    steering_timer_threshold -= scale_game_frames_c(5);
    if (*(int *)(rocket_vars + 0x24) < steering_timer_threshold) {
        target_moby = *(char **)(rocket_vars + 0x28);
        if (target_moby == 0 || *(short *)(target_moby + 0xA6) != *(int *)(rocket_vars + 0x58) || *(unsigned char *)(target_moby + 0x20) == 0xFE || *(unsigned char *)(target_moby + 0x20) == 0xFD) {
            *(float *)(rocket_vars + 0x44) = *(float *)(rocket_vars + 0x44) + D_0015ED60 * 0.1f;
            *(int *)(rocket_vars + 0x28) = 0;
            steering_timer_threshold = scale_game_frames_c(250);
            steering_timer_threshold -= scale_game_frames_c(100);
            if (*(int *)(rocket_vars + 0x24) < steering_timer_threshold) {
                *(float *)(rocket_moby + 0x44) = fast_add_rotations(*(float *)(rocket_moby + 0x44), fast_sin_c(*(float *)(rocket_vars + 0x48)) * (D_0015ED60 * 0.04f));
            }
            *(float *)(rocket_moby + 0x48) = fast_add_rotations(*(float *)(rocket_moby + 0x48), fast_cos_c(*(float *)(rocket_vars + 0x48)) * (D_0015ED60 * 0.05f));
        } else {
            qcopy(&u.v.c, target_moby + 0x10);
            u.v.c.f[2] = u.v.c.f[2] + *(float *)(rocket_vars + 0x2C);
            subtract_vector_xyz(&u.v.b, &u.v.c, rocket_vars);
            subtract_vector_xyz(&u.v.a, &u.v.c, rocket_vars + 0x10);
            qcopy(rocket_vars, &u.v.c);
            {
                float target_displacement_length_squared = FUN_L00_001ff430(&u.v.b);
                float fB = (u.v.b.f[0] * u.v.a.f[0] + u.v.b.f[1] * u.v.a.f[1] + u.v.b.f[2] * u.v.a.f[2]) * -2.0f;
                float fA = *(float *)(rocket_vars + 0x30) * *(float *)(rocket_vars + 0x30) - target_displacement_length_squared;
                float r2f = FUN_L00_001ff430(&u.v.a);
                float sq = square_root_scalar(fB * fB - (4.0f * fA) * (-r2f));
                float intercept_root_plus = (-fB + sq) / (fA + fA);
                float intercept_root_minus = (-fB - sq) / (fA + fA);
                if (0.0f < intercept_root_plus && 0.0f < intercept_root_minus) {
                    selected_intercept_root = (intercept_root_minus < intercept_root_plus) ? intercept_root_plus : intercept_root_minus;
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
                intercept_angle_scratch = fast_arc_tan_c(intercept_direction.f[0], intercept_direction.f[1]);
                *(float *)(rocket_moby + 0x48) = FUN_L00_00258110((float *)(rocket_vars + 0x34), *(float *)(rocket_moby + 0x48), intercept_angle_scratch, D_0015ED70 * 188.49556f, 0.0f, 0.0f);
                intercept_angle_scratch = fast_arc_tan_c(vector_length_xy(&intercept_direction), intercept_direction.f[2]);
                *(float *)(rocket_moby + 0x44) = FUN_L00_00258110((float *)(rocket_vars + 0x38), *(float *)(rocket_moby + 0x44), -intercept_angle_scratch, D_0015ED70 * 188.49556f, 0.0f, 0.0f);
            }
            s = fast_vec_length(&u.v.a);
            if (s < 6.0f) {
                *(float *)(rocket_vars + 0x44) = *(float *)(rocket_vars + 0x44) * (s / 6.0f);
            } else {
                *(float *)(rocket_vars + 0x44) = *(float *)(rocket_vars + 0x44) + D_0015ED60 * 0.1f;
            }
        }
    }

    build_spherical_offset(&forward_step, *(float *)(rocket_vars + 0x30), *(float *)(rocket_moby + 0x48), -*(float *)(rocket_moby + 0x44));
    *(u128_cd *)&u.v.b = 0;
    u.v.b.f[0] = random_float_between_c(-1.0f, 1.0f);
    u.v.b.f[1] = random_float_between_c(-1.0f, 1.0f);
    u.v.b.f[2] = random_float_between_c(-1.0f, 1.0f);
    *(u128_cd *)&u.v.a = *(u128_cd *)&u.v.b;
    fast_vec_normalize(&u.v.a, &u.v.a, random_float_between_c(0.1f, 0.2f) * D_0015ED6C);
    fast_vec_normalize(&z, &forward_step, -random_float_between_c(D_0015ED6C * 0.1f, D_0015ED6C));
    add_vector_xyz(&u.v.a, &u.v.a, &z);
    fast_vec_normalize(&z, &forward_step, random_float_between_c(0.0f, 1.0f) * *(float *)(rocket_vars + 0x30));
    add_vector_xyz(&z, &z, rocket_moby + 0x10);
    {
        int c1 = scale_game_frames_c(FUN_L00_00257b90_c(0x1C, 0x28));
        FUN_L00_00269958(&z, &u.v.a, 0x6F00AFFF, 0x100000FF, c1, 0x28, FUN_L00_00257b90_c(0x28, 0x46), 1);
    }
    qcopy(&previous_position, rocket_moby + 0x10);
    add_vector_xyz(rocket_vars + 0x10, rocket_vars + 0x10, &forward_step);
    *(float *)(rocket_vars + 0x48) = fast_add_rotations(*(float *)(rocket_vars + 0x48), *(float *)(rocket_vars + 0x50));
    *(float *)(rocket_vars + 0x4C) = fast_add_rotations(*(float *)(rocket_vars + 0x4C), *(float *)(rocket_vars + 0x54));
    *(float *)(rocket_vars + 0x48) = fast_add_rotations(*(float *)(rocket_vars + 0x48), fast_sin_c(*(float *)(rocket_vars + 0x4C)) * 0.15f);
    if (0.9f < *(float *)(rocket_vars + 0x44)) *(float *)(rocket_vars + 0x44) = 0.9f;

    flag = 0;
    goto body;
    for (;;) {
        float offset_trig_factor;
        if (!(0.1f < sp)) goto done;
    body:
        offset_trig_factor = fast_sin_c(*(float *)(rocket_vars + 0x48));
        u.v.a.f[1] = *(float *)(rocket_vars + 0x44) * offset_trig_factor * fast_cos_c(*(float *)(rocket_vars + 0x4C));
        offset_trig_factor = fast_cos_c(*(float *)(rocket_vars + 0x48));
        u.v.a.f[2] = *(float *)(rocket_vars + 0x44) * offset_trig_factor * fast_cos_c(*(float *)(rocket_vars + 0x4C));
        zf = 0.0f;
        u.v.a.f[3] = zf;
        u.v.a.f[0] = zf;
        euler_rotation_matrix(va0, rocket_moby + 0x40);
        transform_vector(&u.v.a, &u.v.a, va0);
        add_vector_xyz(rocket_moby + 0x10, rocket_vars + 0x10, &u.v.a);
        if (collision_line(&previous_position, rocket_moby + 0x10, 6, (int)rocket_moby, 0) == 0) goto done;
        goto hit;
    top:;
    }
hit:
    flag = 1;
    sp = *(float *)(rocket_vars + 0x44) * 0.7f;
    *(float *)(rocket_vars + 0x44) = sp;
    goto top;
done:

    if (flag) {
        *(float *)(rocket_moby + 0x48) = fast_arc_tan_c(*(float *)(rocket_moby + 0x10) - previous_position.f[0], *(float *)(rocket_moby + 0x14) - previous_position.f[1]);
        *(float *)(rocket_moby + 0x44) = -fast_arc_tan_c(vector_distance_xy_alt(&previous_position, rocket_moby + 0x10), *(float *)(rocket_moby + 0x18) - previous_position.f[2]);
        *(int *)(rocket_vars + 0x34) = 0;
        *(int *)(rocket_vars + 0x38) = 0;
    }

    if (*(float *)(rocket_moby + 0x10) < 0.0f || *(float *)(rocket_moby + 0x14) < 0.0f || *(float *)(rocket_moby + 0x18) < 0.0f) goto del;
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
            if (p == (char *)D_001413D0_2ccee0 || p == *(char **)(rocket_vars + 0x20)
                || *(short *)(p + 0xA6) == 0x1C9 || *(short *)(p + 0xA6) == 0) goto keep;
            rocket_moby[0xBC] = 2;
            qcopy(rocket_moby + 0x10, gg + 0x20);
            FUN_L00_001ff660(&explosion_direction, &forward_step, gg + 0x40);
            fast_vec_normalize(&explosion_direction, &explosion_direction, D_0015ED6C + D_0015ED6C);
            func_L00_0025F4A8_alt(rocket_moby, &explosion_direction, 0, 0.0f, 0.0f, 10, 3, 9, 4.0f, 2.0f, 9.0f, 1.0f, 0, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_c(rocket_moby);
            return;
        } else {
            if (*(int *)(gg + 0x1C) <= 0) goto keep;
            rocket_moby[0xBC] = 1;
            qcopy(rocket_moby + 0x10, gg + 0x20);
            FUN_L00_001ff660(&explosion_direction, &forward_step, gg + 0x40);
            fast_vec_normalize(&explosion_direction, &explosion_direction, D_0015ED6C + D_0015ED6C);
            func_L00_0025F4A8_alt(rocket_moby, &explosion_direction, 0, 0.0f, 1.0f, 10, 3, 9, 4.0f, 2.0f, 9.0f, 1.0f, 0, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_c(rocket_moby);
            return;
        }
    } else {
        if (tick_countdown_32_alt((int *)(rocket_vars + 0x24)) == 0
            && !(200.0f < vector_distance_xy_alt(rocket_moby + 0x10, (char *)D_001413D0_2ccee0 + 0x10))) goto keep;
    }
del:
    mark_moby_for_removal_c(rocket_moby);
    return;
keep:
    if (*(int *)(rocket_vars + 0x24) % scale_game_frames_c(0x20) != 0) return;
    if (rand() & 1) {
        *(float *)(rocket_vars + 0x50) = *(float *)(rocket_vars + 0x50) * random_float_between_c(-0.7f, -1.5f);
    }
    if (rand() & 1) {
        *(float *)(rocket_vars + 0x54) = *(float *)(rocket_vars + 0x54) * random_float_between_c(-0.6f, -1.6f);
    }
}
#include "qcopy.h"
typedef unsigned int u128_2cdcc0 __attribute__((mode(TI), aligned(16)));
typedef struct {
    unsigned char pad0[0x80];
    float v80[4];
    unsigned char pad90[8];
    float f98;
    unsigned char pad9c[0x1FF5 - 0x9C];
    unsigned char b1FF5;
    unsigned char b1FF6;
    unsigned char pad1ff7[0x2080 - 0x1FF7];
    unsigned char *p2080;
} P_2cdcc0;
typedef struct {
    unsigned char pad0[0xC0];
    unsigned short hC0;
    unsigned short hC2;
    unsigned int wC4;
} S_2cdcc0;
extern S_2cdcc0 D_00141680_2cdcc0 __asm__("D_00141680");
extern int D_0015EEA4_2cdcc0 __asm__("D_0015EEA4");
int FUN_L00_00233e98_2cdcc0(int) __asm__("FUN_L00_00233e98");
void FUN_L00_00233db8_2cdcc0(int, int) __asm__("FUN_L00_00233db8");
float FUN_001f9dc8_2cdcc0(float) __asm__("FUN_001f9dc8");
float FUN_001f9de0_2cdcc0(float) __asm__("FUN_001f9de0");
int FUN_001f96f8_2cdcc0(int) __asm__("FUN_001f96f8");
void FUN_001f9a10_2cdcc0(void *, void *, void *) __asm__("FUN_001f9a10");
unsigned char *FUN_L00_002ce1f0_2cdcc0(unsigned char *, void *) __asm__("FUN_L00_002ce1f0");
void replenish_hero_drones(void) __asm__("FUN_L00_002cdcc0");

void replenish_hero_drones(void) {
    P_2cdcc0 *hero_state = &(*(P_2cdcc0 *)((char *)&hero));
    S_2cdcc0 *usage_stats;
    float d[4] __attribute__((aligned(16)));
    float v[4] __attribute__((aligned(16)));
    unsigned char *spawned_moby;
    int i;
    hero_state->b1FF5 = 0;
    if (FUN_L00_00233e98_2cdcc0(0x18)) {
        d[0] = FUN_001f9dc8_2cdcc0(hero_state->f98) * 0.25f;
        d[1] = FUN_001f9de0_2cdcc0(hero_state->f98) * 0.25f;
        d[2] = 0.25f;
        if (hero_state->b1FF6 < 6) {
            FUN_L00_00233db8_2cdcc0(0x18, 1);
            usage_stats = &D_00141680_2cdcc0;
            if (usage_stats->hC0 < 0xFFFF) {
                usage_stats->hC0++;
            }
            if (FUN_001f96f8_2cdcc0(D_0015EEA4_2cdcc0) / 600 > usage_stats->hC2) {
                usage_stats->hC2 = FUN_001f96f8_2cdcc0(D_0015EEA4_2cdcc0) / 600;
            }
            usage_stats->wC4 = usage_stats->wC4 | (1 << current_level_index) | 0x80000000;

            FUN_001f9a10_2cdcc0(v, hero_state->v80, hero_state->p2080 + 0xE0);
            FUN_001f9a10_2cdcc0(v, v, hero_state->p2080 + 0xC0);
            for (i = hero_state->b1FF6; i < 6; i++) {
                spawned_moby = FUN_L00_002ce1f0_2cdcc0(spawned_moby, v);
                if (spawned_moby) {
                    qcopy(*(void **)(spawned_moby + 0x78), d);
                }
            }
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CF328), where it is exact; names translated to the US level program. */

extern int spawn_moby_point_attached_particle(void *, int, float, float, unsigned, unsigned) __asm__("FUN_L00_002725d8");

void ensure_drone_attached_particles(char *owner_moby) __asm__("FUN_L00_002cde78");

void ensure_drone_attached_particles(char *owner_moby) {
    char *moby_vars = *(char **)(owner_moby + 0x78);
    if (*(int *)(moby_vars + 0x4C) == 0) {
        *(int *)(moby_vars + 0x4C) = spawn_moby_point_attached_particle(owner_moby, 0, 0.05f, 0.01f, 0x80808080, 0x10808080);
    }
    if (*(int *)(moby_vars + 0x50) == 0) {
        *(int *)(moby_vars + 0x50) = spawn_moby_point_attached_particle(owner_moby, 1, 0.05f, 0.01f, 0x80808080, 0x10808080);
    }
}
/* Exact match: tools/ps2eeas_nops.py keeps the hazard nop after the label the branch lands on. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CF3D8), where it is exact; names translated to the US level program. */

extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern int D_L00_0015FFE4;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_002141f8_c(char *) __asm__("FUN_002141f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

char *FUN_L00_002cdf28(char *search_position, int *excl) {
    char *result = 0;
    char *candidate_moby;
    float best = 1e9f;
    float candidate_position_scratch[4];
    for (candidate_moby = (char *)D_L00_0015FFE4; candidate_moby != 0; candidate_moby = *(char **)(candidate_moby + 0x28)) {
        char *candidate_moby_class = *(char **)(candidate_moby + 0x24);
        int class_category = 0;
        float search_to_candidate_distance;
        char *class_callback_data;
        char *x;
        if (candidate_moby_class != 0)
            class_category = *(unsigned char *)(candidate_moby_class + 0x46);
        if (candidate_moby == 0 || *(unsigned char *)(candidate_moby + 0x20) == 0xFE)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x20) == 0xFD)
            continue;
        if (class_category != 5 && class_category != 7 && class_category != 8)
            continue;
        if (excl != 0) {
            int found = 0;
            int i;
            for (i = 0; i < 9; i++) {
                if (excl[i] == (int)candidate_moby) {
                    found = 1;
                    break;
                }
            }
            if (found)
                continue;
        }
        if (class_category == 5 && (*(unsigned short *)(candidate_moby + 0x34) & 0x1000) == 0)
            continue;
        if (*(unsigned char *)(candidate_moby + 0x31) == 0) {
            short c = *(short *)(candidate_moby + 0xA6);
            if (c != 0x350 && c != 0x31)
                continue;
        }
        search_to_candidate_distance = fast_vec_dist(search_position, candidate_moby + 0x10);
        if (*(short *)(candidate_moby + 0xA6) == 0x4D6) {
            normalize_vector_xyz(candidate_position_scratch, *(char **)(candidate_moby + 0x78), search_to_candidate_distance);
            add_vector_xyz(candidate_position_scratch, candidate_position_scratch, candidate_moby + 0x10);
            if (search_to_candidate_distance < fast_vec_dist(search_position, candidate_position_scratch))
                continue;
        } else {
            float category_distance_limit;
            switch (class_category) {
            case 5:
                category_distance_limit = 4.0f;
                break;
            case 7:
                category_distance_limit = 5.3f;
                break;
            case 8:
                category_distance_limit = 6.0f;
                break;
            default:
                goto skip;
            }
            if (category_distance_limit < search_to_candidate_distance)
                continue;
        }
    skip:
        if (!(search_to_candidate_distance < best))
            continue;
        class_callback_data = get_moby_suck_cannon_record(candidate_moby);
        if (class_callback_data != 0 && *(short *)(class_callback_data + 0x68) > 0)
            continue;
        x = (char *)FUN_002141f8_c(candidate_moby);
        qcopy(candidate_position_scratch, candidate_moby + 0x10);
        if (x != 0)
            candidate_position_scratch[2] = candidate_position_scratch[2] + (*(float *)(x + 0x10) + 0.05f);
        if (collision_line(search_position, candidate_position_scratch, 6, (int)candidate_moby, 0) == 0) {
            best = search_to_candidate_distance;
            result = candidate_moby;
        }
    }
    return result;
}

extern char *spawn_2ce1f0(int) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_0025d1b8(void *);
extern float atan2_2ce1f0(float, float) __asm__("FUN_001f9e90");
extern float dist_2ce1f0(void *, void *) __asm__("FUN_001f9b80");

/* Spawns moby 0x1DF at pos, aims it at the player and puts it in the first
 * free of six slots. */
char *spawn_hero_drone(int previous_spawn_result, void *pos) __asm__("FUN_L00_002ce1f0");

char *spawn_hero_drone(int previous_spawn_result, void *pos) {
    char *spawned_moby = spawn_2ce1f0(0x1DF);
    char *moby_vars;
    struct Hero *player;

    if (spawned_moby != 0) {
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        *(unsigned char *)(spawned_moby + 0x31) = 1;
        *(unsigned char *)(spawned_moby + 0x20) = 2;
        moby_vars = *(char **)(spawned_moby + 0x78);
        *(unsigned char *)(spawned_moby + 0xBC) = 0;
        qcopy(spawned_moby + 0x10, pos);
        *(int *)(moby_vars + 0x44) = previous_spawn_result;
        player = &hero;
        player->ammo_used++;
        FUN_L00_00250df8(spawned_moby);
        FUN_L00_0025d1b8(spawned_moby);
        *(float *)(moby_vars + 0x28) = atan2_2ce1f0(*(float *)(spawned_moby + 0x10) - player->motion.pos.f[0], *(float *)(spawned_moby + 0x14) - player->motion.pos.f[1]);
        *(float *)(moby_vars + 0x24) = atan2_2ce1f0(dist_2ce1f0(&player->motion.pos, spawned_moby + 0x10), *(float *)(spawned_moby + 0x18) - player->motion.pos.f[2]);
        *(float *)(moby_vars + 0x20) = dist_2ce1f0(spawned_moby + 0x10, &player->motion.pos);
        *(float *)(moby_vars + 0x3C) = player->motion.pos.f[2];
        for (*(short *)(moby_vars + 0x54) = 0; *(short *)(moby_vars + 0x54) < 6; (*(short *)(moby_vars + 0x54))++) {
            if (hero.unk2020[*(short *)(moby_vars + 0x54)] == 0) {
                hero.unk2020[*(short *)(moby_vars + 0x54)] = spawned_moby;
                break;
            }
        }
        *(short *)(moby_vars + 0x56) = 100;
        *(int *)(spawned_moby + 0x94) = 0;
    }
    return spawned_moby;
}

/* Ported from rac1-decomp (nonmatching/shared/func_L00_002CF808.c: func_L00_002CF808). */
typedef int OvlQuad_2C9820 __attribute__((mode(TI)));

typedef struct {
    u8 pad0[0x18];
    s32 moby; /* 0x18: passed to FUN_002141f8 (reads moby+0x34, moby+0x78); 0 when no moby was hit */
    s32 unk1C; /* 0x1C: > 0 gates copying point and the normal into the hero (shared/ui/help/00226f10.c) */
    union {
        OvlQuad_2C9820 q;
        f32 f[4];
    } point; /* 0x20: f[2] is read as the ground height */
    u8 pad30[0x10]; /* 0x30: position after push-out (copied into hero.motion.pos) */
    f32 normal_x; /* 0x40: start of the surface normal; its angle is tested against 50 degrees (0.87266463) */
    f32 normal_y; /* 0x44: FUN_001f9e90 (atan2) of normal_x, normal_y is stored in the hero */
    f32 normal_z; /* 0x48: ledge probes compare atan2(normal_z, xy length) with 20 degrees */
} CollisionHit_2C9820;

typedef struct {
    Vec4 position;            /* 0x00: movement this frame */
    Vec4 offset;              /* 0x10: last target position */
    f32 strength;             /* 0x20 */
    f32 elevation;            /* 0x24 */
    f32 yaw;                  /* 0x28 */
    f32 unused2c;
    struct Moby *target;      /* 0x30 */
    f32 pitch;                /* 0x34 */
    f32 unused38;
    f32 height;               /* 0x3C */
    f32 desired_yaw;          /* 0x40 */
    f32 unused44;
    s32 target_oclass;        /* 0x48 */
    f32 unused4c;
    f32 unused50;
    s16 slot;                 /* 0x54 */
    s16 health;               /* 0x56 */
    f32 yaw_velocity;         /* 0x58 */
    f32 pitch_velocity;       /* 0x5C */
} DroneVars_2CF808;

typedef struct {
    u8 pad_0[0x1FF4];
    u8 counter;               /* 0x1FF4 */
    u8 unk1FF5;
    u8 ammo_used;             /* 0x1FF6: drones alive */
    u8 ammo_capacity;         /* 0x1FF7: drones still orbiting */
    f32 phase;                /* 0x1FF8 */
    struct Moby *exclusions[9]; /* 0x1FFC */
    struct Moby *slots[6];    /* 0x2020 */
} DroneSlots_2CF808;

/* Hit record passed to FUN_L00_00259a88; the drone builds it over its direction and rotated vectors. */
typedef struct {
    Vec4 direction;           /* 0x00 */
    s32 unk10[2];
    u8 kind;                  /* 0x18 */
    u8 mode;                  /* 0x19 */
    u16 oclass;               /* 0x1A */
    f32 amount;               /* 0x1C: read back after the hit */
} BombHit_2CF808;

extern Vec4 D_L00_00173E70;
extern void clamp_vector_length(void *, void *, f32) __asm__("FUN_001f9c90");
extern s32 func_L00_001F10E0_2CF808(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern s32 func_L00_00259868_2CF808(f32, f32, f32, void *, void *, s32) __asm__("FUN_L00_00258830");
extern void initialize_hit_descriptor(void *, void *, s32, f32) __asm__("FUN_L00_00259858");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void func_L00_0025B040_2CF808(f32, void *) __asm__("FUN_L00_00259fe8");
extern void *func_L00_0025B478_2CF808(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern f32 func_L00_00259148_2CF808(f32, f32, f32, f32, f32, f32 *) __asm__("FUN_L00_00258110");
extern void func_L00_0025F4A8_2CF808(void *, void *, void *, f32, f32, s32, s32, s32, f32, f32, f32, s32, f32, f32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern struct Moby *func_L00_002CF3D8_2CF808(void *, void *) __asm__("FUN_L00_002cdf28");
extern void func_L00_002CF328_2CF808(void *) __asm__("FUN_L00_002cde78");

void update_drone(struct Moby *updated_moby) __asm__("FUN_L00_002ce358");

void update_drone(struct Moby *updated_moby) {
    DroneVars_2CF808 *drone_vars = (DroneVars_2CF808 *)updated_moby->pvars;
    Vec4 collision_data[1], old_pos;
    Vec4 direction, rotated;
    Vec4 target_pos, matrix[4];
    struct Moby *target;
    struct Moby *closest;
    struct Moby *other;
    f32 f, distance;
    s32 i, count, category, random, health;
    s32 state;
    struct Moby *drone;
    s32 k, exclusion_clear_index;

    if (D_L00_0015F5C4_2c8440 == 2 || D_L00_0015F5C4_2c8440 == 5 ||
        D_L00_0015F5C4_2c8440 == 6 || hero.state.current == 0x32) {
        updated_moby->flags |= 0x41;
        return;
    }
    updated_moby->flags &= 0xFFBE;
    if (updated_moby->state < 5) {
        ((DroneSlots_2CF808 *)&hero)->counter++;
        if (((DroneSlots_2CF808 *)&hero)->ammo_used < ((DroneSlots_2CF808 *)&hero)->counter) {
            closest = 0;
            count = 0;
            distance = 1000000.0f;
            ((DroneSlots_2CF808 *)&hero)->counter = 1;
            ((DroneSlots_2CF808 *)&hero)->ammo_capacity = 0;
            ((DroneSlots_2CF808 *)&hero)->phase = fast_add_rotations(((DroneSlots_2CF808 *)&hero)->phase, frame_scale * 0.05f);
            for (k = 0; k < 6; k++) {
                drone = ((DroneSlots_2CF808 *)&hero)->slots[k];
                if (drone != 0) {
                    if (drone->oclass == 0x1DF) {
                        if (drone->state == 0xFE) {
                            ((DroneSlots_2CF808 *)&hero)->slots[k] = 0;
                        } else if (drone->state == 0xFD) {
                            ((DroneSlots_2CF808 *)&hero)->slots[k] = 0;
                        } else {
                            ((DroneSlots_2CF808 *)&hero)->exclusions[count] = *(struct Moby **)(drone->pvars + 0x30);
                            count++;
                            if (drone->state < 4)
                                ((DroneSlots_2CF808 *)&hero)->ammo_capacity++;
                            if (count > 8)
                                break;
                        }
                    } else {
                        ((DroneSlots_2CF808 *)&hero)->slots[k] = 0;
                    }
                }
            }
            for (exclusion_clear_index = count; exclusion_clear_index < 9; exclusion_clear_index++)
                ((DroneSlots_2CF808 *)&hero)->exclusions[exclusion_clear_index] = 0;
            other = func_L00_002CF3D8_2CF808(&hero.motion, ((DroneSlots_2CF808 *)&hero)->exclusions);
            if (other != 0) {
                char *h;
                h = suck_get_aim_height(other);
                qcopy(&old_pos, &other->pos);
                if (h != 0)
                    old_pos.f[2] += *(f32 *)(h + 0x10);
                for (i = 0; i < 6; i++) {
                    drone = ((DroneSlots_2CF808 *)&hero)->slots[i];
                    if (drone != 0) {
                        struct Moby *drone_target = *(struct Moby **)(drone->pvars + 0x30);
                        if (drone_target == 0 || drone_target->state == 0xFE || drone_target->state == 0xFD) {
                            f32 candidate_distance = vector_distance(&drone->pos, &old_pos);
                            if (candidate_distance < distance) {
                                closest = drone;
                                distance = candidate_distance;
                            }
                        }
                    }
                }
                if (closest != 0) {
                    DroneVars_2CF808 *assigned = (DroneVars_2CF808 *)closest->pvars;
                    char *target_height_data;
                    assigned->target = other;
                    assigned->target_oclass = other->oclass;
                    target_height_data = suck_get_aim_height(other);
                    qcopy(&old_pos, &other->pos);
                    if (target_height_data != 0)
                        old_pos.f[2] += *(f32 *)(target_height_data + 0x10);
                    qcopy(&assigned->offset, &old_pos);
                    closest->state = 3;
                    random = rand_2cc608() % 3;
                    allocate_voice_for_target_entry(random + (random > 0), 0, updated_moby);
                    ((DroneSlots_2CF808 *)&hero)->exclusions[count] = assigned->target;
                }
            }
        }
        {
            s32 capacity = ((DroneSlots_2CF808 *)&hero)->ammo_used;
            s32 divisor = capacity ? capacity : 1;
            s32 next = capacity + 1;
            s32 counter = ((DroneSlots_2CF808 *)&hero)->counter;
            if (next == 0)
                next = 1;
            drone_vars->desired_yaw = convert_integer_to_float(counter) / (f32)divisor * 6.2831855f - 3.1415927f;
            drone_vars->pitch = (convert_integer_to_float((counter * (next / 5)) % next) / (f32)divisor * 6.2831855f - 3.1415927f) * 0.25f;
        }
        {
            void *hit = func_L00_0025B478_2CF808(updated_moby, 1, 0);
            if (hit != 0) {
                char *h = *(char **)((char *)hit + 0x20);
                s32 cat = h && *(char **)(h + 0x24) ? *(s16 *)(*(char **)(h + 0x24) + 0x46) : 0;
                if (cat > 8) goto dispatch_state;
                if (cat >= 5) {
                    if (hit == 0 || h == 0 || *(s16 *)(h + 0xA6) != 0x4D6 || fast_dec_timer_s16(&drone_vars->health))
                        updated_moby->state = 4;
                }
            }
        }
    }
dispatch_state:
    state = updated_moby->state;
    if (state == 3) goto state3;
    if (state < 4) {
        if (state == 2) goto state2;
    } else {
        if (state == 4) goto state4;
        if (state == 5) goto state5;
    }
    goto update_effects;
state2:
    {
        updated_moby->collision = 0;
        qcopy(&old_pos, &updated_moby->pos);
        func_L00_002CF328_2CF808(updated_moby);
        {
            f32 step = frame_scale * 0.05f;
            updated_moby->scale += (updated_moby->pclass->scale - updated_moby->scale) * step;
            drone_vars->strength = drone_vars->strength + (1.0f - drone_vars->strength) * step;
        }
        f = fast_add_rotations(drone_vars->desired_yaw, ((DroneSlots_2CF808 *)&hero)->phase);
        distance = fast_subtract_rotations(f, drone_vars->yaw);
        drone_vars->yaw = fast_add_rotations(drone_vars->yaw, distance * (frame_scale * 0.05f));
        distance = fast_subtract_rotations(drone_vars->pitch, drone_vars->elevation);
        drone_vars->elevation = fast_add_rotations(drone_vars->elevation, distance * (frame_scale * 0.01f));
        direction.f[0] = fast_cos(drone_vars->yaw) * drone_vars->strength;
        direction.f[1] = fast_sin(drone_vars->yaw) * drone_vars->strength;
        direction.f[2] = 0.0f;
        rotated.f[0] = 0.0f;
        rotated.f[1] = drone_vars->elevation;
        rotated.f[2] = 0.0f;
        rotated.f[3] = 0.0f;
        euler_rotation_matrix(matrix, &rotated);
        apply_mat4_2cc608(&direction, &direction, matrix);
        add_vector_xyz(&target_pos, &hero.motion, (u8 *)hero.moby + 0xE0);
        target_pos.f[0] += direction.f[0];
        target_pos.f[1] += direction.f[1];
        drone_vars->height = drone_vars->height + (target_pos.f[2] - drone_vars->height) * (frame_scale * 0.05f);
        target_pos.f[2] = drone_vars->height + direction.f[2];
        subtract_vector_xyz_2c8440(&target_pos, &target_pos, &updated_moby->pos);
        clamp_vector_length(&target_pos, &target_pos, frame_time * 10.0f);
        if (func_L00_00259868_2CF808(0.0f, 0.18f, 0.0f, updated_moby, &target_pos, 0) & 1) {
            if (!updated_moby->visible) {
                Vec4 *hitv = &D_L00_00166DC0_2c8440;
                add_vector_xyz(&updated_moby->pos, hitv, hitv + 0x23);
                subtract_vector_xyz_2c8440(&updated_moby->pos, &updated_moby->pos, hitv + 0x21);
            }
        }
        qcopy(&collision_data[0], &updated_moby->pos);
        if (func_L00_001F10E0_2CF808(0.2f, &collision_data[0], 0, updated_moby)) {
            if (((struct Moby *)coll_hit.moby) && ((struct Moby *)coll_hit.moby)->oclass == 0 && ((struct Moby *)coll_hit.moby)->state != 0xFE && ((struct Moby *)coll_hit.moby)->state != 0xFD)
                goto update_rotations;
            category = ((struct Moby *)coll_hit.moby) && ((struct Moby *)coll_hit.moby)->pclass ? *(u8 *)((u8 *)((struct Moby *)coll_hit.moby)->pclass + 0x46) : 0;
            if (((struct Moby *)coll_hit.moby) && ((struct Moby *)coll_hit.moby)->state != 0xFE && ((struct Moby *)coll_hit.moby)->state != 0xFD &&
                (category == 5 || category == 7 || category == 8)) {
                initialize_hit_descriptor(((BombHit_2CF808 *)&direction), updated_moby, 0x10000, 1.0f);
                set_vector_length_xy(((BombHit_2CF808 *)&direction), &drone_vars->position, 1.0f);
                ((BombHit_2CF808 *)&direction)->direction.f[2] = 1.0f;
                ((BombHit_2CF808 *)&direction)->direction.f[3] = 5627.925f;
                ((BombHit_2CF808 *)&direction)->kind = 1;
                ((BombHit_2CF808 *)&direction)->mode = 2;
                ((BombHit_2CF808 *)&direction)->oclass = updated_moby->oclass;
                record_moby_hit((struct Moby *)coll_hit.moby, ((BombHit_2CF808 *)&direction));
                if (category == 5) {
                    if (coll_hit.moby && ((struct Moby *)coll_hit.moby)->oclass == 0x4D6 && (health = drone_vars->health) > 0) {
                        random = truncate_float_to_s32(((BombHit_2CF808 *)&direction)->amount);
                        if (random > 0)
                            random = health - truncate_float_to_s32(((BombHit_2CF808 *)&direction)->amount);
                        else
                            random = health - 1;
                        drone_vars->health = random;
                    } else {
                        func_L00_0025F4A8_2CF808(updated_moby, drone_vars, 0, 0.0f, 0.0f, 0, 1, 1, 0.4f, 0.2f, 4.0f, 1, 1.0f, 8.0f, 1, 1, -1, 0);
                        updated_moby->state = 4;
                    }
                }
            }
        }
update_rotations:
        subtract_vector_xyz_2c8440(&drone_vars->position, &updated_moby->pos, &old_pos);
        {
            f32 yaw = fast_arc_tan(updated_moby->pos.x - old_pos.f[0], updated_moby->pos.y - old_pos.f[1]);
            f32 pitch = -fast_arc_tan(vector_distance(&old_pos, &updated_moby->pos), updated_moby->pos.z - old_pos.f[2]);
            updated_moby->rot.z = func_L00_00259148_2CF808(updated_moby->rot.z, yaw, frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 6.2831855f, &drone_vars->yaw_velocity);
            updated_moby->rot.y = func_L00_00259148_2CF808(updated_moby->rot.y, pitch, frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 6.2831855f, &drone_vars->pitch_velocity);
        }
        goto update_effects;
    }
state3:
    {
        struct MobyClass *pclass = updated_moby->pclass;
        updated_moby->collision = pclass->collision;
        qcopy(&old_pos, &updated_moby->pos);
        updated_moby->scale += (pclass->scale - updated_moby->scale) * (frame_scale * 0.05f);
        updated_moby->collision = pclass->collision;
        func_L00_002CF328_2CF808(updated_moby);
        target = drone_vars->target;
        if (target != 0 && target->oclass == drone_vars->target_oclass && target->state != 0xFE && target->state != 0xFD) {
            char *h = suck_get_aim_height(target);
            qcopy(&target_pos, &drone_vars->target->pos);
            if (h != 0)
                target_pos.f[2] += *(f32 *)(h + 0x10);
            subtract_vector_xyz_2c8440(&direction, &target_pos, &drone_vars->offset);
            scale_vector_xyz_2c8440(&direction, &direction, 3.1f);
            add_vector_xyz(&rotated, &target_pos, &direction);
            subtract_vector_xyz_2c8440(&drone_vars->position, &rotated, &updated_moby->pos);
            scale_vector_xyz_2c8440(&drone_vars->position, &drone_vars->position, frame_scale * -0.8f + 1.0f);
            add_vector_xyz(&updated_moby->pos, &updated_moby->pos, &drone_vars->position);
            qcopy(&drone_vars->offset, &target_pos);
            qcopy(&collision_data[0], &updated_moby->pos);
            if (func_L00_001F10E0_2CF808(0.25f, &collision_data[0], 0, updated_moby)) {
                struct Moby *hm = (struct Moby *)coll_hit.moby;
                if (!hm || hm->state == 0xFE || hm->state == 0xFD || hm != drone_vars->target || hm->oclass == 0) {
                    qcopy(&updated_moby->pos, &D_L00_00173E70);
                } else {
                    s32 cat2 = hm->pclass ? *(u8 *)((u8 *)hm->pclass + 0x46) : 0;
                    initialize_hit_descriptor(((BombHit_2CF808 *)&direction), updated_moby, 0x10000, 1.0f);
                    set_vector_length_xy(((BombHit_2CF808 *)&direction), &drone_vars->position, 1.0f);
                    ((BombHit_2CF808 *)&direction)->direction.f[2] = 1.0f;
                    ((BombHit_2CF808 *)&direction)->direction.f[3] = 5627.925f;
                    ((BombHit_2CF808 *)&direction)->kind = 1;
                    ((BombHit_2CF808 *)&direction)->mode = 2;
                    ((BombHit_2CF808 *)&direction)->oclass = updated_moby->oclass;
                    record_moby_hit((struct Moby *)coll_hit.moby, ((BombHit_2CF808 *)&direction));
                    if (cat2 == 5) {
                        if (coll_hit.moby && ((struct Moby *)coll_hit.moby)->oclass == 0x4D6 && (health = drone_vars->health) > 0) {
                            random = truncate_float_to_s32(((BombHit_2CF808 *)&direction)->amount);
                            if (random > 0)
                                random = health - truncate_float_to_s32(((BombHit_2CF808 *)&direction)->amount);
                            else
                                random = health - 1;
                            drone_vars->health = random;
                        } else {
                            func_L00_0025F4A8_2CF808(updated_moby, drone_vars, 0, 0.0f, 0.0f, 0, 1, 1, 0.4f, 0.2f, 4.0f, 1, 1.0f, 8.0f, 1, 1, -1, 0);
                            updated_moby->state = 4;
                        }
                    }
                }
            }
        }
        else {
            updated_moby->state = 2;
            drone_vars->target_oclass = -1;
            drone_vars->target = 0;
        }
        updated_moby->rot.z = fast_arc_tan(updated_moby->pos.x - old_pos.f[0], updated_moby->pos.y - old_pos.f[1]);
        updated_moby->rot.y = -fast_arc_tan(vector_distance(&old_pos, &updated_moby->pos), updated_moby->pos.z - old_pos.f[2]);
        goto update_effects;
    }
state4:
    {
        target = drone_vars->target;
        if (target == 0) goto fade_clear;
        if (target->oclass != drone_vars->target_oclass) goto fade_clear;
        if (target->state == 0xFE) goto fade_clear;
        if (target->state == 0xFD) goto fade_clear;
        {
            char *h = suck_get_aim_height(target);
            qcopy(&old_pos, &drone_vars->target->pos);
            if (h != 0)
                old_pos.f[2] += *(f32 *)(h + 0x10);
            subtract_vector_xyz_2c8440(&drone_vars->position, &old_pos, &updated_moby->pos);
            scale_vector_xyz_2c8440(&drone_vars->position, &drone_vars->position, frame_scale * -0.8f + 1.0f);
            clamp_vector_length(&drone_vars->position, &drone_vars->position, frame_scale * 0.9f);
            add_vector_xyz(&updated_moby->pos, &updated_moby->pos, &drone_vars->position);
        }
        goto fade_alpha;
fade_clear:
        drone_vars->target = 0;
fade_alpha:
        updated_moby->alpha -= 4;
        if (updated_moby->alpha < 6)
            updated_moby->state = 5;
        goto update_effects;
    }
state5:
    {
        ((DroneSlots_2CF808 *)&hero)->ammo_used--;
        ((DroneSlots_2CF808 *)&hero)->slots[drone_vars->slot] = 0;
        mark_moby_for_removal(updated_moby);
        return;
    }
update_effects:
    func_L00_0025B040_2CF808(0.17f, updated_moby);
}
/* Crate update (pvars at moby + 0x78). */
typedef struct {
    u8 pad00[0x1E];
    u16 flags;
} Coll;

typedef struct {
    u8 pad00[0x20];
    Coll coll;
    u64 unk40;
    u8 pad48[0xA0 - 0x48];
    struct Moby *rider;               /* crate stacked on this one */
    struct Moby *below;               /* crate this one sits on */
    s32 unkA8;
    u32 flags;
    u8 padB0[0xB4 - 0xB0];
    s32 frameB4;
    u64 unkB8;                        /* saved moby spawn_frame word */
    u8 padC0[0xC6 - 0xC0];
    s16 unkC6;
    u8 unkC8;
    u8 unkC9;
    u8 unkCA;
    u8 padCB[0xFC - 0xCB];
    s32 frameFC;
} CrateVars;

/* Fourth argument of collect_mobies_in_sphere. */
typedef struct {
    Vec4f pos;
    struct Moby *moby;
    s32 unk14;
    u8 pad18[0x1C - 0x18];
    f32 unk1C;
    s32 unk20;
} Burst;

/* D_L00_001C40B0 rows (0x18 bytes); unkE is compared with weapon_ammo_counts. */

extern u8 D_0013D4C0[];
extern s32 weapon_ammo_counts[] __asm__("D_0013D428");
extern u8 D_L00_00197BC0[];
extern struct MobyClass *D_L00_00197300[];
extern s32 D_0015ED84 __attribute__((section(".sdata")));
extern int D_L00_0015F5CC_macro __asm__("D_L00_0015F5CC") __attribute__((section(".sdata")));
extern void *D_001413D0 __attribute__((section(".data")));
extern s32 D_001413DC __attribute__((section(".data")));

extern s32 is_moby_in_class_range_500_to_540(void *) __asm__("FUN_L00_0025e3b8");
extern void update_crate_stack_motion(struct Moby *) __asm__("FUN_L00_002d1428");
extern Contact *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int fun_00213928(void *, void *, void *, int, int *, int *, int *, int) __asm__("FUN_00213928");
extern void FUN_L00_002cfcb8(struct Moby *);
extern void create_linked_class_0326_moby(struct Moby *) __asm__("FUN_L00_002d8138");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void update_moby_animation_state(struct Moby *) __asm__("FUN_0020c880");
extern void break_crate(struct Moby *, int, Contact *) __asm__("FUN_L00_002d09b8");
extern void crate_drop_bolts(struct Moby *) __asm__("FUN_L00_002d0538");
extern s32 collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
extern int FUN_L00_001fefc8(void *);
extern int get_first_moby_in_group(struct Moby **, int, int, int) __asm__("FUN_L00_002591d0");
extern int get_next_moby_in_group(struct Moby **, struct Moby *, int, int) __asm__("FUN_L00_002592b8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");

void crate_update(struct Moby *moby) __asm__("FUN_L00_002cf218");
void crate_update(struct Moby *moby) {
    enum {
        CRATE_STATE_INITIALIZE = 0,
        CRATE_STATE_PROCESS_HITS = 1,
        CRATE_STATE_CLASS_CHANGE_COOLDOWN = 2,
        CRATE_STATE_BROKEN = 3,
        CRATE_STATE_REACTIVATION_DELAY = 4,
        CRATE_STATE_DETONATION_COUNTDOWN = 5,
        CRATE_STATE_WAIT_RESPAWN = 6
    };
    Burst burst;
    Vec4f at;
    s32 hitKind;
    s32 hitB;
    s32 hitAngle;
    struct Moby *it;
    CrateVars *crate_vars;
    Contact *other;
    Coll *coll;
    s32 touched;
    s32 doHit;

    crate_vars = (CrateVars *)moby->pvars;
    if (crate_vars->rider != NULL) {
        if (is_moby_in_class_range_500_to_540(crate_vars->rider) == 0) {
            crate_vars->rider = NULL;
        }
    }
    if (crate_vars->below != NULL) {
        if (is_moby_in_class_range_500_to_540(crate_vars->below) == 0) {
            crate_vars->below = NULL;
        }
    }
    crate_vars->frameFC = D_L00_0015F5CC_macro;
    coll = &crate_vars->coll;
    if (crate_vars->rider == NULL) {
        coll->flags |= 1;
    } else {
        coll->flags &= ~1;
    }
    if (crate_vars->frameB4 != D_L00_0015F5CC_macro && moby->state != 0 && (crate_vars->flags & 0xC)) {
        update_crate_stack_motion(moby);
    }
    touched = 0;
    other = find_moby_hit(moby, 0x1830000, 0);
    if (other != NULL && other->hit.s.flags == 0x1000000) {
        other = NULL;
        touched = 1;
    }
    if (moby->oclass >= 0x1FB && moby->oclass <= 0x1FD) {
        fun_00213928(moby, other, coll, 0, &hitKind, &hitB, &hitAngle, 4);
    }

    switch (moby->state) {
    case CRATE_STATE_INITIALIZE:
        clear_u64_value(&crate_vars->unk40);
        if (D_0015ED84 == 0x12 && moby->oclass == 0x1FF && crate_vars->unkC8 != 0) {
            moby->state = 6;
            moby->flags |= 0x41;
            moby->collision = 0;
            moby->flags &= ~0x1000;
        } else {
            moby->state = 1;
        }
        crate_vars->unkA8 = 0;
        if ((crate_vars->flags ^ 1) & 1) {
            FUN_L00_002cfcb8(moby);
        }
        crate_vars->flags |= 0x14;
        if (moby->oclass == 0x1FF) {
            crate_vars->unkC6 = 100;
        } else if (moby->oclass == 0x1F5) {
            create_linked_class_0326_moby(moby);
        }
        break;

    case CRATE_STATE_PROCESS_HITS:
        if (moby->oclass == 0x1F9 &&
            (hero.unk240 == (s32)moby || hero.coll_hit_moby == (u8 *)moby ||
             (hero.ground_moby == moby && hero.air_frames.s == 0) ||
             hero.unk4F8 == (s32)moby || touched)) {
            moby->command = 0;
            moby->state = 5;
            break;
        }
        if (other == NULL || !(other->damage > 0.0f)) {
            break;
        }
        doHit = 1;
        if (moby->oclass == 0x1F6 && !(other->hit.s.flags & 0x20000)) {
            doHit = 0;
        }
        if (moby->oclass == 0x1F9) {
            moby->state = 5;
            if (other->hit.s.flags & 0x20000) {
                moby->command = 1;
            } else {
                moby->command = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(5.0f, 15.0f)));
            }
            crate_vars->unkB8 = moby->spawn_frame;
            doHit = 0;
        }
        if (moby->oclass >= 0x1FB && moby->oclass <= 0x1FD) {
            if (other->hit.s.flags & 0x20000) {
                moby->oclass = 0x1FE;
            }
            if (hitKind == 3) {
                moby->oclass = 0x1FE;
            }
            if ((other->hit.bits & 0x0050000000000000LL) || hitAngle == 0xB4) {
                s32 next = moby->oclass + 1;

                if (next >= 0x1FF) {
                    next = 0x1FE;
                }
                moby->oclass = next;
            }
            if (moby->oclass != 0x1FE) {
                moby->oclass++;
                moby->class_slot = D_L00_00197BC0[moby->oclass];
                moby->pclass = D_L00_00197300[moby->class_slot];
                update_moby_animation_state(moby);
                moby->unk72 = ((Part *)moby->pclass)->unkE;
                moby->state = 2;
                moby->command = scale_game_frames(15);
                doHit = 0;
            }
        }
        if (doHit == 0) {
            break;
        }
        break_crate(moby, moby->oclass == 0x1F9, other);
        if (crate_vars->unkC8 != 0 && other != NULL && other->hit.s.moby != NULL) {
            s16 hit_moby_class_id = other->hit.s.moby->oclass;

            if (hit_moby_class_id == 0x2CD || hit_moby_class_id == 0x22C) {
                s16 saved = moby->unkB4;

                moby->unkB4 = 0;
                crate_drop_bolts(moby);
                moby->unkB4 = saved;
                break;
            }
        }
        crate_drop_bolts(moby);
        break;

    case CRATE_STATE_CLASS_CHANGE_COOLDOWN:
        if (--moby->command == 0xFF) {
            moby->hit_slot = 0xFF;
            moby->state = 1;
        }
        break;

    case CRATE_STATE_BROKEN:
        if (moby->oclass == 0x1F9 && crate_vars->unkCA <= scale_game_frames(20)) {
            f32 burst_radius_scale;

            burst.moby = moby;
            burst.unk1C = 2.0f;
            clear_u64_value(&burst.pos);
            burst.unk20 = 0;
            normalize_vector_xyz(&at, &moby->rot_row2, 0.5f);
            add_vector_xyz(&at, &at, &moby->pos);
            burst_radius_scale = convert_integer_to_float(crate_vars->unkCA) / multiply_global_factor_ed64(20.0f);
            burst.unk14 = 1;
            collect_mobies_in_sphere(burst_radius_scale + burst_radius_scale, &at, 0x10, moby, &burst);
            burst.unk14 = 0x30000;
            collect_mobies_in_sphere(burst_radius_scale * 3.0f, &at, 0x10, D_001413D0, &burst);
            crate_vars->unkCA++;
            break;
        }
        if (crate_vars->unkC8 != 0) {
            moby->flags |= 1;
            moby->collision = 0;
            moby->flags &= ~0x1000;
            moby->visible = 0;
            if (moby->unkB4 >= 2) {
                moby->unkB4 = 1;
            }
            moby->state = 6;
            if (D_0015ED84 == 0x12) {
                crate_vars->unkC9 = FUN_L00_00257b90(300, 720);
            } else {
                crate_vars->unkC9 = FUN_L00_00257b90(30, 60);
            }
            break;
        }
        mark_moby_for_removal(moby);
        return;

    case CRATE_STATE_WAIT_RESPAWN: {
        qcopy(&burst.pos, &moby->pos);
        burst.pos.z += 0.5f;
        burst.pos.w = 1.0f;
        if (fast_bsphere_check(&burst.pos, moby->draw_dist) != -1) {
            break;
        }
        if (!FUN_L00_001fefc8(&crate_vars->unkC9)) {
            break;
        }
        if (moby->group == 0xFF) {
            moby->state = 1;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = ((Part *)moby->pclass)->unk10;
            moby->flags |= 0x1000;
            break;
        }
        {
            s32 ok = 1;

            if (D_0015ED84 == 0x12) {
                s32 low = 0;
                s32 total = 0;
                s32 i;

                for (i = 0; i < 0x25; i++) {
                    if (D_0013D4C0[i] != 0) {
                        VendorItemInfo *weapon_item_info = &D_L00_001C40B0[i];

                        total++;
                        if (weapon_item_info->hE != 0 && (f32)weapon_ammo_counts[i] < (f32)weapon_item_info->hE * 0.8f) {
                            low++;
                        }
                    }
                }
                if ((low < 3 && total >= 3) || (total < 3 && low != total)) {
                    ok = 0;
                }
            }
            if (!ok) {
                break;
            }
        }
        get_first_moby_in_group(&it, moby->group, 0, 0);
        while (it != NULL) {
            if (moby != it && !(vector_distance(&moby->pos, &it->pos) > 0.1f) && it->state == 1) {
                CrateVars *ipv = (CrateVars *)it->pvars;

                if (ipv->rider == NULL) {
                    qcopy(&moby->pos, &it->pos);
                    moby->pos.z += 1.0f;
                    ipv->rider = moby;
                    crate_vars->below = it;
                    crate_vars->rider = NULL;
                    moby->state = 1;
                    moby->visible = 1;
                    moby->flags &= ~1;
                    moby->collision = ((Part *)moby->pclass)->unk10;
                    moby->flags |= 0x1000;
                    crate_vars->flags |= 4;
                    break;
                }
            }
            get_next_moby_in_group(&it, it, 0, 0);
        }
        if (moby->state != 1) {
            moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
            moby->state = 1;
            crate_vars->rider = NULL;
            crate_vars->below = NULL;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = ((Part *)moby->pclass)->unk10;
            moby->flags |= 0x1000;
        }
        break;
    }

    case CRATE_STATE_DETONATION_COUNTDOWN:
        if (moby->command == 0) {
            moby->command = scale_game_frames(60) * 3 - 1;
            crate_vars->unkB8 = moby->spawn_frame;
        } else if (moby->command > scale_game_frames(15) && other != NULL) {
            moby->command = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(5.0f, 15.0f)));
        }
        if (moby->command % scale_game_frames(60) == scale_game_frames(60) - 3 ||
            moby->command == scale_game_frames(13)) {
            allocate_voice_for_target_entry(1, 0x20, (char *)moby);
        }
        if (moby->command % scale_game_frames(60) > scale_game_frames(40) ||
            moby->command < scale_game_frames(15)) {
            set_moby_color(moby, 0xC8, 0x40, 0x40);
        } else {
            moby->spawn_frame = crate_vars->unkB8;
        }
        if (moby->command == 1 || D_001413DC == 0x16) {
            moby->spawn_frame = crate_vars->unkB8;
            break_crate(moby, 1, NULL);
            crate_drop_bolts(moby);
        }
        moby->command--;
        break;

    case CRATE_STATE_REACTIVATION_DELAY:
        if (FUN_L00_001fefc8(&moby->command)) {
            moby->state = 1;
            moby->flags &= ~1;
        }
        break;
    }
    moby->hit_slot = 0xFF;
}
