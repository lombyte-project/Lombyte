/* Sound emitter type 5: a voice that stays at the listener position while the camera field at +0x254 is set. */
#include "types.h"
#include "qcopy.h"
#include "rnc/audio/sound_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/play_voice.h"

extern char D_L01_00167240[];

void update_listener_emitter(int sound_owner) __asm__("FUN_L01_0031a078");

void update_listener_emitter(int sound_owner) {
    char *sound_data = *(char **)(sound_owner + 8);
    int   voice_slot_index = *(int *)(sound_data + 4);
    char *voice_slot;
    int   flag;
    qcopy(voice_positions + voice_slot_index * 0x70, D_L01_00167240);
    voice_slot = voice_positions - 0x90 + voice_slot_index * 0x70;
    flag = *(int *)(voice_slot + 0x8C) == sound_owner && *(unsigned char *)(voice_slot + 0x74) != 0;
    if (*(int *)(D_L01_00167240 + 0x254) != 0) {
        if (!flag) {
            *(int *)(sound_data + 4) = play_voice(*(int *)sound_data, 0x15, sound_owner, 0x400);
        }
    } else {
        if (flag) {
            release_voice_slot(voice_slot_index);
            *(int *)(sound_data + 4) = -1;
        }
    }
}
