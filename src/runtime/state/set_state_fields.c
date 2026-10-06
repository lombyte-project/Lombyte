/* Set the two leading state fields and report success. */

int SetStateFields(int *state_fields, int first_field, int second_field) __asm__("func_0023BA48");

int SetStateFields(int *state_fields, int first_field, int second_field) {
    state_fields[1] = first_field;
    state_fields[0] = second_field;
    return 1;
}
