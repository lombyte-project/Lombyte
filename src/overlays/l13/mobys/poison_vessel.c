/* Level 13 poison vessel moby class and its destruction effects. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/l13/mobys/poison_vessel_fragment.h"
#include "rnc/overlay/l13/mobys/poison_vessel_globals.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float D_0015ED6C;

extern int random_integer_below(int) __asm__("FUN_00213260");

extern char D_L13_001670C0[];

typedef struct {
    int v[6];
} L13EffectColors;

L13EffectColors l13_poison_vessel_particle_first_color_palette __asm__("D_L13_001F5560") = {
    {0x4f00ff8f, 0x4f00ff8f, 0x4f00ff7f, 0x4f00ff6f, 0x2fffffff, 0x2fffffff}
};

L13EffectColors l13_poison_vessel_particle_second_color_palette __asm__("D_L13_001F5578") = {
    {0x2f007f5f, 0x2f007f4f, 0x2f007f3f, 0x2f004f00, 0x2f000000, 0x3f000000}
};

extern void spawn_particle(void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");

extern char *spawn_randomly_rotated_colored_moby(void *,
                                                 void *,
                                                 void *,
                                                 float,
                                                 int,
                                                 unsigned char,
                                                 unsigned char,
                                                 unsigned char,
                                                 int) __asm__("FUN_L00_002df808");

void spawn_poison_vessel_destruction_effects(struct Moby *moby,
                                             void        *vars) __asm__("FUN_L13_003073c8");

void spawn_poison_vessel_destruction_effects(struct Moby *moby, void *vars) {
    struct {
        OvlVec4 direction, velocity, axis, offset, pos, random_direction, random_vector;
    } work;
    int   count;
    int   duration;
    int  *color, *second_color;
    float distance, twice_distance, extra, speed, amount;
    float scale;
    work.axis.q = 0;
    work.axis.f[2] = D_0015ED6C + D_0015ED6C;
    work.velocity.q = work.axis.q;
    work.axis.q = 0;
    scale = 1.5f;
    work.axis.f[2] = 1.0f;
    /* Retail offsets the copied position before emitting the effects. */
    qcopy(&work.pos, &moby->pos);
    work.pos.f[2] += 1.0f;
    fast_vec_scale(&work.velocity, &work.velocity, scale);
    fast_vec_sub(&work.direction, D_L13_001670C0, &work.pos);
    distance = fast_vec_length(&work.direction);
    {
        int i;
        for (i = 0; i < 10; i++) {
            int      lo, hi, duration;
            OvlVec4 *random_direction, *offset, *axis;
            work.random_vector.q = 0;
            work.random_vector.f[0] = random_float_between(-1.0f, 1.0f);
            work.random_vector.f[1] = random_float_between(-1.0f, 1.0f);
            random_direction = &work.random_direction;
            axis = &work.axis;
            offset = &work.offset;
            random_direction->q = work.random_vector.q;
            amount = fast_vec_dot(random_direction, axis);
            fast_vec_normalize(offset, axis, amount);
            fast_vec_sub(random_direction, random_direction, offset);
            fast_vec_normalize(offset, axis, random_float_between(0.0f, 1.0f));
            fast_vec_add(random_direction, random_direction, offset);
            fast_vec_normalize(random_direction,
                               random_direction,
                               scale * random_float_between(3.5f, 6.5f) * D_0015ED6C);
            fast_vec_add(random_direction, random_direction, &work.velocity);
            lo = scale_game_frames(60);
            hi = scale_game_frames(120);
            duration = random_int_between(lo, hi);
            spawn_poison_vessel_fragment(&work.pos, random_direction, duration, 0);
        }
    }
    twice_distance = distance + distance;
    {
        int i;
        for (i = 0; i < 4; i++) {
            int      lo, hi, duration;
            OvlVec4 *random_direction, *axis, *offset;
            random_direction = &work.random_direction;
            axis = &work.axis;
            work.random_vector.q = 0;
            work.random_vector.f[0] = random_float_between(-1.0f, 1.0f);
            work.random_vector.f[1] = random_float_between(-1.0f, 1.0f);
            offset = &work.offset;
            random_direction->q = work.random_vector.q;
            fast_vec_normalize(offset, axis, fast_vec_dot(random_direction, axis));
            fast_vec_sub(random_direction, random_direction, offset);
            fast_vec_normalize(offset, axis, random_float_between(0.0f, 1.0f));
            fast_vec_add(random_direction, random_direction, offset);
            fast_vec_normalize(random_direction,
                               random_direction,
                               scale * random_float_between(6.5f, 10.0f) * D_0015ED6C);
            fast_vec_add(random_direction, random_direction, &work.velocity);
            lo = scale_game_frames(60);
            hi = scale_game_frames(90);
            duration = random_int_between(lo, hi);
            spawn_poison_vessel_fragment(&work.pos, random_direction, duration, 1);
        }
    }
    work.random_vector.q = 0;
    work.random_vector.f[0] = random_float_between(-1.0f, 1.0f);
    work.random_vector.f[1] = random_float_between(-1.0f, 1.0f);
    work.random_vector.f[2] = random_float_between(-1.0f, 1.0f);
    work.direction.f[2] += distance * 0.5f;
    work.random_direction.q = work.random_vector.q;
    fast_vec_normalize(
        &work.random_direction, &work.random_direction, (distance / 5.0f) * D_0015ED6C);
    fast_vec_normalize(&work.direction, &work.direction, twice_distance * D_0015ED6C);
    fast_vec_add(&work.random_direction, &work.random_direction, &work.direction);
    clamp_vector_length(&work.random_direction, &work.random_direction, D_0015ED6C * 10.0f);
    {
        int lo = scale_game_frames(60);
        int hi = scale_game_frames(90);
        duration = random_int_between(lo, hi);
    }
    spawn_poison_vessel_fragment(&work.pos, &work.random_direction, duration, 1);
    count = distance < 6.0f ? truncate_float_to_s32(distance) + 1 : 4;
    extra = 0.0f;
    if (distance < 7.0f)
        extra = 7.0f - distance;
    if (count > 0) {
        int             i = count;
        L13EffectColors colors __attribute__((aligned(16)));
        L13EffectColors second_colors __attribute__((aligned(16)));
        float           magnitude = 400000.0f;
        float           amount;
        do {
            int lo, hi, duration, duration2;
            i--;
            speed = (scale * random_float_between(8.0f, 10.0f)) * D_0015ED6C - extra * D_0015ED6C;
            colors = l13_poison_vessel_particle_first_color_palette;
            second_colors = l13_poison_vessel_particle_second_color_palette;
            amount = scale * magnitude;
            color = &colors.v[random_integer_below(6)];
            second_color = &second_colors.v[random_integer_below(6)];
            {
                int lo = scale_game_frames(15);
                int hi = scale_game_frames(20);
                duration = random_int_between(lo, hi);
            }
            {
                int lo = scale_game_frames(25);
                int hi = scale_game_frames(30);
                duration2 = random_int_between(lo, hi);
            }
            spawn_particle(&work.pos,
                           &work.velocity,
                           *color,
                           *second_color,
                           amount,
                           duration,
                           duration2,
                           0,
                           0,
                           speed);
        } while (i != 0);
    }
    if (distance > 9.0f) {
        float amount;
        amount = scale * 4.0f;
        duration = scale_game_frames(15);
        spawn_randomly_rotated_colored_moby(
            moby, &work.pos, &work.velocity, amount, duration, 127, 127, 127, 32);
        duration = scale_game_frames(24);
        spawn_randomly_rotated_colored_moby(
            moby, &work.pos, &work.velocity, amount, duration, 32, 127, 0, 32);
    }
    {
        float amount;
        amount = scale * 4.0f;
        duration = scale_game_frames(20);
        spawn_randomly_rotated_colored_moby(
            moby, &work.pos, &work.velocity, amount, duration, 63, 127, 0, 48);
    }
    {
        float amount;
        amount = scale * 3.5f;
        duration = scale_game_frames(27);
        spawn_randomly_rotated_colored_moby(
            moby, &work.pos, &work.velocity, amount, duration, 16, 96, 0, 64);
    }
    {
        float amount;
        amount = scale * 3.0f;
        duration = scale_game_frames(29);
        spawn_randomly_rotated_colored_moby(
            moby, &work.pos, &work.velocity, amount, duration, 0, 32, 0, 32);
    }
}

struct BlastHit {
    u8  pad0[0x2C];
    f32 damage;
};

struct BlastVars {
    u8  pad0[0x20];
    f32 health;
    u8  pad24[2];
    s16 effect_timer;
    u8  pad28;
    u8  hit;
    u8  pad2A[0x36];
    u8  effects[7];
    u8  effect_level;
    u8  pad68[8];
    int timer;
};

char D_L13_001F5590[] = "Moby class %d UID:%d  Damage Received: %f\n";

extern char sphere_hits_ed158[] __asm__("D_L13_00178200");

extern int poison_vessel_blast_duration_frames_gp __asm__("D_L13_00161E2C") __attribute__((sda));

extern f32 poison_vessel_blast_start_radius_gp __asm__("D_L13_00161E30") __attribute__((sda));

extern f32 poison_vessel_blast_end_radius_gp __asm__("D_L13_00161E34") __attribute__((sda));

extern f32 poison_vessel_blast_damage_gp __asm__("D_L13_00161E38") __attribute__((sda));

extern int fptodp(float) __asm__("FUN_00120478");

extern void debug_print() __asm__("FUN_001e93b0");

extern int
fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");

extern void stop_moby_sound_7b10(void *, int, int) __asm__("FUN_L00_00257470");

extern void trooper_anim_update(struct Moby *, void *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(struct Moby *, void *) __asm__("FUN_L00_0025d538");

extern void spawn_type_52_particle_near_moby(struct Moby *) __asm__("FUN_L10_002e3de8");

extern float interpolate_scalar_linear(float, float, float) __asm__("FUN_L00_0025b6a8");

extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");

extern void send_blast_hits(struct Moby *,
                            void *,
                            void *,
                            int,
                            int,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float) __asm__("FUN_L00_0025a9f8");

extern float scale_time(float) __asm__("FUN_001f96b0");

void poison_vessel_update(struct Moby *moby) __asm__("FUN_L13_00307b10");

void poison_vessel_update(struct Moby *moby) {
    enum {
        POISON_VESSEL_INTACT = 0,
        POISON_VESSEL_BLASTING = 1
    };
    enum {
        HIT_RESULT_NONE = 0,
        HIT_RESULT_DEATH = 1,
        HIT_RESULT_DYING_KNOCKBACK = 2,
        HIT_RESULT_STUN_KNOCKBACK = 3,
        HIT_RESULT_NO_REACTION = 11
    };
    struct BlastVars *vars = (struct BlastVars *)moby->pvars;
    struct BlastHit  *hit;
    float            *health;
    void             *effects;
    int               kind;
    float             pos[4];
    int               hits;
    int               timer;

    switch (moby->state) {
    case POISON_VESSEL_INTACT:
        vars->hit = 0;
        moby->rot.x *= 0.7f;
        moby->rot.y *= 0.7f;
        hit = find_moby_hit(moby, 0x330001, 0);
        health = &vars->health;
        if (hit != 0) {
            debug_print(D_L13_001F5590, moby->oclass, (s16)moby->save_id, fptodp(hit->damage));
        }
        switch (fun_00213928(moby, hit, health, 0, &kind, 0, 0, 4)) {
        case HIT_RESULT_DEATH:
        case HIT_RESULT_DYING_KNOCKBACK:
            *health = 0;
            break;
        case HIT_RESULT_NONE:
        case HIT_RESULT_STUN_KNOCKBACK:
        case HIT_RESULT_NO_REACTION:
            break;
        }
        effects = vars->effects;
        if (kind >= 2) {
            if (*health <= hit->damage) {
                *health = 0;
                moby->flags &= 0xEFFF;
                stop_moby_sound_7b10(moby, 0, -1);
                vars->effect_level = 0x78;
                trooper_anim_update(moby, effects);
                vars->timer = 0;
                spawn_poison_vessel_destruction_effects(moby, vars);
                spawn_type_52_particle_near_moby(moby);
                allocate_voice_for_target_entry(0, 0, (int)moby);
                moby->state = 1;
            } else {
                *health = *health - hit->damage;
                vars->effect_level = 0xFA;
                vars->effect_timer = scale_game_frames(0x3C);
                trooper_anim_update(moby, effects);
                moby->rot.x = random_float_between(-0.034906585f, 0.034906585f);
                moby->rot.y = random_float_between(-0.034906585f, 0.034906585f);
            }
        }
        moby->hit_slot = 0xFF;
        trooper_anim_finish(moby, effects);
        break;
    case POISON_VESSEL_BLASTING:
        qcopy(pos, &moby->pos);
        pos[2] += 1.0f;
        hits = collect_mobies_in_sphere(
            interpolate_scalar_linear(poison_vessel_blast_start_radius_gp,
                                      poison_vessel_blast_end_radius_gp,
                                      convert_integer_to_float(vars->timer) /
                                          convert_integer_to_float(scale_game_frames(
                                              poison_vessel_blast_duration_frames_gp))),
            pos,
            0x10,
            moby,
            0);
        send_blast_hits(moby,
                        pos,
                        sphere_hits_ed158,
                        hits,
                        0,
                        0x830001,
                        2,
                        1,
                        poison_vessel_blast_damage_gp,
                        1.0f,
                        1.0f);
        if (vars->timer > scale_time((float)poison_vessel_blast_duration_frames_gp * 0.5f)) {
            moby->flags |= 0x41;
            spawn_type_52_particle_near_moby(moby);
        } else {
            moby->alpha = moby->alpha * 7 / 8;
        }
        timer = vars->timer + 1;
        vars->timer = timer;
        if (scale_game_frames(poison_vessel_blast_duration_frames_gp) < timer) {
            spawn_type_52_particle_near_moby(moby);
            mark_moby_for_removal(moby);
        }
        break;
    }
}
