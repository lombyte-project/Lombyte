#include "sda.h"
extern int D_0015EDF0 MACRO_ADDR;
extern int D_0013E5A0 NOT_SDA;

int FUN_0021cb00(void) {
    D_0013E5A0 = D_0015EDF0 * 8 / 10;
    return 0;
}

extern __typeof__(FUN_0021cb00) func_0021CB00 __attribute__((alias("FUN_0021cb00")));
