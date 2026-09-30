#include "sda.h"
extern unsigned char D_0013D3D9 NOT_SDA;

int FUN_00207710(void) {
    return D_0013D3D9 != 0;
}

extern __typeof__(FUN_00207710) func_00207710 __attribute__((alias("FUN_00207710")));
