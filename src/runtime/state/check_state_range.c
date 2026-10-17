#include "types.h"
extern s64 call_global_state_resource(s32 resource, s32 first, s32 second) __asm__("CallGlobalStateResource");
s64 CheckStateRange(s32 resource) {
    return (s64)(s32)(call_global_state_resource(resource, 0, 0xA) << 32 >> 32);
}
