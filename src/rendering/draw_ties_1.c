#include "types.h"

extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_00160F00;
extern u8 D_00160F30[];
extern u8 D_00160F40[];
extern s32 D_00160F68;
extern s32 D_0018A2C8[];
extern u8 D_001DF030[];
extern u8 D_001E3200[];
extern s32 FlushCache();
extern s32 WriteDmaChannel();
extern s32 func_001F21B0();
extern void func_001F21B8();
extern s32 func_001F98D0();
extern s32 dma_tie_textures() __asm__("func_00235640");
extern s32 func_00235BE8();
void draw_ties_1(void) __asm__("FUN_002358c8");

void draw_ties_1(void) {
    register s32 current;
    register u8 *callbackData;
    register s32 callbackArg;
    current = D_00160F00;
    D_00160F68 = current;
    callbackData = D_00160F30;
    current += 0x10;
    D_0015EE74 = D_0015EE78;
    D_00160F00 = current;
    callbackArg = 1;
    func_001F21B8(callbackData, callbackArg);
    if (D_0018A2C8[0] != 0) {
        FlushCache(0);
        func_00235BE8();
        WriteDmaChannel(D_001E3200, 0x3600, 0x40);
    }
    func_001F21B8(D_00160F40, 5);
    dma_tie_textures();
    func_001F98D0(D_00160F00, D_001DF030, 0x20);
    func_001F21B0(D_00160F40, 5);
}

extern __typeof__(draw_ties_1) func_002358C8 __attribute__((alias("FUN_002358c8")));
