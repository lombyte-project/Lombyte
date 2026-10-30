/* Shared moby class rock: debris chunks that fall, bounce and burst into sparks, and the function that spawns them. */

#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/collision.h"

/* pvars of the debris moby spawn_debris_chunk spawns */
typedef struct {
    Vec4f vel;     /* 0x00 */
    s32   unk10;   /* 0x10 */
    f32   scale;   /* 0x14 */
    f32   spin;    /* 0x18: random spin, random sign */
    s32   life;    /* 0x1C: frames */
    s32   unk20;   /* 0x20 */
    f32   gravity; /* 0x24 */
    s32   unk28;   /* 0x28 */
} DebrisVars;

extern s32 rand(void) __asm__("FUN_001160d8");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern s32 coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern s32 FUN_L00_00257b90(s32, s32);

extern f32          D_0015ED6C_2f8530 __asm__("D_0015ED6C");
extern CollisionHit collision_hit __asm__("D_L01_001742C0");

struct Moby *spawn_debris_chunk(Vec4f *pos,
                                Vec4f *vel,
                                s32    oclass,
                                s32    initial_timer,
                                s32    fade_out,
                                f32    scale,
                                f32    gravity,
                                f32    spin,
                                f32    seconds) __asm__("FUN_L01_002f8530");

struct Moby *spawn_debris_chunk(Vec4f *pos,
                                Vec4f *vel,
                                s32    oclass,
                                s32    initial_timer,
                                s32    fade_out,
                                f32    scale,
                                f32    gravity,
                                f32    spin,

                                f32 seconds) {
    u128         spawn_position_snapshot;
    u128         spawn_velocity_snapshot;
    u128        *pp = &spawn_position_snapshot;
    u128        *vp = &spawn_velocity_snapshot;
    struct Moby *debris_moby;
    DebrisVars  *debris_vars;
    f32          random_spin_magnitude;
    f32          initial_spin;

    spawn_position_snapshot = *(u128 *)pos;
    spawn_velocity_snapshot = *(u128 *)vel;
    debris_moby = create_moby(oclass);
    if (debris_moby != 0) {
        debris_vars = (DebrisVars *)debris_moby->pvars;
        copy_hero_moby_field_38(debris_moby);
        debris_moby->scale *= scale;
        debris_moby->update_dist = 0xFF;
        debris_moby->draw_dist = 0xFF;
        debris_moby->visible = 1;
        debris_moby->rot.x = random_float_between(-3.1415927f, 3.1415927f);
        debris_moby->rot.y = random_float_between(-3.1415927f, 3.1415927f);
        debris_moby->rot.z = random_float_between(-3.1415927f, 3.1415927f);
        qcopy(&debris_moby->pos, pp);
        qcopy(&debris_vars->vel, vp);
        random_spin_magnitude =
            random_float_between(D_0015ED6C_2f8530 * 1.5707964f, D_0015ED6C_2f8530 * 6.2831855f);
        if (rand() & 1) {
            initial_spin = -random_spin_magnitude * spin;
        } else {
            initial_spin = random_spin_magnitude * spin;
        }
        debris_vars->unk10 = initial_timer;
        debris_vars->unk28 = initial_timer;
        debris_vars->scale = scale;
        debris_vars->spin = initial_spin;
        debris_vars->life = truncate_float_to_s32(scale_time(seconds * 60.0f));
        debris_vars->unk20 = fade_out;
        debris_vars->gravity = gravity * 9.8f * frame_time_sq;
        update_entity(debris_moby);
    }
    return debris_moby;
}

/* Debris update: falls and spins, bounces off what it hits with a random kick, bursts into 40 sparks once it
   comes to rest (or its unk10 timer runs out), or fades out over unk28 frames when unk20 is set. */
void rock_update(struct Moby *m) __asm__("FUN_L01_002f8718");

void rock_update(struct Moby *debris_moby) {
    DebrisVars *debris_vars = (DebrisVars *)debris_moby->pvars;
    Vec4        old;
    Vec4        bounce_or_particle_velocity;
    Vec4        particle_position;
    Vec4        particle_offset_direction;
    f32         velocity_heading;
    f32         spin;
    s32         i;

    qcopy(&old, &debris_moby->pos);
    debris_vars->vel.z -= debris_vars->gravity;
    fast_vec_add(&debris_moby->pos, &debris_moby->pos, &debris_vars->vel);
    debris_moby->rot.z = fast_add_rotations(debris_moby->rot.z, debris_vars->spin);
    if (debris_moby->pos.x < 2.0f || 1021.0f < debris_moby->pos.x || debris_moby->pos.y < 2.0f ||
        1021.0f < debris_moby->pos.y || debris_moby->pos.z < 2.0f || 1021.0f < debris_moby->pos.z) {
        mark_moby_for_removal(debris_moby);
        return;
    }
    if (fast_dec_timer(&debris_vars->life)) {
        if (collision_line(&old, &debris_moby->pos, 0, debris_moby, 0) != 0 ||
            coll_sphere(debris_vars->scale, &debris_moby->pos, 0, debris_moby) != 0) {
            velocity_heading = fast_arc_tan(debris_vars->vel.x, debris_vars->vel.y);
            bounce_or_particle_velocity.f[0] =
                fast_cos(fast_add_rotations(velocity_heading,
                                            random_float_between(-3.1415927f, 3.1415927f))) *
                random_float_between(0.0f, debris_vars->vel.z * 0.35f);
            bounce_or_particle_velocity.f[1] =
                fast_sin(fast_add_rotations(velocity_heading,
                                            random_float_between(-3.1415927f, 3.1415927f))) *
                random_float_between(0.0f, debris_vars->vel.z * 0.35f);
            bounce_or_particle_velocity.f[2] = 0.0f;
            debris_moby->rot.x = random_float_between(-3.1415927f, 3.1415927f);
            debris_moby->rot.y = random_float_between(-3.1415927f, 3.1415927f);
            debris_moby->rot.z = random_float_between(-3.1415927f, 3.1415927f);
            reflect_vector(&debris_vars->vel, &debris_vars->vel, &collision_hit.normal_x);
            fast_vec_scale(
                &debris_vars->vel, &debris_vars->vel, random_float_between(0.07f, 0.13f));
            fast_vec_add(&debris_vars->vel, &debris_vars->vel, &bounce_or_particle_velocity);
            fast_vec_normalize(&collision_hit.normal_x, &collision_hit.normal_x, 0.1f);
            fast_vec_add(&debris_moby->pos, &collision_hit.normal_x, &collision_hit.point);
            spin = random_float_between(frame_time * 1.5707964f, frame_time * 6.2831855f);
            if (0.0f < debris_vars->spin) {
                spin = -spin;
            }
            debris_vars->spin = spin;
        }
    }
    if (debris_vars->unk20 == 0) {
        if (fast_dec_timer(&debris_vars->unk10) == 0) {
            if (!(fast_vec_length(&debris_vars->vel) < frame_time * 0.5f)) {
                return;
            }
            if (debris_vars->life != 0) {
                return;
            }
        }
        for (i = 0; i < 40; i++) {
            particle_position.q = 0;
            particle_position.f[0] = random_float_between(-1.0f, 1.0f);
            particle_position.f[1] = random_float_between(-1.0f, 1.0f);
            particle_position.f[2] = random_float_between(-1.0f, 1.0f);
            particle_offset_direction.q = 0;
            bounce_or_particle_velocity.q = particle_position.q;
            particle_offset_direction.f[0] = random_float_between(-1.0f, 1.0f);
            particle_offset_direction.f[1] = random_float_between(-1.0f, 1.0f);
            particle_offset_direction.f[2] = random_float_between(-1.0f, 1.0f);
            particle_position.q = particle_offset_direction.q;
            fast_vec_normalize(
                &particle_position, &particle_position, random_float_between(0.0f, 0.5f));
            fast_vec_add(&particle_position, &particle_position, &debris_moby->pos);
            fast_vec_normalize(&bounce_or_particle_velocity,
                               &bounce_or_particle_velocity,
                               random_float_between(0.0f, 0.5f) * frame_time);
            fast_vec_add(
                &bounce_or_particle_velocity, &bounce_or_particle_velocity, &debris_vars->vel);
            spawn_effect_particle_with_target_scalar(&particle_position,
                                                     &bounce_or_particle_velocity,
                                                     0x1F0C1820,
                                                     0x81020,
                                                     random_float_between(70000.0f, 140000.0f),
                                                     scale_game_frames(FUN_L00_00257b90(20, 60)));
        }
        mark_moby_for_removal(debris_moby);
    } else if (fast_dec_timer(&debris_vars->unk10) != 0) {
        mark_moby_for_removal(debris_moby);
    } else {
        debris_moby->alpha = debris_vars->unk10 * 127 / debris_vars->unk28;
    }
}
