#include "types.h"
#include "rnc/audio_load_types.h"
extern struct M2c_D_001516D0 D_001516D0;

extern void ReadGlobalTableEntry(void);
extern s32 func_0012DC80();
extern s32 func_0012EB00();
extern s32 func_00216290();
extern s32 start_audio_stream_read(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_00216788");
extern s32 sceGsSyncV();

s32 load(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_00216828");

s32 load(s32 arg0, s32 arg1, s32 arg2) {
    s32 result = start_audio_stream_read(arg0, arg1, arg2);

    if (result != 0 && D_001516D0.unk8 != 0) {
        do {
            sceGsSyncV(0);
            func_00216290();
            func_0012EB00();
            func_0012DC80();
            ReadGlobalTableEntry();
        } while (D_001516D0.unk8 != 0);
    }
    return result;
}

extern s32 func_00216828(s32 arg0, s32 arg1, s32 arg2) __attribute__((alias("FUN_00216828")));
