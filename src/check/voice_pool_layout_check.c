/* EE ABI checks against the offsets used by the retail voice allocator,
 * release helper and sound_update. Nothing in this file is linked.
 * See docs/audio-voices.md for the binary identities and partial views. */
#include "rnc/audio/voice_pool.h"

#define CHECK_CAT2(a, b) a##b
#define CHECK_CAT(a, b) CHECK_CAT2(a, b)
#define CHECK(cond) typedef char CHECK_CAT(layout_check_, __LINE__)[(cond) ? 1 : -1]
#define OFFSET(type, field, off) \
    CHECK((unsigned long)&((type *)0)->field == (off))

CHECK(sizeof(VoiceVector) == 0x10);
CHECK(__alignof__(VoiceVector) == 0x10);
CHECK(sizeof(VoiceDefinition) == 0x20);
OFFSET(VoiceDefinition, pitch_bend_min, 0x10);
OFFSET(VoiceDefinition, pitch_bend_max, 0x14);
OFFSET(VoiceDefinition, source_state, 0x18);
OFFSET(VoiceDefinition, sound_index, 0x1A);
OFFSET(VoiceDefinition, bank_handle, 0x1C);
OFFSET(VoiceMoby, position, 0x10);
OFFSET(VoiceMoby, class_id, 0xA6);
CHECK(sizeof(VoiceSlot) == 0x70);
CHECK(__alignof__(VoiceSlot) == 0x10);
OFFSET(VoiceSlot, state, 0x04);
OFFSET(VoiceSlot, definition, 0x08);
OFFSET(VoiceSlot, sound_index, 0x0C);
OFFSET(VoiceSlot, linked_index, 0x0E);
OFFSET(VoiceSlot, volume, 0x10);
OFFSET(VoiceSlot, pitch_bend, 0x14);
OFFSET(VoiceSlot, owner, 0x18);
OFFSET(VoiceSlot, owner_context, 0x1C);
OFFSET(VoiceSlot, position, 0x20);
OFFSET(VoiceSlot, position_offset, 0x30);
OFFSET(VoiceSlot, history_position, 0x40);
OFFSET(VoiceSlot, history_storage, 0x44);
OFFSET(VoicePool, voices[0], 0x70);
OFFSET(VoicePool, voices[29], 0xD20);
CHECK(sizeof(VoicePool) == 0xD90);
OFFSET(VoicePoolWindow, voice, 0x70);
