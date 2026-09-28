#include "types.h"
#include "rnc/gameplay_state_process_global_state_flags_types.h"

extern u32 D_0013CAE4[];
extern struct M2c_D_0013D408 D_0013D408;
extern u8 D_0013E05A[];
extern struct M2c_D_0013F350 D_0013F350;
extern u8 D_0014BF08[];
extern s32 D_0015EEA0;
extern s32 D_0015EEB0;
extern u8 D_001D4EC0[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 D_001D5BF8[];
extern void InitializeGlobalStateEntry(s32);
extern void func_001F4A58(s32);
extern s32 func_001F96F8(s32);
extern void func_00209298(s32);
extern void func_0020B178(s32, s32);
extern void func_00226B08(s32);
extern void func_00226F50(void);

s32 process_global_state_flags(void) __asm__("FUN_00222290");

s32 process_global_state_flags(void)
{
    u8 backup[4];
    u8 old_value;
    s32 i;
    s32 result;

    if ((D_0013CAE4[0] & 0x20) != 0) {
        if (D_0015EEB0 == 1 || D_0015EEB0 == 0x10) {
            D_001D5BF8[0] = (s32)D_001D4EC0;
        } else {
            func_00226B08(-1);
            InitializeGlobalStateEntry(0);
            *(u16 *)D_0013E05A = 1;
        }
    } else if ((D_0013CAE4[0] & 0x40) != 0) {
        old_value = D_0013D408.unk1D;
        for (i = 0; i < 4; i++) {
            backup[i] = D_0014BF08[i];
        }
        func_00209298(D_001D5BF0.unkE0);
        D_0013D408.unk1D = old_value;
        for (i = 0; i < 4; i++) {
            D_0014BF08[i] = backup[i];
        }
        D_0015EEA0 = 1;
        func_0020B178(0, -1);
        func_00226F50();
        result = func_001F96F8(0x10);
        func_001F4A58(result);
        D_0013F350.unk20B1 = 1;
        return -1;
    }
    return 0;
}

extern __typeof__(process_global_state_flags) func_00222290 __attribute__((alias("FUN_00222290")));
