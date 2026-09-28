#include "types.h"
#include "rnc/assembly_textbin_fun_0021d1f8_types.h"

extern struct M2c_D_00137B80 D_00137B80;
extern struct M2c_D_001516D0 D_001516D0;
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 D_001D5CF8[];
extern u8 D_001D60B8[];
extern void func_00225AC0(s32);
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("FUN_00216788");

s32 FUN_0021d1f8(struct M2c_arg0 *arg0)
{
    register u32 *entry __asm__("$3");
    u32 id;
    s32 count;
    struct M2c_D_001D5BF0 *state;

    func_00225AC0(1);
    entry = (u32 *)D_001D60B8;
    arg0->unk54 = 0;
    entry += 1;
    arg0->unk38 = 0;
    state = &D_001D5BF0;
    count = 4;
    do {
        id = *(u32 *)((u8 *)entry - 4);
        count -= 1;
        if ((id != 0) && (id < (u32)state->unk10C)) {
            entry[0] |= 2;
        }
        entry += 2;
    } while (count >= 0);
    arg0->unk50 = 0;
    if (D_001516D0.unk8 == 0) {
        if (start_audio_stream_read(D_001D5CF8[0], D_00137B80.unk1528, D_00137B80.unk152C) != 0) {
            arg0->unk50 = 1;
        } else {
            arg0->unk50 = 3;
        }
    }
    arg0->unk10 |= 4;
    return 0;
}

extern __typeof__(FUN_0021d1f8) func_0021D1F8 __attribute__((alias("FUN_0021d1f8")));
