/* Level 10 moby class bombthrower_grass: fires projectiles on a timer. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

/* update: on a timer, fire a projectile from the moby's angle, then rearm the timer */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002D9FC0), where it is exact; names translated to the US level program. */

extern int spawn_bombthrower_grass_projectile() __asm__("FUN_L10_002d8058");

int D_L10_00161BF0 = 240;
int D_L10_00161BF4 = 600;

#include "rnc/overlay/l10/mobys/bombthrower_grass_globals.h"

extern float bombthrower_grass_yaw_offset_limit_degrees_gp __asm__("D_L10_00161BF8")
    __attribute__((sda));
extern float bombthrower_grass_launch_elevation_min_degrees_gp __asm__("D_L10_00161BFC")
    __attribute__((sda));
extern float bombthrower_grass_launch_elevation_max_degrees_gp __asm__("D_L10_00161C00")
    __attribute__((sda));
extern float bombthrower_grass_launch_speed_min_gp __asm__("D_L10_00161C04") __attribute__((sda));
extern float bombthrower_grass_launch_speed_max_gp __asm__("D_L10_00161C08") __attribute__((sda));
void         bombthrower_grass_update(struct Moby *moby) __asm__("FUN_L10_002d8c00");

#include "qcopy.h"

extern void update_entity(void *) __asm__("FUN_L00_00250df8");

struct Swirl {
    char  pad[0x18];
    float a;
    float b;
};

extern f32 D_L10_00161BEC_gp __asm__("D_L10_00161BEC") __attribute__((sda));
int        spawn_bombthrower_grass_projectile(int   owner,
                                              char *initial_position,
                                              char *initial_velocity) __asm__("FUN_L10_002d8058");

int spawn_bombthrower_grass_projectile(int   firing_moby,
                                       char *initial_position,

                                       char *initial_velocity) {
    char *moby = create_moby(0x3AA);
    if (moby != 0) {
        char *bombthrower_projectile_vars = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0x7F;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(int *)(bombthrower_projectile_vars + 0x10) = firing_moby;
        *(short *)(bombthrower_projectile_vars + 0x16) = 0;
        qcopy(moby + 0x10, initial_position);
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + *(float *)&D_L10_00161BEC_gp;
        qcopy(bombthrower_projectile_vars, initial_velocity);
        ((struct Swirl *)bombthrower_projectile_vars)->a =
            random_float_between(frame_time * 1.0471976f, frame_time * 3.1415927f);
        ((struct Swirl *)bombthrower_projectile_vars)->b =
            random_float_between(frame_time * 1.0471976f, frame_time * 3.1415927f);
        *(short *)(bombthrower_projectile_vars + 0x14) = scale_game_frames(0x3C);
        update_entity(moby);
    }
    return moby;
}

/* Moby class 939 */
void bombthrower_grass_update(struct Moby *moby) {
    char *launch_timer = (char *)moby->pvars;
    float launch_offset[4];
    float launch_position[4];
    if (moby->state == 0) {
        *(int *)launch_timer =
            scale_game_frames(random_int_between(D_L10_00161BF0, D_L10_00161BF4));
        moby->state = 1;
    }
    if (fast_dec_timer((int *)launch_timer)) {
        float x = bombthrower_grass_yaw_offset_limit_degrees_gp;
        float yaw_offset = random_float_between(-x, x) * DEG_TO_RAD;
        float launch_yaw = fast_add_rotations(moby->rot.z, yaw_offset);
        float r = random_float_between(bombthrower_grass_launch_elevation_min_degrees_gp,
                                       bombthrower_grass_launch_elevation_max_degrees_gp) *
                  DEG_TO_RAD;
        build_spherical_offset(launch_offset,
                               random_float_between(bombthrower_grass_launch_speed_min_gp,
                                                    bombthrower_grass_launch_speed_max_gp) *
                                   frame_time,
                               launch_yaw,
                               r);
        fast_vec_add(launch_position, &moby->pos, launch_offset);
        spawn_bombthrower_grass_projectile(moby, launch_position, launch_offset);
        allocate_voice_for_target_entry(0, 0, (int)moby);
        *(int *)launch_timer =
            scale_game_frames(random_int_between(D_L10_00161BF0, D_L10_00161BF4));
    }
}
