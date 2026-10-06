#include "types.h"
struct sceGsDispEnv;
extern struct sceGsDispEnv *D_0015EEB8;
extern void sceGsPutDispEnv(struct sceGsDispEnv *);
void put_disp_buffer(void) __asm__("FUN_001fb2a8");

void put_disp_buffer(void) {
    sceGsPutDispEnv(D_0015EEB8);
}

extern __typeof__(put_disp_buffer) func_001FB2A8 __attribute__((alias("FUN_001fb2a8")));
