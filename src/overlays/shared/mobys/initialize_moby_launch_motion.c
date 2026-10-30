/* Starts a moby's launch motion. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"

typedef struct {
    int   id;
    char  pad0[0x1C];
    float s[3];
} Dat;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_00319B58), where it is exact; names translated to the US level program. */

void initialize_moby_launch_motion(char *moby,
                                   int   unused_source_moby,
                                   void *initial_motion_step,
                                   void *local_rotation_offset,
                                   int   lifetime_frames) __asm__("FUN_L05_00318648");

void initialize_moby_launch_motion(char *moby,
                                   int   unused_source_moby,
                                   void *initial_motion_step,
                                   void *local_rotation_offset,

                                   int lifetime_frames) {
    Dat *moby_vars = ((Dat *)MOBY(moby)->pvars);
    moby[0x20] = 1;
    qcopy((char *)moby_vars + 0x30, initial_motion_step);
    qcopy((char *)moby_vars + 0x10, local_rotation_offset);
    moby_vars->id = scale_game_frames(lifetime_frames);
    moby_vars->s[0] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
    moby_vars->s[1] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
    moby_vars->s[2] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
}
