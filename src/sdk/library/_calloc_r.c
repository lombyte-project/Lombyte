/*
 * A version of malloc/free/realloc written by Doug Lea and released to the
 * public domain (dlmalloc 2.6.5), adapted by newlib. See
 * THIRD_PARTY_NOTICES.md.
 */

/* Source: newlib / dlmalloc. */

/* _calloc_r q5: exact mallocr.c else-form with p/csz locals. */

#include "types.h"

struct _reent;
extern void *_malloc_r(struct _reent *ptr, u32 size);
extern void *memset(void *dst, s32 value, u32 size);

#define SIZE_SZ 4
#define MALLOC_ZERO(charp, nbytes)                                                                 \
    do {                                                                                           \
        u32 mzsz = (nbytes);                                                                       \
        if (mzsz <= 9 * SIZE_SZ) {                                                                 \
            u32 *mz = (u32 *)(charp);                                                              \
            if (mzsz >= 5 * SIZE_SZ) {                                                             \
                *mz++ = 0;                                                                         \
                *mz++ = 0;                                                                         \
                if (mzsz >= 7 * SIZE_SZ) {                                                         \
                    *mz++ = 0;                                                                     \
                    *mz++ = 0;                                                                     \
                    if (mzsz >= 9 * SIZE_SZ) {                                                     \
                        *mz++ = 0;                                                                 \
                        *mz++ = 0;                                                                 \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
            *mz++ = 0;                                                                             \
            *mz++ = 0;                                                                             \
            *mz = 0;                                                                               \
        } else {                                                                                   \
            memset((charp), 0, mzsz);                                                              \
        }                                                                                          \
    } while (0)

void *_calloc_r(struct _reent *ptr, u32 n, u32 elem_size) {
    u32 sz = n * elem_size;
    void *mem;

    mem = _malloc_r(ptr, sz);
    if (mem == 0) {
        return 0;
    } else {
        u32 *p = (u32 *)((u8 *)mem - SIZE_SZ);
        u32 csz = *p & ~(u32)(SIZE_SZ - 1);
        MALLOC_ZERO(mem, csz - SIZE_SZ);
        return mem;
    }
}
