#include "types.h"
#include "asm.h"

#include "types.h"

extern u8 D_00153B48[];
extern void _Error(void *pState, u8 *message);

void report_mpeg_sequence_scalable_unsupported(void *pState) __asm__("FUN_0012caf0");

void report_mpeg_sequence_scalable_unsupported(void *pState) {
    _Error(pState, D_00153B48);
}
