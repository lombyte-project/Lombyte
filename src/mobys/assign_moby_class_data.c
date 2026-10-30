#include "rnc/mobys/moby_class_tables.h"
#include "types.h"
#include "rnc/data_labels.h"

struct Obj {
    u8     pad0[0x2C];
    void **methods;
};

extern struct MobyClassUpdate resident_class_defaults[] __asm__("D_001E8B80");

void assign_moby_class_data(s32 id) __asm__("FUN_00212d68");

void assign_moby_class_data(s32 id) {
    struct Obj *class_resource;
    s32         i;

    class_resource = moby_class_resources[D_0015FF00];
    for (i = 0; resident_class_defaults[i].oclass != -1 && resident_class_defaults[i].oclass != id;
         i++) {
    }
    D_001B3580[D_0015FF00] = (s32)resident_class_defaults[i].update;
    if (class_resource != 0) {
        ((struct Obj *)moby_class_resources[D_0015FF00])->methods =
            resident_class_defaults[i].methods;
    }
}

extern __typeof__(assign_moby_class_data) func_00212D68 __attribute__((alias("FUN_00212d68")));
