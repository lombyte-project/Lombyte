/* Return the state field at byte offset 0xA8. */

int get_state_field(int *state_fields) __asm__("func_0023CC80");

int get_state_field(int *state_fields) {
    return state_fields[42];
}

/* Recovered original symbol name. */
extern __typeof__(get_state_field) videoDecGetState __attribute__((alias("func_0023CC80")));
