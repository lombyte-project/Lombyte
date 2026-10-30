#ifndef LOMBYTE_RNC_GAME_GAME_GLOBALS_H
#define LOMBYTE_RNC_GAME_GAME_GLOBALS_H

#include "types.h"

extern s32           ship_substate[] __asm__("D_0013E050");
extern s16           ship_index[] __asm__("D_0013E056");
extern char          item_usage[] __asm__("D_00141680");
extern unsigned char visited52[] __asm__("D_0014C050");
extern s16           cd_read_active[] __asm__("D_001516D8");
extern s32           hero_max_health __asm__("D_0015EDA0");
extern s32          *level_root __asm__("D_0015EE4C");
extern s32   freeze_stats_time_ref[] __asm__("D_0015EE58") __attribute__((section(".sdata")));
extern s32   freeze_stats_score_ref_a[] __asm__("D_0015EE68") __attribute__((section(".sdata")));
extern s32   intro_overlay_alpha __asm__("D_0015EF50");
extern s32   language_intro_overlay_alpha __asm__("D_0015EF54");
extern s32   game_frame_counter __asm__("D_0015F438");
extern float sequence __asm__("D_0015F43C");
extern s32   font_color_codes_enabled __asm__("D_0015F49C");
extern s32   space_load_pending __asm__("D_0015F5B0");
extern s32   space_load __asm__("D_0015F600");
extern s32   world_update_time __asm__("D_0015F60C");
extern s32   space_load_in_progress __asm__("D_0015F618");
extern s32   moby_anim_head __asm__("D_0015FF14");
extern u8   *resident_object_pool __asm__("D_0015FF18");
extern s32   active_decode_buffer __asm__("D_0015FF50");
extern s32   moby_occl_cluster_count __asm__("D_0016119C");
extern s32   moby_render_slots[16] __asm__("D_001B2BC0");
extern u8    moby_backup_buffer[] __asm__("D_001B2E80");

#endif /* LOMBYTE_RNC_GAME_GAME_GLOBALS_H */
