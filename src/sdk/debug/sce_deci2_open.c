typedef int s32;
typedef unsigned int u32;
typedef unsigned short u16;

extern s32 Deci2Call(s32 command, void *request);
extern char D_00154910[];

s32 sceDeci2Open(u16 protocol, void *options, void *handler) {
    u32 request[4];

    request[0] = protocol & 0xFFFF;
    request[1] = (u32)options;
    request[2] = (u32)handler;
    request[3] = (u32)D_00154910 | 0x20000000;

    return Deci2Call(1, request);
}
