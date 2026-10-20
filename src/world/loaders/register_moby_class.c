#include "rnc/gameplay/entities/moby_class_tables.h"
#include "types.h"
#include "rnc/data_labels.h"

/* Class data block of a moby class in the level file (not a Moby instance). */
typedef struct MobyClassData {
    u8 pad[0x2C];
    s32 unk2C;
} MobyClassData;
extern s16 D_001B3900[];
extern void assign_moby_class_data(s32) __asm__("FUN_00212d68");
extern void prepare_resident_class_render_data(MobyClassData *, s32, s32,
                                               s32) __asm__("FUN_00203338");
void register_moby_class(MobyClassData *moby, s32 arg1, s32 arg2, s32 oclass) __asm__("FUN_00203640");

void register_moby_class(MobyClassData *moby, s32 indexed_textures, s32 material_map, s32 oclass) {
    s32 class_slot;

    class_slot = D_0015FF00;
    resident_class_slot_by_id[oclass] = D_0015FF00;
    D_001B3900[class_slot] = oclass;
    moby_class_resources[class_slot] = moby;
    if (moby == 0) {
        assign_moby_class_data(oclass);
        D_0015FF00++;
    } else {
        D_001B6180[class_slot] = moby->unk2C;
        assign_moby_class_data(oclass);
        D_0015FF00++;
        prepare_resident_class_render_data(moby, indexed_textures, material_map, oclass);
    }
}

extern __typeof__(register_moby_class) func_00203640 __attribute__((alias("FUN_00203640")));
