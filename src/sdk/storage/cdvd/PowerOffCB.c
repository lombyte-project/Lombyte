#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/interrupts.h"
#include "rnc/sdk/rpc/cmd.h"

extern u8  D_00120960[];
extern s32 D_001312FC[];
extern s32 DIntr();

s32 PowerOffCB(void) {
    s32 interrupt_state;

    D_001312E4[0] = 1;
    interrupt_state = DIntr();
    sceSifAddCmdHandler(0x80000012, D_00120960, 0);
    if (interrupt_state != 0) {
        EnableInterrupts();
    }
    D_001312E4[0] = 0;
    D_001312FC[0] = 1;
    return 1;
}
