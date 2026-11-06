#include "types.h"
#include "rnc/globals.h"

struct AudioStream {
    u8  pad_0[0x44];
    s32 state;
    s32 left_buffer;
    s32 right_buffer;
};

#include "rnc/streaming/disc_table.h"
#include "rnc/audio/music/music_stream_state.h"

extern s32 start_audio_stream_read() __asm__("FUN_00216788");

s32 advance_audio_stream_state(struct AudioStream *stream) __asm__("FUN_00220648");

s32 advance_audio_stream_state(struct AudioStream *stream) {
    enum {
        AUDIO_STREAM_START_FIRST_READ = 0,
        AUDIO_STREAM_WAIT_FIRST_READ = 1,
        AUDIO_STREAM_START_SECOND_READ = 2,
        AUDIO_STREAM_WAIT_SECOND_READ = 3
    };
    enum {
        CD_READ_STATE_IDLE = 0
    };

    switch (stream->state) {
    case AUDIO_STREAM_START_FIRST_READ:
        if (stream->left_buffer == 0 || music_stream_state.read_state != CD_READ_STATE_IDLE) {
            break;
        }
        if (disc_table.controls_help_left[game_language].size == 0) {
            break;
        }
        if (start_audio_stream_read(stream->left_buffer,
                                    disc_table.controls_help_left[game_language].sector,
                                    disc_table.controls_help_left[game_language].size) != 0) {
            stream->state = stream->state + 1;
        } else {
            stream->state = -1;
        }
        break;
    case AUDIO_STREAM_WAIT_FIRST_READ:
        if (music_stream_state.read_state == CD_READ_STATE_IDLE) {
            stream->state = 2;
        }
        break;
    case AUDIO_STREAM_START_SECOND_READ:
        if (stream->right_buffer == 0 || music_stream_state.read_state != CD_READ_STATE_IDLE) {
            break;
        }
        if (disc_table.controls_help_right[game_language].size == 0) {
            break;
        }
        if (start_audio_stream_read(stream->right_buffer,
                                    disc_table.controls_help_right[game_language].sector,
                                    disc_table.controls_help_right[game_language].size) != 0) {
            stream->state = stream->state + 1;
        } else {
            stream->state = -1;
        }
        break;
    case AUDIO_STREAM_WAIT_SECOND_READ:
        if (music_stream_state.read_state == CD_READ_STATE_IDLE) {
            stream->state = 4;
        }
        break;
    }
    return 0;
}

extern __typeof__(advance_audio_stream_state) func_00220648 __attribute__((alias("FUN_00220648")));
