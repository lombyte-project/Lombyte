#include "types.h"
#include "rnc/movies/decoder/read_mpeg.h"

s32 mpeg_cd_stream_open(struct MpegCdStream *stream,
                        s32                  first_sector,
                        s32                  byte_count) __asm__("func_0023BA48");

s32 mpeg_cd_stream_open(struct MpegCdStream *stream, s32 first_sector, s32 byte_count) {
    stream->next_sector = first_sector;
    stream->byte_count = byte_count;
    return 1;
}
