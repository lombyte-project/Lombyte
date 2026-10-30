#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WORLD_LOAD_LEVEL_GAMEPLAY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WORLD_LOAD_LEVEL_GAMEPLAY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/world/load_level_gameplay.c. */
extern s32 saved_sound_bank_handle __asm__("D_L00_00160004");
extern s32 tfrag_texture_ref_count __asm__("D_L00_00160F54");
extern s32 tie_texture_ref_count __asm__("D_L00_00160FD4");
extern s32 shrub_texture_ref_count __asm__("D_L00_001604AC");
extern s32 moby_class_reference_count __asm__("D_L00_00160008");
extern s32 moby_class_count __asm__("D_L00_0015FFC0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WORLD_LOAD_LEVEL_GAMEPLAY_GLOBALS_H */
