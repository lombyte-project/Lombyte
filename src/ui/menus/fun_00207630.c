#include "sda.h"
extern unsigned char D_0013D3B8 NOT_SDA;

int FUN_00207630(void) {
    return D_0013D3B8 != 0;
}

extern __typeof__(FUN_00207630) func_00207630 __attribute__((alias("FUN_00207630")));
