/* Level 05 moby class pipe_mounted_grating. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/mobys/record_moby_context_bytes.h"
#include "rnc/overlay/shared/mobys/initialize_moby_launch_motion.h"

extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern float scale_time(float) __asm__("FUN_001f96b0");

extern void *find_moby_hit(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern void  random_vector(void *, float, float) __asm__("FUN_L00_00257d78");
extern u8   *FUN_L00_00272060(
      void *pos, s32 life, f32 size0, f32 size1, s32 color, s32 mode, s32 spin, void *vel, f32 f2);
extern char pipe_mounted_grating_command_records[] __asm__("D_L05_001BAAA0");

void         pipe_mounted_grating_update(struct Moby *moby) __asm__("FUN_L05_0030f5c8");
extern float fast_add_rotations(float, float);
extern int   scale_game_frames(int);

/* Moby class 846 */
void pipe_mounted_grating_update(struct Moby *moby) {
    float        piece_initial_motion_step[4];
    float        spin_vel[4];
    float        particle_spawn_offset[4];
    float        drift[4];
    float        pos[4];
    struct Moby *piece;
    float        angle;
    float        life;
    float        speed;
    int          j, spin;
    if (find_moby_hit(moby, 0xB30000, 0) != 0) {
        piece = create_moby(0x3D4);
        if (piece != 0) {
            char *vars;
            piece->spawn_frame = moby->spawn_frame;
            piece->visible = 1;
            vars = (char *)piece->pvars;
            piece->draw_dist = 0x40;
            piece->update_dist = 0x40;
            qcopy(&piece->pos, &moby->pos);
            qcopy(&piece->rot, &moby->rot);
            speed = random_float_between(5.0f, 12.0f);
            fast_vec_scale(piece_initial_motion_step, &moby->rot_row0, -(speed * frame_time));
            clear_u64_value(spin_vel);
            initialize_moby_launch_motion(
                piece, moby, piece_initial_motion_step, spin_vel, scale_game_frames(60));
            *(int *)(vars + 0xC) = 0x200;
        }
        allocate_voice_for_target_entry(0, 0, piece);
        angle = 0.0f;
        {
            int i;
            for (i = 0; i < 6; i++) {
                angle = fast_add_rotations(angle, 1.0471976f);
                piece = create_moby(0x3D5);
                if (piece != 0) {
                    char *vars = (char *)piece->pvars;
                    *(u128 *)spin_vel = 0;
                    *(u128 *)drift = 0;
                    spin_vel[2] = 0.75f;
                    drift[1] = fast_sin(angle) * 1.4f;
                    drift[2] = fast_cos(angle) * 1.4f;
                    *(u128 *)particle_spawn_offset = *(u128 *)drift;
                    transform_vector_by_basis(
                        particle_spawn_offset, particle_spawn_offset, &moby->rot_row0);
                    for (j = 0; j < 10; j++) {
                        float size;
                        random_vector(drift, frame_time * 1.5f, frame_time * 2.0f);
                        fast_vec_scale(pos, drift, 5.0f);
                        fast_vec_add(pos, pos, particle_spawn_offset);
                        fast_vec_add(pos, pos, &moby->pos);
                        spin = random_integer_below(2);
                        if (spin == 0) {
                            spin--;
                        }
                        size = random_float_between(0.4f, 0.5f);
                        FUN_L00_00272060(pos,
                                         scale_game_frames(25),
                                         size * 0.2f,
                                         size,
                                         0x7F207F7F,
                                         0,
                                         spin,
                                         drift,
                                         0.0f);
                        spin = -spin;
                        FUN_L00_00272060(pos,
                                         scale_game_frames(25),
                                         size * 0.14f,
                                         size * 0.7f,
                                         0x7F7F7F7F,
                                         1,
                                         spin,
                                         drift,
                                         0.0f);
                    }
                    piece->spawn_frame = moby->spawn_frame;
                    piece->draw_dist = 0x40;
                    piece->visible = 1;
                    piece->update_dist = 0x40;
                    qcopy(&piece->pos, &moby->pos);
                    qcopy(&piece->rot, &moby->rot);
                    speed = random_float_between(0.0f, 24.0f);
                    life = random_float_between(60.0f, 120.0f);
                    fast_vec_scale(
                        piece_initial_motion_step, &moby->rot_row0, -(speed * frame_time));
                    clear_u64_value(spin_vel);
                    initialize_moby_launch_motion(piece,
                                                  moby,
                                                  piece_initial_motion_step,
                                                  spin_vel,
                                                  truncate_float_to_s32(scale_time(life)));
                    *(int *)(vars + 0xC) = 0x200;
                    piece->rot.x = angle;
                    update_entity(piece);
                }
            }
        }
        moby->command = 1;
        record_moby_context_bytes(&moby->command, 1, moby, 1, pipe_mounted_grating_command_records);
        mark_moby_for_removal(moby);
    } else if (moby->command) {
        mark_moby_for_removal(moby);
    }
}
