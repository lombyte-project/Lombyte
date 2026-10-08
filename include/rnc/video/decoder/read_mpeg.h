#ifndef LOMBYTE_RNC_VIDEO_DECODER_READ_MPEG_H
#define LOMBYTE_RNC_VIDEO_DECODER_READ_MPEG_H

#include "types.h"
#include "rnc/input/pad_state.h"

struct Globals_0013E550 {
    u8 pad_0[0x5C];
    s32 unk5C;
};

/* Minimum accessed layout; init_all reserves through context+0x50040.
   That reservation does not prove the trailing bytes are ReadBuf fields. */
struct ReadBuf {
    /* readBufCreate / BeginPut / EndPut / BeginGet / EndGet:
       0x50000-byte ring followed by its three signed 32-bit counters. */
    u8 data[0x50000];
    s32 write_position;  /* 0x50000: modulo capacity */
    s32 available_bytes; /* 0x50004 */
    s32 capacity;        /* 0x50008: initialized to 0x50000 */
};

/* The caller reserves [context+0xD9040, context+0xD9048) for this
   stream. read_mpeg reads byte_count; read_cd_stream_sectors advances
   the word at +4 as a 2048-byte sector index. No pathname lives here. */
struct MpegCdStream {
    s32 byte_count;
    s32 next_sector;
};

/* Defined in rnc/video/decoder/video_dec.h. */
struct VideoDec;

#endif /* LOMBYTE_RNC_VIDEO_DECODER_READ_MPEG_H */
