#include "sda.h"
extern unsigned char D_0013D3D5 NOT_SDA;

int FUN_002076e0(void) {
    return D_0013D3D5 != 0;
}

extern __typeof__(FUN_002076e0) func_002076E0 __attribute__((alias("FUN_002076e0")));
