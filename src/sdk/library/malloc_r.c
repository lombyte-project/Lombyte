/* _malloc_r exact recovery: the smallbin branch expresses bck/bin_at(0) from the unsorted-bin
   head (u8*)&D_0012F790 - 8 so the compiler keeps &D_0012F790 in a1 and derives the bins base
   with `addiu a1,a1,-8`, matching retail's register reuse and store order. */

#include "types.h"

struct _reent;
extern void func_001154C0(struct _reent *);
extern void func_001154C8(struct _reent *);
extern void malloc_extend_top(struct _reent *ptr, u32 nb);
extern u32 D_0012F788[];
struct malloc_chunk;
struct malloc_chunk {
    u32 prev_size;
    u32 size;
    struct malloc_chunk *fd;
    struct malloc_chunk *bk;
};
extern struct malloc_chunk D_0012F790;
typedef struct malloc_chunk *mchunkptr;
typedef struct malloc_chunk *mbinptr;
void *_malloc_r(struct _reent *ptr, u32 bytes) {
    mchunkptr victim;
    u32 victim_size;
    s32 idx;
    int new_var;
    mbinptr bin;
    mchunkptr remainder;
    mbinptr new_var2;
    s64 remainder_size;
    s32 remainder_index;
    u8 *new_var3;
    unsigned long block;
    s32 startidx;
    mchunkptr fwd;
    mchunkptr bck;
    mbinptr q;
    u32 nb = (((long)(bytes + (4 + (16 - 1)))) < ((long)(16 + (16 - 1))))
                 ? ((16 + (16 - 1)) & (~(16 - 1)))
                 : ((bytes + (4 + (16 - 1))) & (~(16 - 1)));
    func_001154C0(ptr);
    if (nb < (512 - 8)) {
        idx = ((unsigned long)nb) >> 3;
        q = (mbinptr)(((u8 *)(&D_0012F788[(2 * idx) + 2])) - (2 * 4));
        victim = q->bk;
        if (victim != q) {
            victim_size = victim->size & (~(1 | 0x2));
            {
                bck = victim->bk;
                fwd = victim->fd;
                fwd->bk = bck;
                bck->fd = fwd;
            };
            ((mchunkptr)(((char *)victim) + victim_size))->size |= 1;
            (void)0;
            func_001154C8(ptr);
            return (void *)(((char *)victim) + (2 * 4));
        }
        idx += 2;
    } else {
        idx = ((((unsigned long)nb) >> 9) == 0)
                  ? (((unsigned long)nb) >> 3)
                  : ((((((unsigned long)nb) >> 4) >> 5) <= 4)
                         ? (56 + (((unsigned long)nb) >> 6))
                         : (((((unsigned long)nb) >> 9) <= 20)
                                ? (91 + (((unsigned long)nb) >> 9))
                                : (((((unsigned long)nb) >> 9) <= 84)
                                       ? (110 + (((unsigned long)nb) >> 12))
                                       : (((((unsigned long)nb) >> 9) <= 340)
                                              ? (119 + (((unsigned long)nb) >> 15))
                                              : (((((unsigned long)nb) >> 9) <= 1364)
                                                     ? (124 + (((unsigned long)nb) >> 18))
                                                     : (126))))));
        bin = (new_var2 = (mbinptr)(((u8 *)(&D_0012F788[(2 * idx) + 2])) - (2 * 4)));
        for (victim = bin->bk; victim != bin; victim = victim->bk) {
            victim_size = victim->size & (~(1 | 0x2));
            remainder_size =
                (victim_size < nb) ? (-((long)(nb - victim_size))) : (victim_size - nb);
            if (remainder_size >= ((long)16)) {
                --idx;
                break;
            } else if (remainder_size >= 0) {
                {
                    bck = victim->bk;
                    fwd = victim->fd;
                    fwd->bk = bck;
                    bck->fd = fwd;
                };
                ((mchunkptr)(((char *)victim) + victim_size))->size |= 1;
                (void)0;
                func_001154C8(ptr);
                return (void *)(((char *)victim) + (2 * 4));
            }
        }

        ++idx;
    }
    if ((victim = (&D_0012F790)->fd) != (&D_0012F790)) {
        victim_size = victim->size & (~(1 | 0x2));
        remainder_size = (victim_size < nb) ? (-((long)(nb - victim_size))) : (victim_size - nb);
        if (remainder_size >= ((long)16)) {
            remainder = (mchunkptr)(((char *)victim) + nb);
            victim->size = nb | 1;
            {
                (&D_0012F790)->fd = ((&D_0012F790)->bk = remainder);
                remainder->fd = (remainder->bk = &D_0012F790);
            };
            remainder->size = remainder_size | 1;
            ((mchunkptr)(((char *)remainder) + remainder_size))->prev_size = remainder_size;
            (void)0;
            func_001154C8(ptr);
            return (void *)(((char *)victim) + (2 * 4));
        }
        (&D_0012F790)->fd = ((&D_0012F790)->bk = &D_0012F790);
        if (remainder_size >= 0) {
            ((mchunkptr)(((char *)victim) + victim_size))->size |= 1;
            (void)0;
            func_001154C8(ptr);
            return (void *)(((char *)victim) + (2 * 4));
        }
        {
            if (victim_size < 512) {
                remainder_index = ((unsigned long)victim_size) >> 3;
                new_var3 = ((u8 *)&D_0012F790 - (2 * 4)) + ((2 * remainder_index) * 4);
                ((mbinptr)((u8 *)&D_0012F790 - (2 * 4)))->size |=
                    (u32)(((unsigned long)1) << (remainder_index / 4));
                bck = (mbinptr)new_var3;
                fwd = bck->fd;
                victim->bk = bck;
                victim->fd = fwd;
                fwd->bk = (bck->fd = victim);
            } else {
                remainder_index =
                    ((((unsigned long)victim_size) >> 9) == 0)
                        ? (((unsigned long)victim_size) >> 3)
                        : (((((unsigned long)victim_size) >> 9) <= 4)
                               ? (56 + (((unsigned long)victim_size) >> 6))
                               : (((((unsigned long)victim_size) >> 9) <= 20)
                                      ? (91 + (((unsigned long)victim_size) >> 9))
                                      : (((((unsigned long)victim_size) >> 9) <= 84)
                                             ? (110 + (((unsigned long)victim_size) >> 12))
                                             : (((((unsigned long)victim_size) >> 9) <= 340)
                                                    ? (119 + (((unsigned long)victim_size) >> 15))
                                                    : (((((unsigned long)victim_size) >> 9) <= 1364)
                                                           ? (124 +
                                                              (((unsigned long)victim_size) >> 18))
                                                           : (126))))));
                bck = (mbinptr)(((u8 *)(&D_0012F788[(2 * remainder_index) + 2])) - (2 * 4));
                fwd = bck->fd;
                if (fwd == bck) {
                    ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->size |=
                        (u32)(((unsigned long)1) << (remainder_index / 4));
                } else {
                    while ((fwd != bck) && (victim_size < (fwd->size & (~(1 | 0x2))))) {
                        fwd = fwd->fd;
                    }

                    bck = fwd->bk;
                }
                victim->bk = bck;
                victim->fd = fwd;
                fwd->bk = (bck->fd = victim);
            }
        };
    }
    if ((block = ((unsigned long)1) << (idx / 4)) <=
        ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->size) {
        if ((block & ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->size) == 0) {
            idx = (idx & (~(4 - 1))) + 4;
            block <<= 1;
            while ((block & ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->size) == 0) {
                idx += 4;
                block <<= 1;
            }
        }
        for (;;) {
            startidx = idx;
            q = (bin = (mbinptr)(((u8 *)(&D_0012F788[(2 * idx) + 2])) - (2 * 4)));
            do {
                for (victim = bin->bk; victim != bin; victim = victim->bk) {
                    victim_size = victim->size & (~(1 | 0x2));
                    remainder_size =
                        (victim_size < nb) ? (-((long)(nb - victim_size))) : (victim_size - nb);
                    if (remainder_size >= ((long)16)) {
                        remainder = (mchunkptr)(((char *)victim) + nb);
                        victim->size = nb | 1;
                        {
                            bck = victim->bk;
                            fwd = victim->fd;
                            fwd->bk = bck;
                            bck->fd = fwd;
                        };
                        {
                            (&D_0012F790)->fd = ((&D_0012F790)->bk = remainder);
                            remainder->fd = (remainder->bk = &D_0012F790);
                        };
                        remainder->size = remainder_size | 1;
                        ((mchunkptr)(((char *)remainder) + remainder_size))->prev_size =
                            remainder_size;
                        (void)0;
                        func_001154C8(ptr);
                        return (void *)(((char *)victim) + (2 * 4));
                    } else if (remainder_size >= 0) {
                        ((mchunkptr)(((char *)victim) + victim_size))->size |= 1;
                        {
                            bck = victim->bk;
                            fwd = victim->fd;
                            fwd->bk = bck;
                            bck->fd = fwd;
                        };
                        (void)0;
                        func_001154C8(ptr);
                        return (void *)(((char *)victim) + (2 * 4));
                    }
                }

                bin = (mbinptr)(((char *)bin) + (2 * (sizeof(mbinptr))));
                if (idx < ((512 / 8) - 1)) {
                    bin = (mbinptr)(((char *)bin) + (2 * (sizeof(mbinptr))));
                    ++idx;
                }
            } while (((++idx) & (4 - 1)) != 0);
            do {
                if ((startidx & (4 - 1)) == 0) {
                    ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->size &= ~block;
                    break;
                }
                --startidx;
                q = (mbinptr)(((char *)q) - (2 * (sizeof(mbinptr))));
            } while (q->fd == q);
            if (((block <<= 1) <=
                 ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->size) &&
                (block != 0)) {
                while ((block & ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->size) ==
                       0) {
                    idx += 4;
                    block <<= 1;
                }

            } else {
                break;
            }
        }
    }
    remainder_size =
        ((((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size & (~(1 | 0x2))) < nb)
            ? (-((long)(nb - (((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size &
                              (~(1 | 0x2))))))
            : ((((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size &
                (~(1 | 0x2))) -
               nb);
    if (((((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size & (~(1 | 0x2))) <
         nb) ||
        (remainder_size < ((long)16))) {
        malloc_extend_top(ptr, nb);
        remainder_size =
            ((((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size & (~(1 | 0x2))) <
             nb)
                ? (-((long)(nb -
                            (((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size &
                             (~(1 | 0x2))))))
                : ((((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size &
                    (~(1 | 0x2))) -
                   nb);
        if (((((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size & (~(1 | 0x2))) <
             nb) ||
            (remainder_size < ((long)16))) {
            func_001154C8(ptr);
            return 0;
        }
    }
    victim = ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd;
    new_var = 4;
    victim->size = nb | 1;
    ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * new_var)))->fd =
        (mchunkptr)(((char *)victim) + nb);
    ((mbinptr)(((u8 *)(&D_0012F788[(2 * 0) + 2])) - (2 * 4)))->fd->size = remainder_size | 1;
    (void)0;
    func_001154C8(ptr);
    return (void *)(((char *)victim) + (2 * 4));
}
