#include "types.h"
struct DispEnv;
extern struct DispEnv *D_0015EEB8;
extern void sceGsPutDispEnv(struct DispEnv *);
void put_disp_buffer(void) __asm__("FUN_001fb2a8");

void put_disp_buffer(void) {
    sceGsPutDispEnv(D_0015EEB8);
}

extern __typeof__(put_disp_buffer) func_001FB2A8 __attribute__((alias("FUN_001fb2a8")));
