#include "types.h"
#include "rnc/globals.h"
#include "rnc/streaming/disc_table.h"
#include "sda.h"
#include "rnc/game/game_globals.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/packets/put_draw_buffer_large.h"
#include "rnc/audio/rpc/snd_hold_command_batches.h"
#include "rnc/ui/hud/banks/load_hud_banks.h"
#include "rnc/world/loaders/parse_space_scene_chunk.h"
#include "rnc/runtime/iop/initialize_sif_rpc.h"
#include "rnc/streaming/stash/init_stash.h"
#include "rnc/ui/hud/banks/hud_heap_reset.h"
#include "rnc/ui/hud/init_hud.h"

typedef struct {
    s32 off;
    s32 size;
} Chunk;

typedef struct {
    char  pad0[0x20];
    Chunk chunk[6];
} WadHeader;

typedef struct {
    s32 start;
    s32 end;
    s32 sound;
    s32 voice;
} SoundCue;

typedef struct {
    char pad0[0x34];
    s32  time;
    s32  pad38;
    s32  f3C;
} TransferState;

extern s32 pal_mode_macro __asm__("D_0015ED80") MACRO_ADDR;
extern s32 D_0015ED88 __attribute__((sda));

#include "rnc/rendering/render_globals.h"

extern s32           intro_overlay_timer_macro __asm__("D_0015EF58") MACRO_ADDR;
extern f32           sequence_macro __asm__("D_0015F43C") MACRO_ADDR;
extern s32           game_frame_counter_macro __asm__("D_0015F438") MACRO_ADDR;
extern s32           game_mode_macro __asm__("D_0015F604") MACRO_ADDR;
extern SoundCue      D_001862B0[];
extern TransferState render_sequence __asm__("D_0018CB20");
extern WadHeader *volatile D_001940C8[];

extern void load_help_message_group(s32 index);
extern s32  rand(void);
extern void sceGsResetGraph(s16 mode, s16 inter, s16 out, s16 ff);
extern s32  sceGsSyncV(s32 mode);
extern s32  reset_vif1_gif() __asm__("FUN_001204b8");
extern s32  snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32  snd_reset_state_and_flush_commands() __asm__("func_0012EB00");
extern void start_level(int level) __asm__("FUN_001e9488");
extern void transition_load_wad(void) __asm__("FUN_001ea830");
extern void update_gameplay_frame(void) __asm__("func_001EB0A8");
extern void transition_default_draw() __asm__("FUN_001eb410");
extern s32  is_voice_active(s32 entry_index, s32 id) __asm__("FUN_001eb740");
extern s32  set_pal_mode() __asm__("func_001F34E8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern void put_draw_buffer_small(void) __asm__("func_001FB3D0");
extern void append_palette_transfer_packet() __asm__("func_001FB6E0");
extern s32  run_state_handler() __asm__("func_00208840");
extern s32  load_and_initialize_level_chunk() __asm__("func_00209370");
extern s32  memcard_update_state() __asm__("func_002093D8");
extern s32  read_disc_sectors_and_wait(u8 *dst, s32 a, s32 b) __asm__("func_00216828");
extern void update_primary_pad_state(void) __asm__("func_00217A10");
extern s32
allocate_voice_for_group_entry(s32 group_entry_index, s32 flags, s32 owner) __asm__("FUN_0022dba0");
extern void vu1_init_chain(void) __asm__("func_002335D0");
extern void swap_render_buffer_chain(void) __asm__("func_00233630");
extern void vu1_send_chain(void) __asm__("func_002336A0");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");

void transition_do_transition(void) __asm__("FUN_001eb798");

void transition_do_transition(void) {
    WadHeader        *hdr;
    u8               *chunk_destination;
    s32               startup_frame_count;
    s32               level;
    s32               wait;
    s32               i;
    s32               old;
    s32               first_size;
    u32               alignment_mask = 0xFFFFFFF0;
    struct DiscTable *chunk_source_table;

    current_level_index = 0;
    *(volatile u32 *)0x10000010 = 0x83;
    *(volatile u32 *)0x10000000 = 0;
    load_and_initialize_level_chunk();
    chunk_source_table = &disc_table;
    transition_load_wad();
    chunk_destination = (u8 *)D_001940C8[0];
    hdr = (WadHeader *)chunk_destination;
    /* The first 0x60 bytes are reserved; six descriptors start at offset 0x20. */
    /* Load chunks 1..5 before chunk 0, advancing each destination to 16-byte alignment. */
    chunk_destination = (u8 *)hdr + 0x60;
    level_root = hdr;
    first_size = read_disc_sectors_and_wait(
        chunk_destination,
        ((volatile struct DiscFile *)&chunk_source_table->wad_chunks[1])->sector,
        ((volatile struct DiscFile *)&chunk_source_table->wad_chunks[1])->size);
    hdr->chunk[1].size = first_size;
    hdr->chunk[1].off = chunk_destination - (u8 *)hdr;
    chunk_destination += ((u32)first_size + 0xF) & alignment_mask;
    hdr->chunk[2].size = read_disc_sectors_and_wait(chunk_destination,
                                                    chunk_source_table->wad_chunks[2].sector,
                                                    chunk_source_table->wad_chunks[2].size);
    hdr->chunk[2].off = chunk_destination - (u8 *)hdr;
    chunk_destination += ((u32)hdr->chunk[2].size + 0xF) & alignment_mask;
    hdr->chunk[3].size = read_disc_sectors_and_wait(chunk_destination,
                                                    chunk_source_table->wad_chunks[3].sector,
                                                    chunk_source_table->wad_chunks[3].size);
    hdr->chunk[3].off = chunk_destination - (u8 *)hdr;
    chunk_destination += ((u32)hdr->chunk[3].size + 0xF) & alignment_mask;
    hdr->chunk[4].size = read_disc_sectors_and_wait(chunk_destination,
                                                    chunk_source_table->wad_chunks[4].sector,
                                                    chunk_source_table->wad_chunks[4].size);
    hdr->chunk[4].off = chunk_destination - (u8 *)hdr;
    chunk_destination += ((u32)hdr->chunk[4].size + 0xF) & alignment_mask;
    hdr->chunk[5].size = read_disc_sectors_and_wait(chunk_destination,
                                                    chunk_source_table->wad_chunks[5].sector,
                                                    chunk_source_table->wad_chunks[5].size);
    hdr->chunk[5].off = chunk_destination - (u8 *)hdr;
    chunk_destination += ((u32)hdr->chunk[5].size + 0xF) & alignment_mask;
    hdr->chunk[0].size = read_disc_sectors_and_wait(chunk_destination,
                                                    chunk_source_table->wad_chunks[0].sector,
                                                    chunk_source_table->wad_chunks[0].size);
    hdr->chunk[0].off = chunk_destination - (u8 *)hdr;
    hud_heap_reset();
    initialize_sif_rpc();
    init_stash();
    init_hud();
    load_hud_banks();
    for (startup_frame_count = *(volatile u32 *)0x10000000 / 0x109;
         startup_frame_count < scale_game_frames(0xB4);
         startup_frame_count++) {
        sceGsSyncV(0);
    }
    snd_reset_state_and_flush_commands();
    while (snd_flush_sound_commands() != 0) {
    }
    wait = 0;
    level = rand() % 4;
    snd_hold_command_batches();
    while (space_load_pending == 0) {
        vu1_send_chain();
        swap_render_buffer_chain();
        put_draw_buffer_small();
        append_palette_transfer_packet();
        put_draw_buffer_large();
        memcard_update_state();
        run_state_handler();
        update_primary_pad_state();
        load_help_message_group(D_0015ED88);
        space_load_in_progress = 0;
        for (i = 0; D_001862B0[i].start != -1; i++) {
            if (D_001862B0[i].end == -1) {
                if (render_sequence.time < D_001862B0[i].start) {
                    D_001862B0[i].voice = -1;
                    continue;
                }
                if (D_001862B0[i].voice != -1) {
                    continue;
                }
            } else if (render_sequence.time < D_001862B0[i].start ||
                       D_001862B0[i].end < render_sequence.time) {
                goto stop;
            } else if (is_voice_active(D_001862B0[i].voice, D_001862B0[i].sound) != 0) {
                continue;
            }
            D_001862B0[i].voice = allocate_voice_for_group_entry(D_001862B0[i].sound, 0, 0);
            continue;
        stop:
            if (is_voice_active(D_001862B0[i].voice, D_001862B0[i].sound) != 0) {
                release_voice_slot(D_001862B0[i].voice);
            }
            D_001862B0[i].voice = -1;
        }
        update_gameplay_frame();
        transition_default_draw();
        if (game_mode_macro != 0) {
            wait = 0;
        } else if (++wait >= scale_game_frames(0x5DC)) {
            start_level(level);
            wait = 0;
            level = (level + 1) % 4;
            sequence_macro = 1.0f;
            render_sequence.time = 0;
            render_sequence.f3C = 0;
            parse_space_scene_chunk(0);
            intro_overlay_alpha = 0;
            language_intro_overlay_alpha = 0;
            intro_overlay_timer_macro = 0;
            vu1_init_chain();
            continue;
        }
        if (requested_video_mode != pal_mode_macro) {
            pal_mode_macro = pal_mode_macro == 0;
            fade_to_black(4);
            reset_vif1_gif();
            sceGsResetGraph(0, 1, pal_mode_macro != 0 ? 3 : 2, 0);
            old = gs_texture_allocation_start;
            sequence_macro = 1.0f;
            set_pal_mode();
            gs_texture_allocation_start = old;
        }
        vu1_sync_chain(1);
        sceGsSyncV(0);
        game_frame_counter_macro++;
    }
    if (pal_mode_macro == 0) {
        *(volatile s32 *)&gs_texture_allocation_base = 0x280000;
    }
    /* Volatile: retail stores the base and loads it again. */
    gs_texture_allocation_cursor = gs_texture_allocation_start =
        *(volatile s32 *)&gs_texture_allocation_base;
}

extern __typeof__(transition_do_transition) func_001EB798 __attribute__((alias("FUN_001eb798")));
