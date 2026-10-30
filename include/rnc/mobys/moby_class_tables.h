#ifndef LOMBYTE_RNC_MOBYS_MOBY_CLASS_TABLES_H
#define LOMBYTE_RNC_MOBYS_MOBY_CLASS_TABLES_H

#include "types.h"

/* Class data of the loaded classes, one pointer per class slot. register_moby_class
   hands out slots with a counter (D_0015FF00) and records them in three tables that
   follow each other: this one (4 bytes per slot, D_001B3200), D_001B3580 (s32) and
   D_001B3900 (s16); the slot count is the 0xE0 their sizes agree on. The pointee type
   differs per user (class record, streamed resource, preview resource): cast at use. */
extern void *moby_class_resources[224] __asm__("D_001B3200");

/* Slot -> class id, the reverse of resident_class_slot_by_id. */
extern s16 resident_class_id_by_slot[224] __asm__("D_001B3900");

/* Class id -> slot in the table above; initialize_level_runtime and transition_load_wad
   fill all 0x800 bytes with 0xFF before the classes register. */
extern u8 resident_class_slot_by_id[0x800] __asm__("D_001B3AC0");

/* One per moby class a level program defines (its lvl.vtbl section; the
   executable's own list is empty): assign_moby_class_data looks a class up and
   stores its update function and method table. A -1 class ends the list and
   holds the defaults. */
struct MobyClassUpdate {
    s32    oclass;
    void  *update;
    void **methods;
};

#endif /* LOMBYTE_RNC_MOBYS_MOBY_CLASS_TABLES_H */
