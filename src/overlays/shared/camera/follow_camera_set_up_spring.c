/* Follow camera: sets up its spring, sets its rotation, and stores a value in the camera record while it is idle. */
#include "types.h"
#include "asm.h"

#include "eetypes.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/camera/follow_cam.h"

/* Stores f at +0x1CC of D_L00_00166E00+0x70 while the short at 0x86 is 0. */
void FUN_L00_002e8540(float stored_value) {
    char *camera_record = current_follow_cam;
    if (*(short *)(camera_record + 0x86) == 0) {
        *(float *)(*(char **)(camera_record + 0x70) + 0x1cc) = stored_value;
    }
}

typedef struct {
    char b[0x140];
} Pane;

extern char *D_L00_00166E00_002e8590[] __asm__("D_L00_00166E00");
void         follow_camera_set_up_spring(float a, float b) __asm__("FUN_L00_002e85d8");

void follow_camera_set_up_spring(float up_spring_value, float up_spring_setting) {
    char *camera_record = D_L00_00166E00_002e8590[0];
    if (*(short *)(camera_record + 0x86) == 0) {
        char *camera_data_region = *(char **)(camera_record + 0x70) + 0x40;
        if (up_spring_value != 0.0f)
            *(float *)(camera_data_region + 0xe4) = up_spring_value;
        if (up_spring_setting != 0.0f)
            *(float *)(camera_data_region + 0xe8) = up_spring_setting;
    }
}
/* Copies a vector into the current object's slot, switching to state 5 first. */

extern char *D_L00_00166E00 NOT_SDA;
extern char                *find_camera_record_by_id(int) __asm__("FUN_L00_001eb1f8");

void set_camera_rotation(char *src) __asm__("FUN_L00_002eaa30");
void set_camera_rotation(char *rotation_source) __asm__("FUN_L00_002eaa30");

void set_camera_rotation(char *rotation_source) {
    char *current_camera_record = D_L00_00166E00;
    char *camera_record;
    if (*(short *)(current_camera_record + 0x86) != 5)
        camera_record = find_camera_record_by_id(5);
    else
        camera_record = current_camera_record;
    qcopy(*(char **)(camera_record + 0x70) + 0x90, rotation_source);
}
