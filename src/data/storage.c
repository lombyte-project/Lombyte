#include "types.h"
#include "sda.h"
#include "rnc/storage/disc_table.h"
#include "rnc/storage/memory_card/memory_card_state.h"

struct DiscTable disc_table DATA_AT(00137B80) = {0};

struct MemoryCardState memory_card_state DATA_AT(0013D290) = {{{0, 0, 0, 0, 0, -3, 0, 0, {{0}, {0}, {0}, {0}, {0}}, 0, 0, 0}}, 0, 0, -1, 0, 0, 1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 1};
