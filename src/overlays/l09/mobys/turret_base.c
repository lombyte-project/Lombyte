/* Level 09 moby class 0x144: turret base, spawned under each turret at its set-up (its update is the no-op callback). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"
#include "sda.h"

typedef unsigned int u128_ee380 __attribute__((mode(TI)));
char *
spawn_turret_base(struct Moby *owner, void *position, float angle) __asm__("FUN_L09_002efda8");

char *spawn_turret_base(struct Moby *source_moby, void *position, float angle) {
    u128_ee380 position_copy;
    void      *copy = &position_copy;
    char      *moby;

    position_copy = *(u128_ee380 *)position;
    moby = create_moby(0x144);
    if (moby != 0) {
        unsigned char state;

        ((unsigned char *)moby)[0x30] = source_moby->update_dist;
        state = source_moby->update_dist;
        moby[0x31] = 1;
        MOBY(moby)->draw_dist = state;
        qcopy(moby + 0x10, copy);
        qcopy(moby + 0x40, &source_moby->rot);
        MOBY(moby)->spawn_frame = source_moby->spawn_frame;
        update_entity(moby);
    }
    return moby;
}
