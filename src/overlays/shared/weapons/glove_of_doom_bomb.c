/* Shared moby class bomb (0xE6): the glove of doom canister that lands and spawns doom bots. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/math/fast_math.h"

/* Vars of a thrown moby, as ThrowVars in trespasser.c without its first 0x10 bytes. */
typedef struct {
    Vec4         vel;          /* 0x00: velocity per step */
    Vec4         landing;      /* 0x10: predicted landing point */
    Vec4         normal;       /* 0x20: surface normal there */
    struct Moby *holder;       /* 0x30: moby it was thrown from */
    s32          effect_timer; /* 0x34 */
    s16          timer;        /* 0x38 */
    s16          unk46;
    s32          spawn_count; /* 0x3C */
} ThrowVars;

extern s32              D_L00_0015F5C4; /* nonzero: no throws */
extern float D_0015ED6C MACRO_ADDR;     /* frame-rate scale */
extern float D_0015ED70 MACRO_ADDR;
extern CollisionHit     coll_hit __asm__("D_L00_00173E40") NOT_SDA;

extern Vec4 g_debug_camera_lui __asm__("D_L00_00166DC0") NOT_SDA;
extern void add_vector_xyz_2c8440(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern s32  collision_line_2c8440(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void FUN_L00_002c7df8_2c8440(void) __asm__("FUN_L00_002c7df8");
extern unsigned char *FUN_L00_002603d0_2c8440(int) __asm__("FUN_L00_002603d0");
extern s32            FUN_001f0b58_2c8440(void) __asm__("FUN_001f0b58");

extern u8 D_0013E534 NOT_SDA;
extern int           D_L00_0015FFE4;
extern s32           coll_line_fix(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern s32           coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void          reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void          add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern char         *FUN_L00_002603d0(char *);
extern void          spawn_moby_bound_effect_particle(struct Moby *) __asm__("FUN_L00_0026ed28");
extern void          spawn_moby_collision_particles(struct Moby *) __asm__("FUN_L00_002c8078");
extern void          spawn_doom_bot(struct Moby *, void *, void *) __asm__("FUN_L00_002bfe08");
extern void          FUN_L00_00260400(struct Moby *, s32, void *, void *, void *, void *);

/* coll_hit_2c8440's normal as a whole quadword (CollisionHit names only x/y/z) */

/*
 * Predicts where a thrown moby lands: steps the velocity (with gravity) from
 * its start until collision_line_2c8440 hits ground (or a moby other than the hero
 * and the holder), for up to `timer` steps. Stores the landing point and
 * normal in vars and pulls the point 5% toward g_debug_camera_lui. mode 0 needs
 * a live hit moby; mode 1 starts from the hero's hand. Returns 1 on a hit.
 */

s32 predict_doom_bot_spawner_landing(ThrowVars   *throw_vars,
                                     struct Moby *thrown_moby,
                                     s32          mode) __asm__("FUN_L00_002c8440");

s32 predict_doom_bot_spawner_landing(ThrowVars *throw_vars, struct Moby *thrown_moby, s32 mode) {
    Vec4 simulated_position, simulated_velocity, position_scratch, q, landing_probe_position;
    s32  prediction_timer;
    s32  found = 0;
    s32  i;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4 != 0)
        return 0;
    qcopy(&position_scratch, &thrown_moby->pos);
    if (thrown_moby->state == 1) {
        simulated_position.f[0] =
            fast_cos(fast_add_rotations(-0.36196801066398620605f,
                                        fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.85903900861740112305f;
        simulated_position.f[1] =
            fast_sin(fast_add_rotations(-0.36196801066398620605f,
                                        fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.85903900861740112305f;
        simulated_position.f[2] = 0.0f;
        add_vector_xyz_2c8440(&simulated_position, &simulated_position, &hero.motion.pos);
        simulated_position.f[2] += 0.39081999659538269043f;
        qcopy(&simulated_velocity, &throw_vars->vel);
        if (hero.state.control_mode == 15)
            add_vector_xyz_2c8440(
                &simulated_velocity, &simulated_velocity, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0_2c8440((int)hero.ground_moby) && mode)
            add_vector_xyz_2c8440(
                &simulated_velocity, &simulated_velocity, &hero.motion.displacement);
        prediction_timer = scale_game_frames(300);
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
    if (collision_line_2c8440(&q, &simulated_position, 0x10, hero.moby, 0) &&
        (FUN_001f0b58_2c8440() || 0.0f < simulated_velocity.f[2])) {
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby != hero.moby &&
                 (struct Moby *)coll_hit.moby != throw_vars->holder) ||
                prediction_timer < scale_game_frames(300) - scale_game_frames(10)) {
                landing_probe_position.q = 0;
                landing_probe_position.f[2] = 0.5f;
                qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
                found = 1;
                add_vector_xyz_2c8440(
                    &landing_probe_position, &landing_probe_position, &throw_vars->landing);
                probe_ground_height(&landing_probe_position, 0, 0.5f);
            }
        } else if (coll_hit.unk1C > 0) {
            qcopy(&throw_vars->landing, &coll_hit.point);
            found = 1;
        }
    }
    while (!fast_dec_timer(&prediction_timer) && !found) {
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
            if (((struct Moby *)coll_hit.moby == hero.moby ||
                 (struct Moby *)coll_hit.moby == throw_vars->holder) &&
                !(prediction_timer < scale_game_frames(300) - scale_game_frames(10)))
                continue;
            landing_probe_position.q = 0;
            landing_probe_position.f[2] = 0.5f;
            qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
            add_vector_xyz_2c8440(
                &landing_probe_position, &landing_probe_position, &throw_vars->landing);
            probe_ground_height(&landing_probe_position, 0, 0.5f);
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
        add_draw_callback(FUN_L00_002c7df8_2c8440, thrown_moby);
    throw_vars->normal.q = coll_hit_normal.q;
    fast_vec_sub(&position_scratch, &throw_vars->landing, &g_debug_camera_lui);
    fast_vec_scale(&position_scratch, &position_scratch, 0.94999998807907104492f);
    add_vector_xyz_2c8440(&throw_vars->landing, &position_scratch, &g_debug_camera_lui);
    return 1;
ret0:
    /* a second return 0 block, as in retail */
    return 0;
}

/* Six checked switch entries: state 5 shares the default state reset.
 * Case 2/3 shaped after rac1-decomp (nonmatching/shared/func_L00_002C9DC8.c). */

void update_doom_bot_spawner(struct Moby *thrown_moby) __asm__("FUN_L00_002c89e8");

void update_doom_bot_spawner(struct Moby *thrown_moby) {
    enum {
        THROWN_MOBY_STATE_INITIALIZE = 0,
        THROWN_MOBY_STATE_GROW_TO_QUARTER_SCALE = 1,
        THROWN_MOBY_STATE_MOVE_WITH_COLLISION = 2,
        THROWN_MOBY_STATE_SPAWN_SEQUENCE = 3,
        THROWN_MOBY_STATE_FADE_OUT = 4,
        THROWN_MOBY_STATE_RESET_TO_INITIALIZE = 5
    };
    Vec4         next_pos, start, end, probe;
    ThrowVars   *vars;
    struct Moby *other;
    s32          live_count;
    s32          settled;
    s32          settled_frames;
    u8          *collision_point;
    /* Fakematch: hidden zero; the dead `last` keeps the 0xBA count loop's
     * count++ a branch (jump would turn a one-insn then-block into movn). */
    s32          never = 0;
    struct Moby *last;

    if (!thrown_moby)
        return;
    vars = (ThrowVars *)thrown_moby->pvars;
    if (!vars)
        return;
    if (thrown_moby->pos.x < 2.0f || 1021.0f < thrown_moby->pos.x || thrown_moby->pos.y < 2.0f ||
        1021.0f < thrown_moby->pos.y || thrown_moby->pos.z < 2.0f || 1021.0f < thrown_moby->pos.z)
        goto remove;
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
            if (!hero.unk20A5)
                spawn_moby_bound_effect_particle(thrown_moby);
        }
        if (!hero.vehicle_mode && vars->holder && (vars->holder->state & 0xf0) != 0xf0 &&
            hero.items[0].moby && hero.items[0].moby->oclass == 0xe5)
            return;
        goto remove;
    }
    case THROWN_MOBY_STATE_MOVE_WITH_COLLISION: {
        f32          size, speed, rest_speed, sink;
        f32          half = 0.5f;
        u8          *carry;
        struct Hero *player;
        size = convert_integer_to_float(D_0013E534) + 1.0f;
        {
            f32 class_scale = thrown_moby->pclass->scale;
            if (thrown_moby->scale < class_scale * half * size)
                thrown_moby->scale =
                    thrown_moby->scale + class_scale * (frame_scale * 0.05f) * size;
        }
        if (fast_dec_timer(&vars->effect_timer)) {
            vars->effect_timer = scale_game_frames(4);
            spawn_moby_bound_effect_particle(thrown_moby);
        }
        vars->vel.f[2] -= frame_time_sq * 9.0f;
        fast_vec_add(&next_pos, &thrown_moby->pos, vars);
        carry = (u8 *)&hero.motion.platform_carry;
        add_vectors(&probe, &thrown_moby->pos, carry);
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
            if (speed < frame_scale * rest_speed)
                goto settle;
        } else {
            if (!coll_line_fix(&start, &end, 0, player->moby, 0)) {
                goto collision_miss;
            }
            collision_point = (u8 *)&coll_hit.point;
            qcopy_nc(&thrown_moby->pos, collision_point);
            thrown_moby->pos.z +=
                (frame_scale * 0.15f * thrown_moby->scale) / thrown_moby->pclass->scale;
            collision_point += 0x20;
            rest_speed = 0.01f;
            reflect_vector(vars, vars, collision_point);
            fast_vec_scale(vars, vars, half);
            speed = fast_vec_length(vars);
            if (speed < frame_scale * rest_speed)
                goto settle;
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
            FUN_L00_00260400(thrown_moby,
                             coll_hit.moby,
                             collision_point,
                             &thrown_moby->rot,
                             collision_point,
                             &thrown_moby->rot);
        if (fast_dec_timer_s16(&vars->timer)) {
            live_count = 0;
            for (other = (struct Moby *)D_L00_0015FFE4; other; other = other->next) {
                if (other->oclass == 0xba) {
                    /* Fakematch: dead `last = other` (see `never`). */
                    if ((other->state & 0xf0) != 0xf0) {
                        live_count++;
                        last = other;
                    }
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
                    if (spawned > 3)
                        thrown_moby->state = 4;
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
            FUN_L00_00260400(thrown_moby,
                             coll_hit.moby,
                             &thrown_moby->pos,
                             &thrown_moby->rot,
                             &thrown_moby->pos,
                             &thrown_moby->rot);
        if (thrown_moby->alpha)
            return;
    remove:
        mark_moby_for_removal(thrown_moby);
        break;
    }
    case THROWN_MOBY_STATE_RESET_TO_INITIALIZE:
    default:
        thrown_moby->state = 0;
    }
}
