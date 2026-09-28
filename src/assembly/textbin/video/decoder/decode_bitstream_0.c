#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/decoder/decode_bitstream_0/FUN_0023cec8.s", FUN_0023cec8);
#else
#include "rnc/video_decoder_decode_bitstream_0_types.h"
#include "types.h"






extern struct M2c_D_0016120C *D_0016120C[];
extern u8 D_001E8B38[];
extern u8 D_001E8B50[];
extern s32 DebugPrint();
extern s32 func_0012BA48();
extern s32 func_0023A770();
extern s32 func_0023AB78();
extern s32 func_0023B210();
extern s32 func_0023CC80();
extern s32 func_0023D210();
extern s32 func_0023D288();
extern s32 sceMpegGetPicture();
extern s32 sceMpegReset();
s32 decode_bitstream_0(struct M2c_arg0 *arg0) __asm__("FUN_0023cec8");

s32 decode_bitstream_0(struct M2c_arg0 *arg0) {
    s32 sp0;
    s32 temp_22_50;
    s32 temp_23_48;
    s32 temp_2_28;
    s32 var_17_53;
    s32 var_19_58;
    s32 var_20_57;
    s32 *temp_2_43;
    struct M2c_temp_4_45 *temp_4_45;

    sp0 = 1;
    goto loop_13;
block_2:
    goto loop_4;
block_3:
    func_0023A770();
loop_4:
    temp_2_28 = func_0023D288(D_0016120C[0] + 0xD9168);
    if (temp_2_28 == 0) {
        goto block_3;
    }
    if (sceMpegGetPicture(arg0, temp_2_28, 0x340) >= 0) {
        goto block_7;
    }
    func_0023AB78(D_001E8B50);
block_7:
    temp_2_43 = D_0016120C[0];
    if (arg0->unk8 != 0) {
        goto block_12;
    }
    temp_4_45 = D_0016120C[0];
    temp_23_48 = arg0->unk0;
    temp_22_50 = arg0->unk4;
    var_17_53 = 0;
    if (temp_4_45->unkD9178 <= 0) {
        goto block_11;
    }
    var_20_57 = 0;
    var_19_58 = 0;
loop_10:
    var_17_53 += 1;
    func_0023B210(temp_4_45->unkD916C + var_19_58 + 0x40, temp_4_45->unkD9168 + var_20_57, temp_23_48, temp_22_50);
    var_20_57 += 0xD0000;
    var_19_58 += 0x138C0;
    if (var_17_53 < D_0016120C[0]->unkD9178) {
        goto loop_10;
    }
block_11:
block_12:
    func_0023D210(temp_2_43 + 0xD9168);
    func_0023A770();
loop_13:
    if (func_0012BA48(arg0) != 0) {
        goto block_16;
    }
    if (func_0023CC80(arg0) != 1) {
        goto block_2;
    }
    sp0 = -1;
    DebugPrint(D_001E8B38);
block_16:
    sceMpegReset(arg0);
    return sp0;
}
#endif /* NON_MATCHING */
