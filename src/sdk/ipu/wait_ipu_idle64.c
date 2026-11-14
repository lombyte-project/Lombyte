#include "types.h"
#include "rnc/sdk/mpeg/api.h"

s64 _waitIpuIdle64(s32 *ctx) {
    enum {
        IPU_CMD_IDLE_THRESHOLD = 0
    };
    long new_var;
    s64  ipu_command_result;
    int  input_callback_poll_count = 0;
    ipu_command_result = *((volatile s64 *)0x10002000);
    if (ipu_command_result < IPU_CMD_IDLE_THRESHOLD) {
        if (((*((volatile u32 *)0x10002010)) & 0x4000) == 0) {
            do {
                new_var = 0x1389;
                if (input_callback_poll_count >= new_var) {
                    input_callback_poll_count = *((s32 *)(((u8 *)ctx) + 0x858));
                    DispatchMpegCallbackNoData(input_callback_poll_count);
                    input_callback_poll_count = 0;
                } else {
                    input_callback_poll_count++;
                }
                ipu_command_result = *((volatile s64 *)0x10002000);
            } while ((ipu_command_result < IPU_CMD_IDLE_THRESHOLD) &&
                     (((*((volatile u32 *)0x10002010)) & 0x4000) == 0));
        }
    }
    return ipu_command_result;
}
