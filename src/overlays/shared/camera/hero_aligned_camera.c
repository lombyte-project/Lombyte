/* Camera type 4: a camera aligned with the hero, steered by the pad, and its transition step. */

#include "types.h"
#include "rnc/overlay/camera.h"
#include "rnc/hero/hero.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/options.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_camera.h"

#define P D_0013F350_lui
#define Q l00_camera_lui

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V;

typedef struct {
    float a, va, b, vb, ang, z, vz;
} S_9f38;

typedef struct {
    u8    pad0[0x100];
    float f100, f104, f108, f10C;
    u8    pad1[0x1A0 - 0x110];
    int   i1A0;
} C940;

typedef struct {
    u8    pad0[0x180];
    char *current_follow_cam; /* 0x180: struct UpdateCam * */
    u8    pad184[0x270 - 0x184];
    s16   s270;
} G680;

typedef struct {
    f32 f[4];
} V_2ea488;

typedef struct {
    u8  p0[0x20];
    s16 h20;
    s16 h22;
    s32 i24;
} Q_2ea488;

typedef struct {
    u8        p0[0x1C];
    Q_2ea488 *q;
} E_2ea488;

typedef struct {
    u8    p0[0x98];
    f32   f98;
    u8    p1[0x2080 - 0x9C];
    char *p2080;
    u8    p2[0x2284 - 0x2084];
    s32   i2284;
} P_2ea488;

typedef struct {
    char    *o;
    u8       p[0x3C];
    V_2ea488 v;
} S_2ea488;

typedef struct {
    char  pad[0x254];
    int   p;
    char  pad2[0x18];
    short s;
    char  pad3[1];
    char  c;
    char  pad4[0x14];
    float f0;
    char  pad5[8];
    float f1;
} S;

typedef struct {
    unsigned char pad0[0x1C0];
    float         v1C0[4];
    unsigned char pad1D0[0x270 - 0x1D0];
    short         s270;
    unsigned char pad272;
    unsigned char b273;
    unsigned char pad274[0x288 - 0x274];
    float         f288;
    unsigned char pad28C[0x294 - 0x28C];
    float         f294;
} Q_2ea810;

typedef struct {
    unsigned char pad0[0x7E];
    short         s7E;
} A_2ea810;

extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void  FUN_001f9bf8_002e9cc0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  FUN_001f9a68_002e9cc0(void *, void *, float) __asm__("FUN_001f9a68");
extern void  FUN_001f9bf8_2e9dd0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  FUN_001f9a68_2e9dd0(void *, void *, float) __asm__("FUN_001f9a68");
extern void  FUN_001f9a10_2e9dd0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  FUN_001f9ad8_2e9dd0(void *, void *, void *) __asm__("FUN_001f9ad8");
void         FUN_001f9bf8_c(void *, void *, float) __asm__("FUN_001f9bf8");
void         FUN_001f9a10_c(void *, void *, void *) __asm__("FUN_001f9a10");
float        FUN_001ebd78(float, float, float, float, float, float *);
int          coll_sphere(void *, int, void *, float) __asm__("FUN_L00_001f0d60");
extern char *find_camera_collision_volume_for_point(void *, f32) __asm__("FUN_L00_001ee1b0");
extern int   FUN_001f96f8_2ea488(int) __asm__("FUN_001f96f8");
extern void
set_follow_camera_angle_blend_from_direction(void *, f32, f32) __asm__("FUN_L00_002e8918");
extern f32   FUN_001f9ab0_2ea488(void *, void *) __asm__("FUN_001f9ab0");
extern void  FUN_001f9a68_2ea488(void *, void *, f32) __asm__("FUN_001f9a68");
extern void  FUN_001f9a28_2ea488(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32   FUN_001f9af0_2ea488(void *) __asm__("FUN_001f9af0");
extern void  init_camera_system(void) __asm__("FUN_L00_001ed280");
extern void  FUN_L00_002e9ef0_v(void) __asm__("FUN_L00_002e9ef0");
extern float FUN_001f9ab0_2ea810(void *, void *) __asm__("FUN_001f9ab0");
extern void  FUN_001f9a68_2ea810(void *, void *, float) __asm__("FUN_001f9a68");
extern void  FUN_001f9a28_2ea810(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_2ea810(void *) __asm__("FUN_001f9af0");

extern unsigned char     D_0015EDB3_002e9cc0 __asm__("D_0015EDB3");
extern u128              D_0013F3D0_2e9dd0 __asm__("D_0013F3D0");
extern FollowCamSideRef *D_L00_00166E04 __asm__("D_L00_00166E04") NOT_SDA;
extern C940              D_0013C940;
extern float hero_aligned_camera_rotation_gain_at_zero_input_gp __asm__("D_L00_00161DC0")
    __attribute__((sda));
extern float hero_aligned_camera_rotation_gain_at_unit_input_gp __asm__("D_L00_00161DC4")
    __attribute__((sda));
extern float hero_aligned_camera_input_stiffness_gp __asm__("D_L00_00161DC8") __attribute__((sda));
extern float hero_aligned_camera_input_damping_gp __asm__("D_L00_00161DCC") __attribute__((sda));
extern float hero_aligned_camera_input_step_limit_gp __asm__("D_L00_00161DD0") __attribute__((sda));
extern P_2ea488    D_0013F350_lui __asm__("D_0013F350") NOT_SDA;
extern OvlQuad     D_0013F3D0_lui __asm__("D_0013F3D0") NOT_SDA;
extern E_2ea488   *D_L00_0015EF50_2ea488 __asm__("D_L00_0015EF50");
extern char       *D_L00_00166E00_lui __asm__("D_L00_00166E00") NOT_SDA;
extern char        D_L00_00166E10_2ea488[] __asm__("D_L00_00166E10") NOT_SDA;
extern LevelCamera l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;

float     D_L01_00162238 = 0;
float     D_L01_0016223C = 0.02617994f;
float     D_L01_00162240 = 0.03f;
float     D_L01_00162244 = 0.4f;
float     D_L01_00162248 = 0;
float     D_L02_00162370 = 0;
float     D_L02_00162374 = 0.02617994f;
float     D_L02_00162378 = 0.03f;
float     D_L02_0016237C = 0.4f;
float     D_L02_00162380 = 0;
float     D_L03_00161E38 = 0;
float     D_L03_00161E3C = 0.02617994f;
float     D_L03_00161E40 = 0.03f;
float     D_L03_00161E44 = 0.4f;
float     D_L03_00161E48 = 0;
float     D_L04_00161F30 = 0;
float     D_L04_00161F34 = 0.02617994f;
float     D_L04_00161F38 = 0.03f;
float     D_L04_00161F3C = 0.4f;
float     D_L04_00161F40 = 0;
E_2ea488 *D_L04_0015EF50 = 0;
float     D_L05_00162188 = 0;
float     D_L05_0016218C = 0.02617994f;
float     D_L05_00162190 = 0.03f;
float     D_L05_00162194 = 0.4f;
float     D_L05_00162198 = 0;
float     D_L06_00162550 = 0;
float     D_L06_00162554 = 0.02617994f;
float     D_L06_00162558 = 0.03f;
float     D_L06_0016255C = 0.4f;
float     D_L06_00162560 = 0;
float     D_L07_00161E08 = 0;
float     D_L07_00161E0C = 0.02617994f;
float     D_L07_00161E10 = 0.03f;
float     D_L07_00161E14 = 0.4f;
float     D_L07_00161E18 = 0;
E_2ea488 *D_L07_0015EF50 = 0;
float     D_L08_001625C8 = 0;
float     D_L08_001625CC = 0.02617994f;
float     D_L08_001625D0 = 0.03f;
float     D_L08_001625D4 = 0.4f;
float     D_L08_001625D8 = 0;
E_2ea488 *D_L08_0015EF50 = 0;
float     D_L09_00161F20 = 0;
float     D_L09_00161F24 = 0.02617994f;
float     D_L09_00161F28 = 0.03f;
float     D_L09_00161F2C = 0.4f;
float     D_L09_00161F30 = 0;
E_2ea488 *D_L09_0015EF50 = 0;
float     D_L10_001621F0 = 0;
float     D_L10_001621F4 = 0.02617994f;
float     D_L10_001621F8 = 0.03f;
float     D_L10_001621FC = 0.4f;
float     D_L10_00162200 = 0;
float     D_L11_00162758 = 0;
float     D_L11_0016275C = 0.02617994f;
float     D_L11_00162760 = 0.03f;
float     D_L11_00162764 = 0.4f;
float     D_L11_00162768 = 0;
E_2ea488 *D_L11_0015EF50 = 0;
float     D_L12_00162230 = 0;
float     D_L12_00162234 = 0.02617994f;
float     D_L12_00162238 = 0.03f;
float     D_L12_0016223C = 0.4f;
float     D_L12_00162240 = 0;
E_2ea488 *D_L12_0015EF50 = 0;
float     D_L13_001620A8 = 0;
float     D_L13_001620AC = 0.02617994f;
float     D_L13_001620B0 = 0.03f;
float     D_L13_001620B4 = 0.4f;
float     D_L13_001620B8 = 0;
float     D_L14_00162460 = 0;
float     D_L14_00162464 = 0.02617994f;
float     D_L14_00162468 = 0.03f;
float     D_L14_0016246C = 0.4f;
float     D_L14_00162470 = 0;
float     D_L15_001623B0 = 0;
float     D_L15_001623B4 = 0.02617994f;
float     D_L15_001623B8 = 0.03f;
float     D_L15_001623BC = 0.4f;
float     D_L15_001623C0 = 0;
float     D_L16_00162178 = 0;
float     D_L16_0016217C = 0.02617994f;
float     D_L16_00162180 = 0.03f;
float     D_L16_00162184 = 0.4f;
float     D_L16_00162188 = 0;
E_2ea488 *D_L16_0015EF50 = 0;
float     D_L17_00162660 = 0;
float     D_L17_00162664 = 0.02617994f;
float     D_L17_00162668 = 0.03f;
float     D_L17_0016266C = 0.4f;
float     D_L17_00162670 = 0;
E_2ea488 *D_L17_0015EF50 = 0;
float     D_L18_00162778 = 0;
float     D_L18_0016277C = 0.02617994f;
float     D_L18_00162780 = 0.03f;
float     D_L18_00162784 = 0.4f;
float     D_L18_00162788 = 0;

void update_hero_aligned_camera_position(char *camera_object) __asm__("FUN_L00_002e9cc0");

void update_hero_aligned_camera_position(char *camera_object) {
    char         normalized_hero_up_axis[16] __attribute__((aligned(16)));
    char         camera_height_offset[16] __attribute__((aligned(16)));
    struct Hero *hero_data = &hero;
    int          vehicle_mode;
    FUN_001f9bf8_002e9cc0(normalized_hero_up_axis, ((char *)hero_data->moby) + 0xE0, 1.0f);
    vehicle_mode = hero_data->vehicle_mode;
    if (vehicle_mode == 2) {
        FUN_001f9a68_002e9cc0(camera_height_offset, normalized_hero_up_axis, 9.5f);
    } else if (vehicle_mode == 1) {
        if (D_0015EDB3_002e9cc0) {
            FUN_001f9a68_002e9cc0(camera_height_offset, normalized_hero_up_axis, 1.2f);
        } else {
            FUN_001f9a68_002e9cc0(camera_height_offset, normalized_hero_up_axis, 0.9f);
        }
    } else {
        FUN_001f9a68_002e9cc0(camera_height_offset, normalized_hero_up_axis, 1.6f);
    }
    camera_object += 0x30;
    fast_vec_add(camera_object, &hero.motion.pos, camera_height_offset);
    fast_vec_add(camera_object, &hero.motion.platform_carry, camera_object);
}

void initialize_hero_aligned_camera_pose(char *camera_object) __asm__("FUN_L00_002e9dd0");

void initialize_hero_aligned_camera_pose(char *camera_object) {
    u128         hero_up_axis;
    u128         scaled_hero_up_axis;
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
        void *side_axis_reference = D_L00_00166E04;
        qcopy(camera_object + 0x20, &hero_up_axis);
        FUN_001f9ad8_2e9dd0(camera_object + 0x10, side_axis_reference, &hero_up_axis);
    }
    FUN_001f9bf8_2e9dd0(camera_object + 0x10, camera_object + 0x10, 1.0f);
    FUN_001f9ad8_2e9dd0(camera_object, &hero_up_axis, camera_object + 0x10);
    qcopy(camera_object + 0x40, camera_object);
}

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

void update_hero_aligned_camera_from_input(u8 *camera_object) __asm__("FUN_L00_002e9f38");

void update_hero_aligned_camera_from_input(u8 *camera_object) {
    V       up;
    V       side;
    V       horizontal_view_offset;
    S_9f38 *camera_rotation_state;
    float   rotation_input_target, rotation_input_scratch, k;
    camera_rotation_state = *(S_9f38 **)(camera_object + 0x70);
    update_hero_aligned_camera_position(camera_object);
    FUN_001f9bf8_c(&up, ((u8 *)hero.moby) + 0xE0, 1.0f);
    if (l00_camera.unk270 == 0 && hero.unk20A5 == 0)
        hero.unk20A5 = 1;
    rotation_input_target = -D_0013C940.f108;
    if (rotation_input_target == 0.0f)
        rotation_input_target = -D_0013C940.f100;
    if (rotation_input_target == 0.0f)
        rotation_input_target = convert_integer_to_float((D_0013C940.i1A0 >> 15) & 1);
    if (rotation_input_target == 0.0f)
        rotation_input_target = -convert_integer_to_float((D_0013C940.i1A0 >> 13) & 1);
    rotation_input_scratch = absolute_float(camera_rotation_state->a);
    k = hero_aligned_camera_rotation_gain_at_zero_input_gp +
        (hero_aligned_camera_rotation_gain_at_unit_input_gp -
         hero_aligned_camera_rotation_gain_at_zero_input_gp) *
            rotation_input_scratch;
    camera_rotation_state->a = rotation_input_scratch =
        FUN_001ebd78(camera_rotation_state->a,
                     rotation_input_target,
                     hero_aligned_camera_input_stiffness_gp,
                     hero_aligned_camera_input_damping_gp,
                     hero_aligned_camera_input_step_limit_gp,
                     &camera_rotation_state->va);
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
    if (l00_camera.unk270 != 0 && hero.state.prev == 2)
        rotation_input_target = 0.0f;
    if (invert52 == 0)
        rotation_input_target = -rotation_input_target;
    rotation_input_scratch = absolute_float(camera_rotation_state->b);
    k = hero_aligned_camera_rotation_gain_at_zero_input_gp +
        (hero_aligned_camera_rotation_gain_at_unit_input_gp -
         hero_aligned_camera_rotation_gain_at_zero_input_gp) *
            rotation_input_scratch;
    camera_rotation_state->b = FUN_001ebd78(camera_rotation_state->b,
                                            rotation_input_target,
                                            hero_aligned_camera_input_stiffness_gp,
                                            hero_aligned_camera_input_damping_gp,
                                            hero_aligned_camera_input_step_limit_gp,
                                            &camera_rotation_state->vb);
    if (camera_rotation_state->b != 0.0f) {
        float a, prospective_pitch_angle, z;
        fast_vec_cross(&side, camera_object, &up);
        a = k * camera_rotation_state->b;
        prospective_pitch_angle = absolute_float(fast_add_rotations(camera_rotation_state->ang, a));
        if (1.1170107f < prospective_pitch_angle &&
            absolute_float(camera_rotation_state->ang) < prospective_pitch_angle)
            a += (0.0f - a) *
                 (fast_subtract_rotations(prospective_pitch_angle, 1.1170107f) / 0.3490659f);
        if (prospective_pitch_angle >= 1.1152654f && camera_rotation_state->ang > 0.0f)
            camera_rotation_state->z = FUN_001ebd78(
                camera_rotation_state->z, 0.5f, 0.005f, 0.2f, 0.0f, &camera_rotation_state->vz);
        else
            camera_rotation_state->z = FUN_001ebd78(
                camera_rotation_state->z, 0.0f, 0.005f, 0.2f, 0.0f, &camera_rotation_state->vz);
        z = 0.0f;
        horizontal_view_offset.f[0] = *(float *)(camera_object + 0);
        horizontal_view_offset.f[1] = *(float *)(camera_object + 4);
        horizontal_view_offset.f[3] = z;
        horizontal_view_offset.f[2] = z;
        FUN_001f9bf8_c(&horizontal_view_offset, &horizontal_view_offset, camera_rotation_state->z);
        FUN_001f9a10_c(camera_object + 0x30, camera_object + 0x30, &horizontal_view_offset);
        if (coll_sphere(camera_object + 0x30, 0x12, ((u8 *)hero.moby), 0.3f))
            camera_rotation_state->z = FUN_001ebd78(
                camera_rotation_state->z, z, 0.005f, 0.2f, z, &camera_rotation_state->vz);
        prospective_pitch_angle = fast_add_rotations(camera_rotation_state->ang, a);
        if (absolute_float(prospective_pitch_angle) > 1.4660766f) {
            float signed_pitch_limit = 1.4660766f;
            if (!(z < prospective_pitch_angle))
                signed_pitch_limit = -1.4660766f;
            a = fast_subtract_rotations(signed_pitch_limit, camera_rotation_state->ang);
        }
        build_look_at_matrix(camera_object, camera_object, &side, a);
        camera_rotation_state->ang = fast_add_rotations(camera_rotation_state->ang, a);
    }
    FUN_001f9bf8_c(camera_object, camera_object, 1.0f);
    fast_vec_cross(camera_object + 0x10, camera_object, &up);
    FUN_001f9bf8_c(camera_object + 0x10, camera_object + 0x10, 1.0f);
    fast_vec_cross(camera_object + 0x20, camera_object + 0x10, camera_object);
}

/* camera_transition_update -- camera-state steering: either replays the scripted
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
int camera_transition_update(u8 *, char *) __asm__("FUN_L00_002ea488");

int camera_transition_update(u8 *transition_camera_record, char *mode_check_object) {
    Q_2ea488 *q;
    char     *active_transition_camera_record;
    char     *r;
    s32       requested_camera_mode;
    int       checked_object_mode;
    int       active_camera_mode;
    V_2ea488  transition_vector_scratch;
    V_2ea488  transition_camera_vector_residual, transition_hero_vector_residual;
    f32       transition_residual_length_product, x;
    P_2ea488 *g;
    char     *b, *transition_camera_globals, *alignment_comparison_axis;

    q = D_L00_0015EF50_2ea488[*(s16 *)(transition_camera_record + 0x84)].q;
    g = &P;
    requested_camera_mode = g->i2284;
    if (requested_camera_mode == *(s16 *)(transition_camera_record + 0x86) ||
        requested_camera_mode == 0x51) {
        active_transition_camera_record = D_L00_00166E00_lui;
        checked_object_mode = *(s16 *)(mode_check_object + 0x86);
        if (checked_object_mode != 0 && checked_object_mode != 10 && checked_object_mode != 14 &&
            checked_object_mode != 2)
            goto off;
        active_camera_mode = *(s16 *)(active_transition_camera_record + 0x86);
        if (active_camera_mode != 0 && active_camera_mode != 10 && active_camera_mode != 14 &&
            active_camera_mode != 2)
            goto off;
        *(OvlQuad *)&transition_vector_scratch = D_0013F3D0_lui;
        r = find_camera_collision_volume_for_point(&transition_vector_scratch, 0.25f);
        if (r && (*(s32 *)(r + 0x18) & 1))
            return 0;
        q->i24 = 0;
        q->h22 = 1;
        q->h20++;
        if (q->h20 >= FUN_001f96f8_2ea488(7)) {
            if (*(s16 *)(active_transition_camera_record + 0x86) == 0)
                transition_camera_record[0x7C] = 5;
            return 1;
        }
        transition_vector_scratch.f[0] = fast_cos(P.f98);
        transition_vector_scratch.f[1] = fast_sin(P.f98);
        *(s32 *)&transition_vector_scratch.f[2] = 0;
        set_follow_camera_angle_blend_from_direction(
            &transition_vector_scratch, 0.06981316953897476f, transition_vector_scratch.f[2]);
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
    transition_camera_globals = b - 0x190;
    alignment_comparison_axis = b + 0x30;
    FUN_001f9a68_2ea488(&transition_vector_scratch,
                        alignment_comparison_axis,
                        FUN_001f9ab0_2ea488(alignment_comparison_axis,
                                            *(char **)(transition_camera_globals + 0x180)));
    FUN_001f9a28_2ea488(&transition_camera_vector_residual,
                        *(char **)(transition_camera_globals + 0x180),
                        &transition_vector_scratch);
    FUN_001f9a68_2ea488(&transition_vector_scratch,
                        alignment_comparison_axis,
                        FUN_001f9ab0_2ea488(alignment_comparison_axis, g->p2080 + 0xC0));
    FUN_001f9a28_2ea488(
        &transition_hero_vector_residual, g->p2080 + 0xC0, &transition_vector_scratch);
    transition_residual_length_product = FUN_001f9af0_2ea488(&transition_hero_vector_residual);
    if (transition_residual_length_product != 0.0f) {
        transition_residual_length_product *=
            FUN_001f9af0_2ea488(&transition_camera_vector_residual);
    }
    if (transition_residual_length_product != 0.0f) {
        x = fast_arc_sin(FUN_001f9ab0_2ea488(&transition_hero_vector_residual,
                                             &transition_camera_vector_residual) /
                         transition_residual_length_product);
        if (1.3962633609771729f <= 1.5707964f - x) {
            init_camera_system();
            return 0;
        }
    }
    q->i24 = FUN_001f96f8_2ea488(30);
tail:
    if (q->i24) {
        fast_dec_timer(&q->i24);
        transition_vector_scratch.f[0] = fast_cos(P.f98);
        transition_vector_scratch.f[1] = fast_sin(P.f98);
        *(s32 *)&transition_vector_scratch.f[2] = 0;
        set_follow_camera_angle_blend_from_direction(
            &transition_vector_scratch, 0.13962633907794952f, transition_vector_scratch.f[2]);
    }
    return 0;
}

/* Sets s to 1 (2 if set), resets l00_camera, calls 002e9ef0, allocates a voice for its p. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EBC50), where it is exact; names translated to the US level program. */
void initialize_hero_aligned_camera_mode(void) __asm__("FUN_L00_002ea7a0");

void initialize_hero_aligned_camera_mode(void) {
    LevelCamera *shared_camera_globals = &l00_camera;
    LevelCamera *camera_globals;
    if (shared_camera_globals->unk270 == 0)
        shared_camera_globals->unk270 = 1;
    else
        shared_camera_globals->unk270 = 2;
    camera_globals = &l00_camera;
    camera_globals->unk273 = 0;
    camera_globals->unk294 = 0.05f;
    camera_globals->unk288 = 0.05f;
    FUN_L00_002e9ef0_v();
    if (camera_globals->unk254 != 0)
        allocate_voice_for_target_entry(0, 0, camera_globals->unk254);
}

void select_camera_mode_by_hero_vector_alignment(A_2ea810 *camera_object) __asm__("FUN_L00_002ea810");

void select_camera_mode_by_hero_vector_alignment(A_2ea810 *camera_object) {
    float  scaled_axis_scratch[4];
    float  camera_vector_residual[4];
    float  hero_vector_residual[4];
    float *comparison_axis;
    float  residual_length_product;
    float  dot_product_or_asin_result;

    if (hero.unk2284 == 0x51 || hero.unk2284 == 4) {
        return;
    }
    if (Q.unk270 == 0) {
        Q.unk273 = 0;
        camera_object->s7E = 2;
        Q.unk294 = 0.018f;
        Q.unk288 = 0.018f;
        return;
    }
    comparison_axis = Q.unk1C0;
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
            dot_product_or_asin_result =
                fast_arc_sin(FUN_001f9ab0_2ea810(hero_vector_residual, camera_vector_residual) /
                             residual_length_product);
            if (1.5707964f - dot_product_or_asin_result >= 1.3962634f) {
                Q.unk270 = 0;
                camera_object->s7E = 4;
                return;
            }
        }
    }
    Q.unk273 = 0;
    camera_object->s7E = 2;
    Q.unk294 = 0.018f;
    Q.unk288 = 0.018f;
}
