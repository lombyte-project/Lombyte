/* Level 13 moby class tracked_group_watcher (0x629): remembers the classes of up to eight listed mobys and removes itself once fewer than its threshold of them are still alive. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/mobys/moby_spawn.h"

void tracked_group_watcher_update(struct Moby *moby) __asm__("FUN_L13_0030be60");

/* Moby class 1577 */
void tracked_group_watcher_update(struct Moby *moby) {
    enum {
        TRACKED_GROUP_WATCHER_STATE_INITIALIZE_TRACKING = 0,
        TRACKED_GROUP_WATCHER_STATE_MONITOR_TRACKED_MOBYS = 1,
        TRACKED_GROUP_WATCHER_STATE_ZERO_THRESHOLD_IDLE = 2
    };
    int *moby_vars = (int *)moby->pvars;
    if (moby->state == TRACKED_GROUP_WATCHER_STATE_INITIALIZE_TRACKING) {
        int  *ids = moby_vars + 4;
        int  *cls = moby_vars + 12;
        char *base;
        int   i;
        moby->state = TRACKED_GROUP_WATCHER_STATE_MONITOR_TRACKED_MOBYS;
        base = (char *)moby_base;
        for (i = 0; i < 8; i++) {
            unsigned char *o = (unsigned char *)(base + (ids[i] << 8));
            if (o != 0 && o[0x20] != 0xFE && o[0x20] != 0xFD)
                cls[i] = *(short *)(o + 0xA6);
            else
                cls[i] = -1;
        }
        if (moby_vars[0] == 0)
            moby->state = TRACKED_GROUP_WATCHER_STATE_ZERO_THRESHOLD_IDLE;
    } else if (moby->state == TRACKED_GROUP_WATCHER_STATE_MONITOR_TRACKED_MOBYS) {
        int  *ids = moby_vars + 4;
        int  *cls = moby_vars + 12;
        char *base = (char *)moby_base;
        int   valid_tracked_moby_count = 0;
        int   i;
        for (i = 0; i < 8; i++) {
            int            tracked_moby_index = ids[i];
            int            saved_moby_class;
            unsigned char *o;
            if (tracked_moby_index != -1 && (saved_moby_class = cls[i]) != -1) {
                o = (unsigned char *)(base + (tracked_moby_index << 8));
                if (o != 0 && *(short *)(o + 0xA6) == saved_moby_class && o[0x20] != 0xFE &&
                    o[0x20] != 0xFD) {
                    valid_tracked_moby_count++;
                    continue;
                }
            }
            cls[i] = -1;
        }
        if (valid_tracked_moby_count < moby_vars[0])
            mark_moby_for_removal(moby);
    }
}
