#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/video/decoder/decoder_globals.h"

/* Movie decoder context at *D_0016120C (VideoDec lives at +0xD9048). */
struct MovieGlobals {
    u8 pad_0[0xD9174];
    s32 unkD9174;
};

extern s32 vi_buf_reset() __asm__("func_0023BCC0");
extern s32 func_0023CC80();
extern s32 video_dec_set_state() __asm__("func_0023CC88");
extern s32 dec_bs0() __asm__("func_0023CEC8");
extern s32 vo_buf_reset() __asm__("func_0023D1E8");

void video_dec_main(s32 video_dec) __asm__("FUN_0023ce28");

void video_dec_main(s32 video_dec) {
    vi_buf_reset(video_dec + 0x48);
    vo_buf_reset(decoder_state + 0xD9168);
    dec_bs0(video_dec);
    while (((struct MovieGlobals *)decoder_state)->unkD9174 != 0) {
        if (func_0023CC80(video_dec) == 1) {
            break;
        }
    }
    video_dec_set_state(video_dec, 3);
}

extern __typeof__(video_dec_main) func_0023CE28 __attribute__((alias("FUN_0023ce28")));
