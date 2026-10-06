#include "types.h"

struct M2c_D_001516D0 {
    u8 pad_0[0x8];
    s16 unk8;
    u8 pad_A[0x2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
};
extern struct M2c_D_001516D0 D_001516D0;
extern u8 D_001E8650[];
extern s32 DebugPrint();
extern s32 RaiseKernelTrap();
extern s32 snd_stream_safe_cd_read() __asm__("func_0012ED58");
s32 start_audio_stream_read(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_00216788");

s32 start_audio_stream_read(s32 arg0, s32 arg1, s32 arg2) {
    if ((D_001516D0.unk8 == 0) && (arg2 != 0)) {
        if (snd_stream_safe_cd_read(arg1, arg2, arg0, ((u8 *)&D_001516D0 + 0x30)) != 0) {
            D_001516D0.unk14 = arg0;
            D_001516D0.unk8 = 1;
            D_001516D0.unkC = arg1;
            D_001516D0.unk10 = arg2;
            return arg2 << 0xB;
        }
        DebugPrint(D_001E8650);
        RaiseKernelTrap();
    }
    return 0;
}
