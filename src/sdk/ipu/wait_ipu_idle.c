#include "types.h"
#include "rnc/sdk/mpeg/api.h"

void _waitIpuIdle(s32 *ctx) {
    long new_var;
    s32  polls_since_callback = 0;
    if (((*((volatile u32 *)0x10002010)) & 0x80004000) == 0x80000000) {
        do {
            new_var = 0x1389;
            if (polls_since_callback >= new_var) {
                polls_since_callback = *((s32 *)(((u8 *)ctx) + 0x858));
                DispatchMpegCallbackNoData(polls_since_callback);
                polls_since_callback = 0;
            } else {
                polls_since_callback++;
            }
        } while (((*((volatile u32 *)0x10002010)) & 0x80004000) == 0x80000000);
    }
}
