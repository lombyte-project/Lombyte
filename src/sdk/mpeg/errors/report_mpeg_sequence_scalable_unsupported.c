#include "types.h"
#include "asm.h"

#include "types.h"
#include "rnc/sdk/mpeg/errors.h"

const u8 D_00153B48[] = "_sequenceScalableExtension() is not implemented";

void report_mpeg_sequence_scalable_unsupported(void *pState) __asm__("FUN_0012caf0");

void report_mpeg_sequence_scalable_unsupported(void *pState) {
    _Error(pState, D_00153B48);
}
