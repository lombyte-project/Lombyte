#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_H

#include "types.h"

void draw_page_contents(void) __asm__("FUN_L00_001f8f50");
void draw_dialog_background_and_text(void) __asm__("FUN_L00_001f8ec8");
void draw_framebuffer_with_timed_text(void) __asm__("FUN_L00_001f8f78");
void draw_timed_text(void *, s32) __asm__("FUN_L00_001fcca0");
void level_draw_dialog_text(void) __asm__("FUN_L00_002018c8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_H */
