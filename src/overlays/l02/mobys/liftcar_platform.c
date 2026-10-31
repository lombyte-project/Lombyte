/* Level 02 moby class liftcar_platform. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"

#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/unlock_planet.h"

#include "qcopy.h"

extern struct Moby  *D_L02_0015FFD8;
extern unsigned char D_0014C050[][16];

/* Lift platform carried by an animated driver moby: follows the driver's joint 0, waits at an end (3/5) for the
 * hero to step on, then plays the driver's up or down animation (2/4) with its loop sound until it ends. */
void liftcar_platform_update(struct Moby *moby) __asm__("FUN_L02_002e2228");

/* Moby class 792 */
void liftcar_platform_update(struct Moby *moby) {
    enum {
        LIFTCAR_PLATFORM_INITIALIZE = 0,
        LIFTCAR_PLATFORM_WAIT_FOR_MISSION_GATE = 1,
        LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_ABOVE = 2,
        LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_ABOVE = 3,
        LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_BELOW = 4,
        LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_BELOW = 5,
        LIFTCAR_PLATFORM_FOLLOW_DRIVER_ONLY = 6
    };
    char        *platform_vars = (char *)moby->pvars;
    struct Moby *driver = &D_L02_0015FFD8[*(int *)(platform_vars + 0xA0)];
    Vec4         delta;
    Vec4         old_rot;
    float        joint_mtx[16];
    Vec4         pos;
    int          ride;
    int          voice;

    fast_vec_scale(&delta, &moby->pos, -1.0f);
    qcopy(&old_rot, &moby->rot);
    moby_point_position(driver, 0, &pos);
    qcopy(&moby->pos, &pos);
    switch (moby->state) {
    case LIFTCAR_PLATFORM_INITIALIZE:
        build_moby_bone_transform(driver, 0, joint_mtx);
        moby->rot.z = fast_arc_tan(joint_mtx[0], joint_mtx[1]);
        moby->state = 1;
        blend_moby_animation(driver, 3, 0, 0);
        break;
    case LIFTCAR_PLATFORM_WAIT_FOR_MISSION_GATE:
        if (D_0014C050[current_level_index][moby->mission] == 0xFF) {
            moby->state = 5;
        }
        break;
    case LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_ABOVE:
    case LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_BELOW:
        ride = 0;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 && moby->command == 0) {
            ride = 1;
        } else if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 16.0f) {
            if (moby->state == 5 && hero.motion.pos.f[2] < moby->pos.z - 5.0f) {
                ride = 1;
            } else if (moby->state == 3 && moby->pos.z + 5.0f < hero.motion.pos.f[2]) {
                ride = 1;
            }
        }
        if (ride) {
            if (moby->state == 5 || moby->state == 1) {
                moby->state = 2;
                set_moby_animation(driver, 0, 0);
                *(int *)(platform_vars + 0xA4) = allocate_voice_for_target_entry(2, 4, driver);
            } else {
                moby->state = 4;
                set_moby_animation(driver, 2, 0);
                *(int *)(platform_vars + 0xA4) = allocate_voice_for_target_entry(0, 4, driver);
            }
        } else if (hero.ground_moby != moby) {
            moby->command = 0;
        }
        break;
    case LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_ABOVE:
    case LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_BELOW:
        build_moby_bone_transform(driver, 0, joint_mtx);
        moby->rot.z = fast_arc_tan(joint_mtx[0], joint_mtx[1]);
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 1.5f &&
            hero.motion.pos.f[2] < moby->pos.z - 1.5f) {
            driver->anim_speed = 0.0f;
            voice = *(int *)(platform_vars + 0xA4);
            if (voice != -1) {
                char *e = ((char *)&voice_pool) + voice * 0x70;
                if (*(struct Moby **)(e + 0x88) == driver && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(voice);
                }
            }
            *(int *)(platform_vars + 0xA4) = -1;
        }
        if (driver->anim_speed == 0.0f &&
            (vector_distance_xy(&moby->pos, &hero.motion.pos) > 2.0f ||
             moby->pos.z - 1.25f < hero.motion.pos.f[2])) {
            driver->anim_speed = 1.0f;
            if (moby->state == 2) {
                *(int *)(platform_vars + 0xA4) = allocate_voice_for_target_entry(2, 4, driver);
            } else {
                *(int *)(platform_vars + 0xA4) = allocate_voice_for_target_entry(0, 4, driver);
            }
        }
        if (driver->anim_flags & 2) {
            voice = *(int *)(platform_vars + 0xA4);
            if (voice != -1) {
                char *e = ((char *)&voice_pool) + voice * 0x70;
                if (*(struct Moby **)(e + 0x88) == driver && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(voice);
                }
            }
            *(int *)(platform_vars + 0xA4) = -1;
            if (moby->state == 2) {
                moby->state = 3;
                blend_moby_animation(driver, 1, 0, 0);
                allocate_voice_for_target_entry(3, 0, driver);
            } else {
                moby->state = 5;
                blend_moby_animation(driver, 3, 0, 0);
                allocate_voice_for_target_entry(1, 0, driver);
            }
            moby->command = 1;
        }
        break;
    case LIFTCAR_PLATFORM_FOLLOW_DRIVER_ONLY:
        break;
    }
    fast_vec_add(&delta, &delta, &moby->pos);
    update_entity_matrix(platform_vars + 0x60, &delta, &old_rot, &moby->rot);
}
