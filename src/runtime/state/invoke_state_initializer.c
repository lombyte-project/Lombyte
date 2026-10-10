/* Invoke the state initializer and preserve its result. */

extern int state_initializer(void) __asm__("func_0011C938");

int InvokeStateInitializer(void) __asm__("InvokeStateInitializer");

int InvokeStateInitializer(void) {
    return state_initializer();
}
