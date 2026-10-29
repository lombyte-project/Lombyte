/* newlib __errno: errno is the first word of _impure_ptr's struct _reent. */

struct ImpureSlot {
    int *value;
    char padding[0x100];
};

extern struct ImpureSlot _impure_ptr __asm__("D_0012F76C");

int *__errno(void) __asm__("func_001138A8");

int *__errno(void) {
    register int **state_pointer = &_impure_ptr.value;

    return *state_pointer;
}
