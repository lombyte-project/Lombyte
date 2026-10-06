#include "types.h"
struct SifLoadModuleArgs {
    s32 unk0;
    s32 unk4;
};
extern u8 D_00158200[];
extern u8 D_00158400[];
extern s32 _lf_bind();
extern s32 func_0011CAE0();
extern s32 memcpy();
extern s32 sceSifCallRpc();
#define SIFCMD ((struct SifLoadModuleArgs *)D_00158200)
s32 _sceSifLoadModuleBuffer(s32 arg0, s32 size, s32 src, s32 *result_out) {
    s32 var_2_18;
    s32 temp;

    var_2_18 = 0xFFFF0000;
    if (_lf_bind() < 0) {
        return var_2_18;
    }
    if (func_0011CAE0() != 0) {
        return 0xFFFEFFFC;
    }
    SIFCMD->unk0 = arg0;
    if (src != 0) {
        if (size >= 0xFD) {
            memcpy(D_00158200 + 0x104, (void *)src, 0xFC);
            SIFCMD->unk4 = 0xFC;
        } else {
            memcpy(D_00158200 + 0x104, (void *)src, size);
            SIFCMD->unk4 = size;
        }
    } else {
        SIFCMD->unk4 = 0;
    }
    if (sceSifCallRpc(D_00158400, 6, 0, D_00158200, 0x200, D_00158200, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    temp = SIFCMD->unk0;
    *result_out = SIFCMD->unk4;
    return temp;
}
