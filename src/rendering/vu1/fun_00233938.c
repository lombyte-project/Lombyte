#include "types.h"
#include "rnc/assembly_textbin_fun_00233938_types.h"
extern struct M2c_D_00160F00 * volatile D_00160F00;
void FUN_00233938(s32 arg0)
{
    register u32 r2 __asm__("$2");
    register u8 *global_pointer __asm__("$28");
    register struct M2c_D_00160F00 *p3 __asm__("$3");
    register struct M2c_D_00160F00 *p5 __asm__("$5");

    p5 = D_00160F00;
    __asm__ volatile ("" : "+r" (p5) : : "memory");
    r2 = 0x10000000;
    __asm__ volatile ("" : "+r" (r2));
    p5->unk0 = r2;
    p3 = D_00160F00;
    __asm__ volatile ("" : "+r" (p3) : : "memory");
    p3->unk4 = 0;
    r2 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r2) : : "memory");
    *((u32 *)r2 + 2) = 0;
    p3 = D_00160F00;
    __asm__ volatile ("" : "+r" (p3) : : "memory");
    p3->unkC = arg0;
    r2 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r2) : : "memory");
    r2 += 0x10;
    __asm__ volatile ("" : "+r" (r2));
    __asm__ volatile ("" : "=r" (global_pointer));
    *((s32 *) (global_pointer - 0x5D00)) = r2;
}

extern __typeof__(FUN_00233938) func_00233938 __attribute__((alias("FUN_00233938")));
