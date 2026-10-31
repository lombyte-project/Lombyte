/* Level 11 part of the ship rocket class (0x40A, updated by ship_rocket_update in shared/vehicles/ship_rocket.c): the spawner the jet fighter fires it with. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

#include "qcopy.h"

/* Same source as the exact FUN_L13_002e6a58, spawning class 0x40A, without the target class store and the closing effect. */
unsigned char *spawn_ship_rocket(struct Moby *spawning_moby,
                                 char        *spawn_position,
                                 char        *target,
                                 char        *spawn_rotation,
                                 int          arg,
                                 float        scale) __asm__("FUN_L11_00309378");

unsigned char *spawn_ship_rocket(struct Moby *spawning_moby,
                                 char        *spawn_position,
                                 char        *target,
                                 char        *spawn_rotation,
                                 int          duration_frames,

                                 float scale) {
    unsigned char *moby = create_moby(0x40A);
    if (moby != 0) {
        char *projectile_vars = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(char **)(projectile_vars + 0x20) = spawning_moby;
        *(char **)(projectile_vars + 0x24) = target;
        qcopy(moby + 0x10, spawn_position);
        qcopy(moby + 0x40, spawn_rotation);
        if (target != 0) {
            char *target_aux_data;
            target_aux_data = get_moby_target_info(target);
            if (target_aux_data != 0) {
                *(unsigned short *)(target_aux_data + 0x1E) |= 0x80;
            }
        }
        *(int *)(projectile_vars + 0x28) = duration_frames;
        *(float *)(projectile_vars + 0x2C) = scale;
        *(int *)(projectile_vars + 0x30) = 0;
        *(int *)(projectile_vars + 0x34) = 0;
        build_spherical_offset(projectile_vars,
                               scale,
                               *(float *)(spawn_rotation + 8),
                               -*(float *)(spawn_rotation + 4));
        if (moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        fast_vec_add(moby + 0x10, moby + 0x10, projectile_vars);
        if (*(char **)(projectile_vars + 0x24) != 0) {
            qcopy(projectile_vars + 0x10, *(char **)(projectile_vars + 0x24) + 0x10);
        }
        MOBY(moby)->scale =
            *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
            (spawning_moby->scale / *(float *)(((char *)spawning_moby->pclass) + 0x24));
        update_entity(moby);
    }
    return moby;
}
