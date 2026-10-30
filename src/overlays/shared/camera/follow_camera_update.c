/* Camera type 0, the follow camera: builds its view, pulls back and orbits behind the hero. */

#include "types.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"

typedef struct {
    char  p0[0x80];
    float v80[4];
    char  p1[0xA4 - 0x90];
    float fA4;
} C_2e9518;

typedef struct {
    short h0, h2, h4, h6;
    float f8, fC;
} S_2e9518;

typedef struct {
    int pad[4];
    int type;
    int idx;
    int r[6];
} E_9aa0;

f32         FUN_001f9ab0_2e9010(void *, void *) __asm__("FUN_001f9ab0");
void        FUN_001f9a68_2e9010(void *, void *, f32) __asm__("FUN_001f9a68");
void        FUN_001f9bf8_2e9010(void *, void *, f32) __asm__("FUN_001f9bf8");
f32         FUN_001f9af0_2e9010(void *) __asm__("FUN_001f9af0");
f32         cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
f32         rope_spring_turn(void *, f32, f32, f32, f32, f32) __asm__("FUN_L00_001eb328");
s32         FUN_001f96f8_2e9010(s32) __asm__("FUN_001f96f8");
extern void follow_camera_set_distance(int, float, float) __asm__("FUN_L00_002e8450");
extern void subtract_vector_xyz(float *, float *, float *) __asm__("FUN_001f9a28");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern float FUN_001f9af0_9aa0(void *) __asm__("FUN_001f9af0");
extern void  noop_main_callback() __asm__("FUN_001e93e8");
extern void  follow_camera_avoid_mobies(int) __asm__("FUN_L00_002e7d20");
extern void  run_follow_camera_noop(int) __asm__("FUN_L00_002e8378");
extern void  follow_camera_read_stick(int, float *, float *) __asm__("FUN_L00_002e89b0");
extern void  follow_camera_turn_and_pitch(int, float, float) __asm__("FUN_L00_002e8bb8");
extern void  follow_camera_auto_turn(int) __asm__("FUN_L00_002e8f40");
void         apply_follow_camera_translation() __asm__("FUN_L00_002e46b8");
void         follow_camera_apply_overrides() __asm__("FUN_L00_002e6000");
void         follow_camera_springs() __asm__("FUN_L00_002e5830");
void         follow_camera_approach() __asm__("FUN_L00_002e5e38");

extern u8            camera_view[] __asm__("D_L00_00166E10");
extern u8            D_0013F3D0_2e9010[] __asm__("D_0013F3D0");
extern s32           D_L00_00161D24_gp __asm__("D_L00_00161D24") __attribute__((sda));
extern u8            D_L00_00166E30[];
extern C_2e9518      D_L00_00166E10_lui __asm__("D_L00_00166E10") NOT_SDA;
extern char         *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern unsigned char D_001413F4_lui __asm__("D_001413F4") NOT_SDA;
extern float orbit_distance_default_stiffness_gp __asm__("D_L00_00161CEC") __attribute__((sda));
extern float orbit_distance_default_damping_gp __asm__("D_L00_00161CF0") __attribute__((sda));
extern float orbit_distance_pull_back_stiffness_gp __asm__("D_L00_00161D14") __attribute__((sda));
extern float orbit_distance_pull_back_damping_gp __asm__("D_L00_00161D18") __attribute__((sda));
extern char  D_L00_00166E10[];
extern short orbit_yaw_spring_stiffness_gp __asm__("D_L00_00161CF4") __attribute__((sda));
extern short orbit_yaw_spring_damping_gp __asm__("D_L00_00161CF8") __attribute__((sda));
extern short orbit_pitch_spring_stiffness_gp __asm__("D_L00_00161CFC") __attribute__((sda));
extern short orbit_pitch_spring_damping_gp __asm__("D_L00_00161D00") __attribute__((sda));
extern int  *D_L00_0015EF80_9aa0 __asm__("D_L00_0015EF80");
extern char *D_L00_001600F4_9aa0 __asm__("D_L00_001600F4");
extern s32   opt_cam_yaw_rate __asm__("D_0015EDE4");
extern f32   follow_camera_yaw_rate_table[2] __asm__("D_L00_00161DB0") __attribute__((sda));
extern f32   D_L00_0015EF44_gp __asm__("D_L00_0015EF44") __attribute__((sda));

float D_L01_00162164 = 0.02f;
float D_L01_00162168 = 0.2f;
float D_L01_0016218C = 0.04f;
float D_L01_00162190 = 0.3f;
short D_L01_0016216C = -15729;
short D_L01_00162170 = -13107;
short D_L01_00162174 = -15729;
short D_L01_00162178 = -13107;
int  *D_L01_0015EF80 = 0;
char *D_L01_001600F4 = 0;
f32   D_L01_00162228[2] = {0.017453292f, 0.02268928f};

u8 D_L02_00167430 NOT_SDA = 0;
float             D_L02_0016229C = 0.02f;
float             D_L02_001622A0 = 0.2f;
float             D_L02_001622C4 = 0.04f;
float             D_L02_001622C8 = 0.3f;
short             D_L02_001622A4 = -15729;
short             D_L02_001622A8 = -13107;
short             D_L02_001622AC = -15729;
short             D_L02_001622B0 = -13107;
int              *D_L02_0015EF80 = 0;
char             *D_L02_001600F4 = 0;
f32               D_L02_00162360[2] = {0.017453292f, 0.02268928f};

float D_L03_00161D64 = 0.02f;
float D_L03_00161D68 = 0.2f;
float D_L03_00161D8C = 0.04f;
float D_L03_00161D90 = 0.3f;
short D_L03_00161D6C = -15729;
short D_L03_00161D70 = -13107;
short D_L03_00161D74 = -15729;
short D_L03_00161D78 = -13107;
int  *D_L03_0015EF80 = 0;
char *D_L03_001600F4 = 0;
f32   D_L03_00161E28[2] = {0.017453292f, 0.02268928f};

u8 D_L04_00166FB0 NOT_SDA = 0;
float             D_L04_00161E5C = 0.02f;
float             D_L04_00161E60 = 0.2f;
float             D_L04_00161E84 = 0.04f;
float             D_L04_00161E88 = 0.3f;
short             D_L04_00161E64 = -15729;
short             D_L04_00161E68 = -13107;
short             D_L04_00161E6C = -15729;
short             D_L04_00161E70 = -13107;
int              *D_L04_0015EF80 = 0;
char             *D_L04_001600F4 = 0;
f32               D_L04_00161F20[2] = {0.017453292f, 0.02268928f};

u8 D_L05_00167230 NOT_SDA = 0;
float             D_L05_001620B4 = 0.02f;
float             D_L05_001620B8 = 0.2f;
float             D_L05_001620DC = 0.04f;
float             D_L05_001620E0 = 0.3f;
short             D_L05_001620BC = -15729;
short             D_L05_001620C0 = -13107;
short             D_L05_001620C4 = -15729;
short             D_L05_001620C8 = -13107;
int              *D_L05_0015EF80 = 0;
char             *D_L05_001600F4 = 0;
f32               D_L05_00162178[2] = {0.017453292f, 0.02268928f};

u8 D_L06_001675B0 NOT_SDA = 0;
float             D_L06_0016247C = 0.02f;
float             D_L06_00162480 = 0.2f;
float             D_L06_001624A4 = 0.04f;
float             D_L06_001624A8 = 0.3f;
short             D_L06_00162484 = -15729;
short             D_L06_00162488 = -13107;
short             D_L06_0016248C = -15729;
short             D_L06_00162490 = -13107;
int              *D_L06_0015EF80 = 0;
char             *D_L06_001600F4 = 0;
f32               D_L06_00162540[2] = {0.017453292f, 0.02268928f};

u8 D_L07_00166EB0 NOT_SDA = 0;
float             D_L07_00161D34 = 0.02f;
float             D_L07_00161D38 = 0.2f;
float             D_L07_00161D5C = 0.04f;
float             D_L07_00161D60 = 0.3f;
short             D_L07_00161D3C = -15729;
short             D_L07_00161D40 = -13107;
short             D_L07_00161D44 = -15729;
short             D_L07_00161D48 = -13107;
int              *D_L07_0015EF80 = 0;
char             *D_L07_001600F4 = 0;
f32               D_L07_00161DF8[2] = {0.017453292f, 0.02268928f};

u8 D_L08_00167630 NOT_SDA = 0;
float             D_L08_001624F4 = 0.02f;
float             D_L08_001624F8 = 0.2f;
float             D_L08_0016251C = 0.04f;
float             D_L08_00162520 = 0.3f;
short             D_L08_001624FC = -15729;
short             D_L08_00162500 = -13107;
short             D_L08_00162504 = -15729;
short             D_L08_00162508 = -13107;
int              *D_L08_0015EF80 = 0;
char             *D_L08_001600F4 = 0;
f32               D_L08_001625B8[2] = {0.017453292f, 0.02268928f};

u8 D_L09_00166FB0 NOT_SDA = 0;
float             D_L09_00161E4C = 0.02f;
float             D_L09_00161E50 = 0.2f;
float             D_L09_00161E74 = 0.04f;
float             D_L09_00161E78 = 0.3f;
short             D_L09_00161E54 = -15729;
short             D_L09_00161E58 = -13107;
short             D_L09_00161E5C = -15729;
short             D_L09_00161E60 = -13107;
int              *D_L09_0015EF80 = 0;
char             *D_L09_001600F4 = 0;
f32               D_L09_00161F10[2] = {0.017453292f, 0.02268928f};

u8 D_L10_001672B0 NOT_SDA = 0;
float             D_L10_0016211C = 0.02f;
float             D_L10_00162120 = 0.2f;
float             D_L10_00162144 = 0.04f;
float             D_L10_00162148 = 0.3f;
short             D_L10_00162124 = -15729;
short             D_L10_00162128 = -13107;
short             D_L10_0016212C = -15729;
short             D_L10_00162130 = -13107;
int              *D_L10_0015EF80 = 0;
char             *D_L10_001600F4 = 0;
f32               D_L10_001621E0[2] = {0.017453292f, 0.02268928f};

u8 D_L11_00167830 NOT_SDA = 0;
float             D_L11_00162684 = 0.02f;
float             D_L11_00162688 = 0.2f;
float             D_L11_001626AC = 0.04f;
float             D_L11_001626B0 = 0.3f;
short             D_L11_0016268C = -15729;
short             D_L11_00162690 = -13107;
short             D_L11_00162694 = -15729;
short             D_L11_00162698 = -13107;
int              *D_L11_0015EF80 = 0;
char             *D_L11_001600F4 = 0;
f32               D_L11_00162748[2] = {0.017453292f, 0.02268928f};

u8 D_L12_001672B0 NOT_SDA = 0;
float             D_L12_0016215C = 0.02f;
float             D_L12_00162160 = 0.2f;
float             D_L12_00162184 = 0.04f;
float             D_L12_00162188 = 0.3f;
short             D_L12_00162164 = -15729;
short             D_L12_00162168 = -13107;
short             D_L12_0016216C = -15729;
short             D_L12_00162170 = -13107;
int              *D_L12_0015EF80 = 0;
char             *D_L12_001600F4 = 0;
f32               D_L12_00162220[2] = {0.017453292f, 0.02268928f};

u8 D_L13_00167130 NOT_SDA = 0;
float             D_L13_00161FD4 = 0.02f;
float             D_L13_00161FD8 = 0.2f;
float             D_L13_00161FFC = 0.04f;
float             D_L13_00162000 = 0.3f;
short             D_L13_00161FDC = -15729;
short             D_L13_00161FE0 = -13107;
short             D_L13_00161FE4 = -15729;
short             D_L13_00161FE8 = -13107;
int              *D_L13_0015EF80 = 0;
char             *D_L13_001600F4 = 0;
f32               D_L13_00162098[2] = {0.017453292f, 0.02268928f};

u8 D_L14_00167530 NOT_SDA = 0;
float             D_L14_0016238C = 0.02f;
float             D_L14_00162390 = 0.2f;
float             D_L14_001623B4 = 0.04f;
float             D_L14_001623B8 = 0.3f;
short             D_L14_00162394 = -15729;
short             D_L14_00162398 = -13107;
short             D_L14_0016239C = -15729;
short             D_L14_001623A0 = -13107;
int              *D_L14_0015EF80 = 0;
char             *D_L14_001600F4 = 0;
f32               D_L14_00162450[2] = {0.017453292f, 0.02268928f};

u8 D_L15_00167430 NOT_SDA = 0;
float             D_L15_001622DC = 0.02f;
float             D_L15_001622E0 = 0.2f;
float             D_L15_00162304 = 0.04f;
float             D_L15_00162308 = 0.3f;
short             D_L15_001622E4 = -15729;
short             D_L15_001622E8 = -13107;
short             D_L15_001622EC = -15729;
short             D_L15_001622F0 = -13107;
int              *D_L15_0015EF80 = 0;
char             *D_L15_001600F4 = 0;
f32               D_L15_001623A0[2] = {0.017453292f, 0.02268928f};

u8 D_L16_00167230 NOT_SDA = 0;
float             D_L16_001620A4 = 0.02f;
float             D_L16_001620A8 = 0.2f;
float             D_L16_001620CC = 0.04f;
float             D_L16_001620D0 = 0.3f;
short             D_L16_001620AC = -15729;
short             D_L16_001620B0 = -13107;
short             D_L16_001620B4 = -15729;
short             D_L16_001620B8 = -13107;
int              *D_L16_0015EF80 = 0;
char             *D_L16_001600F4 = 0;
f32               D_L16_00162168[2] = {0.017453292f, 0.02268928f};

u8 D_L17_00167730 NOT_SDA = 0;
float             D_L17_0016258C = 0.02f;
float             D_L17_00162590 = 0.2f;
float             D_L17_001625B4 = 0.04f;
float             D_L17_001625B8 = 0.3f;
short             D_L17_00162594 = -15729;
short             D_L17_00162598 = -13107;
short             D_L17_0016259C = -15729;
short             D_L17_001625A0 = -13107;
int              *D_L17_0015EF80 = 0;
char             *D_L17_001600F4 = 0;
f32               D_L17_00162650[2] = {0.017453292f, 0.02268928f};

u8 D_L18_00167830 NOT_SDA = 0;
float             D_L18_001626A4 = 0.02f;
float             D_L18_001626A8 = 0.2f;
float             D_L18_001626CC = 0.04f;
float             D_L18_001626D0 = 0.3f;
short             D_L18_001626AC = -15729;
short             D_L18_001626B0 = -13107;
short             D_L18_001626B4 = -15729;
short             D_L18_001626B8 = -13107;
int              *D_L18_0015EF80 = 0;
char             *D_L18_001600F4 = 0;
f32               D_L18_00162768[2] = {0.017453292f, 0.02268928f};

/* Follow-camera orientation update: rebuilds the basis vectors at p from the
 * target moby, eases pitch/yaw and renormalises. */
void follow_camera_build_view(u8 *p) __asm__("FUN_L00_002e9010");

void follow_camera_build_view(u8 *camera) {
    u8 *follow_state_block;
    u8 *camera_state;
    u8 *reloaded_camera_state;
    u8 *view_blend_and_smoothing_state;
    u8 *reference_orbit_offset;
    u8 *tb;
    u8 *base;
    u8 *side_axis;
    u8 *view_up_axis;
    u8 *side_axis_or_aim_position; /* scratch vector pointer: p + 0x10, later v4 */
    u8 *up;                        /* up vector (D_L00_00166E30), set on both paths */
    f32 projected_view_length;
    f32 smoothed_height_baseline, reference_orbit_up_component, remaining_view_blend_frames,
        view_blend_fraction;
    f32 position_up_component, adjusted_view_length, smoothed_height_difference,
        view_rotation_angle, target_view_angle_offset, x, smoothed_view_angle_offset;
    f32 vector_scratch[4];
    f32 projected_reference_position[4];
    f32 projected_camera_position[4];
    f32 view_direction[4];
    f32 adjusted_aim_position[4];

    base = camera_view;
    camera_state = *(u8 **)(camera + 0x70);
    follow_state_block = camera_state + 0x40;
    view_blend_and_smoothing_state = camera_state + 0x20;
    if (follow_state_block[0xC4] == 0xB) {
        fast_vec_sub(camera, D_0013F3D0_2e9010, camera + 0x30);
        side_axis = camera + 0x10;
        FUN_001f9bf8_2e9010(camera, camera, 1.0f);
        view_up_axis = camera + 0x20;
        fast_dec_timer_s16(view_blend_and_smoothing_state);
        up = D_L00_00166E30;
    } else {
        up = D_L00_00166E30;
        tb = base + 0x20;
        position_up_component = FUN_001f9ab0_2e9010(camera_state + 0x80, tb);
        FUN_001f9a68_2e9010(vector_scratch, tb, position_up_component);
        fast_vec_sub(projected_reference_position, camera_state + 0x80, vector_scratch);
        reloaded_camera_state = *(u8 **)(camera + 0x70);
        reference_orbit_offset = reloaded_camera_state + 0x130;
        position_up_component = FUN_001f9ab0_2e9010(camera + 0x30, tb);
        FUN_001f9a68_2e9010(vector_scratch, tb, position_up_component);
        fast_vec_sub(projected_camera_position, camera + 0x30, vector_scratch);
        fast_vec_sub(view_direction, projected_reference_position, projected_camera_position);
        projected_view_length = FUN_001f9af0_2e9010(view_direction);
        if (0.05f <= projected_view_length) {
            FUN_001f9a68_2e9010(camera, view_direction, 1.0f / projected_view_length);
        }
        side_axis_or_aim_position = camera + 0x10;
        fast_vec_cross(side_axis_or_aim_position, camera, up);
        FUN_001f9bf8_2e9010(side_axis_or_aim_position, side_axis_or_aim_position, 1.0f);
        side_axis = side_axis_or_aim_position;
        *(f32 *)(view_blend_and_smoothing_state + 0x8) =
            cam_interp_values(camera_state + 0x30,
                              *(f32 *)(view_blend_and_smoothing_state + 0x8),
                              *(f32 *)(reference_orbit_offset + 0x30),
                              0.004f,
                              0.2f,
                              0.0f);
        view_up_axis = camera + 0x20;
        smoothed_height_baseline = cam_interp_values(camera_state + 0x2C,
                                                     *(f32 *)(view_blend_and_smoothing_state + 0x4),
                                                     *(f32 *)(follow_state_block + 0xB0),
                                                     0.004f,
                                                     0.2f,
                                                     0.0f);
        smoothed_height_difference =
            *(f32 *)(view_blend_and_smoothing_state + 0x8) - smoothed_height_baseline;
        *(f32 *)(view_blend_and_smoothing_state + 0x4) = smoothed_height_baseline;
        FUN_001f9a68_2e9010(
            vector_scratch, up, -FUN_001f9ab0_2e9010(reloaded_camera_state + 0x140, up));
        fast_vec_sub(vector_scratch, vector_scratch, view_direction);
        side_axis_or_aim_position = (u8 *)adjusted_aim_position;
        fast_vec_sub(side_axis_or_aim_position, camera + 0x30, vector_scratch);
        FUN_001f9a68_2e9010(vector_scratch, up, -smoothed_height_difference);
        fast_vec_sub(side_axis_or_aim_position, side_axis_or_aim_position, vector_scratch);
        fast_vec_sub(view_direction, side_axis_or_aim_position, camera + 0x30);
        adjusted_view_length = FUN_001f9af0_2e9010(view_direction);
        if (adjusted_view_length != 0.0f) {
            view_rotation_angle =
                1.5707964f -
                fast_arc_sin(FUN_001f9ab0_2e9010(camera, view_direction) / adjusted_view_length);
            fast_vec_cross(view_up_axis, side_axis, camera);
            if (FUN_001f9ab0_2e9010(view_up_axis, view_direction) < 0.0f) {
                view_rotation_angle = -view_rotation_angle;
            }
            reference_orbit_up_component = FUN_001f9ab0_2e9010(reference_orbit_offset, up);
            x = fast_arc_tan(FUN_001f9af0_2e9010(reference_orbit_offset),
                             reference_orbit_up_component) /
                0.6981317f;
            target_view_angle_offset = 0.0f;
            if (x < -0.1f) {
                x = -x;
                if (0.5f < x) {
                    x = 1.0f - x;
                }
                x = x + x;
                target_view_angle_offset = x * 0.2617994f + target_view_angle_offset;
            }
            smoothed_view_angle_offset =
                rope_spring_turn(view_blend_and_smoothing_state + 0x18,
                                 *(f32 *)(view_blend_and_smoothing_state + 0x14),
                                 target_view_angle_offset,
                                 0.005f,
                                 0.2f,
                                 0.0f);
            *(f32 *)(view_blend_and_smoothing_state + 0x14) = smoothed_view_angle_offset;
            view_rotation_angle =
                fast_subtract_rotations(view_rotation_angle, smoothed_view_angle_offset);
            if (1.2217305f < view_rotation_angle) {
                view_rotation_angle = 1.2217305f;
            } else if (view_rotation_angle < -1.2217305f) {
                view_rotation_angle = -1.2217305f;
            }
            build_look_at_matrix(camera, camera, side_axis, view_rotation_angle);
            FUN_001f9bf8_2e9010(camera, camera, 1.0f);
        }
    }
    if (*(s16 *)view_blend_and_smoothing_state != 0) {
        fast_dec_timer_s16(view_blend_and_smoothing_state);
        remaining_view_blend_frames =
            convert_integer_to_float(*(s16 *)view_blend_and_smoothing_state);
        view_blend_fraction = remaining_view_blend_frames /
                              convert_integer_to_float(FUN_001f96f8_2e9010(D_L00_00161D24_gp));
        lerp_vector(camera, camera, camera + 0x40, view_blend_fraction);
        FUN_001f9bf8_2e9010(camera, camera, 1.0f);
    }
    fast_vec_cross(side_axis, camera, up);
    FUN_001f9bf8_2e9010(side_axis, side_axis, 1.0f);
    fast_vec_cross(view_up_axis, side_axis, camera);
}

void follow_camera_pull_back(char *o, float *x, float *y) __asm__("FUN_L00_002e9518");

void follow_camera_pull_back(char *camera_object, float *x, float *y) {
    C_2e9518 *C = &D_L00_00166E10_lui;
    char     *camera_state = *(char **)(camera_object + 0x70);
    S_2e9518 *pull_back_state = (S_2e9518 *)(camera_state + 0x220);
    char     *reference_orbit_state = camera_state + 0x130;
    float     k, pull_back_target, interpolated_pull_back_value;

    if (pull_back_state->h6 == 0) {
        if (pull_back_state->h4 != 0 && C->fA4 == 0.0f &&
            fast_vec_dot(D_001413D0_lui + 0xC0, camera_object) <= -0.3f) {
            goto on;
        }
        if (0.0001f < absolute_float(C->fA4) && fast_vec_dot(C->v80, camera_object) <= -0.3f) {
        on:
            pull_back_state->h4 = scale_game_frames(0x78);
            k = C->fA4 * 14.2857f;
            if (1.0f < k)
                k = 1.0f;
            pull_back_target = D_001413F4_lui == 1 ? 4.0f : pull_back_state->fC;
            interpolated_pull_back_value =
                *(float *)(reference_orbit_state + 0x58) +
                (pull_back_target - *(float *)(reference_orbit_state + 0x58)) * k;
            if (pull_back_state->f8 < interpolated_pull_back_value)
                pull_back_state->f8 = interpolated_pull_back_value;
            follow_camera_set_distance(0, pull_back_state->f8, 0.003f);
            *x = orbit_distance_default_stiffness_gp +
                 (orbit_distance_pull_back_stiffness_gp - orbit_distance_default_stiffness_gp) * k;
            *y = orbit_distance_default_damping_gp +
                 (orbit_distance_pull_back_damping_gp - orbit_distance_default_damping_gp) * k;
            return;
        }
    }
    if (pull_back_state->h4 != 0) {
        pull_back_state->h4 = 0;
        pull_back_state->f8 = *(float *)(reference_orbit_state + 0x58);
    }
    *x = orbit_distance_default_stiffness_gp;
    *y = orbit_distance_default_damping_gp;
}

/* Aims a camera rig at its target: updates yaw/pitch smoothing and clamps roll. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002EABD0), where it is exact; names translated to the US level program. */
void follow_camera_orbit(int arg) __asm__("FUN_L00_002e9720");

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
    float x, bc, dot_product, reference_up_component, y, current_up_component, orbit_angle_scratch,
        i, yaw_rotation_sign, current_orbit_length;
    char *base = D_L00_00166E10;
    char *up = base + 0x20;
    char *base2;
    char *fw;
    char *reference_orbit_offset = camera_state + 0x130;
    char *current_orbit_offset = camera_state + 0x140;
    char *p1d0 = camera_state + 0x1D0;
    fast_vec_add(camera_state, camera_state + 0x90, reference_orbit_offset);
    x = *(float *)(reference_orbit_offset + 0x2C);
    x -= *(float *)(p1d0 + 0x30);
    bc = fast_vec_length(current_orbit_offset);
    follow_camera_pull_back(camera_object, &tb[0], &tb[1]);
    bc = cam_interp_values(camera_state + 0x158, bc, x, tb[0], tb[1], 0.0f);
    reference_up_component = fast_vec_dot(reference_orbit_offset, up);
    y = fast_arc_sin(reference_up_component / x);
    current_up_component = fast_vec_dot(current_orbit_offset, up);
    orbit_angle_scratch = fast_arc_sin(current_up_component / bc);
    i = rope_spring_turn(camera_state + 0x154,
                         orbit_angle_scratch,
                         y,
                         *(float *)&orbit_pitch_spring_stiffness_gp,
                         *(float *)&orbit_pitch_spring_damping_gp,
                         0.0f);
    fast_vec_scale(reference_up_projection, up, reference_up_component);
    subtract_vector_xyz(
        projected_reference_offset, reference_orbit_offset, reference_up_projection);
    fast_vec_scale(current_up_projection, up, current_up_component);
    subtract_vector_xyz(projected_current_offset, current_orbit_offset, current_up_projection);
    dot_product = fast_vec_dot(projected_reference_offset, projected_current_offset);
    x = fast_vec_length(projected_reference_offset);
    x = x * fast_vec_length(projected_current_offset);
    orbit_angle_scratch = 1.5707964f - fast_arc_sin(dot_product / x);
    x = rope_spring_turn(camera_state + 0x150,
                         0.0f,
                         orbit_angle_scratch,
                         *(float *)&orbit_yaw_spring_stiffness_gp,
                         *(float *)&orbit_yaw_spring_damping_gp,
                         0.0f);
    fast_vec_cross(orbit_cross_scratch, projected_current_offset, up);
    yaw_rotation_sign =
        (fast_vec_dot(orbit_cross_scratch, projected_reference_offset) >= 0.0f) ? 1.0f : -1.0f;
    base2 = D_L00_00166E10;
    fw = base2 + 0x30;
    build_look_at_matrix(rotated_orbit_offset, projected_current_offset, up, x * yaw_rotation_sign);
    fast_vec_cross(orbit_cross_scratch, up, rotated_orbit_offset);
    build_look_at_matrix(rotated_orbit_offset, rotated_orbit_offset, orbit_cross_scratch, i);
    fast_vec_normalize(current_orbit_offset, rotated_orbit_offset, bc);
    fast_vec_scale(scaled_up_offset, up, *(float *)(reference_orbit_offset + 0x30));
    fast_vec_add(orbit_base_position, scaled_up_offset, camera_state + 0xA0);
    dot_product = fast_vec_dot(current_orbit_offset, fw);
    current_orbit_length = fast_vec_length(current_orbit_offset);
    if (current_orbit_length != 0.0f) {
        x = 1.5707964f - fast_arc_sin(dot_product / current_orbit_length);
        if (absolute_float(x) < 0.2617994f) {
            fast_vec_cross(forward_clamp_axis, current_orbit_offset, fw);
            build_look_at_matrix(current_orbit_offset,
                                 current_orbit_offset,
                                 forward_clamp_axis,
                                 fast_subtract_rotations(0.2617994f, absolute_float(x)));
            *(float *)(reference_orbit_offset + 0x28) = 0.0f;
            *(float *)(reference_orbit_offset + 0x20) = 0.0f;
            *(float *)(reference_orbit_offset + 0x24) = 0.0f;
        }
    }
    fast_vec_add(camera_object + 0x30, orbit_base_position, reference_orbit_offset + 0x10);
}

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
                        char *referenced_record =
                            D_L00_001600F4_9aa0 + ((E_9aa0 *)(grid_cell_block + 16))[k].idx * 128;
                        ((void (*)(void *, float))noop_main_callback)(
                            referenced_record + 0x30, FUN_001f9af0_9aa0(referenced_record));
                    }
                }
            }
        }
    }
}

/* Runs eight FUN_L00_002e* steps on object a in order. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002EB060), where it is exact; names translated to the US level program. */
void update_follow_camera_pose(int camera_moby) __asm__("FUN_L00_002e9bb0");

void update_follow_camera_pose(int camera_moby) {
    float camera_stick_input[2];
    run_type5_grid_record_noop_callbacks(camera_moby);
    follow_camera_auto_turn(camera_moby);
    follow_camera_read_stick(camera_moby, &camera_stick_input[0], &camera_stick_input[1]);
    follow_camera_turn_and_pitch(camera_moby, camera_stick_input[0], camera_stick_input[1]);
    follow_camera_avoid_mobies(camera_moby);
    follow_camera_orbit(camera_moby);
    run_follow_camera_noop(camera_moby);
    follow_camera_build_view(camera_moby);
}

void follow_camera_update(u8 *m) __asm__("FUN_L00_002e9c28");

void follow_camera_update(u8 *camera_moby) {
    u8 *camera_spring_state = *(u8 **)(camera_moby + 0x70) + 0x1D0;
    D_L00_0015EF44_gp = follow_camera_yaw_rate_table[opt_cam_yaw_rate];
    apply_follow_camera_translation();
    follow_camera_apply_overrides(camera_moby);
    noop_main_callback(camera_moby);
    follow_camera_springs(camera_moby);
    noop_main_callback(camera_moby, camera_spring_state);
    update_follow_camera_pose(camera_moby);
    noop_main_callback(camera_moby);
    follow_camera_approach(camera_moby);
}
