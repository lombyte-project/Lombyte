#ifndef LOMBYTE_RNC_MOVIES_DECODER_DECODER_GLOBALS_H
#define LOMBYTE_RNC_MOVIES_DECODER_DECODER_GLOBALS_H

#include "types.h"

extern s32          decode_mode __asm__("D_0015EED8");
extern volatile s32 movie_display_active __asm__("D_001611E0");
extern s32          decoder_buffer_address __asm__("D_00161208");
struct MoviePlayer;
extern struct MoviePlayer *movie_player __asm__("D_0016120C");
extern s32                 decoder_thread __asm__("D_00161210");

#endif /* LOMBYTE_RNC_MOVIES_DECODER_DECODER_GLOBALS_H */
