#ifndef LOMBYTE_RNC_SDK_GRAPH_H
#define LOMBYTE_RNC_SDK_GRAPH_H

#include "rnc/sdk/libgraph.h"
#include "types.h"

struct sceGsDispEnv;
struct sceGsDrawEnv1;

void ClearMemoryRange(u8 *, s32) __asm__("Memclr");
s32  sceGsExecLoadImage(u8 *, u8 *);
void sceGsPutDispEnv(struct sceGsDispEnv *);
s32  sceGsPutDrawEnv(u64 *);
void sceGsResetGraph(s16, s16, s16, s16);
void sceGsSetDefDispEnv(struct sceGsDispEnv *, s16, s16, s16, s16, s16);
s32  sceGsSetDefDrawEnv(struct sceGsDrawEnv1 *, s16, s16, s16, s16, s16);
s32  sceGsSetDefLoadImage(sceGsLoadImage *, s16, s16, s16, s16, s16, s16, s16);
s32  sceGsSyncV(s32);
s32  sceGsSyncVCallback(s32);

#endif /* LOMBYTE_RNC_SDK_GRAPH_H */
