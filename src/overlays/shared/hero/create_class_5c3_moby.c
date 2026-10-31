/* Spawns a class 0x5C3 moby for the hero's item input. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"

char *create_class_5c3_moby(int   owner,
                            char *initial_position,
                            int   arg,
                            float initial_rotation_z,
                            float initial_rotation_y) __asm__("FUN_L05_002d4168");

char *create_class_5c3_moby(int   source_item_moby,
                            char *initial_position,
                            int   arg,
                            float initial_rotation_z,

                            float initial_rotation_y) {
    char *moby = (char *)create_moby(0x5C3);
    if (moby != 0) {
        char *moby_vars = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        MOBY(moby)->rot.z = initial_rotation_z;
        MOBY(moby)->rot.y = initial_rotation_y;
        moby[0x20] = 0;
        *(int *)moby_vars = source_item_moby;
        qcopy(moby + 0x10, initial_position);
        *(float *)(moby_vars + 0x10) = frame_time * 30.0f * 0.7f;
        *(int *)(moby_vars + 8) = arg;
        *(int *)(moby_vars + 0x14) = 0;
        *(int *)(moby_vars + 0x18) = 0;
        *(short *)(moby_vars + 6) = 0;
        MOBY(moby)->scale *= 2.3f;
        *(short *)(moby_vars + 4) = scale_game_frames(0x12C);
    }
    return moby;
}
