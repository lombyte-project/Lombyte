#ifndef LOMBYTE_RNC_GAME_STARTLEVEL_H
#define LOMBYTE_RNC_GAME_STARTLEVEL_H

#include "types.h"

/* A sound definition, 0x20 bytes (MobyClass.sound_defs entries); startlevel
   fills in the bank loaded from disc_table.sound_bank. */
typedef struct {
    s32 pad[7];
    s32 bank;
} SoundDef;

/* The boot level's sound definitions: startlevel publishes this table as
   level_sound_defs with level_sound_def_count 7. */
extern volatile SoundDef l00_primary_sound_defs[7] __asm__("D_00186100");

/* Five more definitions: startlevel fills their banks, transition_load_wad
   hands the table to moby class 0x472 with a count of 5. */
extern volatile SoundDef boot_sound_defs_b[5] __asm__("D_001861E0");

#endif /* LOMBYTE_RNC_GAME_STARTLEVEL_H */
