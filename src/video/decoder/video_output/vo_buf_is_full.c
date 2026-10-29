/* Report whether the two state fields contain the same value. */

int vo_buf_is_full(int *state_fields) __asm__("func_0023D1F8");

int vo_buf_is_full(int *state_fields) {
    return state_fields[3] == state_fields[4];
}
