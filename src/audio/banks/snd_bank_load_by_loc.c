#include "types.h"

extern s32 D_00137B40[];
extern u8 D_00153D20[];
extern u8 D_00153DA8[];
extern u8 D_00153DD8[];
extern u8 D_00153DF8[];
extern u8 D_0015EBE8[];
extern s32 D_0015EC88 __attribute__((sda));
extern s32 D_0015ECC8 __attribute__((sda));
extern u32 D_0015ED00;
extern s32 printf(const char *, ...);
extern s32 snd_stream_safe_cd_sync(s32) __asm__("func_0012EE08");
extern void snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void FlushCache(s32);
extern s32 SceSifCheckStatRpc(void *);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 snd_bank_load_by_loc(s32 location, s32 arg1) __asm__("FUN_0012df20");

s32 snd_bank_load_by_loc(s32 location, s32 arg1) {
    D_0015EC88 = 0;
    if (D_0015ECC8 != 0) {
        printf(D_00153DA8);
        return 0;
    }
    if (snd_stream_safe_cd_sync(1) == 1) {
        printf(D_00153DD8);
        return 0;
    }
    D_00137B40[1] = arg1;
    D_0015ED00 = 0xFFFFFFFF;
    D_00137B40[0] = location;
    while (SceSifCheckStatRpc(D_0015EBE8) != 0) {
        printf(D_00153D20);
        snd_flush_sound_commands();
        FlushCache(0);
    }
    if (sceSifCallRpc(D_0015EBE8, 3, 1, D_00137B40, 8, &D_0015ED00, 4, 0, 0) < 0) {
        printf(D_00153DF8);
        D_0015EC88 = 0x106;
        return 0;
    }
    if (D_0015ED00 == 0xFFFFFFFF) {
        do {
            FlushCache(0);
        } while (D_0015ED00 == 0xFFFFFFFF);
    }
    return D_0015ED00;
}

extern __typeof__(snd_bank_load_by_loc) func_0012DF20 __attribute__((alias("FUN_0012df20")));
