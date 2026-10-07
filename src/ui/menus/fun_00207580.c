/* FUN_00207508's twin with other thresholds. The function is 116 bytes:
   its last three words (`move v0,0; jr ra; nop`) were split off as a
   separate FUN_002075e8, which no caller uses. */
int FUN_00207580(int frame, float unused0, float unused1, float value) {
    if (frame < 0xE0) {
        return (value >= 42.0f && value <= 43.0f) ? 1 : 0;
    }
    return (value >= 39.0f) ? 1 : 0;
}

extern __typeof__(FUN_00207580) func_00207580 __attribute__((alias("FUN_00207580")));
