typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
extern u32 D_0012FC9C[];
extern s32 WaitSema();
extern void _sceFsSemInit();
s32 _sceFsWaitS(void) {
    _sceFsSemInit();
    WaitSema(D_0012FC9C[0]);
    return 0;
}
