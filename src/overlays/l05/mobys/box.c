/* Level 05 moby class box. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"

extern void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");

extern char *D_L05_0015FFD8_c __asm__("D_L05_0015FFD8");
extern void convert_pose_to_reference_local(char *, char *, char *, char *, char *, char *) __asm__("FUN_L00_00260668");
extern void transform_attachment_pose(char *, char *, char *, char *, char *, char *) __asm__("FUN_L00_002604f0");
extern void FUN_L00_00260738_c(char *, float *, float *, char *) __asm__("FUN_L00_00260738");

void box_update(char *moby) __asm__("FUN_L05_0030bf98");

void box_update(char *moby) {
    enum {
        BOX_INITIALIZE = 0,
        BOX_UPDATE_TRANSFORM = 1
    };
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float previous_position[4];
    float previous_rotation[4];
    float position_delta[4];
    switch (MOBY(moby)->state) {
    case BOX_INITIALIZE:
        *(int *)(moby_data + 0x5C) = 1;
        if (*(int *)(moby_data + 0x80) >= 0) {
            convert_pose_to_reference_local(moby,
                                            D_L05_0015FFD8_c + (*(int *)(moby_data + 0x80) << 8),
                                            moby + 0x10,
                                            moby + 0x40,
                                            moby_data + 0x60,
                                            moby_data + 0x70);
            MOBY(moby)->state = 1;
        }
        break;
    case BOX_UPDATE_TRANSFORM:
        qcopy(previous_position, moby + 0x10);
        qcopy(previous_rotation, moby + 0x40);
        transform_attachment_pose(moby,
                                  D_L05_0015FFD8_c + (*(int *)(moby_data + 0x80) << 8),
                                  moby_data + 0x60,
                                  moby_data + 0x70,
                                  moby + 0x10,
                                  moby + 0x40);
        fast_vec_sub(position_delta, moby + 0x10, previous_position);
        FUN_L00_00260738_c(moby_data + 0x20, position_delta, previous_rotation, moby + 0x40);
        break;
    }
}
