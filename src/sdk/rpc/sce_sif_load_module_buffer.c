#include "types.h"
struct M2c_D_00158200 {
    s32 unk0;
    s32 unk4;
};
extern u8 D_00158200[];
extern u8 D_00158400[];
extern s32 _lf_bind();
extern s32 func_0011CAE0();
extern s32 memcpy();
extern s32 sceSifCallRpc();
#define SIFCMD ((struct M2c_D_00158200 *)D_00158200)
s32 _sceSifLoadModuleBuffer(s32 arg0, s32 arg1, s32 arg2, s32 *arg3) {
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
    if (arg2 != 0) {
        if (arg1 >= 0xFD) {
            memcpy(D_00158200 + 0x104, (void *)arg2, 0xFC);
            SIFCMD->unk4 = 0xFC;
        } else {
            memcpy(D_00158200 + 0x104, (void *)arg2, arg1);
            SIFCMD->unk4 = arg1;
        }
    } else {
        SIFCMD->unk4 = 0;
    }
    if (sceSifCallRpc(D_00158400, 6, 0, D_00158200, 0x200, D_00158200, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    temp = SIFCMD->unk0;
    *arg3 = SIFCMD->unk4;
    return temp;
}
