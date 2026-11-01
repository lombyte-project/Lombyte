#ifndef LOMBYTE_RNC_MOVIES_DECODER_H
#define LOMBYTE_RNC_MOVIES_DECODER_H

#include "rnc/sdk/libmpeg.h"
#include "types.h"

struct MpegCdStream;
struct VideoDec;

s32 mpeg_cd_stream_close(struct MpegCdStream *) __asm__("func_0023BA58");
s32 mpeg_cd_stream_open(struct MpegCdStream *, s32, s32) __asm__("func_0023BA48");
s32 video_dec_create(struct VideoDec *, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_0023cac8");
s32 video_dec_is_flushed(struct VideoDec *) __asm__("FUN_0023cde0");
s32 video_dec_set_stream(struct VideoDec *, s32, s32, MpegStreamCallback, void *) __asm__("FUN_0023cbd0");

#endif /* LOMBYTE_RNC_MOVIES_DECODER_H */
