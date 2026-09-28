#include "types.h"
#include "asm.h"
#include "rnc/audio_streaming_finish_audio_stream_read_types.h"
#include "types.h"

#include "rnc/audio_streaming_finish_audio_stream_read_types.h"
#include "types.h"


extern struct M2c_D_001516D0 D_001516D0;
extern s32 func_0012EEF0();
void finish_audio_stream_read(s32 arg0) __asm__("FUN_00216950");

void finish_audio_stream_read(s32 arg0) {
    if (arg0 == 1) {
        D_001516D0.unk8 = 0;
        if (func_0012EEF0() != 0) {
            D_001516D0.unk8 = 2;
        }
    }
}
