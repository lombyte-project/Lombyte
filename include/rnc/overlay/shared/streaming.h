#ifndef LOMBYTE_RNC_OVERLAY_SHARED_STREAMING_H
#define LOMBYTE_RNC_OVERLAY_SHARED_STREAMING_H

#include "types.h"

void load_chunks_with_optional_fade(s32, s32) __asm__("FUN_L00_00245610");
void load_scene_chunks_with_fade(s32) __asm__("FUN_L00_002454c8");
void start_indexed_disc_stream(s32) __asm__("FUN_L02_00298c68");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_STREAMING_H */
