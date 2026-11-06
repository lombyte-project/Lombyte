#ifndef LOMBYTE_RNC_GAME_FREEZE_MODE_STATE_H
#define LOMBYTE_RNC_GAME_FREEZE_MODE_STATE_H

#include "types.h"

/* State of game mode 4 (freeze): the message box mode_freeze_init opens,
   update_mode_freeze steps and draw_dialog_text draws. mode_freeze_init
   writes every field up to 0x28. */
typedef struct FreezeModeState {
    s32   state;            /* 0x00: freeze kind, mode_freeze_init's mode */
    s32   timer;            /* 0x04: frames left (fade-in) */
    char *title;            /* 0x08 */
    char *line1;            /* 0x0C */
    char *line2;            /* 0x10 */
    s32   return_game_mode; /* 0x14: game_mode before the freeze */
    s32   context;          /* 0x18: mode_freeze_init's second argument */
    s32   choice;           /* 0x1C */
    s32 box_timer; /* 0x20: frames counted per state: delay before the prompt fades in, video preview
                        timeout, grow step of the choice box (scale box_timer / 8) */
    s32 fade_step; /* 0x24: counts down to fade the prompt in (1 - fade_step / 30) and to blend the
                        choice box text (fade_step / 8) */
    s32 unk28;     /* 0x28: only cleared by mode_freeze_init */
} FreezeModeState;

extern FreezeModeState freeze __asm__("D_00193300");

#endif /* LOMBYTE_RNC_GAME_FREEZE_MODE_STATE_H */
