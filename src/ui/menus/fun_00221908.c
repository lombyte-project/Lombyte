#include "sda.h"
extern int D_0013CB04 NOT_SDA;
extern int D_001D22F8[];
extern int *D_001D5BF8 NOT_SDA;

int FUN_00221908(void) {
    if (D_0013CB04 & 0x40) {
        D_001D5BF8 = D_001D22F8;
    }
    return 0;
}

extern __typeof__(FUN_00221908) func_00221908 __attribute__((alias("FUN_00221908")));
