/* Compile-time RC1 EE layouts; never linked into the game. */

#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]

#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/ui/help/help_state.h"

#include "rnc/ui/help/message_entry.h"

#include "rnc/game/usage_stats.h"

SIZE_CHECK(window, HelpState, 0x40);

SIZE_CHECK(message, HelpMessageEntry, 0x10);

SIZE_CHECK(usage, struct UsageStat, 8);

OFFSET_CHECK(window_state, HelpState, state, 0x00);

OFFSET_CHECK(window_state_timer, HelpState, timer, 0x04);

OFFSET_CHECK(window_target_half_width, HelpState, half_width, 0x08);

OFFSET_CHECK(window_target_half_height, HelpState, half_height, 0x0C);

OFFSET_CHECK(window_center_x, HelpState, center_x, 0x10);

OFFSET_CHECK(window_center_y, HelpState, center_y, 0x14);

OFFSET_CHECK(window_frame_half_width, HelpState, frame_half_width, 0x18);

OFFSET_CHECK(window_frame_half_height, HelpState, frame_half_height, 0x1C);

OFFSET_CHECK(window_message_index, HelpState, msg, 0x20);

OFFSET_CHECK(window_pending_message_id, HelpState, pending, 0x24);

OFFSET_CHECK(window_help_record_index, HelpState, entry, 0x28);

OFFSET_CHECK(window_message_count, HelpState, count, 0x2C);

OFFSET_CHECK(window_input_delay_elapsed, HelpState, started, 0x30);

OFFSET_CHECK(window_input_delay_timer, HelpState, delay, 0x34);

OFFSET_CHECK(window_queued, HelpState, queued, 0x38);

OFFSET_CHECK(window_shown, HelpState, shown, 0x3A);

OFFSET_CHECK(window_wait_timer, HelpState, wait, 0x3C);

OFFSET_CHECK(message_text, HelpMessageEntry, text, 0);

OFFSET_CHECK(message_message_id, HelpMessageEntry, message_id, 4);

OFFSET_CHECK(message_audio_track_offset, HelpMessageEntry, audio_track_offset, 8);

OFFSET_CHECK(message_unknown_0C, HelpMessageEntry, unknown_0C, 12);

OFFSET_CHECK(usage_count, struct UsageStat, count, 0);

OFFSET_CHECK(usage_time, struct UsageStat, latest_time_bucket, 2);

OFFSET_CHECK(usage_flags, struct UsageStat, level_mask, 4);
