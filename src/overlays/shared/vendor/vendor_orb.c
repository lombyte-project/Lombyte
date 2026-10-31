/* Shared moby class vendor_orb: the spinning orb above a vendor. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/attach_manipulator.h"

void FUN_L00_001fff28(char *, int, float);

float vendor_orb_rotation_rate_degrees_gp __asm__("D_L00_00161BB0") = 45.0f;

float D_L01_00161FF0 = 45.0f;
float D_L02_00161F48 = 45.0f;
float D_L03_00161C68 = 45.0f;
float D_L04_00161D58 = 45.0f;
float D_L05_00161FA8 = 45.0f;
float D_L06_001622B8 = 45.0f;
float D_L07_00161CB0 = 45.0f;
float D_L08_001622D0 = 45.0f;
float D_L09_00161CA0 = 45.0f;
float D_L10_00161DE8 = 45.0f;
float D_L11_00161E98 = 45.0f;
float D_L12_00161DC0 = 45.0f;
float D_L13_00161E18 = 45.0f;
float D_L14_00162080 = 45.0f;
float D_L15_00161F40 = 45.0f;
float D_L16_00161D58 = 45.0f;
float D_L17_00161F20 = 45.0f;
float D_L18_001621F0 = 45.0f;

void vendor_orb_update(unsigned char *p) __asm__("FUN_L00_002df730");

/* Moby class 1143 */
void vendor_orb_update(unsigned char *moby) {
    enum {
        VENDOR_ORB_INITIALIZE = 0,
        VENDOR_ORB_ROTATE = 1
    };
    char *manipulator = ((char *)MOBY(moby)->pvars);
    float rotation_step_radians;
    switch (moby[0x20]) {
    case VENDOR_ORB_INITIALIZE:
        moby[0x20] = 1;
        moby[0x73] = 0x20;
        *(float *)(manipulator + 0x40) = random_angle_radians();
        attach_manipulator(moby, 1, manipulator);
        break;
    case VENDOR_ORB_ROTATE:
        rotation_step_radians = vendor_orb_rotation_rate_degrees_gp * DEG_TO_RAD * frame_time;
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, rotation_step_radians);
        *(float *)(manipulator + 0x40) =
            fast_add_rotations(*(float *)(manipulator + 0x40), rotation_step_radians * -2.0f);
        FUN_L00_001fff28(manipulator + 0x10, 2, *(float *)(manipulator + 0x40));
        break;
    }
}
