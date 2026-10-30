/* Level 13 Qwark's starfighter part moby class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "qzero.h"

extern void spawn_explosion(void *,
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern int coll_sphere(void *, float, int, struct Moby *) __asm__("FUN_L00_001f0d60");

extern char coll_hit_1638[] __asm__("D_L13_00174140");

struct PieceVars {
    u8           pad0[0x10];
    Vec4f        vel;
    u128         unk20;
    f32          spin[3];
    struct Moby *parent;
    int          timer;
    int          point;
};

void update_qwarks_starfighter_part(struct Moby *moby) __asm__("FUN_L13_002ece00");

void update_qwarks_starfighter_part(struct Moby *moby) {
    enum {
        PART_STATE_INITIALIZE = 0,
        PART_STATE_SYNC_PARENT_SCALE = 1,
        PART_STATE_IDLE = 2,
        PART_STATE_MOVE_AND_SPIN = 3,
        PART_STATE_BURST_AND_REMOVE = 4
    };
    struct PieceVars *vars = (struct PieceVars *)moby->pvars;
    struct Moby      *parent = vars->parent;
    struct Moby      *hit;
    char             *coll;
    if (parent == 0 || parent->oclass != 0x184 || parent->state == 0xFE || parent->state == 0xFD) {
        goto remove;
    }
    moby->flags = (moby->flags & 0xFFFC);
    moby->flags = moby->flags | (vars->parent->flags & 3);
    switch (moby->state) {
    case PART_STATE_INITIALIZE:
        moby->state = 1;
        qzero(&vars->unk20);
        break;
    case PART_STATE_SYNC_PARENT_SCALE:
        moby->scale = moby->pclass->scale * vars->parent->scale / vars->parent->pclass->scale;
        moby->spawn_frame = vars->parent->spawn_frame;
        break;
    case PART_STATE_IDLE:
        break;
    case PART_STATE_MOVE_AND_SPIN:
        fast_vec_add(&moby->pos, &moby->pos, &vars->vel);
        fast_vec_scale(&vars->vel, &vars->vel, 0.995f);
        if (coll_sphere(&moby->pos, 2.0f, 0, moby) != 0) {
            coll = coll_hit_1638;
            hit = *(struct Moby **)(coll + 0x18);
            if (hit == 0 ||
                (hit->oclass != 0x191 && hit->oclass != 0x184 && hit->oclass != 0x185 &&
                 hit->oclass != 0x186 && hit->oclass != 0x187 && hit->oclass != 0x188 &&
                 hit->oclass != 0x189 && hit->oclass != 0x18A && hit->oclass != 0x18B &&
                 hit->oclass != 0x18C && hit->oclass != 0x18F && hit->oclass != 0x190)) {
                reflect_vector(&vars->vel, &vars->vel, coll + 0x40);
                moby->state = 4;
            }
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin[0]);
        moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin[1]);
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin[2]);
        vars->spin[0] *= 0.999f;
        vars->spin[1] *= 0.999f;
        vars->spin[2] *= 0.999f;
        if (moby->pos.x < 10.0f || 1013.0f < moby->pos.x || moby->pos.y < 10.0f ||
            1013.0f < moby->pos.y || moby->pos.z < 10.0f || 1013.0f < moby->pos.z) {
            goto remove;
        }
        if (fast_dec_timer(&vars->timer) == 0 || moby->visible != 0) {
            break;
        }
    remove:
        mark_moby_for_removal(moby);
        break;
    case PART_STATE_BURST_AND_REMOVE:
        spawn_explosion(moby,
                        &vars->vel,
                        0,
                        2.0f,
                        4.0f,
                        10,
                        3,
                        16,
                        4.0f,
                        2.0f,
                        9.0f,
                        -1,
                        1.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        break;
    }
}
