#include "types.h"
#include "rnc/rendering/level_render_state.h"
#include "rnc/input/pad_state.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/game/game_globals.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/transitions/do_space_transition.h"

typedef struct {
    s32 off;
    s32 size;
} LevelChunk;

typedef struct {
    s32        gfx;
    s32        pad4;
    s32        gfx_alt;
    s32        padC;
    LevelChunk intro[6];
    LevelChunk loading[6];
    s32        code;
} LevelHeader;

typedef struct {
    s32 pad[7];
    s32 bank;
} SoundSlot;

extern s32                boot_level_active __asm__("D_0015F5E8") MACRO_ADDR;
extern u8                 level_bss_start[] __asm__("D_00161280");
extern s32                game_frame_counter_macro __asm__("D_0015F438") MACRO_ADDR;
extern s32                boot_image_buffer __asm__("D_0015EF5C") MACRO_ADDR;
extern s32                decode_mode_macro __asm__("D_0015EED8") MACRO_ADDR;
extern u8                 D_24135F[];
extern s32                boot_intro_movie_ntsc[] __asm__("D_00139378");
extern s32                boot_intro_movie_pal[] __asm__("D_00139380");
extern char               loading_boot_sound_bank_message[] __asm__("D_001E76C0");
extern volatile SoundSlot boot_sound_defs_b[] __asm__("D_001861E0");
extern volatile SoundSlot *volatile level_sound_defs __asm__("D_0015F634");
extern volatile s32 level_sound_def_count __asm__("D_0015F630");

extern void init_once(void) __asm__("func_00201650");
extern void InitializeStreamingState(void);
extern void vu1_init_chain(void) __asm__("func_002335D0");
extern void dmac_vif1_enable(void) __asm__("func_00233D00");
extern void ClearDmaQueueEntry(void);
extern s32  sceGsSyncV(s32);
extern void put_disp_buffer(void) __asm__("func_001FB2A8");
extern void PackDmaTag(s32, u64, u64);
extern void FlushCache(s32);
extern void decompress_wad(s32, s32) __asm__("func_0020B618");
extern void put_draw_buffer_large(void) __asm__("func_001FB2D0");
extern void append_gif_transfer_packet(void) __asm__("func_001FB368");
extern void draw_boot_image(s32) __asm__("func_002012B8");
extern void put_draw_buffer_small(void) __asm__("func_001FB3D0");
extern void append_palette_transfer_packet(void) __asm__("func_001FB6E0");
extern void vu1_send_chain(void) __asm__("func_002336A0");
extern void swap_render_buffer_chain(void) __asm__("func_00233630");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern s32  check_memory_card(void) __asm__("FUN_00209168");
extern void update_primary_pad_state(void) __asm__("func_00217A10");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern void play_mpeg_movie(s32, s32, s32, s32, s32) __asm__("func_0023A3B8");
extern void DebugPrint(char *, ...);
extern s32  load_audio_bank_by_location(s32) __asm__("func_0022D708");
extern void snd_resolve_bank_xrefs(void) __asm__("func_0012E1A8");
extern s32  transition_do_transition(void) __asm__("FUN_001eb798");
extern s32  reset_vif1_gif(void) __asm__("func_001204B8");
extern void sceGsResetGraph(s16, s16, s16, s16);
extern void set_pal_mode(void) __asm__("func_001F34E8");

static inline void clear_bytes(u8 *p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        p[i] = 0;
    }
}

void startlevel(void) __asm__("FUN_001e9658");

extern u8                 level_bss_end[] __asm__("D_00165430");
extern volatile SoundSlot l00_primary_sound_defs[] __asm__("D_00186100");
extern s32                game_mode_macro __asm__("D_0015F604") MACRO_ADDR;
extern s32                pal_mode_macro __asm__("D_0015ED80") MACRO_ADDR;
extern s32                current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;

void startlevel(void) {
    LevelHeader *hdr;
    LevelChunk  *notice_language_chunks;
    s32          n;
    s32          i;
    s32          memory_card_notice;
    s32          previous_memory_card_notice;
    s32          frames;
    s32          code;
    s32          bank;

    boot_level_active = 1;
    init_once();
    InitializeStreamingState();
    for (i = 0; i < level_bss_end - level_bss_start; i++) {
        level_bss_start[i] = 0;
    }
    previous_memory_card_notice = 0;
    frames = 0;
    vu1_init_chain();
    dmac_vif1_enable();
    ClearDmaQueueEntry();
    game_frame_counter_macro = 0;
    sceGsSyncV(0);
    game_mode_macro = 0;
    put_disp_buffer();
    PackDmaTag(0, 0, 0);
    hdr = (LevelHeader *)(((u32)D_24135F & 0xFFFFC000) + 0x2C0000);
    while ((memory_card_notice = check_memory_card()) != 0 &&
           (frames < 11 || controller_state.pressed == 0)) {
        if (memory_card_notice != previous_memory_card_notice) {
            if (memory_card_notice == 1) {
                notice_language_chunks = hdr->intro;
            } else {
                notice_language_chunks = hdr->loading;
            }
            FlushCache(0);
            decompress_wad(notice_language_chunks[game_language].off + (s32)hdr,
                           hdr->code + (s32)hdr);
            FlushCache(0);
            vu1_init_chain();
            PackDmaTag(0, 0, 0);
            put_draw_buffer_large();
            append_gif_transfer_packet();
            draw_boot_image(hdr->code + (s32)hdr);
            put_draw_buffer_small();
            append_palette_transfer_packet();
            vu1_send_chain();
            swap_render_buffer_chain();
            vu1_sync_chain(1);
        }
        sceGsSyncV(0);
        previous_memory_card_notice = memory_card_notice;
        update_primary_pad_state();
        frames++;
    }
    if (previous_memory_card_notice != 0) {
        fade_to_black(10);
    }
    decode_mode_macro = -1;
    code = hdr->code + (s32)hdr;
    boot_image_buffer = code;
    if (pal_mode_macro == 0) {
        play_mpeg_movie(boot_intro_movie_ntsc[0],
                        boot_intro_movie_ntsc[1],
                        (code + 0x3F) & ~0x3F,
                        (code + 0x2C003F) & ~0x3F,
                        0);
    } else {
        play_mpeg_movie(boot_intro_movie_pal[0],
                        boot_intro_movie_pal[1],
                        (code + 0x3F) & ~0x3F,
                        (code + 0x2C003F) & ~0x3F,
                        0);
    }
    decode_mode_macro = 0;
    fade_to_black(scale_game_frames(0x12));
    FlushCache(0);
    if (pal_mode_macro != 0) {
        decompress_wad(hdr->gfx_alt + (s32)hdr, hdr->code + (s32)hdr);
    } else {
        decompress_wad(hdr->gfx + (s32)hdr, hdr->code + (s32)hdr);
    }
    FlushCache(0);
    vu1_init_chain();
    PackDmaTag(0, 0, 0);
    put_draw_buffer_large();
    append_gif_transfer_packet();
    draw_boot_image(hdr->code + (s32)hdr);
    put_draw_buffer_small();
    append_palette_transfer_packet();
    vu1_send_chain();
    swap_render_buffer_chain();
    vu1_sync_chain(1);
    sceGsSyncV(0);
    game_frame_counter_macro++;
    DebugPrint(loading_boot_sound_bank_message);
    bank = load_audio_bank_by_location(disc_table.sound_bank);
    snd_resolve_bank_xrefs();
    /* Publish the slot tables and metadata in retail order. */
    boot_sound_defs_b[4].bank = bank;
    l00_primary_sound_defs[6].bank = bank;
    level_sound_defs = l00_primary_sound_defs;
    level_sound_def_count = 7;
    l00_primary_sound_defs[0].bank = bank;
    l00_primary_sound_defs[1].bank = bank;
    l00_primary_sound_defs[2].bank = bank;
    l00_primary_sound_defs[3].bank = bank;
    l00_primary_sound_defs[4].bank = bank;
    l00_primary_sound_defs[5].bank = bank;
    boot_sound_defs_b[0].bank = bank;
    /* The ordinary view lets the last bank store fill the call delay slot. */
    ((SoundSlot *)boot_sound_defs_b)[2].bank = bank;
    ((SoundSlot *)boot_sound_defs_b)[3].bank = bank;
    boot_sound_defs_b[1].bank = bank;
    transition_do_transition();
    if (pal_mode_macro != requested_video_mode) {
        pal_mode_macro = requested_video_mode;
        reset_vif1_gif();
        sceGsResetGraph(0, 1, pal_mode_macro != 0 ? 3 : 2, 0);
        set_pal_mode();
        put_disp_buffer();
    }
    space_load = current_level_index_macro;
    level_render_state.unk2A = 1;
    current_level_index_macro = -1;
    do_space_transition();
}

extern __typeof__(startlevel) func_001E9658 __attribute__((alias("FUN_001e9658")));
