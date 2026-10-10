extern int LookupTable[] __asm__("D_00154F80") __attribute__((section(".data")));

int lookup_index(int index) __asm__("func_0011A458");

int lookup_index(int index) {
    return LookupTable[index];
}
