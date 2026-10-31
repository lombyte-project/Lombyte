#ifndef LOMBYTE_RNC_MOVIES_DECODER_VIDEO_DEC_SET_STREAM_H
#define LOMBYTE_RNC_MOVIES_DECODER_VIDEO_DEC_SET_STREAM_H

#include "rnc/sdk/libmpeg.h"
#include "types.h"

struct VideoDec;

s32 video_dec_set_stream(struct VideoDec *, s32, s32, MpegStreamCallback, void *) __asm__("FUN_0023cbd0");

#endif /* LOMBYTE_RNC_MOVIES_DECODER_VIDEO_DEC_SET_STREAM_H */
