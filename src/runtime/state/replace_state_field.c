/* Replace the state field and return its previous value. */

int ReplaceStateField(int *state_fields, int replacement_value) __asm__("func_0023CC88");

int ReplaceStateField(int *state_fields, int replacement_value) {
    int previous_value = state_fields[42];
    state_fields[42] = replacement_value;
    return previous_value;
}
