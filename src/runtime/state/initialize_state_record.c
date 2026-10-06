#include "types.h"

typedef struct StateRecord {
    s32 value;
    s32 state;
    s32 initial_value;
    s32 limit;
} StateRecord;

void InitializeStateRecord(StateRecord *record, s32 value, s32 state) {
    record->value = value;
    record->initial_value = value;
    record->state = state;
    record->limit = value;
}
