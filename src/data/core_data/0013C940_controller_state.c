#include "types.h"
#include "rnc/input/pad_state.h"

struct PadState controller_state __attribute__((section(".data"))) = {0};
