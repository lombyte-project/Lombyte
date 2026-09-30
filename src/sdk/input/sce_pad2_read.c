#include "types.h"
#include "rnc/sdk_input_sce_pad2_read_types.h"

extern u8 D_0015B540[];
extern u8 D_0015B550[];
extern void *memcpy();
extern struct M2c_temp_2_27 *scePad2GetSide();
extern s32 scePad2LinkDriver();
extern s32 scePad2SetButtonOrder();

s32 scePad2Read(s32 port, u8 *data) {
    struct M2c_temp_2_27 *side;
    u8 *src;

    if (*(u32 *)(D_0015B540 + port * 0x330) == 0) {
        return -1;
    }
    if (*(s32 *)(D_0015B540 + port * 0x330 + 4) == 0) {
        if (scePad2LinkDriver(port) < 0) {
            return -1;
        }
    }
    side = scePad2GetSide(port);
    if (side->unk2 != 0) {
        src = (u8 *)side + 0x1C;
        if (src != NULL) {
            memcpy(data, src, side->unk2);
            scePad2SetButtonOrder((u8 *)side + (side->unk2 + 0x1C), D_0015B550 + port * 0x330);
        }
    }
    if (side->unk4 == 0) {
        return -1;
    }
    return side->unk2;
}
