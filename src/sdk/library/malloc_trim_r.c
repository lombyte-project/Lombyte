/*
 * A version of malloc/free/realloc written by Doug Lea and released to the
 * public domain (dlmalloc 2.6.5), adapted by newlib. See
 * THIRD_PARTY_NOTICES.md.
 */

/* Source: newlib / dlmalloc. */

/* _malloc_trim_r t2: newlib mallocr.c malloc_trim with array externs (absolute addressing). */

#include "types.h"

struct _reent;

extern void func_001154C0(struct _reent *);
extern void func_001154C8(struct _reent *);
extern void *_sbrk_r(struct _reent *ptr, s32 delta);

extern u32 D_0012F788[]; /* __malloc_av_ bins (pointer array) */
extern u32 D_0012FBA0[]; /* sbrk_base */
extern u32 D_0012FBB8[]; /* current_mallinfo.arena / sbrked_mem */

struct malloc_chunk {
    u32 prev_size;
    u32 size;
    struct malloc_chunk *fd;
    struct malloc_chunk *bk;
};

#define SIZE_SZ                4
#define MINSIZE                16
#define DEFAULT_TRIM_THRESHOLD ((unsigned long)(64 * 1024))
#define malloc_getpagesize     4096
#define MORECORE(x)            _sbrk_r(ptr, (x))
#define MORECORE_FAILURE       ((void *)-1)
#define chunksize(p)           ((p)->size & ~(SIZE_SZ - 1))
#define set_head(p, s)         ((p)->size = (s))
#define check_chunk(p)         ((void)0)

#define av_       D_0012F788
#define bin_at(i) ((struct malloc_chunk *)((u8 *)&(av_[2 * (i) + 2]) - 2 * SIZE_SZ))
#define top       (bin_at(0)->fd)

#define sbrk_base  (D_0012FBA0[0])
#define sbrked_mem (D_0012FBB8[0])

s32 _malloc_trim_r(struct _reent *ptr, u32 pad) {
    s64 top_size;
    s64 extra;
    u8 *current_brk;
    u8 *new_brk;
    u64 pagesz = malloc_getpagesize;

    func_001154C0(ptr);

    top_size = chunksize(top);
    extra = ((top_size - pad - MINSIZE + (pagesz - 1)) / pagesz - 1) * pagesz;

    if (extra < (s64)pagesz) {
        func_001154C8(ptr);
        return 0;
    } else {
        current_brk = (u8 *)MORECORE(0);
        if (current_brk != (u8 *)(top) + top_size) {
            func_001154C8(ptr);
            return 0;
        } else {
            new_brk = (u8 *)MORECORE(-extra);
            if (new_brk == (u8 *)MORECORE_FAILURE) {
                current_brk = (u8 *)MORECORE(0);
                top_size = current_brk - (u8 *)(top);
                if (top_size >= (s64)MINSIZE) {
                    sbrked_mem = (u32)(current_brk - (u8 *)sbrk_base);
                    set_head(top, top_size | 1);
                }
                check_chunk(top);
                func_001154C8(ptr);
                return 0;
            } else {
                set_head(top, (top_size - extra) | 1);
                sbrked_mem -= (u32)extra;
                check_chunk(top);
                func_001154C8(ptr);
                return 1;
            }
        }
    }
}
