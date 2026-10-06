/* This callback intentionally leaves the state unchanged. */

void NoOpStateCallback(void) __asm__("func_0023B958");

void NoOpStateCallback(void) {
}

/* Recovered original symbol name. */
extern __typeof__(NoOpStateCallback) readBufDelete__FP7ReadBuf
    __attribute__((alias("func_0023B958")));
