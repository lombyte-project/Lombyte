#include "types.h"
#include "rnc/data_labels.h"

extern char significand_format[] __asm__("D_00152780");
extern char nonnegative_exponent_format[] __asm__("D_00152788");
extern char negative_exponent_format[] __asm__("D_00152790");
extern s32  CompareDoubles(f64 left, f64 right) __asm__("dpcmp");
extern f64  dpsub_f(f64 left, f64 right) __asm__("dpsub");
extern f64  dpmul_f(f64 left, f64 right) __asm__("dpmul");
extern f64  dpdiv_f(f64 left, f64 right) __asm__("dpdiv");
extern u64  __fixunsdfdi(f64 value);
extern s64  ftoi(s64 bits);
extern s32  kprintf();

void printfloat(f64 x) {
    s32 exponent;
    s64 significand;

    exponent = 0;
    if (CompareDoubles(x, 0.0) < 0) {
        x = dpsub_f(0.0, x);
        D_0012FC00[0](0x2D);
    }
    /* Absolute loads avoid GP-relative references in the call delay slots. */
    if (CompareDoubles(x, 0.1) < 0) {
        while (CompareDoubles(x, 0.1) < 0) {
            --exponent;
            x = dpmul_f(x, 10.0);
        }
    } else {
        if (CompareDoubles(x, 1.0) >= 0) {
            while (CompareDoubles(x, 1.0) >= 0) {
                ++exponent;
                x = dpdiv_f(x, 10.0);
            }
        }
    }
    significand = ftoi(__fixunsdfdi(dpmul_f(x, 1000000.0)));
    kprintf(significand_format, significand);
    if (exponent >= 0) {
        kprintf(nonnegative_exponent_format, exponent);
    } else {
        kprintf(negative_exponent_format, exponent);
    }
}
