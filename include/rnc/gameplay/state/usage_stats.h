#ifndef LOMBYTE_RNC_GAMEPLAY_STATE_USAGE_STATS_H
#define LOMBYTE_RNC_GAMEPLAY_STATE_USAGE_STATS_H

#include "types.h"

/*
 * Usage counters at D_00141848: an array of 8-byte records. Every writer
 * does the same three steps when an action happens: bump count (saturating
 * at 0xFFFF), raise unk2 to scale_game_frames(D_0015EEA4) / 600, and
 * OR (1 << current_level_index) | 0x80000000 into level_mask.
 * Known writers: record 9 (overlay entities), 11 (button 0x40 held over
 * 90 frames), 12 and 29 (hero), 18 (hero), 21 (quick-select slot assigned,
 * FUN_0021c7a0), 31 (hero). Records 0-8 and the size past 0x100 are unseen.
 */
struct UsageStat {
    u16 count;      /* saturating counter */
    u16 unk2;       /* max of scale_game_frames(D_0015EEA4) / 600 */
    s32 level_mask; /* bit per level seen; bit 31 = ever used */
};

struct UsageStats {
    struct UsageStat stat[32];
};

#endif /* LOMBYTE_RNC_GAMEPLAY_STATE_USAGE_STATS_H */
