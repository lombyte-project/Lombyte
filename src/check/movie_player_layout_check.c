/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/movies/player/movie_player.h"

SIZE_CHECK(read_buf, struct ReadBuf, 0x5000C);
SIZE_CHECK(vo_tag, struct VoTag, 0x138C0);
SIZE_CHECK(vo_buf, struct VoBuf, 0x14);
OFFSET_CHECK(audio_buf, struct MoviePlayer, audio_buf, 0x50040);
OFFSET_CHECK(vi_buf_data, struct MoviePlayer, vi_buf_data, 0x52040);
OFFSET_CHECK(decoder_stack, struct MoviePlayer, decoder_stack, 0xD2040);
OFFSET_CHECK(vi_buf_ts, struct MoviePlayer, vi_buf_ts, 0xD6040);
OFFSET_CHECK(cd_stream, struct MoviePlayer, cd_stream, 0xD9040);
OFFSET_CHECK(video_dec, struct MoviePlayer, video_dec, 0xD9048);
OFFSET_CHECK(audio_dec, struct MoviePlayer, audio_dec, 0xD9100);
OFFSET_CHECK(vo_buf, struct MoviePlayer, vo_buf, 0xD9168);
