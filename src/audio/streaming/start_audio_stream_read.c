#include "types.h"

struct MusicStreamState {
    u8 pad_0[0x8];
    s16 unk8;
    u8 pad_A[0x2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
};
extern struct MusicStreamState D_001516D0;
extern u8 D_001E8650[];
extern s32 DebugPrint();
extern s32 RaiseKernelTrap();
extern s32 snd_stream_safe_cd_read() __asm__("func_0012ED58");
s32 start_audio_stream_read(s32 dst, s32 sector, s32 sector_count) __asm__("FUN_00216788");

s32 start_audio_stream_read(s32 dst, s32 sector, s32 sector_count) {
    if ((D_001516D0.unk8 == 0) && (sector_count != 0)) {
        if (snd_stream_safe_cd_read(sector, sector_count, dst, ((u8 *)&D_001516D0 + 0x30)) != 0) {
            D_001516D0.unk14 = dst;
            D_001516D0.unk8 = 1;
            D_001516D0.unkC = sector;
            D_001516D0.unk10 = sector_count;
            return sector_count << 0xB;
        }
        DebugPrint(D_001E8650);
        RaiseKernelTrap();
    }
    return 0;
}
