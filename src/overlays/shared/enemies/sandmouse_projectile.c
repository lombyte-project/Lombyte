/* Shared moby class 0x661: the sandmouse projectile (fired by sandmouse and sand_mouse), which turns toward its target, flies until its timer ends or it hits something; its spawner. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/spherical_offset.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030DC70), where it is exact; names translated to the US level program. */

extern char *create_moby(int);

char *spawn_sandmouse_projectile(char *owner,
                                 char *spawn_position,
                                 float target_yaw,
                                 float target_pitch,
                                 float initial_yaw,
                                 float initial_pitch) __asm__("FUN_L01_0030c898");

char *spawn_sandmouse_projectile(char *owner,
                                 char *spawn_position,
                                 float target_yaw,
                                 float target_pitch,
                                 float initial_yaw,

                                 float initial_pitch) {
    OvlQuad spawn_position_copy;
    char   *cp = (char *)&spawn_position_copy;
    char   *spawned_moby;
    spawn_position_copy = *(OvlQuad *)spawn_position;
    spawned_moby = create_moby(0x661);
    if (spawned_moby) {
        char *moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        int   lifetime_frames;
        *(char **)(moby_vars + 0x10) = owner;
        ((unsigned char *)spawned_moby)[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x7F;
        spawned_moby[0x31] = 1;
        MOBY(spawned_moby)->spawn_frame = *(long *)(owner + 0x38);
        MOBY(spawned_moby)->rot.y = initial_pitch;
        MOBY(spawned_moby)->rot.z = initial_yaw;
        *(int *)(spawned_moby + 0x40) = 0;
        qcopy(spawned_moby + 0x10, cp);
        *(float *)(moby_vars + 8) = target_yaw;
        *(float *)(moby_vars + 0xC) = target_pitch;
        *(int *)moby_vars = 0;
        *(int *)(moby_vars + 4) = 0;
        lifetime_frames = scale_game_frames(0x23);
        *(short *)(moby_vars + 0x14) = lifetime_frames;
        *(short *)(moby_vars + 0x16) = lifetime_frames;
        spawned_moby[0x23] = 0x40;
        MOBY(spawned_moby)->flags |= 0xA00;
        MOBY(spawned_moby)->scale = MOBY(spawned_moby)->scale * 0.5f;
    }
    return spawned_moby;
}

typedef struct {
    OvlQuad dir;
    void   *owner;
    s32     flags;
    u8      b28;
    u8      b29;
    u16     id;
    f32     scale;
    s32     count;
} DropProbe;

extern f32     spring_turn(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern s32     collision_line_30c9a8(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern s32     collision_hit_material(void) __asm__("FUN_001f0b58");
extern f32     D_L01_00167240_30c9a8[] __asm__("D_L01_00167240");
extern OvlQuad D_0013F3D0_30c9a8 __asm__("D_0013F3D0");

extern OvlQuad D_L01_001742E0;
void           update_sandmouse_projectile(char *moby) __asm__("FUN_L01_0030c9a8");

void update_sandmouse_projectile(char *moby) {
    char     *moby_vars = ((char *)MOBY(moby)->pvars);
    f32      *moby_position;
    OvlVec4   old_pos;
    OvlVec4   offset;
    DropProbe probe;
    f32       desired_yaw, desired_pitch;
    f32       pitch_k, pitch_d, pitch_max;
    f32       height = *(f32 *)(((char *)MOBY(moby)->pclass) + 0x24);

    if (MOBY(moby)->scale < height)
        MOBY(moby)->scale += frame_scale * 0.21f * height;

    if (*(s16 *)(moby_vars + 0x14) < scale_game_frames(0x23) - scale_game_frames(1)) {
        desired_yaw = *(f32 *)(moby_vars + 8);
        desired_pitch = *(f32 *)(moby_vars + 0xc);
    } else {
        desired_yaw = MOBY(moby)->rot.z;
        desired_pitch = MOBY(moby)->rot.y;
    }
    moby_position = (f32 *)&MOBY(moby)->pos;
    desired_yaw = spring_turn((f32 *)moby_vars,
                              MOBY(moby)->rot.z,
                              desired_yaw,
                              frame_time_sq * 4.712389f,
                              frame_time_sq * 3.1415927f,
                              frame_time * 3.1415927f);
    pitch_d = frame_time_sq * 3.1415927f;
    pitch_k = frame_time_sq * 4.712389f;
    pitch_max = frame_time * 3.1415927f;
    MOBY(moby)->rot.z = desired_yaw;
    {
        f32 pitch_result = spring_turn(
            (f32 *)(moby_vars + 4), MOBY(moby)->rot.y, desired_pitch, pitch_k, pitch_d, pitch_max);
        pitch_max = frame_time * 40.0f;
        MOBY(moby)->rot.y = pitch_result;
        build_spherical_offset(offset.f, pitch_max, MOBY(moby)->rot.z, -pitch_result);
    }
    qcopy(&old_pos, moby_position);
    fast_vec_add(moby_position, moby_position, &offset);
    if (MOBY(moby)->pos.x < 2.0f || MOBY(moby)->pos.x > 1021.0f || MOBY(moby)->pos.y < 2.0f ||
        MOBY(moby)->pos.y > 1021.0f || MOBY(moby)->pos.z < 2.0f || MOBY(moby)->pos.z > 1021.0f ||
        MOBY(moby)->pos.x < 0.0f || MOBY(moby)->pos.y < 0.0f || MOBY(moby)->pos.z < 0.0f ||
        vector_distance_xy(moby_position, D_L01_00167240_30c9a8) > 64.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    probe.owner = moby;
    probe.flags = 0x10001;
    probe.scale = 0.5f;
    probe.count = 1;
    fast_vec_sub(&probe.dir, moby_position, &D_0013F3D0_30c9a8);
    set_vector_length_xy(&probe.dir, &probe.dir, 1.0f);
    ((f32 *)&probe.dir)[2] = 1.0f;
    ((f32 *)&probe.dir)[3] = 5627.9248f;
    probe.b28 = 1;
    probe.b29 = 1;
    probe.id = ((u16)MOBY(moby)->oclass);
    if (fast_dec_timer_s16(moby_vars + 0x14)) {
        mark_moby_for_removal(moby);
        return;
    }
    {
        if (!collision_line_30c9a8(
                &old_pos, moby_position, 0, *(void **)(moby_vars + 0x10), &probe))
            return;
        if (!collision_hit_material()) {
            if (*(s16 *)(moby_vars + 0x14) > scale_game_frames(3))
                *(s16 *)(moby_vars + 0x14) = scale_game_frames(3);
            return;
        }
        qcopy(moby_position, &D_L01_001742E0);
    }
    mark_moby_for_removal(moby);
}
