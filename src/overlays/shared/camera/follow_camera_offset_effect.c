/* Camera offset effect: picks a moby ahead of the hero and pulls the follow camera's offset towards it. */

#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/follow_cam.h"

typedef struct {
    int   a;
    int   b;
    float c;
    float duration;
    void (*apply)(void);
} CameraOffsetEffect;

extern void  apply_camera_offset_effect(void) __asm__("FUN_L02_002fa710");
extern void  register_camera_update_callback(void (*)(void)) __asm__("FUN_L02_001fc008");
void         update_camera_offset_effect_state(void) __asm__("FUN_L02_002fa770");
extern float absolute_float(float);

extern CameraOffsetEffect   camera_offset_effect __asm__("D_L02_00167600");
extern float D_L02_0016760C NOT_SDA;
extern short camera_offset_effect_duration_step_gp __asm__("D_L02_0016238C") __attribute__((sda));
extern char  D_0013CA44[];
extern short camera_offset_effect_duration_step __asm__("D_L02_0016238C");

s32 D_L02_00167604[1] __attribute__((section(".data"))) = {0};

f32 D_L02_00167608[1] __attribute__((section(".data"))) = {0};

s32 D_L02_00167610[1] __attribute__((section(".data"))) = {0};

s32 D_L03_00167100[1] __attribute__((section(".data"))) = {0};

s32 D_L03_00167104[1] __attribute__((section(".data"))) = {0};

f32 D_L03_00167108[1] __attribute__((section(".data"))) = {0};

f32 D_L03_0016710C[1] __attribute__((section(".data"))) = {0};

s32 D_L03_00167110[1] __attribute__((section(".data"))) = {0};

s32 D_L04_00167180[1] __attribute__((section(".data"))) = {0};

s32 D_L04_00167184[1] __attribute__((section(".data"))) = {0};

f32 D_L04_00167188[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0016718C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_00167190[1] __attribute__((section(".data"))) = {0};

s32 D_L05_00167400[1] __attribute__((section(".data"))) = {0};

s32 D_L05_00167404[1] __attribute__((section(".data"))) = {0};

f32 D_L05_00167408[1] __attribute__((section(".data"))) = {0};

f32 D_L05_0016740C[1] __attribute__((section(".data"))) = {0};

s32 D_L05_00167410[1] __attribute__((section(".data"))) = {0};

s32 D_L07_00167090[1] __attribute__((section(".data"))) = {0};

s32 D_L09_00167180[1] __attribute__((section(".data"))) = {0};

s32 D_L09_00167184[1] __attribute__((section(".data"))) = {0};

f32 D_L09_00167188[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0016718C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_00167190[1] __attribute__((section(".data"))) = {0};

s32 D_L10_00167490[1] __attribute__((section(".data"))) = {0};

s32 D_L12_00167480[1] __attribute__((section(".data"))) = {0};

s32 D_L12_00167484[1] __attribute__((section(".data"))) = {0};

f32 D_L12_00167488[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0016748C[1] __attribute__((section(".data"))) = {0};

s32 D_L12_00167490[1] __attribute__((section(".data"))) = {0};

s32 D_L15_00167600[1] __attribute__((section(".data"))) = {0};

s32 D_L15_00167604[1] __attribute__((section(".data"))) = {0};

f32 D_L15_00167608[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0016760C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_00167610[1] __attribute__((section(".data"))) = {0};

s32 D_L16_00167400[1] __attribute__((section(".data"))) = {0};

s32 D_L16_00167404[1] __attribute__((section(".data"))) = {0};

f32 D_L16_00167408[1] __attribute__((section(".data"))) = {0};

f32 D_L16_0016740C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_00167410[1] __attribute__((section(".data"))) = {0};

short              D_L03_00161E54 = -15729;
short              D_L04_00161F4C = -15729;
short              D_L05_001621A4 = -15729;
CameraOffsetEffect D_L06_00167780 = {0};

float D_L06_0016778C NOT_SDA = 0;
short                D_L06_0016256C = -15729;
CameraOffsetEffect   D_L07_00167080 = {0};

float D_L07_0016708C NOT_SDA = 0;
short                D_L07_00161E24 = -15729;
short                D_L09_00161F3C = -15729;
CameraOffsetEffect   D_L10_00167480 = {0};

float D_L10_0016748C NOT_SDA = 0;
short                D_L10_0016220C = -15729;
CameraOffsetEffect   D_L11_00167A00 = {0};

float D_L11_00167A0C NOT_SDA = 0;
short                D_L11_00162774 = -15729;
short                D_L12_0016224C = -15729;
CameraOffsetEffect   D_L13_00167300 = {0};

float D_L13_0016730C NOT_SDA = 0;
short                D_L13_001620C4 = -15729;
CameraOffsetEffect   D_L14_00167700 = {0};

float D_L14_0016770C NOT_SDA = 0;
short                D_L14_0016247C = -15729;
short                D_L15_001623CC = -15729;
short                D_L16_00162194 = -15729;
CameraOffsetEffect   D_L17_00167900 = {0};

float D_L17_0016790C NOT_SDA = 0;
short                D_L17_0016267C = -15729;
CameraOffsetEffect   D_L18_00167A00 = {0};

float D_L18_00167A0C NOT_SDA = 0;
short                D_L18_00162794 = -15729;

void initialize_camera_offset_effect_state(void) __asm__("FUN_L02_002fa6d8");

void initialize_camera_offset_effect_state(void) {
    camera_offset_effect.a = 0;
    camera_offset_effect.b = 0;
    camera_offset_effect.c = 10000.0f;
    camera_offset_effect.duration = 0;
    camera_offset_effect.apply = apply_camera_offset_effect;
}

/* Start the timed effect while its duration is nonzero. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FBB48), where it is exact; names translated to the US level program. */
void apply_camera_offset_effect(void) {
    if (D_L02_0016760C != 0.0f) {
        follow_camera_set_offset_scale(1, D_L02_0016760C * 0.35f + 0.0f, 0.005f);
        register_camera_update_callback(update_camera_offset_effect_state);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FBBA8), where it is exact; names translated to the US level program. */
void update_camera_offset_effect_state(void) {
    CameraOffsetEffect *effect_state = &camera_offset_effect;
    effect_state->c = 10000.0f;
    if (effect_state->b == 0) {
        float decremented_duration =
            effect_state->duration - *(float *)&camera_offset_effect_duration_step_gp;
        effect_state->duration = decremented_duration;
        if (decremented_duration < 0.0f) {
            effect_state->duration = 0.0f;
        }
    } else {
        effect_state->b = 0;
    }
    effect_state->a = 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FBC00), where it is exact; names translated to the US level program. */
void try_select_camera_offset_effect_moby(char *candidate_moby) __asm__("FUN_L02_002fa7c8");

void try_select_camera_offset_effect_moby(char *candidate_moby) {
    float candidate_displacement[4];
    char *camera_selection_context = (char *)&l02_camera;
    char *selection_reference = *(char **)(camera_selection_context + 0x180);
    char *candidate_selection_state;
    float vector_distance;
    float accumulated_amount;
    float candidate_reference_angle_radians;
    if (*(short *)(selection_reference + 0x86) == 0) {
        if (!(0.05f < absolute_float(*(float *)(D_0013CA44)))) {
            candidate_selection_state = camera_selection_context + 0x380;
            fast_vec_sub(candidate_displacement, candidate_moby + 0x10, &hero.motion.pos);
            vector_distance = fast_vec_length(candidate_displacement);
            if (vector_distance < 22.0f) {
                if (0.0f < vector_distance) {
                    if (vector_distance < *(float *)(candidate_selection_state + 8)) {
                        if (0.0f < candidate_displacement[2]) {
                            candidate_reference_angle_radians =
                                1.5707964f - fast_arc_sin(fast_vec_dot(selection_reference,
                                                                       candidate_displacement) /
                                                          vector_distance);
                            if (candidate_reference_angle_radians < 1.1170107f) {
                                fast_vec_sub(candidate_displacement,
                                             candidate_moby + 0x10,
                                             selection_reference + 0x30);
                                vector_distance = fast_vec_length(candidate_displacement);
                                if (vector_distance == 0.0f) {
                                    vector_distance = 0.0001f;
                                }
                                candidate_reference_angle_radians =
                                    1.5707964f - fast_arc_sin(fast_vec_dot(selection_reference,
                                                                           candidate_displacement) /
                                                              vector_distance);
                                if (candidate_reference_angle_radians < 0.62831855f) {
                                    accumulated_amount =
                                        *(float *)(candidate_selection_state + 0xC) +
                                        *(float *)&camera_offset_effect_duration_step;
                                    *(char **)(camera_selection_context + 0x380) =
                                        selection_reference;
                                    *(float *)(candidate_selection_state + 8) = vector_distance;
                                    *(char **)(candidate_selection_state + 4) = candidate_moby;
                                    *(float *)(candidate_selection_state + 0xC) =
                                        accumulated_amount;
                                    if (1.0f < accumulated_amount) {
                                        *(float *)(candidate_selection_state + 0xC) = 1.0f;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

CameraOffsetEffect camera_offset_effect __asm__("D_L02_00167600") = {0};

float D_L02_0016760C NOT_SDA = 0;
