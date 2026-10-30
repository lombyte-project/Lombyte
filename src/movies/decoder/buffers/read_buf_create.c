#include "rnc/movies/decoder/read_buf.h"

void read_buf_create(struct ReadBuf *buf) __asm__("func_0023B940");

void read_buf_create(struct ReadBuf *buf) {
    struct ReadBuf *b = buf;

    b->size = READ_BUF_SIZE;
    b->put = 0;
    b->count = 0;
}

/* Recovered original symbol name. */
extern __typeof__(read_buf_create) readBufCreate__FP7ReadBuf
    __attribute__((alias("func_0023B940")));
