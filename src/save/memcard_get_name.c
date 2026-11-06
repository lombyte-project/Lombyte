#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/game/item_state.h"
#include "rnc/game/level_object_states.h"
#include "rnc/game/level_state.h"

extern char *strncpy(char *, const char *, u32);
void         memcard_get_name(u8 *src) __asm__("FUN_00209030");

void memcard_get_name(u8 *rom_configuration) {
    s32 i;

    if (rom_configuration[0x12] == 'E') {
        save_directory_path[2] = rom_configuration[0x12];
    }
    for (i = 3; i < 7; i++) {
        save_directory_path[i] = rom_configuration[i + 0xD];
    }
    for (i = 8; i < 11; i++) {
        save_directory_path[i] = rom_configuration[i + 0xD];
    }
    for (i = 11; i < 13; i++) {
        save_directory_path[i] = rom_configuration[i + 0xE];
    }
    strncpy(save_directory_pattern, save_directory_path, 13);
    strncpy(save_icon_sys_path, save_directory_path, 13);
    strncpy(save_icon_path, save_directory_path, 13);
    strncpy(save_main_file_path, save_directory_path, 13);
    strncpy(save_main_file_path + 0x14, save_directory_path, 13);
    strncpy(save_slot_path_format, save_directory_path, 13);
}

extern __typeof__(memcard_get_name) func_00209030 __attribute__((alias("FUN_00209030")));

char save_directory_path[] __asm__("D_0013D1D0") = "/BA****-*****RATCHET";
char save_directory_pattern[] __asm__("D_0013D1E8") = "/************RATCHET/*";
char save_icon_sys_path[] __asm__("D_0013D200") = "/************RATCHET/icon.sys";
char save_icon_path[] __asm__("D_0013D220") = "/************RATCHET/static.ico";
char save_main_file_path[] __asm__("D_0013D240") = "/************RATCHET/************RATCHET";
char save_slot_path_format[] __asm__("D_0013D270") = "/************RATCHET/save%d.bin";

struct MemoryCardState memory_card_state = {
    .card = {{.save_index = -3}},
    .active_card = -1,
    .busy = 1,
    .fd = -1,
    .pending_state = -1,
    .pending_card = -1,
    .err_card = -1,
    .card_save_active = 1,
};

u8 alternate_item_available[128] = {0};

u8 item_unlocked[32] = {0};

s32 weapon_ammo_counts[37] = {0};

u8 item_available[37] = {0};

u8 items_acquired[37] = {0};

s32 D_0013D510[LEVEL_RECORD_COUNT] = {0};

s32 max_coverage_save_result_by_level[LEVEL_RECORD_COUNT] = {0};

struct LevelObjectState level_object_states[LEVEL_OBJECT_STATE_COUNT] = {0};

u8 level_available[LEVEL_RECORD_COUNT] = {0};

u8 level_visit_state[LEVEL_RECORD_COUNT] = {0};

s32 ammo_bought_stats[37] __asm__("D_0013DD70") = {0};

s32 weapon_ammo_pickup_totals[37] = {0};

s32 weapon_ammo_used_counts[37] = {0};

s32 level_bolts[LEVEL_RECORD_COUNT] __asm__("D_0013DF38") = {0};

s32 D_0013DF88[LEVEL_RECORD_COUNT] = {0};

s32 hero_respawn_counts_by_level[LEVEL_RECORD_COUNT] = {0};
