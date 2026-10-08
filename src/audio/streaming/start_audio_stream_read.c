#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

extern u8 D_001E8650[];
extern s32 DebugPrint();
extern s32 RaiseKernelTrap();
extern s32 snd_stream_safe_cd_read() __asm__("func_0012ED58");
s32 start_audio_stream_read(s32 dst, s32 sector, s32 sector_count) __asm__("FUN_00216788");

s32 start_audio_stream_read(s32 dst, s32 sector, s32 sector_count) {
    if ((music_stream_state.read_state == 0) && (sector_count != 0)) {
        if (snd_stream_safe_cd_read(sector, sector_count, dst, &music_stream_state.cd_mode) != 0) {
            music_stream_state.read_dst = dst;
            music_stream_state.read_state = 1;
            music_stream_state.read_sector = sector;
            music_stream_state.read_sector_count = sector_count;
            return sector_count << 0xB;
        }
        DebugPrint(D_001E8650);
        RaiseKernelTrap();
    }
    return 0;
}
