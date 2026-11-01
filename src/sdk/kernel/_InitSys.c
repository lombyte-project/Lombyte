#include "rnc/sdk/kernel.h"

void _InitSys(void) __asm__("_InitSys");

void _InitSys(void) {
    SupplementCrt0();
    InitSystemCallTableAddress();
    InitAlarm();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
}
