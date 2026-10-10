/* Disable the global state flag. */

extern int GlobalStateFlag __asm__("D_0015F49C");

void disable_global_state_flag(void) __asm__("func_001F61F8");

void disable_global_state_flag(void) {
    GlobalStateFlag = 0;
}
