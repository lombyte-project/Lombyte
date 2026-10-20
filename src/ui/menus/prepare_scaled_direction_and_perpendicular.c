#include "types.h"
#include "sda.h"
#include "eetypes.h"

extern f32 D_00160370[] MACRO_ADDR;
extern f32 D_00160380[] MACRO_ADDR;
extern f32 D_00160390[] MACRO_ADDR;
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void fast_vec_normalize(f32 *, f32 *, f32) __asm__("FUN_001f9bf8");

typedef union {
    u128 q;
    f32 v[4];
} PauseVec;

void prepare_scaled_direction_and_perpendicular(PauseVec *dir, f32 scale) __asm__("FUN_00226fb8");

void prepare_scaled_direction_and_perpendicular(PauseVec *dir, f32 scale) {
    PauseVec direction_copy;
    f32 *v = direction_copy.v;

    direction_copy.q = dir->q;
    fast_vec_normalize(D_00160370, v, 1.0f);
    D_00160390[0] = D_00160370[0] * scale;
    D_00160390[1] = D_00160370[1] * scale;
    D_00160390[2] = D_00160370[2] * scale;
    if (v[0] < v[1]) {
        if (v[0] < v[2]) {
            D_00160380[0] = v[0];
            D_00160380[1] = v[2];
            D_00160380[2] = v[1];
        } else {
            D_00160380[0] = v[1];
            D_00160380[1] = v[0];
            D_00160380[2] = v[2];
        }
    } else if (v[1] < v[2]) {
        D_00160380[0] = v[2];
        D_00160380[1] = v[1];
        D_00160380[2] = v[0];
    } else {
        D_00160380[0] = v[1];
        D_00160380[1] = v[0];
        D_00160380[2] = v[2];
    }
    fast_vec_cross(D_00160380, D_00160380, D_00160370);
    fast_vec_normalize(D_00160380, D_00160380, 1.0f);
}

extern __typeof__(prepare_scaled_direction_and_perpendicular) func_00226FB8 __attribute__((alias("FUN_00226fb8")));
