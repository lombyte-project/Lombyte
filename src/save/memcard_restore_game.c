#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/game/level_object_states.h"
#include "rnc/game/level_progress.h"
#include "rnc/game/level_state.h"
#include "rnc/game/options.h"
#include "rnc/game/usage_stats.h"
#include "rnc/globals.h"
#include "rnc/save/mc_data.h"
#include "rnc/save/saved_globals.h"
#include "rnc/ui/help/help_record.h"
#include "rnc/ui/help/message_state.h"
#include "rnc/save/memcard_get_data_size.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/sdk/debug.h"

const char  out_of_date_save_message[] __asm__("D_001E81D0") = "Out of date save!\n";
extern void memcard_restore_data(u8 *, s32, void *) __asm__("func_0020AF20");
void        memcard_restore_game(u8 *p) __asm__("FUN_00209298");

void memcard_restore_game(u8 *save_buffer) {
    s32 size0 = memcard_get_data_size(mc_gamedata);
    s32 size1 = memcard_get_data_size(mc_leveldata);
    s32 saved_game_span_bytes = ((struct MemoryCardSaveSizes *)save_buffer)->main_size;
    s32 i;

    if (saved_game_span_bytes != size0 ||
        ((struct MemoryCardSaveSizes *)save_buffer)->record_size != size1) {
        DebugPrint(out_of_date_save_message);
        return;
    }
    save_buffer += 8;
    memcard_restore_data(save_buffer, 0, mc_gamedata);
    save_buffer += saved_game_span_bytes;
    for (i = 0; i < 20; i++) {
        memcard_restore_data(save_buffer, i, mc_leveldata);
        save_buffer += size1;
    }
}

extern __typeof__(memcard_restore_game) func_00209298 __attribute__((alias("FUN_00209298")));

struct SaveDataBlock mc_gamedata[48] = {
    {&current_level_index, 4, 0, 0},
    {&current_bolt_count, 4, 1, 0},
    {&alternate_animation_sequence, 4, 2, 0},
    {&D_0015EE24, 4, 3, 0},
    {&D_0015EE98, 8, 4, 0},
    {&alternate_item_available, 0x80, 5, 0},
    {&big_head_cheat_enabled, 0xc, 7, 0},
    {&item_unlocked, 0x20, 8, 0},
    {&weapon_ammo_counts, 0x94, 9, 0},
    {&item_available, 0x25, 10, 0},
    {&items_acquired, 0x25, 11, 0},
    {&D_0015EDD0, 0xc, 12, 0},
    {&pause_iop_sprite_lists, 0x20, 13, 0},
    {&hero_max_health, 4, 19, 0},
    {&level_available, 0x14, 14, 0},
    {&D_0013D510, 0x50, 20, 0},
    {&level_object_states, 0x790, 15, 0},
    {&D_00141968, 0x4a0, 16, 0},
    {&usage_stats, 0x120, 17, 0},
    {&D_00141720, 0x128, 18, 0},
    {&D_0015ED8C, 4, 21, 0},
    {&D_0015ED90, 4, 22, 0},
    {&D_0015ED94, 4, 23, 0},
    {&D_0015ED9C, 4, 24, 0},
    {&opt_cam_invert, 4, 25, 0},
    {&invert52, 4, 26, 0},
    {&opt_cam_yaw_rate, 4, 27, 0},
    {&D_0015EE1C, 1, 28, 0},
    {&D_0015EE1D, 1, 29, 0},
    {&item_text_variant, 0x28, 30, 0},
    {&D_0015EEA0, 4, 31, 0},
    {&D_00141660, 0x1c, 32, 0},
    {&subtitles_enabled, 1, 33, 0},
    {&playback_mode, 4, 34, 0},
    {&music_volume, 4, 35, 0},
    {&sound_volume, 4, 36, 0},
    {&selector_available, 0xc, 37, 0},
    {&ammo_bought_stats, 0x94, 1000, 0},
    {&weapon_ammo_pickup_totals, 0x94, 1001, 0},
    {&weapon_ammo_used_counts, 0x94, 1002, 0},
    {&D_0015EEA4, 4, 1003, 0},
    {&D_0015EEA8, 4, 1004, 0},
    {&D_0015EEAC, 4, 1005, 0},
    {&D_0015EE28, 4, 1008, 0},
    {&D_0015EE2C, 4, 1009, 0},
    {&active_message_records, 0x96, 1010, 0},
    {&active_message_record_count, 4, 1011, 0},
    {0},
};

struct SaveDataBlock mc_leveldata[12] = {
    {&level_visit_state, 1, 3001, 0},
    {&map_coverage_pages, 0x800, 3002, 0},
    {&gold_bolts_collected, 4, 3003, 0},
    {&visited52, 0x10, 3004, 0},
    {&D_0014C190_257218, 0x100, 3005, 0},
    {&bolt_source_stats, 0x100, 3006, 0},
    {&item_usage, 8, 3007, 0},
    {&buried_bolt_states, 0x10, 3008, 0},
    {&level_bolts, 4, 4000, 0},
    {&D_0013DF88, 4, 4001, 0},
    {&hero_respawn_counts_by_level, 4, 4002, 0},
    {0},
};
