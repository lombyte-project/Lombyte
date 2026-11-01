#include "rnc/sdk/kernel/InitExecPS2.h"
#include "rnc/sdk/kernel/InitSystemCallTableAddress.h"
#include "rnc/sdk/kernel/InitThread.h"
#include "rnc/sdk/kernel/init_alarm.h"
#include "rnc/sdk/kernel/init_tlb_functions.h"
#include "rnc/sdk/kernel/supplement_crt0.h"

void _InitSys(void) __asm__("_InitSys");

void _InitSys(void) {
    SupplementCrt0();
    InitSystemCallTableAddress();
    InitAlarm();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
}
