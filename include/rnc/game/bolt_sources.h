#ifndef LOMBYTE_RNC_GAME_BOLT_SOURCES_H
#define LOMBYTE_RNC_GAME_BOLT_SOURCES_H

#include "types.h"

/* Per-level, per-source bolt counts; a bolt moby's source id (byte 0xB1) picks the slot. */
typedef struct BoltSourceStats {
    s16 uid;   /* source uid + 1, 0 for a free slot */
    s16 bolts; /* bolts collected from this source */
} BoltSourceStats;

extern BoltSourceStats bolt_source_stats[][64] __asm__("D_0014D590");

#endif /* LOMBYTE_RNC_GAME_BOLT_SOURCES_H */
