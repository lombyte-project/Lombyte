typedef int s32;

extern s32 Deci2Call(s32 command, void *request);

s32 Kputs(const char *message) {
    const char *request_message = message;

    return Deci2Call(0x10, (void *)&request_message);
}
