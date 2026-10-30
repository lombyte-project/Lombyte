/* Level 10 moby class gate. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/quad.h"
#include "rnc/globals.h"
#include "rnc/game/item_state.h"

/* Collectible drop: waits until it is taken, then sinks below its rest height and records the pickup. */
typedef struct {
    char          pad0[0x454];
    unsigned char collected[1];
} BfState;

extern BfState       l10_gate_completion_records __asm__("D_L10_001BAA50");
extern unsigned char l10_gate_persistence_eligibility_by_mission[8] __asm__("D_L10_0015FC88");
void                 gate_update(unsigned char *m) __asm__("FUN_L10_002befe8");

void gate_update(unsigned char *gate_moby) {
    enum {
        GATE_STATE_WAIT_FOR_SAVE_FLAG = 0,
        GATE_STATE_MOVING_TO_LOWER_Y = 1,
        GATE_STATE_AT_LOWER_Y = 2
    };
    float         *saved_position = ((float *)MOBY(gate_moby)->pvars);
    unsigned short id;
    switch (gate_moby[0x20]) {
    case GATE_STATE_WAIT_FOR_SAVE_FLAG:
        *(OvlQuad *)saved_position = *(OvlQuad *)&MOBY(gate_moby)->pos;
        id = MOBY(gate_moby)->save_id;
        if (l10_save_state.cleared[(short)id] != 0 ||
            (*(int *)((unsigned char *)D_0014C190_257218 +
                      (((short)id >> 5) * 4 + (current_level_index << 8))) >>
             (id & 0x1F)) &
                1) {
            gate_moby[0x20] = 1;
        }
        break;
    case GATE_STATE_MOVING_TO_LOWER_Y:
        MOBY(gate_moby)->pos.y += frame_time * -1.0f;
        if (MOBY(gate_moby)->pos.y <= saved_position[1] + -3.75f) {
            l10_gate_completion_records.collected[((short)MOBY(gate_moby)->save_id)] =
                gate_moby[0xB0] + 2;
            if (gate_moby[0xB0] == 0xFF ||
                (l10_gate_persistence_eligibility_by_mission[gate_moby[0xB0]] != 0xFF &&
                 visited52[gate_moby[0xB0] + (current_level_index << 4)] == 0xFF)) {
                l10_save_state.cleared[((short)MOBY(gate_moby)->save_id)] = gate_moby[0xB0] + 2;
            }
            gate_moby[0x20] = 2;
            if (current_level_index == 10) {
                menu_item_enabled_68[0] = 1;
            }
        }
        break;
    case GATE_STATE_AT_LOWER_Y:
        MOBY(gate_moby)->pos.y = saved_position[1] + -3.75f;
        break;
    }
}
