/* Level 01 moby class rock_formation. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"

#include "rnc/math/vector.h"
#include "rnc/overlay/l01/mobys/debris_class_choices.h"

typedef struct {
    u8  pad0[0x2C];
    f32 force;
} DebrisHit;

DebrisClassChoices l01_rock_formation_debris_oclass_choices __asm__("D_L01_00161C50") = {
    {0x2b8, 0x2b9, 0x2ba}
};

extern s32  FUN_0022da68(s32, s32, void *);
extern int  FUN_L00_00257b90(int lo, int hi);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");
extern struct Moby *
spawn_debris_chunk(void *, void *, s32, f32, s32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");
extern void FUN_L00_00257470(void *, s32, s32);

/* Once hit hard enough: bursts into 200 smoke puffs and 40 debris chunks, then removes itself. */
void rock_formation_update(struct Moby *m) __asm__("FUN_L01_002f9810");

/* Moby class 704 */
void rock_formation_update(struct Moby *rock_formation_moby) {
    DebrisHit *hit;
    s32        i;
    s32        j;
    f32        offset_component_or_particle_size;

    hit = find_moby_hit(rock_formation_moby, 0x10000, 0);
    rock_formation_moby->hit_slot = 0xFF;
    if (hit != NULL && hit->force > 0.0f) {
        FUN_0022da68(0, 0x10, rock_formation_moby);
        for (i = 0; i < 200; i++) {
            /* A block of its own: retail reuses these stack slots in the next loop. */
            {
                Vec4 vel;
                Vec4 pos;
                Vec4 off;

                pos.q = 0;
                pos.f[0] = random_float_between(-1.0f, 1.0f);
                pos.f[1] = random_float_between(-1.0f, 1.0f);
                pos.f[2] = random_float_between(-1.0f, 1.0f);
                off.q = 0;
                vel.q = pos.q;
                offset_component_or_particle_size = random_float_between(-1.0f, 1.0f);
                offset_component_or_particle_size *= fast_cos(rock_formation_moby->rot.z);
                off.f[0] = offset_component_or_particle_size;
                offset_component_or_particle_size = random_float_between(-0.3f, 0.3f);
                offset_component_or_particle_size *= fast_sin(rock_formation_moby->rot.z);
                off.f[1] = offset_component_or_particle_size;
                off.f[2] = random_float_between(0.0f, 2.0f);
                pos.q = off.q;
                fast_vec_add(&pos, &pos, &rock_formation_moby->pos);
                fast_vec_normalize(&vel, &vel, random_float_between(0.5f, 1.5f) * frame_time);
                offset_component_or_particle_size = random_float_between(0.5f, 1.0f) * 210000.0f;
                spawn_effect_particle_with_target_scalar(
                    &pos,
                    &vel,
                    0x5F787878,
                    0x181818,
                    offset_component_or_particle_size,
                    scale_game_frames(FUN_L00_00257b90(30, 90)));
            }
        }
        for (j = 0; j < 40; j++) {
            DebrisClassChoices oclass_choices;
            Vec4               vel;
            Vec4               pos;
            Vec4               off;

            oclass_choices = l01_rock_formation_debris_oclass_choices;
            pos.q = 0;
            pos.f[0] = random_float_between(-1.0f, 1.0f);
            pos.f[1] = random_float_between(-1.0f, 1.0f);
            pos.f[2] = random_float_between(-1.0f, 1.0f);
            off.q = 0;
            vel.q = pos.q;
            offset_component_or_particle_size = random_float_between(-1.2f, 1.2f);
            offset_component_or_particle_size *= fast_cos(rock_formation_moby->rot.z);
            off.f[0] = offset_component_or_particle_size;
            offset_component_or_particle_size = random_float_between(-0.4f, 0.4f);
            offset_component_or_particle_size *= fast_sin(rock_formation_moby->rot.z);
            off.f[1] = offset_component_or_particle_size;
            off.f[2] = random_float_between(0.5f, 2.0f);
            pos.q = off.q;
            fast_vec_add(&pos, &pos, &rock_formation_moby->pos);
            fast_vec_normalize(&vel, &vel, random_float_between(1.0f, 5.0f) * frame_time);
            spawn_debris_chunk(&pos,
                               &vel,
                               oclass_choices.oclass[random_integer_below(3)],
                               random_float_between(0.04f, 0.09f),
                               FUN_L00_00257b90(60, 180),
                               1.0f,
                               1.0f,
                               0.75f,
                               0);
        }
        FUN_L00_00257470(rock_formation_moby, 0, -1);
        mark_moby_for_removal(rock_formation_moby);
    }
}

extern float fast_cos(float);
extern float fast_sin(float);
extern float random_float_between(float, float);
extern int   scale_game_frames(int);

#include "qcopy.h"
