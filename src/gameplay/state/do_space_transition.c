#include "rnc/gameplay/state/do_space_transition.h"
#include "sda.h"

extern struct SaveSlotTable D_0013D290;
extern struct Globals_0013DD40 D_0013DD40;
extern u8 D_0013DD58[];
extern struct Globals_0013E030 D_0013E030;
extern struct MusicStreamState D_001516D0;
extern s32 D_0015ED5C MACRO_ADDR;
extern s32 D_0015ED84 MACRO_ADDR;
extern s32 D_0015ED88;
extern s16 D_0015EE48 MACRO_ADDR;
extern s16 D_0015EE4A MACRO_ADDR;
extern s32 D_0015F438 MACRO_ADDR;
extern s32 D_0015F600 MACRO_ADDR;
extern s32 D_0015F604;
extern s32 D_0015F618 MACRO_ADDR;
extern struct Globals_0015F634 *D_0015F634;
extern struct Globals_0018CD00 D_0018CD00;
extern struct Globals_00194100 D_00194100;
extern u8 D_001E8988[];

extern void DebugPrint();
extern void FlushCache(s32);
extern void PackDmaTag(u64, u64, u64);
extern void snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void snd_resolve_bank_xrefs(void) __asm__("func_0012E1A8");
extern void snd_unload_bank(s32) __asm__("FUN_0012e1d8");
extern void snd_reset_state_and_flush_commands(void) __asm__("func_0012EB00");
extern s32 snd_stream_safe_cd_sync(s32) __asm__("func_0012EE08");
extern s32 snd_stream_safe_cd_callback(s32) __asm__("FUN_0012ef28");
extern void snd_set_reverb_ex(s32, s32, s32, s32, s32) __asm__("func_0012EF68");
extern s32 read_file_entry_with_retry(s32) __asm__("func_0012F368");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void put_draw_buffer_large(void) __asm__("func_001FB2D0");
extern void put_draw_buffer_small(void) __asm__("func_001FB3D0");
extern void append_palette_transfer_packet(void) __asm__("func_001FB6E0");
extern void load_level_chunk_from_disc(void) __asm__("func_002043B0");
extern s32 service_level_archive_load(void) __asm__("func_00204428");
extern void run_state_handler(void) __asm__("func_00208840");
extern void memcard_update_state(void) __asm__("func_002093D8");
extern void music_stop(void) __asm__("func_00215EE8");
extern void update_primary_pad_state(void) __asm__("func_00217A10");
extern void FUN_00226e08(void) __asm__("FUN_00226e08");
extern void sound_stop_all_sounds(void) __asm__("func_0022DCD0");
extern void update_resident_gameplay_state(void) __asm__("func_0022F778");
extern void dispatch_game_state_update(void) __asm__("func_00230EE8");
extern void initialize_level_runtime(void) __asm__("func_00230F60");
extern void play_level_transition_movie(s32) __asm__("func_00231608");
extern void play_level_loading_slides(s32, s32, s32, s32, s32) __asm__("FUN_00231bd8");
extern void swap_render_buffer_chain(void) __asm__("func_00233630");
extern void vu1_send_chain(void) __asm__("func_002336A0");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern void dmac_vif1_disable(void) __asm__("func_00233D90");
extern s32 sceCdSync(s32);
extern s32 sceGsSyncV(s32);

void do_space_transition(void) __asm__("FUN_00231ff0");

void do_space_transition(void) {
    s32 lvl;
    s32 ok;
    s32 done;

    lvl = D_0015ED88 - 1;
    if (lvl < 0) {
        lvl = 0;
    }
    D_00194100.unk10 |= 0x80000000;
    D_0015F604 = 6;
    D_0013E030.unk26 = 0;
    if (D_0013DD40.unk8 != 0 || D_0015F600 >= 8) {
        D_0013E030.unk26 = 1;
    }
    if (D_0013DD40.unkE != 0 || D_0015F600 >= 14) {
        D_0013E030.unk26 = 2;
    }
    FlushCache(0);
    snd_set_reverb_ex(2, 0, 0, 0, 0);
    snd_reset_state_and_flush_commands();
    snd_flush_sound_commands();
    sound_stop_all_sounds();
    music_stop();
    D_001516D0.updates_suspended = 1;
    if (D_0015F634 != 0) {
        snd_unload_bank(D_0015F634->unk1C);
        snd_resolve_bank_xrefs();
        DebugPrint(D_001E8988, D_0015F634->unk1C);
    }
    D_0015ED5C = 0;
    snd_stream_safe_cd_callback(0);
    snd_stream_safe_cd_sync(0);
    D_0018CD00.unk238 = 16;
    D_0018CD00.unk21C = 524288.0f;
    D_0018CD00.unk228 = 255.0f;
    D_0018CD00.unk230 = 0;
    D_0018CD00.unk234 = 0;
    D_0018CD00.unk218 = 0;
    D_0018CD00.unk22C = 128.0f;
    PackDmaTag(0, 0, 0);
    if (D_0015F600 < 0) {
        while (D_0013D290.unkD4 >= 3 || D_0013D290.unkDC >= 0) {
            memcard_update_state();
            run_state_handler();
        }
        fade_to_black(scale_game_frames(6));
        D_0015ED84 = D_0015F600;
        load_level_chunk_from_disc();
        sceCdSync(0);
        D_001516D0.updates_suspended = 0;
        dmac_vif1_disable();
        return;
    }
    if (D_0015F600 == 0 && D_0013DD58[0] == 0) {
        fade_to_black(scale_game_frames(6));
        play_level_loading_slides(lvl, 0, 1, scale_game_frames(240), 0);
        play_level_transition_movie(0);
        play_level_loading_slides(lvl, 2, 2, scale_game_frames(180), 0);
        play_level_transition_movie(1);
        D_0015ED84 = D_0015F600;
        play_level_loading_slides(lvl, 3, 4, scale_game_frames(240), 1);
        play_level_transition_movie(2);
    } else if (D_0015ED84 == 0 && D_0015F600 == 1 && D_0013DD58[1] == 0) {
        fade_to_black(scale_game_frames(6));
        play_level_loading_slides(lvl, 5, 6, scale_game_frames(240), 0);
        play_level_transition_movie(3);
        play_level_transition_movie(4);
        play_level_loading_slides(lvl, 7, 7, scale_game_frames(180), 0);
        play_level_transition_movie(5);
        D_0013DD58[D_0015ED84] = 2;
        D_0015ED84 = D_0015F600;
        play_level_loading_slides(lvl, 8, 8, scale_game_frames(240), 1);
    } else {
        if (D_0015F600 == 4 && D_0013DD58[4] == 0) {
            fade_to_black(scale_game_frames(12));
            play_level_loading_slides(lvl, 9, 10, scale_game_frames(240), 0);
            play_level_transition_movie(6);
        }
        if (D_0015ED84 == 7 && D_0013DD58[7] != 2 && D_0013DD40.unk8 != 0) {
            fade_to_black(scale_game_frames(12));
            play_level_loading_slides(lvl, 11, 11, scale_game_frames(240), 0);
            play_level_transition_movie(7);
        }
        if (D_0015F600 == 13 && D_0013DD58[13] == 0) {
            fade_to_black(scale_game_frames(12));
            play_level_loading_slides(lvl, 12, 13, scale_game_frames(240), 0);
            play_level_transition_movie(8);
        }
        if (D_0015ED84 == 14 && D_0013DD58[14] != 2 && D_0013DD40.unkF != 0) {
            fade_to_black(scale_game_frames(12));
            play_level_loading_slides(lvl, 14, 14, scale_game_frames(240), 0);
            play_level_transition_movie(9);
        }
        if (D_0015F600 == 16 && D_0013DD58[16] == 0) {
            fade_to_black(scale_game_frames(12));
            play_level_loading_slides(lvl, 15, 16, scale_game_frames(240), 0);
            play_level_transition_movie(10);
        }
        if ((u32)D_0015ED84 < 19) {
            ok = 1;
            if (D_0015ED84 == 7 && D_0013DD40.unk8 == 0) {
                ok = 0;
            }
            if (D_0015ED84 == 14 && D_0013DD40.unkF == 0) {
                ok = 0;
            }
            if (ok) {
                D_0013DD58[D_0015ED84] = 2;
            }
        }
        D_0015EE4A = 1;
        done = 0;
        D_0015ED84 = D_0015F600;
        D_0015EE48 = 0;
        initialize_level_runtime();
        read_file_entry_with_retry(D_0015ED84);
        while (D_0015F618 == 0) {
            vu1_send_chain();
            swap_render_buffer_chain();
            put_draw_buffer_small();
            append_palette_transfer_packet();
            put_draw_buffer_large();
            update_primary_pad_state();
            update_resident_gameplay_state();
            dispatch_game_state_update();
            memcard_update_state();
            run_state_handler();
            vu1_sync_chain(1);
            sceGsSyncV(0);
            D_0015F438++;
            FUN_00226e08();
            if (!done) {
                done = service_level_archive_load();
            }
        }
        if (!done) {
            do {
                FlushCache(0);
                sceGsSyncV(0);
                memcard_update_state();
                run_state_handler();
                FUN_00226e08();
            } while (service_level_archive_load() == 0);
        }
        while (D_0013D290.unkD4 != 2 || D_0013D290.unkDC >= 0) {
            FlushCache(0);
            sceGsSyncV(0);
            memcard_update_state();
            run_state_handler();
            FUN_00226e08();
        }
    }
    sceCdSync(0);
    D_001516D0.updates_suspended = 0;
    dmac_vif1_disable();
}

extern __typeof__(do_space_transition) func_00231FF0 __attribute__((alias("FUN_00231ff0")));
