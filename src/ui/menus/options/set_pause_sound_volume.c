#include "sda.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/audio/voice_pool.h"

extern s32 sound_volume_macro __asm__("D_0015EDF0") MACRO_ADDR;
int        set_pause_sound_volume(void) __asm__("FUN_0021cb00");

int set_pause_sound_volume(void) {
    voice_pool.group_2_volume = sound_volume_macro * 8 / 10;
    return 0;
}

extern __typeof__(set_pause_sound_volume) func_0021CB00 __attribute__((alias("FUN_0021cb00")));
