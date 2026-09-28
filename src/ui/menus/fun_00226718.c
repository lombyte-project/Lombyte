#include "types.h"
struct M2c_D_001D5BF0 {
    u8 pad_0[0xCB];
    u8 unkCB;
};

extern s16 D_001516D8[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern u32 D_001D608C[];
extern s32 request_audio_stream_break() __asm__("FUN_002166e8");
s32 FUN_00226718(s32 arg0) {
    if (D_001516D8[0] != 0) {
        if (D_001D5BF0.unkCB != 0) {
            request_audio_stream_break();
            D_001D5BF0.unkCB = 0U;
        }
    }
    D_001D608C[0] = 0;
    *(u32 *)0x160350 = 0;
    return 0;
}
