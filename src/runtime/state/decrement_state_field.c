/* Decrement the state field at byte offset 0x0C when it is positive. */

void DecrementStateFieldIfPositive(volatile int *state_fields) __asm__("func_0023D340");

void DecrementStateFieldIfPositive(volatile int *state_fields) {
    if (state_fields[3] > 0) {
        state_fields[3] = state_fields[3] - 1;
    }
}
