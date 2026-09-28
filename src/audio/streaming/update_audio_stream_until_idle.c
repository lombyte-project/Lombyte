#include "types.h"
#include "rnc/audio_streaming_update_audio_stream_until_idle_types.h"
extern struct M2c_D_001516D0 D_001516D0;

extern void ReadGlobalTableEntry(void);
extern s32 func_0012DC80();
extern s32 func_0012EB00();
extern s32 func_00216290();
extern s32 sceGsSyncV();
s16 update_audio_stream_until_idle(s32 arg0) __asm__("FUN_002168a8");

s16 update_audio_stream_until_idle(s32 arg0) {
    if (arg0 != 0) {
        if (D_001516D0.unk8 != 0) {
            do {
                sceGsSyncV(0);
                func_00216290();
                func_0012EB00();
                func_0012DC80();
                ReadGlobalTableEntry();
            } while (D_001516D0.unk8 != 0);
        }
    } else {
        func_00216290();
        func_0012EB00();
        func_0012DC80();
        ReadGlobalTableEntry();
    }
    return D_001516D0.unk8;
}
