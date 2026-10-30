/* Level 08 moby class 0x545: help hints, the help messages and voice lines of the level's areas. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

typedef struct {
    u16 count;
    u16 best;
    u32 flags;
} VendorEventRecord;

#include "rnc/game/item_state.h"

extern VendorEventRecord help_records[] __asm__("D_00141968");
extern void              help_request(s32, s32) __asm__("FUN_L00_00203908");
extern void              allocate_voice_for_bank_entry(s32, s32, s32) __asm__("FUN_0022db10");

typedef struct {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
    s32 state;
} VendorEventData;

void update_help_hints(struct Moby *moby) __asm__("FUN_L08_00307540");

void update_help_hints(struct Moby *moby) {
    enum {
        VENDOR_EVENT_WAIT_FOR_ARMING = 0,
        VENDOR_EVENT_ARMED = 1
    };
    VendorEventData *data = *(VendorEventData **)((char *)moby + 0x78);
    s32              elapsed;
    s32              time;
    f32              volume[4];

    switch (data->state) {
    case VENDOR_EVENT_WAIT_FOR_ARMING:
        if (is_point_inside_clip_volume(&hero.motion.pos, data->third) &&
            hero.state.control_mode == 15)
            data->state = 1;
        break;
    case VENDOR_EVENT_ARMED:
        if (hero.state.current == 0x42) {
            data->state = 0;
        } else if (is_point_inside_clip_volume(&hero.motion.pos, data->fourth) &&
                   item_unlocked[11] == 0) {
            item_unlocked[11] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
        break;
    }
    if (item_available[29] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->first)) {
            elapsed = scale_game_frames(D_0015EEA4) - help_records[48].best * 600;
            if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                help_records[48].best * 600 == 0) {
                help_request(8000, 0x30);
            } else {
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[48].best < time)
                    help_records[48].best = scale_game_frames(D_0015EEA4) / 600;
            }
        }
    }
    if (item_available[29] != 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->first) &&
            alternate_item_available[47] == 0) {
            if (help_records[49].count != 0) {
                elapsed = scale_game_frames(D_0015EEA4) - help_records[49].best * 600;
                if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                    help_records[49].best * 600 == 0) {
                    help_request(8001, 0x31);
                } else {
                    time = scale_game_frames(D_0015EEA4) / 600;
                    if (help_records[49].best < time)
                        help_records[49].best = scale_game_frames(D_0015EEA4) / 600;
                }
            } else {
                help_records[49].count++;
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[49].best < time)
                    help_records[49].best = scale_game_frames(D_0015EEA4) / 600;
                help_records[49].flags =
                    help_records[49].flags | (1 << current_level_index) | 0x80000000;
            }
        }
    }
    if (item_available[28] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->second) && D_0013DD4A != 0) {
            elapsed = scale_game_frames(D_0015EEA4) - help_records[50].best * 600;
            if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                help_records[50].best * 600 == 0) {
                help_request(8002, 0x32);
            } else {
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[50].best < time)
                    help_records[50].best = scale_game_frames(D_0015EEA4) / 600;
            }
            goto sample_volume;
        }
    }
    if (item_available[28] != 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->second) && D_0013DD4A != 0) {
            if (help_records[112].count != 0) {
                elapsed = scale_game_frames(D_0015EEA4) - help_records[112].best * 600;
                if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                    help_records[112].best * 600 == 0) {
                    help_request(8005, 0x70);
                } else {
                    time = scale_game_frames(D_0015EEA4) / 600;
                    if (help_records[112].best < time)
                        help_records[112].best = scale_game_frames(D_0015EEA4) / 600;
                }
            } else {
                help_records[112].count++;
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[112].best < time)
                    help_records[112].best = scale_game_frames(D_0015EEA4) / 600;
                help_records[112].flags =
                    help_records[112].flags | (1 << current_level_index) | 0x80000000;
            }
        }
    }
sample_volume:
    *(u128 *)volume = 0;
    volume[2] = -1.0f;
    if (fast_vec_dot((char *)&hero + 0x290, volume) < 0.1f)
        help_records[112].count = -1;
    if (alternate_item_available[47] == 0 && hero.state.control_mode == 15)
        alternate_item_available[47] = 1;
}
