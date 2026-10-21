#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"

struct Chunk {
    u8 *dst;
    s32 size;
    s32 pad8;
    s32 id;
};

s64 parse_bin(void) __asm__("FUN_0012d8f8");

/* Copies consecutive chunks (dst, size, pad, id + payload) that carry the
   same id (the first chunk sets it) to their destinations, 8 bytes at a time
   when size, source and destination are all 8-aligned, else 4. The chunk
   fetch sits in the loop condition, which is what puts retail's test at the
   bottom with the entry jump. Returns the id of the first chunk that
   differs. */
s64 parse_bin(void) {
    u8 *chunk_cursor;
    struct Chunk *chunk;
    u8 *dst;
    u8 *payload_source_cursor;
    s64 id;

    id = 0;
    chunk_cursor = (u8 *)(level_root[0] + (s32)level_root);
    while (chunk = (struct Chunk *)chunk_cursor, chunk_cursor = (u8 *)(chunk + 1), dst = chunk->dst, payload_source_cursor = chunk_cursor,
           (id == 0) ? (id = chunk->id, 1) : (id == chunk->id)) {
        if (((chunk->size & 7) == 0) && (((u32)chunk_cursor & 7) == 0) && (((u32)dst & 7) == 0)) {
            u8 *d = dst;
            u8 *s = chunk_cursor;
            u8 *e = dst + chunk->size;
            while (d != e) {
                *(u64 *)d = *(u64 *)s;
                d += 8;
                s += 8;
            }
        } else {
            u8 *e = dst + chunk->size;
            while (dst != e) {
                *(u32 *)dst = *(u32 *)payload_source_cursor;
                dst += 4;
                payload_source_cursor += 4;
            }
        }
        chunk_cursor += chunk->size;
    }
    return id;
}

extern __typeof__(parse_bin) func_0012D8F8 __attribute__((alias("FUN_0012d8f8")));
