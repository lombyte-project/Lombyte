/* Level 07 moby class 0x372: bomb, the tumbling ballistic bomb the bombomatic throws. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"

extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern int  collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

/* Pvars of a tumbling piece of debris. */
typedef struct {
    Vec4f vel;     /* 0x00 */
    void *ignore;  /* 0x10: collision owner to skip */
    s32   variant; /* 0x14 */
    f32   spin_y;  /* 0x18 */
    f32   spin_z;  /* 0x1C */
} DebrisVars;

extern void         spawn_explosion(void *,
                                    void *,
                                    void *,
                                    float,
                                    float,
                                    int,
                                    int,
                                    int,
                                    float,
                                    float,
                                    float,
                                    float,
                                    int,
                                    float,
                                    int,
                                    int,
                                    int,
                                    int) __asm__("FUN_L00_0025e450");
extern struct Moby *debris_target __asm__("D_L07_00173ED8") NOT_SDA;
extern char         debris_far_point[] __asm__("D_L07_00166E40");

/* Debris update: moves and spins it, bursts it on impact, removes it out of bounds. */
void        bombomatic_bomb_update(struct Moby *moby) __asm__("FUN_L07_0030bbc8");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
s32         allocate_voice_for_target_entry(s32   entry_index,
                                            s32   flags,
                                            void *target) __asm__("FUN_0022da68");

/* Moby class 882 */
void bombomatic_bomb_update(struct Moby *moby) {
    Vec4f      *pos = &moby->pos;
    DebrisVars *debris_vars = (DebrisVars *)moby->pvars;
    Vec4        previous_debris_position;
    float       size;

    qcopy(&previous_debris_position, pos);
    fast_vec_add(pos, pos, debris_vars);
    debris_vars->vel.z -= frame_time_sq * 9.8f;
    moby->rot.y = fast_add_rotations(moby->rot.y, debris_vars->spin_y);
    moby->rot.z = fast_add_rotations(moby->rot.z, debris_vars->spin_z);
    if (collision_line(&previous_debris_position, pos, 0, debris_vars->ignore, NULL) != 0 ||
        coll_sphere(0.5f, pos, 0, debris_vars->ignore) != 0) {
        if (debris_target == NULL || (u16)(debris_target->oclass - 0x371) >= 2) {
            if (moby->visible != 0 || vector_distance_xy(pos, &hero.motion.pos) < 5.0f) {
                size = 0.0f;
                if (debris_vars->variant % 3 == 0) {
                    size = 10.0f;
                }
                if (vector_distance_xy(pos, debris_far_point) > 50.0f) {
                    if (debris_vars->variant & 1) {
                        spawn_explosion(moby,
                                        debris_vars,
                                        0,
                                        2.0f,
                                        1.0f,
                                        1,
                                        2,
                                        3,
                                        0.0f,
                                        0.0f,
                                        1000.0f,
                                        1.3f,
                                        -1,
                                        size,
                                        0,
                                        1,
                                        -1,
                                        0);
                    } else {
                        spawn_explosion(moby,
                                        debris_vars,
                                        0,
                                        2.0f,
                                        1.0f,
                                        0,
                                        2,
                                        3,
                                        0.0f,
                                        0.0f,
                                        1000.0f,
                                        1.2f,
                                        0,
                                        size,
                                        0,
                                        1,
                                        -1,
                                        0);
                    }
                } else {
                    if (debris_vars->variant & 1) {
                        spawn_explosion(moby,
                                        debris_vars,
                                        0,
                                        2.0f,
                                        1.0f,
                                        0,
                                        2,
                                        4,
                                        0.0f,
                                        1.0f,
                                        1000.0f,
                                        1.0f,
                                        0,
                                        size,
                                        0,
                                        1,
                                        -1,
                                        0);
                    } else {
                        spawn_explosion(moby,
                                        debris_vars,
                                        0,
                                        2.0f,
                                        1.0f,
                                        1,
                                        2,
                                        4,
                                        0.0f,
                                        0.0f,
                                        1000.0f,
                                        1.0f,
                                        0,
                                        size,
                                        0,
                                        1,
                                        -1,
                                        0);
                    }
                }
            } else {
                allocate_voice_for_target_entry(0, 0, moby);
            }
            mark_moby_for_removal(moby);
            return;
        }
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
    }
}
