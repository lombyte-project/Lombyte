int clear_menu_selected_index(int *destination) __asm__("FUN_0021a318");
int clear_menu_selected_index(int *destination) {
    destination[0x11] = -1;
    return 0;
}

extern __typeof__(clear_menu_selected_index) func_0021A318 __attribute__((alias("FUN_0021a318")));
