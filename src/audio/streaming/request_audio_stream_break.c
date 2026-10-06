#include "types.h"
#include "asm.h"
#include "rnc/audio/streaming/request_audio_stream_break.h"
#include "types.h"

#include "rnc/audio/streaming/request_audio_stream_break.h"
#include "types.h"

extern struct MusicStreamState D_001516D0;
extern s32 snd_stream_safe_cd_break() __asm__("func_0012EEA8");
void request_audio_stream_break(s32 arg0) __asm__("FUN_002166e8");

void request_audio_stream_break(s32 arg0) {
    if (D_001516D0.unk8 != 0) {
        snd_stream_safe_cd_break();
        D_001516D0.unkA = 1;
    }
}
