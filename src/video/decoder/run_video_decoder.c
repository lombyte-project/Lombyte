#include "types.h"
#include "rnc/video_decoder_run_video_decoder_types.h"

extern s32 D_0016120C;
extern s32 func_0023BCC0();
extern s32 func_0023CC80();
extern s32 func_0023CC88();
extern s32 func_0023CEC8();
extern s32 func_0023D1E8();

void run_video_decoder(s32 arg0) __asm__("FUN_0023ce28");

void run_video_decoder(s32 arg0)
{
    func_0023BCC0(arg0 + 0x48);
    func_0023D1E8(D_0016120C + 0xD9168);
    func_0023CEC8(arg0);
    while (((struct M2c_temp_2_25 *)D_0016120C)->unkD9174 != 0) {
        if (func_0023CC80(arg0) == 1) {
            break;
        }
    }
    func_0023CC88(arg0, 3);
}

extern __typeof__(run_video_decoder) func_0023CE28 __attribute__((alias("FUN_0023ce28")));
