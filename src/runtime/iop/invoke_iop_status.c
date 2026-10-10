/* Exact low-cost entry recovered with target symbolic relocations. */

#include "types.h"

extern s32 load_iop_module_buffer(s32 module_id, s32 argument_count, const char *arguments,
                               void *result) __asm__("_sceSifLoadModuleBuffer");

s32 InvokeIopStatus(s32 module_id, s32 argument_count, const char *arguments) {
    s32 result;

    return load_iop_module_buffer(module_id, argument_count, arguments, &result);
}
