#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/input/pad_state.h"
#include "sda.h"
#include "rnc/globals.h"

typedef struct {
    s32 state;    /* 0x00 */
    s32 timer;    /* 0x04 */
    s32 pad08[6]; /* 0x08 */
    s32 msg;      /* 0x20 */
    s32 pending;  /* 0x24 */
    s32 entry;    /* 0x28 */
    s32 count;    /* 0x2C */
    s32 started;  /* 0x30 */
    s32 delay;    /* 0x34 */
    s16 queued;   /* 0x38 */
    s16 shown;    /* 0x3A */
    s32 wait[1];  /* 0x3C */
} HelpState;

typedef struct {
    void *text;
    s32 pad04;
    s32 id;
    s32 pad0C;
} TextEntry;

typedef struct {
    u16 count;
    u16 best;
    u32 flags;
} HelpRecord;


extern HelpRecord D_00141968[];
extern u8 D_0015EE1D;
extern s32 D_0015EEA4;
extern TextEntry *D_0015F6A0;
extern HelpState D_001996D0;

extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 func_001F9740(void *);
extern s32 find_help_message_index(s32 arg0) __asm__("FUN_001fdca0");
extern void link_localized_display_text(void) __asm__("FUN_001fdd58");
extern s32 force_help_message(s32 arg0, s32 arg1) __asm__("FUN_002151d8");
extern void continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");

void update_help_state(void) __asm__("FUN_001fde90");

#define PLAY_TIME (scale_game_frames(D_0015EEA4) / 600)

#define RECORD_COUNT()                                                                             \
    if (D_00141968[D_001996D0.entry].count < 0xFFFF) {                                             \
        D_00141968[D_001996D0.entry].count++;                                                      \
    }

#define RECORD_BEST()                                                                              \
    if (PLAY_TIME > D_00141968[D_001996D0.entry].best) {                                           \
        D_00141968[D_001996D0.entry].best = PLAY_TIME;                                             \
    }

#define RECORD_FLAGS()                                                                             \
    D_00141968[D_001996D0.entry].flags =                                                           \
        D_00141968[D_001996D0.entry].flags | (1 << current_level_index) | 0x80000000

void update_help_state(void) {

    if (D_001996D0.started == 0) {
        if (D_001996D0.delay == 0 && (controller_state.held & 0xF000)) {
            D_001996D0.delay = 1;
        }
        if (D_001996D0.delay != 0) {
            if (++D_001996D0.delay >= scale_game_frames(0x78)) {
                D_001996D0.started = 1;
            }
        }
    }

    if (game_mode != 0 || D_001996D0.started == 0) {
        D_001996D0.pending = -1;
        D_001996D0.state = 0;
        D_001996D0.timer = 0;
        return;
    }

    D_001996D0.timer++;
    switch (D_001996D0.state) {
    case 8:
        if (D_001996D0.queued != 0) {
            break;
        }
        if (D_001996D0.msg >= 0) {
            if (D_001996D0.shown == 0) {
                if (func_001F9740(D_001996D0.wait) != 0) {
                    link_localized_display_text();
                    D_001996D0.shown++;
                }
                break;
            }
            RECORD_COUNT();
        }
        RECORD_BEST();
        RECORD_FLAGS();
        D_001996D0.msg = -1;
        D_001996D0.state = 0;
        D_001996D0.shown = 0;
        break;

    case 0:
        if (D_001996D0.pending >= 0) {
            D_001996D0.msg = find_help_message_index(D_001996D0.pending);
            D_001996D0.pending = -1;
            if (D_001996D0.msg >= 0) {
                link_localized_display_text();
            }
        }
        break;

    case 1: {
        s32 id;

        force_help_message(5, 0);
        id = D_0015F6A0[D_001996D0.msg].id;
        if (id >= 0 && music_stream_state.secondary.handle == 0 && music_stream_state.queued_secondary_track == -1) {
            music_stream_state.queued_secondary_track = id + 0x7530;
        }
        if (controller_state.pressed & 0x10) {
            RECORD_COUNT();
            RECORD_BEST();
            RECORD_FLAGS();
            D_001996D0.timer = 8 - D_001996D0.timer;
            D_001996D0.state = 7;
        } else if (D_001996D0.timer >= 6) {
            D_001996D0.timer = 0;
            D_001996D0.state = 2;
        }
        break;
    }

    case 2: {
        s32 id;

        force_help_message(5, 0);
        if (controller_state.pressed & 0x10) {
            RECORD_COUNT();
            RECORD_BEST();
            D_001996D0.state = 7;
            D_001996D0.timer = 0;
            RECORD_FLAGS();
        } else if (D_001996D0.timer >= scale_game_frames(0x18)) {
            if (music_stream_state.secondary.state == 3 || (id = D_0015F6A0[D_001996D0.msg].id) == -1 ||
                id != music_stream_state.secondary.track - 0x7530) {
                D_001996D0.state = 3;
                D_001996D0.timer = 0;
            }
        }
        break;
    }

    case 3:
        force_help_message(5, 0);
        if (controller_state.pressed & 0x10) {
            RECORD_COUNT();
            RECORD_BEST();
            D_001996D0.state = 7;
            D_001996D0.timer = 0;
            RECORD_FLAGS();
        } else if (D_001996D0.timer >= 8) {
            D_001996D0.timer = 0;
            D_001996D0.state = 4;
        }
        break;

    case 4: {
        s32 id;

        force_help_message(5, 0);
        if (controller_state.pressed & 0x10) {
            RECORD_COUNT();
            RECORD_BEST();
            D_001996D0.state = 6;
            RECORD_FLAGS();
            D_001996D0.timer = 4 - D_001996D0.timer;
        } else if (D_001996D0.timer >= 4) {
            id = D_0015F6A0[D_001996D0.msg].id;
            if (id != -1 && id == music_stream_state.secondary.track - 0x7530 && music_stream_state.secondary.state == 3) {
                continue_audio_stream_if_ready();
            }
            D_001996D0.state = 5;
            D_001996D0.timer = 0;
        }
        break;
    }

    case 5: {
        s32 id;

        force_help_message(5, 0);
        if ((D_001996D0.timer >= scale_game_frames(0x1A4) &&
             ((id = D_0015F6A0[D_001996D0.msg].id) == -1 || id != music_stream_state.secondary.track - 0x7530 ||
              (music_stream_state.secondary.handle == 0 && music_stream_state.queued_secondary_track == -1))) ||
            (controller_state.pressed & 0x10)) {
            RECORD_COUNT();
            RECORD_BEST();
            D_001996D0.state = 6;
            D_001996D0.timer = 0;
            RECORD_FLAGS();
        }
        break;
    }

    case 6:
        if (D_0015EE1D == 0 || D_001996D0.timer >= 4 || (controller_state.pressed & 0x10)) {
            D_001996D0.state = 7;
            D_001996D0.timer = 0;
        }
        break;

    case 7: {
        s32 id;

        force_help_message(5, 0);
        id = D_0015F6A0[D_001996D0.msg].id;
        if (id != -1 && id == music_stream_state.secondary.track - 0x7530 && (u16)music_stream_state.secondary.state - 6U >= 2) {
            music_stream_state.secondary.state = 5;
        }
        if (D_001996D0.timer >= 8) {
            if (D_001996D0.queued != 0) {
                D_001996D0.state = 8;
            } else {
                D_001996D0.state = 0;
                D_001996D0.msg = -1;
            }
            D_001996D0.timer = 0;
        }
        break;
    }
    }
}

extern __typeof__(update_help_state) func_001FDE90 __attribute__((alias("FUN_001fde90")));
