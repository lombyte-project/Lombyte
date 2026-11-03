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
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"

#include "rnc/ui/help/help_record.h"

#include "rnc/game/item_state.h"

extern HelpRecord help_records[] __asm__("D_00141968");
extern void       help_request(s32, s32) __asm__("FUN_L00_00203908");

typedef struct {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
    s32 state;
} VendorEventData;

void help_hints_update(struct Moby *moby) __asm__("FUN_L08_00307540");

/* Moby class 1349 */
void help_hints_update(struct Moby *moby) {
    enum {
        VENDOR_EVENT_WAIT_FOR_ARMING = 0,
        VENDOR_EVENT_ARMED = 1
    };
    VendorEventData *event_trigger_data = *(VendorEventData **)((char *)moby + 0x78);
    s32              elapsed;
    s32              time;
    f32              volume[4];

    switch (event_trigger_data->state) {
    case VENDOR_EVENT_WAIT_FOR_ARMING:
        if (is_point_inside_clip_volume(&hero.motion.pos, event_trigger_data->third) &&
            hero.state.control_mode == HERO_MODE_GRIND)
            event_trigger_data->state = VENDOR_EVENT_ARMED;
        break;
    case VENDOR_EVENT_ARMED:
        if (hero.state.current == 0x42) {
            event_trigger_data->state = VENDOR_EVENT_WAIT_FOR_ARMING;
        } else if (is_point_inside_clip_volume(&hero.motion.pos, event_trigger_data->fourth) &&
                   item_unlocked[11] == 0) {
            item_unlocked[11] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
        break;
    }
    if (item_available[29] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, event_trigger_data->first)) {
            elapsed = scale_game_frames(D_0015EEA4) - help_records[48].latest_time_bucket * 600;
            if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                help_records[48].latest_time_bucket * 600 == 0) {
                help_request(8000, 0x30);
            } else {
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[48].latest_time_bucket < time)
                    help_records[48].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
        }
    }
    if (item_available[29] != 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, event_trigger_data->first) &&
            alternate_item_available[47] == 0) {
            if (help_records[49].count != 0) {
                elapsed = scale_game_frames(D_0015EEA4) - help_records[49].latest_time_bucket * 600;
                if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                    help_records[49].latest_time_bucket * 600 == 0) {
                    help_request(8001, 0x31);
                } else {
                    time = scale_game_frames(D_0015EEA4) / 600;
                    if (help_records[49].latest_time_bucket < time)
                        help_records[49].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
                }
            } else {
                help_records[49].count++;
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[49].latest_time_bucket < time)
                    help_records[49].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
                help_records[49].level_flags =
                    help_records[49].level_flags | (1 << current_level_index) | 0x80000000;
            }
        }
    }
    if (item_available[28] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, event_trigger_data->second) &&
            D_0013DD4A != 0) {
            elapsed = scale_game_frames(D_0015EEA4) - help_records[50].latest_time_bucket * 600;
            if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                help_records[50].latest_time_bucket * 600 == 0) {
                help_request(8002, 0x32);
            } else {
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[50].latest_time_bucket < time)
                    help_records[50].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
            }
            goto sample_volume;
        }
    }
    if (item_available[28] != 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, event_trigger_data->second) &&
            D_0013DD4A != 0) {
            if (help_records[112].count != 0) {
                elapsed =
                    scale_game_frames(D_0015EEA4) - help_records[112].latest_time_bucket * 600;
                if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                    help_records[112].latest_time_bucket * 600 == 0) {
                    help_request(8005, 0x70);
                } else {
                    time = scale_game_frames(D_0015EEA4) / 600;
                    if (help_records[112].latest_time_bucket < time)
                        help_records[112].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
                }
            } else {
                help_records[112].count++;
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[112].latest_time_bucket < time)
                    help_records[112].latest_time_bucket = scale_game_frames(D_0015EEA4) / 600;
                help_records[112].level_flags =
                    help_records[112].level_flags | (1 << current_level_index) | 0x80000000;
            }
        }
    }
sample_volume:
    *(u128 *)volume = 0;
    volume[2] = -1.0f;
    if (fast_vec_dot((char *)&hero + 0x290, volume) < 0.1f)
        help_records[112].count = -1;
    if (alternate_item_available[47] == 0 && hero.state.control_mode == HERO_MODE_GRIND)
        alternate_item_available[47] = 1;
}
