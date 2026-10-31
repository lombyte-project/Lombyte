/* Level 01 moby class smoke_emitter. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"

extern s32  FUN_0022da68(s32, s32, void *);
extern int  FUN_L00_00257b90(int lo, int hi);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");

#include "rnc/overlay/cuboid.h"

extern float random_float_between(float, float);
extern int   scale_game_frames(int);

typedef struct {
    s32 sound;
} SmokeVars;

typedef struct {
    u8         pad0[0x10];
    u128       pos;
    u8         pad20[0x58];
    SmokeVars *vars;
    u8         pad7c[0x44];
    u128       mtx[4];
} SmokeMoby;

void smoke_emitter_update(SmokeMoby *self) __asm__("FUN_L01_002ffb28");

/* Moby class 779 */
void smoke_emitter_update(SmokeMoby *smoke_emitter_moby) {
    SmokeVars *smoke_vars;
    Vec4       pos;
    Vec4       particle_velocity;
    Vec4       local_velocity;
    f32        size;
    s32        i;

    smoke_vars = smoke_emitter_moby->vars;
    if (voice_is_playing(smoke_emitter_moby, smoke_vars->sound) == 0) {
        smoke_vars->sound = FUN_0022da68(0, 4, smoke_emitter_moby);
    }
    for (i = 0; i < 2; i++) {
        pos.q = 0;
        local_velocity.q = 0;
        pos.f[0] = -0.2f;
        pos.f[2] = 1.2f;
        local_velocity.f[0] = random_float_between(0.8f, 1.2f) * (frame_time * 3.0f);
        local_velocity.f[2] = random_float_between(0.8f, 1.2f) * (frame_time + frame_time);
        particle_velocity.q = local_velocity.q;
        transform_vector_by_basis(&particle_velocity, &particle_velocity, smoke_emitter_moby->mtx);
        fast_vec_add(&pos, &pos, &smoke_emitter_moby->pos);
        size = random_float_between(125000.0f, 175000.0f);
        spawn_effect_particle_with_target_scalar(&pos,
                                                 &particle_velocity,
                                                 0x207F7F7F,
                                                 0x272727,
                                                 size,
                                                 scale_game_frames(FUN_L00_00257b90(0x2D, 0x3C)));
    }
}

#include "qcopy.h"
