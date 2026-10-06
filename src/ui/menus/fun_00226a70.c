#include "types.h"
#include "sda.h"
struct SaveSlotTable {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0xA8];
    s32 unkC0;
    u8 pad_C4[0x18];
    s32 unkDC;
    s32 unkE0;
    u8 pad_E4[0x8];
    s32 unkEC;
    u8 pad_F0[0x4];
    s32 unkF4;
};

extern struct SaveSlotTable D_0013D290;
extern u8 D_0015EE98[] MACRO_ADDR;
extern s32 load_and_initialize_level_chunk() __asm__("func_00209370");
extern s32 memcard_make_whole_save() __asm__("func_0020ABB0");
extern s32 sceCdReadClock();
extern s32 sceScfGetLocalTimefromRTC();
void FUN_00226a70(s32 save_data, s32 slot) {
    load_and_initialize_level_chunk();
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    memcard_make_whole_save(save_data);
    D_0013D290.unkC0 = 0;
    D_0013D290.unk14 = slot;
    *(s32 *)((u8 *)&D_0013D290 + slot * 0x1C + 0x20) = 0;
    D_0013D290.unkF4 = 1;
    D_0013D290.unkEC = save_data;
    if (D_0013D290.unkDC < 0) {
        D_0013D290.unkE0 = 0;
        D_0013D290.unkDC = 0x13;
    }
}
