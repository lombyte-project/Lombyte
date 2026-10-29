#include "types.h"
#include "eeregs.h"
#include "rnc/sdk/libmpeg.h"

extern s32 DIntr(void);
extern void EnableInterrupts(void);
extern void _dispatchMpegCallback(struct sceMpeg *, struct sceMpegCbData *);
extern void _sendIpuCommand(struct MpegDecoder *, s32);
extern void _waitIpuIdle(struct MpegDecoder *);

void _setDefaultQM(struct MpegDecoder *decoder, s32 command, u32 matrix) {
    s32 interrupts_enabled;
    struct sceMpegCbData callback;

    callback.type = 2;
    _dispatchMpegCallback(decoder->mpeg, &callback);
    _waitIpuIdle(decoder);
    *IPU_CMD = 0;
    _waitIpuIdle(decoder);
    interrupts_enabled = DIntr();
    *D4_MADR = matrix & 0x0FFFFFFF;
    *D4_QWC = 4;
    *D4_CHCR = 0x101;
    if (interrupts_enabled != 0) {
        EnableInterrupts();
    }
    _sendIpuCommand(decoder, command);
    _waitIpuIdle(decoder);
    callback.type = 3;
    _dispatchMpegCallback(decoder->mpeg, &callback);
}
