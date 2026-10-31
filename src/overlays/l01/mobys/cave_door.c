/* Level 01 moby class cave_door. */
#include "types.h"
#include "rnc/math_consts.h"

#include "rnc/math/asm_helpers.h"

#include "rnc/overlay/cuboid.h"

#include "rnc/mobys/moby.h"

typedef struct {
    s32 target;
    f32 angle;
    f32 speed;
} RotVars;

extern struct Moby *D_L01_0015FFD8;

void cave_door_update(struct Moby *self) __asm__("FUN_L01_002f4348");

/* Moby class 641 */
void cave_door_update(struct Moby *cave_door_moby) {
    enum {
        CAVE_DOOR_STATE_INITIALIZE = 0,
        CAVE_DOOR_STATE_UPDATE_ROTATION = 1
    };
    RotVars     *cave_door_vars;
    struct Moby *target_moby;
    f32         *mv;

    cave_door_vars = (RotVars *)cave_door_moby->pvars;
    switch (cave_door_moby->state) {
    case CAVE_DOOR_STATE_INITIALIZE:
        cave_door_vars->angle = cave_door_moby->rot.x;
        cave_door_vars->speed = cave_door_vars->speed * DEG_TO_RAD;
        cave_door_moby->state = 1;
        break;
    case CAVE_DOOR_STATE_UPDATE_ROTATION:
        if (cave_door_vars->target != -1) {
            target_moby = &D_L01_0015FFD8[cave_door_vars->target];
            if (target_moby->oclass == 0x118) {
                mv = (f32 *)target_moby->pvars;
                target_moby->update_dist = 0xFF;
                cave_door_moby->rot.x = fast_add_rotations(
                    cave_door_vars->angle,
                    fast_normalize_angle(
                        fast_add_rotations(cave_door_vars->speed, -cave_door_vars->angle) * mv[0]));
            }
        }
        break;
    }
}

#include "qcopy.h"
