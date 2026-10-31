#include "types.h"
#include "rnc/audio/voice_pool.h"

extern s32 snd_bank_load_by_loc() __asm__("FUN_0012df20");
extern s32 snd_resolve_bank_xrefs() __asm__("FUN_0012e1a8");
extern s32 snd_stream_safe_check_cd_idle() __asm__("FUN_0012ed30");
extern s32 snd_stream_safe_cd_sync() __asm__("FUN_0012ee08");
extern s32 sceCdSync();
s32        load_audio_bank_by_location(s32 location) __asm__("FUN_0022d708");

s32 load_audio_bank_by_location(s32 location) {
    s32 bank_load_result;

    bank_load_result = 0;
    if (voice_pool.bank_load_busy == 0) {
        voice_pool.bank_load_busy = 1;
        sceCdSync(0);
        bank_load_result = snd_bank_load_by_loc(location, 0);
        snd_stream_safe_check_cd_idle(1);
        snd_stream_safe_cd_sync(0);
        snd_resolve_bank_xrefs();
        voice_pool.bank_load_busy = 0;
    }
    return bank_load_result;
}

extern s32 func_0022D708(s32 location) __attribute__((alias("FUN_0022d708")));
