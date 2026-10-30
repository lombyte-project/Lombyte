/* Level 04 moby class upper_door and its rise speed helper. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"

/* Rise speed of a riser by its class. */
float riser_rise_speed(struct Moby *moby) __asm__("FUN_L04_002e1768");

float riser_rise_speed(struct Moby *moby) {
    enum {
        RISER_CLASS_ASCENDING_WITH_COMPLETION_VOICE = 0x44D,
        RISER_CLASS_FAST_DESCENDING = 0x5FB,
        RISER_CLASS_ASCENDING_WITHOUT_COMPLETION_VOICE = 0x5FC
    };
    switch (moby->oclass) {
    case RISER_CLASS_ASCENDING_WITH_COMPLETION_VOICE:
    case RISER_CLASS_ASCENDING_WITHOUT_COMPLETION_VOICE:
        return 1.3f;
    case RISER_CLASS_FAST_DESCENDING:
        return -2.55f;
    }
    return -2.05f;
}

extern int D_L04_0015FFD8; /* no foreign declaration */

/* Riser: once its id is collected or its trigger moby reaches the open state, rises for a
 * second, then records its id in the level bitsets. */
typedef struct {
    u8 pad[0x454];
    u8 collected[1];
} LevelCollected;

struct RiserVars {
    s32 trigger; /* moby index whose state opens it, -1 for none */
    f32 baseZ;
    s32 timer;
};

extern s32            level_completed_save_id_bits[] __asm__("D_L04_001BA650");
extern LevelCollected l04_save_id_collection_state __asm__("D_L04_001BB3B0");
extern int            trooper_play_sound(int, int, struct Moby *, int) __asm__("FUN_L00_0028dc90");

void upper_door_update(struct Moby *m) __asm__("FUN_L04_002e17d8");

void upper_door_update(struct Moby *moby) {
    enum {
        UPPER_DOOR_INITIALIZE = 0,
        UPPER_DOOR_WAIT_FOR_ACTIVATION = 1,
        UPPER_DOOR_MOVE_VERTICAL = 2
    };
    struct RiserVars *riser_vars = (struct RiserVars *)moby->pvars;
    struct Moby      *trigger_moby;
    s16               save_id_index;
    u16               id;

    switch (moby->state) {
    case UPPER_DOOR_INITIALIZE:
        riser_vars->baseZ = moby->pos.z;
        moby->state = 1;
        break;
    case UPPER_DOOR_WAIT_FOR_ACTIVATION:
        id = moby->save_id;
        save_id_index = id;
        if (l04_save_id_collection_state.collected[save_id_index] == 0 &&
            !((D_0014C190_257218[current_level_index][save_id_index >> 5] >> (id & 0x1F)) & 1)) {
            if (riser_vars->trigger == -1)
                break;
            trigger_moby = (struct Moby *)((riser_vars->trigger << 8) + D_L04_0015FFD8);
            if (!((trigger_moby->oclass == 0x267 && trigger_moby->state == 4) ||
                  (trigger_moby->oclass == 0x4A6 && trigger_moby->state == 2)))
                return;
        }
        if (moby->oclass == 0x44D || moby->oclass == 0x5FC)
            trooper_play_sound(0, 0, moby, 0x44D);
        moby->state = 2;
        riser_vars->timer = scale_game_frames(60);
        break;
    case UPPER_DOOR_MOVE_VERTICAL:
        moby->pos.z += riser_rise_speed(moby) * frame_time;
        if (fast_dec_timer(&riser_vars->timer)) {
            D_0014C190_257218[current_level_index][(s16)moby->save_id >> 5] |=
                1 << (moby->save_id & 0x1F);
            level_completed_save_id_bits[(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
            if (moby->oclass == 0x44D)
                allocate_voice_for_target_entry(1, 0, moby);
            moby->state = 3;
        }
        break;
    }
}
