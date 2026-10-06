typedef int s32;
typedef unsigned short u16;

extern s32 Deci2Call(s32 command, void *request);

s32 SceDeci2ExSend(s32 socket, void *buffer, u16 byte_count) {
    s32 request[3];

    request[0] = socket;
    request[1] = (s32)buffer;
    request[2] = byte_count;

    return Deci2Call(-6, request);
}
