#include "types.h"

extern void func_00227D80();
extern void func_00227ED0();

u8 *parse_typed_resource_record(s16 *record) __asm__("FUN_002270e8");

u8 *parse_typed_resource_record(s16 *typed_resource_record) {
    u8 *record_cursor;

    record_cursor = (u8 *)typed_resource_record;
    if (*typed_resource_record == 0) {
        func_00227D80(typed_resource_record);
        record_cursor += 0x20;
    } else if (*typed_resource_record == 1) {
        func_00227ED0(typed_resource_record);
        record_cursor += 0x30;
    }
    return record_cursor;
}

extern __typeof__(parse_typed_resource_record) func_002270E8 __attribute__((alias("FUN_002270e8")));
