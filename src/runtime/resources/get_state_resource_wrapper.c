struct GlobalStatePointer {
    int *value;
    char padding[0x100];
};

extern struct GlobalStatePointer GlobalStatePointer __asm__("D_0012F76C");

int *GetStateResource(void *state_resource) __asm__("func_001144D8");

int *GetStateResourceWrapper(void) __asm__("GetStateResourceWrapper");

int *GetStateResourceWrapper(void) {
    return GetStateResource(GlobalStatePointer.value);
}
