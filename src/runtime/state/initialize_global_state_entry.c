#include "types.h"
#include "rnc/data_labels.h"
void InitializeGlobalStateEntry(s32 value) {
    space_load = value;
    space_load_in_progress = space_load_pending = 1;
}
