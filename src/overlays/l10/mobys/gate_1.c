/* Level 10 moby class gate_1: the first of the paired gates. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/game/item_state.h"
#include "asm.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/l10/mobys/gate_state.h"

typedef struct Moby Moby;

extern float FUN_L04_001f1978(float);
extern int   D_L10_001BA950[] __asm__("D_L10_001BA950");

typedef struct {
    int   unk0;
    float home_yaw;
    int   timer;
    int   clip_volume;
    int   sound;
} DoorVars;

extern unsigned char D_001413F4_e9648[] __asm__("D_001413F4");
extern unsigned char D_0013DD4C_e9648[] __asm__("D_0013DD4C");
extern void          activate_descriptor(int) __asm__("FUN_L10_002f5a50");

/* Level 10 door: swings open once the hero enters its clip volume and sets its save bit; starts open when the bit is already set. */
void gate_1_update(struct Moby *moby) __asm__("FUN_L10_002e9648");
void activate_descriptor(int i) __asm__("FUN_L10_002f5a50");

/* Moby class 1421 */
void gate_1_update(struct Moby *moby) {
    DoorVars      *gate_control = (DoorVars *)moby->pvars;
    unsigned short save_id;

    switch (moby->state) {
    case GATE_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        if (D_001413F4_e9648[0] == 0) {
            save_id = moby->save_id;
            if (l10_save_state.cleared[(short)save_id] == 0 &&
                ((D_0014C190_257218[current_level_index][(short)save_id >> 5] >> (save_id & 31)) &
                 1) == 0) {
                gate_control->home_yaw = moby->rot.y;
                moby->state = 1;
                return;
            }
        }
        moby->rot.y = fast_add_rotations(moby->rot.y, -1.3962634f);
        moby->state = 3;
        return;
    case GATE_STATE_CLOSED:
        if (hero.vehicle_mode != 0)
            goto open;
        save_id = moby->save_id;
        if (l10_save_state.cleared[(short)save_id] != 0 ||
            ((D_0014C190_257218[current_level_index][(short)save_id >> 5] >> (save_id & 31)) & 1) !=
                0) {
        open:
            moby->rot.y = fast_add_rotations(moby->rot.y, -1.3962634f);
            moby->state = 3;
            return;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, gate_control->clip_volume) == 0 &&
            D_0013DD4C_e9648[0] == 0)
            return;
        if (gate_control->unk0 <= 0 || gate_control->sound <= 0)
            return;
        save_id = moby->save_id;
        D_0014C190_257218[current_level_index][(short)save_id >> 5] |= 1 << (save_id & 31);
        save_id = moby->save_id;
        D_L10_001BA950[(short)save_id >> 5] |= 1 << (save_id & 31);
        if (is_point_inside_clip_volume(&hero.motion.pos, gate_control->clip_volume))
            activate_descriptor(gate_control->sound);
        gate_control->timer = truncate_float_to_s32(FUN_L04_001f1978(90.0f));
        moby->state = 2;
        return;
    case GATE_STATE_OPENING:
        moby->rot.y = fast_add_rotations(moby->rot.y, -1.3962634f / FUN_L04_001f1978(90.0f));
        if (fast_dec_timer(&gate_control->timer))
            moby->state = 3;
        return;
    case GATE_STATE_OPEN:
        if (D_001413F4_e9648[0] != 0)
            return;
        save_id = moby->save_id;
        if (l10_save_state.cleared[(short)save_id] != 0)
            return;
        if ((((D_0014C190_257218[current_level_index][(short)save_id >> 5] >> (save_id & 31)) ^ 1) &
             1) == 0)
            return;
        moby->rot.y = fast_add_rotations(moby->rot.y, 1.3962634f);
        moby->state = 0;
        return;
    }
}
