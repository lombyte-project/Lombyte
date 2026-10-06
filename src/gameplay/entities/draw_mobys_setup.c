#include "types.h"

extern u8 D_00100080[];
extern u8 D_0010FAA0[];
extern u16 D_0010FA90[];
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015F620;
extern s32 D_0015F638;
extern s32 D_0015F63C;
extern u8 D_0015FED0[];
extern s32 D_0015FF0C;
extern s32 D_0015FF14;
extern s32 D_0015FF40;
extern s32 D_00160F00;
extern s32 D_00160F08;
extern void func_001F21B8();
extern void vu0_load_micro_program() __asm__("func_002334D8");
extern void vu1_add_data_ref() __asm__("func_00233830");
extern void vu1_add_g_sregister(s32, s64) __asm__("func_00233980");

void draw_mobys_setup(void) __asm__("FUN_0020d278");

void draw_mobys_setup(void) {
    register s32 current;
    register s32 callbackArg;
    vu1_add_data_ref(D_0010FAA0, D_0010FA90[0]);
    D_0015F620 = 6;
    vu0_load_micro_program(D_00100080);
    vu1_add_g_sregister(0x47, 0x5360B);
    current = D_00160F00;
    D_0015FF0C = current;
    callbackArg = 1;
    current += 0x10;
    D_0015EE74 = D_0015EE78;
    D_00160F00 = current;
    func_001F21B8(D_0015FED0, callbackArg);
    D_0015FF40 = 0;
    D_00160F08 = D_0015F63C + 0xFFFF0000;
    D_0015FF14 = D_0015F638;
}

extern __typeof__(draw_mobys_setup) func_0020D278 __attribute__((alias("FUN_0020d278")));
