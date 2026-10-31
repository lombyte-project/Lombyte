#include "rnc/movies/decoder/read_buf.h"

void read_buf_create(struct ReadBuf *buf) __asm__("func_0023B940");

void read_buf_create(struct ReadBuf *read_buffer_to_initialize) {
    struct ReadBuf *buffer_to_initialize = read_buffer_to_initialize;

    buffer_to_initialize->size = READ_BUF_SIZE;
    buffer_to_initialize->put = 0;
    buffer_to_initialize->count = 0;
}

/* Recovered original symbol name. */
extern __typeof__(read_buf_create) readBufCreate__FP7ReadBuf
    __attribute__((alias("func_0023B940")));
