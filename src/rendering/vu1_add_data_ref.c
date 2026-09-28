#include "types.h"
#include "rnc/rendering_vu1_add_data_ref_types.h"
extern struct M2c_D_00160F00 * volatile D_00160F00;
void vu1_add_data_ref(s32 arg0, s32 arg1) __asm__("FUN_00233830");

void vu1_add_data_ref(s32 arg0, s32 arg1)
{
    register u32 r2 __asm__("$2");
    register u8 *global_pointer __asm__("$28");
    register struct M2c_D_00160F00 *p3 __asm__("$3");
    register struct M2c_D_00160F00 *p4 __asm__("$4");

    p3 = D_00160F00;
    __asm__ volatile ("" : "+r" (p3) : : "memory");
    r2 = 0x30000000;
    __asm__ volatile ("" : "+r" (r2));
    arg1 |= (s32)r2;
    p3->unk0 = arg1;
    r2 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r2) : : "memory");
    *((s32 *)(r2 + 4)) = arg0;
    p3 = D_00160F00;
    __asm__ volatile ("" : "+r" (p3) : : "memory");
    *((s32 *)((u8 *)p3 + 8)) = 0;
    p4 = D_00160F00;
    __asm__ volatile ("" : "+r" (p4) : : "memory");
    *((s32 *)((u8 *)p4 + 0xC)) = 0;
    r2 = (u32)D_00160F00;
    __asm__ volatile ("" : "+r" (r2) : : "memory");
    r2 += 0x10;
    __asm__ volatile ("" : "+r" (r2));
    __asm__ volatile ("" : "=r" (global_pointer));
    *((s32 *)(global_pointer - 0x5D00)) = r2;
}

extern __typeof__(vu1_add_data_ref) func_00233830 __attribute__((alias("FUN_00233830")));
