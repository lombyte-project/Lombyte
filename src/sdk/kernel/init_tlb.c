#include "types.h"

extern s32 GetMemorySize();
extern s32 _InitTLB();
extern s32 InitTLB32MB();

s32 InitTLB(void) {
    s32 tlb_init_result;

    if (GetMemorySize() == 0x2000000) {
        tlb_init_result = InitTLB32MB();
    } else {
        tlb_init_result = _InitTLB();
    }
    return tlb_init_result;
}
