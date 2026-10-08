#include "types.h"
#include "sda.h"
#include "rnc/rendering/dma_tag.h"

extern s32 D_0015ED80[] MACRO_ADDR;
extern s32 D_0015EE78[] MACRO_ADDR;
extern s32 D_0015EE78_gp;
extern s32 D_0015F618[] MACRO_ADDR;
extern volatile s32 D_00160EE0[] MACRO_ADDR;
extern s32 func_001204B8();
extern s32 set_pal_mode() __asm__("func_001F34E8");
extern s32 init_dma() __asm__("func_0020B418");
extern s32 vu1_init_chain() __asm__("func_002335D0");
extern s32 dmac_vif1_enable() __asm__("func_00233D00");
extern s32 dmac_vif1_disable() __asm__("func_00233D90");
extern s32 sceDmaReset();
extern s32 sceGsResetGraph();

void reset_graphics(void) __asm__("FUN_001f21c0");

void reset_graphics(void) {
    s32 temp_16_31;

    /* The volatile flag store stays out of the call's delay slot, and the
       do-while keeps the D_0015EE78 load after vu1_init_chain, as retail. */
    D_0015F618[0] = 1;
    D_00160EE0[0] = 0;
    dmac_vif1_disable();
    sceDmaReset(1);
    init_dma();
    sceGsResetGraph(0, 1, (D_0015ED80[0] != 0) ? 3 : 2, 0);
    func_001204B8();
    do {
        vu1_init_chain();
        temp_16_31 = D_0015EE78[0];
    } while (0);
    render_packet_cursor.addr = 0;
    set_pal_mode();
    D_0015EE78_gp = temp_16_31;
    vu1_init_chain();
    dmac_vif1_enable();
}
