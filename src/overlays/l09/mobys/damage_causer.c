/* Level 09 moby class 0x4b6: damage causer. */
#include "types.h"
#include "asm.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"

#define F(p, o) (*(float *)((p) + (o)))

#include "rnc/mobys/moby.h"

extern int  D_L09_0015F5CC;
extern int  stub_printf() __asm__("FUN_001e93b0");
extern void FUN_001f9bf8(f32, void *, void *);
extern void initialize_hit_descriptor(void *, void *, s32, f32) __asm__("FUN_L00_00259858");
extern s32  collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern char damage_causer_contact_log_format[] __asm__("D_L09_002090B0");

typedef struct {
    char  pad[0x18];
    char *hit;
} CollisionState;

extern CollisionState D_L09_00173FC0;
void                  damage_causer_update(struct Moby *moby) __asm__("FUN_L09_00305a28");

extern s32 D_L09_0015F5CC NOT_SDA;

/* Moby class 1206 */
void damage_causer_update(struct Moby *moby) {
    char *damage_causer_vars = (char *)moby->pvars;
    float sphere_query_workspace[24];
    int   i;
    float count_as_float;
    float start;
    float end;

    if (moby->state == 0) {
        moby->state = 1;
        *(s32 *)(damage_causer_vars + 0x14) = -1;
    }
    if ((D_L09_0015F5CC & 7) == (((int)moby >> 8) & 7)) {
        if (*(s32 *)(damage_causer_vars + 0x14) == -1 ||
            (voice_is_playing(moby, *(s32 *)(damage_causer_vars + 0x14)) == 0 &&
             *(s32 *)(damage_causer_vars + 0x14) == -1)) {
            *(s32 *)(damage_causer_vars + 0x14) = allocate_voice_for_target_entry(0, 4, moby);
        }
    }
    count_as_float = convert_integer_to_float(*(s32 *)(damage_causer_vars + 8));
    start = F(damage_causer_vars, 0);
    end = F(damage_causer_vars, 4);
    *(OvlQuad *)sphere_query_workspace = 0;
    sphere_query_workspace[2] = (end - (start + start)) / count_as_float;
    qcopy(sphere_query_workspace + 4, (char *)moby + 0x10);
    transform_vector_by_basis(sphere_query_workspace, sphere_query_workspace, (char *)moby + 0xc0);
    FUN_001f9bf8(F(damage_causer_vars, 0), sphere_query_workspace + 8, sphere_query_workspace);
    fast_vec_add(
        sphere_query_workspace + 4, sphere_query_workspace + 4, sphere_query_workspace + 8);
    initialize_hit_descriptor(sphere_query_workspace + 12,
                              moby,
                              *(s32 *)(damage_causer_vars + 0x10),
                              *(float *)(damage_causer_vars + 0xc));
    i = 0;
    if (*(s32 *)(damage_causer_vars + 8) > 0)
        do {
            if (collect_mobies_in_sphere(F(damage_causer_vars, 0),
                                         sphere_query_workspace + 4,
                                         1,
                                         moby,
                                         sphere_query_workspace + 12) != 0) {
                char *hit = D_L09_00173FC0.hit;
                stub_printf(damage_causer_contact_log_format,
                            *(s16 *)((char *)moby + 0xb2),
                            *(s16 *)(hit + 0xa6),
                            *(s16 *)(hit + 0xb2),
                            F(damage_causer_vars, 0xc));
            }
            i++;
            fast_vec_add(
                sphere_query_workspace + 4, sphere_query_workspace + 4, sphere_query_workspace);
        } while (i < *(s32 *)(damage_causer_vars + 8));
}
