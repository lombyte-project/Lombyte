#ifndef LOMBYTE_RNC_UI_HELP_HELP_STATE_H
#define LOMBYTE_RNC_UI_HELP_HELP_STATE_H

#include "types.h"

/* The help message box state (D_001996D0): update_help_state runs it,
   link_localized_display_text lays out the box around help_msgs[msg]. */
typedef struct {
    s32 state;             /* 0x00 */
    s32 timer;             /* 0x04 */
    s32 half_width;        /* 0x08: half the text width + 10 */
    s32 half_height;       /* 0x0C: half the text height + 5 */
    s32 center_x;          /* 0x10 */
    s32 center_y;          /* 0x14: kept 12 px above the screen bottom */
    s32 frame_half_width;  /* 0x18: half size of the frame drawn now */
    s32 frame_half_height; /* 0x1C */
    s32 msg;               /* 0x20: help_msgs index shown */
    s32 pending;           /* 0x24 */
    s32 entry;             /* 0x28 */
    s32 count;             /* 0x2C: entries in the help_msgs table */
    s32 started;           /* 0x30: draw_help draws nothing while 0 */
    s32 delay;             /* 0x34 */
    s16 queued;            /* 0x38 */
    s16 shown;             /* 0x3A */
    s32 wait[1];           /* 0x3C */
} HelpState;

extern HelpState help_state __asm__("D_001996D0");

#endif /* LOMBYTE_RNC_UI_HELP_HELP_STATE_H */
