#include "types.h"
#include "asm.h"

#include "types.h"
#include "sda.h"
#include "rnc/data_labels.h"
extern void (*D_00159744)(void *) NOT_SDA;
extern void *D_00159748 NOT_SDA;

void dispatch_native_cd_power_event(void) __asm__("FUN_00120960");

void dispatch_native_cd_power_event(void) {
    if (D_00159744 != 0 && D_001312E4[0] == 0) {
        D_00159744(D_00159748);
    }
}

extern __typeof__(dispatch_native_cd_power_event) D_00120960 __attribute__((alias("FUN_00120960")));
