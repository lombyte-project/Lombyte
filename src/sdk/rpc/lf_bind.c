#include "types.h"
#include "sifrpc.h"
extern s32 D_0012FCB0[];
extern u8 D_00158200[];
extern struct sceSifClientData D_00158400;
extern u8 D_00158428[];
extern s32 sceSifBindRpc();
extern s32 sceSifCallRpc();
extern void *memcpy();
s32 _lf_bind(void) {
    s32 i;

    if (D_0012FCB0[0] < 0) {
    loop_2:
        if (sceSifBindRpc(&D_00158400, 0x80000006, 0) < 0) {
            return -1;
        }
        if (D_00158400.serve != 0) {
            D_0012FCB0[0] = 0;
            if (sceSifCallRpc(&D_00158400, 0xFF, 0, 0, 0, D_00158200, 4, 0, 0) < 0) {
                return 0xFFFEFFFF;
            }
            memcpy(D_00158428, D_00158200, 4);
            return 0;
        }
        for (i = 0x100000; i != -1; i--) {
        }
        goto loop_2;
    }
    return 0;
}
