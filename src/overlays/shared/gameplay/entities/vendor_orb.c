/* Shared moby classes: vendor_orb, hydrodisplacer, pilots_helmet, pterodactyl, red_plant, robot_arm. */
#include "types.h"
#include "rnc/gameplay/camera/camera_modes.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/quad.h"

extern float D_L00_00161BB0 __attribute__((sda));
float random_angle_radians(void) __asm__("FUN_00213308");
void attach_manipulator(unsigned char *, int, char *) __asm__("FUN_0020cb10");
float fast_add_rotations(float, float) __asm__("FUN_001fa580");
void FUN_L00_001fff28(char *, int, float);
void vendor_orb_update(unsigned char *p) __asm__("FUN_L00_002df730");
void vendor_orb_update(unsigned char *moby) {
    enum {
        VENDOR_ORB_INITIALIZE = 0,
        VENDOR_ORB_ROTATE = 1
    };
    char *manipulator = *(char **)(moby + 0x78);
    float rotation_step_radians;
    switch (moby[0x20]) {
    case VENDOR_ORB_INITIALIZE:
        moby[0x20] = 1;
        moby[0x73] = 0x20;
        *(float *)(manipulator + 0x40) = random_angle_radians();
        attach_manipulator(moby, 1, manipulator);
        break;
    case VENDOR_ORB_ROTATE:
        rotation_step_radians = D_L00_00161BB0 * DEG_TO_RAD * frame_time;
        *(float *)(moby + 0x48) = fast_add_rotations(*(float *)(moby + 0x48), rotation_step_radians);
        *(float *)(manipulator + 0x40) = fast_add_rotations(*(float *)(manipulator + 0x40), rotation_step_radians * -2.0f);
        FUN_L00_001fff28(manipulator + 0x10, 2, *(float *)(manipulator + 0x40));
        break;
    }
}
#include "qcopy.h"
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2df808;
typedef unsigned int U128_2df808 __attribute__((mode(TI), aligned(16)));
typedef union {
    U128_2df808 q;
    float f[4];
} R_2df808;
extern unsigned char *FUN_L00_0024e838_2df808(int) __asm__("FUN_L00_0024e838");
extern float FUN_L00_00257be8_2df808(float, float) __asm__("FUN_L00_00257be8");
extern void glow_n(unsigned char *, int, int, int) __asm__("FUN_L00_002502f0");
extern void upd_a(unsigned char *) __asm__("FUN_L00_00250df8");
unsigned char *FUN_L00_002df808(int owner, V_2df808 *position, V_2df808 *vector,
                                 int flags, unsigned char red, unsigned char green,
                                 unsigned char blue, unsigned char alpha,
                                 float scale) {
    unsigned char *moby = FUN_L00_0024e838_2df808(0x4a8);
    unsigned char *data;
    R_2df808 rotation_copy;
    R_2df808 rotation;
    if (moby != 0) {
        data = *(unsigned char **)(moby + 0x78);
        rotation.q = 0;
        rotation.f[0] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation.f[1] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation.f[2] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation_copy.q = rotation.q;
        moby[0x20] = 0;
        moby[0x23] = alpha;
        moby[0x30] = 0xff;
        *(unsigned short *)(moby + 0x32) = 0xff;
        moby[0x31] = 1;
        glow_n(moby, red, green, blue);
        *(float *)(data + 0x18) = *(float *)(moby + 0x2c) * scale;
        *(int *)(moby + 0x2c) = 0;
        *(int *)(data + 0x10) = owner;
        qcopy(moby + 0x10, position);
        qcopy(data, vector);
        qcopy(moby + 0x40, &rotation_copy);
        *(unsigned short *)(data + 0x1e) = alpha;
        *(int *)(data + 0x14) = flags;
        *(unsigned short *)(data + 0x1c) = *(unsigned short *)(data + 0x14);
        upd_a(moby);
    }
    return moby;
}
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E0E50), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct PadState PadState;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2E0E50;

typedef struct {
    int i0;
    int i4;
    int i8;
    int iC;
    float f10;
    float f14;
} Data_2E0E50;

typedef struct {
    char p0[0x30];
    Vec_2E0E50 v30;
    char p40[0x30];
    Vec_2E0E50 v70;
} Node_2E0E50;

extern char D_0013F350[] __attribute__((section(".data")));
extern char D_L00_00166DC0[] __attribute__((section(".data")));
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 approach_value(f32 target, f32 step, f32 *value) __asm__("FUN_00213ed8");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations_c2(f32, f32) __asm__("func_001FA5C8");
extern int FUN_001f96b0_c() __asm__("FUN_001f96b0");
extern int FUN_001f9a40_c() __asm__("FUN_001f9a40");
extern int hero_set_state() __asm__("FUN_L00_002223f8");
extern int FUN_L00_00266d60();
extern int allocate_voice_for_target_entry_alt(int,int,int) __asm__("FUN_0022da68");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void FUN_L00_002323b8(int,int);
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
s32 FUN_L00_002e02e0_c(void) __asm__("FUN_L00_002e02e0");
s32 scale_game_frames_alt_c(s32) __asm__("FUN_001f96f8");
void FUN_L00_002e01b0_c(char *m) __asm__("FUN_L00_002e01b0");
void FUN_L00_002e0340_c(char *a) __asm__("FUN_L00_002e0340");
void FUN_L00_002ea9d8(char *src);
void FUN_L00_002eaa30(char *src);
void jet_mount(void *pa, void *pb, s32 mode, s32 w, s32 x) __asm__("FUN_L00_002eaaa0");
void FUN_L00_002eac18_c(int mode) __asm__("FUN_L00_002eac18");
extern char D_0013E633_2E0E50[] __asm__("D_0013F350");
extern char D_0013A5E0_2E0E50[] __asm__("D_0013CAE0");
extern int D_L00_0015F6A8_2E0E50 __asm__("D_L00_0015F5C4") __attribute__((section(".sdata")));
extern int D_L00_0015F404_macro __asm__("D_L00_0015F404") __attribute__((section(".sdata")));
extern char *D_L00_00160098_2E0E50 __asm__("D_L00_0015FFD8") __attribute__((section(".sdata")));
extern char *D_L00_001601AC_2E0E50 __asm__("D_L00_001600EC") __attribute__((section(".sdata")));
extern float D_L00_0015F4FC_2E0E50 __asm__("D_L00_0015F3FC") __attribute__((section(".sdata")));
extern float D_L00_00161C84_2E0E50 __asm__("D_L00_00161BB4") __attribute__((section(".sdata")));
extern float D_0015EE6C_2E0E50 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float frame_scale_sq_macro __asm__("D_0015ED64") __attribute__((section(".sdata")));
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern char D_L00_00166EC0_2E0E50[] __asm__("D_L00_00166DC0");
extern void func_L00_002E1660_2E0E50(Moby *) __asm__("FUN_L00_002e01b0");
extern int func_0022ED80_2E0E50(int, int, Moby *) __asm__("FUN_0022da68");
extern int func_001F9850_2E0E50(int) __asm__("FUN_001f96f8");
extern int func_L00_00267BA8_2E0E50(int, int, int) __asm__("FUN_L00_00266d60");
extern int func_L00_002E1790_2E0E50(Moby *) __asm__("FUN_L00_002e02e0");
extern int func_L00_00222B80_2E0E50(int, int) __asm__("FUN_L00_002223f8");
extern void func_L00_00232C10_2E0E50(int, int, float) __asm__("FUN_L00_002323b8");
extern void func_L00_002EBF50_2E0E50(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern float func_001F9878_2E0E50(float) __asm__("FUN_001f96b0");
extern int func_001FA898_2E0E50(float) __asm__("FUN_001fa6d0");
extern void func_L00_002E17F0_2E0E50(Moby *) __asm__("FUN_L00_002e0340");
extern float func_00214D28_2E0E50(float *, float, float) __asm__("FUN_00213ed8");
extern float func_00214D88_2E0E50(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void func_L00_002EBE88_2E0E50(void *) __asm__("FUN_L00_002ea9d8");
extern void func_L00_002EBEE0_2E0E50(void *) __asm__("FUN_L00_002eaa30");
extern void func_001F9C08_2E0E50(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void func_001F9BC0_2E0E50(void *) __asm__("FUN_001f99f8");
extern float func_001FA790_2E0E50(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA748_2E0E50(float, float) __asm__("FUN_001fa580");
extern void func_L00_002EC0C8_2E0E50(int) __asm__("FUN_L00_002eac18");
extern int func_001F9908_2E0E50(void *) __asm__("FUN_001f9740");

void hydrodisplacer_update(Moby *moby) __asm__("FUN_L00_002df9a0");
void hydrodisplacer_update(Moby *moby) {
    enum {
        HYDRODISPLACER_INITIALIZE = 0,
        HYDRODISPLACER_WAIT_FOR_ANIMATION_FLAG = 1,
        HYDRODISPLACER_WAIT_FOR_ACTIVATION = 2,
        HYDRODISPLACER_WAIT_FOR_HERO_FLAG = 3,
        HYDRODISPLACER_CAMERA_TRANSITION = 4,
        HYDRODISPLACER_WAIT_FOR_TIMER = 5
    };
    enum {
        HYDRODISPLACER_CAMERA_PREPARE_INTERPOLATION = 0,
        HYDRODISPLACER_CAMERA_INTERPOLATE = 1,
        HYDRODISPLACER_CAMERA_RETURN_TO_ACTIVATION = 2
    };
    Vec_2E0E50 camera_pos, camera_rot;
    Data_2E0E50 *data = (void *)moby->pvars;

    if (((Hero *)D_0013E633_2E0E50)->items[0].state == 3) {
        func_L00_002E17F0_2E0E50(moby);
        return;
    }
    if (((Hero *)D_0013E633_2E0E50)->unk20B0 != 0) {
        if (D_L00_0015F6A8_2E0E50 == 0 && !((unsigned int)(((Hero *)D_0013E633_2E0E50)->state.current - 0x38) < 2)) {
            func_L00_002E1660_2E0E50(moby);
        }
        if (((Hero *)D_0013E633_2E0E50)->unk20B0 != 0) {
            float accel = frame_scale_sq_macro * 0.02f;
            float max_speed = frame_scale_sq_macro * 0.3f;

            ((Hero *)D_0013E633_2E0E50)->unk18F0[0].unkA0 = 1;
            ((Hero *)D_0013E633_2E0E50)->unk18F0[0].scale = 2.3f;
            ((Hero *)D_0013E633_2E0E50)->unk18F0[0].accel = accel;
            ((Hero *)D_0013E633_2E0E50)->unk18F0[0].max_speed = max_speed;
            if (!((unsigned int)(((Hero *)D_0013E633_2E0E50)->state.current - 0x38) < 2)) {
                float offset_y = 1024.0f / moby->scale * -0.2f;

                ((Hero *)D_0013E633_2E0E50)->unk18F0[1].accel = accel;
                ((Hero *)D_0013E633_2E0E50)->unk18F0[1].max_speed = max_speed;
                ((Hero *)D_0013E633_2E0E50)->unk18F0[2].max_speed = max_speed;
                ((Hero *)D_0013E633_2E0E50)->unk18F0[1].unkA0 = 2;
                ((Hero *)D_0013E633_2E0E50)->unk18F0[2].unkA0 = 3;
                ((Hero *)D_0013E633_2E0E50)->unk18F0[2].accel = accel;
                ((Hero *)D_0013E633_2E0E50)->unk18F0[1].pos_target.f[1] = offset_y;
                ((Hero *)D_0013E633_2E0E50)->unk18F0[2].pos_target.f[1] = offset_y;
            }
        }
    }
    switch (moby->state) {
    case HYDRODISPLACER_INITIALIZE:
        func_0022ED80_2E0E50(0, 0, moby);
        moby->state = 1;
        break;
    case HYDRODISPLACER_WAIT_FOR_ANIMATION_FLAG:
        if (moby->anim_flags & 2) {
            moby->state = 2;
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_ACTIVATION:
        if (func_L00_00267BA8_2E0E50(((Hero *)D_0013E633_2E0E50)->items[0].button_mask, func_001F9850_2E0E50(8), 0) == 0) {
            break;
        }
        if ((*(int *)D_0013A5E0_2E0E50 & ((Hero *)D_0013E633_2E0E50)->items[0].button_mask) == 0) {
            break;
        }
        if (((Hero *)D_0013E633_2E0E50)->ground_moby == 0 || ((Hero *)D_0013E633_2E0E50)->air_frames.s != 0
            || ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass != 0x155) {
            break;
        }
        if (func_L00_002E1790_2E0E50(moby) == 0 || (unsigned int)((Hero *)D_0013E633_2E0E50)->state.control_mode >= 2) {
            break;
        }
        moby->state = 3;
        func_L00_00222B80_2E0E50(0x38, 0);
        if (((Hero *)D_0013E633_2E0E50)->unk20B0 != 0) {
            func_L00_00232C10_2E0E50(0x83, 0, func_001F9850_2E0E50(6));
        } else {
            func_L00_00232C10_2E0E50(0x46, 0, func_001F9850_2E0E50(6));
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_HERO_FLAG: {
        int *platform;
        char *linked;

        if (((Hero *)D_0013E633_2E0E50)->ground_moby == 0 || ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass != 0x155) {
            func_L00_002E17F0_2E0E50(moby);
            return;
        }
        if (!(((Hero *)D_0013E633_2E0E50)->unkA98 & 2)) {
            break;
        }
        platform = (void *)((Hero *)D_0013E633_2E0E50)->ground_moby->pvars;
        linked = D_L00_00160098_2E0E50 + (platform[0] << 8);
        func_L00_00222B80_2E0E50(0x39, 0);
        if (((Hero *)D_0013E633_2E0E50)->unk20B0 != 0) {
            func_L00_00232C10_2E0E50(0x84, 0, func_001F9850_2E0E50(6));
        } else {
            func_L00_00232C10_2E0E50(0x47, 0, func_001F9850_2E0E50(6));
        }
        if (((Hero *)D_0013E633_2E0E50)->unk20B0 == 0) {
            func_0022ED80_2E0E50(0x14, 0, ((Hero *)D_0013E633_2E0E50)->moby);
        } else {
            func_0022ED80_2E0E50(0x15, 0, ((Hero *)D_0013E633_2E0E50)->moby);
        }
        if (platform[1] != -1 && platform[2] != -1) {
            moby->command = 0;
            moby->state = 4;
            data->f10 = 0.0f;
            func_L00_002EBF50_2E0E50(D_L00_00166EC0_2E0E50, D_L00_00166EC0_2E0E50 + 0x10, 1, 0, 0);
            if (((Hero *)D_0013E633_2E0E50)->unk20B0 != 0) {
                data->i8 = platform[1];
                data->iC = platform[2];
            } else {
                data->iC = platform[1];
                data->i8 = platform[2];
            }
            break;
        }
        moby->state = 5;
        data->i0 = func_001FA898_2E0E50(func_001F9878_2E0E50(D_L00_00161C84_2E0E50));
        if (((Hero *)D_0013E633_2E0E50)->unk20B0 == 0) {
            ((Moby *)linked)->command = 8;
            ((Hero *)D_0013E633_2E0E50)->unk20B0 = 1;
        } else {
            ((Moby *)linked)->command = 4;
            ((Hero *)D_0013E633_2E0E50)->unk20B0 = 0;
        }
        break;
    }
    case HYDRODISPLACER_CAMERA_TRANSITION:
        if (((Hero *)D_0013E633_2E0E50)->ground_moby == 0 || ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass != 0x155) {
            func_L00_002E17F0_2E0E50(moby);
            return;
        }
        switch (moby->command) {
        case HYDRODISPLACER_CAMERA_PREPARE_INTERPOLATION: {
            char *linked;

            func_00214D28_2E0E50(&D_L00_0015F4FC_2E0E50, 1.0f, 1.0f / func_001F9850_2E0E50(15));
            if (!(1.0f <= D_L00_0015F4FC_2E0E50)) {
                break;
            }
            linked = D_L00_00160098_2E0E50 + (*(int *)((Hero *)D_0013E633_2E0E50)->ground_moby->pvars << 8);
            moby->command = 1;
            func_L00_002EBE88_2E0E50(D_L00_001601AC_2E0E50 + (data->i8 << 7) + 0x30);
            func_L00_002EBEE0_2E0E50(D_L00_001601AC_2E0E50 + (data->i8 << 7) + 0x70);
            D_L00_0015F404_macro = 1;
            if (((Hero *)D_0013E633_2E0E50)->unk20B0 == 0) {
                ((Moby *)linked)->command = 8;
                ((Hero *)D_0013E633_2E0E50)->unk20B0 = 1;
            } else {
                ((Moby *)linked)->command = 4;
                ((Hero *)D_0013E633_2E0E50)->unk20B0 = 0;
            }
            break;
        }
        case HYDRODISPLACER_CAMERA_INTERPOLATE:
            if (data->f10 == 0.0f && 0.0f < D_L00_0015F4FC_2E0E50) {
                func_00214D28_2E0E50(&D_L00_0015F4FC_2E0E50, 0.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            if (data->f10 < 1.0f) {
                func_00214D88_2E0E50(&data->f10, &data->f14, 1.0f, frame_time_sq_macro * 0.666f, frame_time_sq_macro * 0.666f, D_0015EE6C_2E0E50 * 0.5f);
                func_001F9C08_2E0E50(&camera_pos, D_L00_001601AC_2E0E50 + (data->i8 << 7) + 0x30, D_L00_001601AC_2E0E50 + (data->iC << 7) + 0x30, data->f10);
                func_001F9BC0_2E0E50(&camera_rot);
                camera_rot.y = func_001FA790_2E0E50(((Node_2E0E50 *)D_L00_001601AC_2E0E50)[data->iC].v70.y, ((Node_2E0E50 *)D_L00_001601AC_2E0E50)[data->i8].v70.y) * data->f10;
                camera_rot.y = func_001FA748_2E0E50(((Node_2E0E50 *)D_L00_001601AC_2E0E50)[data->i8].v70.y, camera_rot.y);
                camera_rot.z = func_001FA790_2E0E50(((Node_2E0E50 *)D_L00_001601AC_2E0E50)[data->iC].v70.z, ((Node_2E0E50 *)D_L00_001601AC_2E0E50)[data->i8].v70.z) * data->f10;
                camera_rot.z = func_001FA748_2E0E50(((Node_2E0E50 *)D_L00_001601AC_2E0E50)[data->i8].v70.z, camera_rot.z);
                func_L00_002EBE88_2E0E50(&camera_pos);
                func_L00_002EBEE0_2E0E50(&camera_rot);
                return;
            }
            if (D_L00_0015F4FC_2E0E50 < 1.0f) {
                func_00214D28_2E0E50(&D_L00_0015F4FC_2E0E50, 1.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            moby->command = 2;
            D_L00_0015F404_macro = 0;
            func_L00_002EC0C8_2E0E50(3);
            return;
        case HYDRODISPLACER_CAMERA_RETURN_TO_ACTIVATION:
            if (0.0f < D_L00_0015F4FC_2E0E50) {
                func_00214D28_2E0E50(&D_L00_0015F4FC_2E0E50, 0.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            moby->state = 2;
            func_L00_00222B80_2E0E50(0x3A, 0);
            if (((Hero *)D_0013E633_2E0E50)->unk20B0 != 0) {
                func_L00_00232C10_2E0E50(0x48, 0, func_001F9850_2E0E50(6));
            } else {
                func_L00_00232C10_2E0E50(0x85, 0, func_001F9850_2E0E50(6));
            }
            return;
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_TIMER:
        if (((Hero *)D_0013E633_2E0E50)->ground_moby != 0 && ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass == 0x155
            && func_001F9908_2E0E50(data) == 0) {
            break;
        }
        func_L00_002E17F0_2E0E50(moby);
        moby->state = 2;
        break;
    }
}
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2e01b0;
extern float D_0015ED6C_2e01b0 __asm__("D_0015ED6C");
int FUN_001f9740_2e01b0(void *) __asm__("FUN_001f9740");
void add_vector_xyz_cf(char *, int, void *) __asm__("FUN_L00_0024f7c8");
float FUN_002132a8_2e01b0(float, float) __asm__("FUN_002132a8");
void fast_vec_normalize(V_2e01b0 *, void *, float) __asm__("FUN_001f9bf8");
int random_int_between(int, int) __asm__("FUN_L00_00257b90");
void FUN_L00_0026f548_2e01b0(V_2e01b0 *, V_2e01b0 *, int, int) __asm__("FUN_L00_0026f548");
int FUN_001f96f8_2e01b0(int) __asm__("FUN_001f96f8");
void FUN_L00_002e01b0(char *moby) {
    V_2e01b0 effect_position, v;
    char *moby_data = *(char **)(moby + 0x78);
    struct Hero *player;
    int countdown_lower_bound;
    float normalization_scale;
    if (FUN_001f9740_2e01b0(moby_data + 4) == 0)
        return;
    add_vector_xyz_cf(moby, 0, &effect_position);
    normalization_scale = -FUN_002132a8_2e01b0(D_0015ED6C_2e01b0, D_0015ED6C_2e01b0 * 3.0f);
    fast_vec_normalize(&v, moby + 0xD0, normalization_scale);
    player = &hero;
    v.x += player->motion.displacement.f[0] * 0.8f;
    v.y += player->motion.displacement.f[1] * 0.8f;
    v.z += player->motion.displacement.f[2] +
           FUN_002132a8_2e01b0(D_0015ED6C_2e01b0 * -0.5f, D_0015ED6C_2e01b0 + D_0015ED6C_2e01b0);
    FUN_L00_0026f548_2e01b0(&effect_position, &v, 2, random_int_between(0x5A, 0x78));
    countdown_lower_bound = FUN_001f96f8_2e01b0(5);
    *(int *)(moby_data + 4) = random_int_between(countdown_lower_bound, FUN_001f96f8_2e01b0(15));
}
extern u8 *D_L00_0015FFD8;

/* Tests flag 2 (or 1 when &hero.unk20B0 is set) at 0xBC of the moby selected by +0x2FC. */
s32 FUN_L00_002e02e0(void) {
    struct Hero *base;
    u8 *linked_moby;
    s32 ground_moby_address;
    s32 idx;
    u8 flags;
    base = &hero;
    linked_moby = D_L00_0015FFD8;
    ground_moby_address = ((s32)base->ground_moby);
    flags = (u8)base->unk20B0;
    linked_moby += *(s32 *)*(s32 *)((u8 *)ground_moby_address + 0x78) << 8;
    if (!flags) {
        if (linked_moby[0xBC] & 2)
            return 1;
    } else {
        if (linked_moby[0xBC] & 1)
            return 1;
    }
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E17F0), where it is exact; names translated to the US level program. */

extern int D_L00_0015F404;
extern short D_L00_0015F3FC;
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_002eac18(int);

/* Moby state 4: clears two globals, calls 002eac18(3); control mode 0x13: calls 002223f8(0, 1). */
void FUN_L00_002e0340(char *moby) {
    if (*(unsigned char *)(moby + 0x20) == 4) {
        D_L00_0015F404 = 0;
        *(int *)&D_L00_0015F3FC = 0;
        FUN_L00_002eac18(3);
    }
    if (hero.state.control_mode == 0x13) {
        hero_set_state(HERO_STATE_IDLE, 1);
    }
}
typedef struct {
    u8 pad[0xB0];
    u8 b0;
    u8 pad1;
    s16 h;
    u8 pad2[0x4C];
} E_2e03a0_2e03a0;
typedef struct {
    s32 fa_2e03a0;
    s32 b;
} P_2e03a0_2e03a0;
typedef struct {
    u8 pad[0x18];
    f32 z;
    u8 pad1[4];
    u8 state;
    u8 pad2[0x13];
    u16 fl;
    u8 pad3[0x12];
    f32 ang;
    u8 pad4[0x2C];
    P_2e03a0_2e03a0 *p;
    u8 pad5[0x34];
    u8 b0;
    u8 pad6;
    s16 h;
} M_2e03a0_2e03a0;
extern f32 D_0015ED6C_2e03a0 __asm__("D_0015ED6C");
extern s32 D_L00_0015F5C4_2e03a0 __asm__("D_L00_0015F5C4");
extern s32 D_L00_0015F640 __asm__("D_L00_0015F640");
extern E_2e03a0_2e03a0 *D_L00_0015FFD8_2e03a0 __asm__("D_L00_0015FFD8");
typedef struct {
    u8 pad[0x454];
    u8 arr[1];
} T_2e03a0_2e03a0;
extern u8 mission_state_macro[] __asm__("D_L00_0015FC88") __attribute__((section(".sdata")));
extern u8 D_0014C050_2e03a0[] __asm__("D_0014C050");
extern s32 D_00141410_2e03a0[] __asm__("D_00141410");
extern u8 item_available[] __asm__("D_0013D4C0");
extern f32 FUN_001fa580_2e03a0(f32, f32) __asm__("FUN_001fa580");
extern void FUN_L00_002d6bf0_2e03a0(void *) __asm__("FUN_L00_002d6bf0");
extern void stub_printf() __asm__("FUN_001e93b0");
extern void FUN_0020c828_2e03a0(void *) __asm__("FUN_0020c828");
extern void FUN_L00_002e0788_2e03a0(void *) __asm__("FUN_L00_002e0788");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 FUN_001f99c0_2e03a0(f32) __asm__("FUN_001f99c0");
extern void start_scene(s32) __asm__("FUN_L00_00298840");
extern void queue_dialogue_message(s32, s32) __asm__("FUN_L00_00263d40");
extern void FUN_L00_00260860_2e03a0(s32, s32) __asm__("FUN_L00_00260860");
extern void give_reward(s32, s32) __asm__("FUN_L00_00203908");
extern void FUN_L00_002502a0_2e03a0(s32) __asm__("FUN_L00_002502a0");
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");

extern u8 D_L00_001EA1B0[];
extern u8 D_L00_001EA1D8[];
extern u8 D_L00_001EA200[];
extern u8 D_L00_00161BC0[];
extern u16 D_00141B08[];
extern T_2e03a0_2e03a0 D_L00_001BA5D0;
extern T_2e03a0_2e03a0 D_L00_001BB230;
extern s32 current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));
void pilots_helmet_update(M_2e03a0_2e03a0 *m) __asm__("FUN_L00_002e03a0");
void pilots_helmet_update(M_2e03a0_2e03a0 *moby) {
    enum {
        PILOTS_HELMET_INITIALIZE = 0,
        PILOTS_HELMET_WAIT_FOR_HERO_PROXIMITY = 1,
        PILOTS_HELMET_GRANT_REWARD = 2,
        PILOTS_HELMET_SAVE_AND_REMOVE = 3
    };
    P_2e03a0_2e03a0 *p = moby->p;
    char *hero_data;
    E_2e03a0_2e03a0 *e;
    u32 k;
    s32 n;

    moby->ang = FUN_001fa580_2e03a0(moby->ang, D_0015ED6C_2e03a0 * 1.5707964f);
    switch (moby->state) {
    case PILOTS_HELMET_INITIALIZE:
        FUN_L00_002d6bf0_2e03a0(moby);
        if (p->fa_2e03a0 == -1)
            stub_printf(D_L00_001EA1B0, moby->h);
        if (item_available[7]) {
            stub_printf(D_L00_001EA1D8);
            FUN_0020c828_2e03a0(moby);
            return;
        }
        moby->state = 1;
        moby->z += 1.0f;
        break;
    case PILOTS_HELMET_WAIT_FOR_HERO_PROXIMITY:
        FUN_L00_002e0788_2e03a0(moby);
        hero_data = (char *)&hero.motion.pos;
        if (vector_distance((char *)moby + 0x10, hero_data) < 3.0f) {
            hero_data -= 0x80;
            if (FUN_001f99c0_2e03a0(moby->z - *(f32 *)(hero_data + 0x88)) < 2.0f && *(s32 *)(hero_data + 0x22A8)) {
                moby->fl |= 0x41;
                if (p->fa_2e03a0 != -1)
                    start_scene(p->fa_2e03a0);
                else
                    stub_printf(D_L00_001EA1B0, moby->h);
                moby->state = 2;
            }
        }
        break;
    case PILOTS_HELMET_GRANT_REWARD:
        if (D_L00_0015F5C4_2e03a0 == 2)
            return;
        stub_printf(D_L00_00161BC0);
        queue_dialogue_message(0x232A, -1);
        D_L00_0015F640 = 0xB4;
        FUN_L00_00260860_2e03a0(7, 1);
        if (D_00141B08[0] == 0)
            give_reward(0x2328, 0x34);
        if (p->b != -1) {
            D_L00_001BA5D0.arr[D_L00_0015FFD8_2e03a0[p->b].h] =
                D_L00_0015FFD8_2e03a0[p->b].b0 + 2;
            if (D_L00_0015FFD8_2e03a0[p->b].b0 == 0xFF ||
                (mission_state_macro[D_L00_0015FFD8_2e03a0[p->b].b0] != 0xFF &&
                 D_0014C050_2e03a0[D_L00_0015FFD8_2e03a0[p->b].b0 + current_level_index_gp * 16] ==
                     0xFF)) {
                D_L00_001BB230.arr[D_L00_0015FFD8_2e03a0[p->b].h] =
                    D_L00_0015FFD8_2e03a0[p->b].b0 + 2;
            }
        } else {
            stub_printf(D_L00_001EA200, moby->h);
        }
        if (moby->b0 != 0xFF)
            FUN_L00_002502a0_2e03a0(moby->b0);
        D_00141410_2e03a0[0] = 7;
        moby->state = 3;
        break;
    case PILOTS_HELMET_SAVE_AND_REMOVE:
        memcard_save_data(0, -1);
        FUN_0020c828_2e03a0(moby);
        break;
    }
}
extern char D_L00_00166DC0[] __attribute__((section(".data")));
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vscl_2e0788(void *, void *, float) __asm__("FUN_001f9bf8");
extern int f9740_2e0788(void *) __asm__("FUN_001f9740");
extern int ticks_2e0788(int) __asm__("FUN_001f96f8");
extern float i2f_2e0788(int) __asm__("FUN_001fa6c0");
extern float f99c0_2e0788(float) __asm__("FUN_001f99c0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int f2i_2e0788(float) __asm__("FUN_001fa6d0");
extern unsigned char *spr_2e0788(OvlQuad *, int, unsigned char, int, int, int, int,
                                 float) __asm__("FUN_L00_00272f68");
typedef struct {
    char pad[0x10];
    float a[4];
    float b[4];
    int t[4];
    float c[4];
} M_2e0788;
void FUN_L00_002e0788(unsigned char *moby) {
    OvlQuad sprite_position_and_step[2];
    M_2e0788 *moby_data = *(M_2e0788 **)(moby + 0x78);
    float *a;
    int *t;
    int i, j, c;
    float pre_wrap_sum, color_cycle_progress;
    j = 0;
    moby += 0x10;
    fast_vec_sub(sprite_position_and_step, D_L00_00166DC0, moby);
    t = moby_data->t;
    a = moby_data->a;
    vscl_2e0788(sprite_position_and_step, sprite_position_and_step, -0.3f);
    vscl_2e0788(&sprite_position_and_step[1], sprite_position_and_step, 0.1f);
    fast_vec_add(sprite_position_and_step, sprite_position_and_step, moby);
    for (i = 3; i >= 0; i--) {
        pre_wrap_sum = *a + moby_data->b[j];
        *a = pre_wrap_sum;
        if (pre_wrap_sum >= 255.0f) {
            *a = pre_wrap_sum - 255.0f;
        } else if (pre_wrap_sum <= 0.0f) {
            *a = pre_wrap_sum + 255.0f;
        }
        if (f9740_2e0788(t)) {
            moby_data->t[j] = ticks_2e0788(0xff);
        }
        t++;
        color_cycle_progress = i2f_2e0788(ticks_2e0788(0xff) - moby_data->t[j]);
        color_cycle_progress = color_cycle_progress / (float)ticks_2e0788(0xff);
        c = fast_tween_color(0x4040FFFF, 0x1040FFFF, f99c0_2e0788(0.5f - color_cycle_progress));
        spr_2e0788(sprite_position_and_step, c, f2i_2e0788(*a++), 0x35, 1, 2, 0, moby_data->c[j]);
        j++;
        fast_vec_add(sprite_position_and_step, sprite_position_and_step, &sprite_position_and_step[1]);
    }
}
typedef int s32_2e1f28;
extern s32_2e1f28 D_BF8_2e1f28[] __asm__("D_L00_00161BF8") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C08_2e1f28[] __asm__("D_L00_00161C08") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C18_2e1f28[] __asm__("D_L00_00161C18") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C28_2e1f28[] __asm__("D_L00_00161C28") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C38_2e1f28[] __asm__("D_L00_00161C38") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C48_2e1f28[] __asm__("D_L00_00161C48") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C58_2e1f28[] __asm__("D_L00_00161C58") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C68_2e1f28[] __asm__("D_L00_00161C68") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C78_2e1f28[] __asm__("D_L00_00161C78") __attribute__((section(".sdata")));
extern s32_2e1f28 D_C88_2e1f28[] __asm__("D_L00_00161C88") __attribute__((section(".sdata")));
s32_2e1f28 FUN_L00_002e1f28(s32_2e1f28 id, s32_2e1f28 a, s32_2e1f28 b);
s32_2e1f28 FUN_L00_002e1f28(s32_2e1f28 id, s32_2e1f28 endpoint_effect_flags, s32_2e1f28 endpoint_spark_tag_mode) {
    s32_2e1f28 i, j;
    for (i = 0; i < 3; i++) {
        if (D_C88_2e1f28[i] == id)
            return 1;
    }
    for (j = 0; j < 3; j++) {
        if (D_C88_2e1f28[j] == 0) {
            D_C88_2e1f28[j] = id;
            D_C08_2e1f28[j] = 0;
            D_C18_2e1f28[j] = 0;
            D_C28_2e1f28[j] = 0;
            D_C38_2e1f28[j] = -1;
            D_C48_2e1f28[j] = -1;
            D_C58_2e1f28[j] = endpoint_effect_flags;
            D_C68_2e1f28[j] = endpoint_spark_tag_mode;
            D_C78_2e1f28[j] = 0;
            D_BF8_2e1f28[j] = 1;
            return 1;
        }
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E34F0), where it is exact; names translated to the US level program. */

extern s32 D_L00_00161BF8_c[] __asm__("D_L00_00161BF8") __attribute__((section(".sdata")));
extern s32 D_L00_00161C38_c[] __asm__("D_L00_00161C38") __attribute__((section(".sdata")));
extern s32 D_L00_00161C48_c[] __asm__("D_L00_00161C48") __attribute__((section(".sdata")));
extern s32 D_L00_00161C88_c[] __asm__("D_L00_00161C88") __attribute__((section(".sdata")));
extern void FUN_L00_0023e838(int);

int FUN_L00_002e2040(int id);
int FUN_L00_002e2040(int id) {
    unsigned int i;
    for (i = 0; (int)i < 3; i++) {
        if (D_L00_00161C88_c[i] == id) {
            D_L00_00161BF8_c[i] = 0;
            D_L00_00161C88_c[i] = 0;
            if (D_L00_00161C38_c[i] != -1) {
                FUN_L00_0023e838(D_L00_00161C38_c[i]);
                D_L00_00161C38_c[i] = -1;
            }
            if (D_L00_00161C48_c[i] != -1) {
                FUN_L00_0023e838(D_L00_00161C48_c[i]);
                D_L00_00161C48_c[i] = -1;
            }
            return 1;
        }
    }
    return 0;
}
#include "sda.h"

extern s32 D_L00_00161BF8[] MACRO_ADDR;
extern s32 D_L00_00161C38[] MACRO_ADDR;
extern s32 D_L00_00161C48[] MACRO_ADDR;
extern s32 D_L00_00161C88[] MACRO_ADDR;
extern void FUN_L00_0023e838(s32);

void FUN_L00_002e20f8(s32 i) {
    s32 first_endpoint_light_index;
    first_endpoint_light_index = D_L00_00161C38[i];
    D_L00_00161BF8[i] = 0;
    D_L00_00161C88[i] = 0;
    if (first_endpoint_light_index != -1) {
        FUN_L00_0023e838(first_endpoint_light_index);
        D_L00_00161C38[i] = -1;
    }
    if (D_L00_00161C48[i] != -1) {
        FUN_L00_0023e838(D_L00_00161C48[i]);
        D_L00_00161C48[i] = -1;
    }
}
#include "qcopy.h"
#include "sda.h"
typedef struct {
    u8 b[0x40];
} B002e2190;
typedef struct {
    u8 b[0x10];
} Q002e2190;
extern B002e2190 D_L00_001E6D20[];
extern Q002e2190 D_L00_001E6DE0[];
extern s32 D_L00_00161BF8[] MACRO_ADDR;
extern s32 D_L00_00161C88[] MACRO_ADDR;
void fast_mem_copy(void *, void *, s32) __asm__("FUN_001f98d0");
s32 FUN_L00_002e2190(s32 id, void *src, void *v);
s32 FUN_L00_002e2190(s32 id, void *src, void *endpoint_position) {
    s32 i;
    for (i = 0; i < 3; i++) {
        if (id == D_L00_00161C88[i]) {
            if (D_L00_00161BF8[i] == 1)
                D_L00_00161BF8[i] = 2;
            else if (D_L00_00161BF8[i] == 4)
                D_L00_00161BF8[i] = 3;
            fast_mem_copy(&D_L00_001E6D20[i], src, 0x40);
            qcopy(&D_L00_001E6DE0[i], endpoint_position);
            return 1;
        }
    }
    return 0;
}
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

/* Per-frame update of one beam slot: lays out the two 20-point strips between the slot's ends
   (a sine wave and a noisy one), spawns sparks at both ends and keeps the two looping sounds
   at the ends. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E3700), where it is exact; names translated to the US level program. */

typedef struct { unsigned char b[0x40]; } B002E3640;

typedef struct { unsigned char b[0x10]; } Q002E3640;

typedef float Vec_2E3700[4] __attribute__((aligned(16)));

typedef struct { float x, y, z, w; } Pt_2E3700;

typedef struct { char pad0[0x30]; float v30[4]; } Slot_2E3700;

typedef struct { float v[4]; char pad10[0x30]; } End_2E3700;

typedef struct { float v[4]; } Q_2E3700;

typedef struct {
    char pad0[0x10];
    float f10;
    short f14;
    short f16;
    int f18;
} PartSub_2E3700;

typedef struct {
    char pad0[0xA];
    short fA;
    float fC;
    char pad10[0x10];
    PartSub_2E3700 sub;
} Part_2E3700;

typedef struct { char pad0[0x10]; float v10[3]; float f1C; } Snd_2E3700;

typedef struct { char b[0x140]; } Pane_2e3fa0;

typedef struct {
    f32 v[4];
    u8 pad[0x30];
} T_e3388;

extern Pane_2e3fa0 D_L00_001E6E40[];
extern char D_L00_0015F580[] __attribute__((section(".sdata")));
extern f32 convert_integer_to_float_c2(s32) __asm__("func_001FA6C0");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations_c(f32, f32) __asm__("func_001FA5C8");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_c(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
f32 FUN_L00_00257c48(f32, f32);
f32 normalize_angle(f32 a) __asm__("FUN_L00_0025e310");
int FUN_L00_0023e738(void *v, float a, float b_0023e738, float c, float d, float e);
unsigned char *FUN_L00_00274948(void *a, void *b, int c, int d);
void random_spherical_offset(void *out, f32 a, f32 b) __asm__("FUN_00213358");
extern Slot_2E3700 D_L00_001E7070_2E3700[] __asm__("D_L00_001E6D20");
extern End_2E3700 D_L00_001E70A0_2E3700[] __asm__("D_L00_001E6D50");
extern Q_2E3700 D_L00_001E7130_2E3700[] __asm__("D_L00_001E6DE0");
extern Q_2E3700 D_L00_001E7160_2E3700[] __asm__("D_L00_001E6E10");
extern Pt_2E3700 D_L00_001E7190_2E3700[][20] __asm__("D_L00_001E6E40");
extern Snd_2E3700 D_L00_001803C0_2E3700[] __asm__("D_L00_001802C0");
extern char D_0013E633_2E3700[] __asm__("D_0013F5E0");
extern char D_L00_0015F660_2E3700[] __asm__("D_L00_0015F580") __attribute__((section(".sdata")));
extern int D_L00_00161CC8_2E3700[] __asm__("D_L00_00161BF8") __attribute__((section(".sdata")));
extern float D_L00_00161CD8_2E3700[] __asm__("D_L00_00161C08") __attribute__((section(".sdata")));
extern float D_L00_00161CE8_2E3700[] __asm__("D_L00_00161C18") __attribute__((section(".sdata")));
extern float D_L00_00161CF8_2E3700[] __asm__("D_L00_00161C28") __attribute__((section(".sdata")));
extern int D_L00_00161D08_2E3700[] __asm__("D_L00_00161C38") __attribute__((section(".sdata")));
extern int D_L00_00161D18_2E3700[] __asm__("D_L00_00161C48") __attribute__((section(".sdata")));
extern int D_L00_00161D28_2E3700[] __asm__("D_L00_00161C58") __attribute__((section(".sdata")));
extern int D_L00_00161D38_2E3700[] __asm__("D_L00_00161C68") __attribute__((section(".sdata")));
extern int D_L00_00161D58_2E3700[] __asm__("D_L00_00161C88") __attribute__((section(".sdata")));
extern float D_L00_00161CC0_2E3700 __asm__("D_L00_00161BF0") __attribute__((sda));
extern void func_001F9BF0_2E3700(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float func_001FA748_2E3700(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_2E3700(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA888_2E3700(int) __asm__("FUN_001fa6c0");
extern float func_L00_0025F368_2E3700(float) __asm__("FUN_L00_0025e310");
extern float func_001F9FA8_2E3700(float) __asm__("FUN_001f9de0");
extern float func_001F9F90_2E3700(float) __asm__("FUN_001f9dc8");
extern void func_L00_001FF4B0_2E3700(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9CA0_2E3700(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void func_001F9EE8_2E3700(void *, void *, void *) __asm__("FUN_001f9d20");
extern void func_001F9EC0_2E3700(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float func_L00_00258C80_2E3700(float, float) __asm__("FUN_L00_00257c48");
extern float func_002140F8_2E3700(float, float) __asm__("FUN_002132a8");
extern Part_2E3700 *func_L00_002757E8_2E3700(void *, void *, int, int) __asm__("FUN_L00_00274948");
extern int func_002140B0_2E3700(int) __asm__("FUN_00213260");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9BD8_2E3700(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F9850_2E3700(int) __asm__("FUN_001f96f8");
extern void func_002141A8_2E3700(void *, float, float) __asm__("FUN_00213358");
extern int func_L00_0023F0D0_2E3700(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");

void FUN_L00_002e2250(int unused, int beam) {
    Vec_2E3700 span;
    Vec_2E3700 beam_direction;
    Vec_2E3700 side;
    Vec_2E3700 up;
    Vec_2E3700 from;
    Vec_2E3700 span2;
    Vec_2E3700 tip;
    Vec4 particle_velocity;
    Vec_2E3700 pos;
    int strand = beam * 2;
    int next_strand = strand + 1;
    float step;
    float phase_step;
    float one;
    Pt_2E3700 *point;
    int i;
    int j;
    int second_strand_point_index;

    func_001F9BF0_2E3700(span, &D_L00_001E7130_2E3700[beam], &D_L00_001E70A0_2E3700[beam]);
    step = fast_vec_length(span) / 20.0f;
    if (D_L00_00161CC8_2E3700[beam] == 2) {
        D_L00_00161CC8_2E3700[beam] = 3;
        for (i = 0; i < 20; i++) {
            Pt_2E3700 *first = &D_L00_001E7190_2E3700[strand][i];
            Pt_2E3700 *second = &D_L00_001E7190_2E3700[next_strand][i];

            first->x = step * (float)i;
            first->y = first->z = 0.0f;
            first->w = 1.0f;
            second->x = step * (float)i;
            second->y = second->z = 0.0f;
            second->w = 1.0f;
        }
    }
    D_L00_00161CD8_2E3700[beam] = func_001FA748_2E3700(D_L00_00161CD8_2E3700[beam], 0.17453292f);
    D_L00_00161CE8_2E3700[beam] = func_001FA790_2E3700(D_L00_00161CE8_2E3700[beam], 0.80285144f);
    D_L00_00161CF8_2E3700[beam] = func_001FA748_2E3700(D_L00_00161CF8_2E3700[beam], 0.05236f);
    phase_step = 360.0f / (func_001FA888_2E3700(20) * 0.5f) * 0.017453292f;
    for (j = 0; j < 19; j++) {
        float fj = (float)j;
        float amplitude = func_001FA888_2E3700(j) / func_001FA888_2E3700(20) * 0.3f + 0.1f;
        Pt_2E3700 *point = &D_L00_001E7190_2E3700[strand][j];

        amplitude = amplitude * func_001F9FA8_2E3700(func_L00_0025F368_2E3700(D_L00_00161CD8_2E3700[beam] + fj * phase_step));
        point->y = 0.0f;
        point->w = 1.0f;
        point->z = amplitude;
        point->x = step * fj;
    }
    qcopy_nc(from, &D_L00_001E70A0_2E3700[beam]);
    one = 1.0f;
    func_001F9BF0_2E3700(span2, &D_L00_001E7130_2E3700[beam], from);
    func_L00_001FF4B0_2E3700(beam_direction, span2, one);
    func_001F9CA0_2E3700(side, beam_direction, D_0013E633_2E3700);
    func_L00_001FF4B0_2E3700(side, side, -1.0f);
    func_001F9CA0_2E3700(up, side, beam_direction);
    func_001F9EE8_2E3700(tip, &D_L00_001E7190_2E3700[strand][18], beam_direction);
    qcopy(&D_L00_001E7160_2E3700[beam], tip);
    point = D_L00_001E7190_2E3700[next_strand];
    for (second_strand_point_index = 0; second_strand_point_index < 19; second_strand_point_index++) {
        float fn = (float)second_strand_point_index;
        float twist;
        float swell;
        float height;
        float ripple;
        float random;

        point->z = 0.0f;
        twist = func_L00_0025F368_2E3700(D_L00_00161CE8_2E3700[beam] + fn * 0.99483764f);
        swell = func_L00_0025F368_2E3700(D_L00_00161CF8_2E3700[beam] + fn * 0.20944f);
        height = func_001F9FA8_2E3700(swell) * 0.5f;
        ripple = func_001F9FA8_2E3700(twist);
        random = func_L00_00258C80_2E3700(0.0f, 0.2f);
        point->z += height * ripple + random;
        point->y = func_001F9F90_2E3700(twist) * 0.3f;
        point->w = 1.0f;
        point->x = step * fn;
        point++;
    }
    {
        float one;
        float speed = func_002140F8_2E3700(0.01f, 0.05f);
        float angle;

        one = 1.0f;
        angle = func_L00_00258C80_2E3700(0.34906584f, 0.6981317f);
        particle_velocity.q = 0;
        particle_velocity.f[0] = one;
        particle_velocity.f[3] = one;
        particle_velocity.f[0] = speed * func_001F9F90_2E3700(angle);
        particle_velocity.f[1] = speed * func_001F9FA8_2E3700(angle);
        func_001F9EC0_2E3700(&particle_velocity, &particle_velocity, &D_L00_001E7070_2E3700[beam]);
        func_L00_002757E8_2E3700(D_L00_001E7070_2E3700[beam].v30, &particle_velocity, 0x7F, D_L00_00161D58_2E3700[beam]);
        if (func_002140B0_2E3700(3) == 0) {
            Part_2E3700 *spark;

            fast_vec_scale(pos, &D_L00_001E7070_2E3700[beam], 0.3f);
            func_001F9BD8_2E3700(pos, pos, D_L00_001E7070_2E3700[beam].v30);
            spark = func_L00_002757E8_2E3700(pos, D_L00_0015F660_2E3700, 0x7F, D_L00_00161D58_2E3700[beam]);
            if (spark != 0) {
                PartSub_2E3700 *sub = &spark->sub;

                spark->fC = 120000.0f;
                spark->fA = func_001F9850_2E3700(4);
                sub->f10 = one / func_001FA888_2E3700(spark->fA);
                sub->f18 = 0x7F7F7F;
                if (D_L00_00161D38_2E3700[beam] == 1) sub->f16 = 1;
            }
        }
        func_002140F8_2E3700(0.01f, 0.05f);
        func_002141A8_2E3700(&particle_velocity, 0.01f, 0.05f);
        func_L00_002757E8_2E3700(tip, &particle_velocity, 0x7F, D_L00_00161D58_2E3700[beam]);
        if (func_002140B0_2E3700(3) == 0) {
            Part_2E3700 *spark = func_L00_002757E8_2E3700(tip, D_L00_0015F660_2E3700, 0x7F, D_L00_00161D58_2E3700[beam]);

            if (spark != 0) {
                PartSub_2E3700 *sub = &spark->sub;

                spark->fC = 120000.0f;
                spark->fA = func_001F9850_2E3700(4);
                sub->f10 = 1.0f / func_001FA888_2E3700(spark->fA);
                sub->f18 = 0x7F7F7F;
                if (D_L00_00161D38_2E3700[beam] == 1) sub->f16 = 2;
            }
        }
    }
    if (D_L00_00161D28_2E3700[beam] != 0) {
        if (D_L00_00161D28_2E3700[beam] & 1) {
            qcopy(pos, &D_L00_001E70A0_2E3700[beam]);
            pos[2] -= 0.25f;
            if (D_L00_00161D08_2E3700[beam] == -1) {
                D_L00_00161D08_2E3700[beam] = func_L00_0023F0D0_2E3700(pos, D_L00_00161CC0_2E3700, 0.0f, 1.0f, 1.0f, 2.0f);
            }
            if (D_L00_00161D08_2E3700[beam] >= 0) {
                Snd_2E3700 *light = &D_L00_001803C0_2E3700[D_L00_00161D08_2E3700[beam]];

                qcopy(light->v10, pos);
                light->f1C = D_L00_00161CC0_2E3700;
            }
        }
        if (D_L00_00161D28_2E3700[beam] & 2) {
            tip[2] += 0.5f;
            if (D_L00_00161D18_2E3700[beam] == -1) {
                D_L00_00161D18_2E3700[beam] = func_L00_0023F0D0_2E3700(tip, D_L00_00161CC0_2E3700, 0.0f, 1.0f, 1.0f, 2.0f);
            }
            if (D_L00_00161D18_2E3700[beam] >= 0) {
                Snd_2E3700 *light = &D_L00_001803C0_2E3700[D_L00_00161D18_2E3700[beam]];

                qcopy(light->v10, tip);
                light->f1C = D_L00_00161CC0_2E3700;
            }
        }
    }
}
/* Scrolls and draws the two panes of each active slot (3 slots). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E3FA0), where it is exact; names translated to the US level program. */

extern Pane_2e3fa0 D_L00_001E6E40[];
extern float D_L00_00161C78_f[] __asm__("D_L00_00161C78") __attribute__((section(".sdata")));
extern int D_L00_00161BF8_c2[] __asm__("D_L00_00161BF8") __attribute__((section(".sdata")));
extern int D_L00_00161C88_c2[] __asm__("D_L00_00161C88") __attribute__((section(".sdata")));
extern void FUN_L00_002e2c88(int, int, void *, int, int, int);
extern void FUN_L00_002e3388_c(int, int) __asm__("FUN_L00_002e3388");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

void FUN_L00_002e2af0(char *moby) {
    int *moby_data = *(int **)(moby + 0x78);
    int i;
    vu1_add_g_sregister(0x47, 0x51001);
    for (i = 0; i < 3; i++) {
        float scroll;
        int texture_id, glow_texture_id;
        int pane, next_pane;
        if (D_L00_00161C88_c2[i] == 0 || D_L00_00161BF8_c2[i] < 2) continue;
        pane = i << 1;
        next_pane = pane + 1;
        scroll = D_L00_00161C78_f[i] - frame_scale * 0.2f;
        D_L00_00161C78_f[i] = scroll;
        if (scroll <= -8.0f) D_L00_00161C78_f[i] = scroll + 8.0f;
        FUN_L00_002e3388_c(D_L00_00161C88_c2[i], i);
        texture_id = moby_data[1] + 0x28;
        glow_texture_id = moby_data[0] + 0x28;
        FUN_L00_002e2c88(texture_id, glow_texture_id, &D_L00_001E6E40[pane], 0x80, 0x40, i);
        FUN_L00_002e2c88(texture_id, glow_texture_id, &D_L00_001E6E40[next_pane], 0x30, 0x10, i);
    }
}
/* Builds the trail quad and its glow quad from the slot tables and draws them along the
   19 steps of points. */
#include "rnc/rendering/geometry_quad.h"
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E4138). */
typedef struct { OvlVec4 axis, unk10, position, unk30; } TrailSlot;
extern TrailSlot D_L00_001E6D30[];
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void fast_draw_quad_real(void *, int, int) __asm__("FUN_001f7d30");
void FUN_L00_002e2c88(int texture_id, int glow_texture_id, void *points, int alpha, int glow_alpha, int slot)
{
    struct GeometryQuad quads[2];
    OvlVec4 width, glow_width;
    OvlVec4 basis[4];
    OvlVec4 transform[4];
    OvlVec4 direction, from, to;
    float zero, half_width, uv_offset;
    int color, glow_color;
    int i;

    quads[0].texture = get_effect_texture(texture_id);
    color = (alpha << 24) | 0x7F7F7F;
    quads[0].primitive = 0x8000000048L;
    quads[0].texture_state = 0xFF9000000260L;
    quads[0].reserved.value = 0;
    quads[0].colors[0] = quads[0].colors[1] = quads[0].colors[2] = quads[0].colors[3] = color;
    quads[1].texture = get_effect_texture(glow_texture_id);
    *(int *)&quads[1].texture_coordinates[0][1] = 0;
    glow_color = (glow_alpha << 24) | 0x7F4040;
    zero = quads[1].texture_coordinates[0][1];
    uv_offset = *(float *)&D_C78_2e1f28[slot];
    quads[1].colors[0] = quads[1].colors[1] = quads[1].colors[2] = quads[1].colors[3] = glow_color;
    quads[1].primitive = quads[0].primitive;
    quads[1].texture_state = quads[0].texture_state;
    quads[1].reserved.value = quads[0].reserved.value;
    quads[1].texture_coordinates[0][0] = 1.0f;
    quads[1].texture_coordinates[1][0] = 1.0f;
    quads[1].texture_coordinates[1][1] = 1.0f;
    quads[1].texture_coordinates[2][0] = 0.0f;
    quads[1].texture_coordinates[2][1] = 0.0f;
    quads[1].texture_coordinates[3][0] = 0.0f;
    quads[1].texture_coordinates[3][1] = 1.0f;
    quads[0].texture_coordinates[1][0] = quads[0].texture_coordinates[0][0] = uv_offset + zero;
    quads[0].texture_coordinates[0][1] = zero;
    quads[0].texture_coordinates[1][1] = 1.0f;
    quads[0].texture_coordinates[3][0] = quads[0].texture_coordinates[2][0] = uv_offset + 1.0f;
    quads[0].texture_coordinates[2][1] = zero;
    quads[0].texture_coordinates[3][1] = 1.0f;
    half_width = 0.6f;
    if (*(short *)(D_L00_00161C88[slot] + 0xA6) == 0x1C) half_width = 0.3f;
    width.q = 0;
    glow_width.q = 0;
    glow_width.f[2] = half_width + half_width;
    width.f[3] = 1.0f;
    width.f[2] = half_width;
    glow_width.f[3] = 1.0f;

    qcopy(&basis[0], &D_L00_001E6D30[slot].axis);
    subtract_vector_xyz(&basis[1], D_L00_00166DC0, &D_L00_001E6D30[slot].position);
    fast_vec_scale(&basis[1], &basis[1], 1.0f);
    cross_vectors_xyz(&basis[2], &basis[1], &basis[0]);
    normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
    cross_vectors_xyz(&basis[0], &basis[2], &basis[1]);
    qcopy(&basis[3], &D_L00_001E6D30[slot].position);
    qcopy(&transform[3], &D_L00_001E6D30[slot].position);
    subtract_vector_xyz(&direction, &D_L00_001E6DE0[slot], &transform[3]);
    normalize_vector_xyz(&transform[0], &direction, 1.0f);
    cross_vectors_xyz(&transform[1], &transform[0], D_0013E633_2E3700);
    normalize_vector_xyz(&transform[1], &transform[1], -1.0f);
    cross_vectors_xyz(&transform[2], &transform[1], &transform[0]);

    qcopy(&quads[0].positions[0], &width);
    fast_vec_scale(&quads[0].positions[1], &quads[0].positions[0], -1.0f);
    {
        /* Fakematch: plain C copies &glow_width here and in the loop and stores glow_width.f[2];
           the pointer copies, the empty loop (it stops regmove) and the dead reset in the loop
           make 2.95.2 copy the pointer to $v1 before this qcopy, as retail does. */
        OvlVec4 *glow_src = &glow_width;
        OvlVec4 *glow;
        {
            OvlVec4 *glow_dst = &quads[1].positions[0];
            glow = glow_src;
            /* Fakematch: the empty loop stops regmove (see above). */
            do { } while (0);
            qcopy(glow_dst, glow);
        }
        fast_vec_scale(&quads[1].positions[1], &quads[1].positions[0], -1.0f);
        transform_vector_c(&from, &((OvlVec4 *)points)[0], transform);
        transform_vector_c(&to, &((OvlVec4 *)points)[1], transform);
        subtract_vector_xyz(&basis[0], &to, &from);
        normalize_vector_xyz(&basis[0], &basis[0], 1.0f);
        subtract_vector_xyz(&basis[1], D_L00_00166DC0, &from);
        cross_vectors_xyz(&basis[2], &basis[1], &basis[0]);
        normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
        cross_vectors_xyz(&basis[1], &basis[0], &basis[2]);
        qcopy(&basis[3], &from);
        transform_vector_c(&quads[0].positions[0], &quads[0].positions[0], basis);
        transform_vector_c(&quads[0].positions[1], &quads[0].positions[1], basis);
        transform_vector_c(&quads[1].positions[0], &quads[1].positions[0], basis);
        transform_vector_c(&quads[1].positions[1], &quads[1].positions[1], basis);

        i = 1;
        do {
            float taper, scroll;
            qcopy(&quads[0].positions[2], &width);
            fast_vec_scale(&quads[0].positions[3], &quads[0].positions[2], -1.0f);
            qcopy(&quads[1].positions[2], glow);
            fast_vec_scale(&quads[1].positions[3], &quads[1].positions[2], -1.0f);
            transform_vector_c(&from, &((OvlVec4 *)points)[i], transform);
            transform_vector_c(&to, &((OvlVec4 *)points)[i + 1], transform);
            subtract_vector_xyz(&basis[0], &to, &from);
            normalize_vector_xyz(&basis[0], &basis[0], 1.0f);
            subtract_vector_xyz(&basis[1], D_L00_00166DC0, &from);
            cross_vectors_xyz(&basis[2], &basis[1], &basis[0]);
            normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
            cross_vectors_xyz(&basis[1], &basis[0], &basis[2]);
            qcopy(&basis[3], &from);
            transform_vector_c(&quads[0].positions[2], &quads[0].positions[2], basis);
            transform_vector_c(&quads[0].positions[3], &quads[0].positions[3], basis);
            transform_vector_c(&quads[1].positions[2], &quads[1].positions[2], basis);
            transform_vector_c(&quads[1].positions[3], &quads[1].positions[3], basis);
            if (i == 1) {
                int saved_color = quads[0].colors[0];
                int saved_glow_color = quads[1].colors[0];
                quads[0].colors[0] = quads[0].colors[1] = 0xFFFFFF;
                quads[1].colors[0] = quads[1].colors[1] = 0x7F4040;
                fast_draw_quad_real(&quads[0], 0, 0);
                fast_draw_quad_real(&quads[1], 0, 0);
                quads[0].colors[0] = quads[0].colors[1] = quads[0].colors[2] = quads[0].colors[3] = saved_color;
                quads[1].colors[0] = quads[1].colors[1] = quads[1].colors[2] = quads[1].colors[3] = saved_glow_color;
            } else {
                fast_draw_quad_real(&quads[0], 0, 0);
                fast_draw_quad_real(&quads[1], 0, 0);
            }
            qcopy(&quads[0].positions[0], &quads[0].positions[2]);
            qcopy(&quads[0].positions[1], &quads[0].positions[3]);
            qcopy(&quads[1].positions[0], &quads[1].positions[2]);
            qcopy(&quads[1].positions[1], &quads[1].positions[3]);
            taper = cubic_hermite_scalar(-1.0f, zero, 1.0f, zero, convert_integer_to_float_c2(i) / convert_integer_to_float_c2(20));
            scroll = D_L00_00161C78_f[slot];
            quads[0].texture_coordinates[0][0] = scroll + zero;
            quads[0].texture_coordinates[0][1] = zero;
            quads[0].texture_coordinates[1][0] = scroll + zero;
            quads[0].texture_coordinates[1][1] = 1.0f;
            quads[0].texture_coordinates[2][0] = scroll + 1.0f;
            quads[0].texture_coordinates[2][1] = zero;
            quads[0].texture_coordinates[3][0] = scroll + 1.0f;
            quads[0].texture_coordinates[3][1] = 1.0f;
            width.f[2] = half_width + (0.05f - half_width) * taper;
            /* Fakematch: plain C stores glow_width.f[2]; the dead reset keeps glow_src live up to the loop. */
            { OvlVec4 *next_glow = &glow_width; next_glow->f[2] = width.f[2] + width.f[2]; }
            glow_src = &glow_width;
            i++;
        } while (i < 19);
    }
}
typedef struct {
    f32 v[4][4];
    s32 col[4];
    f32 uv[4][2];
    s64 r70;
    s64 r78;
    u64 r80;
    u64 r88;
} Q_e3388;

extern T_e3388 D_L00_001E6D50_e3388[] __asm__("D_L00_001E6D50");
extern f32 D_L00_001E6E10_e3388[][4] __asm__("D_L00_001E6E10");
extern u8 g_debug_camera[] __asm__("D_L00_00166DC0");

void FUN_001f9a28_e3388(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_001f9bf8_e3388(void *, void *, f32) __asm__("FUN_001f9bf8");
void FUN_001f9ad8_e3388(void *, void *, void *) __asm__("FUN_001f9ad8");
s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
f32 FUN_002132a8_e3388(f32, f32) __asm__("FUN_002132a8");
f32 spread_k(f32, f32) __asm__("FUN_L00_00257c48");
void FUN_001f9a68_e3388(void *, void *, f32) __asm__("FUN_001f9a68");
void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
void FUN_001f9a10_e3388(void *, void *, void *) __asm__("FUN_001f9a10");
void draw_geometry_quad(void *, s32, s32) __asm__("FUN_001f7d30");

extern f32 D_L00_001E6CE0[4][4];
void FUN_L00_002e3388(s32 unused, s32 beam_slot) {
    Q_e3388 geometry_quad;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 c[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 e[4] __attribute__((aligned(16)));
    f32 quad_scale;
    s32 i;
    s32 j;

    qcopy(d, &D_L00_001E6D50_e3388[beam_slot]);
    d[3] = 1.0f;
    FUN_001f9a28_e3388(a, g_debug_camera, d);
    FUN_001f9bf8_e3388(a, a, 1.0f);
    FUN_001f9ad8_e3388(b, a, &hero.gravity_dir);
    FUN_001f9bf8_e3388(b, b, -1.0f);
    FUN_001f9ad8_e3388(c, b, a);
    geometry_quad.r78 = get_effect_texture(0xB);
    geometry_quad.r88 = 0xFF00000044;
    geometry_quad.r80 = 0xFF9000000260;
    geometry_quad.r70 = 5;
    geometry_quad.col[3] = -1;
    geometry_quad.col[2] = -1;
    geometry_quad.col[1] = -1;
    geometry_quad.col[0] = -1;
    geometry_quad.uv[0][0] = 0.0f;
    geometry_quad.uv[0][1] = 0.0f;
    geometry_quad.uv[1][0] = 0.0f;
    geometry_quad.uv[1][1] = 1.0f;
    geometry_quad.uv[2][0] = 1.0f;
    geometry_quad.uv[2][1] = 0.0f;
    geometry_quad.uv[3][0] = 1.0f;
    geometry_quad.uv[3][1] = 1.0f;
    quad_scale = FUN_002132a8_e3388(0.3f, 0.325f);
    e[0] = spread_k(0.0f, 0.05f);
    e[1] = spread_k(0.0f, 0.05f);
    e[2] = spread_k(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], D_L00_001E6CE0[i], quad_scale);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], a);
        FUN_001f9a10_e3388(geometry_quad.v[i], geometry_quad.v[i], e);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    geometry_quad.r88 = 0xFF00000048;
    geometry_quad.col[3] = 0x607F4040;
    geometry_quad.col[2] = 0x607F4040;
    geometry_quad.col[1] = 0x607F4040;
    geometry_quad.col[0] = 0x607F4040;
    for (j = 0; j < 4; j++) {
        FUN_001f9a68_e3388(geometry_quad.v[j], D_L00_001E6CE0[j], quad_scale * 3.0f);
        transform_vector(geometry_quad.v[j], geometry_quad.v[j], a);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    qcopy(d, D_L00_001E6E10_e3388[beam_slot]);
    FUN_001f9a28_e3388(a, g_debug_camera, d);
    FUN_001f9bf8_e3388(a, a, 1.0f);
    FUN_001f9ad8_e3388(b, a, &hero.gravity_dir);
    FUN_001f9bf8_e3388(b, b, -1.0f);
    FUN_001f9ad8_e3388(c, b, a);
    quad_scale = FUN_002132a8_e3388(0.2f, 0.225f);
    geometry_quad.r88 = 0xFF00000044;
    geometry_quad.col[3] = -1;
    geometry_quad.col[2] = -1;
    geometry_quad.col[1] = -1;
    geometry_quad.col[0] = -1;
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], D_L00_001E6CE0[i], quad_scale);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], a);
        FUN_001f9a10_e3388(geometry_quad.v[i], geometry_quad.v[i], e);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    geometry_quad.r88 = 0xFF00000048;
    geometry_quad.col[3] = 0x607F4040;
    geometry_quad.col[2] = 0x607F4040;
    geometry_quad.col[1] = 0x607F4040;
    geometry_quad.col[0] = 0x607F4040;
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], D_L00_001E6CE0[i], quad_scale * 3.0f);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], a);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
}
typedef struct {
    char p0[0x60];
    int path;
    float f64, f68, f6C;
    int i70;
    float f74;
} D_2e3a88;
extern int *vend_paths[] __asm__("D_L00_001B04B0") __attribute__((section(".data")));
extern float D_0015ED6C_2e3a88 __asm__("D_0015ED6C");
void FUN_0020c828_2e3a88(void *) __asm__("FUN_0020c828");
float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
float FUN_001f99c0_2e3a88(float) __asm__("FUN_001f99c0");
void sample_camera_path(void *, float, int, void *, void *, int) __asm__("FUN_00214e58");
int random_integer_below(int) __asm__("FUN_00213260");
void FUN_00212f90_2e3a88(void *, int, int, int) __asm__("FUN_00212f90");
extern float D_L00_00161C98 __attribute__((sda));
extern float D_L00_00161C9C __attribute__((sda));
void pterodactyl_update(unsigned char *m) __asm__("FUN_L00_002e3a88");
void pterodactyl_update(unsigned char *moby) {
    enum {
        PTERODACTYL_INITIALIZE = 0,
        PTERODACTYL_FOLLOW_PATH = 1
    };
    D_2e3a88 *moby_data = *(D_2e3a88 **)(moby + 0x78);
    int *path = vend_paths[moby_data->path];
    float y;
    switch (moby[0x20]) {
    case PTERODACTYL_INITIALIZE:
        if (moby_data->path == -1) {
            FUN_0020c828_2e3a88(moby);
            return;
        }
        {
            float first_segment_length = distance_xyz(path + 4, path + 8);
            float first_to_last_distance = distance_xyz(path + 4, path + *path * 4);
            moby_data->f74 = D_L00_00161C98 * D_0015ED6C_2e3a88 / first_segment_length;
            moby_data->i70 = FUN_001f99c0_2e3a88(first_segment_length - first_to_last_distance) < 0.1f;
        }
        moby_data->f6C = moby_data->f68 * (float)*path;
        moby[0x20] = 1;
        *(float *)(moby + 0x2C) *= D_L00_00161C9C;
        moby[0x30] = 0xFF;
    case PTERODACTYL_FOLLOW_PATH:
        y = *(float *)(moby + 0x18);
        sample_camera_path(path, moby_data->f6C, moby_data->i70, moby + 0x10, moby + 0x40, 0);
        *(float *)(moby + 0x44) *= 0.5f;
        *(float *)(moby + 0x18) += moby_data->f64;
        moby_data->f6C += moby_data->f74;
        if ((float)*path < moby_data->f6C)
            moby_data->f6C -= (float)*path;
        if (moby[0x70] & 2) {
            if (y < *(float *)(moby + 0x18) || random_integer_below(3) != 0) {
                if (moby[0x53])
                    FUN_00212f90_2e3a88(moby, 0, 0, 10);
            } else {
                if (moby[0x53] != 1)
                    FUN_00212f90_2e3a88(moby, 1, 0, 10);
            }
        }
        break;
    }
}
float FUN_001f9b48(void *, void *);
float fast_vec_length(void *) __asm__("FUN_001f9af0");
int scale_game_frames(int) __asm__("FUN_001f96f8");
void blend_moby_animation(unsigned char *, int, int, int) __asm__("FUN_00212f90");
void red_plant_update(unsigned char *p) __asm__("FUN_L00_002e3ca8");
void red_plant_update(unsigned char *moby) {
    char *hero_position = (char *)&hero.motion.pos;
    if (FUN_001f9b48(hero_position, moby + 0x10) < 1.0f) {
        if (fast_vec_length(hero_position + 0x80) > frame_time + frame_time && moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(6));
        }
    }
    if (moby[0x70] & 2) {
        if (moby[0x53] == 1) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(6));
        }
    }
}
/* Update of an effect-group moby: state machine over its life, then it steers a sprite along its path and bounces it off surfaces. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5238), where it is exact; names translated to the US level program. */

extern char D_L00_0015F580[] __attribute__((section(".sdata")));
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void quaternion_to_matrix(void *, void *) __asm__("FUN_001fa4f8");
extern void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void spawn_explosion(void *, void *, void *, float, float, int, float, int, float, float, int, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_0025f3e8(void *, void *, int, float, float);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_quaternion_from_axis_angle(void *, void *, f32) __asm__("FUN_00214530");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void multiply_matrix_basis_columns(void *, void *, void *) __asm__("func_001FA328");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");
extern int func_001F9908_r(int *) __asm__("FUN_001f9740");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern int func_L00_001F10E0_s(void *, float, int, void *) __asm__("FUN_L00_001f0d60");

extern float coll_hit_normal[] __asm__("D_L00_00173E80") __attribute__((section(".data")));
void robot_arm_update(char *m) __asm__("FUN_L00_002e3d88");
void robot_arm_update(char *moby) {
    enum {
        ROBOT_ARM_REMOVE = 0,
        ROBOT_ARM_WAIT_FOR_TIMER = 1,
        ROBOT_ARM_SPAWN_EFFECT_AND_REMOVE = 2,
        ROBOT_ARM_FADE_AND_REMOVE = 3
    };
    char *moby_data = *(char **)(moby + 0x78);
    float effect_offset_or_world_position[4];
    float rotation_step_basis[12];
    float offset_after_rotation[4];
    float rotation_offset_delta[4];
    float collision_probe_position[4];
    float bounce_rotation_matrix[16];
    float aD0[4];
    float aE0[4];
    char *rotation_step_euler;
    float collision_radius;
    float *collision_normal;

    switch (*(unsigned char *)(moby + 0x20)) {
    case ROBOT_ARM_REMOVE:
        mark_moby_for_removal(moby);
        return;
    case ROBOT_ARM_WAIT_FOR_TIMER:
        if (func_001F9908_r((int *)(moby_data + 0x30))) {
            moby[0x20] = (*(int *)(moby_data + 0x34) & 1) ? 3 : 2;
            *(int *)(moby_data + 0x30) = func_001FA898_r(multiply_global_factor_ed64(10.0f));
            return;
        }
        break;
    case ROBOT_ARM_SPAWN_EFFECT_AND_REMOVE:
        transform_vector_by_basis(effect_offset_or_world_position, moby_data + 0x20, moby + 0xC0);
        add_vector_xyz(effect_offset_or_world_position, effect_offset_or_world_position, moby + 0x10);
        if (*(int *)(moby_data + 0x34) & 2) {
            FUN_L00_0025f3e8(moby, effect_offset_or_world_position, -1, *(float *)(moby_data + 0x2C) * 0.5f, 0.0f);
        } else {
            float half_collision_radius = *(float *)(moby_data + 0x2C) * 0.5f;
            spawn_explosion(moby, D_L00_0015F580, effect_offset_or_world_position, 0.0f, 0.0f, 5, *(float *)(moby_data + 0x2C), 3, half_collision_radius, 9.0f, 5, half_collision_radius, -1, 0.0f, 0, 0, -1, 0);
        }
        mark_moby_for_removal(moby);
        return;
    case ROBOT_ARM_FADE_AND_REMOVE:
        if (func_001F9908_r((int *)(moby_data + 0x30))) {
            mark_moby_for_removal(moby);
            return;
        }
        moby[0x23] = func_001FA898_r((float)*(int *)(moby_data + 0x30) / multiply_global_factor_ed64(10.0f) * 128.0f);
        break;
    }
    rotation_step_euler = moby_data + 0x10;
    vu_euler_rotation_basis(rotation_step_basis, rotation_step_euler);
    transform_vector_by_basis(effect_offset_or_world_position, moby_data + 0x20, moby + 0xC0);
    multiply_matrix_basis_columns(moby + 0xC0, rotation_step_basis, moby + 0xC0);
    transform_vector_by_basis(offset_after_rotation, moby_data + 0x20, moby + 0xC0);
    subtract_vector_xyz(rotation_offset_delta, offset_after_rotation, effect_offset_or_world_position);
    subtract_vector_xyz((float *)(moby + 0x10), (float *)(moby + 0x10), rotation_offset_delta);
    add_vector_xyz(moby + 0x10, moby + 0x10, moby_data);
    collision_radius = *(float *)(moby_data + 0x2C);
    *(float *)(moby_data + 8) = *(float *)(moby_data + 8) - *(float *)(moby_data + 0x38);
    transform_vector_by_basis(collision_probe_position, moby_data + 0x20, moby + 0xC0);
    add_vector_xyz(collision_probe_position, collision_probe_position, moby + 0x10);
    if (func_L00_001F10E0_s(collision_probe_position, collision_radius, 0, moby)) {
        float speed, approach;
        collision_normal = coll_hit_normal;
        normalize_vector_xyz(collision_normal, collision_normal, 1.0f);
        speed = vector_length_xyz(moby_data);
        approach = dot_vectors_xyz(moby_data, collision_normal);
        if (approach < 0.0f) {
            FUN_L00_001ff660(moby_data, moby_data, collision_normal);
            normalize_vector_xyz(aD0, collision_normal, approach * 0.5f);
            add_vector_xyz(moby_data, moby_data, aD0);
            normalize_vector_xyz(moby_data, moby_data, speed);
            cross_vectors_xyz(aE0, collision_normal, moby_data);
            normalize_vector_xyz(aE0, aE0, 1.0f);
            build_quaternion_from_axis_angle(aE0, aE0, vector_length_xyz(rotation_step_euler));
            quaternion_to_matrix(aE0, bounce_rotation_matrix);
            matrix_to_euler(bounce_rotation_matrix, rotation_step_euler);
        }
    }
}
/* Copies the vector at D_0013F3D0 into b; a is unused. */
void follow_camera_hero_target(int a, void *b) __asm__("FUN_L00_002e4168");
void follow_camera_hero_target(int unused_camera, void *out_hero_position) {
    qcopy(out_hero_position, &hero.motion.pos);
}
#include "eetypes.h"
typedef struct {
    char b[0x80];
} M_2e4180_002e4180;
extern M_2e4180_002e4180 *D_L00_001600F4_002e4180 __asm__("D_L00_001600F4");
extern char D_L00_00173E70_002e4180[] __asm__("D_L00_00173E70");
extern char *FUN_L00_001ee1b0_002e4180(void *) __asm__("FUN_L00_001ee1b0");
extern void FUN_001f9a28_002e4180(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_002e4180(void *) __asm__("FUN_001f9af0");
extern void FUN_001f9a68_002e4180(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_001f9a10_002e4180(void *, void *, void *) __asm__("FUN_001f9a10");

int FUN_L00_002e4180(u128 *input_position, float radial_distance_extension) {
    u128 lookup_position_copy;
    u128 position_delta;
    char *selected_record;
    char *reference_position;
    char *extended_position;
    float selected_record_vector_length, inv;
    lookup_position_copy = *input_position;
    {
        int *lookup_result = (int *)FUN_L00_001ee1b0_002e4180(&lookup_position_copy);
        if (lookup_result == 0)
            goto fail;
        selected_record = (char *)&D_L00_001600F4_002e4180[lookup_result[5]];
    }
    reference_position = selected_record + 0x30;
    FUN_001f9a28_002e4180(&position_delta, input_position, reference_position);
    selected_record_vector_length = FUN_001f9af0_002e4180(selected_record);
    inv = 1.0f / FUN_001f9af0_002e4180(&position_delta);
    extended_position = D_L00_00173E70_002e4180;
    FUN_001f9a68_002e4180(extended_position, &position_delta, (selected_record_vector_length + radial_distance_extension) * inv);
    FUN_001f9a68_002e4180(extended_position - 0x10, &position_delta, selected_record_vector_length * inv);
    FUN_001f9a10_002e4180(extended_position, extended_position, reference_position);
    FUN_001f9a10_002e4180(extended_position - 0x10, extended_position - 0x10, reference_position);
    return 1;
fail:
    return 0;
}
int FUN_L00_001ee1b0(OvlQuad *, float);
/* True when FUN_L00_001ee1b0 returns non-zero for a copy of v and 0. */
int FUN_L00_002e4290(OvlQuad *input_quad) {
    OvlQuad input_copy = *input_quad;
    return FUN_L00_001ee1b0(&input_copy, 0.0f) != 0;
}
typedef struct {
    u8 p0[0x2DC];
    f32 f2DC;
    u8 p1[0x30C - 0x2E0];
    s16 h30C;
    u8 p2[0x2080 - 0x30E];
    void *p2080;
} G_002e42c0;
extern struct Hero hero_data __asm__("D_0013F350") __attribute__((section(".data")));
extern u8 D_L00_00166E10_002e42c0[] __asm__("D_L00_00166E10") __attribute__((section(".data")));
extern OvlQuad D_L00_00173E60_002e42c0 __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
void FUN_L00_002e4168_002e42c0(s32, OvlQuad *) __asm__("FUN_L00_002e4168");
void FUN_001f9a68_002e42c0(OvlQuad *, void *, f32) __asm__("FUN_001f9a68");
void FUN_001f9a10_002e42c0(OvlQuad *, OvlQuad *,
                           OvlQuad *) __asm__("FUN_001f9a10");
s32 FUN_001efa68_002e42c0(OvlQuad *, OvlQuad *, s32, void *,
                          s32) __asm__("FUN_001efa68");
s32 FUN_L00_002e42c0(OvlQuad *out, s32 flag) {
    OvlQuad position_and_offset_vectors[4];
    struct Hero *player = &hero_data;
    u8 *camera_view = D_L00_00166E10_002e42c0;
    f32 ground_distance;
    if (player->frames_off_ground.s == 0) {
        FUN_L00_002e4168_002e42c0(*(s32 *)(camera_view - 0x10), out);
        return 1;
    }
    FUN_L00_002e4168_002e42c0(*(s32 *)(camera_view - 0x10), &position_and_offset_vectors[0]);
    ground_distance = player->ground_distance;
    if (30.0f <= ground_distance || flag == 0) {
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[3], camera_view + 0x30, 0.2f);
        FUN_001f9a10_002e42c0(&position_and_offset_vectors[1], &position_and_offset_vectors[0], &position_and_offset_vectors[3]);
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[3], camera_view + 0x30, -30.0f);
        FUN_001f9a10_002e42c0(&position_and_offset_vectors[2], &position_and_offset_vectors[0], &position_and_offset_vectors[3]);
        if (FUN_001efa68_002e42c0(&position_and_offset_vectors[1], &position_and_offset_vectors[2], 0x12, player->moby, 0)) {
            qcopy(out, &D_L00_00173E60_002e42c0);
            return 0;
        }
        qcopy(out, &position_and_offset_vectors[0]);
        return -1;
    } else {
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[1], camera_view + 0x30, -ground_distance);
        FUN_001f9a10_002e42c0(out, &position_and_offset_vectors[0], &position_and_offset_vectors[1]);
        return 1;
    }
}
typedef float V4_2e4430[4] __attribute__((aligned(16)));
extern char D_L00_00166E10_002e4430[] __asm__("D_L00_00166E10") __attribute__((section(".data")));
void FUN_001f9a68_002e4430(void *, void *, float) __asm__("FUN_001f9a68");
void FUN_001f9a10_002e4430(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9a28_002e4430(void *, void *, void *) __asm__("FUN_001f9a28");
float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
void clear_u64_value(void *) __asm__("FUN_001f99f8");
void FUN_L00_002e42c0_002e4430(void *, int) __asm__("FUN_L00_002e42c0");
void follow_camera_reset(char *o) __asm__("FUN_L00_002e4430");
void follow_camera_reset(char *camera) {
    V4_2e4430 target_position, target_up_projection, b;
    char *g = D_L00_00166E10_002e4430;
    char *camera_up_axis = g + 0x30;
    char *camera_state = *(char **)(camera + 0x70);
    char *attach_state, *r;
    FUN_L00_002e42c0_002e4430(target_position, 0);
    attach_state = camera_state + 0x40;
    r = camera_state + 0x130;
    {
        float x = fast_vec_dot(target_position, camera_up_axis);
        FUN_001f9a68_002e4430(target_up_projection, camera_up_axis, x);
    }
    FUN_001f9a28_002e4430(attach_state, target_position, target_up_projection);
    qcopy(camera_state + 0x50, target_up_projection);
    FUN_001f9a10_002e4430(attach_state, attach_state, camera_state + 0x50);
    g += 0x20;
    FUN_001f9a68_002e4430(b, g, *(float *)(attach_state + 0xB0));
    FUN_001f9a10_002e4430(camera_state + 0x80, b, attach_state);
    FUN_001f9a10_002e4430(camera_state + 0xD0, b, attach_state);
    clear_u64_value(camera_state + 0xE0);
    attach_state[0xC4] = 0;
    *(float *)(attach_state + 0xC8) = 1.0f;
    attach_state[0xC5] = 0;
    *(s16 *)(attach_state + 0xC6) = 0;
    qcopy(camera_state + 0xA0, attach_state);
    clear_u64_value(camera_state + 0xB0);
    clear_u64_value(camera_state + 0xC0);
    FUN_001f9a68_002e4430(b, g, *(float *)(r + 0x30));
    FUN_001f9a10_002e4430(camera_state + 0x90, b, attach_state);
}
/* angle between a direction and an axis, signed */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5A30), where it is exact; names translated to the US level program. */

extern char D_L00_00166E10[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_arc_sin(float) __asm__("FUN_001f9df8");
extern void FUN_001f9a10_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28(float *, float *, float *);
extern void FUN_001f9a68_c3(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_L00_002e4168_c2(char *, void *) __asm__("FUN_L00_002e4168");

float FUN_L00_002e4580(char *camera, float target_offset_scale) {
    float hero_position[4], offset_target_position[4], angle_direction[4], axis_projection[4];
    float reference_dot_or_signed_angle, axis_dot_or_direction_length;
    char *camera_view = D_L00_00166E10;
    FUN_L00_002e4168_c2(camera, hero_position);
    FUN_001f9a68_c3(offset_target_position, camera_view + 0x20, target_offset_scale);
    FUN_001f9a10_c2(offset_target_position, offset_target_position, hero_position);
    FUN_001f9a28(angle_direction, offset_target_position, (float *)(camera + 0x30));
    axis_dot_or_direction_length = fast_vec_dot(angle_direction, camera + 0x10);
    FUN_001f9a68_c3(axis_projection, camera + 0x10, axis_dot_or_direction_length);
    FUN_001f9a28(angle_direction, angle_direction, axis_projection);
    reference_dot_or_signed_angle = fast_vec_dot(angle_direction, camera);
    axis_dot_or_direction_length = fast_vec_length(angle_direction);
    if (axis_dot_or_direction_length == 0.0f) {
        return 0.0f;
    }
    reference_dot_or_signed_angle = 1.5707964f - fast_arc_sin(reference_dot_or_signed_angle / axis_dot_or_direction_length);
    if (fast_vec_dot(angle_direction, camera + 0x20) < 0.0f) {
        reference_dot_or_signed_angle = -reference_dot_or_signed_angle;
    }
    return reference_dot_or_signed_angle;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5B68), where it is exact; names translated to the US level program. */

typedef struct {
    int a[4];
} Vq4 __attribute__((aligned(16)));

extern char D_0013E533[];
extern char D_L00_00166E10[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern int FUN_001f96f8_c(int) __asm__("FUN_001f96f8");
extern void FUN_001f9a10_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28(float *, float *, float *);
extern void FUN_001f9a68_c(void *, void *, float) __asm__("FUN_001f9a68");

void FUN_L00_002e46b8(char *camera) {
    Vq4 translation_delta;
    Vq4 camera_translation_adjustment, lateral_displacement;
    char *camera_view = D_L00_00166E10;
    char *camera_state = *(char **)(camera + 0x70);
    char *attach_state = camera_state + 0x40;
    char *g, *g3;
    qcopy(&translation_delta, D_0013E533 + 0xF5D);
    FUN_001f9a10_c(camera + 0x30, &translation_delta, camera + 0x30);
    FUN_001f9a10_c(camera_state + 0xA0, &translation_delta, camera_state + 0xA0);
    FUN_001f9a10_c(attach_state, &translation_delta, attach_state);
    FUN_001f9a10_c(camera_state + 0xD0, &translation_delta, camera_state + 0xD0);
    FUN_001f9a10_c(camera_state, &translation_delta, camera_state);
    g = D_0013E533 + 0xE1D;
    if (*(short *)(g + 0x308) == 0) {
        char *ground_moby = *(char **)(g + 0x2FC);
        char *q;
        if (ground_moby == 0 || *(short *)(ground_moby + 0xA6) != 0x22D ||
            ((q = camera_view - 0x190), FUN_001f96f8_c(0x1E)) < *(int *)(q + 0x398)) {
            char *g2 = D_0013E533 + 0xE1D;
            *(float *)(*(char **)(camera + 0x70) + 0x1C8) = *(float *)(g2 + 0x14C);
        }
    }
    g3 = D_0013E533 + 0xE1D;
    if ((unsigned)(*(int *)(g3 + 0x2084) - 0xB) < 2) {
        char *reloaded_camera_state = *(char **)(camera + 0x70);
        char *camera_up_axis = camera_view + 0x30;
        float translation_up_component;
        *(short *)(reloaded_camera_state + 0x10) = 0;
        FUN_001f9a68_c(&camera_translation_adjustment, camera_view + 0x80, *(float *)(camera_view + 0xA4));
        translation_up_component = fast_vec_dot(camera_up_axis, &translation_delta);
        FUN_001f9a68_c(&lateral_displacement, camera_up_axis, translation_up_component);
        FUN_001f9a28((float *)&lateral_displacement, (float *)&translation_delta, (float *)&lateral_displacement);
        FUN_001f9a28((float *)&camera_translation_adjustment, (float *)&camera_translation_adjustment, (float *)&lateral_displacement);
        FUN_001f9a10_c(camera + 0x30, &camera_translation_adjustment, camera + 0x30);
        FUN_001f9a10_c(attach_state + 0x60, &camera_translation_adjustment, attach_state + 0x60);
        FUN_001f9a10_c(attach_state, &camera_translation_adjustment, attach_state);
        FUN_001f9a10_c(attach_state + 0x90, &camera_translation_adjustment, attach_state + 0x90);
        FUN_001f9a10_c(reloaded_camera_state, &camera_translation_adjustment, reloaded_camera_state);
    }
}
typedef float V4[4] __attribute__((aligned(16)));
extern char D_L00_00166E10[];
void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void vcopy_2e48a0(void *, void *) __asm__("FUN_001f9a68");
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
void follow_camera_hero_target(int, void *) __asm__("FUN_L00_002e4168");
void FUN_L00_002e48a0(void *out) {
    V4 offset_target_position, hero_position;
    char *camera_view = D_L00_00166E10;
    vcopy_2e48a0(offset_target_position, camera_view + 0x20);
    follow_camera_hero_target(*(int *)(camera_view - 0x10), hero_position);
    fast_vec_add(offset_target_position, offset_target_position, hero_position);
    fast_vec_scale(out, camera_view + 0x30, fast_vec_dot(camera_view + 0x30, offset_target_position));
}
/* camera attach mode transitions: picks the next state from the player's state and arms its timers */
/* Migrated from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5DE0). */

typedef struct {
    float v00[4];
    float v10[4];
    float v20[4];
    float v30[4];
    char pad40[0x84];
    unsigned char state;
    unsigned char prev;
    short timer;
    float invTimer;
    float invMove;
    float dist;
    short moveTimer;
    char padD6;
    unsigned char hit;
} CamAttach;

extern int D_001413D4 __attribute__((section(".data")));
extern int D_001413DC __attribute__((section(".data")));
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern float D_L00_00166EEC __attribute__((section(".data")));
extern int FUN_001f9770_c2(void *) __asm__("FUN_001f9770");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 D_L00_00161D24_gp __asm__("D_L00_00161D24") __attribute__((sda));
extern short D_L00_00161D38 __attribute__((sda));
extern short D_L00_00161D3C_gp __asm__("D_L00_00161D3C") __attribute__((sda));
extern short D_L00_00161D40_gp __asm__("D_L00_00161D40") __attribute__((sda));
extern short D_L00_00161D48 __attribute__((sda));
extern short D_L00_00161D4C __attribute__((sda));
extern short D_L00_00161D50 __attribute__((sda));
extern short D_L00_00161D54_gp __asm__("D_L00_00161D54") __attribute__((sda));
extern short D_L00_00161D58 __attribute__((sda));
extern short D_L00_00161D5C __attribute__((sda));
extern short D_L00_00161D60 __attribute__((sda));
extern short D_L00_00161D64 __attribute__((sda));
extern short D_L00_00161D68 __attribute__((sda));
extern short D_L00_00161D6C_gp __asm__("D_L00_00161D6C") __attribute__((sda));
extern short D_L00_00161D78 __attribute__((sda));
extern short D_L00_00161D7C __attribute__((sda));
extern void FUN_L00_002e48a0_c(void *out, float f) __asm__("FUN_L00_002e48a0");

void FUN_L00_002e4930(char *camera) {
    char *camera_state = *(char **)(camera + 0x70);
    CamAttach *attach_state = (CamAttach *)(camera_state + 0x40);
    char *timer_camera_state;
    float inv, x, y;
    int camera_timer_base_frames;

    if (attach_state->state == 0) {
        struct Hero *player = &hero;
        if (player->state.control_mode == 2) {
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = D_L00_00161D3C_gp;
            attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        } else {
            struct Hero *current_player = &hero;
            if (current_player->state.current == 0xF && D_L00_00166EEC == 0.0f) {
                attach_state->prev = attach_state->state;
                attach_state->state = 3;
                attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D4C);
                attach_state->invMove = 1.0f / convert_integer_to_float_c(attach_state->moveTimer);
                attach_state->timer = D_L00_00161D50;
                inv = 1.0f / convert_integer_to_float_c(attach_state->timer);
                attach_state->dist = *(float *)&D_L00_00161D58;
                attach_state->hit = 0;
                x = *(float *)&D_L00_00161D58;
                attach_state->invTimer = inv;
                FUN_L00_002e48a0_c(attach_state->v30, x);
                qcopy(attach_state->v20, attach_state->v10);
            } else {
                struct Hero *state_check_player = &hero;
                if (state_check_player->state.current == 0xC && D_L00_00166EEC == 0.0f) {
                    attach_state->prev = attach_state->state;
                    attach_state->state = 8;
                    attach_state->timer = D_L00_00161D60;
                    attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
                } else {
                    struct Hero *p3 = &hero;
                    if (p3->state.current == 0xB && D_L00_00166EEC == 0.0f) {
                        attach_state->prev = attach_state->state;
                        attach_state->state = 6;
                        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
                        attach_state->invMove = 1.0f / convert_integer_to_float_c(attach_state->moveTimer);
                        attach_state->timer = D_L00_00161D50;
                        inv = 1.0f / convert_integer_to_float_c(attach_state->timer);
                        attach_state->dist = *(float *)&D_L00_00161D48;
                        attach_state->hit = 0;
                        x = *(float *)&D_L00_00161D48;
                        attach_state->invTimer = inv;
                        FUN_L00_002e48a0_c(attach_state->v30, x);
                        qcopy(attach_state->v20, attach_state->v10);
                    } else {
                        struct Hero *p4 = &hero;
                        if ((p4->state.current == 0xD || p4->state.current == 0xE) &&
                            D_L00_00166EEC == 0.0f) {
                            attach_state->prev = attach_state->state;
                            attach_state->state = 5;
                            attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D5C);
                            attach_state->invMove = 1.0f / convert_integer_to_float_c(attach_state->moveTimer);
                            attach_state->timer = D_L00_00161D60;
                            inv = 1.0f / convert_integer_to_float_c(attach_state->timer);
                            attach_state->dist = *(float *)&D_L00_00161D68;
                            attach_state->hit = 0;
                            x = *(float *)&D_L00_00161D68;
                            attach_state->invTimer = inv;
                            FUN_L00_002e48a0_c(attach_state->v30, x);
                            qcopy(attach_state->v20, attach_state->v10);
                        } else {
                            struct Hero *control_mode_player = &hero;
                            if (control_mode_player->state.control_mode == 4 && D_L00_00166EEC == 0.0f) {
                                attach_state->prev = attach_state->state;
                                attach_state->state = 2;
                                attach_state->timer = D_L00_00161D38;
                                attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
                            }
                        }
                    }
                }
            }
        }
        return;
    }
    if (attach_state->state != 9 && attach_state->state != 1 && D_001413D4 == 0x14) {
        float r;
        attach_state->prev = attach_state->state;
        attach_state->state = 9;
        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D6C_gp);
        r = 1.0f / convert_integer_to_float_c(attach_state->moveTimer);
        y = *(float *)&D_L00_00161D7C;
        attach_state->hit = 0;
        attach_state->timer = 0;
        x = *(float *)&D_L00_00161D7C;
        attach_state->invTimer = 1.0f;
        attach_state->dist = y;
        attach_state->invMove = r;
        FUN_L00_002e48a0_c(attach_state->v30, x);
        qcopy(attach_state->v20, attach_state->v10);
        return;
    }
    if ((attach_state->state == 1 || attach_state->state == 2) && D_001413DC != 4 && D_001413DC != 2) {
        attach_state->timer = 0;
        attach_state->invTimer = 1.0f;
        attach_state->state = 0;
        return;
    }
    if (attach_state->state == 7) {
        struct Hero *p = &hero;
        if (p->unk41E == 0) {
            attach_state->prev = attach_state->state;
            attach_state->state = 6;
            attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
            attach_state->invMove = 1.0f / convert_integer_to_float_c(attach_state->moveTimer);
            attach_state->timer = D_L00_00161D50;
            inv = 1.0f / convert_integer_to_float_c(attach_state->timer);
            attach_state->dist = *(float *)&D_L00_00161D48;
            attach_state->hit = 0;
            x = *(float *)&D_L00_00161D48;
            attach_state->invTimer = inv;
            FUN_L00_002e48a0_c(attach_state->v30, x);
            qcopy(attach_state->v20, attach_state->v10);
            return;
        }
        if (p->state.control_mode == 2) {
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
            return;
        }
    }
    if (attach_state->state == 4 && D_001413DC != 4) {
        if (D_001413DC == 2) {
            attach_state->prev = 4;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        } else {
            attach_state->prev = 4;
            attach_state->state = 0;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        }
        return;
    }
    if (attach_state->state == 3 && D_001413D4 == 0xB) {
        attach_state->prev = attach_state->state;
        attach_state->state = 6;
        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
        attach_state->invMove = 1.0f / convert_integer_to_float_c(attach_state->moveTimer);
        attach_state->timer = D_L00_00161D50;
        inv = 1.0f / convert_integer_to_float_c(attach_state->timer);
        attach_state->dist = *(float *)&D_L00_00161D48;
        attach_state->hit = 0;
        x = *(float *)&D_L00_00161D48;
        attach_state->invTimer = inv;
        FUN_L00_002e48a0_c(attach_state->v30, x);
        qcopy(attach_state->v20, attach_state->v10);
        return;
    }
    if (attach_state->state == 5 && D_001413D4 == 0xC) {
        attach_state->prev = attach_state->state;
        attach_state->state = 8;
        attach_state->timer = D_L00_00161D60;
        attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        return;
    }
    if (attach_state->state == 8 && D_001413D4 != 0xC) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D64);
        attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        return;
    }
    if (attach_state->state == 6 && D_001413D4 != 0xB) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        return;
    }
    if ((attach_state->state == 3 || attach_state->state == 5) && D_001413DC != 4) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        return;
    }
    if (attach_state->state == 9) {
        struct Hero *p = &hero;
        if (p->state.control_mode == 2 || p->frames_off_ground.s >= scale_game_frames(0x37)) {
            goto to_one;
        }
    }
    if (attach_state->state == 0xA) {
        struct Hero *p = &hero;
        if (p->state.control_mode == 2 || p->frames_off_ground.s >= scale_game_frames(0x37)) {
            float r;
        to_one:
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            r = 1.0f / convert_integer_to_float_c(attach_state->timer);
            camera_timer_base_frames = *(int *)&D_L00_00161D24_gp;
            attach_state->invTimer = r;
            timer_camera_state = *(char **)(camera + 0x70);
            *(short *)(timer_camera_state + 0x20) = scale_game_frames(camera_timer_base_frames);
            qcopy(camera + 0x40, camera);
            return;
        }
    }
    if (attach_state->state == 9 && D_001413D4 != 0x14) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0xA;
        attach_state->timer = D_L00_00161D78;
        attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        return;
    }
    if (attach_state->state == 0xA && FUN_001f9770_c2(&attach_state->moveTimer)) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
        return;
    }
    if (attach_state->state == 5 && D_001413D4 == 0x10) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D64);
        attach_state->invTimer = 1.0f / convert_integer_to_float_c(attach_state->timer);
    }
}
/* per-frame state machine that eases a moby's attach pose toward the camera block's vector */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E6498), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern int FUN_001f96f8_c2(int) __asm__("FUN_001f96f8");
extern int FUN_001f9770_c(void *) __asm__("FUN_001f9770");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern float FUN_L00_002e4580_u(void *, float) __asm__("FUN_L00_002e4580");
extern s32 D_L00_00161D24_gp __asm__("D_L00_00161D24") __attribute__((sda));
extern short D_L00_00161D3C_gp __asm__("D_L00_00161D3C") __attribute__((sda));
extern short D_L00_00161D40_gp __asm__("D_L00_00161D40") __attribute__((sda));
extern short D_L00_00161D54_gp __asm__("D_L00_00161D54") __attribute__((sda));
extern short D_L00_00161D6C_gp __asm__("D_L00_00161D6C") __attribute__((sda));
extern void lerp_vector(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern void FUN_001f9a68_c2(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_L00_002e4168_c(char *, void *) __asm__("FUN_L00_002e4168");
extern void FUN_L00_002e42c0_c(void *, int) __asm__("FUN_L00_002e42c0");
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_L00_002E5770_r(void *, int) __asm__("FUN_L00_002e42c0");

extern short D_L00_00161D0C __attribute__((sda));
extern short D_L00_00161D10 __attribute__((sda));
extern short D_L00_00161D80 __attribute__((sda));
void FUN_L00_002e4fe8(char *camera, float *out_focus_velocity) {
    float hero_position[4];
    float target_position_or_up_offset[4];
    char *camera_state;
    char *attach_state;
    char *camera_view;
    struct Hero *pad;
    struct Hero *padb;
    struct Hero *padc;
    char *timer_camera_state;
    float x, y;
    int flag;
    camera_view = D_L00_00166E10;
    camera_state = *(char **)(camera + 0x70);
    FUN_L00_002e4168_c(camera, hero_position);
    pad = &hero;
    attach_state = camera_state + 0x40;
    if (pad->state.current == 0x77) {
        x = fast_vec_dot(attach_state, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        FUN_001f9a28(attach_state, attach_state, out_focus_velocity);
        y = fast_vec_dot(camera_view + 0x30, camera_state + 0xA0);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, y);
        qcopy(camera_state + 0x50, out_focus_velocity);
        if (*(unsigned char *)(attach_state + 0xC4) != 0xB) {
            *(unsigned char *)(attach_state + 0xC4) = 0xB;
            qcopy(camera + 0x40, camera);
            timer_camera_state = *(char **)(camera + 0x70);
            *(short *)(timer_camera_state + 0x20) = FUN_001f96f8_c2(*(int *)&D_L00_00161D24_gp);
        }
        return;
    }
    switch (*(unsigned char *)(attach_state + 0xC4)) {
    case CAM_ATTACH_GROUND_AWARE_FOCUS_VELOCITY:
        padb = &hero;
        if (padb->state.current == 0x11 ||
            (padb->unk41E != 0 && padb->ground_distance > 0.2f &&
             ((y = FUN_L00_002e4580_u(camera, *(float *)(attach_state + 0xB0))) < *(float *)&D_L00_00161D10 ||
              *(float *)&D_L00_00161D0C < y))) {
            int v;
            *(unsigned char *)(attach_state + 0xC4) = 1;
            v = *(unsigned short *)&D_L00_00161D3C_gp;
            *(short *)(attach_state + 0xC6) = v;
            *(float *)(attach_state + 0xC8) = 1.0f / convert_integer_to_float((short)v);
        }
        padc = &hero;
        flag = 0;
        if (padc->frames_off_ground.s != 0 && *(float *)(camera_view + 0xDC) == 0.0f &&
            (padc->unk41E == 0 || padc->unk44C >= 0x10)) {
            flag = 1;
        } else {
            if (func_L00_002E5770_r(target_position_or_up_offset, 1) == 1) {
                x = fast_vec_dot(target_position_or_up_offset, camera_view + 0x30);
                if (x < fast_vec_length(attach_state + 0x10))
                    flag = 1;
            } else {
                flag = 1;
            }
        }
        if (flag != 0) {
            if (absolute_float(*(float *)(camera_view + 0x30) - *(float *)(camera_view + 0x40)) > 0.001f ||
                absolute_float(*(float *)(camera_view + 0x34) - *(float *)(camera_view + 0x44)) > 0.001f ||
                absolute_float(*(float *)(camera_view + 0x38) - *(float *)(camera_view + 0x48)) > 0.001f) {
                x = fast_vec_dot(hero_position, camera_view + 0x30);
                FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
                FUN_001f9a28(attach_state, hero_position, out_focus_velocity);
            } else {
                qcopy(out_focus_velocity, attach_state + 0x10);
                x = fast_vec_dot(hero_position, camera_view + 0x30);
                FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
                FUN_001f9a28(attach_state, hero_position, target_position_or_up_offset);
            }
        } else {
            FUN_L00_002e42c0_c(target_position_or_up_offset, 1);
            x = fast_vec_dot(target_position_or_up_offset, camera_view + 0x30);
            FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
            FUN_001f9a28(attach_state, target_position_or_up_offset, out_focus_velocity);
        }
        break;
    case CAM_ATTACH_BLEND_TO_PROJECT_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_FOCUS_VELOCITY_WITH_HOLD_TRANSITION:
    case CAM_ATTACH_BLEND_OUT_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_BACK_FOCUS_VELOCITY:
        lerp_vector((float)*(short *)(attach_state + 0xD4) * *(float *)(attach_state + 0xCC), attach_state + 0x10, attach_state + 0x30,
                     attach_state + 0x20);
        if (*(unsigned char *)(attach_state + 0xD7) == 0) {
            if (coll_sphere(0.2f, camera + 0x30, 0x12, 0)) {
                qcopy(attach_state + 0x30, attach_state + 0x10);
                *(unsigned char *)(attach_state + 0xD7) = 1;
            }
        }
        qcopy(out_focus_velocity, attach_state + 0x10);
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
        FUN_001f9a28(attach_state, hero_position, target_position_or_up_offset);
        if (FUN_001f9770_c(attach_state + 0xD4)) {
            if (*(unsigned char *)(attach_state + 0xC4) == 9) {
                *(unsigned char *)(attach_state + 0xC4) = 0xA;
                *(short *)(attach_state + 0xD4) = FUN_001f96f8_c2(*(int *)&D_L00_00161D6C_gp);
                qcopy(attach_state + 0x20, attach_state + 0x10);
                FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, *(float *)(attach_state + 0xD0));
                FUN_001f9a28(attach_state + 0x30, attach_state + 0x20, target_position_or_up_offset);
            } else if (*(unsigned char *)(attach_state + 0xC4) == 6) {
                *(unsigned char *)(attach_state + 0xC4) = 7;
                *(short *)(attach_state + 0xD4) = FUN_001f96f8_c2(*(int *)&D_L00_00161D40_gp);
                qcopy(attach_state + 0x30, attach_state + 0x20);
                qcopy(attach_state + 0x20, attach_state + 0x10);
            } else {
                int v;
                *(unsigned char *)(attach_state + 0xC4) = 4;
                v = *(unsigned short *)&D_L00_00161D54_gp;
                *(short *)(attach_state + 0xC6) = v;
                *(float *)(attach_state + 0xC8) = 1.0f / convert_integer_to_float((short)v);
            }
        }
        break;
    case CAM_ATTACH_HOLD_FOCUS_VELOCITY:
        qcopy(out_focus_velocity, attach_state + 0x10);
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
        FUN_001f9a28(attach_state, hero_position, target_position_or_up_offset);
        break;
    case CAM_ATTACH_BLEND_PROJECTED_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_CONTINUE_BLEND_PROJECTED_HERO_FOCUS_VELOCITY:
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        FUN_001f9a28(attach_state, hero_position, out_focus_velocity);
        lerp_vector(*(float *)&D_L00_00161D80, out_focus_velocity, out_focus_velocity, attach_state + 0x20);
        break;
    case CAM_ATTACH_PROJECT_HERO_FOCUS_VELOCITY:
    default:
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        FUN_001f9a28(attach_state, hero_position, out_focus_velocity);
        break;
    }
}
f32 cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
void FUN_L00_002e55b8(void *mm, void *vv, void *tt, f32 stiffness, f32 damping, f32 limit) {
    u8 *camera_state = *(u8 **)((u8 *)mm + 0x70);
    f32 *current_values = vv;
    f32 *target_values = tt;
    current_values[0] = cam_interp_values(camera_state + 0xE0, current_values[0], target_values[0], stiffness, damping, limit);
    current_values[1] = cam_interp_values(camera_state + 0xE4, current_values[1], target_values[1], stiffness, damping, limit);
    current_values[2] = cam_interp_values(camera_state + 0xE8, current_values[2], target_values[2], stiffness, damping, limit);
}
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
s32 FUN_L00_002e5678(void *mm, void *input_offset, void *out_target_position, s32 k) {
    u8 *camera = mm;
    s32 attach_mode_return_flag = 1;
    u8 *stored_focus_position = *(u8 **)(camera + 0x70) + 0x40;
    f32 t[4] __attribute__((aligned(16)));
    switch (k) {
    case CAM_ATTACH_BLEND_TO_PROJECT_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_PROJECT_HERO_FOCUS_VELOCITY:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        attach_mode_return_flag = 0;
        break;
    case CAM_ATTACH_BLEND_FOCUS_VELOCITY_WITH_HOLD_TRANSITION:
    case CAM_ATTACH_BLEND_OUT_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_BACK_FOCUS_VELOCITY:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        attach_mode_return_flag = 0;
        break;
    case CAM_ATTACH_HERO_POSITION_BASE:
        follow_camera_hero_target((int)camera, t);
        fast_vec_add(out_target_position, input_offset, t);
        break;
    case CAM_ATTACH_GROUND_AWARE_FOCUS_VELOCITY:
    default:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        break;
    }
    return attach_mode_return_flag;
}
typedef struct {
    f32 v[4];
} V002e5730 __attribute__((aligned(16)));
typedef struct {
    u8 p0[0x90];
    u8 x90[0x34];
    u8 xc4;
    u8 xc5;
    s16 xc6;
    f32 xc8;
} A002e5730;
typedef struct {
    u8 p[0x70];
    u8 *m;
} O002e5730;
extern f32 D_L00_00161D28 __attribute__((sda));
extern f32 D_L00_00161D2C __attribute__((sda));
extern f32 D_L00_00161D30 __attribute__((sda));
extern f32 D_L00_00161D34 __attribute__((sda));
s32 fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
s32 FUN_L00_002e5678(void *, void *, void *, s32);
void FUN_L00_002e55b8(void *, void *, void *, f32, f32, f32);
void follow_camera_transition(O002e5730 *o, void *a) __asm__("FUN_L00_002e5730");
void follow_camera_transition(O002e5730 *camera, void *input_offset) {
    V002e5730 target_position;
    u8 *m = camera->m;
    A002e5730 *attach_state = (A002e5730 *)(m + 0x40);
    f32 k, x, y;
    u8 attach_mode;
    if (fast_dec_timer_s16(m + 0x106))
        attach_state->xc5 = attach_state->xc4;
    k = (f32)attach_state->xc6 * attach_state->xc8;
    FUN_L00_002e5678(camera, input_offset, &target_position, attach_state->xc4);
    attach_mode = attach_state->xc4;
    if (attach_mode == 3 || attach_mode == 5 || attach_mode == 6 || attach_mode == 7 || attach_mode == 1) {
        x = D_L00_00161D30 + (D_L00_00161D28 - D_L00_00161D30) * k;
        y = D_L00_00161D34 + (D_L00_00161D2C - D_L00_00161D34) * k;
    } else {
        x = D_L00_00161D28;
        y = D_L00_00161D2C;
    }
    FUN_L00_002e55b8(camera, attach_state->x90, &target_position, x, y, 0.0f);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E6CE0); names and data translated to the US level program. */

#include "rnc/gameplay/camera/follow_cam.h"

/* The camera view: +0x20 its forward axis, +0x30 its up axis. */
extern u8 camera_view[] __asm__("D_L00_00166E10");
extern f32 follow_cam_min_distance_gp __asm__("D_L00_00161CD8") __attribute__((sda));
extern s32 follow_cam_blend_frames __asm__("D_L00_00161D20");
extern s32 camera_collision_mask __asm__("D_L00_0015EF58");
/* Point where the last collision line stopped (CollisionHit.point). */
extern f32 collision_hit_point[4] __asm__("D_L00_00173E60") __attribute__((aligned(16)));

extern f32 vector_length_xyz(void *v) __asm__("FUN_001f9af0");
extern void add_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void lerp_vector(f32 t, void *out, void *a, void *b) __asm__("FUN_001f9a40");
extern f32 convert_integer_to_float(s32 i) __asm__("FUN_001fa6c0");
extern s32 fast_dec_timer(void *timer) __asm__("FUN_001f9770");
extern f32 cam_interp_values(void *state, f32 value, f32 target, f32 stiffness, f32 damping, f32 limit) __asm__("FUN_001ebd78");
extern s32 collision_line(void *from, void *to, s32 mask, s32 ignore, s32 flags) __asm__("FUN_001efa68");
extern void follow_cam_hero_position(void *cam, void *out) __asm__("FUN_L00_002e4168");
extern void follow_cam_update_mode(void *cam) __asm__("FUN_L00_002e4930");
extern void follow_cam_focus_velocity(void *cam, f32 *out) __asm__("FUN_L00_002e4fe8");
extern void follow_cam_spring_offset(void *cam, f32 *offset) __asm__("FUN_L00_002e5730");
extern void follow_cam_turn_towards(f32 amount, f32 rate) __asm__("FUN_L00_002e84b8");

/* Updates the follow camera: eases its focus and offset vectors, then places
   the eye behind the hero, pulled in where collision lines hit. */
void follow_camera_springs(void *cam) __asm__("FUN_L00_002e5830");
void follow_camera_springs(void *camera_moby) {
    f32 velocity[4], hero_pos[4], focus[4], up[4], lateral[4], look[4], base[4], probe[4], left[4], collision_resolution_delta[4], mid[4];
    u8 *view = camera_view;
    FollowCamState *camera_state = FOLLOW_CAM_STATE((struct UpdateCam *)camera_moby);
    FollowCamTail *camera_tail = &camera_state->tail;
    FollowCamBlock *follow_block = &camera_state->follow;
    f32 len, back;

    follow_cam_hero_position(camera_moby, hero_pos);
    follow_cam_update_mode(camera_moby);
    follow_cam_focus_velocity(camera_moby, velocity);
    qcopy(&camera_state->follow.velocity, velocity);
    qcopy(focus, follow_block);
    len = dot_vectors_xyz(view + 0x30, camera_state->follow.offset);
    scale_vector_xyz(up, view + 0x30, len);
    subtract_vector_xyz(lateral, camera_state->follow.offset, up);
    up[0] = cam_interp_values(&camera_state->follow.up_ease[0], up[0], follow_block->velocity[0], follow_block->up_stiffness, follow_block->up_damping, 0.0f);
    up[1] = cam_interp_values(&camera_state->follow.up_ease[1], up[1], follow_block->velocity[1], follow_block->up_stiffness, follow_block->up_damping, 0.0f);
    up[2] = cam_interp_values(&camera_state->follow.up_ease[2], up[2], follow_block->velocity[2], follow_block->up_stiffness, follow_block->up_damping, 0.0f);
    lateral[0] = cam_interp_values(&camera_state->follow.lateral_ease[0], lateral[0], follow_block->focus[0], follow_block->lateral_stiffness, follow_block->lateral_damping, 0.0f);
    lateral[1] = cam_interp_values(&camera_state->follow.lateral_ease[1], lateral[1], follow_block->focus[1], follow_block->lateral_stiffness, follow_block->lateral_damping, 0.0f);
    lateral[2] = cam_interp_values(&camera_state->follow.lateral_ease[2], lateral[2], follow_block->focus[2], follow_block->lateral_stiffness, follow_block->lateral_damping, 0.0f);
    add_vector_xyz(follow_block, follow_block, camera_state->follow.velocity);
    add_vector_xyz(camera_state->follow.offset, lateral, up);
    scale_vector_xyz(look, view + 0x20, follow_block->look_height);
    if (follow_block->blending_in) {
        if (++follow_block->blend_timer > scale_game_frames(follow_cam_blend_frames)) {
            follow_block->blend_timer = scale_game_frames(follow_cam_blend_frames);
        }
        lerp_vector((f32)follow_block->blend_timer / convert_integer_to_float(scale_game_frames(follow_cam_blend_frames)), base, follow_block, camera_state->follow.offset);
        add_vector_xyz(camera_state->follow.look_target, look, base);
    } else {
        fast_dec_timer(&camera_state->follow.blend_timer);
        if (follow_block->blend_timer < 0) {
            follow_block->blend_timer = 0;
        }
        lerp_vector((f32)follow_block->blend_timer / convert_integer_to_float(scale_game_frames(follow_cam_blend_frames)), base, follow_block, camera_state->follow.offset);
        add_vector_xyz(camera_state->follow.look_target, look, base);
    }
    follow_cam_spring_offset(camera_moby, look);
    if (hero.vehicle_mode == 2) {
        scale_vector_xyz(look, view + 0x20, 3.0f);
    } else if (hero.vehicle_mode == 1) {
        scale_vector_xyz(look, view + 0x20, 0.4f);
    } else {
        scale_vector_xyz(look, view + 0x20, 0.5f);
    }
    add_vector_xyz(base, hero_pos, look);
    scale_vector_xyz(look, view + 0x20, camera_tail->distance);
    add_vector_xyz(follow_block->eye, look, follow_block);
    len = follow_cam_min_distance_gp + FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom_range * FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom_scale * (FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom - 1.0f);
    scale_vector_xyz(probe, view + 0x20, 20.0f);
    add_vector_xyz(probe, base, probe);
    if (collision_line(base, probe, camera_collision_mask, (s32)hero.moby, 0)) {
        qcopy(left, collision_hit_point);
    } else {
        qcopy(left, probe);
    }
    scale_vector_xyz(probe, view + 0x20, 20.0f);
    subtract_vector_xyz(probe, base, probe);
    if (collision_line(base, probe, camera_collision_mask, (s32)hero.moby, 0)) {
        qcopy(probe, collision_hit_point);
    }
    subtract_vector_xyz(collision_resolution_delta, probe, left);
    if (vector_length_xyz(collision_resolution_delta) < len + len) {
        add_vector_xyz(mid, left, probe);
        scale_vector_xyz(mid, mid, 0.5f);
        qcopy(follow_block->eye, mid);
        subtract_vector_xyz(collision_resolution_delta, mid, follow_block);
        follow_cam_turn_towards(dot_vectors_xyz(collision_resolution_delta, view + 0x20), 0.003f);
    } else {
        f32 reach;
        subtract_vector_xyz(collision_resolution_delta, left, follow_block);
        back = vector_length_xyz(collision_resolution_delta);
        reach = camera_tail->distance;
        if (back <= reach + len) {
            scale_vector_xyz(mid, view + 0x20, len);
            subtract_vector_xyz(mid, left, mid);
            qcopy(follow_block->eye, mid);
            subtract_vector_xyz(collision_resolution_delta, mid, follow_block);
            follow_cam_turn_towards(dot_vectors_xyz(collision_resolution_delta, view + 0x20), 0.003f);
        } else {
            len = dot_vectors_xyz(follow_block, view + 0x30);
            if (dot_vectors_xyz(probe, view + 0x30) < len) {
                qcopy(probe, follow_block);
            }
            scale_vector_xyz(look, view + 0x20, reach);
            add_vector_xyz(follow_block->eye, look, probe);
        }
    }
}
/* Empty apart from an unused 0x50-byte volatile stack buffer. */
void FUN_L00_002e8378(void) {
    volatile char unused_stack_reservation[0x50];
}
struct P2_388 {
    char pad[0x220];
    s32 cur;
};
struct P1_388 {
    char pad0[0x70];
    struct P2_388 *p;
    char pad1[0x12];
    s16 f86;
};
extern struct P1_388 *D_L00_00166E00_c __asm__("D_L00_00166E00");
/* -1 while D_L00_00166E00+0x86 is set; else clears its current id, 1 if it was id. */
s32 FUN_L00_002e8388(s32 id);
s32 FUN_L00_002e8388(s32 id) {
    struct P1_388 *current_camera = D_L00_00166E00_c;
    if (current_camera->f86 != 0) {
        return -1;
    }
    if (current_camera->p->cur == id) {
        current_camera->p->cur = 0;
        return 1;
    }
    return 0;
}
extern void *D_L00_00166E00_c2 __asm__("D_L00_00166E00");
extern u32 D_L00_0015EF50;
typedef struct {
    char pad00[0x86];
    short f86;
} B842e83c0;
typedef struct {
    char pad00[0x70];
    u32 f70;
} B2842e83c0;
typedef struct {
    char pad00[0x220];
    u32 f220;
} C842e83c0;
typedef struct {
    char pad00[0x84];
    short f84;
} O842e83c0;
int FUN_L00_002e83c0(int arg1, int arg2);
int FUN_L00_002e83c0(int candidate_object, int unused) {
    void *current_camera = D_L00_00166E00_c2;
    C842e83c0 *camera_state;
    u8 x;
    u8 y;
    u32 tab;
    if (((B842e83c0 *)current_camera)->f86 != 0) {
        return -1;
    }
    camera_state = (C842e83c0 *)(u32)((B2842e83c0 *)current_camera)->f70;
    if (camera_state->f220 == 0) {
        camera_state->f220 = candidate_object;
        return 1;
    }
    if (camera_state->f220 == candidate_object) {
        return 1;
    }
    tab = (u32)D_L00_0015EF50;
    x = *(u8 *)(*(u32 *)(tab + ((O842e83c0 *)(u32)camera_state->f220)->f84 * 0x20 + 0x1C) + 0x1C);
    y = *(u8 *)(*(u32 *)(tab + ((O842e83c0 *)(u32)candidate_object)->f84 * 0x20 + 0x1C) + 0x1C);
    if (x < y) {
        camera_state->f220 = candidate_object;
        return 1;
    }
    return 0;
}
extern u8 *vd_2e8450 __asm__("D_L00_00166E00") __attribute__((section(".data")));
/* Sets a flag and floats a/b at +0x130 of D_L00_00166E00+0x70; f also stores a at +0x58. */
void follow_camera_set_distance(s32 f, f32 a, f32 b) __asm__("FUN_L00_002e8450");
void follow_camera_set_distance(s32 copy_distance_to_secondary_slot, f32 distance, f32 distance_override_companion_value) {
    u8 *current_camera = vd_2e8450;
    if (*(s16 *)(current_camera + 0x86) == 0) {
        u8 *camera_distance_state = *(u8 **)(current_camera + 0x70) + 0x130;
        *(s16 *)(camera_distance_state + 0x3C) = 1;
        *(f32 *)(camera_distance_state + 0x48) = distance_override_companion_value;
        *(f32 *)(camera_distance_state + 0x40) = distance;
        if (copy_distance_to_secondary_slot)
            *(f32 *)(camera_distance_state + 0x58) = distance;
    }
}
extern char *D_L00_00166E00_002e8490 __asm__("D_L00_00166E00");
void FUN_L00_002e8490(float camera_state_value) {
    char *current_camera = D_L00_00166E00_002e8490;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x22C) = camera_state_value;
    }
}
extern char *D_L00_00166E00_002e84b8 __asm__("D_L00_00166E00") __attribute__((section(".data")));
/* Sets a flag and two floats at +0x130 of D_L00_00166E00+0x70 while its short at 0x86 is 0. */
void follow_camera_set_height(float a, float b) __asm__("FUN_L00_002e84b8");
void follow_camera_set_height(float height, float height_override_companion_value) {
    char *current_camera = D_L00_00166E00_002e84b8;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *camera_height_state = *(char **)(current_camera + 0x70) + 0x130;
        *(short *)(camera_height_state + 0x3e) = 1;
        *(float *)(camera_height_state + 0x4c) = height;
        *(float *)(camera_height_state + 0x54) = height_override_companion_value;
    }
}
extern char *D_L00_00166E00_c3 __asm__("D_L00_00166E00") __attribute__((section(".data")));
extern char *D_L00_00168FC0 __attribute__((section(".data")));
/* Sets a flag and floats +0xF4/+0xF8 of D_L00_00166E00+0x70; add offsets a by 00168FC0+0xF0. */
void follow_camera_set_offset_scale(int add, float a, float b) __asm__("FUN_L00_002e84f0");
void follow_camera_set_offset_scale(int add, float offset_scale, float offset_scale_companion_value) {
    char *current_camera = D_L00_00166E00_c3;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *attach_state = *(char **)(current_camera + 0x70) + 0x40;
        *(short *)(attach_state + 0xDA) = 1;
        *(float *)(attach_state + 0xB8) = offset_scale_companion_value;
        if (add) {
            *(float *)(attach_state + 0xB4) = *(float *)(D_L00_00168FC0 + 0xF0) + offset_scale;
        } else {
            *(float *)(attach_state + 0xB4) = offset_scale;
        }
    }
}
typedef struct {
    char pad[0x10];
    short h;
} S2e8568b;
typedef struct {
    char pad[0x70];
    S2e8568b *p;
    char pad2[0x12];
    short f86;
} S2e8568;
extern S2e8568 *D_L00_00166E00;
/* Stores v through the pointer in D_L00_00166E00 while its short at 0x86 is 0. */
void FUN_L00_002e8568(int camera_state_short_value) {
    S2e8568 *current_camera = D_L00_00166E00;
    if (current_camera->f86 == 0)
        current_camera->p->h = camera_state_short_value;
}
extern char *D_L00_00166E00_002e8590[] __asm__("D_L00_00166E00");
/* Stores non-zero a/b at +0x11C/+0x120 of D_L00_00166E00+0x70 while its short at 0x86 is 0. */
void follow_camera_set_lateral_spring(float a, float b) __asm__("FUN_L00_002e8590");
void follow_camera_set_lateral_spring(float lateral_spring_value, float lateral_spring_companion_value) {
    char *current_camera = D_L00_00166E00_002e8590[0];
    if (*(short *)(current_camera + 0x86) == 0) {
        char *attach_state = *(char **)(current_camera + 0x70) + 0x40;
        if (lateral_spring_value != 0.0f)
            *(float *)(attach_state + 0xDC) = lateral_spring_value;
        if (lateral_spring_companion_value != 0.0f)
            *(float *)(attach_state + 0xE0) = lateral_spring_companion_value;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E9AD0), where it is exact; names translated to the US level program. */

extern char *D_L00_00166E00_002e8620 __asm__("D_L00_00166E00") __attribute__((section(".data")));

/* Sets the short at +0x226 of D_L00_00166E00+0x70 while the short at 0x86 is 0. */
void FUN_L00_002e8620(void);
void FUN_L00_002e8620(void) {
    char *current_camera = D_L00_00166E00_002e8620;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(short *)(*(char **)(current_camera + 0x70) + 0x226) = 1;
    }
}
extern u8 *vd_2e8648 __asm__("D_L00_00166E00") __attribute__((section(".data")));
/* Sets the byte at +0x116 of D_L00_00166E00+0x70 while the short at 0x86 is 0. */
void follow_camera_start_slide(void) __asm__("FUN_L00_002e8648");
void follow_camera_start_slide(void) {
    u8 *current_camera = vd_2e8648;
    if (*(s16 *)(current_camera + 0x86) == 0) {
        *(u8 *)(*(u8 **)(current_camera + 0x70) + 0x116) = 1;
    }
}
extern char *D_L00_00166E00_002e8680 __asm__("D_L00_00166E00") __attribute__((section(".data")));
void FUN_L00_002e8680(void) {
    char *current_camera = D_L00_00166E00_002e8680;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *blend_output_block = *(char **)(current_camera + 0x70) + 0x1a8;
        *(int *)(blend_output_block + 0x10) |= 3;
    }
}
/* Measures the signed angle between two offsets of an object's data relative to an axis, and (within the limit) stores a scaled blend in the object's data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E9B60), where it is exact; names translated to the US level program. */

extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float cubic_hermite_scalar(float a, float b, float c, float d, float t) __asm__("FUN_L00_00257e20");
extern void FUN_001f9a68_c4(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
float absolute_float(float input) __asm__("func_001F99C0");

float FUN_L00_002e86b0(char *camera, float *input_direction, float blend_update_angle_limit) {
    float axis_projection[4];
    float planar_input_direction[4];
    float planar_reference_direction[4];
    float input_cross_axis[4];
    char *base = D_L00_00166E10;
    char *axis = base + 0x20;
    char *camera_state = *(char **)(camera + 0x70);
    char *blend_output_block = camera_state + 0x1A8;
    char *reference_direction = camera_state + 0x130;
    float dot;
    float planar_input_length_or_length_product;
    float planar_separation_angle;
    float sign;
    float angle_delta = 0.0f;
    float z2;
    float angle_blend_fraction;

    FUN_001f9a68_c4(axis_projection, axis, fast_vec_dot(input_direction, axis));
    FUN_001f9a28(planar_input_direction, input_direction, axis_projection);
    FUN_001f9a68_c4(axis_projection, axis, fast_vec_dot(reference_direction, axis));
    FUN_001f9a28(planar_reference_direction, (float *)reference_direction, axis_projection);
    dot = fast_vec_dot(planar_input_direction, planar_reference_direction);
    planar_input_length_or_length_product = fast_vec_length(planar_input_direction);
    if (planar_input_length_or_length_product == angle_delta) {
        return angle_delta;
    }
    z2 = 0.0f;
    planar_input_length_or_length_product = planar_input_length_or_length_product * fast_vec_length(planar_reference_direction);
    if (planar_input_length_or_length_product == angle_delta) {
        return angle_delta;
    }
    planar_separation_angle = 1.5707964f - fast_arc_sin(dot / planar_input_length_or_length_product);
    sign = -1.0f;
    fast_vec_cross(input_cross_axis, planar_input_direction, axis);
    if (fast_vec_dot(input_cross_axis, planar_reference_direction) >= 0.0f) {
        sign = 1.0f;
    }
    angle_delta = fast_subtract_rotations(3.1415927f, planar_separation_angle);
    if (blend_update_angle_limit != z2 && blend_update_angle_limit < absolute_float(angle_delta)) {
        return angle_delta;
    }
    angle_blend_fraction = angle_delta / 1.5707964f;
    if (angle_blend_fraction > 1.0f) {
        angle_blend_fraction = 1.0f;
    }
    *(float *)(blend_output_block + 0x1C) = cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, angle_blend_fraction) * sign;
    return angle_delta;
}
void FUN_001f9a28_c2(void *, void *, void *) __asm__("FUN_001f9a28");
/* Passes b minus the vector at +0x40 of p's object, with x, to FUN_L00_002e86b0. */
float FUN_L00_002e88c8(char *camera, void *target_position, f32 x) {
    float focus_to_target_direction[4];
    FUN_001f9a28_c2(focus_to_target_direction, target_position, *(char **)(camera + 0x70) + 0x40);
    return FUN_L00_002e86b0(camera, focus_to_target_direction, x);
}
extern char *D_L00_00166E00_002e8918 __asm__("D_L00_00166E00") __attribute__((section(".data")));
void FUN_L00_002e86b0_002e8918(char *, char *, float) __asm__("FUN_L00_002e86b0");
/* For non-zero x: stores x at +0x1BC of D_L00_00166E00+0x70 and calls FUN_L00_002e86b0. */
void FUN_L00_002e8918(char *input_direction, float x, float y) {
    char *current_camera = D_L00_00166E00_002e8918;
    if (*(short *)(current_camera + 0x86) == 0 && x != 0.0f) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x1bc) = x;
        FUN_L00_002e86b0_002e8918(current_camera, input_direction, y);
    }
}
extern char *D_L00_00166E00_002e8970 __asm__("D_L00_00166E00") __attribute__((section(".data")));
void FUN_L00_002e88c8_002e8970(char *, char *, float) __asm__("FUN_L00_002e88c8");
/* Stores x at +0x1BC of D_L00_00166E00+0x70 and calls FUN_L00_002e88c8 with a and y. */
void FUN_L00_002e8970(char *a, float x, float y);
void FUN_L00_002e8970(char *target_position, float x, float y) {
    char *target_position_arg = target_position;
    char *current_camera = D_L00_00166E00_002e8970;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x1bc) = x;
        FUN_L00_002e88c8_002e8970(current_camera, target_position_arg, y);
    }
}

f32 D_L01_0016736C[1] __attribute__((section(".data"))) = {0};
f32 D_L02_001674EC[1] __attribute__((section(".data"))) = {0};
f32 D_L03_00166FEC[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0016706C[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001672EC[1] __attribute__((section(".data"))) = {0};
f32 D_L06_0016766C[1] __attribute__((section(".data"))) = {0};
f32 D_L07_00166F6C[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0016706C[1] __attribute__((section(".data"))) = {0};
f32 D_L10_0016736C[1] __attribute__((section(".data"))) = {0};
f32 D_L12_0016736C[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001671EC[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001675EC[1] __attribute__((section(".data"))) = {0};
f32 D_L15_001674EC[1] __attribute__((section(".data"))) = {0};
f32 D_L16_001672EC[1] __attribute__((section(".data"))) = {0};
