#include "types.h"
struct DbCBlob {
    u8 data[0x40];
};
struct DbCState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad_14[0x10];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
};
extern u8 D_001535B8[];
extern u8 D_0015B008[];
extern struct DbCState D_0015B080;
extern u8 D_0015B480[];
extern u8 D_0015B500[];
extern s32 DIntr();
extern s32 EnableInterrupts();
extern s32 synchronize_cache_range() __asm__("func_00118F88");
extern s32 debug_print_stub() __asm__("func_00124A20");
extern s32 sceSifCallRpc();
s32 sceDbcGetDepNumber(s32 arg0) {
    s32 result;

    synchronize_cache_range(D_0015B480, D_0015B480 + 0x80);
    DIntr();
    *(struct DbCBlob *)D_0015B500 = *(struct DbCBlob *)D_0015B480;
    EnableInterrupts();
    if (((s32 *)D_0015B500)[arg0] == 1) {
        D_0015B080.unk0 = arg0;
        if (sceSifCallRpc(D_0015B008, 0x80000903, 0, &D_0015B080, 0x400, &D_0015B080, 0x400, 0, 0) <
            0) {
            debug_print_stub(D_001535B8);
            return 0;
        }
        return D_0015B080.unk4;
    }
    return -0xC;
}
