#ifndef LOMBYTE_RNC_SAVE_MC_DATA_H
#define LOMBYTE_RNC_SAVE_MC_DATA_H

#include "types.h"

/* One block of a save: SIZE bytes at DATA, stored in the file under ID
   (0-37 and 1000-1011 in the main block, 3001-3008 and 4000-4002 in a
   level record).
   memcard_prepare_data packs each block (record SLOT at data + slot * size),
   memcard_restore_data copies it back and sets restore_status (1: sizes
   match, -1: the saved block is shorter, -2: longer). A null DATA ends the
   list; memcard_get_data_size sums the packed size. */
struct SaveDataBlock {
    void *data;
    s32   size;
    s32   id;
    s32   restore_status;
};

/* The main save block list (written once) and the per-level record list
   (written for each of the 20 levels). */
extern struct SaveDataBlock mc_gamedata[48] __asm__("D_001A04C0");
extern struct SaveDataBlock mc_leveldata[12] __asm__("D_001A07C0");

#endif /* LOMBYTE_RNC_SAVE_MC_DATA_H */
