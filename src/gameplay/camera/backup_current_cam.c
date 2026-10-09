#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"
struct CameraBackupState {
    u8 pad_0[0x70];
    s32 unk70;
};

extern u32 D_001870C0[];
extern struct CameraBackupState D_00189210;
extern s32 FUN_001f98d0();
void backup_current_cam(void) __asm__("FUN_001ebc90");

void backup_current_cam(void) {
    FUN_001f98d0(&D_00189210, D_001870C0[0], 0xA0);
    FUN_001f98d0(&camera_saved_states[2], &camera_saved_states[0], 0x280);
    D_00189210.unk70 = (s32)&camera_saved_states[2];
}

struct CameraSavedState camera_saved_states[3] = {0};
