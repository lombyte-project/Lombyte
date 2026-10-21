/* newlib dtoa.c (libc.a), built by the SDK compiler. Identified by
   rac1-decomp (src/core/00112468.c, quorem). */
/* newlib dtoa.c quorem(b,S): one digit of b/S for dtoa's digit-generation
 * loop -- estimate the digit q from the top limbs, subtract q*S from b
 * (trimming b's leading zero limbs), then bump q by one more if b is
 * still >= S (the estimate can undershoot by 1), subtracting S again. */
extern int Dcmp(void *, void *);

typedef struct Bigint {
    struct Bigint *next;
    int k, maxwds, sign, wds;
    unsigned int x[1];
} Bigint;

#define STOREINC(xc, hi, lo)                                                                       \
    (((unsigned short *)(xc))[1] = (unsigned short)(hi),                                           \
     ((unsigned short *)(xc))[0] = (unsigned short)(lo), (xc)++)

int quorem(Bigint *dividend, Bigint *S) {
    int word_count_or_index;
    int borrow, y, z;
    unsigned int carry, quotient_digit, ys, si, zs;
    unsigned int *bx, *bxe, *sx, *sxe;

    word_count_or_index = S->wds;
    if (dividend->wds < word_count_or_index)
        return 0;
    sx = S->x;
    sxe = sx + --word_count_or_index;
    bx = dividend->x;
    bxe = bx + word_count_or_index;
    quotient_digit = *bxe / (*sxe + 1);
    if (quotient_digit) {
        borrow = 0;
        carry = 0;
        do {
            si = *sx++;
            ys = (si & 0xffff) * quotient_digit + carry;
            zs = (si >> 16) * quotient_digit + (ys >> 16);
            carry = zs >> 16;
            y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
            borrow = y >> 16;
            z = (*bx >> 16) - (zs & 0xffff) + borrow;
            borrow = z >> 16;
            STOREINC(bx, z, y);
        } while (sx <= sxe);
        if (!*bxe) {
            bx = dividend->x;
            while (--bxe > bx && !*bxe)
                --word_count_or_index;
            dividend->wds = word_count_or_index;
        }
    }
    if (Dcmp(dividend, S) >= 0) {
        quotient_digit++;
        borrow = 0;
        carry = 0;
        bx = dividend->x;
        sx = S->x;
        do {
            si = *sx++;
            ys = (si & 0xffff) + carry;
            zs = (si >> 16) + (ys >> 16);
            carry = zs >> 16;
            y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
            borrow = y >> 16;
            z = (*bx >> 16) - (zs & 0xffff) + borrow;
            borrow = z >> 16;
            STOREINC(bx, z, y);
        } while (sx <= sxe);
        bx = dividend->x;
        bxe = bx + word_count_or_index;
        if (!*bxe) {
            while (--bxe > bx && !*bxe)
                --word_count_or_index;
            dividend->wds = word_count_or_index;
        }
    }
    return quotient_digit;
}
