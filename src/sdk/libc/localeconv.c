struct ImpureSlot {
    int *value;
    char padding[0x100];
};

extern struct ImpureSlot _impure_ptr __asm__("D_0012F76C");

int *_localeconv_r(void *state_resource) __asm__("func_001144D8");

int *localeconv(void) __asm__("GetStateResourceWrapper");

int *localeconv(void) {
    return _localeconv_r(_impure_ptr.value);
}
