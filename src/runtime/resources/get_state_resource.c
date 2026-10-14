#include "types.h"
/* Return the shared state resource storage. */

int *get_state_resource(void *state_resource) __asm__("func_001144D8");

extern char D_00152370[0x100];
int *get_state_resource(void *state_resource) {
    return (int *)D_00152370;
}
