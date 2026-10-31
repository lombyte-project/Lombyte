#include "types.h"
#include "rnc/sdk/mpeg/api/dispatch_mpeg_cb_nodata.h"

void _waitIpuIdle(s32 *ctx) {
    long new_var;
    s32  count = 0;
    if (((*((volatile u32 *)0x10002010)) & 0x80004000) == 0x80000000) {
        do {
            new_var = 0x1389;
            if (count >= new_var) {
                count = *((s32 *)(((u8 *)ctx) + 0x858));
                DispatchMpegCallbackNoData(count);
                count = 0;
            } else {
                count++;
            }
        } while (((*((volatile u32 *)0x10002010)) & 0x80004000) == 0x80000000);
    }
}
