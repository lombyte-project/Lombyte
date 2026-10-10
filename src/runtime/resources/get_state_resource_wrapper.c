struct GlobalStatePointer {
    int *value;
    char padding[0x100];
};

extern struct GlobalStatePointer GlobalStatePointer __asm__("D_0012F76C");

int *get_state_resource(void *state_resource) __asm__("func_001144D8");

int *get_state_resource_wrapper(void) __asm__("GetStateResourceWrapper");

int *get_state_resource_wrapper(void) {
    return get_state_resource(GlobalStatePointer.value);
}
