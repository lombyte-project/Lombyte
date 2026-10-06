#include "types.h"
extern void func_00227D80();
extern void func_00227ED0();

u8 *parse_typed_resource_record(s16 *record) __asm__("FUN_002270e8");

u8 *parse_typed_resource_record(s16 *record) {
    u8 *p;

    p = (u8 *)record;
    if (*record == 0) {
        func_00227D80(record);
        p += 0x20;
    } else if (*record == 1) {
        func_00227ED0(record);
        p += 0x30;
    }
    return p;
}

extern __typeof__(parse_typed_resource_record) func_002270E8 __attribute__((alias("FUN_002270e8")));
