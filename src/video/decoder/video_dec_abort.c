/* Set the state field to one and report success. */

int video_dec_abort(int *state_fields) __asm__("func_0023CC70");

int video_dec_abort(int *state_fields) {
    int value = 1;

    do {
        state_fields[42] = value;
    } while (0);
    return value;
}
