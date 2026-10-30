typedef int s32;

extern s32 Deci2Call(s32 command, void *request);

s32 SceDeci2Poll(s32 poll_value) {
    s32 request_value = poll_value;

    return Deci2Call(4, &request_value);
}
