/* Ported from rac1-decomp (src/game/music.c, func_00216F48). */
extern void snd_continue_vag_stream(void *) __asm__("func_0012ECA0");
typedef struct {
    unsigned int handle; /* 0x00: 0 none, 0xFFFFFFFF starting/released */
    short id;            /* 0x04 */
    short unk06;
    short unk08;
    short state; /* 0x0A: low bits the state, 0x8000 paused */
    short fade;  /* 0x0C: 0x8000 fading */
    short fadeT; /* 0x0E */
} MusicPlaying;
extern void snd_stop_sound(int) __asm__("func_0012E368");
extern void snd_pause_vag_stream(int) __asm__("FUN_0012ec70");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern void snd_get_vag_stream_time_remaining_cb(int, void (*)(int, long),
                                                 long) __asm__("FUN_0012ecd0");
extern void snd_sound_is_still_playing_cb(int, void (*)(int, long), long) __asm__("FUN_0012e448");
extern void snd_is_vag_stream_buffered_cb(int, void (*)(int, long), long) __asm__("FUN_0012ed00");
extern void music_remaining_time_callback(int, long) __asm__("FUN_00216bc0");
extern void set_sound_handle_id(int, long) __asm__("func_00216B68");
extern void music_channel_ready_callback(int, long) __asm__("FUN_00216990");
/* music_UpdateStream(music_Playing &): with a live handle and state other
   than 9, state 5 stops the stream (state 6) and state 6 without a
   handle resets; a fading record (+0xC bit 15) pauses the handle once
   (+0xA bit 15) and polls fast_dec_timer_s16 until it reports 2 (+0xC = 4),
   otherwise a paused one is resumed. Unpaused, states 1/8/9 are left
   alone, 2 hands the handle to snd_is_vag_stream_buffered_cb unless it is 0xFFFFFFFF,
   and anything but 2/3 releases the handle through snd_get_vag_stream_time_remaining_cb and
   snd_sound_is_still_playing_cb. With no live handle (or state 9) the state is cleared
   when it is 7 or the handle is 0. The handle is unsigned (retail builds
   0xFFFFFFFF with lui/ori), the special case is the else arm (retail
   places it last), and the dispatch reads p->state directly so the 2/3
   range test works on the loaded halfword as retail's does. */
void music_update_stream(MusicPlaying *p) __asm__("FUN_002160a8");

void music_update_stream(MusicPlaying *music_stream) {
    int sound_handle;

    if (music_stream->state != 9 && music_stream->handle != 0 && music_stream->handle != 0xFFFFFFFF) {
        if (music_stream->state == 5) {
            if (music_stream->handle != 0) {
                snd_stop_sound(music_stream->handle);
                music_stream->state = 6;
            } else {
                music_stream->state = 0;
            }
        } else if (music_stream->state == 6) {
            if (music_stream->handle == 0) {
                music_stream->state = 0;
            }
        }
        if (music_stream->handle == 0) {
            return;
        }
        if (music_stream->fade & 0x8000) {
            if (!(music_stream->state & 0x8000)) {
                snd_pause_vag_stream(music_stream->handle);
                music_stream->state |= 0x8000;
            }
            if (fast_dec_timer_s16(&music_stream->fadeT) == 2) {
                music_stream->fade = 4;
            }
        } else if (music_stream->state & 0x8000) {
            snd_continue_vag_stream((void *)music_stream->handle);
            music_stream->state ^= 0x8000;
        }
        if (music_stream->state & 0x8000) {
            return;
        }
        if (music_stream->state == 1 || music_stream->state == 8 || music_stream->state == 9) {
            return;
        }
        if (music_stream->state != 2 && music_stream->state != 3) {
            sound_handle = music_stream->handle;
            music_stream->handle = 0xFFFFFFFF;
            snd_get_vag_stream_time_remaining_cb(sound_handle, music_remaining_time_callback,
                                                 (long)(unsigned int)music_stream);
            snd_sound_is_still_playing_cb(sound_handle, set_sound_handle_id, (long)(unsigned int)music_stream);
            return;
        }
        if (music_stream->handle != 0xFFFFFFFF && music_stream->state == 2) {
            snd_is_vag_stream_buffered_cb(music_stream->handle, music_channel_ready_callback,
                                          (long)(unsigned int)music_stream);
        }
    } else if (music_stream->state == 7 || music_stream->handle == 0) {
        music_stream->state = 0;
    }
}

extern __typeof__(music_update_stream) func_002160A8 __attribute__((alias("FUN_002160a8")));
