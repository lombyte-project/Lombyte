/* Store a value in the shared state record's third word. */

struct GlobalStateResource {
    int *value;
    char padding[0x100];
};

extern struct GlobalStateResource GlobalStateResource __asm__("D_0012F76C");

void set_global_state_slot(int value) __asm__("func_001160C8");

void set_global_state_slot(int value) {
    GlobalStateResource.value[22] = value;
}
