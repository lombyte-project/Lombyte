/*
 * A version of malloc/free/realloc written by Doug Lea and released to the
 * public domain (dlmalloc 2.6.5), adapted by newlib. See
 * THIRD_PARTY_NOTICES.md.
 */

/* Source: newlib / dlmalloc. */

/* _free_r f1: newlib mallocr.c free with the game's arena/globals. */

#include "types.h"

struct _reent;

extern void func_001154C0(struct _reent *);
extern void func_001154C8(struct _reent *);
extern s32 _malloc_trim_r(struct _reent *ptr, u32 pad);

extern u32 D_0012F788[]; /* __malloc_av_ bins */
extern u64 D_0012FB90[]; /* trim_threshold */
extern u64 D_0012FB98[]; /* top_pad */

struct malloc_chunk {
    u32 prev_size;
    u32 size;
    struct malloc_chunk *fd;
    struct malloc_chunk *bk;
};

#define SIZE_SZ               4
#define MINSIZE               16
#define PREV_INUSE            1
#define SIZE_BITS             (PREV_INUSE | 0x2)
#define MAX_SMALLBIN_SIZE     512
#define BINBLOCKWIDTH         4
#define chunksize(p)          ((p)->size & ~(SIZE_BITS))
#define set_head(p, s)        ((p)->size = (s))
#define set_head_size(p, s)   ((p)->size = (((p)->size & PREV_INUSE) | (s)))
#define set_foot(p, s)        (((struct malloc_chunk *)((char *)(p) + (s)))->prev_size = (s))
#define mem2chunk(mem)        ((struct malloc_chunk *)((char *)(mem) - 2 * SIZE_SZ))
#define chunk_at_offset(p, s) ((struct malloc_chunk *)(((char *)(p)) + (s)))
#define inuse_bit_at_offset(p, s)                                                                  \
    (((struct malloc_chunk *)(((char *)(p)) + (s)))->size & PREV_INUSE)
#define smallbin_index(sz) (((unsigned long)(sz)) >> 3)
#define idx2binblock(ix)   ((unsigned long)1 << ((ix) / BINBLOCKWIDTH))
#define bin_index(sz)                                                                              \
    (((((unsigned long)(sz)) >> 9) == 0)      ? (((unsigned long)(sz)) >> 3)                       \
     : ((((unsigned long)(sz)) >> 9) <= 4)    ? 56 + (((unsigned long)(sz)) >> 6)                  \
     : ((((unsigned long)(sz)) >> 9) <= 20)   ? 91 + (((unsigned long)(sz)) >> 9)                  \
     : ((((unsigned long)(sz)) >> 9) <= 84)   ? 110 + (((unsigned long)(sz)) >> 12)                \
     : ((((unsigned long)(sz)) >> 9) <= 340)  ? 119 + (((unsigned long)(sz)) >> 15)                \
     : ((((unsigned long)(sz)) >> 9) <= 1364) ? 124 + (((unsigned long)(sz)) >> 18)                \
                                              : 126)

#define av_               D_0012F788
#define bin_at(i)         ((struct malloc_chunk *)((u8 *)&(av_[2 * (i) + 2]) - 2 * SIZE_SZ))
#define top               (bin_at(0)->fd)
#define last_remainder    (bin_at(1))
#define binblocks         (bin_at(0)->size)
#define mark_binblock(ii) (binblocks |= (u32)idx2binblock(ii))

#define trim_threshold (D_0012FB90[0])
#define top_pad        (D_0012FB98[0])

#define unlink(P, BK, FD)                                                                          \
    {                                                                                              \
        BK = P->bk;                                                                                \
        FD = P->fd;                                                                                \
        FD->bk = BK;                                                                               \
        BK->fd = FD;                                                                               \
    }

#define link_last_remainder(P)                                                                     \
    {                                                                                              \
        last_remainder->fd = last_remainder->bk = P;                                               \
        P->fd = P->bk = last_remainder;                                                            \
    }

#define frontlink(P, S, IDX, BK, FD)                                                               \
    {                                                                                              \
        if (S < MAX_SMALLBIN_SIZE) {                                                               \
            IDX = smallbin_index(S);                                                               \
            mark_binblock(IDX);                                                                    \
            BK = bin_at(IDX);                                                                      \
            FD = BK->fd;                                                                           \
            P->bk = BK;                                                                            \
            P->fd = FD;                                                                            \
            FD->bk = BK->fd = P;                                                                   \
        } else {                                                                                   \
            IDX = bin_index(S);                                                                    \
            BK = bin_at(IDX);                                                                      \
            FD = BK->fd;                                                                           \
            if (FD == BK)                                                                          \
                mark_binblock(IDX);                                                                \
            else {                                                                                 \
                while (FD != BK && S < chunksize(FD))                                              \
                    FD = FD->fd;                                                                   \
                BK = FD->bk;                                                                       \
            }                                                                                      \
            P->bk = BK;                                                                            \
            P->fd = FD;                                                                            \
            FD->bk = BK->fd = P;                                                                   \
        }                                                                                          \
    }

void _free_r(struct _reent *ptr, void *mem) {
    struct malloc_chunk *p;
    u32 hd;
    u32 sz;
    s32 idx;
    struct malloc_chunk *next;
    u32 nextsz;
    u32 prevsz;
    struct malloc_chunk *bck;
    struct malloc_chunk *fwd;
    s32 islr;

    if (mem == 0)
        return;

    func_001154C0(ptr);

    p = mem2chunk(mem);
    hd = p->size;

    sz = hd & ~PREV_INUSE;
    next = chunk_at_offset(p, sz);
    nextsz = chunksize(next);

    if (next == top) {
        sz += nextsz;

        if (!(hd & PREV_INUSE)) {
            prevsz = p->prev_size;
            p = chunk_at_offset(p, -prevsz);
            sz += prevsz;
            unlink(p, bck, fwd);
        }

        set_head(p, sz | PREV_INUSE);
        top = p;
        if ((unsigned long)(sz) >= (unsigned long)trim_threshold)
            _malloc_trim_r(ptr, top_pad);
        func_001154C8(ptr);
        return;
    }

    set_head(next, nextsz);

    islr = 0;

    if (!(hd & PREV_INUSE)) {
        prevsz = p->prev_size;
        p = chunk_at_offset(p, -prevsz);
        sz += prevsz;

        if (p->fd == last_remainder)
            islr = 1;
        else
            unlink(p, bck, fwd);
    }

    if (!(inuse_bit_at_offset(next, nextsz))) {
        sz += nextsz;

        if (!islr && next->fd == last_remainder) {
            islr = 1;
            link_last_remainder(p);
        } else
            unlink(next, bck, fwd);
    }

    set_head(p, sz | PREV_INUSE);
    set_foot(p, sz);
    if (!islr)
        frontlink(p, sz, idx, bck, fwd);

    func_001154C8(ptr);
}
