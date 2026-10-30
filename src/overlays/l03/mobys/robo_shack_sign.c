/* Level 03 moby class robo_shack_sign. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"

extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

/* Spinner: turns about z at a fixed rate (27.5 degrees times D_0015ED6C) and carries its collision record along. */
struct SpinnerVars {
    u8 pad0[0x20];
    u8 coll[0x10]; /* 0x20: collision record moved with the rotation */
};

void robo_shack_sign_update(struct Moby *m) __asm__("FUN_L03_002d3e58");

void robo_shack_sign_update(struct Moby *sign_moby) {
    Vec4                move;
    Vec4                oldRot;
    struct SpinnerVars *sign_vars;
    move.q = 0;
    oldRot.q = *(u128 *)&sign_moby->rot;
    sign_vars = (struct SpinnerVars *)sign_moby->pvars;
    sign_moby->draw_dist = 0xFF;
    sign_moby->update_dist = 0xFF;
    sign_moby->rot.z = fast_add_rotations(sign_moby->rot.z, frame_time * 0.47996554f);
    update_entity_matrix(sign_vars->coll, &move, &oldRot, &sign_moby->rot);
}
