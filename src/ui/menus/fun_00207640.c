#include "sda.h"
extern unsigned char D_0013D3B9 NOT_SDA;

int FUN_00207640(void) {
    return D_0013D3B9 != 0;
}

extern __typeof__(FUN_00207640) func_00207640 __attribute__((alias("FUN_00207640")));
