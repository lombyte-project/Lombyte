/* Vendor: sets the volume of a voice; returns 1. */
#include "types.h"
#include "asm.h"
#include "rnc/audio/voice_pool.h"

/* Sets the volume of voice i; returns 1. */
s32 vendor_volume(s32 i, s32 v) __asm__("FUN_L01_002a1968");

s32 vendor_volume(s32 i, s32 volume_value) {
    VoicePoolWindow *voice_entry = (VoicePoolWindow *)((u8 *)&voice_pool + i * 0x70);
    voice_entry->voice.volume = volume_value;
    return 1;
}
