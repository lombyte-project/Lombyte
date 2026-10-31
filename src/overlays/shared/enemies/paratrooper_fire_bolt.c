/* Paratrooper fire bolt (class 0x50C): the bolt the Blarg paratrooper fires, its spawner and its update. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00308650), where it is exact; names translated to the US level program. */

extern char *create_moby(int);

char *spawn_paratrooper_projectile(char *initial_velocity,
                                   char *spawn_position,
                                   int   collision_ignore_moby,
                                   int   tracked_moby,
                                   int   lifetime_frames,
                                   int   burst_mode) __asm__("FUN_L08_00307190");
char *spawn_paratrooper_projectile(char *initial_velocity,
                                   char *spawn_position,
                                   int   collision_ignore_moby,
                                   int   tracked_moby,
                                   int   lifetime_frames,

                                   int burst_mode) {
    char *spawned_moby = create_moby(0x50C);
    if (spawned_moby != 0) {
        char *moby_data;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x7E;
        moby_data = ((char *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(moby_data, initial_velocity);
        *(int *)(moby_data + 0x18) = scale_game_frames(lifetime_frames);
        *(int *)(moby_data + 0x10) = collision_ignore_moby;
        *(int *)(moby_data + 0x14) = tracked_moby;
        *(int *)(moby_data + 0x2C) = burst_mode;
        *(int *)(moby_data + 0x28) = 0;
        MOBY(spawned_moby)->rot.z = fast_arc_tan(*(float *)moby_data, *(float *)(moby_data + 4));
        MOBY(spawned_moby)->rot.y =
            -fast_arc_tan(vector_length_xy(moby_data), *(float *)(moby_data + 8));
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
/* Fire bolt: flies along its velocity with a smoke trail (while the level is not underwater), damaging what it hits. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00308758), where it is exact; names translated to the US level program. */

extern char  D_L08_00174640[];
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void fire_bolt_update(char *fire_bolt) __asm__("FUN_L08_00307298");

/* Moby class 1292 */
void fire_bolt_update(char *fire_bolt) {
    enum {
        FIRE_BOLT_STATE_FLYING = 1,
        FIRE_BOLT_STATE_BURST_AND_REMOVE = 2
    };
    char *bolt_data = *(char **)(fire_bolt + 0x78);
    float previous_position[4];
    float sc[4];
    float trail_spawn_position[4];
    float unused_vector_argument[4];
    char *tracked_moby;
    switch (((unsigned char *)fire_bolt)[0x20]) {
    case FIRE_BOLT_STATE_FLYING:
        qcopy(previous_position, fire_bolt + 0x10);
        tracked_moby = *(char **)(bolt_data + 0x14);
        if (tracked_moby == 0 || ((unsigned char *)tracked_moby)[0x20] == 0xFE ||
            ((unsigned char *)tracked_moby)[0x20] == 0xFD) {
            fire_bolt[0x20] = 2;
        }
        if (D_L08_0015F5D4 < 0.9f && D_L08_0015F5D0 < 0.9f) {
            int   trail_frame_count;
            float trail_velocity_scale = random_float_between(0.95f, 0.985f);
            fast_vec_scale(sc, bolt_data, trail_velocity_scale);
            fast_vec_normalize(trail_spawn_position, bolt_data, -0.1f);
            add_vectors(unused_vector_argument, trail_spawn_position, fire_bolt + 0x10);
            trail_frame_count = scale_game_frames(random_int_between(0xF, 0x14));
            spawn_effect(trail_spawn_position,
                         sc,
                         0x6F00AFFF,
                         0xFF,
                         trail_frame_count,
                         0x28,
                         scale_game_frames(random_int_between(7, 0xA)),
                         1);
        }
        fast_vec_add(fire_bolt + 0x10, fire_bolt + 0x10, bolt_data);
        if (collision_line(
                fire_bolt + 0x10, previous_position, 0x10, *(int *)(bolt_data + 0x10), 0) != 0) {
            char *collision_result = D_L08_00174640;
            if (*(int *)(collision_result + 0x18) != *(int *)(bolt_data + 0x10)) {
                if (*(int *)(collision_result + 0x18) != 0) {
                    spawn_nearby_effect(*(void **)(collision_result + 0x18),
                                        (int)fire_bolt,
                                        0x10001,
                                        collision_result + 0x20,
                                        bolt_data,
                                        1.0f);
                }
                fire_bolt[0x20] = 2;
            }
        }
        if (fast_dec_timer(bolt_data + 0x18) != 0)
            fire_bolt[0x20] = 2;
        break;
    case FIRE_BOLT_STATE_BURST_AND_REMOVE:
        if ((D_L08_0015F5D4 < 0.85f && D_L08_0015F5D0 < 0.85f) || *(int *)(bolt_data + 0x2C) == 1) {
            cart_burst(fire_bolt, fire_bolt + 0x10, -1, 0.25f, 13.0f);
        } else {
            cart_burst(fire_bolt, fire_bolt + 0x10, -1, 0.25f, 0.0f);
        }
        mark_moby_for_removal(fire_bolt);
        break;
    }
}
