/* Level 14 group clear trigger moby class (0x617): unlocks item 0x17 once its moby group is gone, and points the cutscene camera at a mission moby. */
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/l14/cutscene_state.h"

extern s32 D_L14_0015F5C4;

extern void queue_dialogue_message(s32, s32) __asm__("FUN_L00_00263d40");

extern int D_L14_0015F5C4;

extern void cutscene_set_camera_target(void *) __asm__("FUN_L00_002637f8");

extern void queue_dialogue_message(int message_id,
                                   int message_duration_frames) __asm__("FUN_L00_00263d40");

s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void update_group_clear_trigger(struct Moby *moby) __asm__("FUN_L14_003087c0");

void update_group_clear_trigger(struct Moby *moby) {
    enum {
        MOBY_UPDATE_DISTANCE_INITIALIZE = 0,
        MOBY_UPDATE_CUTSCENE_TARGET = 1
    };
    int     *moby_pvars = (int *)moby->pvars;
    unsigned level_state_offset;
    int      i;
    if (moby_pvars[0] != -1) {
        if (count_active_moby_group_members(moby_pvars[0], -1) == 0) {
            if (item_unlocked[0x17] == 0) {
                item_unlocked[0x17] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53d6, -1);
            }
        }
    }
    switch (moby->state) {
    case MOBY_UPDATE_DISTANCE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_UPDATE_CUTSCENE_TARGET:
        if (D_L14_0015F5C4 == 2) {
            level_state_offset = D_L14_0016CF60.scene - 3;
            if (level_state_offset < 3) {
                i = 0;
                if (level_state_offset <= D_L14_0015F5C4)
                    i = moby->state;
                cutscene_set_camera_target(D_L14_0016CF60.scene_mobys[i]);
            }
        }
        break;
    }
}
