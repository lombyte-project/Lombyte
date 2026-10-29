/* newlib srand: seeds _impure_ptr's _rand_next. */

struct ImpureSlot {
    int *value;
    char padding[0x100];
};

extern struct ImpureSlot _impure_ptr __asm__("D_0012F76C");

void srand(int value) __asm__("func_001160C8");

void srand(int value) {
    _impure_ptr.value[22] = value;
}
