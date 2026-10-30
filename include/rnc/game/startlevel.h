#ifndef LOMBYTE_RNC_GAME_STARTLEVEL_H
#define LOMBYTE_RNC_GAME_STARTLEVEL_H

#include "types.h"

/* A sound definition, 0x20 bytes (MobyClass.sound_defs entries); startlevel
   fills in the bank loaded from disc_table.sound_bank. */
typedef struct {
    s32 unk0;
    f32 unk4;        /* 0x04: 128.0f in every boot definition */
    s32 far_volume;  /* 0x08: calculate_voice_distance_volume */
    s32 near_volume; /* 0x0C */
    s32 unk10;
    s32 unk14;
    u8  unk18;
    u8  attenuation_flags; /* 0x19: bit 0 = quadratic distance falloff, else linear */
    s16 sound;             /* 0x1A: sound index (sound_update) */
    s32 bank;              /* 0x1C: bank handle */
} SoundDef;

/* The boot level's sound definitions: startlevel publishes this table as
   level_sound_defs with level_sound_def_count 7. */
extern volatile SoundDef l00_primary_sound_defs[7] __asm__("D_00186100");

/* Five more definitions: startlevel fills their banks, transition_load_wad
   hands the table to moby class 0x472 with a count of 5. */
extern volatile SoundDef boot_sound_defs_b[5] __asm__("D_001861E0");

#endif /* LOMBYTE_RNC_GAME_STARTLEVEL_H */
