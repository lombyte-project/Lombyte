/* This callback intentionally leaves the state unchanged. */

void no_op_state_callback(void) __asm__("func_0023B958");

void no_op_state_callback(void) {
}

/* Recovered original symbol name. */
extern __typeof__(no_op_state_callback) readBufDelete__FP7ReadBuf
    __attribute__((alias("func_0023B958")));
