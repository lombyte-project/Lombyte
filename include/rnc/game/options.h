#ifndef LOMBYTE_RNC_GAME_OPTIONS_H
#define LOMBYTE_RNC_GAME_OPTIONS_H

#include "types.h"

extern int           pause_sound_volume __asm__("D_0013E5A0") __attribute__((section(".data")));
extern u8            big_head_cheat_enabled __asm__("D_0015EDB0");
extern unsigned char opt_mirror_anim __asm__("D_0015EDB5");
extern int           invert52 __asm__("D_0015EDDC");
extern int           opt_cam_invert __asm__("D_0015EDE0");
extern s32           playback_mode __asm__("D_0015EDE8");
extern s32           music_volume __asm__("D_0015EDEC");
extern s32           sound_volume __asm__("D_0015EDF0");
extern s32           alternate_animation_sequence __asm__("D_0015EE20");
extern s32           selected_save_slot __asm__("D_0015EE34");
extern s32           boot_level_active __asm__("D_0015F5E8");

#endif /* LOMBYTE_RNC_GAME_OPTIONS_H */
