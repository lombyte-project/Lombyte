int pause_page_enter_noop(void) __asm__("FUN_002217f8");
int pause_page_enter_noop(void) {
    return 0;
}

extern __typeof__(pause_page_enter_noop) func_002217F8 __attribute__((alias("FUN_002217f8")));
