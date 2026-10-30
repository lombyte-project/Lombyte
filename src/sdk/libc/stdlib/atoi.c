#include "types.h"

extern s64 strtol(s32 resource, s32 first, s32 second) __asm__("CallGlobalStateResource");
s64        atoi(s32 resource) __asm__("CheckStateRange");

s64 atoi(s32 resource) {
    return (s64)(s32)(strtol(resource, 0, 0xA) << 32 >> 32);
}
