#include "types.h"
#include "rnc/audio/sound_state.h"

extern s32  snd_cd_bank_load_request[] __asm__("D_00137B40");
extern char snd_non_idle_error_string[] __asm__("D_00153D20");
extern u8   snd_bank_load_by_loc_rpc_error_string[] __asm__("D_00153DF8");
extern u8   sound_stream_client[] __asm__("D_0015EBE8");
extern s32  snd_cd_load_error __asm__("D_0015EC88") __attribute__((sda));
extern u32  snd_cd_callback_arg __asm__("D_0015ED00");
extern s32  printf(const char *, ...);
extern s32  snd_stream_safe_cd_sync(s32) __asm__("func_0012EE08");
extern void snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void FlushCache(s32);
extern s32  SceSifCheckStatRpc(void *);
extern s32  sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 snd_bank_load_by_loc(s32 location, s32 byte_offset) __asm__("FUN_0012df20");

s32 snd_bank_load_by_loc(s32 location, s32 byte_offset) {
    snd_cd_load_error = 0;
    if (snd_cd_callback_pending != 0) {
        printf("snd_BankLoadByLoc: Load already in progress!\n");
        return 0;
    }
    if (snd_stream_safe_cd_sync(1) == 1) {
        printf("snd_BankLoadByLoc: CD BUSY!\n");
        return 0;
    }
    snd_cd_bank_load_request[1] = byte_offset;
    snd_cd_callback_arg = 0xFFFFFFFF;
    snd_cd_bank_load_request[0] = location;
    while (SceSifCheckStatRpc(sound_stream_client) != 0) {
        printf(snd_non_idle_error_string);
        snd_flush_sound_commands();
        FlushCache(0);
    }
    if (sceSifCallRpc(
            sound_stream_client, 3, 1, snd_cd_bank_load_request, 8, &snd_cd_callback_arg, 4, 0, 0) <
        0) {
        printf(snd_bank_load_by_loc_rpc_error_string);
        snd_cd_load_error = 0x106;
        return 0;
    }
    if (snd_cd_callback_arg == 0xFFFFFFFF) {
        do {
            FlushCache(0);
        } while (snd_cd_callback_arg == 0xFFFFFFFF);
    }
    return snd_cd_callback_arg;
}

extern __typeof__(snd_bank_load_by_loc) func_0012DF20 __attribute__((alias("FUN_0012df20")));
