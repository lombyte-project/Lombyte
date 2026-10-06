/* Exact low-cost entry recovered with target symbolic relocations. */

#include "types.h"

extern s32 LoadIopModuleBuffer(s32 module_id, s32 argument_count, const char *arguments,
                               void *result) __asm__("_sceSifLoadModuleBuffer");

s32 InvokeIopStatus(s32 module_id, s32 argument_count, const char *arguments) {
    s32 result;

    return LoadIopModuleBuffer(module_id, argument_count, arguments, &result);
}
