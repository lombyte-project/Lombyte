#include "types.h"
extern s32 _dispatchMpegCbNodata();

s64 _waitIpuIdle64(s32 *ctx) {
    long new_var;
    s64 status;
    int count = 0;
    status = *((volatile s64 *)0x10002000);
    if (status < 0) {
        if (((*((volatile u32 *)0x10002010)) & 0x4000) == 0) {
            do {
                new_var = 0x1389;
                if (count >= new_var) {
                    count = *((s32 *)(((u8 *)ctx) + 0x858));
                    _dispatchMpegCbNodata(count);
                    count = 0;
                } else {
                    count++;
                }
                status = *((volatile s64 *)0x10002000);
            } while ((status < 0) && (((*((volatile u32 *)0x10002010)) & 0x4000) == 0));
        }
    }
    return status;
}
