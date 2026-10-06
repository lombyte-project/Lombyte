/* The SDK libraries were built without small data: an extern of unknown size
   keeps this flag at an absolute address, as retail addresses it. */
extern int D_001596EC[];
extern void run_global_constructors(void) __asm__("func_0011DC18");

void UpdateRfuDispatchState(void) {
    if (D_001596EC[0] == 0) {
        D_001596EC[0] = 1;
        run_global_constructors();
    }
}
