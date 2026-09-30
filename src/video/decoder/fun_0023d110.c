#include "sda.h"
extern char *D_0016120C MACRO_ADDR;
extern void vi_buf_restart_dma(void *) __asm__("FUN_0023c280");

int FUN_0023d110(void) {
    vi_buf_restart_dma(D_0016120C + 0xD9090);
    return 1;
}

extern __typeof__(FUN_0023d110) func_0023D110 __attribute__((alias("FUN_0023d110")));
