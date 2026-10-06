/* newlib dtoa.c (libc.a), built by the SDK compiler. Identified by
   rac1-decomp (src/core/00112468.c, func_001124C0). */
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

int quorem(Bigint *b, Bigint *S) {
    int n;
    int borrow, y, z;
    unsigned int carry, q, ys, si, zs;
    unsigned int *bx, *bxe, *sx, *sxe;

    n = S->wds;
    if (b->wds < n)
        return 0;
    sx = S->x;
    sxe = sx + --n;
    bx = b->x;
    bxe = bx + n;
    q = *bxe / (*sxe + 1);
    if (q) {
        borrow = 0;
        carry = 0;
        do {
            si = *sx++;
            ys = (si & 0xffff) * q + carry;
            zs = (si >> 16) * q + (ys >> 16);
            carry = zs >> 16;
            y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
            borrow = y >> 16;
            z = (*bx >> 16) - (zs & 0xffff) + borrow;
            borrow = z >> 16;
            STOREINC(bx, z, y);
        } while (sx <= sxe);
        if (!*bxe) {
            bx = b->x;
            while (--bxe > bx && !*bxe)
                --n;
            b->wds = n;
        }
    }
    if (Dcmp(b, S) >= 0) {
        q++;
        borrow = 0;
        carry = 0;
        bx = b->x;
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
        bx = b->x;
        bxe = bx + n;
        if (!*bxe) {
            while (--bxe > bx && !*bxe)
                --n;
            b->wds = n;
        }
    }
    return q;
}
