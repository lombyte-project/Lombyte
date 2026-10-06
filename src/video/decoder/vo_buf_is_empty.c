/* Report whether the state field at byte offset 0x0C is zero. */

int vo_buf_is_empty(int *state_fields) __asm__("func_0023D2C8");

int vo_buf_is_empty(int *state_fields) {
    return state_fields[3] == 0;
}
