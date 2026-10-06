#include "types.h"
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
extern u8 D_00153558[];
extern u8 D_0015B008[];
extern struct DbCState D_0015B080;
extern s32 debug_print_stub() __asm__("func_00124A20");
extern s32 sceSifCallRpc();
s32 sceDbcSetWorkAddr(s32 arg0) {
    D_0015B080.unk4 = arg0;
    if (sceSifCallRpc(D_0015B008, 0x80000904, 0, &D_0015B080, 0x400, &D_0015B080, 0x400, 0, 0) <
        0) {
        debug_print_stub(D_00153558);
        return 0;
    }
    return D_0015B080.unk0;
}
