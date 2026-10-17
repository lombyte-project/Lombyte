#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/video/decoder/decoder_globals.h"
extern void vi_buf_get_ts(s32, u64 *) __asm__("func_0023C920");
struct Out {
    u8 pad[8];
    u64 a;
    u64 b;
};
s32 get_mpeg_timestamp(s32 arg0, struct Out *out) __asm__("FUN_0023d140");

s32 get_mpeg_timestamp(s32 unused_parameter, struct Out *out) {
    u64 tmp[3];

    vi_buf_get_ts(decoder_state + 0xD9090, tmp);
    out->a = tmp[0];
    out->b = tmp[1];
    return 1;
}

extern __typeof__(get_mpeg_timestamp) func_0023D140 __attribute__((alias("FUN_0023d140")));
extern __typeof__(get_mpeg_timestamp) D_0023D140 __attribute__((alias("FUN_0023d140")));
