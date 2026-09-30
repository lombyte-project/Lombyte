#include "types.h"

struct Chunk {
    u8 *dst;
    s32 size;
    s32 pad8;
    s32 id;
};

extern s32 *D_0015EE4C;

s64 parse_bin(void) __asm__("FUN_0012d8f8");

/* Copies consecutive chunks (dst, size, pad, id + payload) that carry the
   same id (the first chunk sets it) to their destinations, 8 bytes at a time
   when size, source and destination are all 8-aligned, else 4. The chunk
   fetch sits in the loop condition, which is what puts retail's test at the
   bottom with the entry jump. Returns the id of the first chunk that
   differs. */
s64 parse_bin(void) {
    u8 *p;
    struct Chunk *c;
    u8 *dst;
    u8 *src;
    s64 id;

    id = 0;
    p = (u8 *)(D_0015EE4C[0] + (s32)D_0015EE4C);
    while (c = (struct Chunk *)p, p = (u8 *)(c + 1), dst = c->dst, src = p,
           (id == 0) ? (id = c->id, 1) : (id == c->id)) {
        if (((c->size & 7) == 0) && (((u32)p & 7) == 0) && (((u32)dst & 7) == 0)) {
            u8 *d = dst;
            u8 *s = p;
            u8 *e = dst + c->size;
            while (d != e) {
                *(u64 *)d = *(u64 *)s;
                d += 8;
                s += 8;
            }
        } else {
            u8 *e = dst + c->size;
            while (dst != e) {
                *(u32 *)dst = *(u32 *)src;
                dst += 4;
                src += 4;
            }
        }
        p += c->size;
    }
    return id;
}

extern __typeof__(parse_bin) func_0012D8F8 __attribute__((alias("FUN_0012d8f8")));
