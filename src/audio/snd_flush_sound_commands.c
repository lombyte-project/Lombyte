#include "types.h"
#include "sda.h"

typedef void (*SndCallback)(s32, long);

struct SndCommand {
    SndCallback fn;
    s32 pad4;
    long arg;
};

extern s32 D_0015EC80 __attribute__((sda));          /* pending RPC record */
extern void (*D_0015EC90)(s32) __attribute__((sda)); /* stop callback */
extern s32 D_0015EC94 __attribute__((sda));          /* stop requested */
extern s32 D_0015EC98 __attribute__((sda));
extern s32 D_0015EC9C __attribute__((sda));     /* abort */
extern s32 *D_0015ECA0[2] __attribute__((sda)); /* command counts */
extern struct SndCommand *D_0015ECB0[2] __attribute__((sda));
extern s32 *D_0015ECB8[2] __attribute__((sda)); /* replies */
extern s32 D_0015ECC0 __attribute__((sda));     /* current buffer */
extern s32 D_0015ECC4 __attribute__((sda));
extern s32 D_0015ECC8 __attribute__((sda));         /* CD read pending */
extern SndCallback D_0015ECD0 __attribute__((sda)); /* CD read callback */
extern volatile long D_0015ECD8 MACRO_ADDR;         /* its argument */
extern SndCallback D_0015ECE0 __attribute__((sda)); /* abort callback */
extern long D_0015ECE8 __attribute__((sda));        /* its argument */
extern u32 D_0015ED00 MACRO_ADDR;                   /* CD read reply */
extern s32 D_00133104[];
extern s32 snd_got_returns(void) __asm__("func_0012DE70");
extern void snd_send_current_batch(void) __asm__("func_0012E9D8");
extern s32 snd_stream_safe_cd_sync(s32) __asm__("func_0012EE08");
extern void FlushCache(s32);

s32 snd_flush_sound_commands(void) __asm__("FUN_0012dc80");

/* Once the pending RPC completes, run the callbacks queued with the other
   command buffer (or the abort callback); finish a CD read (FlushCache,
   then its callback with the reply word); send the next batch when idle;
   and poll a stop request. Returns whether an RPC or a CD read is still
   in flight. */
s32 snd_flush_sound_commands(void) {
    s32 i;
    s32 idx;
    SndCallback fn;
    long arg;

    if (D_0015EC80 != 0 && snd_got_returns() != 0) {
        if (D_0015EC9C != 0) {
            if (D_0015ECE0 != 0) {
                D_0015ECE0(D_00133104[0], D_0015ECE8);
            }
            D_0015ECE0 = 0;
            D_0015EC9C = 0;
        } else {
            idx = D_0015ECC0 != 1;
            for (i = 0; i < *D_0015ECA0[idx]; i++) {
                if (D_0015ECB0[idx][i].fn != 0) {
                    D_0015ECB0[idx][i].fn(D_0015ECB8[idx][i + 1], D_0015ECB0[idx][i].arg);
                }
            }
        }
    }
    if (D_0015ECC8 != 0) {
        FlushCache(0);
        if (D_0015ED00 != 0xFFFFFFFF) {
            if (D_0015ECD0 != 0) {
                arg = D_0015ECD8;
                fn = D_0015ECD0;
                D_0015ECD0 = 0;
                D_0015ECD8 = 0;
                fn(D_0015ED00, arg);
            }
            D_0015ED00 = 0;
            D_0015ECC8 = 0;
        }
    }
    if (D_0015EC80 == 0) {
        if (*D_0015ECA0[D_0015ECC0] != 0 && D_0015ECC4 == 0) {
            snd_send_current_batch();
        }
    }
    if (D_0015EC94 != 0) {
        snd_stream_safe_cd_sync(1);
        if (D_0015EC98 != 0) {
            D_0015EC94 = 0;
            D_0015EC98 = 0;
            if (D_0015EC90 != 0) {
                D_0015EC90(1);
            }
        }
    }
    return D_0015EC80 != 0 || D_0015ECC8 != 0;
}

extern __typeof__(snd_flush_sound_commands) func_0012DC80 __attribute__((alias("FUN_0012dc80")));
