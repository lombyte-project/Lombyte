#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _setDefaultQM; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/setdefaultqm/_setDefaultQM.s",
            _setDefaultQM);
#else
#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern s32 DIntr(void);
extern void EnableInterrupts(void);
extern void _dispatchMpegCallback(void *, struct sceMpegCbData *);
extern void _sendIpuCommand(struct MpegDecoder *, s32);
extern void _waitIpuIdle(struct MpegDecoder *);

void LoadDefaultMpegQuantizerMatrix(struct MpegDecoder *state, s32 command,
                                    u32 source_address) __asm__("_setDefaultQM");

void LoadDefaultMpegQuantizerMatrix(struct MpegDecoder *state, s32 command, u32 source_address) {
    s32 interrupts_enabled;
    struct sceMpegCbData callback;

    callback.type = 2;
    _dispatchMpegCallback(state->mpeg, &callback);
    _waitIpuIdle(state);
    _waitIpuIdle((*(volatile u32 *)0x10002000 = 0, state));
    interrupts_enabled = DIntr();
    *(volatile s32 *)0x1000B410 = source_address & 0x0FFFFFFF;
    *(volatile s32 *)0x1000B420 = 4;
    *(volatile s32 *)0x1000B400 = 0x101;
    if (interrupts_enabled != 0) {
        EnableInterrupts();
    }
    _sendIpuCommand(state, command);
    _waitIpuIdle(state);
    callback.type = 3;
    _dispatchMpegCallback(state->mpeg, &callback);
}
#endif /* NON_MATCHING */
