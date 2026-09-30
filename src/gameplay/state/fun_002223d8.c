int FUN_002223d8(int *p) {
    p[0x10] = 0;
    p[0x14] = 0;
    p[0xF] = 0;
    return 0;
}

extern __typeof__(FUN_002223d8) func_002223D8 __attribute__((alias("FUN_002223d8")));
