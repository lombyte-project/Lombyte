#ifndef LOMBYTE_RNC_GAME_USAGE_STATS_H
#define LOMBYTE_RNC_GAME_USAGE_STATS_H

#include "types.h"

/*
 * Usage counters (usage_stats): an array of 8-byte records, one per action.
 * Every writer does the same three steps when the action happens: bump count
 * (saturating at 0xFFFF), raise unk2 to scale_game_frames(D_0015EEA4) / 600,
 * and OR (1 << current_level_index) | 0x80000000 into level_mask.
 * Record 0 counts hyperstrikes; the save file restores the counters.
 * Records 36 on carry their own label, D_00141968.
 */
struct UsageStat {
    u16 count;      /* saturating counter */
    u16 unk2;       /* max of scale_game_frames(D_0015EEA4) / 600 */
    s32 level_mask; /* bit per level seen; bit 31 = ever used */
};

struct UsageStats {
    struct UsageStat stat[36];
};

extern struct UsageStats usage_stats __asm__("D_00141848");

#endif /* LOMBYTE_RNC_GAME_USAGE_STATS_H */
