typedef unsigned short u16;

typedef struct RenderState {
    unsigned char reserved_00[0x40];
    u16 command_40;
    unsigned char reserved_42[0x1A];
    u16 command_5C;
    unsigned char reserved_5E[0x1A];
    u16 command_78;
} RenderState;

extern RenderState GlobalRenderState __asm__("D_001516D0") __attribute__((section(".data")));

void music_unpause(void) __asm__("music_unpause");

void music_unpause(void) {
    GlobalRenderState.command_40 = 4;
    GlobalRenderState.command_78 = 4;
    GlobalRenderState.command_5C = 4;
}
