extern int peek_bits(void *value, int shift) __asm__("SignExtendPackedValue");
extern void SysbitFlush(void *value, int count) __asm__("_sysbitFlush");

int SysbitMarker(void *value) __asm__("_sysbitMarker");

int SysbitMarker(void *value) {
    int result = peek_bits(value, 1);
    SysbitFlush(value, 1);
    return result;
}
