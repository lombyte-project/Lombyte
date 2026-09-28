#include "types.h"
#include "rnc/assembly_textbin_fun_00233c28_types.h"
extern struct M2c_D_00160F00 * volatile D_00160F00;
extern u8 D_001DE3F0[];
void FUN_00233c28(void)
{
    register u32 r2 __asm__("$2");
    register u32 r3 __asm__("$3");
    register u32 r4 __asm__("$4");
    register u32 r5 __asm__("$5");
    register u8 *global_pointer __asm__("$28");

    r4 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r4) : : "memory");
    r2 = 0x30000003;
    __asm__ volatile ("" : "+r" (r2));
    *((s32 *)r4) = r2;
    r3 = (u32)D_001DE3F0;
    __asm__ volatile ("" : "+r" (r3));
    r4 = 0x50000000;
    __asm__ volatile ("" : "+r" (r4));
    r5 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r5) : : "memory");
    r4 |= 3;
    __asm__ volatile ("" : "+r" (r4));
    *((s32 *)(r5 + 4)) = r3;
    r2 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r2) : : "memory");
    *((s32 *)(r2 + 8)) = 0;
    r3 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r3) : : "memory");
    *((s32 *)(r3 + 0xC)) = r4;
    r2 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r2) : : "memory");
    r2 += 0x10;
    __asm__ volatile ("" : "+r" (r2));
    __asm__ volatile ("" : "=r" (global_pointer));
    *((s32 *)(global_pointer - 0x5D00)) = r2;
}

extern __typeof__(FUN_00233c28) func_00233C28 __attribute__((alias("FUN_00233c28")));
