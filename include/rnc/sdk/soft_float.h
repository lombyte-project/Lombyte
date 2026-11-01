#ifndef LOMBYTE_RNC_SDK_SOFT_FLOAT_H
#define LOMBYTE_RNC_SDK_SOFT_FLOAT_H

#include "rnc/sdk/library/fp_number.h"
#include "types.h"

u64       __fixunsdfdi(f64);
u64       MakeDoublePacket(s32, s32, s32, u64) __asm__("__make_dp");
u64       __muldi3(u64, u64);
f64       __pack_d(FpNumber *);
s32       CompareDoubles(double, double) __asm__("dpcmp");
f64       dpmul(f64, f64);
FpNumber *_fpadd_parts(FpNumber *, FpNumber *, FpNumber *);
u64       __udivdi3(u64, u64);
u64       __umoddi3(u64, u64);

#endif /* LOMBYTE_RNC_SDK_SOFT_FLOAT_H */
