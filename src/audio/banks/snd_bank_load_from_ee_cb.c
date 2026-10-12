#include "types.h"
extern s32 snd_cd_load_error __asm__("D_0015EC88") __attribute__((sda));
extern s32 snd_cd_callback_pending __asm__("D_0015ECC8") __attribute__((sda));
extern s32 snd_cd_callback_fn __asm__("D_0015ECD0") __attribute__((sda));
extern s64 snd_cd_callback_data __asm__("D_0015ECD8");
extern u32 snd_cd_callback_arg __asm__("D_0015ED00");
extern s32 snd_cd_bank_load_request[4] __asm__("D_00137B40");
extern u8 D_0015EBE8[];
extern char snd_bank_load_from_ee_progress_string[] __asm__("D_00153EC0");
extern char snd_bank_load_from_ee_cd_busy_string[] __asm__("D_00153EF8");
extern char snd_non_idle_error_string[] __asm__("D_00153D20");
extern s32 printf(const char *, ...);
extern s32 snd_stream_safe_cd_sync(s32) __asm__("func_0012EE08");
extern s32 snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern s32 FlushCache(s32);
extern s32 SceSifCheckStatRpc(void *);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

// Retail keeps this symbol out of small data here: the size decides the
// HI16/LO16 pair instead of a gp-relative reference.
void snd_bank_load_from_ee_cb(s32 cmd, s32 arg, s64 data) __asm__("FUN_0012e088");

void snd_bank_load_from_ee_cb(s32 cmd, s32 arg, s64 data) {
    snd_cd_load_error = 0;
    if (snd_cd_callback_pending != 0) {
        printf(snd_bank_load_from_ee_progress_string);
        return;
    }
    if (snd_stream_safe_cd_sync(1) == 1) {
        printf(snd_bank_load_from_ee_cd_busy_string);
        return;
    }
    snd_cd_bank_load_request[0] = cmd;
    snd_cd_callback_arg = 0xFFFFFFFF;
    snd_cd_callback_fn = arg;
    snd_cd_callback_data = data;
    while (SceSifCheckStatRpc(D_0015EBE8) != 0) {
        printf(snd_non_idle_error_string);
        snd_flush_sound_commands();
        FlushCache(0);
    }
    snd_cd_callback_pending = 1;
    sceSifCallRpc(D_0015EBE8, 0x57, 1, snd_cd_bank_load_request, 4, &snd_cd_callback_arg, 4, 0, 0);
}

extern __typeof__(snd_bank_load_from_ee_cb) func_0012E088 __attribute__((alias("FUN_0012e088")));
