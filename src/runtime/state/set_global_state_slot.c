/* Store a value in the shared state record's third word. */

struct GlobalStatePointer {
    int *value;
    char padding[0x100];
};

extern struct GlobalStatePointer GlobalStatePointer __asm__("D_0012F76C");

void SetGlobalStateSlot(int value) __asm__("func_001160C8");

void SetGlobalStateSlot(int value) {
    GlobalStatePointer.value[22] = value;
}
