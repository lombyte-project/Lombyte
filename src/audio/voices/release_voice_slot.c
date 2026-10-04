#include "types.h"

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

void release_voice_slot(s32 slot_index) {
    u8 state;
    struct VoiceReleaseWindow *slot;

    if (slot_index < 0) {
        goto done;
    }
    slot = (struct VoiceReleaseWindow *)((slot_index * 0x70) + D_0013E550);
    state = slot->state;
    if (state != 7) {
        goto request_release;
    }
    slot->owner = 0;
    slot->owner_context = 0;
    slot->state = 0U;
    return;
request_release:
    if (state == 0) {
        goto done;
    }
    if (state == 6) {
        goto done;
    }
    slot->state = 4U;
done:
    return;
}
