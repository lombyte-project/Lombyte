#include "types.h"

extern s32 GetMemorySize();
extern s32 _InitTLB();
extern s32 InitTLB32MB();

s32 InitTLB(void) {
    s32 result;

    if (GetMemorySize() == 0x2000000) {
        result = InitTLB32MB();
    } else {
        result = _InitTLB();
    }
    return result;
}
