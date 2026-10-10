/* Initialize the object header fields at the original game entry point. */

int initialize_object(int unused, int *object) __asm__("func_00118EC8");

int initialize_object(int unused, int *object) {
    *(long long *)((char *)object + 72) = 0;
    object[1] = 8192;
    return 0;
}
