#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_H

#include "types.h"

/* One line of timed (subtitle) text: shown from frame `start` for `duration` frames. */
typedef struct {
    s32 text_id;
    s16 x;
    s16 y;
    s16 start;
    s16 duration;
    s16 mode;
    s16 separator_mode;
} LevelTextEntry;

#define LEVEL_TIMED_TEXT_COUNT 183

extern LevelTextEntry timed_text_entries[LEVEL_TIMED_TEXT_COUNT] __asm__("D_L00_00173080");

void draw_page_contents(void) __asm__("FUN_L00_001f8f50");
void draw_dialog_background_and_text(void) __asm__("FUN_L00_001f8ec8");
void draw_framebuffer_with_timed_text(void) __asm__("FUN_L00_001f8f78");
void draw_timed_text(LevelTextEntry *, s32) __asm__("FUN_L00_001fcca0");
void level_draw_dialog_text(void) __asm__("FUN_L00_002018c8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_H */
