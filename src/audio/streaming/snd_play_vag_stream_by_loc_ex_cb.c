
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
extern s32 snd_send_iop_command_no_wait(s32, s32, void *, s32, u64) __asm__("FUN_0012e6e0");
void snd_play_vag_stream_by_loc_ex_cb(s32 loc, s32 size, s32 vol_l, s32 vol_r, s32 pitch, s32 flags,
                                      s32 voice, s32 channel, s32 a8, s32 id,
                                      u64 cb_data) __asm__("FUN_0012ec08");
void snd_play_vag_stream_by_loc_ex_cb(s32 loc, s32 size, s32 vol_l, s32 vol_r, s32 pitch, s32 flags,
                                      s32 voice, s32 channel, s32 a8, s32 id,
                                      u64 cb_data) __asm__("FUN_0012ec08");

void snd_play_vag_stream_by_loc_ex_cb(s32 loc, s32 size, s32 vol_l, s32 vol_r, s32 pitch, s32 flags,
                                      s32 voice, s32 channel, s32 request_word_6, s32 id, u64 cb_data) {
    int new_var;
    s32 playback_request_payload[7];
    new_var = (pitch << 16) | (vol_l & 0xFFFF);
    playback_request_payload[0] = loc;
    playback_request_payload[1] = size;
    playback_request_payload[2] = new_var;
    playback_request_payload[3] = (flags << 16) | (vol_r & 0xFFFF);
    playback_request_payload[4] = voice;
    playback_request_payload[5] = channel;
    playback_request_payload[6] = request_word_6;
    snd_send_iop_command_no_wait(0x2C, 0x1C, playback_request_payload, id, cb_data);
}

extern __typeof__(snd_play_vag_stream_by_loc_ex_cb) func_0012EC08
    __attribute__((alias("FUN_0012ec08")));
