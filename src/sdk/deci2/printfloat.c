#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/soft_float.h"

extern char significand_format[] __asm__("D_00152780");
extern char nonnegative_exponent_format[] __asm__("D_00152788");
extern char negative_exponent_format[] __asm__("D_00152790");
extern f64  dpsub_f(f64 left, f64 right) __asm__("dpsub");
extern f64  dpdiv_f(f64 left, f64 right) __asm__("dpdiv");
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
            x = dpmul(x, 10.0);
        }
    } else {
        if (CompareDoubles(x, 1.0) >= 0) {
            while (CompareDoubles(x, 1.0) >= 0) {
                ++exponent;
                x = dpdiv_f(x, 10.0);
            }
        }
    }
    significand = ftoi(__fixunsdfdi(dpmul(x, 1000000.0)));
    kprintf(significand_format, significand);
    if (exponent >= 0) {
        kprintf(nonnegative_exponent_format, exponent);
    } else {
        kprintf(negative_exponent_format, exponent);
    }
}
