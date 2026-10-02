#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _setDefaultQM; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/setdefaultqm/_setDefaultQM.s", _setDefaultQM);
#else
#include "types.h"

struct MpegQuantizerState {
    u8 pad_0[0x858];
    void *callback_context;
};

typedef struct {
    s32 type;
    u8 pad[0x1C];
} MpegCallbackArgument;

extern s32 DIntr(void);
extern void EnableInterrupts(void);
extern void _dispatchMpegCallback(void *, MpegCallbackArgument *);
extern void _sendIpuCommand(struct MpegQuantizerState *, s32);
extern void _waitIpuIdle(struct MpegQuantizerState *);

void LoadDefaultMpegQuantizerMatrix(struct MpegQuantizerState *state, s32 command, s32 source_address) __asm__("_setDefaultQM");

void LoadDefaultMpegQuantizerMatrix(struct MpegQuantizerState *state, s32 command, s32 source_address) {
    s32 interrupts_enabled;
    MpegCallbackArgument callback;

    callback.type = 2;
    _dispatchMpegCallback(state->callback_context, &callback);
    _waitIpuIdle(state);
    *(volatile s32 *)0x10002000 = 0;
    _waitIpuIdle(state);
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
    _dispatchMpegCallback(state->callback_context, &callback);
}
#endif /* NON_MATCHING */
