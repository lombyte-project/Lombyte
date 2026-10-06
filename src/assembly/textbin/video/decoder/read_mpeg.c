#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/decoder/read_mpeg/FUN_0023a460.s", FUN_0023a460);
#else
#include "rnc/video_decoder_read_mpeg_types.h"

extern struct M2c_D_0013C940 D_0013C940;
extern struct M2c_D_0013E550 D_0013E550;
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
extern void process_audio_stream() __asm__("func_0023ABA0");
extern void audio_dec_start() __asm__("func_0023ACB8");
extern void audio_dec_reset() __asm__("func_0023AD10");
extern void wait_for_display_vsync() __asm__("func_0023B590");
extern void func_0023B5E0();
extern s32 func_0023B960();
extern void func_0023B990();
extern s32 read_buf_begin_get() __asm__("func_0023B9D8");
extern void func_0023BA20();
extern s32 func_0023BA60();
extern void func_0023CC70();
extern s32 func_0023CC80();
extern s32 video_dec_flush() __asm__("func_0023CD08");
extern s32 video_dec_is_flushed() __asm__("func_0023CDE0");
extern s32 func_0023D1F8();
s32 read_mpeg(s32 arg0, struct M2c_arg1 *arg1, s32 *arg2) __asm__("FUN_0023a460");

s32 read_mpeg(s32 arg0, struct M2c_arg1 *arg1, s32 *arg2) {
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
    struct M2c_D_0013C940 *pad_state;

    var_22_8 = 0;
    var_21_10 = 0;
    var_18_22 = *arg2;
    func_00118A80(0);
    var_19_25 = var_18_22;
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
    var_2_44 = 1;
    if (D_0013C940.unk1A4 != 0) {
        goto block_13;
    }
block_5:
    pad_state = &D_0013C940;
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
    if (pad_state->unk1A4 & 0x800) {
        goto block_13;
    }
block_12:
    temp_4_74 = 0x8000000000FULL;
    var_2_44 = 1;
    if ((*(u64 *)&pad_state->unk1A0 & temp_4_74) != temp_4_74) {
        var_2_44 = 0;
    }
block_13:
    if (var_2_44 == 0) {
        goto block_15;
    }
    var_22_8 = 1;
    func_0023CC70(D_0016120C + 0xD9048);
block_15:
    temp_3_92 = func_0023B960(arg1, &sp0);
    if (var_19_25 <= 0) {
        goto block_18;
    }
    if (temp_3_92 <= 0xFFFF) {
        goto block_18;
    }
    temp_2_99 = func_0023BA60(arg2, sp0, 0x10000, 0);
    var_19_25 -= temp_2_99;
    func_0023B990(arg1, temp_2_99);
block_18:
    process_audio_stream();
    switch_thread();
    temp_2_111 = read_buf_begin_get(arg1, &sp4);
    if (temp_2_111 <= 0) {
        goto block_20;
    }
    temp_2_120 = func_0012ABD0(arg0, sp4, temp_2_111, arg1, arg1->unk50008);
    var_18_22 -= temp_2_120;
    func_0023BA20(arg1, temp_2_120);
block_20:
    process_audio_stream();
    if (var_21_10 != 0) {
        goto loop_25;
    }
    if (func_0023D1F8(D_0016120C + 0xD9168) == 0) {
        goto loop_25;
    }
    if (is_audio_ok() == 0) {
        goto loop_24;
    }
    var_21_10 = 1;
    wait_for_display_vsync(1);
    audio_dec_start(D_0016120C + 0xD9100);
loop_24:
loop_25:
    if (var_18_22 >= 5 && func_0023CC80(arg0) != 3) {
        goto block_2;
    }
    /* Retail reuses arg1 as the status value 3 during the shutdown waits. */
    while (video_dec_flush(arg0) == 0) {
        arg1 = (struct M2c_arg1 *)3;
        process_audio_stream();
        switch_thread();
    }
    arg1 = (struct M2c_arg1 *)3;
    while (video_dec_is_flushed(arg0) == 0 && func_0023CC80(arg0) != (s32)arg1) {
        process_audio_stream();
        switch_thread();
    }
    func_0023B5E0();
    audio_dec_reset(D_0016120C + 0xD9100);
    snd_set_master_volume(5, D_0013E550.unk5C);
    snd_flush_sound_commands();
    return var_22_8;
}
#endif /* NON_MATCHING */
