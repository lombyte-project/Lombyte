#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern void vi_buf_get_ts(struct ViBuf *, u64 *) __asm__("func_0023C920");

struct Out {
    u8  pad[8];
    u64 a;
    u64 b;
};

s32 get_mpeg_timestamp(s32 unused_parameter, struct Out *out) __asm__("FUN_0023d140");

s32 get_mpeg_timestamp(s32 unused_parameter, struct Out *timestamp_output) {
    u64 timestamp_values[3];

    vi_buf_get_ts(&movie_player->video_dec.vi_buf, timestamp_values);
    timestamp_output->a = timestamp_values[0];
    timestamp_output->b = timestamp_values[1];
    return 1;
}

extern __typeof__(get_mpeg_timestamp) func_0023D140 __attribute__((alias("FUN_0023d140")));
extern __typeof__(get_mpeg_timestamp) D_0023D140 __attribute__((alias("FUN_0023d140")));
