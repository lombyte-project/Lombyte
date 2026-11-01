#ifndef LOMBYTE_RNC_UI_HELP_WINDOW_STATE_H
#define LOMBYTE_RNC_UI_HELP_WINDOW_STATE_H

#include "types.h"

/* RC1 help window at D_001996D0; level copies keep this layout.
 * Geometry is measured by link_localized_display_text and drawn by draw_help.
 * Keep the integer fields: state values are not an enum-sized storage type. */
typedef struct HelpWindowState {
    s32 state;               /* 0x00 */
    s32 state_timer;         /* 0x04 */
    s32 target_half_width;   /* 0x08 */
    s32 target_half_height;  /* 0x0C */
    s32 center_x;            /* 0x10 */
    s32 center_y;            /* 0x14 */
    s32 frame_half_width;    /* 0x18 */
    s32 frame_half_height;   /* 0x1C */
    s32 message_index;       /* 0x20 */
    s32 pending_message_id;  /* 0x24: -1 when no request is pending */
    s32 help_record_index;   /* 0x28 */
    s32 message_count;       /* 0x2C */
    s32 input_delay_elapsed; /* 0x30 */
    s32 input_delay_timer;   /* 0x34 */
    s16 queued;              /* 0x38 */
    s16 shown;               /* 0x3A */
    s32 wait_timer[1];       /* 0x3C: array preserves timer call sites */
} HelpWindowState;

enum HelpWindowPhase {
    HELP_WINDOW_IDLE = 0,
    HELP_WINDOW_OPENING = 1,
    HELP_WINDOW_PROMPT = 2,
    HELP_WINDOW_EXPANDING = 3,
    HELP_WINDOW_TEXT_FADE_IN = 4,
    HELP_WINDOW_TEXT_VISIBLE = 5,
    HELP_WINDOW_TEXT_FADE_OUT = 6,
    HELP_WINDOW_CLOSING = 7,
    HELP_WINDOW_QUEUED_MESSAGE_WAIT = 8
};
#endif /* LOMBYTE_RNC_UI_HELP_WINDOW_STATE_H */
