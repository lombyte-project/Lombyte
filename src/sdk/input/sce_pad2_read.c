#include "types.h"
#include "rnc/sdk_input_sce_pad2_read_types.h"

extern u8 D_0015B540[];
extern u8 D_0015B550[];
extern void *memcpy();
extern struct M2c_temp_2_27 *scePad2GetSide();
extern s32 scePad2LinkDriver();
extern s32 scePad2SetButtonOrder();

s32 scePad2Read(s32 arg0, s32 arg1) {
    s32 temp_4_9;
    register s32 temp_2_30 __asm__("v0");
    struct M2c_temp_2_27 *temp_2_27;
    register u8 *temp_5_33 __asm__("a1");
    register s32 copy_len __asm__("a2");
    register s32 output __asm__("s2");
    register s32 multiplier __asm__("v1");

    output = arg1;
    temp_4_9 = arg0 * 0x330;
    if (*(u32 *)((u8 *)D_0015B540 + temp_4_9) == 0) {
        return -1;
    }
    if (*(s32 *)((u8 *)(D_0015B540 + temp_4_9) + 0x4) == 0) {
        if (scePad2LinkDriver(arg0) < 0) {
            return -1;
        }
    }
    temp_2_27 = scePad2GetSide(arg0);
    temp_2_30 = (s32)temp_2_27->unk2;
    if (temp_2_30 != 0) {
        temp_5_33 = ((u8 *)temp_2_27 + (0x1C));
        copy_len = temp_2_30;
        if (temp_5_33 != NULL) {
            memcpy((void *)output, temp_5_33, copy_len);
            multiplier = 0x330;
            scePad2SetButtonOrder(((u8 *)temp_2_27 + ((temp_2_27->unk2 + 0x1C))), (multiplier * arg0) + D_0015B550);
        }
    }
    if (temp_2_27->unk4 == 0) {
        return -1;
    }
    return temp_2_27->unk2;
}
