/* Suck cannon methods that only level classes use (peckbot and robosquawker; sand shark and circular saw enemy): they keep the moby state in step with the suction and the launch. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "eetypes.h"
#include "sda.h"

extern void FUN_0022da68(int, int, char *);

extern char *get_moby_suck_cannon_record(char *) __asm__("FUN_L00_002db890");
extern int   launch_suck_cannon_projectile(char *, int, int, float) __asm__("FUN_L00_002dbc30");

/* runs the moby update and sets its state byte */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F30F0), where it is exact; names translated to the US level program. */

int launch_suck_cannon_projectile_and_sync_moby_state(
    struct Moby *moby,
    int          a,
    int          position_source_moby,
    float        position_z_offset) __asm__("FUN_L01_002f1d18");

int launch_suck_cannon_projectile_and_sync_moby_state(struct Moby *moby,
                                                      int          launch_data_source,
                                                      int          position_source_moby,

                                                      float position_z_offset) {
    char *callback_record = get_moby_suck_cannon_record(moby);
    int   projectile_launch_result = launch_suck_cannon_projectile(
        moby, launch_data_source, position_source_moby, position_z_offset);
    if (projectile_launch_result != 0) {
        moby->state = 7;
    } else if ((unsigned char)((char)moby->state) == 7) {
        moby->state = 1;
    }
    if (callback_record != 0) {
        if (*(short *)(callback_record + 0x68) == 6) {
            FUN_0022da68(2, 0, moby);
        }
    }
    return projectile_launch_result;
}

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

/* Moby state 9 while FUN_L00_002db8f8 reports a hit, back to 1 when it stops. */
int update_moby_suck_cannon_state(struct Moby *a) __asm__("FUN_L02_002d61c0");

int update_moby_suck_cannon_state(struct Moby *moby) {
    int suction_result = process_moby_suck_cannon_suction(moby);
    if (suction_result == 0) {
        if (moby->state == 0x9)
            moby->state = 1;
    } else {
        moby->state = 0x9;
    }
    return suction_result;
}
