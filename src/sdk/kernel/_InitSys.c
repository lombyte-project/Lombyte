void supplement_crt0(void) __asm__("supplement_crt0");
void InitSystemCallTableAddress(void) __asm__("InitSystemCallTableAddress");
void InitAlarm(void) __asm__("InitAlarm");
void InitThread(void) __asm__("InitThread");
void InitExecPS2(void) __asm__("InitExecPS2");
void InitTLBFunctions(void) __asm__("InitTLBFunctions");

void _InitSys(void) __asm__("_InitSys");

void _InitSys(void) {
    supplement_crt0();
    InitSystemCallTableAddress();
    InitAlarm();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
}
