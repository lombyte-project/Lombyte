/* Starts the indexed disc stream (a movie) in the game language; respawns the hero instead when it has no health, and does nothing while a fall fade is pending. */
#include "types.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/shared/cutscenes/update_cutscene_playback.h"

void hero_respawn() __asm__("FUN_L00_00211250");
/* Picks a disc table entry pair by D_0015ED80 and passes it with the language to FUN_L00_002995d0. */
void start_indexed_disc_stream(s32 i) __asm__("FUN_L02_00298c68");

void start_indexed_disc_stream(s32 i) {
    s32          sector, stream_size;
    struct Hero *hero_ptr;
    if (i < 0)
        return;
    if (pal_mode) {
        sector = disc_table.streams_alt[i].sector;
        stream_size = disc_table.streams_alt[i].size;
    } else {
        sector = disc_table.streams[i].sector;
        stream_size = disc_table.streams[i].size;
    }
    hero_ptr = &hero;
    if (hero_ptr->health.hp == 0) {
        hero_respawn();
    } else if (hero_ptr->fall_fade_pending == 0) {
        prepare_movie_playback(sector, stream_size, game_language);
    }
}
