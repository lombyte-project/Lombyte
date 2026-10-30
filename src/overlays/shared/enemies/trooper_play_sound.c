/* Troopers: plays a trooper sound. */
#include "types.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"

/* Ported from rac1-decomp (src/overlays/shared/sound_0028EB98.c: func_L00_0028EF68), where it is exact; names translated to the US level program. */

extern int allocate_voice_slot(void *, int, int, int, int) __asm__("FUN_0022d7f0");

int trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");

int trooper_play_sound(int i, int voice_flags, int moby, int k) {
    char *class_sound_data = D_L00_00197300[class_slot[k]];
    char *voice_slot;
    int   voice_slot_index;
    if (class_sound_data == 0) {
        return -1;
    }
    if (i >= *(unsigned char *)(class_sound_data + 0xD)) {
        return -1;
    }
    voice_slot_index = allocate_voice_slot(
        *(char **)(class_sound_data + 0x28) + i * 32, voice_flags, moby, 0, 0x400);
    if (voice_slot_index >= 0) {
        voice_slot = D_0013E533 + 0x1D + voice_slot_index * 0x70;
        *(int *)(voice_slot + 0x88) = moby;
        *(short *)(voice_slot + 0x7E) = i;
    }
    return voice_slot_index;
}
