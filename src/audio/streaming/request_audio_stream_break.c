#include "types.h"
#include "asm.h"
#include "rnc/audio_streaming_request_audio_stream_break_types.h"
#include "types.h"

#include "rnc/audio_streaming_request_audio_stream_break_types.h"
#include "types.h"


extern struct M2c_D_001516D0 D_001516D0;
extern s32 func_0012EEA8();
void request_audio_stream_break(s32 arg0) __asm__("FUN_002166e8");

void request_audio_stream_break(s32 arg0) {
    if (D_001516D0.unk8 != 0) {
        func_0012EEA8();
        D_001516D0.unkA = 1;
    }
}
