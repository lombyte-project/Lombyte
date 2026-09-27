#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceTtyHandler; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/sce_tty_handler/sceTtyHandler.s", sceTtyHandler);
#else
#include "types.h"

/*
 * This header contains macros emitted by m2c in "valid syntax" mode,
 * which can be enabled by passing `--valid-syntax` on the command line.
 *
 * In this mode, unhandled types and expressions are emitted as macros so
 * that the output is compilable without human intervention.
 */

#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/* Unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Unknown field access, like `*(type_ptr) &expr->unk_offset` */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* Bitwise (reinterpret) cast */
#define M2C_BITWISE(type, expr) ((type)(expr))

/* Unaligned reads */
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

/* Unhandled instructions */
#define M2C_ERROR(desc) (0)
#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)
#define REVERSE_BITS(x) (0)
#define ROTATE_RIGHT(x, shift) (0)
#define ARM_RRX(x, carry) (0)
#define BSWAP32(x) (0)
#define BSWAP16(x) (0)
#define BSWAP16X2(x) (0)

/* Carry/overflow bits from partially-implemented instructions */
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)

/* Memcpy patterns */
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy
#define M2C_STRUCT_COPY memcpy

/* Sh2 control register loads/stores */
#define M2C_LOAD_SR() (0)
#define M2C_LOAD_GBR() (0)
#define M2C_LOAD_VBR() (0)
#define M2C_STORE_SR(a)
#define M2C_STORE_GBR(a)
#define M2C_STORE_VBR(a)

#define M2C_CMP_STR(a, b) (0)
#define M2C_TAS_B(a) (0)

#endif /* M2C_MACROS_H */

extern u8 D_00152710[];
extern u8 D_00152738[];
extern u8 D_00152750[];
extern u8 D_00152768[];
extern void QueuePeekWriteDone();
extern s32 SceDeci2ExRecv();
extern s32 SceDeci2ExSend();
extern s32 kprintf();
void sceTtyHandler(s32 arg0, s32 arg1, s32 *arg2) {
    s32 temp_2_36;
    s32 temp_2_81;
    s32 var_16_51;
    u16 *temp_18_23;
    u8 temp_3_63;

    switch (arg0) {                                 /* irregular */
    default:
        if (arg1 == 0) {
            temp_18_23 = M2C_FIELD(arg2, u16 **, 0x14);
    __asm__ volatile ("" : "+r" (temp_18_23));
            var_16_51 = 0xC;
            if ((s32) *temp_18_23 > 0xC) {
loop_16:
                temp_3_63 = *(M2C_FIELD(arg2, u16 **, 0x14) + var_16_51);
                var_16_51 += 1;
                *M2C_FIELD(M2C_FIELD(arg2, void **, 0x18), u8 **, 0xC) = temp_3_63;
                QueuePeekWriteDone(M2C_FIELD(arg2, void **, 0x18));
                if (var_16_51 < (s32) *temp_18_23) {
                    goto loop_16;
                }
            }
            M2C_FIELD(arg2, s32 *, 8) = 0;
        } else {
            if ((u32) (M2C_FIELD(arg2, s32 *, 8) + arg1) >= 0x141U) {
                kprintf(D_00152710);
            }
            temp_2_36 = SceDeci2ExRecv(M2C_FIELD(arg2, s32 *, 0), M2C_FIELD(arg2, u16 **, 0x14) + M2C_FIELD(arg2, s32 *, 8), arg1 & 0xFFFF);
            if (temp_2_36 < 0) {
                kprintf(D_00152738);
            }
            M2C_FIELD(arg2, s32 *, 8) = (s32) (M2C_FIELD(arg2, s32 *, 8) + temp_2_36);
        }
        return;
    case 3:
        temp_2_81 = SceDeci2ExSend(M2C_FIELD(arg2, s32 *, 0), M2C_FIELD(arg2, s32 *, 0x10), M2C_FIELD(arg2, s32 *, 4) & 0xFFFF);
        if (temp_2_81 >= 0) {
            M2C_FIELD(arg2, s32 *, 0x10) = (s32) (M2C_FIELD(arg2, s32 *, 0x10) + temp_2_81);
            M2C_FIELD(arg2, s32 *, 4) = (s32) (M2C_FIELD(arg2, s32 *, 4) - temp_2_81);
        } else {
            kprintf(D_00152750, temp_2_81);
block_27:
            M2C_FIELD(arg2, s32 *, 0xC) = 0;
        }
        break;
    case 4:
        if (M2C_FIELD(arg2, s32 *, 4) != 0) {
            kprintf(D_00152768, M2C_FIELD(arg2, s32 *, 4));
        }
        goto block_27;
    }
}
#endif /* NON_MATCHING */
