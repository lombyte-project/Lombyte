/* Clear the two state fields used by the utility object. */

void clear_state_fields(volatile int *state_fields) __asm__("func_0023D1E8");

void clear_state_fields(volatile int *state_fields) {
    state_fields[3] = 0;
    state_fields[2] = 0;
}

/* Recovered original symbol name. */
extern __typeof__(clear_state_fields) voBufReset__FP5VoBuf __attribute__((alias("func_0023D1E8")));
