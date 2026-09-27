/* Ported from rac1-decomp, the PAL decompilation (src/core/0012AC80.c, func_0012C8B0). */
#define UNCMASK 0x0fffffff
#define UNCBASE 0x20000000
static inline void *UncAddr(void *val) {
    return (void *)(((unsigned int)val & UNCMASK) | UNCBASE);
}
/* _initRefImages (libmpeg.a:init.o): hands out each plane's uncached
 * address twice (out0..2, out3..5) and, q*384 bytes on (q = w*h/512),
 * once more (out6..8). The pointer arithmetic is load-bearing: on plain
 * integers gcc merges the three multiplies retail keeps separate. q is
 * computed just before its first use, for retail's register numbering. */
void _initRefImages(unsigned int *out0, unsigned int *out1, unsigned int *out2,
                    unsigned int *out3, unsigned int *out4, unsigned int *out5,
                    unsigned int *out6, unsigned int *out7, unsigned int *out8,
                    void *addr0, void *addr1, void *addr2,
                    int width, int height) {
    int q;

    *out0 = (unsigned int)UncAddr(addr0);
    *out1 = (unsigned int)UncAddr(addr1);
    *out2 = (unsigned int)UncAddr(addr2);
    *out3 = (unsigned int)UncAddr(addr0);
    *out4 = (unsigned int)UncAddr(addr1);
    *out5 = (unsigned int)UncAddr(addr2);

    q = (width * height) / 512;

    *out6 = (unsigned int)UncAddr((char *)addr0 + q * 384);
    *out7 = (unsigned int)UncAddr((char *)addr1 + q * 384);
    *out8 = (unsigned int)UncAddr((char *)addr2 + q * 384);
}
