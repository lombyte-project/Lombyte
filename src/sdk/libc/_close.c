/* _close stub for newlib's _close_r: always fails. */

int _close(void) __asm__("func_00118E00");

int _close(void) {
    return -1;
}
