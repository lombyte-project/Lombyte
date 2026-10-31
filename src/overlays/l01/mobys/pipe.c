/* Level 01 moby class pipe. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"

extern int  FUN_L00_00257b90(int lo, int hi);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");

extern void update_entity() __asm__("FUN_L00_00250df8");

#include "rnc/overlay/quad.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002FABE8.c: func_L01_00300C38), where it is exact; names translated to the US level program. */

extern char *FUN_L01_002ffa90_c(void *owner) __asm__("FUN_L01_002ffa90");
extern float fast_cos(float);
extern float fast_sin(float);
extern float random_float_between(float, float);
extern int   allocate_voice_for_target_entry(int, int, int);
extern int   scale_game_frames(int);

void pipe_update(struct Moby *m) __asm__("FUN_L01_002ff860");

/* Moby class 778 */
void pipe_update(struct Moby *moby) {
    char *pipe_vars = (char *)moby->pvars;
    char *hit = find_moby_hit(moby, 0x10000, 0);
    moby->hit_slot = 0xFF;
    if (hit != 0 && 0.0f < *(float *)(hit + 0x2C)) {
        float particle_position[4];
        float particle_velocity[4];
        int   random_countdown =
            random_integer_below(*(int *)(pipe_vars + 4) - *(int *)pipe_vars + 1) +
            *(int *)pipe_vars;
        int k;
        if (random_countdown > 0) {
            do {
                random_countdown--;
            } while (random_countdown != 0);
        }
        allocate_voice_for_target_entry(0, 0, (int)moby);
        for (k = 0x95; k >= 0;) {
            float velocity_component_or_size, horizontal_speed_sample;
            *(OvlQuad *)particle_position = 0;
            particle_position[0] = -0.2f;
            particle_position[2] = 1.2f;
            k--;
            fast_vec_add(particle_position, particle_position, &moby->pos);
            velocity_component_or_size = fast_cos(random_angle_radians());
            horizontal_speed_sample = random_float_between(1.5f, 3.0f);
            velocity_component_or_size =
                velocity_component_or_size * (horizontal_speed_sample * frame_time);
            particle_velocity[0] = velocity_component_or_size;
            velocity_component_or_size = fast_sin(random_angle_radians());
            horizontal_speed_sample = random_float_between(1.5f, 3.0f);
            velocity_component_or_size =
                velocity_component_or_size * (horizontal_speed_sample * frame_time);
            particle_velocity[1] = velocity_component_or_size;
            particle_velocity[2] = 0;
            particle_velocity[2] = random_float_between(particle_velocity[2], 3.0f) * frame_time;
            velocity_component_or_size = random_float_between(125000.0f, 175000.0f);
            spawn_effect_particle_with_target_scalar(
                particle_position,
                particle_velocity,
                0x207F7F7F,
                0x272727,
                velocity_component_or_size,
                scale_game_frames(FUN_L00_00257b90(0x2D, 0x3C)));
        }
        FUN_L01_002ffa90_c(moby);
        mark_moby_for_removal(moby);
    }
}

typedef struct {
    u8   pad0[0x10];
    u128 pos;
    u8   state;
    u8   pad21[0xF];
    u8   b30;
    u8   b31;
    s16  h32;
    u8   pad34[4];
    u64  d38;
    u128 rot;
    u8   pad50[0x28];
    s32 *pvars;
} SpawnedMoby;

SpawnedMoby *
spawn_pipe_replacement_moby(SpawnedMoby *transform_source_moby) __asm__("FUN_L01_002ffa90");

SpawnedMoby *spawn_pipe_replacement_moby(SpawnedMoby *transform_source_moby) {
    SpawnedMoby *spawned_moby;
    s32         *spawned_moby_vars;

    spawned_moby = create_moby(0x30B);
    if (spawned_moby != 0) {
        spawned_moby->b30 = 0x20;
        spawned_moby->h32 = 0xFF;
        spawned_moby->b31 = 1;
        spawned_moby->state = 0;
        spawned_moby_vars = spawned_moby->pvars;
        spawned_moby->d38 = transform_source_moby->d38;
        qcopy(&spawned_moby->pos, &transform_source_moby->pos);
        qcopy(&spawned_moby->rot, &transform_source_moby->rot);
        *spawned_moby_vars = -1;
        update_entity(spawned_moby, -1);
    }
    return spawned_moby;
}
