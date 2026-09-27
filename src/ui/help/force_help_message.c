#include "types.h"

extern s32 D_0015F5D0;
extern s32 D_0015F5D4;
extern s32 D_00161288;
extern s32 func_001FDD10();
extern s32 copy_text_to_shared_buffer() __asm__("FUN_001ff658");
extern s32 try_set_help_message() __asm__("FUN_00215130");

s32 force_help_message(s32 arg0, s32 arg1) __asm__("FUN_002151d8");

s32 force_help_message(s32 arg0, s32 arg1) {
    s32 var_2_8;

    var_2_8 = try_set_help_message();
    if (var_2_8 == 0) {
        if (arg1 != 0) {
            copy_text_to_shared_buffer(func_001FDD10(arg1));
        }
        D_0015F5D4 = arg0;
        D_0015F5D0 = 2;
        D_00161288 = arg1;
        var_2_8 = 3;
    }
    return var_2_8;
}
