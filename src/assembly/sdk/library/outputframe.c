#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/outputframe/_outputFrame.s", _outputFrame);
#else
#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0xF8];
    s32 unkF8;
    u8 pad_FC[0x54];
    s32 unk150;
    u8 pad_154[0x20];
    s32 unk174;
    u8 pad_178[0x40];
    s32 unk1B8;
    u8 pad_1BC[0x8];
    s32 unk1C4;
    s32 unk1C8;
    u8 pad_1CC[0x8];
    s32 unk1D4;
    s32 unk1D8;
    u8 pad_1DC[0x8];
    s32 unk1E4;
};

extern s32 _dispRefImage(struct M2c_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 _dispRefImageField(struct M2c_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3);
void _outputFrame(struct M2c_arg0 *arg0, s32 arg1, s32 arg2) {
    register s32 a1save __asm__("a3") = arg1;
    s32 var_5_14;
    s32 var_5_24;
    s32 var_6_27;

    if (arg2 != 0) {
        if (arg0->unk174 == 3) {
            if (arg0->unk150 == 3) {
                var_5_14 = arg0->unk1C4;
            } else {
                var_5_14 = arg0->unk1B8;
            }
            {
                register s32 d2 __asm__("a2") = a1save - 1;
                _dispRefImage(arg0, var_5_14, d2, a1save);
            }
        } else {
            if (arg0->unk150 == 3) {
                var_5_24 = arg0->unk1D4;
                var_6_27 = arg0->unk1E4;
            } else {
                var_5_24 = arg0->unk1C8;
                var_6_27 = arg0->unk1D8;
            }
            {
                register s32 d3 __asm__("a3") = a1save - 1;
                _dispRefImageField(arg0, var_5_24, var_6_27, d3);
            }
        }
    }
    if (arg0->unkF8 == 1) {
        arg0->unkF8 = 2;
    }
}

#endif /* NON_MATCHING */
