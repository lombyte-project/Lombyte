#include "types.h"
#include "asm.h"

extern void _Error(void *pState, char *message);
extern char D_00153BC8[];

void report_mpeg_picture_temporal_scalable_unsupported(void *pState) __asm__("FUN_0012cb20");

void report_mpeg_picture_temporal_scalable_unsupported(void *pState) {
    _Error(pState, D_00153BC8);
}
