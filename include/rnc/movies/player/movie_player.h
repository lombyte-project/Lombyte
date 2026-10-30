#ifndef LOMBYTE_RNC_MOVIES_PLAYER_MOVIE_PLAYER_H
#define LOMBYTE_RNC_MOVIES_PLAYER_MOVIE_PLAYER_H

#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"
#include "rnc/movies/decoder/read_buf.h"
#include "rnc/movies/decoder/read_mpeg.h"
#include "rnc/movies/decoder/vi_buf.h"
#include "rnc/movies/decoder/video_dec.h"
#include "rnc/movies/decoder/vo_buf.h"

#define MOVIE_VI_BUF_BLOCKS      0x100
#define MOVIE_VI_BUF_TIMESTAMPS  0x200
#define MOVIE_AUDIO_BUF_SIZE     0x2000
#define MOVIE_DECODER_STACK_SIZE 0x4000

/* Work area of one movie, handed to play_mpeg_movie and laid out by
   init_all. The read ring is also the anyData of the stream callbacks. */
struct MoviePlayer {
    struct ReadBuf read_buf;
    u8             pad_5000C[0x34];
    u8             audio_buf[MOVIE_AUDIO_BUF_SIZE];           /* 0x50040: audio_dec_create */
    u8             vi_buf_data[MOVIE_VI_BUF_BLOCKS][0x800];   /* 0x52040: video_dec_create */
    u8             decoder_stack[MOVIE_DECODER_STACK_SIZE];   /* 0xD2040: video_dec_main thread */
    struct ViBufTimeStamp vi_buf_ts[MOVIE_VI_BUF_TIMESTAMPS]; /* 0xD6040 */
    struct MpegCdStream   cd_stream;                          /* 0xD9040 */
    struct VideoDec       video_dec;                          /* 0xD9048 */
    struct AudioDec       audio_dec;                          /* 0xD9100 */
    u8                    pad_D9164[0x4];
    struct VoBuf          vo_buf; /* 0xD9168 */
};

#endif /* LOMBYTE_RNC_MOVIES_PLAYER_MOVIE_PLAYER_H */
