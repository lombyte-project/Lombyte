#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/decoder/read_mpeg/FUN_0023a460.s",
            FUN_0023a460);
#else
#include "rnc/video/decoder/read_mpeg.h"

#include "rnc/input/pad_state.h"
extern struct Globals_0013E550 D_0013E550;
extern s32 D_0015ED84;
extern s32 D_0015EE20;
extern s32 D_0015EEA0;
extern s32 D_0015EED8;
extern s32 D_0016120C;
extern void FlushCache(s32);
extern s32 sceGsSyncV(s32);
extern s32 sceMpegDemuxPssRing(struct VideoDec *, u8 *, s32, struct ReadBuf *, s32);
extern s32 snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void snd_set_master_volume(s32, s32) __asm__("func_0012E208");
extern void update_primary_pad_state(void) __asm__("func_00217A10");
extern void switch_thread(void) __asm__("func_0023A770");
extern s32 is_audio_ok(void) __asm__("func_0023A790");
extern s32 proceed_audio(void) __asm__("func_0023ABA0");
extern void audio_dec_start(s32) __asm__("func_0023ACB8");
extern void audio_dec_reset(s32) __asm__("func_0023AD10");
extern void start_display(s32) __asm__("func_0023B590");
extern void end_display(void) __asm__("func_0023B5E0");
extern s32 read_buf_begin_put(struct ReadBuf *, u8 **) __asm__("func_0023B960");
extern void read_buf_end_put(struct ReadBuf *, s32) __asm__("func_0023B990");
extern s32 read_buf_begin_get(struct ReadBuf *, u8 **) __asm__("func_0023B9D8");
extern s32 read_buf_end_get(struct ReadBuf *, s32) __asm__("func_0023BA20");
extern s32 read_cd_stream_sectors(struct MpegCdStream *, u8 *, s32, s32) __asm__("func_0023BA60");
extern s32 video_dec_abort(s32) __asm__("func_0023CC70");
extern s32 videoDecGetState(struct VideoDec *) __asm__("func_0023CC80");
extern s32 video_dec_flush(struct VideoDec *) __asm__("func_0023CD08");
extern s32 video_dec_is_flushed(struct VideoDec *) __asm__("func_0023CDE0");
extern s32 vo_buf_is_full(s32) __asm__("func_0023D1F8");
/* Retail 0x0023a460..0x0023a76f; play_mpeg_movie passes VideoDec at
   context+0xD9048, ReadBuf at context+0, and the 8-byte CD stream at +0xD9040.
   This is a PSS input pump, not the IPU decoding thread. Signed 32-bit
   read/decode counters start at the stream byte count. Read 0x10000 bytes
   synchronously only when bytes remain and ring space exceeds 0xFFFF;
   decrement/commit the returned count without adding an error test.
   Demux all available bytes with the ring base and capacity, then decrement
   the independent decode count and commit exactly the demux return value.

   Input mode -1 bypasses skip tests. Mode 2 permits any nonzero pad word
   at +0x1A4; the other gates permit bit 0x800, or the 64-bit chord
   0x8000000000F at +0x1A0. Preserve the second input-mode load. A skip
   requests VideoDec state 1 and remembers return value 1; it does not
   jump straight to cleanup. The main loop stops below five undecoded
   bytes or when decoder state becomes 3. No timeout or extra negative
   read/demux handling exists in this routine; errors are delegated.

   Audio is serviced before/after each feed and during both shutdown waits.
   Display/audio start once vo_buf_is_full and is_audio_ok both succeed.
   Flush until the end code can be queued; then wait for flush completion
   or state 3. End display, reset AudioDec at context+0xD9100, restore
   mixer channel 5 from D_0013E550+0x5C and flush sound commands, in order.

   Timestamp handling is delegated, not omitted: init_all registers
   video_callback/pcm_callback; sceMpegDemuxPssRing supplies PES timestamps.
   The callback record has data at +0x08, signed byte count at +0x0C,
   and signed 64-bit PTS/DTS at +0x10/+0x18. video_callback forwards
   these timestamps through video_dec_put_ts
   to ViBuf, while get_mpeg_timestamp returns them to libmpeg. The PCM
   callback feeds AudioDec; is_audio_ok gates startup, not a timestamp
   comparison here. These contracts follow existing C and retail callers;
   playback timing and malformed-stream behavior have not been run on PS2.
   video_dec_set_stream explicitly forwards the five callback-registration
   arguments; video_dec_put_ts returns the timestamp queue result checked
   by video_callback. Mechanical import must retain those contracts.

   Status: readable pending C, with retail oracle retained. Unknown pad and
   global meanings remain unnamed; this does not supply native IOP services. */
s32 read_mpeg(struct VideoDec *video_dec, struct ReadBuf *read_buf,
              struct MpegCdStream *cd_stream) __asm__("FUN_0023a460");

s32 read_mpeg(struct VideoDec *video_dec, struct ReadBuf *read_buf,
              struct MpegCdStream *cd_stream) {
    u8 *write_ptr;
    u8 *read_ptr;
    u64 pad_mask;
    s32 skipped;
    s32 audio_started;
    s32 decode_remaining;
    s32 read_remaining;
    s32 skip_requested;
    s32 buf_space;
    s32 buf_avail;
    s32 bytes_read;
    s32 bytes_decoded;
    struct PadState *mask_state;

    skipped = 0;
    audio_started = 0;
    decode_remaining = cd_stream->byte_count;
    FlushCache(0);
    read_remaining = decode_remaining;
    FlushCache(2);
    sceGsSyncV(0);
    goto check_decoder_state;
feed_movie:
    update_primary_pad_state();
    if (D_0015EED8 == -1) {
        goto feed_input;
    }
    if (D_0015EED8 != 2) {
        goto check_skip_policy;
    }
    skip_requested = 1;
    if (controller_state.pressed != 0) {
        goto apply_skip;
    }
check_skip_policy:
    if (D_0015EEA0 != 0) {
        goto check_start_button;
    }
    if (D_0015EE20 != 0) {
        goto check_start_button;
    }
    if (*(volatile s32 *)&D_0015EED8 != 0) {
        goto check_start_button;
    }
    if (D_0015ED84 > 0) {
        goto check_pad_chord;
    }
check_start_button:
    skip_requested = 1;
    if (controller_state.pressed & 0x800) {
        goto apply_skip;
    }
check_pad_chord:
    mask_state = &controller_state;
    pad_mask = 0x8000000000FULL;
    skip_requested = 1;
    if ((*(u64 *)&mask_state->held & pad_mask) != pad_mask) {
        skip_requested = 0;
    }
apply_skip:
    if (skip_requested == 0) {
        goto feed_input;
    }
    skipped = 1;
    video_dec_abort(D_0016120C + 0xD9048);
feed_input:
    buf_space = read_buf_begin_put(read_buf, &write_ptr);
    if (read_remaining <= 0) {
        goto feed_decoder;
    }
    if (buf_space <= 0xFFFF) {
        goto feed_decoder;
    }
    bytes_read = read_cd_stream_sectors(cd_stream, write_ptr, 0x10000, 0);
    read_remaining -= bytes_read;
    read_buf_end_put(read_buf, bytes_read);
feed_decoder:
    proceed_audio();
    switch_thread();
    buf_avail = read_buf_begin_get(read_buf, &read_ptr);
    if (buf_avail <= 0) {
        goto start_audio_when_ready;
    }
    bytes_decoded = sceMpegDemuxPssRing(video_dec, read_ptr, buf_avail, read_buf, read_buf->capacity);
    decode_remaining -= bytes_decoded;
    read_buf_end_get(read_buf, bytes_decoded);
start_audio_when_ready:
    proceed_audio();
    if (audio_started != 0) {
        goto check_remaining_bytes;
    }
    if (vo_buf_is_full(D_0016120C + 0xD9168) == 0) {
        goto check_remaining_bytes;
    }
    if (is_audio_ok() == 0) {
        goto check_decoder_state;
    }
    audio_started = 1;
    start_display(1);
    audio_dec_start(D_0016120C + 0xD9100);
check_decoder_state:
check_remaining_bytes:
    if (decode_remaining >= 5 && videoDecGetState(video_dec) != 3) {
        goto feed_movie;
    }
    /* Retail reuses the ring-pointer register for decoder state 3 here. */
    while (video_dec_flush(video_dec) == 0) {
        read_buf = (struct ReadBuf *)3;
        proceed_audio();
        switch_thread();
    }
    read_buf = (struct ReadBuf *)3;
    while (video_dec_is_flushed(video_dec) == 0 && videoDecGetState(video_dec) != (s32)read_buf) {
        proceed_audio();
        switch_thread();
    }
    end_display();
    audio_dec_reset(D_0016120C + 0xD9100);
    snd_set_master_volume(5, *(s32 *)((u8 *)&D_0013E550 + 0x5C));
    snd_flush_sound_commands();
    return skipped;
}
extern __typeof__(read_mpeg) func_0023A460 __attribute__((alias("FUN_0023a460")));

#endif /* NON_MATCHING */
