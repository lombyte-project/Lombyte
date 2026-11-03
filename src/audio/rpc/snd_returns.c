#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"
#include "rnc/sdk/libc/stdio.h"
#include "rnc/sdk/rpc/rpc.h"

/* The IOP return buffer this file owns: both functions below reach these
   gp-relative, which needs their definitions ahead of them. */
s32        *snd_current_buffer __asm__("D_0015EC80") = 0;
s32         snd_current_buffer_index __asm__("D_0015EC84") = 0;
extern void FlushCache();

const char snd_no_return_error_string[] __asm__("D_00153C98") =
    "989snd.c: Sif says RPC isn't busy, but we still don't have returns from the IOP!\n";

s32 snd_got_returns(void) __asm__("FUN_0012de70");

s32 snd_got_returns(void) {
    s32 *p;

    FlushCache(0);
    if (snd_current_buffer == 0) {
        return 1;
    }
    if (SceSifCheckStatRpc(sound_command_client) != 0) {
        return 0;
    }
    p = snd_current_buffer;
    if (p[0] != 0xFFFFFFFF) {
        Printf(snd_no_return_error_string);
        return 0;
    }
    if (p[snd_current_buffer_index + 1] != 0xFFFFFFFF) {
        Printf(snd_no_return_error_string);
        return 0;
    }
    snd_current_buffer = 0;
    return 1;
}

extern s32 func_0012DE70(void) __attribute__((alias("FUN_0012de70")));
extern s32 snd_GotReturns(void) __attribute__((alias("FUN_0012de70")));

void snd_prepare_return_buffer(s32 *obj, s32 idx) {
    snd_current_buffer_index = idx;
    snd_current_buffer = obj;
    obj[idx + 1] = 0;
    obj[0] = 0;
}
