#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/soft_float.h"

extern f64  dpsub_f(f64 left, f64 right) __asm__("dpsub");
extern f64  dpdiv_f(f64 left, f64 right) __asm__("dpdiv");
extern s64  ftoi(s64 bits);
extern s32  kprintf();

/* Explicit .rodata: at 8 bytes or less they would go to .sdata. */
const char significand_format[] __asm__("D_00152780") __attribute__((section(".rodata"))) = "0.%d";
const char nonnegative_exponent_format[] __asm__("D_00152788") __attribute__((section(".rodata"))) =
    "e+%d";
const char negative_exponent_format[] __asm__("D_00152790") __attribute__((section(".rodata"))) = "e%d";

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
