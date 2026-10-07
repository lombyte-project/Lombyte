#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit printfloat; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/printfloat/printfloat.s", printfloat);
#else
#include "types.h"

extern char D_00152780[];
extern char D_00152788[];
extern char D_00152790[];
extern void (*D_0012FC00[])(s32);
extern s32 dpcmp_f(f64 left, f64 right) __asm__("dpcmp");
extern f64 dpsub_f(f64 left, f64 right) __asm__("dpsub");
extern f64 dpmul_f(f64 left, f64 right) __asm__("dpmul");
extern f64 dpdiv_f(f64 left, f64 right) __asm__("dpdiv");
extern u64 __fixunsdfdi(f64 value);
extern s64 ftoi(s64 bits);
extern s32 kprintf();

void printfloat(f64 x) {
    s32 exponent;
    s64 significand;

    exponent = 0;
    if (dpcmp_f(x, 0.0) < 0) {
        x = dpsub_f(0.0, x);
        D_0012FC00[0](0x2D);
    }
    /* Absolute loads avoid GP-relative references in the call delay slots. */
    if (dpcmp_f(x, 0.1) < 0) {
        while (dpcmp_f(x, 0.1) < 0) {
            exponent -= 1;
            x = dpmul_f(x, 10.0);
        }
    } else {
        if (dpcmp_f(x, 1.0) >= 0) {
            while (dpcmp_f(x, 1.0) >= 0) {
                exponent += 1;
                x = dpdiv_f(x, 10.0);
            }
        }
    }
    significand = ftoi(__fixunsdfdi(dpmul_f(x, 1000000.0)));
    kprintf(D_00152780, significand);
    if (exponent >= 0) {
        kprintf(D_00152788, exponent);
    } else {
        kprintf(D_00152790, exponent);
    }
}
#endif /* NON_MATCHING */
