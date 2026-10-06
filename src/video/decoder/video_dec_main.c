#include "types.h"
#include "rnc/video/decoder/video_dec_main.h"

extern s32 D_0016120C;
extern s32 vi_buf_reset() __asm__("func_0023BCC0");
extern s32 func_0023CC80();
extern s32 func_0023CC88();
extern s32 dec_bs0() __asm__("func_0023CEC8");
extern s32 func_0023D1E8();

void video_dec_main(s32 video_dec) __asm__("FUN_0023ce28");

void video_dec_main(s32 video_dec) {
    vi_buf_reset(video_dec + 0x48);
    func_0023D1E8(D_0016120C + 0xD9168);
    dec_bs0(video_dec);
    while (((struct VideoDec *)D_0016120C)->unkD9174 != 0) {
        if (func_0023CC80(video_dec) == 1) {
            break;
        }
    }
    func_0023CC88(video_dec, 3);
}

extern __typeof__(video_dec_main) func_0023CE28 __attribute__((alias("FUN_0023ce28")));
