/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_globals.h"

extern u8 D_L00_00166E10_2e9010[] __asm__("D_L00_00166E10");
extern u8 D_0013F3D0_2e9010[] __asm__("D_0013F3D0");
extern s32 D_L00_00161D24_gp __asm__("D_L00_00161D24") __attribute__((sda));

void FUN_001f9a28_2e9010(void *, void *, void *) __asm__("FUN_001f9a28");
f32 FUN_001f9ab0_2e9010(void *, void *) __asm__("FUN_001f9ab0");
void FUN_001f9a68_2e9010(void *, void *, f32) __asm__("FUN_001f9a68");
void FUN_001f9ad8_2e9010(void *, void *, void *) __asm__("FUN_001f9ad8");
void FUN_001f9bf8_2e9010(void *, void *, f32) __asm__("FUN_001f9bf8");
f32 FUN_001f9af0_2e9010(void *) __asm__("FUN_001f9af0");
f32 FUN_001f9df8_2e9010(f32) __asm__("FUN_001f9df8");
f32 FUN_001f9e90_2e9010(f32, f32) __asm__("FUN_001f9e90");
f32 FUN_001ebd78_2e9010(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
f32 FUN_L00_001eb328_2e9010(void *, f32, f32, f32, f32, f32) __asm__("FUN_L00_001eb328");
f32 FUN_001fa5c8_2e9010(f32, f32) __asm__("FUN_001fa5c8");
f32 FUN_001fa6c0_2e9010(s32) __asm__("FUN_001fa6c0");
s32 FUN_001f96f8_2e9010(s32) __asm__("FUN_001f96f8");
s32 fast_dec_timer(void *) __asm__("FUN_001f9770");
void FUN_00214890_2e9010(void *, void *, void *, f32) __asm__("FUN_00214890");
void lerp_vector(void *, void *, void *, f32) __asm__("FUN_001f9a40");

/* Follow-camera orientation update: rebuilds the basis vectors at p from the
 * target moby, eases pitch/yaw and renormalises. */
void follow_camera_build_view(u8 *p) __asm__("FUN_L00_002e9010");
extern u8 D_L00_00166E30[];
void follow_camera_build_view(u8 *camera) {
    u8 *q40;
    u8 *camera_state;
    u8 *reloaded_camera_state;
    u8 *q20;
    u8 *reference_orbit_offset;
    u8 *tb;
    u8 *base;
    u8 *side_axis;
    u8 *view_up_axis;
    u8 *w;     /* scratch vector pointer: p + 0x10, later v4 */
    u8 *up;    /* up vector (D_L00_00166E30), set on both paths */
    f32 projected_view_length;
    f32 g2, reference_orbit_up_component, e2, view_blend_fraction;
    f32 position_up_component, adjusted_view_length, smoothed_height_difference, view_rotation_angle, target_view_angle_offset, x, smoothed_view_angle_offset;
    f32 vector_scratch[4];
    f32 projected_reference_position[4];
    f32 projected_camera_position[4];
    f32 view_direction[4];
    f32 adjusted_aim_position[4];

    base = D_L00_00166E10_2e9010;
    camera_state = *(u8 **)(camera + 0x70);
    q40 = camera_state + 0x40;
    q20 = camera_state + 0x20;
    if (q40[0xC4] == 0xB) {
        FUN_001f9a28_2e9010(camera, D_0013F3D0_2e9010, camera + 0x30);
        side_axis = camera + 0x10;
        FUN_001f9bf8_2e9010(camera, camera, 1.0f);
        view_up_axis = camera + 0x20;
        fast_dec_timer(q20);
        up = D_L00_00166E30;
    } else {
        up = D_L00_00166E30;
        tb = base + 0x20;
        position_up_component = FUN_001f9ab0_2e9010(camera_state + 0x80, tb);
        FUN_001f9a68_2e9010(vector_scratch, tb, position_up_component);
        FUN_001f9a28_2e9010(projected_reference_position, camera_state + 0x80, vector_scratch);
        reloaded_camera_state = *(u8 **)(camera + 0x70);
        reference_orbit_offset = reloaded_camera_state + 0x130;
        position_up_component = FUN_001f9ab0_2e9010(camera + 0x30, tb);
        FUN_001f9a68_2e9010(vector_scratch, tb, position_up_component);
        FUN_001f9a28_2e9010(projected_camera_position, camera + 0x30, vector_scratch);
        FUN_001f9a28_2e9010(view_direction, projected_reference_position, projected_camera_position);
        projected_view_length = FUN_001f9af0_2e9010(view_direction);
        if (0.05f <= projected_view_length) {
            FUN_001f9a68_2e9010(camera, view_direction, 1.0f / projected_view_length);
        }
        w = camera + 0x10;
        FUN_001f9ad8_2e9010(w, camera, up);
        FUN_001f9bf8_2e9010(w, w, 1.0f);
        side_axis = w;
        *(f32 *)(q20 + 0x8) = FUN_001ebd78_2e9010(camera_state + 0x30, *(f32 *)(q20 + 0x8), *(f32 *)(reference_orbit_offset + 0x30),
                               0.004f, 0.2f, 0.0f);
        view_up_axis = camera + 0x20;
        g2 = FUN_001ebd78_2e9010(camera_state + 0x2C, *(f32 *)(q20 + 0x4), *(f32 *)(q40 + 0xB0),
                               0.004f, 0.2f, 0.0f);
        smoothed_height_difference = *(f32 *)(q20 + 0x8) - g2;
        *(f32 *)(q20 + 0x4) = g2;
        FUN_001f9a68_2e9010(vector_scratch, up, -FUN_001f9ab0_2e9010(reloaded_camera_state + 0x140, up));
        FUN_001f9a28_2e9010(vector_scratch, vector_scratch, view_direction);
        w = (u8 *)adjusted_aim_position;
        FUN_001f9a28_2e9010(w, camera + 0x30, vector_scratch);
        FUN_001f9a68_2e9010(vector_scratch, up, -smoothed_height_difference);
        FUN_001f9a28_2e9010(w, w, vector_scratch);
        FUN_001f9a28_2e9010(view_direction, w, camera + 0x30);
        adjusted_view_length = FUN_001f9af0_2e9010(view_direction);
        if (adjusted_view_length != 0.0f) {
            view_rotation_angle = 1.5707964f - FUN_001f9df8_2e9010(FUN_001f9ab0_2e9010(camera, view_direction) / adjusted_view_length);
            FUN_001f9ad8_2e9010(view_up_axis, side_axis, camera);
            if (FUN_001f9ab0_2e9010(view_up_axis, view_direction) < 0.0f) {
                view_rotation_angle = -view_rotation_angle;
            }
            reference_orbit_up_component = FUN_001f9ab0_2e9010(reference_orbit_offset, up);
            x = FUN_001f9e90_2e9010(FUN_001f9af0_2e9010(reference_orbit_offset), reference_orbit_up_component) / 0.6981317f;
            target_view_angle_offset = 0.0f;
            if (x < -0.1f) {
                x = -x;
                if (0.5f < x) {
                    x = 1.0f - x;
                }
                x = x + x;
                target_view_angle_offset = x * 0.2617994f + target_view_angle_offset;
            }
            smoothed_view_angle_offset = FUN_L00_001eb328_2e9010(q20 + 0x18, *(f32 *)(q20 + 0x14), target_view_angle_offset, 0.005f, 0.2f, 0.0f);
            *(f32 *)(q20 + 0x14) = smoothed_view_angle_offset;
            view_rotation_angle = FUN_001fa5c8_2e9010(view_rotation_angle, smoothed_view_angle_offset);
            if (1.2217305f < view_rotation_angle) {
                view_rotation_angle = 1.2217305f;
            } else if (view_rotation_angle < -1.2217305f) {
                view_rotation_angle = -1.2217305f;
            }
            FUN_00214890_2e9010(camera, camera, side_axis, view_rotation_angle);
            FUN_001f9bf8_2e9010(camera, camera, 1.0f);
        }
    }
    if (*(s16 *)q20 != 0) {
        fast_dec_timer(q20);
        e2 = FUN_001fa6c0_2e9010(*(s16 *)q20);
        view_blend_fraction = e2 / FUN_001fa6c0_2e9010(FUN_001f96f8_2e9010(D_L00_00161D24_gp));
        lerp_vector(camera, camera, camera + 0x40, view_blend_fraction);
        FUN_001f9bf8_2e9010(camera, camera, 1.0f);
    }
    FUN_001f9ad8_2e9010(side_axis, camera, up);
    FUN_001f9bf8_2e9010(side_axis, side_axis, 1.0f);
    FUN_001f9ad8_2e9010(view_up_axis, side_axis, camera);
}

typedef struct {
    char p0[0x80];
    float v80[4];
    char p1[0xA4 - 0x90];
    float fA4;
} C_2e9518;
typedef struct {
    short h0, h2, h4, h6;
    float f8, fC;
} S_2e9518;
extern C_2e9518 D_166E10_2e9518 __asm__("D_L00_00166E10") __attribute__((section(".data")));
extern char *D_001413D0_2e9518 __asm__("D_001413D0") __attribute__((section(".data")));
extern unsigned char D_001413F4_2e9518 __asm__("D_001413F4") __attribute__((section(".data")));
extern float dot_2e9518(void *, void *) __asm__("FUN_001f9ab0");
extern float fabs_2e9518(float) __asm__("FUN_001f99c0");
extern int fr_2e9518(int) __asm__("FUN_001f96f8");
extern void pitch_s(int, float, float) __asm__("FUN_L00_002e8450");

void follow_camera_pull_back(char *o, float *x, float *y) __asm__("FUN_L00_002e9518");
extern float D_L00_00161CEC __attribute__((sda));
extern float D_L00_00161CF0 __attribute__((sda));
extern float D_L00_00161D14 __attribute__((sda));
extern float D_L00_00161D18 __attribute__((sda));
void follow_camera_pull_back(char *camera_object, float *x, float *y) {
    C_2e9518 *C = &D_166E10_2e9518;
    char *camera_state = *(char **)(camera_object + 0x70);
    S_2e9518 *pull_back_state = (S_2e9518 *)(camera_state + 0x220);
    char *reference_orbit_state = camera_state + 0x130;
    float k, pull_back_target, interpolated_pull_back_value;

    if (pull_back_state->h6 == 0) {
        if (pull_back_state->h4 != 0 && C->fA4 == 0.0f && dot_2e9518(D_001413D0_2e9518 + 0xC0, camera_object) <= -0.3f) {
            goto on;
        }
        if (0.0001f < fabs_2e9518(C->fA4) && dot_2e9518(C->v80, camera_object) <= -0.3f) {
        on:
            pull_back_state->h4 = fr_2e9518(0x78);
            k = C->fA4 * 14.2857f;
            if (1.0f < k)
                k = 1.0f;
            pull_back_target = D_001413F4_2e9518 == 1 ? 4.0f : pull_back_state->fC;
            interpolated_pull_back_value = *(float *)(reference_orbit_state + 0x58) + (pull_back_target - *(float *)(reference_orbit_state + 0x58)) * k;
            if (pull_back_state->f8 < interpolated_pull_back_value)
                pull_back_state->f8 = interpolated_pull_back_value;
            pitch_s(0, pull_back_state->f8, 0.003f);
            *x = D_L00_00161CEC + (D_L00_00161D14 - D_L00_00161CEC) * k;
            *y = D_L00_00161CF0 + (D_L00_00161D18 - D_L00_00161CF0) * k;
            return;
        }
    }
    if (pull_back_state->h4 != 0) {
        pull_back_state->h4 = 0;
        pull_back_state->f8 = *(float *)(reference_orbit_state + 0x58);
    }
    *x = D_L00_00161CEC;
    *y = D_L00_00161CF0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Aims a camera rig at its target: updates yaw/pitch smoothing and clamps roll. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002EABD0), where it is exact; names translated to the US level program. */

extern char D_L00_00166E10[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_arc_sin(float) __asm__("FUN_001f9df8");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float rope_spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_001eb328");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(float *, float *, float *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_002e9518_c(char *, float *, float *) __asm__("FUN_L00_002e9518");
extern void build_look_at_matrix(void *dst, void *vec, void *axis,
                                 float angle) __asm__("FUN_00214890");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
f32 cam_interp_values(void *p, f32 to, f32 stiffness, f32 damping, f32 max,
                      f32 vel) __asm__("FUN_001ebd78");
float absolute_float(float input) __asm__("func_001F99C0");

void follow_camera_orbit(int arg) __asm__("FUN_L00_002e9720");
extern short D_L00_00161CF4 __attribute__((sda));
extern short D_L00_00161CF8 __attribute__((sda));
extern short D_L00_00161CFC __attribute__((sda));
extern short D_L00_00161D00 __attribute__((sda));
void follow_camera_orbit(int camera_moby) {
    char *camera_object = (char *)camera_moby;
    char *camera_state = *(char **)(camera_object + 0x70);
    float reference_up_projection[4];
    float projected_reference_offset[4];
    float current_up_projection[4];
    float projected_current_offset[4];
    float orbit_cross_scratch[4];
    float rotated_orbit_offset[4];
    float scaled_up_offset[4];
    float orbit_base_position[4];
    float forward_clamp_axis[4];
    float tb[4];
    float x, bc, dot_product, reference_up_component, y, current_up_component, orbit_angle_scratch, i, yaw_rotation_sign, current_orbit_length;
    char *base = D_L00_00166E10;
    char *up = base + 0x20;
    char *base2;
    char *fw;
    char *reference_orbit_offset = camera_state + 0x130;
    char *current_orbit_offset = camera_state + 0x140;
    char *p1d0 = camera_state + 0x1D0;
    add_vector_xyz(camera_state, camera_state + 0x90, reference_orbit_offset);
    x = *(float *)(reference_orbit_offset + 0x2C);
    x -= *(float *)(p1d0 + 0x30);
    bc = fast_vec_length(current_orbit_offset);
    FUN_L00_002e9518_c(camera_object, &tb[0], &tb[1]);
    bc = cam_interp_values(camera_state + 0x158, bc, x, tb[0], tb[1], 0.0f);
    reference_up_component = fast_vec_dot(reference_orbit_offset, up);
    y = fast_arc_sin(reference_up_component / x);
    current_up_component = fast_vec_dot(current_orbit_offset, up);
    orbit_angle_scratch = fast_arc_sin(current_up_component / bc);
    i = rope_spring_turn(camera_state + 0x154, orbit_angle_scratch, y, *(float *)&D_L00_00161CFC, *(float *)&D_L00_00161D00,
                         0.0f);
    fast_vec_scale(reference_up_projection, up, reference_up_component);
    subtract_vector_xyz(projected_reference_offset, reference_orbit_offset, reference_up_projection);
    fast_vec_scale(current_up_projection, up, current_up_component);
    subtract_vector_xyz(projected_current_offset, current_orbit_offset, current_up_projection);
    dot_product = fast_vec_dot(projected_reference_offset, projected_current_offset);
    x = fast_vec_length(projected_reference_offset);
    x = x * fast_vec_length(projected_current_offset);
    orbit_angle_scratch = 1.5707964f - fast_arc_sin(dot_product / x);
    x = rope_spring_turn(camera_state + 0x150, 0.0f, orbit_angle_scratch, *(float *)&D_L00_00161CF4,
                         *(float *)&D_L00_00161CF8, 0.0f);
    fast_vec_cross(orbit_cross_scratch, projected_current_offset, up);
    yaw_rotation_sign = (fast_vec_dot(orbit_cross_scratch, projected_reference_offset) >= 0.0f) ? 1.0f : -1.0f;
    base2 = D_L00_00166E10;
    fw = base2 + 0x30;
    build_look_at_matrix(rotated_orbit_offset, projected_current_offset, up, x * yaw_rotation_sign);
    fast_vec_cross(orbit_cross_scratch, up, rotated_orbit_offset);
    build_look_at_matrix(rotated_orbit_offset, rotated_orbit_offset, orbit_cross_scratch, i);
    fast_vec_normalize(current_orbit_offset, rotated_orbit_offset, bc);
    fast_vec_scale(scaled_up_offset, up, *(float *)(reference_orbit_offset + 0x30));
    add_vector_xyz(orbit_base_position, scaled_up_offset, camera_state + 0xA0);
    dot_product = fast_vec_dot(current_orbit_offset, fw);
    current_orbit_length = fast_vec_length(current_orbit_offset);
    if (current_orbit_length != 0.0f) {
        x = 1.5707964f - fast_arc_sin(dot_product / current_orbit_length);
        if (absolute_float(x) < 0.2617994f) {
            fast_vec_cross(forward_clamp_axis, current_orbit_offset, fw);
            build_look_at_matrix(current_orbit_offset, current_orbit_offset, forward_clamp_axis,
                                 fast_subtract_rotations(0.2617994f, absolute_float(x)));
            *(float *)(reference_orbit_offset + 0x28) = 0.0f;
            *(float *)(reference_orbit_offset + 0x20) = 0.0f;
            *(float *)(reference_orbit_offset + 0x24) = 0.0f;
        }
    }
    add_vector_xyz(camera_object + 0x30, orbit_base_position, reference_orbit_offset + 0x10);
}
extern int *D_L00_0015EF80_9aa0 __asm__("D_L00_0015EF80");
extern char *D_L00_001600F4_9aa0 __asm__("D_L00_001600F4");
extern float FUN_001f9af0_9aa0(void *) __asm__("FUN_001f9af0");
extern void no_op_main_callback_alt(void *, float) __asm__("FUN_001e93e8");
typedef struct {
    int pad[4];
    int type;
    int idx;
    int r[6];
} E_9aa0;
void run_type5_grid_record_noop_callbacks(int unused_camera_moby) __asm__("FUN_L00_002e9aa0");

void run_type5_grid_record_noop_callbacks(int unused_camera_moby) {
    int i, j, k;
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            char *grid_cell_block = (char *)D_L00_0015EF80_9aa0[i * 64 + j];
            if (grid_cell_block != 0) {
                grid_cell_block += (int)D_L00_0015EF80_9aa0;
                for (k = 0; k < *(int *)grid_cell_block; k++) {
                    if (((E_9aa0 *)(grid_cell_block + 16))[k].type == 5) {
                        char *referenced_record = D_L00_001600F4_9aa0 + ((E_9aa0 *)(grid_cell_block + 16))[k].idx * 128;
                        no_op_main_callback_alt(referenced_record + 0x30, FUN_001f9af0_9aa0(referenced_record));
                    }
                }
            }
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002EB060), where it is exact; names translated to the US level program. */

extern void follow_camera_avoid_mobies(int) __asm__("FUN_L00_002e7d20");
extern void run_follow_camera_noop(int) __asm__("FUN_L00_002e8378");
extern void follow_camera_read_stick(int, float *, float *) __asm__("FUN_L00_002e89b0");
extern void follow_camera_turn_and_pitch(int, float, float) __asm__("FUN_L00_002e8bb8");
extern void follow_camera_auto_turn(int) __asm__("FUN_L00_002e8f40");
extern void FUN_L00_002e9010_u(int) __asm__("FUN_L00_002e9010");
extern void FUN_L00_002e9720_u(int) __asm__("FUN_L00_002e9720");
extern void run_type5_grid_record_noop_callbacks(int) __asm__("FUN_L00_002e9aa0");

/* Runs eight FUN_L00_002e* steps on object a in order. */
void update_follow_camera_pose(int camera_moby) __asm__("FUN_L00_002e9bb0");

void update_follow_camera_pose(int camera_moby) {
    float camera_stick_input[2];
    run_type5_grid_record_noop_callbacks(camera_moby);
    follow_camera_auto_turn(camera_moby);
    follow_camera_read_stick(camera_moby, &camera_stick_input[0], &camera_stick_input[1]);
    follow_camera_turn_and_pitch(camera_moby, camera_stick_input[0], camera_stick_input[1]);
    follow_camera_avoid_mobies(camera_moby);
    FUN_L00_002e9720_u(camera_moby);
    run_follow_camera_noop(camera_moby);
    FUN_L00_002e9010_u(camera_moby);
}
extern s32 opt_cam_yaw_rate __asm__("D_0015EDE4");
extern f32 D_L00_00161DB0[2] __attribute__((sda));
void apply_follow_camera_translation() __asm__("FUN_L00_002e46b8");
void follow_camera_apply_overrides() __asm__("FUN_L00_002e6000");
void noop_main_callback() __asm__("FUN_001e93e8");
void follow_camera_springs() __asm__("FUN_L00_002e5830");
void update_follow_camera_pose();
void follow_camera_approach() __asm__("FUN_L00_002e5e38");
void follow_camera_update(u8 *m) __asm__("FUN_L00_002e9c28");
extern f32 D_L00_0015EF44_gp __asm__("D_L00_0015EF44") __attribute__((sda));
void follow_camera_update(u8 *camera_moby) {
    u8 *camera_spring_state = *(u8 **)(camera_moby + 0x70) + 0x1D0;
    D_L00_0015EF44_gp = D_L00_00161DB0[opt_cam_yaw_rate];
    apply_follow_camera_translation();
    follow_camera_apply_overrides(camera_moby);
    noop_main_callback(camera_moby);
    follow_camera_springs(camera_moby);
    noop_main_callback(camera_moby, camera_spring_state);
    update_follow_camera_pose(camera_moby);
    noop_main_callback(camera_moby);
    follow_camera_approach(camera_moby);
}
extern unsigned char D_0015EDB3_002e9cc0 __asm__("D_0015EDB3");
extern void FUN_001f9bf8_002e9cc0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_001f9a68_002e9cc0(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_001f9a10_002e9cc0(void *, void *, void *) __asm__("FUN_001f9a10");

void update_hero_aligned_camera_position(char *camera_object) __asm__("FUN_L00_002e9cc0");

void update_hero_aligned_camera_position(char *camera_object) {
    char a[16] __attribute__((aligned(16)));
    char b[16] __attribute__((aligned(16)));
    struct Hero *hero_data = &hero;
    int vehicle_mode;
    FUN_001f9bf8_002e9cc0(a, ((char *)hero_data->moby) + 0xE0, 1.0f);
    vehicle_mode = hero_data->vehicle_mode;
    if (vehicle_mode == 2) {
        FUN_001f9a68_002e9cc0(b, a, 9.5f);
    } else if (vehicle_mode == 1) {
        if (D_0015EDB3_002e9cc0) {
            FUN_001f9a68_002e9cc0(b, a, 1.2f);
        } else {
            FUN_001f9a68_002e9cc0(b, a, 0.9f);
        }
    } else {
        FUN_001f9a68_002e9cc0(b, a, 1.6f);
    }
    camera_object += 0x30;
    FUN_001f9a10_002e9cc0(camera_object, &hero.motion.pos, b);
    FUN_001f9a10_002e9cc0(camera_object, &hero.motion.platform_carry, camera_object);
}
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
extern u128 D_0013F3D0_2e9dd0 __asm__("D_0013F3D0");
extern void *D_L00_00166E04_2e9dd0 __asm__("D_L00_00166E04") __attribute__((section(".data")));
extern void FUN_001f9bf8_2e9dd0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_001f9a68_2e9dd0(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_001f9a10_2e9dd0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9ad8_2e9dd0(void *, void *, void *) __asm__("FUN_001f9ad8");

void initialize_hero_aligned_camera_pose(char *camera_object) __asm__("FUN_L00_002e9dd0");

void initialize_hero_aligned_camera_pose(char *camera_object) {
    u128 hero_up_axis;
    u128 scaled_hero_up_axis;
    struct Hero *hero_state = &hero;
    FUN_001f9bf8_2e9dd0(&hero_up_axis, ((char *)hero_state->moby) + 0xE0, 1.0f);
    if (hero_state->vehicle_mode == 2) {
        FUN_001f9a68_2e9dd0(&scaled_hero_up_axis, &hero_up_axis, 9.5f);
    } else if (hero_state->vehicle_mode == 1) {
        FUN_001f9a68_2e9dd0(&scaled_hero_up_axis, &hero_up_axis, 0.9f);
    } else {
        FUN_001f9a68_2e9dd0(&scaled_hero_up_axis, &hero_up_axis, 1.6f);
    }
    FUN_001f9a10_2e9dd0(camera_object + 0x30, &D_0013F3D0_2e9dd0, &scaled_hero_up_axis);
    {
        void *side_axis_reference = D_L00_00166E04_2e9dd0;
        qcopy(camera_object + 0x20, &hero_up_axis);
        FUN_001f9ad8_2e9dd0(camera_object + 0x10, side_axis_reference, &hero_up_axis);
    }
    FUN_001f9bf8_2e9dd0(camera_object + 0x10, camera_object + 0x10, 1.0f);
    FUN_001f9ad8_2e9dd0(camera_object, &hero_up_axis, camera_object + 0x10);
    qcopy(camera_object + 0x40, camera_object);
}
void initialize_hero_aligned_camera_pose();
/* Clears seven words of the object at p+0x70, calls FUN_L00_002e9dd0, clears 0x7E. */
void reset_hero_aligned_camera(char *camera_object) __asm__("FUN_L00_002e9ef0");

void reset_hero_aligned_camera(char *camera_object) {
    int *camera_state = *(int **)(camera_object + 0x70);
    camera_state[0] = 0;
    camera_state[2] = 0;
    camera_state[1] = 0;
    camera_state[3] = 0;
    camera_state[4] = 0;
    camera_state[5] = 0;
    camera_state[6] = 0;
    initialize_hero_aligned_camera_pose(camera_object);
    *(short *)(camera_object + 0x7E) = 0;
}
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V;
typedef struct {
    float a, va, b, vb, ang, z, vz;
} S_9f38;
typedef struct {
    u8 pad0[0x100];
    float f100, f104, f108, f10C;
    u8 pad1[0x1A0 - 0x110];
    int i1A0;
} C940;
typedef struct {
    u8 pad0[0x180];
    char *current_follow_cam; /* 0x180: struct UpdateCam * */
    u8 pad184[0x270 - 0x184];
    s16 s270;
} G680;
extern C940 D_0013C940;
extern G680 D_L00_00166C80_c __asm__("D_L00_00166C80");
extern int D_0015EDDC;
extern float D_L00_00161DC0 __attribute__((sda));
extern float D_L00_00161DC4 __attribute__((sda));
extern float D_L00_00161DC8 __attribute__((sda));
extern float D_L00_00161DCC __attribute__((sda));
extern float D_L00_00161DD0 __attribute__((sda));
void FUN_L00_002e9cc0_c(void *) __asm__("FUN_L00_002e9cc0");
void FUN_001f9bf8_c(void *, void *, float) __asm__("FUN_001f9bf8");
void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
void FUN_001f9a10_c(void *, void *, void *) __asm__("FUN_001f9a10");
float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
float FUN_001f99c0(float);
float FUN_001ebd78(float, float, float, float, float, float *);
void build_look_at_matrix(void *, void *, void *, float) __asm__("FUN_00214890");
float FUN_001fa580_c(float, float) __asm__("FUN_001fa580");
float FUN_001fa5c8_c(float, float) __asm__("FUN_001fa5c8");
int coll_sphere(void *, int, void *, float) __asm__("FUN_L00_001f0d60");
void update_hero_aligned_camera_from_input(u8 *camera_object) __asm__("FUN_L00_002e9f38");

void update_hero_aligned_camera_from_input(u8 *camera_object) {
    V up;
    V side;
    V horizontal_view_offset;
    S_9f38 *camera_rotation_state;
    float rotation_input_target, rotation_input_scratch, k;
    camera_rotation_state = *(S_9f38 **)(camera_object + 0x70);
    FUN_L00_002e9cc0_c(camera_object);
    FUN_001f9bf8_c(&up, ((u8 *)hero.moby) + 0xE0, 1.0f);
    if (D_L00_00166C80_c.s270 == 0 && hero.unk20A5 == 0)
        hero.unk20A5 = 1;
    rotation_input_target = -D_0013C940.f108;
    if (rotation_input_target == 0.0f)
        rotation_input_target = -D_0013C940.f100;
    if (rotation_input_target == 0.0f)
        rotation_input_target = convert_integer_to_float((D_0013C940.i1A0 >> 15) & 1);
    if (rotation_input_target == 0.0f)
        rotation_input_target = -convert_integer_to_float((D_0013C940.i1A0 >> 13) & 1);
    rotation_input_scratch = FUN_001f99c0(camera_rotation_state->a);
    k = D_L00_00161DC0 + (D_L00_00161DC4 - D_L00_00161DC0) * rotation_input_scratch;
    camera_rotation_state->a = rotation_input_scratch = FUN_001ebd78(camera_rotation_state->a, rotation_input_target, D_L00_00161DC8, D_L00_00161DCC, D_L00_00161DD0, &camera_rotation_state->va);
    if (rotation_input_scratch != 0.0f)
        build_look_at_matrix(camera_object, camera_object, &up, k * rotation_input_scratch);
    if (hero.motion.platform_carry.f[3] != 0.0f && hero.unk308 == 0)
        build_look_at_matrix(camera_object, camera_object, &up, hero.motion.platform_carry.f[3]);
    rotation_input_target = -D_0013C940.f10C;
    if (rotation_input_target == 0.0f)
        rotation_input_target = -D_0013C940.f104;
    if (rotation_input_target == 0.0f)
        rotation_input_target = convert_integer_to_float((D_0013C940.i1A0 >> 12) & 1);
    if (rotation_input_target == 0.0f)
        rotation_input_target = -convert_integer_to_float((D_0013C940.i1A0 >> 14) & 1);
    if (D_L00_00166C80_c.s270 != 0 && hero.state.prev == 2)
        rotation_input_target = 0.0f;
    if (D_0015EDDC == 0)
        rotation_input_target = -rotation_input_target;
    rotation_input_scratch = FUN_001f99c0(camera_rotation_state->b);
    k = D_L00_00161DC0 + (D_L00_00161DC4 - D_L00_00161DC0) * rotation_input_scratch;
    camera_rotation_state->b = FUN_001ebd78(camera_rotation_state->b, rotation_input_target, D_L00_00161DC8, D_L00_00161DCC, D_L00_00161DD0, &camera_rotation_state->vb);
    if (camera_rotation_state->b != 0.0f) {
        float a, prospective_pitch_angle, z;
        fast_vec_cross(&side, camera_object, &up);
        a = k * camera_rotation_state->b;
        prospective_pitch_angle = FUN_001f99c0(FUN_001fa580_c(camera_rotation_state->ang, a));
        if (1.1170107f < prospective_pitch_angle && FUN_001f99c0(camera_rotation_state->ang) < prospective_pitch_angle)
            a += (0.0f - a) * (FUN_001fa5c8_c(prospective_pitch_angle, 1.1170107f) / 0.3490659f);
        if (prospective_pitch_angle >= 1.1152654f && camera_rotation_state->ang > 0.0f)
            camera_rotation_state->z = FUN_001ebd78(camera_rotation_state->z, 0.5f, 0.005f, 0.2f, 0.0f, &camera_rotation_state->vz);
        else
            camera_rotation_state->z = FUN_001ebd78(camera_rotation_state->z, 0.0f, 0.005f, 0.2f, 0.0f, &camera_rotation_state->vz);
        z = 0.0f;
        horizontal_view_offset.f[0] = *(float *)(camera_object + 0);
        horizontal_view_offset.f[1] = *(float *)(camera_object + 4);
        horizontal_view_offset.f[3] = z;
        horizontal_view_offset.f[2] = z;
        FUN_001f9bf8_c(&horizontal_view_offset, &horizontal_view_offset, camera_rotation_state->z);
        FUN_001f9a10_c(camera_object + 0x30, camera_object + 0x30, &horizontal_view_offset);
        if (coll_sphere(camera_object + 0x30, 0x12, ((u8 *)hero.moby), 0.3f))
            camera_rotation_state->z = FUN_001ebd78(camera_rotation_state->z, z, 0.005f, 0.2f, z, &camera_rotation_state->vz);
        prospective_pitch_angle = FUN_001fa580_c(camera_rotation_state->ang, a);
        if (FUN_001f99c0(prospective_pitch_angle) > 1.4660766f) {
            float signed_pitch_limit = 1.4660766f;
            if (!(z < prospective_pitch_angle))
                signed_pitch_limit = -1.4660766f;
            a = FUN_001fa5c8_c(signed_pitch_limit, camera_rotation_state->ang);
        }
        build_look_at_matrix(camera_object, camera_object, &side, a);
        camera_rotation_state->ang = FUN_001fa580_c(camera_rotation_state->ang, a);
    }
    FUN_001f9bf8_c(camera_object, camera_object, 1.0f);
    fast_vec_cross(camera_object + 0x10, camera_object, &up);
    FUN_001f9bf8_c(camera_object + 0x10, camera_object + 0x10, 1.0f);
    fast_vec_cross(camera_object + 0x20, camera_object + 0x10, camera_object);
}
/* FUN_L00_002ea488 -- camera-state steering: either replays the scripted
 * look (state 0x51 or the moby's own state) or steers toward the current
 * camera target, bailing out when the two are already within 80 degrees.
 *
 * Two spellings are load-bearing and were found by diffing against retail:
 *  - the two mode-validity reads (a1+0x86 and o+0x86) must go into TWO
 *    separate locals.  Reusing one local lets the allocator keep the second
 *    read in $5 (the dead second-argument register); retail puts it in $3.
 *  - the two length tests are FLAT, not nested:
 *        if (l != 0.0f) { l *= len(&d1); }
 *        if (l != 0.0f) { ... }
 *    Nesting the second test inside the first leaves the first `bc1t`
 *    delay slot empty; flat lets the `daddu $4,$s0,$zero` that sets up the
 *    later fast_vec_dot call fall into it, as retail has it.
 */

typedef struct {
    f32 f[4];
} V_2ea488;
typedef struct {
    u8 p0[0x20];
    s16 h20;
    s16 h22;
    s32 i24;
} Q_2ea488;
typedef struct {
    u8 p0[0x1C];
    Q_2ea488 *q;
} E_2ea488;
typedef struct {
    u8 p0[0x98];
    f32 f98;
    u8 p1[0x2080 - 0x9C];
    char *p2080;
    u8 p2[0x2284 - 0x2084];
    s32 i2284;
} P_2ea488;
typedef struct {
    char *o;
    u8 p[0x3C];
    V_2ea488 v;
} S_2ea488;
extern P_2ea488 D_0013F350_2ea488 __asm__("D_0013F350") __attribute__((section(".data")));
extern OvlQuad D_0013F3D0_2ea488 __asm__("D_0013F3D0") __attribute__((section(".data")));
extern E_2ea488 *D_L00_0015EF50_2ea488 __asm__("D_L00_0015EF50");
extern char *D_L00_00166E00_2ea488 __asm__("D_L00_00166E00") __attribute__((section(".data")));
extern char D_L00_00166E10_2ea488[] __asm__("D_L00_00166E10") __attribute__((section(".data")));
extern char *FUN_L00_001ee1b0_2ea488(void *, f32) __asm__("FUN_L00_001ee1b0");
extern int FUN_001f96f8_2ea488(int) __asm__("FUN_001f96f8");
extern f32 FUN_001f9dc8_2ea488(f32) __asm__("FUN_001f9dc8");
extern f32 FUN_001f9de0_2ea488(f32) __asm__("FUN_001f9de0");
extern f32 FUN_001f9df8_2ea488(f32) __asm__("FUN_001f9df8");
extern void FUN_L00_002e8918_2ea488(void *, f32, f32) __asm__("FUN_L00_002e8918");
extern f32 FUN_001f9ab0_2ea488(void *, void *) __asm__("FUN_001f9ab0");
extern void FUN_001f9a68_2ea488(void *, void *, f32) __asm__("FUN_001f9a68");
extern void FUN_001f9a28_2ea488(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 FUN_001f9af0_2ea488(void *) __asm__("FUN_001f9af0");
extern void FUN_L00_001ed280_2ea488(void) __asm__("FUN_L00_001ed280");
extern void FUN_001f9740_2ea488(void *) __asm__("FUN_001f9740");
#define P D_0013F350_2ea488
int FUN_L00_002ea488(u8 *, char *) __asm__("FUN_L00_002ea488");

int FUN_L00_002ea488(u8 *m, char *mode_check_object) {
    Q_2ea488 *q;
    char *o;
    char *r;
    s32 st;
    int t;
    int s;
    V_2ea488 v;
    V_2ea488 d1, d2;
    f32 l, x;
    P_2ea488 *g;
    char *b, *b2, *vec;

    q = D_L00_0015EF50_2ea488[*(s16 *)(m + 0x84)].q;
    g = &P;
    st = g->i2284;
    if (st == *(s16 *)(m + 0x86) || st == 0x51) {
        o = D_L00_00166E00_2ea488;
        t = *(s16 *)(mode_check_object + 0x86);
        if (t != 0 && t != 10 && t != 14 && t != 2)
            goto off;
        s = *(s16 *)(o + 0x86);
        if (s != 0 && s != 10 && s != 14 && s != 2)
            goto off;
        *(OvlQuad *)&v = D_0013F3D0_2ea488;
        r = FUN_L00_001ee1b0_2ea488(&v, 0.25f);
        if (r && (*(s32 *)(r + 0x18) & 1))
            return 0;
        q->i24 = 0;
        q->h22 = 1;
        q->h20++;
        if (q->h20 >= FUN_001f96f8_2ea488(7)) {
            if (*(s16 *)(o + 0x86) == 0)
                m[0x7C] = 5;
            return 1;
        }
        v.f[0] = FUN_001f9dc8_2ea488(P.f98);
        v.f[1] = FUN_001f9de0_2ea488(P.f98);
        *(s32 *)&v.f[2] = 0;
        FUN_L00_002e8918_2ea488(&v, 0.06981316953897476f, v.f[2]);
        goto tail;
    off:
        q->h20 = 0;
        q->h22 = 0;
        goto tail;
    }
    q->h20 = 0;
    if (q->h22 == 0)
        goto tail;
    q->h22 = 0;
    b = D_L00_00166E10_2ea488;
    b2 = b - 0x190;
    vec = b + 0x30;
    FUN_001f9a68_2ea488(&v, vec, FUN_001f9ab0_2ea488(vec, *(char **)(b2 + 0x180)));
    FUN_001f9a28_2ea488(&d1, *(char **)(b2 + 0x180), &v);
    FUN_001f9a68_2ea488(&v, vec, FUN_001f9ab0_2ea488(vec, g->p2080 + 0xC0));
    FUN_001f9a28_2ea488(&d2, g->p2080 + 0xC0, &v);
    l = FUN_001f9af0_2ea488(&d2);
    if (l != 0.0f) {
        l *= FUN_001f9af0_2ea488(&d1);
    }
    if (l != 0.0f) {
        x = FUN_001f9df8_2ea488(FUN_001f9ab0_2ea488(&d2, &d1) / l);
        if (1.3962633609771729f <= 1.5707964f - x) {
            FUN_L00_001ed280_2ea488();
            return 0;
        }
    }
    q->i24 = FUN_001f96f8_2ea488(30);
tail:
    if (q->i24) {
        FUN_001f9740_2ea488(&q->i24);
        v.f[0] = FUN_001f9dc8_2ea488(P.f98);
        v.f[1] = FUN_001f9de0_2ea488(P.f98);
        *(s32 *)&v.f[2] = 0;
        FUN_L00_002e8918_2ea488(&v, 0.13962633907794952f, v.f[2]);
    }
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EBC50), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x254];
    int p;
    char pad2[0x18];
    short s;
    char pad3[1];
    char c;
    char pad4[0x14];
    float f0;
    char pad5[8];
    float f1;
} S;

extern S D_L00_00166C80;
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_002e9ef0_v(void) __asm__("FUN_L00_002e9ef0");

/* Sets s to 1 (2 if set), resets D_L00_00166C80, calls 002e9ef0, allocates a voice for its p. */
void initialize_hero_aligned_camera_mode(void) __asm__("FUN_L00_002ea7a0");

void initialize_hero_aligned_camera_mode(void) {
    S *shared_camera_globals = &D_L00_00166C80;
    S *camera_globals;
    if (shared_camera_globals->s == 0)
        shared_camera_globals->s = 1;
    else
        shared_camera_globals->s = 2;
    camera_globals = &D_L00_00166C80;
    camera_globals->c = 0;
    camera_globals->f1 = 0.05f;
    camera_globals->f0 = 0.05f;
    FUN_L00_002e9ef0_v();
    if (camera_globals->p != 0)
        allocate_voice_for_target_entry(0, 0, camera_globals->p);
}
typedef struct {
    unsigned char pad0[0x1C0];
    float v1C0[4];
    unsigned char pad1D0[0x270 - 0x1D0];
    short s270;
    unsigned char pad272;
    unsigned char b273;
    unsigned char pad274[0x288 - 0x274];
    float f288;
    unsigned char pad28C[0x294 - 0x28C];
    float f294;
} Q_2ea810;
typedef struct {
    unsigned char pad0[0x7E];
    short s7E;
} A_2ea810;
extern Q_2ea810 D_L00_00166C80_2ea810 __asm__("D_L00_00166C80") __attribute__((section(".data")));
extern float FUN_001f9ab0_2ea810(void *, void *) __asm__("FUN_001f9ab0");
extern void FUN_001f9a68_2ea810(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_001f9a28_2ea810(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_2ea810(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9df8_2ea810(float) __asm__("FUN_001f9df8");
#define Q D_L00_00166C80_2ea810

void select_camera_mode_by_hero_vector_alignment(A_2ea810 *camera_object) __asm__("FUN_L00_002ea810");

void select_camera_mode_by_hero_vector_alignment(A_2ea810 *camera_object) {
    float scaled_axis_scratch[4];
    float camera_vector_residual[4];
    float hero_vector_residual[4];
    float *comparison_axis;
    float residual_length_product;
    float dot_product_or_asin_result;

    if (hero.unk2284 == 0x51 || hero.unk2284 == 4) {
        return;
    }
    if (Q.s270 == 0) {
        Q.b273 = 0;
        camera_object->s7E = 2;
        Q.f294 = 0.018f;
        Q.f288 = 0.018f;
        return;
    }
    comparison_axis = Q.v1C0;
    dot_product_or_asin_result = FUN_001f9ab0_2ea810(comparison_axis, camera_object);
    FUN_001f9a68_2ea810(scaled_axis_scratch, comparison_axis, dot_product_or_asin_result);
    FUN_001f9a28_2ea810(camera_vector_residual, camera_object, scaled_axis_scratch);
    dot_product_or_asin_result = FUN_001f9ab0_2ea810(comparison_axis, ((char *)hero.moby) + 0xC0);
    FUN_001f9a68_2ea810(scaled_axis_scratch, comparison_axis, dot_product_or_asin_result);
    FUN_001f9a28_2ea810(hero_vector_residual, ((char *)hero.moby) + 0xC0, scaled_axis_scratch);
    residual_length_product = FUN_001f9af0_2ea810(hero_vector_residual);
    if (residual_length_product != 0.0f) {
        residual_length_product *= FUN_001f9af0_2ea810(camera_vector_residual);
        if (residual_length_product != 0.0f) {
            dot_product_or_asin_result = FUN_001f9df8_2ea810(FUN_001f9ab0_2ea810(hero_vector_residual, camera_vector_residual) / residual_length_product);
            if (1.5707964f - dot_product_or_asin_result >= 1.3962634f) {
                Q.s270 = 0;
                camera_object->s7E = 4;
                return;
            }
        }
    }
    Q.b273 = 0;
    camera_object->s7E = 2;
    Q.f294 = 0.018f;
    Q.f288 = 0.018f;
}
/* Copies a vector into the current object's slot, switching to state 5 first. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EBE88), where it is exact; names translated to the US level program. */

extern char *D_L00_00166E00 __attribute__((section(".data")));
extern char *find_camera_record_by_id(int) __asm__("FUN_L00_001eb1f8");

/* Copies src to +0x80 of the object of entry 5 (from FUN_L00_001eb1f8 if needed). */
void FUN_L00_002ea9d8(char *src);
void FUN_L00_002ea9d8(char *requested_position) {
    char *current_camera_record = D_L00_00166E00;
    char *camera_record;
    if (*(short *)(current_camera_record + 0x86) != 5)
        camera_record = find_camera_record_by_id(5);
    else
        camera_record = current_camera_record;
    qcopy(*(char **)(camera_record + 0x70) + 0x80, requested_position);
}
typedef struct {
    f32 v[4];
} __attribute__((aligned(16))) V_2eaaa0;
extern V_2eaaa0 D_L00_00166DC0_2eaaa0[2] __asm__("D_L00_00166DC0")
    __attribute__((section(".data")));
extern V_2eaaa0 D_0013F3D0_2eaaa0 __asm__("D_0013F3D0") __attribute__((section(".data")));
u8 *FUN_L00_001eb1f8_2eaaa0(s32) __asm__("FUN_L00_001eb1f8");
void FUN_001ebf10_2eaaa0(u8 *) __asm__("FUN_001ebf10");
void FUN_L00_001ed630_2eaaa0(void *, V_2eaaa0 *, V_2eaaa0 *) __asm__("FUN_L00_001ed630");
void jet_mount(V_2eaaa0 *pa, V_2eaaa0 *pb, s32 mode, s32 w, s32 x) __asm__("FUN_L00_002eaaa0");
void jet_mount(V_2eaaa0 *pa, V_2eaaa0 *pb, s32 mode, s32 transition_duration_frames, s32 x) {
    u8 *camera_record = FUN_L00_001eb1f8_2eaaa0(5);
    V_2eaaa0 requested_position, requested_rotation, saved_camera_position, saved_camera_rotation;
    u8 *camera_state;
    u8 *transition_state;
    u8 *target_position;
    qcopy(&requested_position, pa);
    qcopy(&requested_rotation, pb);
    qcopy(&saved_camera_position, &D_L00_00166DC0_2eaaa0[0]);
    qcopy(&saved_camera_rotation, &D_L00_00166DC0_2eaaa0[1]);
    FUN_001ebf10_2eaaa0(camera_record);
    camera_state = *(u8 **)(camera_record + 0x70);
    transition_state = camera_state + 0xE0;
    target_position = camera_state + 0x80;
    camera_record[0x7C] = 7;
    camera_record[0x7D] = 1;
    qcopy(camera_record + 0x30, &requested_position);
    qcopy(&D_L00_00166DC0_2eaaa0[0], &requested_position);
    qcopy(camera_record + 0x40, &requested_rotation);
    qcopy(target_position, &requested_position);
    qcopy(camera_state + 0x90, &requested_rotation);
    *(s32 *)(transition_state + 0x40) = x;
    camera_record[0x88] = mode;
    if (mode != 2) {
        if (mode != 3)
            return;
        *(f32 *)(transition_state + 0x44) = 1.0f;
        *(f32 *)(transition_state + 0x48) = 0.0f;
        qcopy(camera_record + 0x30, &saved_camera_position);
        qcopy(camera_record + 0x40, &saved_camera_rotation);
        qcopy(camera_state + 0x110, &D_0013F3D0_2eaaa0);
        FUN_L00_001ed630_2eaaa0(camera_state + 0xFC, &saved_camera_position, &D_0013F3D0_2eaaa0);
    }
    *(s32 *)(transition_state + 4) = transition_duration_frames;
    *(s32 *)(camera_state + 0xE0) = transition_duration_frames;
}
extern char D_L00_00166C80_2eac18[] __asm__("D_L00_00166C80") __attribute__((section(".data")));
void initialize_camera_record_handoff_defaults(void) __asm__("FUN_L00_001eb448");
void FUN_001fa050_2eac18(void *, void *) __asm__("FUN_001fa050");
void FUN_001fa2b8_2eac18(void *, void *) __asm__("FUN_001fa2b8");
void FUN_001f9bf8_2eac18(void *, void *, float) __asm__("FUN_001f9bf8");
void FUN_001f9a10_2eac18(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L00_002eac18(int mode);
void FUN_L00_002eac18(int mode) {
    char *camera_globals = D_L00_00166C80_2eac18;
    char *current_follow_camera = *(char **)(camera_globals + 0x180);
    initialize_camera_record_handoff_defaults();
    if (mode == 0) {
        float v0[4] __attribute__((aligned(16)));
        float v1[4] __attribute__((aligned(16)));
        float m[16] __attribute__((aligned(16)));
        char *hero_rotation = (char *)&hero.motion.rot;
        char *camera_position = current_follow_camera + 0x30;
        FUN_001fa050_2eac18(m, hero_rotation);
        FUN_001fa2b8_2eac18(current_follow_camera, m);
        FUN_001f9bf8_2eac18(v1, m, -1.0f);
        FUN_001f9bf8_2eac18(v0, &m[8], 1.2f);
        FUN_001f9a10_2eac18(camera_position, hero_rotation - 0x10, v0);
        FUN_001f9a10_2eac18(camera_position, camera_position, v1);
        *(short *)(*(char **)(camera_globals + 0x180) + 0x8A) = 1;
    } else if (mode == 2) {
        *(char *)(camera_globals + 0x273) = 0;
        *(short *)(current_follow_camera + 0x7E) = 2;
        *(float *)(camera_globals + 0x294) = 0.018f;
        *(float *)(camera_globals + 0x288) = 0.018f;
    } else if (mode == 1) {
        *(short *)(current_follow_camera + 0x7E) = 3;
    } else if (mode == 4) {
        *(char *)(camera_globals + 0x273) = 0;
        *(short *)(current_follow_camera + 0x7E) = 5;
        *(float *)(camera_globals + 0x294) = 0.018f;
        *(float *)(camera_globals + 0x288) = 0.018f;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC210), where it is exact; names translated to the US level program. */

extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");

void initialize_camera_pose_springs(char *camera_object) __asm__("FUN_L00_002ead60");

void initialize_camera_pose_springs(char *camera_object) {
    float rotation_matrix[16];
    char *rotation_spring_state;
    char *camera_state;
    *(int *)(*(char **)(camera_object + 0x70) + 0xC0) = 4;
    rotation_spring_state = *(char **)(camera_object + 0x70) + 0x30;
    *(float *)(rotation_spring_state + 0x20) = 0.05f;
    *(float *)(rotation_spring_state + 0x24) = 0.3f;
    *(float *)(rotation_spring_state + 0x28) = 1.0f;
    *(short *)(rotation_spring_state + 0x2C) = 1;
    clear_u64_value(rotation_spring_state);
    camera_state = *(char **)(camera_object + 0x70);
    *(float *)(camera_state + 0x10) = 0.3f;
    *(float *)(camera_state + 0x14) = 0.3f;
    *(float *)(camera_state + 0x18) = 1.0f;
    *(short *)(camera_state + 0x1C) = 1;
    clear_u64_value(camera_state);
    euler_rotation_matrix(rotation_matrix, camera_object + 0x40);
    copy_matrix3x4(camera_object, rotation_matrix);
    *(short *)(camera_object + 0x7E) = 0;
}
#define F(p, o) (*(float *)((char *)(p) + (o)))

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC2D0), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L00_00166DD8 __attribute__((section(".data")));
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float rope_spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_001eb328");
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern float interpolate_wrapped_angle_cosine_eased(float, float, float) __asm__("FUN_L00_00257ef0");
extern float cosine_interpolate_alt(float, float, float) __asm__("FUN_002133d0");
extern int tick_countdown_32(int *) __asm__("FUN_001f9740");
extern void compute_position_from_spherical_offset(void *, void *, void *) __asm__("FUN_L00_001ed580");
extern void compute_spherical_offset_from_positions(void *, void *, void *) __asm__("FUN_L00_001ed630");
extern void interpolate_vector_xyz_cosine_eased(void *, void *, void *, float) __asm__("FUN_L00_00257f70");
extern void interpolate_wrapped_rotations_cosine_eased(float *, float *, float *, float) __asm__("FUN_L00_00258050");
f32 cam_interp_values(void *p, f32 to, f32 stiffness, f32 damping, f32 max,
                      f32 vel) __asm__("FUN_001ebd78");

void update_camera_pose_from_targets(void *camera) __asm__("FUN_L00_002eae20");

void update_camera_pose_from_targets(void *camera) {
    char *camera_object = camera;
    float mtx[16];
    char *camera_state = *(char **)(camera_object + 0x70);
    char *rotation_spring_state = camera_state + 0x30;
    char *target_position = camera_state + 0x80;
    char *transition_state = camera_state + 0xE0;
    int t = *(unsigned char *)(camera_object + 0x88);
    if (t == 1) {
        qcopy(camera_object + 0x30, target_position);
        qcopy(camera_object + 0x40, camera_state + 0x90);
    } else if (t == 2) {
        float transition_fraction;
        tick_countdown_32((int *)transition_state);
        transition_fraction = (float)(*(int *)(transition_state + 4) - *(int *)(camera_state + 0xE0)) / convert_integer_to_float(*(int *)(transition_state + 4));
        interpolate_vector_xyz_cosine_eased(camera_object + 0x30, camera_object + 0x30, target_position, transition_fraction);
        interpolate_wrapped_rotations_cosine_eased((float *)(camera_object + 0x40), (float *)(camera_object + 0x40), (float *)(camera_state + 0x90), transition_fraction);
    } else if (t == 3) {
        if (tick_countdown_32((int *)transition_state) != 0) {
            qcopy(camera_object + 0x30, target_position);
            qcopy(camera_object + 0x40, camera_state + 0x90);
        } else {
            float t, x, y, transition_azimuth_delta, z;
            t = (float)(*(int *)(transition_state + 4) - *(int *)(camera_state + 0xE0)) /
                convert_integer_to_float(*(int *)(transition_state + 4));
            compute_spherical_offset_from_positions(camera_state + 0xE8, target_position, &hero.motion.pos);
            x = fast_subtract_rotations(F(transition_state, 8), F(transition_state, 0x1C));
            y = fast_subtract_rotations(F(target_position, 0x18), D_L00_00166DD8);
            if ((x > 0.34906584f && y < -0.34906584f) || (x < -0.34906584f && y > 0.34906584f)) {
                transition_azimuth_delta = F(transition_state, 8) - F(transition_state, 0x1C);
                z = 0;
                if (transition_azimuth_delta < z && y > z) {
                    transition_azimuth_delta = -transition_azimuth_delta;
                } else if (transition_azimuth_delta > z && y < z) {
                    transition_azimuth_delta = -transition_azimuth_delta;
                }
                F(transition_state, 0x1C) += cosine_interpolate_alt(0, transition_azimuth_delta, t);
            } else {
                F(transition_state, 0x1C) = interpolate_wrapped_angle_cosine_eased(F(transition_state, 0x1C), F(transition_state, 8), t);
            }
            F(transition_state, 0x24) = F(transition_state, 0x24) + (F(transition_state, 0x10) - F(transition_state, 0x24)) *
                                          cubic_hermite_scalar(F(transition_state, 0x44), 0, 1.0f, F(transition_state, 0x48), t);
            F(transition_state, 0x20) = interpolate_wrapped_angle_cosine_eased(F(transition_state, 0x20), F(transition_state, 0xC), t);
            interpolate_vector_xyz_cosine_eased(transition_state + 0x30, transition_state + 0x30, &hero.motion.pos, t);
            compute_position_from_spherical_offset(camera_object + 0x30, transition_state + 0x1C, transition_state + 0x30);
            interpolate_wrapped_rotations_cosine_eased((float *)(camera_object + 0x40), (float *)(camera_object + 0x40), (float *)(target_position + 0x10), t);
        }
    } else {
        if (*(short *)(camera_state + 0x1C) == 1) {
            F(camera_object, 0x30) =
                cam_interp_values(camera_state, F(camera_object, 0x30), F(target_position, 0), F(camera_state, 0x10), F(camera_state, 0x14), F(camera_state, 0x18));
            F(camera_object, 0x34) =
                cam_interp_values(camera_state + 4, F(camera_object, 0x34), F(target_position, 4), F(camera_state, 0x10), F(camera_state, 0x14), F(camera_state, 0x18));
            F(camera_object, 0x38) =
                cam_interp_values(camera_state + 8, F(camera_object, 0x38), F(target_position, 8), F(camera_state, 0x10), F(camera_state, 0x14), F(camera_state, 0x18));
        }
        if (*(short *)(rotation_spring_state + 0x2C) == 1) {
            F(camera_object, 0x40) =
                rope_spring_turn(rotation_spring_state, F(camera_object, 0x40), F(target_position, 0x10), F(rotation_spring_state, 0x20), F(rotation_spring_state, 0x24), F(rotation_spring_state, 0x28));
            F(camera_object, 0x44) = rope_spring_turn(camera_state + 0x34, F(camera_object, 0x44), F(target_position, 0x14), F(rotation_spring_state, 0x20), F(rotation_spring_state, 0x24),
                                          F(rotation_spring_state, 0x28));
            F(camera_object, 0x48) = rope_spring_turn(camera_state + 0x38, F(camera_object, 0x48), F(target_position, 0x18), F(rotation_spring_state, 0x20), F(rotation_spring_state, 0x24),
                                          F(rotation_spring_state, 0x28));
        }
    }
    euler_rotation_matrix(mtx, camera_object + 0x40);
    copy_matrix3x4(camera_object, mtx);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC6C8), where it is exact; names translated to the US level program. */

extern void resolve_camera_movement_collisions(void *, float) __asm__("FUN_L00_001ed6a8");
extern void update_camera_pose_from_targets(void *) __asm__("FUN_L00_002eae20");

/* Runs FUN_L00_002eae20; if word +0x120 of its object is set, clears 0x89 and calls 001ed6a8. */
void update_camera_pose_with_optional_collision(char *camera_object) __asm__("FUN_L00_002eb218");

void update_camera_pose_with_optional_collision(char *camera_object) {
    char *transition_state = *(char **)(camera_object + 0x70) + 0xE0;
    update_camera_pose_from_targets(camera_object);
    if (*(int *)(transition_state + 0x40) != 0) {
        camera_object[0x89] = 0;
        resolve_camera_movement_collisions(camera_object + 0x30, 0.5f);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC728), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void compute_position_from_spherical_offset(void *, void *, void *) __asm__("FUN_L00_001ed580");

void initialize_orbit_camera_pose(void *camera) __asm__("FUN_L00_002eb278");

void initialize_orbit_camera_pose(void *camera) {
    char *camera_object = camera;
    float rotation_matrix[16];
    char *orbit_origin = *(char **)(camera_object + 0x70) + 0xB0;
    compute_position_from_spherical_offset(camera_object + 0x30, camera_object + 0x50, orbit_origin);
    *(float *)(camera_object + 0x50) = fast_add_rotations(*(float *)(camera_object + 0x50), *(float *)(orbit_origin + 0x18));
    *(int *)(camera_object + 0x40) = 0;
    *(float *)(camera_object + 0x44) = fast_add_rotations(*(float *)(camera_object + 0x54), *(float *)(camera_object + 0x5C));
    *(float *)(camera_object + 0x48) = fast_add_rotations(3.14159274101257324f, *(float *)(camera_object + 0x50));
    *(float *)(camera_object + 0x48) = fast_add_rotations(*(float *)(camera_object + 0x48), *(float *)(camera_object + 0x60));
    *(int *)(camera_object + 0x4C) = 0;
    euler_rotation_matrix(rotation_matrix, camera_object + 0x40);
    copy_matrix3x4(camera_object, rotation_matrix);
}
#include "eetypes.h"
#include "qcopy.h"
void FUN_L00_001eb0c8_002eb320(void *, void *, float) __asm__("FUN_L00_001eb0c8");
void zero_five_words(void *) __asm__("FUN_L00_001eb0b0");
void clear_twelve_bytes(void *) __asm__("FUN_L00_001ff250");
void FUN_L00_002eb278_002eb320(char *) __asm__("FUN_L00_002eb278");
void initialize_orbit_camera_from_parameters(char *camera_object, void *camera_initialization_values, u128 *initial_orbit_origin, float x) __asm__("FUN_L00_002eb320");

void initialize_orbit_camera_from_parameters(char *camera_object, void *camera_initialization_values, u128 *initial_orbit_origin, float x) {
    char *camera_state = *(char **)(camera_object + 0x70);
    char *orbit_origin = camera_state + 0xb0;
    qcopy(orbit_origin, initial_orbit_origin);
    qcopy(camera_state + 0x80, initial_orbit_origin);
    *(float *)(orbit_origin + 0x18) = x;
    FUN_L00_001eb0c8_002eb320(camera_state + 0x14, camera_initialization_values, x);
    FUN_L00_001eb0c8_002eb320(camera_object + 0x50, camera_initialization_values, 0.0f);
    zero_five_words(camera_state);
    clear_twelve_bytes(camera_state + 0x90);
    FUN_L00_002eb278_002eb320(camera_object);
}
typedef struct {
    u8 pad[0x28];
    f32 f28;
    f32 f2C;
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
    f32 f40;
    f32 f44;
    f32 f48;
    f32 f4C;
    u8 pad50[0x4C];
    s32 f9C;
    f32 fA0;
    f32 fA4;
    s32 fA8;
    s32 fAC;
    u8 padB0[0x40];
    s32 fF0;
} Moby002eb3b0;

typedef struct {
    u8 pad[0x70];
    Moby002eb3b0 *m;
    u8 pad78[0xA];
    s16 f7E;
} S002eb3b0;

extern void zero_five_words(void *) __asm__("FUN_L00_001eb0b0");
extern void clear_twelve_bytes(void *) __asm__("FUN_L00_001ff250");
extern void FUN_L00_002eb278_c(void *) __asm__("FUN_L00_002eb278");

void initialize_orbit_camera_state(S002eb3b0 *camera_object) __asm__("FUN_L00_002eb3b0");

void initialize_orbit_camera_state(S002eb3b0 *camera_object) {
    Moby002eb3b0 *m;
    camera_object->m->fF0 = 0;
    m = camera_object->m;
    m->f9C = 0;
    m->fA8 = 0;
    m->fAC = 0;
    m->fA0 = 0.3f;
    m->fA4 = 0.3f;
    m->f48 = 0.99f;
    m->f3C = 0.04f;
    m->f40 = 0.04f;
    m->f44 = 0.04f;
    m->f4C = 0.04f;
    m->f28 = 0.07f;
    m->f2C = 1.0f;
    m->f30 = 0.07f;
    m->f34 = 1.0f;
    m->f38 = 0.07f;
    zero_five_words((u8 *)m + 0x50);
    zero_five_words(m);
    clear_twelve_bytes((u8 *)m + 0x90);
    FUN_L00_002eb278_c(camera_object);
    camera_object->f7E = 0;
}
float FUN_001fa580_2eb480(float, float) __asm__("FUN_001fa580");
void FUN_001fa050_2eb480(void *, void *) __asm__("FUN_001fa050");
void FUN_001fa2b8_2eb480(void *, void *) __asm__("FUN_001fa2b8");
void compute_position_from_spherical_offset(void *, void *, void *) __asm__("FUN_L00_001ed580");
float FUN_L00_001eb328_2eb480(void *, float, float, float, float,
                              float) __asm__("FUN_L00_001eb328");
float FUN_001ebd78_2eb480(void *, float, float, float, float, float) __asm__("FUN_001ebd78");
#define F(p, o) (*(float *)((char *)(p) + (o)))
void update_orbit_camera_pose(char *camera_object) __asm__("FUN_L00_002eb480");

void update_orbit_camera_pose(char *camera_object) {
    float rotation_matrix[16];
    char *camera_state = *(char **)(camera_object + 0x70);
    char *orbit_origin = camera_state + 0xB0;
    F(camera_object, 0x50) = FUN_L00_001eb328_2eb480(camera_state, F(camera_object, 0x50), FUN_001fa580_2eb480(F(orbit_origin, 0x18), F(camera_state, 0x14)),
                                         F(camera_state, 0x28), F(camera_state, 0x3C), F(camera_state, 0x50));
    F(camera_object, 0x54) =
        FUN_L00_001eb328_2eb480(camera_state + 4, F(camera_object, 0x54), F(camera_state, 0x18), F(camera_state, 0x2C), F(camera_state, 0x40), F(camera_state, 0x54));
    F(camera_object, 0x58) =
        FUN_001ebd78_2eb480(camera_state + 8, F(camera_object, 0x58), F(camera_state, 0x1C), F(camera_state, 0x30), F(camera_state, 0x44), F(camera_state, 0x58));
    F(camera_object, 0x5C) = FUN_L00_001eb328_2eb480(camera_state + 0xC, F(camera_object, 0x5C), F(camera_state, 0x20), F(camera_state, 0x34), F(camera_state, 0x48),
                                         F(camera_state, 0x5C));
    F(camera_object, 0x60) = FUN_L00_001eb328_2eb480(camera_state + 0x10, F(camera_object, 0x60), F(camera_state, 0x24), F(camera_state, 0x38), F(camera_state, 0x4C),
                                         F(camera_state, 0x60));
    if (*(int *)(camera_state + 0x9C) == 0) {
        F(orbit_origin, 0) =
            FUN_001ebd78_2eb480(camera_state + 0x90, F(orbit_origin, 0), F(camera_state, 0x80), F(camera_state, 0xA0), F(camera_state, 0xA4), F(camera_state, 0xA8));
        F(orbit_origin, 4) =
            FUN_001ebd78_2eb480(camera_state + 0x94, F(orbit_origin, 4), F(camera_state, 0x84), F(camera_state, 0xA0), F(camera_state, 0xA4), F(camera_state, 0xA8));
        F(orbit_origin, 8) =
            FUN_001ebd78_2eb480(camera_state + 0x98, F(orbit_origin, 8), F(camera_state, 0x88), F(camera_state, 0xA0), F(camera_state, 0xA4), F(camera_state, 0xA8));
    } else {
        qcopy(orbit_origin, camera_state + 0x80);
    }
    compute_position_from_spherical_offset(camera_object + 0x30, camera_object + 0x50, orbit_origin);
    F(camera_object, 0x40) = F(camera_state, 0xAC);
    F(camera_object, 0x44) = FUN_001fa580_2eb480(F(camera_object, 0x54), F(camera_object, 0x5C));
    F(camera_object, 0x48) = FUN_001fa580_2eb480(3.14159274101257324f, F(camera_object, 0x50));
    F(camera_object, 0x48) = FUN_001fa580_2eb480(F(camera_object, 0x48), F(camera_object, 0x60));
    FUN_001fa050_2eb480(rotation_matrix, camera_object + 0x40);
    FUN_001fa2b8_2eb480(camera_object, rotation_matrix);
}
#undef F
extern struct Hero hero_data __asm__("D_0013F350") __attribute__((section(".data")));
extern char D_L00_00166C80_2eb648[] __asm__("D_L00_00166C80") __attribute__((section(".data")));
void FUN_001f9cf8_2eb648(void *, void *, void *) __asm__("FUN_001f9cf8");
void FUN_001f9a10_2eb648(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L00_001eb0c8_2eb648(void *, void *, float) __asm__("FUN_L00_001eb0c8");
void FUN_L00_002eb320_2eb648(char *, void *, void *, float) __asm__("FUN_L00_002eb320");
float FUN_001fa580_2eb648(float, float) __asm__("FUN_001fa580");
float FUN_001f9e90_2eb648(float, float) __asm__("FUN_001f9e90");
float FUN_001fa688_2eb648(float, float) __asm__("FUN_001fa688");
float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float D_L00_00161DD4 __attribute__((section(".data")));
int FUN_L00_002eb648(char *camera_object, char *target_moby) {
    u128 v[1];
    u128 target_orbit_origin[1];
    char *camera_state;
    float *camera_initialization_values;
    char *target_private_data;
    u128 *pw;
    struct Hero *hero_state;
    char *camera_globals;
    char *pp;
    char *cp;
    float ang, hero_to_camera_distance, x;
    camera_state = *(char **)(camera_object + 0x70);
    camera_initialization_values = (float *)(camera_state + 0x64);
    if (!target_moby)
        goto one;
    v[0] = 0;
    ((float *)v)[2] = 0.12f;
    ((float *)v)[0] = D_L00_00161DD4;
    target_private_data = *(char **)(target_moby + 0x78);
    FUN_001f9cf8_2eb648(v, v, target_moby + 0xC0);
    FUN_001f9a10_2eb648(target_orbit_origin, target_moby + 0x10, v);
    pw = target_orbit_origin;
    if (*(int *)(target_private_data + 0x10) == -1) {
        FUN_L00_001eb0c8_2eb648(camera_initialization_values, camera_object + 0x50, 0.0f);
        camera_initialization_values[2] = 6.0f;
        camera_initialization_values[1] = camera_initialization_values[3];
        camera_initialization_values[3] = 0.0f;
        FUN_L00_002eb320_2eb648(camera_object, camera_initialization_values, target_orbit_origin, 0.0f);
    }
    if (*(int *)(target_private_data + 0x10) > 0)
        goto pos;
    *(float *)(camera_state + 0xAC) = FUN_001fa580_2eb648(*(float *)(camera_state + 0xAC), *(float *)(target_private_data + 0x18));
    *(float *)(target_private_data + 0x18) *= 0.97f;
    hero_state = &hero_data;
    camera_globals = D_L00_00166C80_2eb648;
    pp = &hero_state->motion.pos;
    cp = camera_globals + 0x140;
    ang = FUN_001f9e90_2eb648(*(float *)(camera_globals + 0x140) - hero_state->motion.pos.f[0],
                              *(float *)(camera_globals + 0x144) - hero_state->motion.pos.f[1]);
    ang = FUN_001fa688_2eb648(ang, *(float *)(camera_globals + 0x158));
    hero_to_camera_distance = vector_distance(pp, cp);
    if (hero_to_camera_distance < vector_distance(((char *)hero_state->items[0].moby) + 0x10, cp)) {
        x = vector_distance(pp, cp);
    } else {
        x = vector_distance(((char *)hero_state->items[0].moby) + 0x10, cp);
    }
    if (x < 0.77f) {
        if (ang < 1.5707964f) {
            *(float *)(camera_state + 0x1C) -= 0.7f - x;
            *(int *)(target_private_data + 4) = 0;
        } else {
            *(float *)(camera_state + 0x1C) += 0.7f - x;
        }
    } else {
        *(float *)(camera_state + 0x1C) += *(float *)(target_private_data + 4);
        *(float *)(target_private_data + 4) *= 0.97f;
    }
    qcopy(camera_state + 0x80, pw);
    return 0;
pos:
    *(float *)(camera_state + 0xAC) = *(float *)(target_moby + 0x40) * 0.5f;
    *(float *)(camera_state + 0x20) += (*(float *)(target_moby + 0x44) - *(float *)(camera_state + 0x20)) * 0.05f;
    qcopy(camera_state + 0x80, target_orbit_origin);
    qcopy(camera_state + 0xC0, target_moby + 0x40);
one:
    return 1;
}
extern short D_L00_00166EF0_002eb930[] __asm__("D_L00_00166EF0");
char *FUN_L00_001eb1f8_002eb930(int) __asm__("FUN_L00_001eb1f8");
void FUN_001ebf10_002eb930(char *) __asm__("FUN_001ebf10");
void FUN_L00_002eb648_002eb930(char *, void *) __asm__("FUN_L00_002eb648");
void FUN_L00_001eb0c8_002eb930(void *, void *, float) __asm__("FUN_L00_001eb0c8");
void FUN_L00_002eb3b0_002eb930(char *) __asm__("FUN_L00_002eb3b0");
void zero_five_words(void *) __asm__("FUN_L00_001eb0b0");
extern char D_L00_001E77C0[];
extern char D_L00_001E77D8[];
extern char D_L00_001E77F0[];
char *initialize_mode6_orbit_camera(void *target_moby) __asm__("FUN_L00_002eb930");

char *initialize_mode6_orbit_camera(void *target_moby) {
    char *camera_record = FUN_L00_001eb1f8_002eb930(6);
    char *camera_state;
    D_L00_00166EF0_002eb930[0] = 0;
    FUN_001ebf10_002eb930(camera_record);
    FUN_L00_002eb648_002eb930(camera_record, target_moby);
    camera_state = *(char **)(camera_record + 0x70);
    qcopy(camera_state + 0xB0, camera_state + 0x80);
    FUN_L00_001eb0c8_002eb930(camera_state + 0x14, D_L00_001E77C0, 0.0f);
    FUN_L00_001eb0c8_002eb930(camera_record + 0x50, camera_state + 0x14, 0.0f);
    FUN_L00_002eb3b0_002eb930(camera_record);
    *(int *)(camera_state + 0x9C) = 1;
    FUN_L00_001eb0c8_002eb930(camera_state + 0x28, D_L00_001E77D8, 0.0f);
    FUN_L00_001eb0c8_002eb930(camera_state + 0x3C, D_L00_001E77F0, 0.0f);
    zero_five_words(camera_state + 0x50);
    return camera_record;
}
extern char D_00166C80_c[] __asm__("D_L00_00166C80");
float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
float FUN_001f9b48(void *, void *);
void select_camera_mode_by_hero_alignment_and_distance(char *camera_object) __asm__("FUN_L00_002eba18");

void select_camera_mode_by_hero_alignment_and_distance(char *camera_object) {
    struct Hero *hero_data = &hero;
    char *camera_globals = D_00166C80_c;
    char *hero_moby = ((char *)hero_data->moby);
    float a = fast_arc_tan(*(float *)(hero_moby + 0x10) - *(float *)(camera_globals + 0x140),
                           *(float *)(hero_moby + 0x14) - *(float *)(camera_globals + 0x144));
    if (fast_difference_between_rotations(*(float *)(camera_globals + 0x158), a) < 0.5585053563117981f &&
        FUN_001f9b48(&hero_data->motion.pos, camera_globals + 0x140) < 8.0f) {
        *(char *)(camera_globals + 0x273) = 0;
        *(short *)(camera_object + 0x7E) = 2;
        *(float *)(camera_globals + 0x294) = 0.018f;
        *(float *)(camera_globals + 0x288) = 0.018f;
    } else {
        *(short *)(camera_object + 0x7E) = 4;
    }
    *(char *)(camera_object + 0x7D) = 0;
}
#include "qcopy.h"
typedef struct {
    u8 pad[0x50];
    u8 x50[0x20];
    u8 *m;
} O002ebb00;
void zero_five_words(void *) __asm__("FUN_L00_001eb0b0");
void copy_five_floats_with_rotation_offset(void *, void *, f32) __asm__("FUN_L00_001eb0c8");
void FUN_L00_002eb3b0_u(O002ebb00 *) __asm__("FUN_L00_002eb3b0");
void initialize_orbit_camera_from_hero(O002ebb00 *camera_object) __asm__("FUN_L00_002ebb00");

void initialize_orbit_camera_from_hero(O002ebb00 *camera_object) {
    u8 *m = camera_object->m;
    u8 *raised_hero_position = m + 0xB0;
    u8 *camera_state;
    qcopy(raised_hero_position, &hero.motion.pos);
    *(f32 *)(raised_hero_position + 8) += 2.0f;
    qcopy(m + 0xC0, &hero.motion.rot);
    camera_state = camera_object->m;
    qcopy(camera_state + 0x80, raised_hero_position);
    zero_five_words(camera_state + 0x14);
    *(f32 *)(camera_state + 0x1C) = 4.64f;
    copy_five_floats_with_rotation_offset(camera_object->x50, camera_state + 0x14, 0.0f);
    FUN_L00_002eb3b0_u(camera_object);
}
/* Sets up the camera activation moby: tuning floats, then the pose copied from the current camera. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002ED088), where it is exact; names translated to the US level program. */

extern char D_0013F350[] __attribute__((section(".data")));
extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern int D_0015ED84 __attribute__((section(".sdata")));
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");

void initialize_target_tracking_camera(void *camera) __asm__("FUN_L00_002ebbd8");

void initialize_target_tracking_camera(void *camera) {
    char *camera_object = camera;
    char *hero_data;
    int lvl0 = D_0015ED84;
    int lvl;
    char *follow_block = *(char **)(camera_object + 0x70) + 0x40;
    float f;
    char *view_turn_spring_state, *camera_state, *r, *camera_target_state, *camera_data, *pose_source_camera;
    char *gg, *gg0, *hero_data_base;
    *(int *)(follow_block + 0x40) = 0;
    f = 1.5f;
    if (lvl0 == 0xE && (hero_data_base = D_0013F350, (unsigned)(*(int *)(hero_data_base + 0x2084) - 0x2C) < 2))
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
    r = *(char **)(camera_object + 0x70);
    *(int *)(r + 0x18) = 0;
    *(float *)(r + 0x10) = 0.01f;
    *(float *)(r + 0x14) = 0.3f;
    *(short *)(r + 0x1E) = scale_game_frames_c(0x78);
    *(short *)(r + 0x1C) = 0;
    clear_u64_value_c(r);
    camera_state = *(char **)(camera_object + 0x70);
    view_turn_spring_state = camera_state + 0x30;
    *(float *)(view_turn_spring_state + 4) = 0.02f;
    *(float *)(view_turn_spring_state + 8) = 0.2f;
    *(int *)(view_turn_spring_state + 0xC) = 0;
    *(int *)(camera_state + 0x30) = 0;
    camera_data = *(char **)(camera_object + 0x70);
    camera_target_state = camera_data + 0xB8;
    *(int *)(camera_target_state + 4) = 0;
    gg0 = (char *)&D_L00_00166C80;
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
        subtract_vector_xyz_c(follow_block + 0x30, *(char **)(hero_data + 0x964) + 0x10, camera_object + 0x30);
        *(int *)(follow_block + 0x38) = 0;
        *(int *)(follow_block + 0x3C) = 0;
        fast_vec_normalize(follow_block + 0x30, follow_block + 0x30, 1.0f);
        *(float *)(camera_data + 0xB8) = fast_arc_tan_c(*(float *)(follow_block + 0x30), *(float *)(follow_block + 0x34));
        *(int *)(camera_target_state + 8) = *(int *)(hero_data + 0x964);
    }
    gg = (char *)&D_L00_00166C80;
    *(char *)(gg + 0x273) = 2;
    *(short *)(gg + 0x270) = 1;
    *(int *)(gg + 0x2F4) = scale_game_frames_c(0x3C);
}

extern void FUN_L00_0025b868(float, float, float *);
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001ff500_2ebe70(float, void *, void *) __asm__("FUN_001f9bf8");
extern void FUN_001ff300_2ebe70(float, void *, void *) __asm__("FUN_001f9a68");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int coll_line_fix(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_001f0d60_2ebe70(float, void *, int, int) __asm__("FUN_L00_001f0d60");
extern unsigned short *D_L00_001AB840_2ebe70[] __asm__("D_L00_001AB840");
extern char *D_0015FFD8_2ebe70 __asm__("D_L00_0015FFD8");
extern OvlQuad D_L00_00173E60_2ebe70 __asm__("D_L00_00173E60");
extern float D_L00_00173E70_2ebe70[] __asm__("D_L00_00173E70");
extern float D_L00_00161DD8_2ebe70 __asm__("D_L00_00161DD8") __attribute__((sda));

void update_target_tracking_camera_position(char *camera_object) __asm__("FUN_L00_002ebe70");

void update_target_tracking_camera_position(char *camera_object) {
    char *state = *(char **)(camera_object + 0x70);
    char *follow = state + 0x40;
    float *distance = (float *)(state + 0xa0);
    float *angle = (float *)(state + 0xb8);
    float position[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    float direction[4] __attribute__((aligned(16)));
    float aim[4] __attribute__((aligned(16)));
    float target, rate, fraction, score, best, length;
    char *candidate, *chosen, *current_target;
    float *turn;
    OvlQuad *collision_position;
    unsigned short *neighbours;
    int i;

    if (hero.unk994 != 0 && hero.state.current == 0x2c) {
        fraction = ((float *)hero.unk994)[6] - hero.motion.pos.f[2];
        if (fraction > hero.unk9A0) fraction = hero.unk9A0;
        else if (fraction < 0.0f) fraction = 0.0f;
        fraction /= hero.unk9A0;
        target = fraction + 3.65f;
        rate = fraction + fraction;
    } else {
        target = 4.64f;
        rate = 2.0f;
    }
    FUN_L00_0025b868(target, frame_time * 8.0f, (float *)(*(char **)(D_L00_00166C80_c.current_follow_cam + 0x70) + 0xa0));
    FUN_L00_0025b868(rate, frame_time * 4.0f, (float *)(*(char **)(D_L00_00166C80_c.current_follow_cam + 0x70) + 0xb0));
    if (current_level_index == 14 && hero.state.current == 0x2c)
        FUN_L00_0025b868(rate + 0.25f, frame_time * 8.0f, (float *)(*(char **)(D_L00_00166C80_c.current_follow_cam + 0x70) + 0x8c));
    else
        FUN_L00_0025b868(rate - 0.5f, frame_time * 8.0f, (float *)(*(char **)(D_L00_00166C80_c.current_follow_cam + 0x70) + 0x8c));

    if (hero.state.current == 0x2c) {
        if ((current_level_index != 14 && current_level_index != 7 && current_level_index != 9) ||
            (hero.unk994 != 0 && *(unsigned char *)((char *)hero.unk994 + 0x21) == 255)) {
            *angle = rope_spring_turn(angle + 1, *angle, hero.motion.rot.f[2], 0.00125f, 0.175f, 0.0f);
        } else {
            chosen = 0;
            turn = angle + 1;
            best = 1000.0f;
            current_target = (char *)hero.unk994;
            /* Fakematch: a dead compare with 10.0f loads that constant early, which fixes the
             * register order of the loop constants and of turn. */
            if (target == 10.0f) { }
            neighbours = D_L00_001AB840_2ebe70[*(unsigned char *)(current_target + 0x21)];
            if (neighbours != 0) {
                do {
                    candidate = D_0015FFD8_2ebe70 + (*neighbours & 0x7fff) * 0x100;
                    if (candidate != current_target &&
                        (*(short *)(candidate + 0xa6) == 0x323 || *(short *)(candidate + 0xa6) == 0x2f6)) {
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
                *angle = rope_spring_turn(turn, *angle, fast_arc_tan(offset[0], offset[1]), 0.00125f, 0.175f, 0.0f);
            } else if (*(int *)(follow + 0x40) == 1)
                *angle = rope_spring_turn(turn, *angle, *(float *)(*(char **)(follow + 0x44) + 0x48), 0.00125f, 0.175f, 0.0f);
            else
                *angle = rope_spring_turn(turn, *angle, hero.motion.rot.f[2], 0.00125f, 0.175f, 0.0f);
        }
    } else if (hero.state.current == 0x25) {
        if (*(int *)(follow + 0x40) == 1)
            *angle = rope_spring_turn(angle + 1, *angle, *(float *)(*(char **)(follow + 0x44) + 0x48), D_L00_00161DD8_2ebe70, 0.175f, 0.0f);
        else
            *angle = rope_spring_turn(angle + 1, *angle, hero.motion.rot.f[2], D_L00_00161DD8_2ebe70, 0.175f, 0.0f);
    }
    *(float *)(follow + 0x30) = fast_cos(*angle);
    *(float *)(follow + 0x34) = fast_sin(*angle);
    *(int *)(follow + 0x38) = 0;
    *(float *)(follow + 0x3c) = *(float *)(follow + 0x38);
    FUN_001ff500_2ebe70(-*distance, position, follow + 0x30);
    position[2] += distance[4];
    fast_vec_add(position, position, &hero.motion.pos);
    if (coll_line_fix(camera_object + 0x30, position, 0x12, 0, 0) != 0) {
        fast_vec_sub(offset, &D_L00_00173E60_2ebe70, camera_object + 0x30);
        collision_position = &D_L00_00173E60_2ebe70;
        length = fast_vec_length(offset);
        if (length > 0.5f) {
            FUN_001ff300_2ebe70((length - 0.5f) / length, offset, offset);
            fast_vec_add(position, camera_object + 0x30, offset);
            for (i = 0; i < 5; i++) {
                if (FUN_001f0d60_2ebe70(0.75f, position, 0, 0) != 0) qcopy(position, collision_position + 1);
                else break;
            }
        }
        qcopy(camera_object + 0x30, position);
        fast_vec_sub(offset, &hero.motion.pos, camera_object + 0x30);
        *angle = fast_arc_tan(offset[0], offset[1]);
        *(int *)(angle + 1) = 0;
    } else qcopy(camera_object + 0x30, position);
}
typedef union {
    OvlQuad q;
    float f[4];
} V_2ec4a8;
void FUN_001f9a28_2ec4a8(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_001f9bf8_2ec4a8(void *, void *, float) __asm__("FUN_001f9bf8");
float FUN_001f9ab0_2ec4a8(void *, void *) __asm__("FUN_001f9ab0");
float FUN_001f9df8_2ec4a8(float) __asm__("FUN_001f9df8");
float FUN_L00_001eb328_2ec4a8(void *, float, float, float, float,
                              float) __asm__("FUN_L00_001eb328");
void FUN_001f9ad8_2ec4a8(void *, void *, void *) __asm__("FUN_001f9ad8");
void FUN_00214890_2ec4a8(void *, void *, void *, float) __asm__("FUN_00214890");

extern u8 D_L00_00166E30_lui[] __asm__("D_L00_00166E30") __attribute__((section(".data")));
void turn_camera_view_toward_hero(unsigned char *camera_object) __asm__("FUN_L00_002ec4a8");

void turn_camera_view_toward_hero(unsigned char *camera_object) {
    V_2ec4a8 raised_hero_position;
    V_2ec4a8 target_view_direction;
    V_2ec4a8 x;
    unsigned char *hero_position = (unsigned char *)&hero.motion.pos;
    unsigned char *camera_state = *(unsigned char **)(camera_object + 0x70);
    unsigned char *follow_block = camera_state + 0x40;
    float *view_turn_state = (float *)(camera_state + 0x30);
    float view_angle_error, k;
    qcopy(&raised_hero_position, hero_position);
    raised_hero_position.f[2] += *(float *)(follow_block + 0x4C);
    FUN_001f9a28_2ec4a8(&target_view_direction, &raised_hero_position, camera_object + 0x30);
    FUN_001f9bf8_2ec4a8(&target_view_direction, &target_view_direction, 1.0f);
    view_angle_error = 1.5707964f - FUN_001f9df8_2ec4a8(FUN_001f9ab0_2ec4a8(&target_view_direction, camera_object));
    if (*(int *)(hero_position + 0x2004) == 0x25) {
        *(float *)(camera_state + 0x30) += 0.017f;
        if (1.0f < *(float *)(camera_state + 0x30))
            *(float *)(camera_state + 0x30) = 1.0f;
        k = view_angle_error * *(float *)(camera_state + 0x30);
    } else {
        k = FUN_L00_001eb328_2ec4a8(view_turn_state, 0.0f, view_angle_error, 0.02f, 0.175f, 0.0f);
    }
    FUN_001f9ad8_2ec4a8(&x, &target_view_direction, camera_object + 0x40);
    FUN_00214890_2ec4a8(camera_object, camera_object + 0x40, &x, k);
    FUN_001f9bf8_2ec4a8(camera_object + 0x40, camera_object, 1.0f);
    qcopy(camera_object, camera_object + 0x40);
    FUN_001f9ad8_2ec4a8(camera_object + 0x10, camera_object, D_L00_00166E30_lui);
    FUN_001f9bf8_2ec4a8(camera_object + 0x10, camera_object + 0x10, 1.0f);
    FUN_001f9ad8_2ec4a8(camera_object + 0x20, camera_object + 0x10, camera_object);
}
/* camera update: copies the moby's matrix when the camera mode changes */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EDB70), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void initialize_target_tracking_camera(void *) __asm__("FUN_L00_002ebbd8");
void refresh_point_light(s32 arg0) __asm__("FUN_00201f58");
extern char D_L00_00166D80_c[] __asm__("D_L00_00166C80");

void update_target_tracking_camera(char *camera_object) __asm__("FUN_L00_002ec6c0");

void update_target_tracking_camera(char *camera_object) {
    struct Hero *b = &hero;
    char *camera_target_state = *(char **)(camera_object + 0x70) + 0xB8;
    int hero_state = b->state.current;
    char *camera_position;
    int x, y;
    if (hero_state == 0x2C) {
        camera_position = camera_object + 0x30;
        x = *(int *)(camera_target_state + 8);
        y = ((int)b->unk994);
    } else {
        camera_position = camera_object + 0x30;
        if ((unsigned)(hero_state - 0x24) >= 3)
            goto done;
        x = *(int *)(camera_target_state + 8);
        y = ((int)b->unk964);
    }
    if (x != y) {
        char *g = D_L00_00166D80_c;
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
#define NOT_SDA

#define MACRO_ADDR

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
/*
 * Camera volumes. Each camera record (UpdateCam) has a descriptor in
 * camera_descriptors (UpdateCam.descriptor_index); the descriptor points at
 * the parameters of its camera type. Two types are handled here:
 *  - swing cameras: volume_camera_swing_check -> volume_camera_swing;
 *  - aim cameras: volume_camera_check -> volume_camera_aim.
 * Both run while the hero stands in one of the volumes the parameters name
 * (clip volumes and the FUN_L00_0025fa38 / FUN_L00_0025faf0 / path tests),
 * then end the camera with FUN_L00_002e8388 and set `done`.
 */
#include "rnc/gameplay/camera/follow_cam.h"
#include "rnc/input/pad_state.h"
#include "rnc/math/fast_math.h"

typedef float CameraVec[4] __attribute__((aligned(16)));

/* Parameters of a swing camera. */
typedef struct {
    f32 swing;                   /* 0x00: degrees swung around `yaw` over the first 200 frames */
    u8 pad04[4];
    s32 volume_25faf0;           /* 0x08: FUN_L00_0025faf0 volume, -1 none */
    s32 clip_volume;             /* 0x0C: is_point_inside_clip_volume id, -1 none */
    s32 volume_25fa38;           /* 0x10: FUN_L00_0025fa38 volume, -1 none */
    s32 path;                    /* 0x14: point_in_path_xy path index, -1 none */
    u8 pad18[8];
    f32 pitch;                   /* 0x20: degrees, second angle for FUN_L00_002e8918 */
    f32 distance;                /* 0x24: follow_camera_set_distance */
    f32 height;                  /* 0x28: follow_camera_set_height */
    s16 type;                    /* 0x2C: 1..11, picks timing and easing */
    s16 timer;                   /* 0x2E: frames since the camera started, 0 when off */
    f32 offset_scale;            /* 0x30: follow_camera_set_offset_scale; follow.look_height on a type 5 snap */
    s16 keep_unk10;              /* 0x34: 0 clears FollowCamState.unk10 each frame */
    s16 check_yaw;               /* 0x36: nonzero ends the camera when D_L00_00166DD8 is 80 degrees or more off `yaw` */
    f32 lateral_stiffness;       /* 0x38: eased in over 120 frames from start_lateral_stiffness */
    f32 lateral_damping;         /* 0x3C: likewise from start_lateral_damping */
    f32 start_lateral_stiffness; /* 0x40: follow.lateral_stiffness of the player camera when the camera started */
    f32 start_lateral_damping;   /* 0x44 */
    s16 l_buttons_end;           /* 0x48: type 5 ends on L1 or L2; with `done` set it does not restart */
    s16 done;                    /* 0x4A: set when the camera ends */
    s32 clip_volume2;            /* 0x4C: second clip volume, -1 none */
    f32 unk50;                   /* 0x50: degrees, passed to FUN_L00_002e8540 */
    s16 set_flag1;               /* 0x54: sets tail.ctl.flags bit 1 each frame */
    s16 set_flag0;               /* 0x56: sets tail.ctl.flags bit 0 each frame */
} SwingCameraParams;

/* Parameters of an aim camera: it looks from the descriptor's position at the hero. */
typedef struct {
    u8 pad00[8];
    s32 volume_25faf0;           /* 0x08: FUN_L00_0025faf0 volume, -1 none */
    s32 clip_volume;             /* 0x0C: is_point_inside_clip_volume id, -1 none */
    s32 volume_25fa38;           /* 0x10: FUN_L00_0025fa38 volume, -1 none */
    u8 pad14[0xC];
    s32 timer;                   /* 0x20: frames since the camera started (capped at 200), 0 when off */
    s16 keep_unk10;              /* 0x24: 0 clears FollowCamState.unk10 each frame */
    s16 done;                    /* 0x26: set when the camera ends */
    f32 unk28;                   /* 0x28: degrees, passed to FUN_L00_002e8540 */
    f32 angle;                   /* 0x2C: radians; with height and distance gives offset_scale */
    u8 pad30[4];
    s32 need_still;              /* 0x34: start only while tail.unk164 and tail.unk1B0 are (nearly) 0 */
    f32 distance;                /* 0x38: horizontal distance from the hero to the position */
    f32 height;                  /* 0x3C: height of the hero below the position */
    f32 offset_scale;            /* 0x40 */
    s16 not_in_mode_1;           /* 0x44: nonzero blocks the camera while D_001413F4 is 1 */
    s16 only_in_mode_1;          /* 0x46: nonzero allows it only while D_001413F4 is 1 */
} AimCameraParams;

typedef struct {
    f32 pos[3];                  /* 0x00: aim cameras look from here */
    u8 pad0C[0xC];
    f32 yaw;                     /* 0x18: swing cameras turn around this */
    void *params;                /* 0x1C: SwingCameraParams or AimCameraParams */
} CameraDescriptor;

extern CameraDescriptor *camera_descriptors __asm__("D_L00_0015EF50");
/* controller_state.held (the label sits inside struct PadState). */
extern s32 pad_held __asm__("D_0013CAE0") __attribute__((section(".data")));
extern f32 D_L00_00166DD8 __asm__("D_L00_00166DD8") __attribute__((section(".data")));
extern f32 D_L00_00166EB4 __asm__("D_L00_00166EB4") __attribute__((section(".data")));
extern u8 D_001413F4 __asm__("D_001413F4") __attribute__((section(".data")));
extern s32 D_001413DC __asm__("D_001413DC") __attribute__((section(".data")));
extern s32 D_001415D4[];
extern u8 *D_L00_001B04B0[];

extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern s32 test_point_in_transformed_unit_cylinder(void *, s32) __asm__("FUN_L00_0025fa38");
extern s32 test_point_in_transformed_unit_sphere(void *, s32) __asm__("FUN_L00_0025faf0");
extern s32 point_in_path_xy(void *, void *, s32) __asm__("FUN_L00_00259740");
extern s32 try_select_follow_camera_volume(struct UpdateCam *) __asm__("FUN_L00_002e83c0");
extern void clear_follow_camera_volume_if_current(struct UpdateCam *) __asm__("FUN_L00_002e8388");
extern void fun_l00_002e8490(f32) __asm__("FUN_L00_002e8490");
extern void fun_l00_002e8540(f32) __asm__("FUN_L00_002e8540");
extern void fun_l00_002e8568(s32) __asm__("FUN_L00_002e8568");
extern void fun_l00_002e8620(void) __asm__("FUN_L00_002e8620");
extern void set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");
extern void fun_l00_002e8918(void *, f32, f32) __asm__("FUN_L00_002e8918");

/* Nonzero while the hero is in one of the swing camera's volumes. */
s32 volume_camera_swing_active(struct UpdateCam *cam) __asm__("FUN_L00_002ec7e8");
s32 volume_camera_swing_active(struct UpdateCam *swing_camera_record) {
    SwingCameraParams *params;
    s32 forced = 0;
    u16 type;

    params = camera_descriptors[swing_camera_record->descriptor_index].params;
    if (current_follow_cam->unk86 != 0) {
        return 0;
    }
    type = *(u16 *)&params->type;
    if (params->type == 2 && D_001413DC == 0x1A && params->timer > 0) {
        forced = 1;
    } else if ((s16)type == 4 && D_001415D4[0] == 3) {
        forced = 0 < params->timer;
    }
    if (params->clip_volume >= 0 && (forced || is_point_inside_clip_volume(&hero.motion.pos, params->clip_volume)))
        return 1;
    if (params->clip_volume2 >= 0 && (forced || is_point_inside_clip_volume(&hero.motion.pos, params->clip_volume2)))
        return 1;
    if (params->volume_25fa38 >= 0 && (forced || test_point_in_transformed_unit_cylinder(&hero.motion.pos, params->volume_25fa38)))
        return 1;
    if (params->volume_25faf0 >= 0 && (forced || test_point_in_transformed_unit_sphere(&hero.motion.pos, params->volume_25faf0)))
        return 1;
    if (params->path >= 0 && (forced || point_in_path_xy(&hero.motion.pos, D_L00_001B04B0[params->path] + 0x10,
                                                         *(s32 *)D_L00_001B04B0[params->path])))
        return 1;
    return 0;
}

/* Runs a swing camera for one frame: by its type, sets the follow camera's
   distance, height, offset scale and springs, and swings it around the
   descriptor's yaw over `timer`. */
void volume_camera_swing(struct UpdateCam *cam) __asm__("FUN_L00_002ec988");
void volume_camera_swing(struct UpdateCam *swing_camera_record) {
    CameraVec gravity_offset_or_yaw_direction;
    SwingCameraParams *params;
    struct UpdateCam *rec;
    Vec4f *pos;
    FollowCamState *rig;
    FollowCamState *state;
    FollowCamBlock *follow;
    FollowCamTail *tail;
    FollowCamSnap *snap;
    FollowCamReset *reset;
    s32 at_end;
    f32 total;
    f32 swing;
    f32 lateral_spring_blend_fraction;

    params = camera_descriptors[swing_camera_record->descriptor_index].params;
    if (params->type == 8 && FOLLOW_CAM_STATE(current_follow_cam)->tail.mode == 0) {
        return;
    }
    if (params->type == 6 && FOLLOW_CAM_STATE(current_follow_cam)->tail.mode != 0) {
        return;
    }
    if (volume_camera_swing_active(swing_camera_record) == 0) {
        goto stop;
    }
    if (try_select_follow_camera_volume(swing_camera_record) <= 0) {
        if (params->timer != 0) {
            params->done = 1;
        }
        params->timer = 0;
        return;
    }
    if (params->check_yaw != 0
        && fast_difference_between_rotations(camera_descriptors[swing_camera_record->descriptor_index].yaw, D_L00_00166DD8) >= 1.3962634f) {
        goto stop;
    }
    if (params->l_buttons_end != 0 && params->type == 5 && (pad_held & (PAD_L2 | PAD_L1))) {
stop:
        if (params->timer != 0) {
            params->done = 1;
        }
        params->timer = 0;
        clear_follow_camera_volume_if_current(swing_camera_record);
        return;
    }
    if (params->timer == 0) {
        u8 *camera_globals = (u8 *)&D_L00_00166C80;
        params->start_lateral_stiffness = FOLLOW_CAM_STATE(*(struct UpdateCam **)(camera_globals + 0x180))->follow.lateral_stiffness;
        params->start_lateral_damping = FOLLOW_CAM_STATE(*(struct UpdateCam **)(camera_globals + 0x180))->follow.lateral_damping;
    }
    /* One word over type and timer: type 5 on its first frame snaps the camera. */
    if (*(s32 *)&params->type == 5) {
        *(s32 *)&params->swing = 0;
        rec = current_follow_cam;
        rig = FOLLOW_CAM_STATE(rec);
        follow = &rig->follow;
        tail = &rig->tail;
        snap = &rig->tail.snap;
        /* A second pointer to the same state: retail keeps both. */
        state = rig;
        reset = &state->reset;
        fast_vec_normalize(tail->unk130, tail->unk130, params->distance);
        tail->unk15C = params->distance;
        tail->distance = params->height;
        follow->look_height = params->offset_scale;
        qcopy(state->tail.unk140, tail->unk130);
        fast_vec_scale(gravity_offset_or_yaw_direction, &hero.gravity_dir, -tail->distance);
        pos = &rec->pos;
        fast_vec_add(state->follow.eye, &hero.motion.pos, gravity_offset_or_yaw_direction);
        fast_vec_add(pos, state->follow.eye, tail->unk130);
        fast_vec_scale(gravity_offset_or_yaw_direction, &hero.gravity_dir, -follow->look_height);
        fast_vec_add(state->follow.look_target, &hero.motion.pos, gravity_offset_or_yaw_direction);
        qcopy(state->follow.unkD0, state->follow.look_target);
        qcopy(follow->focus, &hero.motion.pos);
        qcopy(state->follow.offset, follow->focus);
        qcopy(state->position, pos);
        qcopy(state->tail.snap.unk1F0, pos);
        snap->unk200 = tail->unk15C - fast_vec_length(tail->unk130);
        *(s32 *)&reset->unk34 = 0;
        reset->look_height = follow->look_height;
        { f32 zero = reset->unk34;
        f32 distance = tail->distance;
        reset->distance = distance;
        reset->unk38 = zero;
        reset->unk30 = zero;
        reset->unk2C = zero; }
    }
    at_end = 0;
    total = convert_integer_to_float(scale_game_frames(200));
    params->timer++;
    if (params->type == 10) {
        set_follow_camera_blend_output_flags();
        FOLLOW_CAM_STATE(current_follow_cam)->tail.mode = 0x14D;
    }
    if (params->type == 11) {
        set_follow_camera_blend_output_flags();
        FOLLOW_CAM_STATE(current_follow_cam)->tail.mode = 2;
    }
    if (params->type == 2 || params->type == 4) {
        { FollowCamFlags *ctl = &FOLLOW_CAM_STATE(current_follow_cam)->tail.ctl; ctl->flags |= 3; }
        if (params->timer > scale_game_frames(200)) {
            at_end = 1;
            params->timer = scale_game_frames(200);
        }
        fun_l00_002e8620();
    } else if (params->type == 1 || params->type == 7 || params->type == 10 || params->type == 9) {
        /* Moving the left stick skips to the end of the swing. */
        struct PadState *pad = &controller_state;
        if (pad->analog[0] != 0.0f || pad->analog[1] != 0.0f) {
            params->timer = scale_game_frames(200);
        }
        if (params->timer >= scale_game_frames(200)) {
            at_end = 1;
            if (D_L00_00166EB4 > 0.0f) {
                params->timer = scale_game_frames(200);
            }
        }
        if (params->timer >= scale_game_frames(400)) {
            at_end = 0;
            params->timer = 1;
        }
    } else if (params->type == 3) {
        total = convert_integer_to_float(scale_game_frames(1));
        if (params->timer > scale_game_frames(1)) {
            at_end = 1;
            params->timer = scale_game_frames(1);
        }
    } else {
        if (params->timer > scale_game_frames(200)) {
            at_end = 1;
            params->timer = scale_game_frames(200);
        }
    }
    gravity_offset_or_yaw_direction[0] = fast_cos(camera_descriptors[swing_camera_record->descriptor_index].yaw);
    gravity_offset_or_yaw_direction[1] = fast_sin(camera_descriptors[swing_camera_record->descriptor_index].yaw);
    *(s32 *)&gravity_offset_or_yaw_direction[2] = 0;
    swing = params->swing * 0.017453292f * (params->timer / total);
    if ((params->type != 1 && params->type != 7 && params->type != 9) || at_end == 0) {
        fun_l00_002e8918(gravity_offset_or_yaw_direction, swing, params->pitch * 0.017453292f);
    }
    if (params->distance != 0.0f) {
        if (params->type == 6 || params->type == 8) {
            follow_camera_set_distance(0, params->distance, 0.002f);
            fun_l00_002e8490(params->distance + 1.36f);
        } else {
            follow_camera_set_distance(0, params->distance, 0.003f);
            fun_l00_002e8620();
        }
    }
    if (params->height != 0.0f) {
        if (params->type == 6 || params->type == 8) {
            follow_camera_set_height(params->height, 0.002f);
        } else {
            follow_camera_set_height(params->height, 0.003f);
        }
    }
    if (params->offset_scale != 0.0f) {
        if (params->type == 6 || params->type == 8) {
            follow_camera_set_offset_scale(0, params->offset_scale, 0.002f);
        } else {
            follow_camera_set_offset_scale(0, params->offset_scale, 0.005f);
        }
    }
    if (params->unk50 != 0.0f) {
        fun_l00_002e8540(params->unk50 * 0.017453292f);
    }
    if (params->set_flag1 != 0) {
        { FollowCamFlags *ctl = &FOLLOW_CAM_STATE(current_follow_cam)->tail.ctl; ctl->flags |= 2; }
    }
    if (params->set_flag0 != 0) {
        { FollowCamFlags *ctl = &FOLLOW_CAM_STATE(current_follow_cam)->tail.ctl; ctl->flags |= 1; }
    }
    if (params->keep_unk10 == 0) {
        fun_l00_002e8568(0);
    }
    if (params->lateral_stiffness != 0.0f || params->lateral_damping != 0.0f) {
        if (params->timer >= scale_game_frames(120)) {
            follow_camera_set_lateral_spring(params->lateral_stiffness, params->lateral_damping);
        } else {
            lateral_spring_blend_fraction = params->timer / convert_integer_to_float(scale_game_frames(120));
            follow_camera_set_lateral_spring(
                params->start_lateral_stiffness + (params->lateral_stiffness - params->start_lateral_stiffness) * lateral_spring_blend_fraction,
                params->start_lateral_damping + (params->lateral_damping - params->start_lateral_damping) * lateral_spring_blend_fraction);
        }
    }
    if (params->type == 1 || params->type == 7 || params->type == 10) {
        fun_l00_002e8620();
        follow_camera_start_slide();
        follow_camera_set_up_spring(0.01f, 0.2f);
    }
}

/* Runs volume_camera_swing unless its type is blocked right now (by
   D_001413F4, D_001413DC, or the hero being off the ground). */
s32 volume_camera_swing_check(struct UpdateCam *cam) __asm__("FUN_L00_002ed230");
s32 volume_camera_swing_check(struct UpdateCam *swing_camera_record) {
    SwingCameraParams *params = camera_descriptors[swing_camera_record->descriptor_index].params;
    if (params->l_buttons_end == 1 && params->done != 0)
        return 0;
    if (params->type == 11 && D_001413F4 != 2)
        return 0;
    if ((params->type == 1 || params->type == 10) && params->timer == 0) {
        struct Hero *hero_data = &hero;
        if (hero_data->frames_off_ground.s != 0) {
            if (current_level_index != 14)
                return 0;
            if (hero_data->unk2284 != 13)
                return 0;
        }
    }
    if (params->type == 2 && D_001413DC != 0x1a) {
        params->timer = 0;
        return 0;
    }
    volume_camera_swing(swing_camera_record);
    return 0;
}

/* Nonzero while the hero is in one of the aim camera's volumes; sets `done` otherwise. */
s32 volume_camera_aim_active(struct UpdateCam *cam) __asm__("FUN_L00_002ed348");
s32 volume_camera_aim_active(struct UpdateCam *aim_camera_record) {
    struct UpdateCam *rec = current_follow_cam;
    AimCameraParams *params = camera_descriptors[aim_camera_record->descriptor_index].params;
    FollowCamState *state = FOLLOW_CAM_STATE(rec);
    FollowCamTail *tail = &state->tail;
    FollowCamFlags *ctl = &state->tail.ctl;
    if (rec->unk86 != 0)
        return 0;
    if (params->only_in_mode_1 != 0 && D_001413F4 != 1)
        return 0;
    if (params->not_in_mode_1 != 0 && D_001413F4 == 1)
        return 0;
    if (params->need_still != 0) {
        if (0.01f < absolute_float(tail->unk164))
            return 0;
        if (0.01f < absolute_float(ctl->unk1B0))
            return 0;
    }
    if (params->clip_volume >= 0 && is_point_inside_clip_volume(&hero.motion.pos, params->clip_volume))
        return 1;
    if (params->volume_25fa38 >= 0 && test_point_in_transformed_unit_cylinder(&hero.motion.pos, params->volume_25fa38))
        return 1;
    if (params->volume_25faf0 >= 0 && test_point_in_transformed_unit_sphere(&hero.motion.pos, params->volume_25faf0))
        return 1;
    params->done = 1;
    return 0;
}

/* Runs an aim camera for one frame: on its first two frames places the
   camera at the descriptor's position and fits distance, height and offset
   scale to the hero; then eases them each frame. */
void volume_camera_aim(struct UpdateCam *cam) __asm__("FUN_L00_002ed498");
void volume_camera_aim(struct UpdateCam *aim_camera_record) {
    CameraVec pos;
    CameraVec to_pos;
    CameraVec gravity_offset;
    CameraVec level;
    AimCameraParams *params;
    struct UpdateCam *rec;
    Vec4f *rec_pos;
    FollowCamState *rig;
    FollowCamState *state;
    FollowCamBlock *follow;
    FollowCamTail *tail;
    FollowCamSnap *snap;
    FollowCamReset *reset;
    f32 up;
    f32 angle;

    params = camera_descriptors[aim_camera_record->descriptor_index].params;
    if (volume_camera_aim_active(aim_camera_record)) {
        if (try_select_follow_camera_volume(aim_camera_record) <= 0) {
            if (params->timer) {
                params->done = 1;
            }
            params->timer = 0;
            return;
        }
        if (!(pad_held & (PAD_L2 | PAD_L1))) {
            goto run;
        }
    }
    if (params->timer) {
        params->done = 1;
    }
    params->timer = 0;
    clear_follow_camera_volume_if_current(aim_camera_record);
    return;
run:
    if (params->timer < 2) {
        {
            pos[0] = camera_descriptors[aim_camera_record->descriptor_index].pos[0];
            pos[1] = camera_descriptors[aim_camera_record->descriptor_index].pos[1];
            pos[2] = camera_descriptors[aim_camera_record->descriptor_index].pos[2];
        }
        *(s32 *)&pos[3] = 0;
        fast_vec_sub(to_pos, pos, &hero.motion.pos);
        up = fast_vec_dot(to_pos, &hero.gravity_dir);
        fast_vec_scale(gravity_offset, &hero.gravity_dir, up);
        fast_vec_sub(level, to_pos, gravity_offset);
        params->distance = fast_vec_length(level);
        params->height = -up;
        angle = params->angle;
        if (absolute_float(angle) == 1.5707964f) {
            *(s32 *)&params->offset_scale = 0;
        } else {
            params->offset_scale = params->height - params->distance * (fast_sin(angle) / fast_cos(angle));
        }
        rec = current_follow_cam;
        rig = FOLLOW_CAM_STATE(rec);
        rec_pos = &rec->pos;
        follow = &rig->follow;
        tail = &rig->tail;
        snap = &rig->tail.snap;
        /* A second pointer to the same state: retail keeps both. */
        state = rig;
        reset = &state->reset;
        qcopy(rec_pos, pos);
        tail->unk15C = params->distance;
        tail->distance = params->height;
        follow->look_height = params->offset_scale;
        fast_vec_scale(gravity_offset, &hero.gravity_dir, -tail->distance);
        fast_vec_add(state->follow.eye, &hero.motion.pos, gravity_offset);
        fast_vec_sub(tail->unk130, pos, state->follow.eye);
        qcopy(state->tail.unk140, tail->unk130);
        fast_vec_scale(gravity_offset, &hero.gravity_dir, -follow->look_height);
        fast_vec_add(state->follow.look_target, &hero.motion.pos, gravity_offset);
        qcopy(state->follow.unkD0, state->follow.look_target);
        qcopy(follow->focus, &hero.motion.pos);
        qcopy(state->follow.offset, follow->focus);
        qcopy(state->position, rec_pos);
        qcopy(state->tail.snap.unk1F0, rec_pos);
        *(s32 *)&snap->unk200 = 0;
        *(s32 *)&reset->unk34 = 0;
        reset->look_height = follow->look_height;
        { f32 zero = reset->unk34;
        f32 distance = tail->distance;
        reset->distance = distance;
        reset->unk22 = 0;
        reset->unk38 = zero;
        reset->unk30 = zero;
        reset->unk2C = zero; }
    }
    params->timer += 1;
    if (scale_game_frames(200) < params->timer) {
        params->timer = scale_game_frames(200);
    }
    if (params->distance != 0.0f) {
        follow_camera_set_distance(0, params->distance, 0.003f);
        fun_l00_002e8620();
    }
    if (params->height != 0.0f) {
        follow_camera_set_height(params->height, 0.003f);
    }
    if (params->offset_scale != 0.0f) {
        follow_camera_set_offset_scale(0, params->offset_scale, 0.005f);
    }
    if (params->unk28 != 0.0f) {
        fun_l00_002e8540(params->unk28 * 0.017453292f);
    }
    if (params->keep_unk10 == 0) {
        fun_l00_002e8568(0);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EED60), where it is exact; names translated to the US level program. */


/* Runs volume_camera_aim unless the camera is done. While D_L00_0015F5C4 is
   set and the hero is in the volumes, resets the camera instead. */
s32 volume_camera_check(struct UpdateCam *cam) __asm__("FUN_L00_002ed8b0");
s32 volume_camera_check(struct UpdateCam *aim_camera_record) {
    AimCameraParams *params = camera_descriptors[aim_camera_record->descriptor_index].params;
    if (params->done != 0)
        return 0;
    if (D_L00_0015F5C4 != 0) {
        if (volume_camera_aim_active(aim_camera_record) != 0) {
            params->done = 0;
            params->timer = 0;
            return 0;
        }
    }
    volume_camera_aim(aim_camera_record);
    return 0;
}
typedef struct {
    s32 id;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 snd;
} S_2ed950;
typedef struct {
    u8 p[0x74];
    u8 b74;
    u8 p2[0x8C - 0x75];
    void *o;
} E_2ed950;
#define ENT_2ed950(i) ((E_2ed950 *)((u8 *)D_0013E550_2ed950 + (i) * 0x70))
typedef struct {
    u8 p[0x18];
    u8 b18;
    u8 p2[7];
} T_2ed950;
extern u8 D_L00_00166DC0_2ed950[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern E_2ed950 D_0013E550_2ed950[] __asm__("D_0013E550") __attribute__((section(".data")));
extern T_2ed950 *D_L00_0015F5F4_2ed950 __asm__("D_L00_0015F5F4");
f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
s32 FUN_001f9740_2ed950(s32 *) __asm__("FUN_001f9740");
s32 FUN_L00_0028ddd8_2ed950(s32, s32, void *, s32) __asm__("FUN_L00_0028ddd8");
s32 FUN_00213260_2ed950(s32) __asm__("FUN_00213260");
s32 FUN_001f96f8_2ed950(s32) __asm__("FUN_001f96f8");
void release_voice_slot(s32) __asm__("FUN_0022d798");
void update_sound_emitter_playback_by_distance(u8 *sound_emitter) __asm__("FUN_L00_002ed950");

void update_sound_emitter_playback_by_distance(u8 *sound_emitter) {
    S_2ed950 *playback_state = *(S_2ed950 **)(sound_emitter + 8);
    f32 reference_distance, playback_radius;
    if (playback_state->id < 0)
        return;
    reference_distance = distance_xyz(D_L00_00166DC0_2ed950, sound_emitter + 0x40);
    playback_radius = *(f32 *)(sound_emitter + 0xC);
    if (reference_distance < playback_radius) {
        E_2ed950 *e = ENT_2ed950(playback_state->snd);
        s32 fl = 0;
        s32 four = 4;
        if (D_L00_0015F5F4_2ed950[playback_state->id].b18)
            fl = four;
        if (e->o == sound_emitter && e->b74)
            return;
        if (!FUN_001f9740_2ed950(&playback_state->wC))
            return;
        playback_state->snd = FUN_L00_0028ddd8_2ed950(playback_state->id, fl, sound_emitter, 0x400);
        if (playback_state->w8 > 0) {
            playback_state->wC = FUN_001f96f8_2ed950(playback_state->w4 + FUN_00213260_2ed950(playback_state->w8 - playback_state->w4)) * 60.0f;
        }
    } else if (playback_radius + 1.0f < reference_distance) {
        s32 voice_slot_index = playback_state->snd;
        if (voice_slot_index != -1) {
            E_2ed950 *e = ENT_2ed950(voice_slot_index);
            if (e->o == sound_emitter && e->b74)
                release_voice_slot(voice_slot_index);
        }
        playback_state->snd = -1;
    }
}
typedef struct {
    s32 snd;
    s32 lo;
    s32 hi;
    s32 timer;
    s32 h;
} S_2edad8;
typedef struct {
    f32 f[4];
} V_2edad8;
extern u8 D_L00_00166DC0_2edad8[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern u8 D_0013E550_2edad8[] __asm__("D_0013E550");
extern void sub_2edad8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_2edad8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void mul_2edad8(void *, void *, void *) __asm__("FUN_001f9cf8");
extern f32 fabs_2edad8(f32) __asm__("FUN_001f99c0");
extern s32 tick_2edad8(void *) __asm__("FUN_001f9740");
extern s32 play_2edad8(s32, s32, void *, s32) __asm__("FUN_L00_0028ddd8");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 irand_2edad8(s32) __asm__("FUN_00213260");
extern s32 scale_2edad8(s32) __asm__("FUN_001f96f8");
void update_sound_emitter_playback_at_random_position(u8 *sound_emitter) __asm__("FUN_L00_002edad8");

void update_sound_emitter_playback_at_random_position(u8 *sound_emitter) {
    V_2edad8 reference_position_delta;
    V_2edad8 uniform_range_vector;
    V_2edad8 transformed_reference_position_delta;
    V_2edad8 b;
    V_2edad8 r;
    S_2edad8 *playback_state;
    f32 f;
    u8 *sound_slot_entry;
    playback_state = *(S_2edad8 **)(sound_emitter + 8);
    {
        f32 g = *(f32 *)(sound_emitter + 0xC);
        uniform_range_vector.f[0] = g;
        uniform_range_vector.f[1] = g;
        uniform_range_vector.f[2] = g;
    }
    sub_2edad8(&reference_position_delta, D_L00_00166DC0_2edad8, sound_emitter + 0x40);
    reference_position_delta.f[3] = 0;
    mul_2edad8(&transformed_reference_position_delta, &reference_position_delta, sound_emitter + 0x50);
    mul_2edad8(&b, &uniform_range_vector, sound_emitter + 0x50);
    f = fabs_2edad8(transformed_reference_position_delta.f[0]);
    if (!(f <= fabs_2edad8(b.f[0]) + 1.0f))
        return;
    f = fabs_2edad8(transformed_reference_position_delta.f[1]);
    if (!(f <= fabs_2edad8(b.f[1]) + 1.0f))
        return;
    f = fabs_2edad8(transformed_reference_position_delta.f[2]);
    if (!(f <= fabs_2edad8(b.f[2]) + 1.0f))
        return;
    sound_slot_entry = D_0013E550_2edad8 + playback_state->h * 0x70;
    if (*(u8 **)(sound_slot_entry + 0x8C) == sound_emitter && sound_slot_entry[0x74] != 0)
        return;
    if (tick_2edad8(&playback_state->timer) != 0) {
        playback_state->h = play_2edad8(playback_state->snd, 0, sound_emitter, 0x400);
        if (playback_state->h != -1) {
            r.f[0] = random_float_between(-1.0f, 1.0f);
            r.f[1] = random_float_between(-1.0f, 1.0f);
            r.f[2] = random_float_between(-1.0f, 1.0f);
            mul_2edad8(&r, &r, sound_emitter + 0x10);
            add_2edad8(&r, &r, sound_emitter + 0x40);
            qcopy(D_0013E550_2edad8 + 0x90 + playback_state->h * 0x70, &r);
        }
        if (playback_state->hi > 0) {
            playback_state->timer = (f32)scale_2edad8(playback_state->lo + irand_2edad8(playback_state->hi - playback_state->lo)) * 60.0f;
        }
    } else {
        playback_state->h = -1;
    }
}
