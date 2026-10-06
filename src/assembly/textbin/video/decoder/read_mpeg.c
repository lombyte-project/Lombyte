#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/decoder/read_mpeg/FUN_0023a460.s",
            FUN_0023a460);
#else
#include "rnc/video/decoder/read_mpeg.h"

extern struct PadState D_0013C940;
extern struct Globals_0013E550 D_0013E550;
extern s32 D_0015ED84;
extern s32 D_0015EE20;
extern s32 D_0015EEA0;
extern s32 D_0015EED8;
extern s32 D_0016120C;
extern void func_00118A80();
extern void func_00122298();
extern s32 func_0012ABD0();
extern void snd_flush_sound_commands() __asm__("func_0012DC80");
extern void snd_set_master_volume() __asm__("func_0012E208");
extern void update_primary_pad_state() __asm__("func_00217A10");
extern void switch_thread() __asm__("func_0023A770");
extern s32 is_audio_ok() __asm__("func_0023A790");
extern void proceed_audio() __asm__("func_0023ABA0");
extern void audio_dec_start() __asm__("func_0023ACB8");
extern void audio_dec_reset() __asm__("func_0023AD10");
extern void start_display() __asm__("func_0023B590");
extern void func_0023B5E0();
extern s32 func_0023B960();
extern void func_0023B990();
extern s32 read_buf_begin_get() __asm__("func_0023B9D8");
extern void func_0023BA20();
extern s32 read_cd_stream_sectors() __asm__("func_0023BA60");
extern void func_0023CC70();
extern s32 func_0023CC80();
extern s32 video_dec_flush() __asm__("func_0023CD08");
extern s32 video_dec_is_flushed() __asm__("func_0023CDE0");
extern s32 func_0023D1F8();
s32 read_mpeg(s32 video_dec, struct ReadBuf *arg1, s32 *cd_stream) __asm__("FUN_0023a460");

s32 read_mpeg(s32 arg0, struct ReadBuf *arg1, s32 *arg2) {
    s32 sp0;
    s32 sp4;
    s32 temp_2_111;
    s32 temp_2_120;
    s32 temp_2_99;
    s32 temp_3_92;
    u64 temp_4_74;
    s32 var_18_22;
    s32 var_19_25;
    s32 var_21_10;
    s32 var_22_8;
    s32 var_2_44;
    struct PadState *mask_state;

    skipped = 0;
    audio_started = 0;
    decode_remaining = *cd_stream;
    func_00118A80(0);
    read_remaining = decode_remaining;
    func_00118A80(2);
    func_00122298(0);
    goto loop_24;
block_2:
    update_primary_pad_state();
    if (D_0015EED8 == -1) {
        goto block_15;
    }
    if (D_0015EED8 != 2) {
        goto block_5;
    }
    skip_requested = 1;
    if (D_0013C940.unk1A4 != 0) {
        goto block_13;
    }
block_5:
    if (D_0015EEA0 != 0) {
        goto block_10;
    }
    if (D_0015EE20 != 0) {
        goto block_10;
    }
    if (*(volatile s32 *)&D_0015EED8 != 0) {
        goto block_10;
    }
    if (D_0015ED84 > 0) {
        goto block_12;
    }
block_10:
    var_2_44 = 1;
    if (D_0013C940.unk1A4 & 0x800) {
        goto block_13;
    }
block_12:
    mask_state = &D_0013C940;
    temp_4_74 = 0x8000000000FULL;
    var_2_44 = 1;
    if ((*(u64 *)&mask_state->unk1A0 & temp_4_74) != temp_4_74) {
        var_2_44 = 0;
    }
block_13:
    if (skip_requested == 0) {
        goto block_15;
    }
    skipped = 1;
    func_0023CC70(D_0016120C + 0xD9048);
block_15:
    buf_space = func_0023B960(arg1, &write_ptr);
    if (read_remaining <= 0) {
        goto block_18;
    }
    if (buf_space <= 0xFFFF) {
        goto block_18;
    }
    bytes_read = read_cd_stream_sectors(cd_stream, write_ptr, 0x10000, 0);
    read_remaining -= bytes_read;
    func_0023B990(arg1, bytes_read);
block_18:
    proceed_audio();
    switch_thread();
    buf_avail = read_buf_begin_get(arg1, &read_ptr);
    if (buf_avail <= 0) {
        goto block_20;
    }
    bytes_decoded = func_0012ABD0(video_dec, read_ptr, buf_avail, arg1, arg1->unk50008);
    decode_remaining -= bytes_decoded;
    func_0023BA20(arg1, bytes_decoded);
block_20:
    proceed_audio();
    if (audio_started != 0) {
        goto loop_25;
    }
    if (func_0023D1F8(D_0016120C + 0xD9168) == 0) {
        goto loop_25;
    }
    if (is_audio_ok() == 0) {
        goto loop_24;
    }
    audio_started = 1;
    start_display(1);
    audio_dec_start(D_0016120C + 0xD9100);
loop_24:
loop_25:
    if (decode_remaining >= 5 && func_0023CC80(video_dec) != 3) {
        goto block_2;
    }
    /* Retail reuses arg1 as the status value 3 during the shutdown waits. */
    while (video_dec_flush(video_dec) == 0) {
        arg1 = (struct ReadBuf *)3;
        proceed_audio();
        switch_thread();
    }
    arg1 = (struct ReadBuf *)3;
    while (video_dec_is_flushed(video_dec) == 0 && func_0023CC80(video_dec) != (s32)arg1) {
        proceed_audio();
        switch_thread();
    }
    func_0023B5E0();
    audio_dec_reset(D_0016120C + 0xD9100);
    snd_set_master_volume(5, *(s32 *)((u8 *)&D_0013E550 + 0x5C));
    snd_flush_sound_commands();
    return skipped;
}
#endif /* NON_MATCHING */
