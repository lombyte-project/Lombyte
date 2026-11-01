#ifndef LOMBYTE_RNC_MOVIES_DECODER_VIDEO_INPUT_H
#define LOMBYTE_RNC_MOVIES_DECODER_VIDEO_INPUT_H

#include "types.h"

struct ViBuf;

s32 vi_buf_count(struct ViBuf *) __asm__("FUN_0023c610");
s32 vi_buf_delete(struct ViBuf *) __asm__("FUN_0023c5b8");
s32 vi_buf_restart_dma(struct ViBuf *) __asm__("FUN_0023c280");

#endif /* LOMBYTE_RNC_MOVIES_DECODER_VIDEO_INPUT_H */
