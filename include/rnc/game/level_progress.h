#ifndef LOMBYTE_RNC_GAME_LEVEL_PROGRESS_H
#define LOMBYTE_RNC_GAME_LEVEL_PROGRESS_H

#include "types.h"
#include "rnc/game/bolt_sources.h"

/* Per-level progress records, one row per level slot (20; the level save
   file holds 20 records, memcard_make_whole_save). The save table
   mc_leveldata lists each array with its row size. */
#define LEVEL_RECORD_COUNT 20

/* Per-level counters of the main save (mc_gamedata saves 0x50 bytes of each). */
extern s32 D_0013D510[LEVEL_RECORD_COUNT] __asm__("D_0013D510");
/* Best coverage result save_map_coverage got per level (one slot per level record). */
extern s32 max_coverage_save_result_by_level[LEVEL_RECORD_COUNT] __asm__("D_0013D560");
/* Per-level counters the hero code bumps; mc_leveldata saves 4 bytes of each per level. */
extern s32 level_bolts[LEVEL_RECORD_COUNT] __asm__("D_0013DF38"); /* bolts collected per level */
extern s32 D_0013DF88[LEVEL_RECORD_COUNT] __asm__("D_0013DF88");
extern s32 hero_respawn_counts_by_level[LEVEL_RECORD_COUNT] __asm__("D_0013DFD8");

/* Explored map pages: save_map_coverage packs one 0x800-byte page per level. */
#define MAP_COVERAGE_PAGE_SIZE 0x800
extern u8 map_coverage_pages[LEVEL_RECORD_COUNT * MAP_COVERAGE_PAGE_SIZE] __asm__("D_00141EC0");
/* Gold bolts collected, 4 per level, indexed level * 4 + bolt. */
extern u8 gold_bolts_collected[LEVEL_RECORD_COUNT * 4] __asm__("D_0014BEC0");
/* Buried bolt states dug up with the metal detector, 16 bytes per level. */
extern u8 buried_bolt_states[LEVEL_RECORD_COUNT * 16] __asm__("D_0014BF10");
/* Mission state per level slot (0xFF: done), indexed slot + level * 16. */
extern unsigned char visited52[LEVEL_RECORD_COUNT * 16] __asm__("D_0014C050");
/* One bit per moby save id, per level. */
extern s32 D_0014C190_257218[LEVEL_RECORD_COUNT][64] __asm__("D_0014C190");
/* bolt_source_stats (D_0014D590, rnc/game/bolt_sources.h) follows. */
/* Hero respawns per level and damage source category; level code counts them. */
extern s32
    hero_respawn_counts_by_level_and_source_category[LEVEL_RECORD_COUNT][16] __asm__("D_0014E990");
/* Per-mission counters the level loader clears (16 slots). */
extern s32 D_0014EE90[16];

#endif /* LOMBYTE_RNC_GAME_LEVEL_PROGRESS_H */
