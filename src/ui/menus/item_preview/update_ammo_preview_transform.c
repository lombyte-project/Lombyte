#include "types.h"
#include "rnc/ui/menus/item_preview/preview_animation.h"
#include "qcopy.h"

struct AmmoPreviewMoby;
struct AmmoPreviewOwner {
    u8 pad00[0x44];
    struct AmmoPreviewMoby *source_moby;
};
struct AmmoPreviewVars {
    struct AmmoPreviewOwner *owner;
};
struct AmmoPreviewResource {
    u8 pad00[0x24];
    f32 scale;
};
struct AmmoPreviewMoby {
    u8 pad00[0x10];
    f32 position[4];
    u8 pad20[4];
    struct AmmoPreviewResource *resource;
    u8 pad28[4];
    f32 scale;
    u8 pad30[0x48];
    struct AmmoPreviewVars *preview_vars;
    u8 pad7C[0x40];
    u8 slot;
    u8 padBD[3];
    f32 basis[12];
};

extern f32 frame_delta __asm__("D_0015ED6C");
extern f32 animation_delta __asm__("D_0015ED70");
extern s32 game_frame __asm__("D_0015F438");
extern void clear_vector(void *) __asm__("func_001F99F8");
extern void add_vector_xyz(void *, void *, void *) __asm__("func_001F9A10");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void multiply_matrix_basis_columns(void *, void *, void *) __asm__("func_001FA328");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 wrap_angle(f32) __asm__("func_001FA610");
extern f32 ConvertIntegerToFloat(s32) __asm__("func_001FA6C0");
extern void advance_moby_animation(void *) __asm__("func_0020D580");
extern void refresh_moby_spatial_bounds(void *) __asm__("func_0020DEF8");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("func_0020E098");
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");

void update_ammo_preview_transform(struct AmmoPreviewMoby *moby) __asm__("FUN_00225180");

/* The hand-gadget loader installs this callback on its ammo preview Mobys.
   The slot selects a phase and a spring offset around the source Moby. */
void update_ammo_preview_transform(struct AmmoPreviewMoby *moby) {
    f32 offset[4];
    f32 rotation_basis[12];
    f32 angles[4];
    struct AmmoPreviewMoby *source_moby;
    f32 *basis;
    s32 phase_index;
    f32 phase;
    f32 orbit_angle;
    f32 phase_angle;
    f32 bob_angle;
    f32 double_phase;
    f32 zero;

    source_moby = moby->preview_vars->owner->source_moby;
    advance_moby_animation(moby);
    refresh_moby_spatial_bounds(moby);
    qcopy(moby->position, source_moby->position);
    basis = moby->basis;
    copy_matrix3x4(basis, source_moby->basis);
    normalize_vector_triplet(basis);
    moby->scale = moby->resource->scale;
    if (moby->slot < 3U) {
        phase_index = (moby->slot * 2) % 6;
    } else {
        phase_index = (moby->slot * 2 + 1) % 6;
    }
    orbit_angle = (((f32)(game_frame % 200) / ConvertIntegerToFloat(200)) * 6.28318f) - 3.14159f;
    phase = (f32)phase_index;
    phase_angle = ((phase * 6.28318f) / ConvertIntegerToFloat(6)) - 3.14159f;
    bob_angle = (((f32)(game_frame % 170) / ConvertIntegerToFloat(170)) * 6.28318f) - 3.14159f;
    double_phase = (phase * 12.56636f) / ConvertIntegerToFloat(6);
    wrap_angle(double_phase);
    zero = 0.0f;
    orbit_angle = fast_add_rotations(orbit_angle, phase_angle);
    bob_angle = fast_add_rotations(bob_angle, double_phase);
    if (ammo_preview_offsets[moby->slot] != zero) {
        advance_accelerated_scalar(&ammo_preview_offsets[moby->slot],
                                   &ammo_preview_velocities[moby->slot], zero, 1.0f,
                                   animation_delta * 6.0f, frame_delta * 6.0f);
    }
    offset[0] = fast_cos(orbit_angle);
    offset[1] = fast_sin(orbit_angle);
    offset[2] = zero;
    offset[2] = fast_sin(bob_angle) * 0.25f + 0.5f + ammo_preview_offsets[moby->slot];
    transform_vector_by_basis(offset, offset, source_moby->basis);
    add_vector_xyz(moby->position, moby->position, offset);
    clear_vector(angles);
    angles[2] = fast_add_rotations(orbit_angle, 1.5707964f);
    vu_euler_rotation_basis(rotation_basis, angles);
    multiply_matrix_basis_columns(moby->basis, rotation_basis, moby->basis);
    refresh_moby_spatial_bounds_from_basis(moby);
}

extern __typeof__(update_ammo_preview_transform) func_00225180
    __attribute__((alias("FUN_00225180")));
