/* Level 04 moby class woodcut_bot_debris (0x5ec): the chunks the woodcut bot throws, which fall and bounce, then fade out. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "qcopy.h"

typedef struct {
    u8  pad0[4];
    f32 f4;
    f32 f8;
    s16 hC;
    s16 hE;
    f32 vel[4] __attribute__((aligned(16)));
    f32 f20, f24, f28;
} PV2e5178;

typedef struct {
    u8        pad0[0x10];
    f32       pos[4] __attribute__((aligned(16)));
    u8        b20, b21, b22, b23;
    u8        pad24[8];
    f32       f2c;
    u8        pad30[0x10];
    f32       f40, f44, f48;
    u8        pad4c[0x2C];
    PV2e5178 *pv;
} M2e5178;

f32         woodcut_debris_bounce_spin_min_degrees_gp __asm__("D_L04_00161D78") = 0;
f32         woodcut_debris_bounce_spin_max_degrees_gp __asm__("D_L04_00161D7C") = 3.6e+02f;
extern u128 coll_hit_normal_lui[] __asm__("D_L04_00174000") NOT_SDA;
extern s32  coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
void        woodcut_bot_debris_update(M2e5178 *entity) __asm__("FUN_L04_002e5178");

/* Moby class 1516 */
void woodcut_bot_debris_update(M2e5178 *entity) {
    enum {
        ENTITY_MOTION_STATE_BOUNCE = 0,
        ENTITY_MOTION_STATE_DECAY_AND_REMOVE = 1
    };
    PV2e5178 *pv = entity->pv;
    u128      previous_position;
    u128     *collision_normal;
    qcopy(&previous_position, entity->pos);
    pv->vel[2] -= frame_time_sq * 20.0f;
    fast_vec_add(entity->pos, entity->pos, pv->vel);
    entity->f40 = fast_add_rotations(entity->f40, pv->f20);
    entity->f44 = fast_add_rotations(entity->f44, pv->f24);
    entity->f48 = fast_add_rotations(entity->f48, pv->f28);
    switch (entity->b20) {
    case ENTITY_MOTION_STATE_BOUNCE:
        if (entity->pos[2] < pv->f4 - 3.0f) {
            entity->b20 = 1;
        } else if (fast_dec_timer_s16(&pv->hE) && pv->hC != 0 &&
                   coll_sphere(entity->pos, 2, 0, pv->f8)) {
            collision_normal = coll_hit_normal_lui;
            if (fast_vec_dot(pv->vel, collision_normal) < 0.0f) {
                qcopy(entity->pos, collision_normal - 1);
                pv->hC--;
                reflect_vector(pv->vel, pv->vel, collision_normal);
                fast_vec_scale(pv->vel, pv->vel, 0.75f);
                pv->f20 = 0.0f;
                pv->f24 = randf_sym(woodcut_debris_bounce_spin_min_degrees_gp,
                                    woodcut_debris_bounce_spin_max_degrees_gp) *
                          DEG_TO_RAD * frame_time;
                pv->f28 = randf_sym(woodcut_debris_bounce_spin_min_degrees_gp,
                                    woodcut_debris_bounce_spin_max_degrees_gp) *
                          DEG_TO_RAD * frame_time;
            }
        }
        break;
    case ENTITY_MOTION_STATE_DECAY_AND_REMOVE:
        entity->f2c *= 0.9f;
        if (entity->b23 < 4) {
            mark_moby_for_removal(entity);
        } else {
            entity->b23 -= 4;
        }
        break;
    }
}
