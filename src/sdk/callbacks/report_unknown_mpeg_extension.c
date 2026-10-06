#include "types.h"
#include "asm.h"

typedef struct MpegVideoDecoderState MpegVideoDecoderState;

extern unsigned char D_00153B78[];
extern void _Error(MpegVideoDecoderState *pState, unsigned char *message);

void report_unknown_mpeg_extension(MpegVideoDecoderState *pState) __asm__("FUN_0012cb00");

void report_unknown_mpeg_extension(MpegVideoDecoderState *pState) {
    _Error(pState, D_00153B78);
}
