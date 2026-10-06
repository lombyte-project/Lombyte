#include "types.h"
#include "asm.h"

#include "types.h"

struct M2c_D_00154A50 {
    volatile s32 unk0; /* deci2 handle from sceDeci2Open */
    volatile s32 unk4; /* published payload length */
    volatile s32 unk8;
    volatile s32 unkC; /* busy flag, also written by sceTtyHandler */
    s32 unk10;         /* MMIO block pointer (with 0x20000000 flag bit) */
    s32 unk14;
    s32 unk18;
};

struct Mmio {
    u16 unk0; /* transmit length */
    u16 unk2;
    u16 unk4;
    u8 unk6;
    s8 unk7; /* CallDebugCharacter mode byte (read with `lb`) */
    u32 unk8;
    u8 data[0xF4]; /* payload window, addressed at block + 0xC */
};

extern struct M2c_D_00154A50 D_00154A50;
extern u8 D_00154A80[];
extern s32 CallDebugCharacter(s32, s32);
extern s32 DIntr();
extern s32 EnableInterrupts();
extern void SceDeci2Poll(s32);

s32 sceTtyWrite(s8 *arg0, s32 arg1) {
    s32 var_20_8;  /* characters consumed -> return value */
    s32 var_18_12; /* countdown, hits -1 after len+1 tests */
    s32 var_17_14; /* bytes resident in the payload window */
    s8 *var_16_16; /* source cursor */
    struct Mmio *var_6_26;
    u8 *var_4_32; /* payload cursor */

    var_20_8 = 0;
    var_18_12 = arg1;
    var_17_14 = 0;
    var_16_16 = arg0;
    if (D_00154A50.unkC == 0) {
        DIntr();
        D_00154A50.unkC = 1;
        var_6_26 = (struct Mmio *)((u32)D_00154A80 | 0x20000000);
        D_00154A50.unk10 = (s32)var_6_26;
        var_4_32 = var_6_26->data;
        do {
            var_18_12 = var_18_12 - 1;
            if (var_18_12 == -1) {
                break;
            }
            if (*var_16_16 == 0xA) {
                *var_4_32 = 0xD;
                var_17_14 = var_17_14 + 1;
                var_4_32 = var_4_32 + 1;
                if (var_17_14 >= 0x100) {
                    break;
                }
            }
            *var_4_32 = (u8)*var_16_16;
            var_17_14 = var_17_14 + 1;
            var_16_16 = var_16_16 + 1;
            var_4_32 = var_4_32 + 1;
            var_20_8 = var_20_8 + 1;
        } while (var_17_14 < 0x100);
        D_00154A50.unk4 = var_17_14 + 0xC;
        var_6_26->unk0 = D_00154A50.unk4;
        if (CallDebugCharacter(D_00154A50.unk0, var_6_26->unk7) < 0) {
            *(s32 *)&D_00154A50.unkC = 0;
            EnableInterrupts();
            return -1;
        }
        if (D_00154A50.unkC != 0) {
            do {
                SceDeci2Poll(D_00154A50.unk0);
            } while (D_00154A50.unkC != 0);
        }
        EnableInterrupts();
        return var_20_8;
    }
    return -1;
}
