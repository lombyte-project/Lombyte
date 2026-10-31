/* Shared moby class garage_door: rises once its moby is collected. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/overlay/level_save_state.h"

typedef struct Moby Moby;

extern s32 current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));

/*
 * Rises once its moby (unkB2) is collected: waits in state 0 (keeping the
 * target point at its position), then climbs D_0015ED6C per frame until 4
 * above the target, and stays there.
 */

void garage_door_update(struct Moby *m) __asm__("FUN_L03_002dca30");

/* Moby class 997 */
void garage_door_update(struct Moby *door_moby) {
    enum {
        GARAGE_DOOR_WAITING_FOR_COLLECTION = 0,
        GARAGE_DOOR_RISING = 1,
        GARAGE_DOOR_RAISED = 2
    };
    Vec4 *target = (Vec4 *)door_moby->pvars;
    s16   save_id_index;
    u16   id;

    switch (door_moby->state) {
    case GARAGE_DOOR_WAITING_FOR_COLLECTION:
        /* one quadword copy (lq/sq); qcopy() schedules differently */
        target->q = ((Vec4 *)&door_moby->pos)->q;
        id = door_moby->save_id;
        save_id_index = id;
        if (l03_save_state.cleared[save_id_index] == 0) {
            if (!((D_0014C190_257218[current_level_index_gp][save_id_index >> 5] >> (id & 0x1F)) &
                  1))
                return;
        }
        door_moby->state = 1;
        return;
    case GARAGE_DOOR_RISING:
        door_moby->pos.z += frame_time;
        if (target->f[2] + 4.0f <= door_moby->pos.z)
            door_moby->state = 2;
        return;
    case GARAGE_DOOR_RAISED:
        door_moby->pos.z = target->f[2] + 4.0f;
        return;
    }
}
