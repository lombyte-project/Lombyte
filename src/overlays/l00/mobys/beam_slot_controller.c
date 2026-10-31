/* Level 00 moby class beam_slot_controller (0x5bf): resets the beam slots, then dispatches and releases them each frame. */
#include "types.h"
#include "sda.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0xF];
    u8 alpha;
} SlotMoby;

extern int  beam_slot_state[3] __asm__("D_L00_00161BF8") MACRO_ADDR;
extern int  beam_slot_id[3] __asm__("D_L00_00161C88") MACRO_ADDR;
extern void release_beam_slot(s32) __asm__("FUN_L00_002e20f8");
extern void update_slot_panes_and_effects(s32, s32) __asm__("FUN_L00_002e2250");
extern void draw_active_beam_slots() __asm__("FUN_L00_002e2af0");

void beam_slot_controller_update(SlotMoby *slot_controller) __asm__("FUN_L00_002e1df0");

/* Moby class 1471 */
void beam_slot_controller_update(SlotMoby *slot_controller) {
    enum {
        SLOT_CONTROLLER_STATE_INITIALIZE = 0,
        SLOT_CONTROLLER_STATE_PROCESS_SLOTS = 1
    };
    s32 i;
    s32 j;
    s32 dispatched_slot_count;
    s32 slot_state;

    switch (slot_controller->state) {
    case SLOT_CONTROLLER_STATE_INITIALIZE:
        for (j = 0; j < 3; j++) {
            beam_slot_state[j] = 0;
            beam_slot_id[j] = 0;
        }
        slot_controller->state = 1;
        slot_controller->alpha = 0xFF;
        break;
    case SLOT_CONTROLLER_STATE_PROCESS_SLOTS:
        dispatched_slot_count = 0;
        for (i = 0; i < 3; i++) {
            slot_state = beam_slot_state[i];
            if (slot_state == 1) {
            } else if (slot_state == 4) {
                release_beam_slot(i);
            } else if (slot_state != 0) {
                dispatched_slot_count++;
                update_slot_panes_and_effects(beam_slot_id[i], i);
                beam_slot_state[i] = 4;
            }
        }
        if (dispatched_slot_count != 0) {
            add_draw_callback((s32)draw_active_beam_slots, (s32)slot_controller);
        }
        break;
    }
}
