/* Return the pointer to the shared state record. */

struct GlobalStatePointer {
    int *value;
    char padding[0x100];
};

extern struct GlobalStatePointer GlobalStatePointer __asm__("D_0012F76C");

int *get_global_state_pointer(void) __asm__("func_001138A8");

int *get_global_state_pointer(void) {
    register int **state_pointer = &GlobalStatePointer.value;

    return *state_pointer;
}
