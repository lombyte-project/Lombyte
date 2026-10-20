#include "types.h"
#include "rnc/audio/voice_state.h"

/* This window includes the pool header; state is slot-relative offset 4. */
struct VoiceReleaseWindow {
    u8 pad_0[0x74];
    u8 state;
    u8 pad_auto_75[3];
    u8 pad_78[0x10];
    s32 owner;
    s32 owner_context;
};

extern u8 D_0013E550[];
void release_voice_slot(s32 slot_index) __asm__("FUN_0022d798");

/* Cancel a queued start immediately; otherwise request asynchronous stop.
 * Negative indices are ignored. Nonnegative indices must be valid pool
 * indices: retail has no upper-bound check. The legacy name does not imply
 * that an already-started slot becomes free before sound_update/callbacks. */
void release_voice_slot(s32 slot_index) {
    u8 state;
    struct VoiceReleaseWindow *slot;

    if (slot_index < 0) {
        goto done;
    }
    slot = (struct VoiceReleaseWindow *)((slot_index * 0x70) + D_0013E550);
    state = slot->state;
    if (state != VOICE_SLOT_START_QUEUED) {
        goto request_release;
    }
    slot->owner = 0;
    slot->owner_context = 0;
    slot->state = (u8)VOICE_SLOT_FREE;
    return;
request_release:
    if (state == VOICE_SLOT_FREE) {
        goto done;
    }
    if (state == VOICE_SLOT_STOP_PENDING) {
        goto done;
    }
    slot->state = (u8)VOICE_SLOT_STOP_REQUESTED;
done:
    return;
}
