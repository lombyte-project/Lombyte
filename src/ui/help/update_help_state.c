#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/input/pad_state.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/ui/help/force_help_message.h"
#include "rnc/ui/help/find_help_message_index.h"

#include "rnc/ui/help/window_state.h"

#include "rnc/ui/help/message_entry.h"

#include "rnc/ui/help/help_record.h"
#include "rnc/math/fast_math.h"

extern HelpMessageEntry *help_msgs __asm__("D_0015F6A0");
extern HelpWindowState   help_window_state __asm__("D_001996D0");

extern s32  fast_dec_timer(void *) __asm__("func_001F9740");
extern void link_localized_display_text(void) __asm__("FUN_001fdd58");
extern void continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");

void update_help_state(void) __asm__("FUN_001fde90");

#define PLAY_TIME_BUCKET (scale_game_frames(D_0015EEA4) / 600)

#define RECORD_COUNT()                                                    \
    if (D_00141968[help_window_state.help_record_index].count < 0xFFFF) { \
        D_00141968[help_window_state.help_record_index].count++;          \
    }

#define RECORD_LATEST_TIME()                                                                     \
    if (PLAY_TIME_BUCKET > D_00141968[help_window_state.help_record_index].latest_time_bucket) { \
        D_00141968[help_window_state.help_record_index].latest_time_bucket = PLAY_TIME_BUCKET;   \
    }

#define RECORD_FLAGS()                                                                             \
    D_00141968[help_window_state.help_record_index].level_flags =                                  \
        D_00141968[help_window_state.help_record_index].level_flags | (1 << current_level_index) | \
        0x80000000

void update_help_state(void) {
    if (help_window_state.input_delay_elapsed == 0) {
        if (help_window_state.input_delay_timer == 0 &&
            (controller_state.held & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT))) {
            help_window_state.input_delay_timer = 1;
        }
        if (help_window_state.input_delay_timer != 0) {
            if (++help_window_state.input_delay_timer >= scale_game_frames(0x78)) {
                help_window_state.input_delay_elapsed = 1;
            }
        }
    }

    if (game_mode != 0 || help_window_state.input_delay_elapsed == 0) {
        help_window_state.pending_message_id = -1;
        help_window_state.state = HELP_WINDOW_IDLE;
        help_window_state.state_timer = 0;
        return;
    }

    help_window_state.state_timer++;
    switch (help_window_state.state) {
    case HELP_WINDOW_QUEUED_MESSAGE_WAIT:
        if (help_window_state.queued != 0) {
            break;
        }
        if (help_window_state.message_index >= 0) {
            if (help_window_state.shown == 0) {
                if (fast_dec_timer(help_window_state.wait_timer) != 0) {
                    link_localized_display_text();
                    help_window_state.shown++;
                }
                break;
            }
            RECORD_COUNT();
        }
        RECORD_LATEST_TIME();
        RECORD_FLAGS();
        help_window_state.message_index = -1;
        help_window_state.state = HELP_WINDOW_IDLE;
        help_window_state.shown = 0;
        break;

    case HELP_WINDOW_IDLE:
        if (help_window_state.pending_message_id >= 0) {
            help_window_state.message_index =
                find_help_message_index(help_window_state.pending_message_id);
            help_window_state.pending_message_id = -1;
            if (help_window_state.message_index >= 0) {
                link_localized_display_text();
            }
        }
        break;

    case HELP_WINDOW_OPENING: {
        s32 audio_track_offset;

        force_help_message(5, 0);
        audio_track_offset = help_msgs[help_window_state.message_index].audio_track_offset;
        if (audio_track_offset >= 0 && music_stream_state.secondary.handle == 0 &&
            music_stream_state.queued_secondary_track == -1) {
            music_stream_state.queued_secondary_track = audio_track_offset + 0x7530;
        }
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            RECORD_FLAGS();
            help_window_state.state_timer = 8 - help_window_state.state_timer;
            help_window_state.state = HELP_WINDOW_CLOSING;
        } else if (help_window_state.state_timer >= 6) {
            help_window_state.state_timer = 0;
            help_window_state.state = HELP_WINDOW_PROMPT;
        }
        break;
    }

    case HELP_WINDOW_PROMPT: {
        s32 audio_track_offset;

        force_help_message(5, 0);
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_window_state.state = HELP_WINDOW_CLOSING;
            help_window_state.state_timer = 0;
            RECORD_FLAGS();
        } else if (help_window_state.state_timer >= scale_game_frames(0x18)) {
            if (music_stream_state.secondary.state == 3 ||
                (audio_track_offset =
                     help_msgs[help_window_state.message_index].audio_track_offset) == -1 ||
                audio_track_offset != music_stream_state.secondary.track - 0x7530) {
                help_window_state.state = HELP_WINDOW_EXPANDING;
                help_window_state.state_timer = 0;
            }
        }
        break;
    }

    case HELP_WINDOW_EXPANDING:
        force_help_message(5, 0);
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_window_state.state = HELP_WINDOW_CLOSING;
            help_window_state.state_timer = 0;
            RECORD_FLAGS();
        } else if (help_window_state.state_timer >= 8) {
            help_window_state.state_timer = 0;
            help_window_state.state = HELP_WINDOW_TEXT_FADE_IN;
        }
        break;

    case HELP_WINDOW_TEXT_FADE_IN: {
        s32 audio_track_offset;

        force_help_message(5, 0);
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_window_state.state = HELP_WINDOW_TEXT_FADE_OUT;
            RECORD_FLAGS();
            help_window_state.state_timer = 4 - help_window_state.state_timer;
        } else if (help_window_state.state_timer >= 4) {
            audio_track_offset = help_msgs[help_window_state.message_index].audio_track_offset;
            if (audio_track_offset != -1 &&
                audio_track_offset == music_stream_state.secondary.track - 0x7530 &&
                music_stream_state.secondary.state == 3) {
                continue_audio_stream_if_ready();
            }
            help_window_state.state = HELP_WINDOW_TEXT_VISIBLE;
            help_window_state.state_timer = 0;
        }
        break;
    }

    case HELP_WINDOW_TEXT_VISIBLE: {
        s32 audio_track_offset;

        force_help_message(5, 0);
        if ((help_window_state.state_timer >= scale_game_frames(0x1A4) &&
             ((audio_track_offset =
                   help_msgs[help_window_state.message_index].audio_track_offset) == -1 ||
              audio_track_offset != music_stream_state.secondary.track - 0x7530 ||
              (music_stream_state.secondary.handle == 0 &&
               music_stream_state.queued_secondary_track == -1))) ||
            (controller_state.pressed & PAD_TRIANGLE)) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_window_state.state = HELP_WINDOW_TEXT_FADE_OUT;
            help_window_state.state_timer = 0;
            RECORD_FLAGS();
        }
        break;
    }

    case HELP_WINDOW_TEXT_FADE_OUT:
        if (D_0015EE1D == 0 || help_window_state.state_timer >= 4 ||
            (controller_state.pressed & PAD_TRIANGLE)) {
            help_window_state.state = HELP_WINDOW_CLOSING;
            help_window_state.state_timer = 0;
        }
        break;

    case HELP_WINDOW_CLOSING: {
        s32 audio_track_offset;

        force_help_message(5, 0);
        audio_track_offset = help_msgs[help_window_state.message_index].audio_track_offset;
        if (audio_track_offset != -1 &&
            audio_track_offset == music_stream_state.secondary.track - 0x7530 &&
            (u16)music_stream_state.secondary.state - 6U >= 2) {
            music_stream_state.secondary.state = 5;
        }
        if (help_window_state.state_timer >= 8) {
            if (help_window_state.queued != 0) {
                help_window_state.state = HELP_WINDOW_QUEUED_MESSAGE_WAIT;
            } else {
                help_window_state.state = HELP_WINDOW_IDLE;
                help_window_state.message_index = -1;
            }
            help_window_state.state_timer = 0;
        }
        break;
    }
    }
}

extern __typeof__(update_help_state) func_001FDE90 __attribute__((alias("FUN_001fde90")));

HelpRecord D_00141968[HELP_RECORD_COUNT] = {{0}};
