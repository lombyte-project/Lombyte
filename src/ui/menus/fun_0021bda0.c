#include "types.h"
#include "rnc/ui/menus/menu_system.h"
struct MusicStreamState {
    u8 pad_0[0x8];
    s16 unk8;
    u8 pad_A[0x2];
};
struct UiGlobals {
    u8 pad_0[0x28C];
    s32 unk28C[5];
    s32 unk2A0;
};
extern struct MusicStreamState D_001516D0;
extern struct UiGlobals D_001A00F0;
extern u8 D_001D5CBB[];
extern void request_audio_stream_break() __asm__("FUN_002166e8");

s32 FUN_0021bda0(void) {
    if (D_001516D0.unk8 == 0) {
        if (D_001A00F0.unk2A0 != -1) {
            D_001D5CBB[0] = 0;
            D_001A00F0.unk28C[D_001A00F0.unk2A0] ^= 0x1000;
            D_001A00F0.unk2A0 = -1;
        }
    }
    if (D_001516D0.unk8 != 0) {
        if (menu_system.unkCB != 0) {
            request_audio_stream_break();
            menu_system.unkCB = 0;
            D_001A00F0.unk28C[D_001A00F0.unk2A0] = -1;
            D_001A00F0.unk2A0 = -1;
        }
    }
    return 0;
}

extern __typeof__(FUN_0021bda0) func_0021BDA0 __attribute__((alias("FUN_0021bda0")));
