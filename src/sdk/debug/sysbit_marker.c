extern int SignExtendPackedValue(void *value, int shift) __asm__("SignExtendPackedValue");
extern void SysbitFlush(void *value, int count) __asm__("_sysbitFlush");

int SysbitMarker(void *value) __asm__("_sysbitMarker");

int SysbitMarker(void *value) {
    int result = SignExtendPackedValue(value, 1);
    SysbitFlush(value, 1);
    return result;
}
