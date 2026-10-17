#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/video/decoder/decoder_globals.h"

extern s32 ChangeThreadPriority();
extern s32 GetThreadId();
extern s32 read_mpeg() __asm__("func_0023A460");
extern s32 init_all() __asm__("func_0023A7C0");
extern s32 term_all() __asm__("func_0023AA68");

s32 play_mpeg_movie(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_0023a3b8");

s32 play_mpeg_movie(s32 movie_source, s32 arg1, s32 decoder_buffer_base, s32 decoder_state_base, s32 arg4) {
    decoder_buffer_address = decoder_buffer_base;
    decoder_state = decoder_state_base;
    ChangeThreadPriority(GetThreadId(), 1);
    if (init_all(movie_source, arg1, arg4) != 0) {
        read_mpeg(decoder_state + 0xD9048, decoder_state, decoder_state + 0xD9040);
    }
    term_all();
    *(s32 *)0x161208 = 0;
    *(s32 *)0x16120C = 0;
    return 0;
}

extern s32 func_0023A3B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_0023a3b8")));
