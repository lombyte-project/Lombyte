int enter_timed_menu_sequence(int *destination) __asm__("FUN_002223d8");
int enter_timed_menu_sequence(int *destination) {
    destination[0x10] = 0;
    destination[0x14] = 0;
    destination[0xF] = 0;
    return 0;
}

extern __typeof__(enter_timed_menu_sequence) func_002223D8 __attribute__((alias("FUN_002223d8")));
