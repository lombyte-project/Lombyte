/* Level 11 moby class ammocrate: the ship ammo and health pickup and its child. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern float fast_add_rotations(float, float);

#include "qzero.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_00310B28), where it is exact; names translated to the US level program. */

extern char *FUN_L11_0030fd98_c(char *) __asm__("FUN_L11_0030fd98");
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

int sync_ship_pickup_child_pose(struct Moby *owner, char *data) __asm__("FUN_L11_0030f660");

int sync_ship_pickup_child_pose(struct Moby *owner, char *data) {
    char *child;
    if (!*(char **)(data + 0x34)) {
        *(char **)(data + 0x34) = FUN_L11_0030fd98_c(owner);
        if (!*(char **)(data + 0x34))
            goto missing;
    }
    child = *(char **)(data + 0x34);
    if (*(unsigned char *)(child + 0x20) == 0xFE)
        goto missing;
    if (*(unsigned char *)(child + 0x20) == 0xFD)
        goto missing;
    qcopy(child + 0x10, &owner->pos);
    qcopy(child + 0x40, &owner->rot);
    update_entity(child);
    moby_point_position(*(char **)(data + 0x34), 0, data);
    fast_vec_sub(data + 0x20, *(char **)(data + 0x34) + 0x10, data);
    return 1;
missing:
    qcopy(data, &owner->pos);
    qzero(data + 0x20);
    return 0;
}

extern void FUN_L00_0024f7c8_f728(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void FUN_L00_0025e450_f728(void *,
                                  void *,
                                  void *,
                                  float,
                                  float,
                                  int,
                                  int,
                                  int,
                                  float,
                                  float,
                                  float,
                                  float,
                                  int,
                                  float,
                                  int,
                                  int,
                                  int,
                                  int) __asm__("FUN_L00_0025e450");
extern void jet_animate(void *, float) __asm__("FUN_L00_00259fe8");

extern struct Moby *D_L11_00174858 NOT_SDA;
/* Ship ammo / health pickup: falls and bounces, is pulled toward the hero's ship, and refills it on contact. */
void ammocrate_update(struct Moby *m) __asm__("FUN_L11_0030f728");

void ammocrate_update(struct Moby *pickup) {
    enum {
        AMMOCRATE_STATE_INITIALIZE = 0,
        AMMOCRATE_STATE_DAMPED_FALL = 1,
        AMMOCRATE_STATE_ATTRACT_TO_SHIP = 2,
        AMMOCRATE_STATE_FREE_FALL = 3,
        AMMOCRATE_STATE_COLLECTED = 4,
        AMMOCRATE_STATE_REMOVE = 5
    };
    enum {
        OCLASS_SHIP_AMMO_PICKUP = 0x4C2,
        OCLASS_SHIP_HEALTH_PICKUP = 0x4C4
    };
    struct Moby *ship = hero.ship_moby;
    char        *pickup_vars = pickup->pvars;
    float        ship_pickup_distance_metric = 100000.0f;
    float        mid = ship_pickup_distance_metric;
    int          taken = 0;
    Vec4f        pickup_vector_scratch;

    FUN_L00_0024f7c8_f728(pickup, 0, &pickup_vector_scratch);
    if (coll_sphere(0.55f, &pickup_vector_scratch, 0, pickup)) {
        struct Moby *collision_moby = D_L11_00174858;
        if (collision_moby != 0 &&
            (collision_moby->oclass == 0x4DA || collision_moby->oclass == 0x45 ||
             collision_moby->oclass == 0x563)) {
            taken = 1;
        } else {
            FUN_L00_0025e450_f728(pickup,
                                  pickup_vars + 0x10,
                                  &pickup_vector_scratch,
                                  0.0f,
                                  0.0f,
                                  3,
                                  2,
                                  5,
                                  0.0f,
                                  0.0f,
                                  9.0f,
                                  1.0f,
                                  -1,
                                  0.0f,
                                  0,
                                  1,
                                  -1,
                                  0);
            pickup->state = 5;
        }
    }
    if (pickup->state < 4) {
        if (hero.state.current == 0x32 && ship != 0 && ship->state != 0xFE && ship->state != 0xFD &&
            (ship->oclass == 0x4DA || ship->oclass == 0x45 || ship->oclass == 0x563)) {
            ship_pickup_distance_metric = fast_vec_dist(&ship->pos, &pickup->pos);
            mid = vector_distance_xy(&ship->pos, &pickup->pos);
            fast_vec_add(&pickup_vector_scratch, &pickup->pos, pickup_vars);
            fast_vec_scale(&pickup_vector_scratch, &pickup_vector_scratch, 0.5f);
            mid = vector_distance_xy(&ship->pos, &pickup_vector_scratch);
        }
        if (taken || ship_pickup_distance_metric < 4.0f || mid < 4.0f) {
            pickup->collision = 0;
            if (*(struct Moby **)(pickup_vars + 0x34) != 0) {
                (*(struct Moby **)(pickup_vars + 0x34))->collision = 0;
            }
            switch (pickup->oclass) {
            case OCLASS_SHIP_AMMO_PICKUP:
                hero.ship_ammo += 5;
                if (hero.ship_moby != 0 && hero.ship_moby->state != 0xFE &&
                    hero.ship_moby->state != 0xFD) {
                    allocate_voice_for_target_entry(4, 0, hero.ship_moby);
                }
                if (hero.ship_ammo_max < hero.ship_ammo) {
                    hero.ship_ammo = hero.ship_ammo_max;
                }
                pickup->state = 4;
                break;
            case OCLASS_SHIP_HEALTH_PICKUP:
                hero.ship_hp += hero.ship_hp_max * 0.25f;
                if (hero.ship_moby != 0 && hero.ship_moby->state != 0xFE &&
                    hero.ship_moby->state != 0xFD) {
                    allocate_voice_for_target_entry(5, 0, hero.ship_moby);
                }
                if (hero.ship_hp_max < hero.ship_hp) {
                    hero.ship_hp = hero.ship_hp_max;
                }
                pickup->state = 4;
                break;
            default:
                pickup->state = 4;
                break;
            }
        }
    }
    switch (pickup->state) {
    case AMMOCRATE_STATE_INITIALIZE:
        *(struct Moby **)(pickup_vars + 0x34) = 0;
        sync_ship_pickup_child_pose(pickup, pickup_vars);
        *(float *)(pickup_vars + 0x30) = 0.0f;
        pickup->state = 1;
        break;
    case AMMOCRATE_STATE_DAMPED_FALL:
        fast_vec_scale(pickup_vars + 0x10, pickup_vars + 0x10, 0.9f);
        *(float *)(pickup_vars + 0x18) -= frame_time_sq * 9.8f;
        fast_vec_add(pickup_vars, pickup_vars, pickup_vars + 0x10);
        fast_vec_add(&pickup->pos, pickup_vars, pickup_vars + 0x20);
        if (sync_ship_pickup_child_pose(pickup, pickup_vars)) {
            if ((*(struct Moby **)(pickup_vars + 0x34))->seq == 1) {
                fast_vec_scale(pickup_vars + 0x10, pickup_vars + 0x10, 0.8f);
                pickup->state = 2;
            }
        }
        break;
    case AMMOCRATE_STATE_ATTRACT_TO_SHIP: {
        float k = 0.8f;
        pickup->rot.z = fast_add_rotations(pickup->rot.z, frame_time * 0.034906585f);
        *(float *)(pickup_vars + 0x30) =
            fast_add_rotations(*(float *)(pickup_vars + 0x30), frame_time * 1.5707964f);
        pickup->rot.y = fast_cos(*(float *)(pickup_vars + 0x30)) * 0.7853982f;
        fast_vec_scale(pickup_vars + 0x10, pickup_vars + 0x10, k);
        *(float *)(pickup_vars + 0x18) -= frame_time_sq * 9.8f;
        if (ship_pickup_distance_metric < 40.0f && pickup->visible != 0) {
            k = k / square_root(ship_pickup_distance_metric);
            fast_vec_sub(&pickup_vector_scratch, &ship->pos, &pickup->pos);
            fast_vec_normalize(&pickup_vector_scratch, &pickup_vector_scratch, k);
            fast_vec_add(pickup_vars + 0x10, pickup_vars + 0x10, &pickup_vector_scratch);
        }
        fast_vec_add(pickup_vars, pickup_vars, pickup_vars + 0x10);
        fast_vec_add(&pickup->pos, pickup_vars, pickup_vars + 0x20);
        if (sync_ship_pickup_child_pose(pickup, pickup_vars)) {
            if ((*(struct Moby **)(pickup_vars + 0x34))->state == 2) {
                pickup->state = (*(struct Moby **)(pickup_vars + 0x34))->state;
            }
        }
        break;
    }
    case AMMOCRATE_STATE_FREE_FALL:
        *(float *)(pickup_vars + 0x18) -= frame_time_sq * 9.8f;
        fast_vec_add(&pickup->pos, &pickup->pos, pickup_vars + 0x10);
        break;
    case AMMOCRATE_STATE_COLLECTED:
        pickup->state = 5;
        break;
    case AMMOCRATE_STATE_REMOVE:
        mark_moby_for_removal(pickup);
        return;
    }
    if (pickup->pos.z < 20.0f) {
        mark_moby_for_removal(pickup);
        return;
    }
    if (pickup->pos.x < 20.0f) {
        pickup->pos.x = 20.0f;
    }
    if (1003.0f < pickup->pos.x) {
        pickup->pos.x = 1003.0f;
    }
    if (pickup->pos.y < 20.0f) {
        pickup->pos.y = 20.0f;
    }
    if (1003.0f < pickup->pos.y) {
        pickup->pos.y = 1003.0f;
    }
    if (pickup->pos.z < 20.0f) {
        pickup->pos.z = 20.0f;
    }
    if (1003.0f < pickup->pos.z) {
        pickup->pos.z = 1003.0f;
    }
    jet_animate(pickup, 0.5f);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_00311260), where it is exact; names translated to the US level program. */

char *spawn_ship_pickup_child(struct Moby *owner) __asm__("FUN_L11_0030fd98");

char *spawn_ship_pickup_child(struct Moby *owner) {
    char *moby = create_moby(0x4C3);
    if (moby != 0) {
        char *data;
        data = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        qcopy(moby + 0x10, &owner->pos);
        qcopy(moby + 0x40, &owner->rot);
        *(char **)(data + 0x70) = owner;
        if ((unsigned char)moby[0x53] != 2) {
            blend_moby_animation(moby, 2, 0xD, 0);
        }
        update_entity(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}
