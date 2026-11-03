#ifndef LOMBYTE_RNC_GAME_LEVEL_PROGRESS_H
#define LOMBYTE_RNC_GAME_LEVEL_PROGRESS_H

#include "types.h"
#include "rnc/game/bolt_sources.h"

/* Per-level progress records, one row per level slot (20; the level save
   file holds 20 records, memcard_make_whole_save). The save table
   mc_leveldata lists each array with its row size. */
#define LEVEL_RECORD_COUNT 20

/* Explored map pages: save_map_coverage packs one 0x800-byte page per level. */
#define MAP_COVERAGE_PAGE_SIZE 0x800
extern u8 map_coverage_pages[LEVEL_RECORD_COUNT * MAP_COVERAGE_PAGE_SIZE] __asm__("D_00141EC0");
/* Gold bolts collected, 4 per level, indexed level * 4 + bolt. */
extern u8 gold_bolts_collected[LEVEL_RECORD_COUNT * 4] __asm__("D_0014BEC0");
/* Buried bolt states dug up with the metal detector, 16 bytes per level. */
extern u8 D_0014BF10[LEVEL_RECORD_COUNT * 16];
/* Mission state per level slot (0xFF: done), indexed slot + level * 16. */
extern unsigned char visited52[LEVEL_RECORD_COUNT * 16] __asm__("D_0014C050");
/* One bit per moby save id, per level. */
extern s32 D_0014C190_257218[LEVEL_RECORD_COUNT][64] __asm__("D_0014C190");
/* bolt_source_stats (D_0014D590, rnc/game/bolt_sources.h) follows. */
/* Hero respawns per level and damage source category; level code counts them. */
extern s32 hero_respawn_counts_by_level_and_source_category[LEVEL_RECORD_COUNT][16] __asm__("D_0014E990");
/* Per-mission counters the level loader clears (16 slots). */
extern s32 D_0014EE90[16];

#endif /* LOMBYTE_RNC_GAME_LEVEL_PROGRESS_H */
