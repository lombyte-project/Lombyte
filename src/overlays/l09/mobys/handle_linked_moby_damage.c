/* Level 09 linked moby damage handler (no caller in C). */
#include "types.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/mobys/moby.h"

/* Damage source returned by find_moby_hit. */
typedef struct {
    u8  pad0[0x2C];
    f32 damage;
} VendorHitSource;

extern struct Moby *moby_list __asm__("D_L09_0015FFD8");

char D_L09_00208FF0[] = "Moby class %d Damage Received: %f\n";

extern s32  fptodp(f32) __asm__("FUN_00120478");
extern void debug_print(char *, s32, s32) __asm__("FUN_001e93b0");
extern s32  resolve_moby_hit_response(
     struct Moby *, VendorHitSource *, f32 *, s32, s32 *, s32, s32, s32) __asm__("FUN_00213928");
extern void FUN_L00_00257470(struct Moby *, s32, s32);

typedef struct {
    u8  pad0[0x26];
    s16 hurt_timer;
    u8  pad28[0x38];
    s32 link;
    s32 link2;
} VendorVars2;

void                handle_linked_moby_damage(struct Moby *moby,
                                              VendorVars2 *vars,
                                              f32         *health) __asm__("FUN_L09_00303fc8");
typedef struct Moby Moby;
extern Moby        *moby_list __asm__("D_L09_0015FFD8");

void handle_linked_moby_damage(struct Moby *moby, VendorVars2 *vars, f32 *health) {
    enum {
        DAMAGE_RESULT_UNAVAILABLE = 0,
        DAMAGE_RESULT_NO_HIT_OR_SUPPRESSED_RESPONSE = 11
    };
    VendorHitSource *damage_hit_record;
    struct Moby     *link;
    s32              hit;

    if (moby->state != 3) {
        damage_hit_record = find_moby_hit(moby, 0x330000, 0);
        if (damage_hit_record != 0) {
            debug_print(D_L09_00208FF0, moby->oclass, fptodp(damage_hit_record->damage));
        }
        switch (resolve_moby_hit_response(moby, damage_hit_record, health, 0, &hit, 0, 0, 4)) {
        case 1:
        case 2:
            *health = 0.0f;
            break;
        case DAMAGE_RESULT_UNAVAILABLE:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case DAMAGE_RESULT_NO_HIT_OR_SUPPRESSED_RESPONSE:
            break;
        }
        if (hit >= 2) {
            if (*health <= damage_hit_record->damage) {
                *health = 0.0f;
                moby->flags &= ~0x1000;
                if (vars->link != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD &&
                        (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                /* Retail checks the 0x64 link but indexes with the 0x60 one. */
                if (vars->link2 != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD &&
                        (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                FUN_L00_00257470(moby, 0, -1);
                moby->state = 3;
            } else {
                *health -= damage_hit_record->damage;
                vars->hurt_timer = scale_game_frames(60);
            }
        }
        moby->hit_slot = 0xFF;
    }
}
