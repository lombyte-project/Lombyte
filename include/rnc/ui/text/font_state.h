#ifndef LOMBYTE_RNC_UI_TEXT_FONT_STATE_H
#define LOMBYTE_RNC_UI_TEXT_FONT_STATE_H

#include "types.h"

extern unsigned short font_vu_program_tag[] __asm__("D_0010E800");
extern u8 font_vu_program[] __asm__("D_0010E810");
extern f32 font_depth_bias __asm__("D_0015F348") __attribute__((sda));
extern s32 font_window_active __asm__("D_0015F4A0");
extern s32 font_state __asm__("D_0015F620");
extern s32 text_shadow_x __asm__("D_001601B8") __attribute__((sda));
extern s32 text_shadow_y __asm__("D_001601BC") __attribute__((sda));
extern u8 debug_font_buffer[] __asm__("D_001AABC0");

#endif /* LOMBYTE_RNC_UI_TEXT_FONT_STATE_H */
