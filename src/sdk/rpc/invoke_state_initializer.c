/* Invoke the state initializer and preserve its result. */

extern int state_initializer(void) __asm__("func_0011C938");

int invoke_state_initializer(void) __asm__("InvokeStateInitializer");

int invoke_state_initializer(void) {
    return state_initializer();
}
