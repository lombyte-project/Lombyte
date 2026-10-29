/* isatty stub: every descriptor is a terminal (__smakebuf asks it). */

int isatty(void) __asm__("func_00118EC0");

int isatty(void) {
    return 1;
}
