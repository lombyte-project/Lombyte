/* Camera: registers a camera update callback. */
#include "types.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/shared/camera/register_camera_update_callback_globals.h"

extern int D_L02_001695F0[] NOT_SDA;

void register_camera_update_callback(int x) __asm__("FUN_L02_001fc008");

void register_camera_update_callback(int x) {
    int i;

    if (camera_update_callback_count < 16) {
        for (i = 0; i < camera_update_callback_count; i++) {
            if (x == D_L02_001695F0[i]) {
                return;
            }
        }
        D_L02_001695F0[camera_update_callback_count++] = x;
    }
}

int D_L03_001690F0 NOT_SDA = 0;
int D_L04_00169170 NOT_SDA = 0;
int D_L05_001693F0 NOT_SDA = 0;
int D_L06_00169770 NOT_SDA = 0;
int D_L07_00169070 NOT_SDA = 0;
int D_L09_00169170 NOT_SDA = 0;
int D_L10_00169470 NOT_SDA = 0;
int D_L11_001699F0 NOT_SDA = 0;
int D_L12_00169470 NOT_SDA = 0;
int D_L13_001692F0 NOT_SDA = 0;
int D_L14_001696F0 NOT_SDA = 0;
int D_L15_001695F0 NOT_SDA = 0;
int D_L16_001693F0 NOT_SDA = 0;
int D_L17_001698F0 NOT_SDA = 0;
int D_L18_001699F0 NOT_SDA = 0;
