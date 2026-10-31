/* Level 18 moby class cutscene_joint_trail (0x707): during the cutscene draws a trail between two joints of the current cutscene moby. */
#include "types.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

extern int D_L18_0015F5C4;

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FC188), where it is exact; names translated to the US level program. */

typedef struct {
    char         pad[0x30];
    unsigned int scene;
    char         pad2[0x178 - 0x34];
    char        *mobys[5];
} Level18CutsceneState;

typedef struct {
    char          pad[0x20];
    unsigned char state;
    char          pad2[0xF];
    unsigned char update_dist;
} CutsceneJointTrailMoby;

extern Level18CutsceneState D_L18_0016D260;

extern short cutscene_joint_trail_velocity_scale_gp __asm__("D_L18_00162578") __attribute__((sda));
extern short cutscene_joint_trail_first_spawn_particle_value_gp __asm__("D_L18_0016257C")
    __attribute__((sda));
extern short cutscene_joint_trail_second_spawn_particle_value_gp __asm__("D_L18_00162580")
    __attribute__((sda));
void cutscene_joint_trail_update(CutsceneJointTrailMoby *moby) __asm__("FUN_L18_002fad28");

/* Moby class 1799 */
void cutscene_joint_trail_update(CutsceneJointTrailMoby *moby) {
    enum {
        INITIALIZE_TRAIL_EFFECT = 0,
        UPDATE_TRAIL_EFFECT = 1
    };
    char  joint_eight_world_position[16];
    char  joint_nine_world_position[16];
    char  trail_velocity[16];
    int   trail_source_moby_index;
    char *trail_source_moby;
    switch (moby->state) {
    case INITIALIZE_TRAIL_EFFECT:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case UPDATE_TRAIL_EFFECT:
        if (D_L18_0015F5C4 == 2) {
            if (D_L18_0016D260.scene < 4) {
                trail_source_moby_index = 0;
                if (D_L18_0016D260.scene == 0 || D_L18_0016D260.scene == D_L18_0015F5C4 ||
                    D_L18_0016D260.scene == 3)
                    trail_source_moby_index = 3;
                if (D_L18_0016D260.scene == 1)
                    trail_source_moby_index = 4;
                trail_source_moby = D_L18_0016D260.mobys[trail_source_moby_index];
                moby_point_position(trail_source_moby, 8, joint_eight_world_position);
                moby_point_position(trail_source_moby, 9, joint_nine_world_position);
                fast_vec_sub(trail_velocity, joint_eight_world_position, joint_nine_world_position);
                fast_vec_normalize(trail_velocity,
                                   trail_velocity,
                                   *(float *)&cutscene_joint_trail_velocity_scale_gp);
                draw_trail(joint_eight_world_position,
                           joint_eight_world_position,
                           trail_velocity,
                           *(float *)&cutscene_joint_trail_first_spawn_particle_value_gp,
                           *(float *)&cutscene_joint_trail_second_spawn_particle_value_gp);
            }
        }
        break;
    }
}
