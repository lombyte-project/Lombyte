#include "types.h"
#include "rnc/globals.h"
#include "rnc/input/pad_state.h"
#include "rnc/movies/decoder/read_mpeg.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"
#include "rnc/game/options.h"
#include "rnc/audio/mixer/snd_set_master_volume.h"
#include "rnc/audio/voice_pool.h"

extern struct PadStateWords pad_words __asm__("D_0013C940");
extern s32                  D_0015EEA0;
/* Movie skip mode (decode_mode): -1 no skipping, 2 any new button. Read
   through a volatile view because retail loads it once for the -1 and 2
   tests and then again inside the skip-allowed test, with no store or call
   in between; this compiler only repeats such a load for a volatile
   object. */
#define DECODE_MODE (*(volatile s32 *)&decode_mode)
extern void FlushCache(s32);
extern s32  sceGsSyncV(s32);
extern s32  sceMpegDemuxPssRing(struct VideoDec *, u8 *, s32, struct ReadBuf *, s32);
extern s32  snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void update_primary_pad_state(void) __asm__("func_00217A10");
extern void switch_thread(void) __asm__("func_0023A770");
extern s32  is_audio_ok(void) __asm__("func_0023A790");
extern s32  proceed_audio(void) __asm__("func_0023ABA0");
extern void audio_dec_start(struct AudioDec *) __asm__("func_0023ACB8");
extern void audio_dec_reset(struct AudioDec *) __asm__("FUN_0023ad10");
extern void start_display(s32) __asm__("func_0023B590");
extern void end_display(void) __asm__("ClearStageStateFlag");
extern s32  read_cd_stream_sectors(struct MpegCdStream *, u8 *, s32, s32) __asm__("func_0023BA60");
extern s32  video_dec_flush(struct VideoDec *) __asm__("func_0023CD08");
extern s32  video_dec_is_flushed(struct VideoDec *) __asm__("FUN_0023cde0");

/* Each pad test takes the pad by pointer, so retail forms the pad address
   afresh for every test from one shared upper half. */
static inline s32 pad_pressed(struct PadStateWords *pad, s32 mask) {
    return pad->buttons.w.pressed & mask;
}

static inline s32 pad_chord(struct PadStateWords *pad, u64 mask) {
    if ((pad->buttons.held_pressed & mask) != mask) {
        return 0;
    }
    return 1;
}

s32 read_mpeg(struct VideoDec     *video_dec,
              struct ReadBuf      *read_buf,
              struct MpegCdStream *cd_stream) __asm__("FUN_0023a460");

/* Feeds a PSS movie from the CD stream through the ring buffer into the
   demuxer until fewer than five bytes are left to decode or the decoder
   stops (state 3), starts display and audio once the video output buffer
   is full and the audio is ready, then flushes the decoder, shuts the
   movie down and restores mixer channel 5. Returns 1 if the player
   skipped the movie: any new button in skip mode 2, start where skipping
   is allowed, or the L1+L2+R1+R2+start chord. */
s32 read_mpeg(struct VideoDec *video_dec,
              struct ReadBuf  *read_buf,

              struct MpegCdStream *cd_stream) {
    u8 *write_ptr;
    u8 *read_ptr;
    s32 skipped;
    s32 started;
    s32 skip;
    s32 mode;
    s32 decode_remaining;
    s32 read_remaining;
    s32 space;
    s32 avail;
    s32 count;

    skipped = 0;
    started = 0;
    decode_remaining = cd_stream->byte_count;
    FlushCache(0);
    read_remaining = decode_remaining;
    FlushCache(2);
    sceGsSyncV(0);
    while (decode_remaining >= 5 && video_dec_get_state(video_dec) != VD_STATE_END) {
        update_primary_pad_state();
        mode = DECODE_MODE;
        if (mode != -1) {
            if ((mode == 2 && pad_pressed(&pad_words, -1)) ||
                ((D_0015EEA0 != 0 || alternate_animation_sequence != 0 || DECODE_MODE != 0 ||
                  current_level_index <= 0) &&
                 pad_pressed(&pad_words, 0x800))) {
                skip = 1;
            } else {
                skip = pad_chord(&pad_words, 0x8000000000FULL);
            }
            if (skip) {
                skipped = 1;
                video_dec_abort(&movie_player->video_dec);
            }
        }
        space = read_buf_begin_put(read_buf, &write_ptr);
        if (read_remaining > 0 && space > 0xFFFF) {
            count = read_cd_stream_sectors(cd_stream, write_ptr, 0x10000, 0);
            read_remaining -= count;
            read_buf_end_put(read_buf, count);
        }
        proceed_audio();
        switch_thread();
        avail = read_buf_begin_get(read_buf, &read_ptr);
        if (avail > 0) {
            count = sceMpegDemuxPssRing(video_dec, read_ptr, avail, read_buf, read_buf->size);
            decode_remaining -= count;
            read_buf_end_get(read_buf, count);
        }
        proceed_audio();
        if (!started && vo_buf_is_full(&movie_player->vo_buf) && is_audio_ok()) {
            started = 1;
            start_display(1);
            audio_dec_start(&movie_player->audio_dec);
        }
    }
    while (!video_dec_flush(video_dec)) {
        proceed_audio();
        switch_thread();
    }
    while (!video_dec_is_flushed(video_dec) && video_dec_get_state(video_dec) != VD_STATE_END) {
        proceed_audio();
        switch_thread();
    }
    end_display();
    audio_dec_reset(&movie_player->audio_dec);
    snd_set_master_volume(5, voice_pool.group_5_volume);
    snd_flush_sound_commands();
    return skipped;
}

extern __typeof__(read_mpeg) func_0023A460 __attribute__((alias("FUN_0023a460")));
