#include "types.h"
#include "rnc/storage/disc_table.h"

struct DiscTable disc_table __attribute__((section(".data"))) = {0};
