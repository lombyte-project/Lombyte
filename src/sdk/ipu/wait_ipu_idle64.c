#include "types.h"
#include "rnc/sdk/mpeg/api.h"

s64 _waitIpuIdle64(s32 *ctx) {
    enum {
        IPU_CMD_IDLE_THRESHOLD = 0
    };
    long new_var;
    s64  status;
    int  count = 0;
    status = *((volatile s64 *)0x10002000);
    if (status < IPU_CMD_IDLE_THRESHOLD) {
        if (((*((volatile u32 *)0x10002010)) & 0x4000) == 0) {
            do {
                new_var = 0x1389;
                if (count >= new_var) {
                    count = *((s32 *)(((u8 *)ctx) + 0x858));
                    DispatchMpegCallbackNoData(count);
                    count = 0;
                } else {
                    count++;
                }
                status = *((volatile s64 *)0x10002000);
            } while ((status < IPU_CMD_IDLE_THRESHOLD) &&
                     (((*((volatile u32 *)0x10002010)) & 0x4000) == 0));
        }
    }
    return status;
}
