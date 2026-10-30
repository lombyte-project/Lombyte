/* Sentrybot projectile (class 0x3A6): the spinning shot the sentrybot fires, its update and its spawner. */
#include "rnc/mobys/moby.h"
#include "types.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"

extern char *spawn_source_attached_fading_effect_particle(
    char *src, int col, int w, int v, float x) __asm__("FUN_L00_0026daa0");
extern char *create_moby(int);
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern void  FUN_001f9a68(void *, void *, float);
extern int   D_L15_00174458[];
extern int   D_L15_0015F5CC_s __asm__("D_L15_0015F5CC");
extern void  make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern int   coll_sphere(void *, int, void *, float) __asm__("FUN_L00_001f0d60");
extern void  play_entity_sound(void *, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void cart_burst(void *, void *, float, float, int) __asm__("FUN_L00_0025f090");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void spawn_moby_point_offset_particle(void *, float, int, float) __asm__("FUN_L07_0029b8d8");

/* Spinning shot: flies along its velocity and blows up when it hits something, nears the hero or times out. */

void update_sentrybot_projectile(unsigned char *moby) __asm__("FUN_L15_002e4380");

void update_sentrybot_projectile(unsigned char *moby) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float next[4];
    char  col[0x30];
    char *hit;
    if (moby_vars == 0) {
        mark_moby_for_removal(moby);
        return;
    }
    if (moby[0x20] != 0)
        return;
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, frame_time * 6.2831855f);
    fast_vec_add(next, moby + 0x10, moby_vars);
    make_hit_descriptor(col, moby, 1, 2.0f, moby_vars);
    *(short *)(col + 0x1A) = MOBY(moby)->oclass;
    if (collision_line(moby + 0x10, next, 0, moby, col) != 0 ||
        coll_sphere(moby + 0x10, 0, moby, 0.3f) != 0 || fast_dec_timer(moby_vars + 0x38) != 0) {
        hit = (char *)D_L15_00174458[0];
        if (hit != 0 && (int)hit != *(int *)(moby_vars + 0x3C)) {
            if (*(short *)(hit + 0xA6) != 0x4E)
                play_entity_sound(moby, 0.5f, moby + 0x10, 1, 2.0f, 0.0f, 0, 1, 0);
            cart_burst(moby, moby + 0x10, 0.5f, 13.0f, -1);
            allocate_voice_for_target_entry(0, 0, moby);
            mark_moby_for_removal(moby);
            return;
        }
    }
    qcopy(moby + 0x10, next);
    if (D_L15_0015F5CC_s % 3 == 0)
        spawn_moby_point_offset_particle(moby, 30000.0f, 0, 0.1f);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002E5970), where it is exact; names translated to the US level program. */

char *spawn_sentrybot_projectile(char  *source_moby,
                                 float *direction_vector,
                                 char  *pos) __asm__("FUN_L15_002e4580");

char *spawn_sentrybot_projectile(char *source_moby, float *direction_vector, char *spawn_position) {
    char *spawned_moby = create_moby(0x3A6);
    if (spawned_moby != 0) {
        char *moby_vars;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        MOBY(spawned_moby)->spawn_frame = ((long)MOBY(source_moby)->spawn_frame);
        MOBY(spawned_moby)->scale = MOBY(spawned_moby)->scale * 20.0f;
        qcopy(spawned_moby + 0x10, spawn_position);
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        FUN_001f9a68(moby_vars, direction_vector, 5.0f);
        *(int *)(moby_vars + 0x38) = scale_game_frames(0xF0);
        *(char **)(moby_vars + 0x3C) = source_moby;
        spawned_moby[0x23] = 0x30;
        MOBY(spawned_moby)->flags |= 0x200;
        MOBY(spawned_moby)->rot.z = fast_arc_tan(direction_vector[0], direction_vector[1]);
        MOBY(spawned_moby)->rot.y =
            -fast_arc_tan(vector_length_xy(direction_vector), direction_vector[2]);
        update_entity(spawned_moby);
        spawn_source_attached_fading_effect_particle(spawned_moby, 0x600040FF, 0x80, -1, 170000.0f);
    }
    return spawned_moby;
}
