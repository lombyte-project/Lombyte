/* Return the shared state resource storage. */

extern char GlobalStateResource[0x100] __asm__("D_00152370");

int *get_state_resource(void *state_resource) __asm__("func_001144D8");

int *get_state_resource(void *state_resource) {
    return (int *)GlobalStateResource;
}
