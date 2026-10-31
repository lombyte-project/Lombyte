/* Ported from rac1-decomp (src/core/0012AC80.c, func_0012C8B0). */
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
void _initRefImages(unsigned int *frame0_buffer_address,
                    unsigned int *frame1_buffer_address,
                    unsigned int *frame2_buffer_address,
                    unsigned int *first_field0_buffer_address,
                    unsigned int *first_field1_buffer_address,
                    unsigned int *first_field2_buffer_address,
                    unsigned int *second_field0_buffer_address,
                    unsigned int *second_field1_buffer_address,
                    unsigned int *second_field2_buffer_address,
                    void         *addr0,
                    void         *addr1,
                    void         *addr2,
                    int           width,

                    int height) {
    int pixel_count_div_512;

    *frame0_buffer_address = (unsigned int)UncAddr(addr0);
    *frame1_buffer_address = (unsigned int)UncAddr(addr1);
    *frame2_buffer_address = (unsigned int)UncAddr(addr2);
    *first_field0_buffer_address = (unsigned int)UncAddr(addr0);
    *first_field1_buffer_address = (unsigned int)UncAddr(addr1);
    *first_field2_buffer_address = (unsigned int)UncAddr(addr2);

    pixel_count_div_512 = (width * height) / 512;

    *second_field0_buffer_address =
        (unsigned int)UncAddr((char *)addr0 + pixel_count_div_512 * 384);
    *second_field1_buffer_address =
        (unsigned int)UncAddr((char *)addr1 + pixel_count_div_512 * 384);
    *second_field2_buffer_address =
        (unsigned int)UncAddr((char *)addr2 + pixel_count_div_512 * 384);
}
