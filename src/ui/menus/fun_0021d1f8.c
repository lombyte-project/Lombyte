#include "types.h"
#include "rnc/ui_menus_fun_0021d1f8_types.h"

typedef struct {
    u32 key;
    s32 flags;
} PadBind;

extern struct M2c_D_00137B80 D_00137B80;
extern struct M2c_D_001516D0 D_001516D0;
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 D_001D5CF8[];
extern PadBind D_001D60B8[];
extern void func_00225AC0(s32);
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("FUN_00216788");

s32 FUN_0021d1f8(struct M2c_arg0 *arg0)
{
    s32 i;
    struct M2c_D_001D5BF0 *g;

    func_00225AC0(1);
    arg0->unk54 = 0;
    arg0->unk38 = 0;
    g = &D_001D5BF0;
    for (i = 0; i < 5; i++) {
        if (D_001D60B8[i].key != 0 && D_001D60B8[i].key < (u32)g->unk10C) {
            D_001D60B8[i].flags |= 2;
        }
    }
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
