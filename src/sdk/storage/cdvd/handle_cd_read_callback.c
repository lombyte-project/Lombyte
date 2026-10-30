#include "types.h"
#include "rnc/sdk/libcdvd.h"

extern volatile s32 D_001312D4[];
extern volatile s32 D_00131314[];
extern volatile s32 native_cd_last_command_state[] __asm__("D_00131318");
extern volatile s32 D_00159740[];
extern s32          iSignalSema();

void handle_cd_read_callback(s32 *status) __asm__("FUN_001206d8");

void handle_cd_read_callback(s32 *status) {
    D_00131314[0] = *status;
    native_cd_last_command_state[0] = D_00131314[0];
    if (D_00131314[0] == 11) {
        D_00131314[0] = 0;
        cd_read_pending = 0;
        return;
    }
    iSignalSema(cd_ncmd_sema);
    if (D_001312D4[0] != 0 && D_00159740[0] != 0) {
        iSignalSema(cd_callback_sema);
    } else {
        cd_read_pending = 0;
    }
    D_00131314[0] = 0;
}

extern void func_001206D8(s32 *status) __attribute__((alias("FUN_001206d8")));
