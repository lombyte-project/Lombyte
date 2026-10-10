/* Return the success status used by the adjacent core callback helpers. */

int return_success_code(void) __asm__("func_00118EC0");

int return_success_code(void) {
    return 1;
}
