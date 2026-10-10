/* Clear the state field used by the small state helpers. */

void clear_state_field(int *state_fields) __asm__("func_0023CC30");

void clear_state_field(int *state_fields) {
    state_fields[42] = 0;
}

/* Recovered original symbol name. */
extern __typeof__(clear_state_field) videoDecReset__FP8VideoDec
    __attribute__((alias("func_0023CC30")));
