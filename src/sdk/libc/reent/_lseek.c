/* _lseek stub for newlib's _lseek_r: always fails. */

int _lseek(void) __asm__("func_00118E08");

int _lseek(void) {
    return -1;
}
