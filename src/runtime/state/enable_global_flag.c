/* Enable the global state flag. */

extern int GlobalStateFlag __asm__("D_0015F49C");

void enable_global_state_flag(void) __asm__("func_001F61E8");

void enable_global_state_flag(void) {
    GlobalStateFlag = 1;
}
