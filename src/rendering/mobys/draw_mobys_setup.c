#include "types.h"
#include "rnc/globals.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/data_labels.h"
#include "rnc/ui/text/font_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/vu1_registers.h"

extern u8   moby_draw_vu0_micro_program[] __asm__("D_00100080");
extern u8   moby_texture_label[] __asm__("D_0015FED0");
extern s32  vu1_chain_limit __asm__("D_00160F08");
extern void noop_core_callback_secondary() __asm__("func_001F21B8");
extern void vu0_load_micro_program() __asm__("func_002334D8");
extern void vu1_add_data_ref() __asm__("func_00233830");

void draw_mobys_setup(void) __asm__("FUN_0020d278");

void draw_mobys_setup(void) {
    register s32 current;
    register s32 callbackArg;
    vu1_add_data_ref(D_0010FAA0, D_0010FA90[0]);
    font_state = 6;
    vu0_load_micro_program(moby_draw_vu0_micro_program);
    vu1_add_g_sregister(0x47, 0x5360B);
    current = render_packet_cursor.addr;
    D_0015FF0C = current;
    callbackArg = 1;
    current += 0x10;
    gs_texture_allocation_cursor = gs_texture_allocation_start;
    render_packet_cursor.addr = current;
    noop_core_callback_secondary(moby_texture_label, callbackArg);
    D_0015FF40 = 0;
    vu1_chain_limit = D_0015F63C + 0xFFFF0000;
    moby_anim_head = D_0015F638;
}

extern __typeof__(draw_mobys_setup) func_0020D278 __attribute__((alias("FUN_0020d278")));
