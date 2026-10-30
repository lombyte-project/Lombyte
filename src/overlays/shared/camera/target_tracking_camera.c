/* Camera type 7: a camera that tracks a target and turns its view towards the hero. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/level_labels.h"

typedef union {
    OvlQuad q;
    float   f[4];
} V_2ec4a8;

f32 rope_spring_turn(void *, f32, f32, f32, f32, f32) __asm__("FUN_L00_001eb328");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void resolve_camera_movement_collisions(void *, float) __asm__("FUN_L00_001ed6a8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_0025b868(float, float, float *);
extern void FUN_001ff500_2ebe70(float, void *, void *) __asm__("FUN_001f9bf8");
extern void FUN_001ff300_2ebe70(float, void *, void *) __asm__("FUN_001f9a68");
extern int  collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int  FUN_001f0d60_2ebe70(float, void *, int, int) __asm__("FUN_L00_001f0d60");
void        FUN_001f9a28_2ec4a8(void *, void *, void *) __asm__("FUN_001f9a28");
void        FUN_001f9bf8_2ec4a8(void *, void *, float) __asm__("FUN_001f9bf8");
float       FUN_001f9ab0_2ec4a8(void *, void *) __asm__("FUN_001f9ab0");
void        FUN_001f9ad8_2ec4a8(void *, void *, void *) __asm__("FUN_001f9ad8");
void        refresh_point_light(s32 arg0) __asm__("FUN_00201f58");

extern char            D_0013F350[] NOT_SDA;
extern int             current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern unsigned short *D_L00_001AB840_2ebe70[] __asm__("D_L00_001AB840");
extern OvlQuad         D_L00_00173E60_2ebe70 __asm__("D_L00_00173E60");
float                  target_tracking_camera_turn_spring_gp __asm__("D_L00_00161DD8") = 0.0035f;
extern u8              D_L00_00166E30_lui[] __asm__("D_L00_00166E30") NOT_SDA;

float D_L01_00162250 = 0.0035f;
float D_L02_00162388 = 0.0035f;
float D_L03_00161E50 = 0.0035f;
float D_L04_00161F48 = 0.0035f;
float D_L05_001621A0 = 0.0035f;
float D_L06_00162568 = 0.0035f;
float D_L07_00161E20 = 0.0035f;
float D_L08_001625E0 = 0.0035f;
float D_L09_00161F38 = 0.0035f;
float D_L10_00162208 = 0.0035f;
float D_L11_00162770 = 0.0035f;
float D_L12_00162248 = 0.0035f;
float D_L13_001620C0 = 0.0035f;
float D_L14_00162478 = 0.0035f;
float D_L15_001623C8 = 0.0035f;
float D_L16_00162190 = 0.0035f;
float D_L17_00162678 = 0.0035f;
float D_L18_00162790 = 0.0035f;

/* Sets up the camera activation moby: tuning floats, then the pose copied from the current camera. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002ED088), where it is exact; names translated to the US level program. */
void initialize_target_tracking_camera(void *camera) __asm__("FUN_L00_002ebbd8");

void initialize_target_tracking_camera(void *camera) {
    char *camera_object = camera;
    char *hero_data;
    int   lvl0 = current_level_index_macro;
    int   lvl;
    char *follow_block = *(char **)(camera_object + 0x70) + 0x40;
    float f;
    char *view_turn_spring_state, *camera_state, *camera_reset_state, *camera_target_state,
        *camera_data, *pose_source_camera;
    char *gg, *gg0, *hero_data_base;
    *(int *)(follow_block + 0x40) = 0;
    f = 1.5f;
    if (lvl0 == 0xE &&
        (hero_data_base = D_0013F350, (unsigned)(*(int *)(hero_data_base + 0x2084) - 0x2C) < 2))
        *(float *)(follow_block + 0x4C) = 2.0f;
    else
        *(float *)(follow_block + 0x4C) = 1.5f;
    hero_data = D_0013F350;
    *(float *)(follow_block + 0x48) = 0.20943951606750488f;
    *(float *)(follow_block + 0x58) = 0.001745329238474369f;
    *(float *)(follow_block + 0x5C) = -0.004363323096185923f;
    *(int *)(follow_block + 0x44) = *(int *)(hero_data + 0x2080);
    {
        char *distance_tuning_cursor = *(char **)(camera_object + 0x70);
        *(float *)(distance_tuning_cursor + 0xA0) = 4.639999866485596f;
        distance_tuning_cursor += 0xA0;
        *(float *)(distance_tuning_cursor + 4) = 0.5f;
        *(float *)(distance_tuning_cursor + 0x10) = 2.0f;
        *(float *)(distance_tuning_cursor + 0x14) = 0.05f;
    }
    camera_reset_state = *(char **)(camera_object + 0x70);
    *(int *)(camera_reset_state + 0x18) = 0;
    *(float *)(camera_reset_state + 0x10) = 0.01f;
    *(float *)(camera_reset_state + 0x14) = 0.3f;
    *(short *)(camera_reset_state + 0x1E) = scale_game_frames(0x78);
    *(short *)(camera_reset_state + 0x1C) = 0;
    clear_u64_value(camera_reset_state);
    camera_state = *(char **)(camera_object + 0x70);
    view_turn_spring_state = camera_state + 0x30;
    *(float *)(view_turn_spring_state + 4) = 0.02f;
    *(float *)(view_turn_spring_state + 8) = 0.2f;
    *(int *)(view_turn_spring_state + 0xC) = 0;
    *(int *)(camera_state + 0x30) = 0;
    camera_data = *(char **)(camera_object + 0x70);
    camera_target_state = camera_data + 0xB8;
    *(int *)(camera_target_state + 4) = 0;
    gg0 = (char *)&l00_camera;
    pose_source_camera = *(char **)(gg0 + 0x184);
    qcopy(camera_object + 0x30, pose_source_camera + 0x30);
    qcopy(camera_object, pose_source_camera);
    qcopy(camera_object + 0x10, pose_source_camera + 0x10);
    qcopy(camera_object + 0x20, pose_source_camera + 0x20);
    qcopy(camera_object + 0x40, camera_object);
    *(short *)(camera_object + 0x7E) = 0;
    lvl = *(int *)(hero_data + 0x2084);
    if ((unsigned)(lvl - 0x2C) < 2) {
        *(float *)(follow_block + 0x30) = fast_cos(*(float *)(hero_data + 0x9AC));
        *(float *)(follow_block + 0x34) = fast_sin(*(float *)(hero_data + 0x9AC));
        *(int *)(follow_block + 0x38) = 0;
        *(float *)(camera_data + 0xB8) = *(float *)(hero_data + 0x9AC);
        *(int *)(camera_target_state + 8) = *(int *)(hero_data + 0x994);
    } else {
        subtract_vector_xyz_c(
            follow_block + 0x30, *(char **)(hero_data + 0x964) + 0x10, camera_object + 0x30);
        *(int *)(follow_block + 0x38) = 0;
        *(int *)(follow_block + 0x3C) = 0;
        fast_vec_normalize(follow_block + 0x30, follow_block + 0x30, 1.0f);
        *(float *)(camera_data + 0xB8) =
            fast_arc_tan(*(float *)(follow_block + 0x30), *(float *)(follow_block + 0x34));
        *(int *)(camera_target_state + 8) = *(int *)(hero_data + 0x964);
    }
    gg = (char *)&l00_camera;
    *(char *)(gg + 0x273) = 2;
    *(short *)(gg + 0x270) = 1;
    *(int *)(gg + 0x2F4) = scale_game_frames(0x3C);
}

void update_target_tracking_camera_position(char *camera_object) __asm__("FUN_L00_002ebe70");

void update_target_tracking_camera_position(char *camera_object) {
    char           *state = *(char **)(camera_object + 0x70);
    char           *follow = state + 0x40;
    float          *distance = (float *)(state + 0xa0);
    float          *angle = (float *)(state + 0xb8);
    float           position[4] __attribute__((aligned(16)));
    float           offset[4] __attribute__((aligned(16)));
    float           direction[4] __attribute__((aligned(16)));
    float           aim[4] __attribute__((aligned(16)));
    float           target, rate, fraction, score, best, length;
    char           *candidate, *chosen, *current_target;
    float          *turn;
    OvlQuad        *collision_position;
    unsigned short *neighbours;
    int             i;

    if (hero.swing_target_moby != 0 && hero.state.current == 0x2c) {
        fraction = ((float *)hero.swing_target_moby)[6] - hero.motion.pos.f[2];
        if (fraction > hero.unk9A0)
            fraction = hero.unk9A0;
        else if (fraction < 0.0f)
            fraction = 0.0f;
        fraction /= hero.unk9A0;
        target = fraction + 3.65f;
        rate = fraction + fraction;
    } else {
        target = 4.64f;
        rate = 2.0f;
    }
    FUN_L00_0025b868(
        target, frame_time * 8.0f, (float *)(*(char **)((char *)l00_camera.unk180 + 0x70) + 0xa0));
    FUN_L00_0025b868(
        rate, frame_time * 4.0f, (float *)(*(char **)((char *)l00_camera.unk180 + 0x70) + 0xb0));
    if (current_level_index == 14 && hero.state.current == 0x2c)
        FUN_L00_0025b868(rate + 0.25f,
                         frame_time * 8.0f,
                         (float *)(*(char **)((char *)l00_camera.unk180 + 0x70) + 0x8c));
    else
        FUN_L00_0025b868(rate - 0.5f,
                         frame_time * 8.0f,
                         (float *)(*(char **)((char *)l00_camera.unk180 + 0x70) + 0x8c));

    if (hero.state.current == 0x2c) {
        if ((current_level_index != 14 && current_level_index != 7 && current_level_index != 9) ||
            (hero.swing_target_moby != 0 &&
             *(unsigned char *)((char *)hero.swing_target_moby + 0x21) == 255)) {
            *angle =
                rope_spring_turn(angle + 1, *angle, hero.motion.rot.f[2], 0.00125f, 0.175f, 0.0f);
        } else {
            chosen = 0;
            turn = angle + 1;
            best = 1000.0f;
            current_target = (char *)hero.swing_target_moby;
            /* Fakematch: a dead compare with 10.0f loads that constant early, which fixes the
             * register order of the loop constants and of turn. */
            if (target == 10.0f) {
            }
            neighbours = D_L00_001AB840_2ebe70[*(unsigned char *)(current_target + 0x21)];
            if (neighbours != 0) {
                do {
                    candidate = moby_table + (*neighbours & 0x7fff) * 0x100;
                    if (candidate != current_target && (*(short *)(candidate + 0xa6) == 0x323 ||
                                                        *(short *)(candidate + 0xa6) == 0x2f6)) {
                        fast_vec_sub(offset, candidate + 0x10, current_target + 0x10);
                        FUN_001ff500_2ebe70(1.0f, direction, offset);
                        aim[0] = fast_cos(hero.motion.rot.f[2]);
                        aim[1] = fast_sin(hero.motion.rot.f[2]);
                        aim[2] = 0.0f;
                        score = fast_vec_dot(direction, aim);
                        if (score > 0.0f) {
                            target = fast_vec_length(offset) - score * 10.0f;
                            if (target < best) {
                                chosen = candidate;
                                best = target;
                            }
                        }
                    }
                } while ((short)*neighbours++ >= 0);
            }
            if (chosen != 0) {
                fast_vec_sub(offset, chosen + 0x10, current_target + 0x10);
                *angle = rope_spring_turn(
                    turn, *angle, fast_arc_tan(offset[0], offset[1]), 0.00125f, 0.175f, 0.0f);
            } else if (*(int *)(follow + 0x40) == 1)
                *angle = rope_spring_turn(turn,
                                          *angle,
                                          *(float *)(*(char **)(follow + 0x44) + 0x48),
                                          0.00125f,
                                          0.175f,
                                          0.0f);
            else
                *angle =
                    rope_spring_turn(turn, *angle, hero.motion.rot.f[2], 0.00125f, 0.175f, 0.0f);
        }
    } else if (hero.state.current == 0x25) {
        if (*(int *)(follow + 0x40) == 1)
            *angle = rope_spring_turn(angle + 1,
                                      *angle,
                                      *(float *)(*(char **)(follow + 0x44) + 0x48),
                                      target_tracking_camera_turn_spring_gp,
                                      0.175f,
                                      0.0f);
        else
            *angle = rope_spring_turn(angle + 1,
                                      *angle,
                                      hero.motion.rot.f[2],
                                      target_tracking_camera_turn_spring_gp,
                                      0.175f,
                                      0.0f);
    }
    *(float *)(follow + 0x30) = fast_cos(*angle);
    *(float *)(follow + 0x34) = fast_sin(*angle);
    *(int *)(follow + 0x38) = 0;
    *(float *)(follow + 0x3c) = *(float *)(follow + 0x38);
    FUN_001ff500_2ebe70(-*distance, position, follow + 0x30);
    position[2] += distance[4];
    fast_vec_add(position, position, &hero.motion.pos);
    if (collision_line(camera_object + 0x30, position, 0x12, 0, 0) != 0) {
        fast_vec_sub(offset, &D_L00_00173E60_2ebe70, camera_object + 0x30);
        collision_position = &D_L00_00173E60_2ebe70;
        length = fast_vec_length(offset);
        if (length > 0.5f) {
            FUN_001ff300_2ebe70((length - 0.5f) / length, offset, offset);
            fast_vec_add(position, camera_object + 0x30, offset);
            for (i = 0; i < 5; i++) {
                if (FUN_001f0d60_2ebe70(0.75f, position, 0, 0) != 0)
                    qcopy(position, collision_position + 1);
                else
                    break;
            }
        }
        qcopy(camera_object + 0x30, position);
        fast_vec_sub(offset, &hero.motion.pos, camera_object + 0x30);
        *angle = fast_arc_tan(offset[0], offset[1]);
        *(int *)(angle + 1) = 0;
    } else
        qcopy(camera_object + 0x30, position);
}

void turn_camera_view_toward_hero(unsigned char *camera_object) __asm__("FUN_L00_002ec4a8");

void turn_camera_view_toward_hero(unsigned char *camera_object) {
    V_2ec4a8       raised_hero_position;
    V_2ec4a8       target_view_direction;
    V_2ec4a8       x;
    unsigned char *hero_position = (unsigned char *)&hero.motion.pos;
    unsigned char *camera_state = *(unsigned char **)(camera_object + 0x70);
    unsigned char *follow_block = camera_state + 0x40;
    float         *view_turn_state = (float *)(camera_state + 0x30);
    float          view_angle_error, k;
    qcopy(&raised_hero_position, hero_position);
    raised_hero_position.f[2] += *(float *)(follow_block + 0x4C);
    FUN_001f9a28_2ec4a8(&target_view_direction, &raised_hero_position, camera_object + 0x30);
    FUN_001f9bf8_2ec4a8(&target_view_direction, &target_view_direction, 1.0f);
    view_angle_error =
        1.5707964f - fast_arc_sin(FUN_001f9ab0_2ec4a8(&target_view_direction, camera_object));
    if (*(int *)(hero_position + 0x2004) == 0x25) {
        *(float *)(camera_state + 0x30) += 0.017f;
        if (1.0f < *(float *)(camera_state + 0x30))
            *(float *)(camera_state + 0x30) = 1.0f;
        k = view_angle_error * *(float *)(camera_state + 0x30);
    } else {
        k = rope_spring_turn(view_turn_state, 0.0f, view_angle_error, 0.02f, 0.175f, 0.0f);
    }
    FUN_001f9ad8_2ec4a8(&x, &target_view_direction, camera_object + 0x40);
    build_look_at_matrix(camera_object, camera_object + 0x40, &x, k);
    FUN_001f9bf8_2ec4a8(camera_object + 0x40, camera_object, 1.0f);
    qcopy(camera_object, camera_object + 0x40);
    FUN_001f9ad8_2ec4a8(camera_object + 0x10, camera_object, D_L00_00166E30_lui);
    FUN_001f9bf8_2ec4a8(camera_object + 0x10, camera_object + 0x10, 1.0f);
    FUN_001f9ad8_2ec4a8(camera_object + 0x20, camera_object + 0x10, camera_object);
}

/* camera update: copies the moby's matrix when the camera mode changes */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EDB70), where it is exact; names translated to the US level program. */
void update_target_tracking_camera(char *camera_object) __asm__("FUN_L00_002ec6c0");

void update_target_tracking_camera(char *camera_object) {
    struct Hero *b = &hero;
    char        *camera_target_state = *(char **)(camera_object + 0x70) + 0xB8;
    int          hero_state = b->state.current;
    char        *camera_position;
    int          x, y;
    if (hero_state == 0x2C) {
        camera_position = camera_object + 0x30;
        x = *(int *)(camera_target_state + 8);
        y = ((int)b->swing_target_moby);
    } else {
        camera_position = camera_object + 0x30;
        if ((unsigned)(hero_state - 0x24) >= 3)
            goto done;
        x = *(int *)(camera_target_state + 8);
        y = ((int)b->grapple_target);
    }
    if (x != y) {
        char *g = (char *)&l00_camera;
        char *r = *(char **)(g + 0x184);
        qcopy(r + 0x30, camera_position);
        qcopy(r, camera_object);
        qcopy(r + 0x10, camera_object + 0x10);
        qcopy(r + 0x20, camera_object + 0x20);
        initialize_target_tracking_camera(camera_object);
        *(int *)(g + 0x2F4) = scale_game_frames(0x2D);
    }
done:
    refresh_point_light((int)camera_object);
    resolve_camera_movement_collisions(camera_position, 0.5f);
    *(int *)(*(char **)(camera_object + 0x70) + 0x80) = 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EDC60), where it is exact; names translated to the US level program. */

/* Resets moby bytes 0x7D/0x7E unless its id at 0x86 is &hero.unk2284 or 0x2084 is 6. */
void conditionally_set_camera_state_three(char *camera_object) __asm__("FUN_L00_002ec7b0");

void conditionally_set_camera_state_three(char *camera_object) {
    struct Hero *hero_data = &hero;
    if (hero_data->unk2284 != *(short *)(camera_object + 0x86) && hero_data->state.current != 6) {
        *(char *)(camera_object + 0x7D) = 0;
        *(short *)(camera_object + 0x7E) = 3;
    }
}
