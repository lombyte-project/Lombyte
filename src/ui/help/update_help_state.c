#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/input/pad_state.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/ui/help/force_help_message.h"
#include "rnc/ui/help/find_help_message_index.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"
#include "rnc/ui/text.h"

#include "rnc/ui/help/help_state.h"

#include "rnc/ui/help/message_entry.h"

#include "rnc/ui/help/help_record.h"
#include "rnc/math/fast_math.h"

extern HelpMessageEntry *help_msgs __asm__("D_0015F6A0");

extern s32 fast_dec_timer(void *) __asm__("func_001F9740");

void update_help_state(void) __asm__("FUN_001fde90");

#define PLAY_TIME_BUCKET (scale_game_frames(D_0015EEA4) / 600)

#define RECORD_COUNT()                                 \
    if (D_00141968[help_state.entry].count < 0xFFFF) { \
        D_00141968[help_state.entry].count++;          \
    }

#define RECORD_LATEST_TIME()                                                  \
    if (PLAY_TIME_BUCKET > D_00141968[help_state.entry].latest_time_bucket) { \
        D_00141968[help_state.entry].latest_time_bucket = PLAY_TIME_BUCKET;   \
    }

#define RECORD_FLAGS()                         \
    D_00141968[help_state.entry].level_flags = \
        D_00141968[help_state.entry].level_flags | (1 << current_level_index) | 0x80000000

void update_help_state(void) {
    enum {
        HELP_STATE_IDLE = 0,
        HELP_STATE_DISPLAY_START_DELAY = 1,
        HELP_STATE_DISMISSAL_WAIT_AUDIO_GATE = 2,
        HELP_STATE_DISMISSAL_WAIT_TRANSITION_DELAY = 3,
        HELP_STATE_DISMISSAL_WAIT_START_DELAY = 4,
        HELP_STATE_WAIT_FOR_DISMISSAL = 5,
        HELP_STATE_DISMISS_DELAY = 6,
        HELP_STATE_DISMISSING = 7,
        HELP_STATE_QUEUED_MESSAGE_WAIT = 8
    };

    if (help_state.started == 0) {
        if (help_state.delay == 0 &&
            (controller_state.held & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT))) {
            help_state.delay = 1;
        }
        if (help_state.delay != 0) {
            if (++help_state.delay >= scale_game_frames(0x78)) {
                help_state.started = 1;
            }
        }
    }

    if (game_mode != 0 || help_state.started == 0) {
        help_state.pending = -1;
        help_state.state = HELP_STATE_IDLE;
        help_state.timer = 0;
        return;
    }

    help_state.timer++;
    switch (help_state.state) {
    case HELP_STATE_QUEUED_MESSAGE_WAIT:
        if (help_state.queued != 0) {
            break;
        }
        if (help_state.msg >= 0) {
            if (help_state.shown == 0) {
                if (fast_dec_timer(help_state.wait) != 0) {
                    link_localized_display_text();
                    help_state.shown++;
                }
                break;
            }
            RECORD_COUNT();
        }
        RECORD_LATEST_TIME();
        RECORD_FLAGS();
        help_state.msg = -1;
        help_state.state = HELP_STATE_IDLE;
        help_state.shown = 0;
        break;

    case HELP_STATE_IDLE:
        if (help_state.pending >= 0) {
            help_state.msg = find_help_message_index(help_state.pending);
            help_state.pending = -1;
            if (help_state.msg >= 0) {
                link_localized_display_text();
            }
        }
        break;

    case HELP_STATE_DISPLAY_START_DELAY: {
        s32 id;

        force_help_message(5, 0);
        id = help_msgs[help_state.msg].audio_track_offset;
        if (id >= 0 && music_stream_state.secondary.handle == 0 &&
            music_stream_state.queued_secondary_track == -1) {
            music_stream_state.queued_secondary_track = id + 0x7530;
        }
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            RECORD_FLAGS();
            help_state.timer = 8 - help_state.timer;
            help_state.state = HELP_STATE_DISMISSING;
        } else if (help_state.timer >= 6) {
            help_state.timer = 0;
            help_state.state = HELP_STATE_DISMISSAL_WAIT_AUDIO_GATE;
        }
        break;
    }

    case HELP_STATE_DISMISSAL_WAIT_AUDIO_GATE: {
        s32 id;

        force_help_message(5, 0);
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_state.state = HELP_STATE_DISMISSING;
            help_state.timer = 0;
            RECORD_FLAGS();
        } else if (help_state.timer >= scale_game_frames(0x18)) {
            if (music_stream_state.secondary.state == 3 ||
                (id = help_msgs[help_state.msg].audio_track_offset) == -1 ||
                id != music_stream_state.secondary.track - 0x7530) {
                help_state.state = HELP_STATE_DISMISSAL_WAIT_TRANSITION_DELAY;
                help_state.timer = 0;
            }
        }
        break;
    }

    case HELP_STATE_DISMISSAL_WAIT_TRANSITION_DELAY:
        force_help_message(5, 0);
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_state.state = HELP_STATE_DISMISSING;
            help_state.timer = 0;
            RECORD_FLAGS();
        } else if (help_state.timer >= 8) {
            help_state.timer = 0;
            help_state.state = HELP_STATE_DISMISSAL_WAIT_START_DELAY;
        }
        break;

    case HELP_STATE_DISMISSAL_WAIT_START_DELAY: {
        s32 id;

        force_help_message(5, 0);
        if (controller_state.pressed & PAD_TRIANGLE) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_state.state = HELP_STATE_DISMISS_DELAY;
            RECORD_FLAGS();
            help_state.timer = 4 - help_state.timer;
        } else if (help_state.timer >= 4) {
            id = help_msgs[help_state.msg].audio_track_offset;
            if (id != -1 && id == music_stream_state.secondary.track - 0x7530 &&
                music_stream_state.secondary.state == 3) {
                continue_audio_stream_if_ready();
            }
            help_state.state = HELP_STATE_WAIT_FOR_DISMISSAL;
            help_state.timer = 0;
        }
        break;
    }

    case HELP_STATE_WAIT_FOR_DISMISSAL: {
        s32 id;

        force_help_message(5, 0);
        if ((help_state.timer >= scale_game_frames(0x1A4) &&
             ((id = help_msgs[help_state.msg].audio_track_offset) == -1 ||
              id != music_stream_state.secondary.track - 0x7530 ||
              (music_stream_state.secondary.handle == 0 &&
               music_stream_state.queued_secondary_track == -1))) ||
            (controller_state.pressed & PAD_TRIANGLE)) {
            RECORD_COUNT();
            RECORD_LATEST_TIME();
            help_state.state = HELP_STATE_DISMISS_DELAY;
            help_state.timer = 0;
            RECORD_FLAGS();
        }
        break;
    }

    case HELP_STATE_DISMISS_DELAY:
        if (D_0015EE1D == 0 || help_state.timer >= 4 || (controller_state.pressed & PAD_TRIANGLE)) {
            help_state.state = HELP_STATE_DISMISSING;
            help_state.timer = 0;
        }
        break;

    case HELP_STATE_DISMISSING: {
        s32 id;

        force_help_message(5, 0);
        id = help_msgs[help_state.msg].audio_track_offset;
        if (id != -1 && id == music_stream_state.secondary.track - 0x7530 &&
            (u16)music_stream_state.secondary.state - 6U >= 2) {
            music_stream_state.secondary.state = 5;
        }
        if (help_state.timer >= 8) {
            if (help_state.queued != 0) {
                help_state.state = HELP_STATE_QUEUED_MESSAGE_WAIT;
            } else {
                help_state.state = HELP_STATE_IDLE;
                help_state.msg = -1;
            }
            help_state.timer = 0;
        }
        break;
    }
    }
}

extern __typeof__(update_help_state) func_001FDE90 __attribute__((alias("FUN_001fde90")));

HelpRecord D_00141968[HELP_RECORD_COUNT] = {{0}};
