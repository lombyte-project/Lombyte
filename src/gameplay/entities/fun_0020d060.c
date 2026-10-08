#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern struct TagPtr D_0015FF40;
extern s32 D_0015FF3C;
extern u8 D_001C8680[];
extern u8 D_0015FEC0[];
extern void FUN_00227740(void);
extern void submit_graphics_setup_command_stream(void *) __asm__("FUN_00227548");
extern void func_001F21B0(void *, s32);
void FUN_0020d060(void) {
    struct DmaTag *tag;

    if (D_0015FF3C == 0) {
        D_0015FF40.p->tag = 0x10000000;
        D_0015FF40.p->addr = 0;
        D_0015FF40.p->vif0 = 0;
        D_0015FF40.p->vif1 = 0;
        return;
    }
    tag = render_packet_cursor.tag++;
    D_0015FF40.p->tag = 0x20000000;
    D_0015FF40.p->addr = (u32)render_packet_cursor.tag;
    D_0015FF40.p->vif0 = 0;
    D_0015FF40.p->vif1 = 0;
    FUN_00227740();
    submit_graphics_setup_command_stream(D_001C8680);
    render_packet_cursor.tag->tag = 0x20000000;
    render_packet_cursor.tag->addr = (u32)(D_0015FF40.p + 1);
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0;
    render_packet_cursor.tag++;
    tag->tag = 0x20000000;
    tag->addr = (u32)render_packet_cursor.tag;
    tag->vif0 = 0;
    tag->vif1 = 0;
    func_001F21B0(D_0015FEC0, 8);
}

extern __typeof__(FUN_0020d060) func_0020D060 __attribute__((alias("FUN_0020d060")));
