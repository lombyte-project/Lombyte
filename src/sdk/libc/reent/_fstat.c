/* newlib's _fstat stub: every descriptor is a character device. _fstat_r
   (_fstat_r) calls it with the stat buffer. */

int _fstat(int file_descriptor, int *stat_buffer) __asm__("func_00118EC8");

int _fstat(int file_descriptor, int *stat_buffer) {
    *(long long *)((char *)stat_buffer + 72) = 0;
    stat_buffer[1] = 8192; /* st_mode = S_IFCHR */
    return 0;
}
