#include "rnc/assembly_textbin_fun_0023ba60_types.h"
#include "types.h"

extern s32 sceCdRead(s32, s32, s32, u8 *);
extern s32 sceCdSync(s32);

s32 FUN_0023ba60(struct M2c_arg0 *arg0, u32 arg1, s32 arg2, s32 arg3)
{
    u8 readcmd[0x10];
    s32 blocks;
    s32 result;

    blocks = arg2 >> 11;
    result = 0;
    readcmd[0] = 0x64;
    readcmd[1] = 0;
    readcmd[2] = 0;
    sceCdRead(arg0->unk4, blocks, arg1, readcmd);
    if (arg3 == 0)
    {
        arg0->unk4 += blocks;
        sceCdSync(0);
        result = arg2;
    }
    return result;
}

extern __typeof__(FUN_0023ba60) func_0023BA60 __attribute__((alias("FUN_0023ba60")));
