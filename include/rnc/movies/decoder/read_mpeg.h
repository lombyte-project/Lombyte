#ifndef LOMBYTE_RNC_MOVIES_DECODER_READ_MPEG_H
#define LOMBYTE_RNC_MOVIES_DECODER_READ_MPEG_H

#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/movies/decoder/read_buf.h"

/* The caller reserves [context+0xD9040, context+0xD9048) for this
   stream. read_mpeg reads byte_count; read_cd_stream_sectors advances
   the word at +4 as a 2048-byte sector index. No pathname lives here. */
struct MpegCdStream {
    s32 byte_count;
    s32 next_sector;
};

/* Defined in rnc/movies/decoder/video_dec.h. */
struct VideoDec;

#endif /* LOMBYTE_RNC_MOVIES_DECODER_READ_MPEG_H */
