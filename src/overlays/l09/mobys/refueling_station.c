/* Level 09 moby class 0x494: refueling station. */
#include "types.h"
#include "asm.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/mobys/moby.h"

/* Damage source returned by find_moby_hit. */
typedef struct {
    u8  pad0[0x2C];
    f32 damage;
} VendorHitSource;

typedef struct {
    u8  pad0[0x26];
    s16 hurt_timer;
    u8  pad28[0x38];
    u8  anim[8];
    u8  pad68[8];
    s32 link;
} VendorVars;

extern struct Moby *moby_list __asm__("D_L09_0015FFD8");

char D_L09_00208F90[] = "Moby class %d Damage Received: %f\n";

extern s32  fptodp(f32) __asm__("FUN_00120478");
extern void debug_print(char *, s32, s32) __asm__("FUN_001e93b0");
extern s32 fun_00213928(struct Moby *, VendorHitSource *, f32 *, s32, s32 *, s32, s32, s32) __asm__("FUN_00213928");
extern void FUN_L00_00257470(struct Moby *, s32, s32);
extern void trooper_anim_update(struct Moby *, u8 *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(struct Moby *, u8 *) __asm__("FUN_L00_0025d538");
void        handle_refueling_station_damage(struct Moby *moby,
                                            VendorVars  *vars,
                                            f32         *health) __asm__("FUN_L09_00303b30");

/* Pressure plate: waits for its trigger slot, hums while armed, wakes its linked moby when stepped on, then shatters. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_003050C0), where it is exact; names translated to the US level program. */
extern char         D_0013D388[];
extern float        D_L09_00166F40[];
extern int          D_L09_0015F5CC;
extern void         spawn_explosion(void *,
                                    void *,
                                    void *,
                                    float,
                                    float,
                                    int,
                                    int,
                                    int,
                                    float,
                                    float,
                                    float,
                                    float,
                                    int,
                                    float,
                                    int,
                                    int,
                                    int,
                                    int) __asm__("FUN_L00_0025e450");
void                refueling_station_update(struct Moby *m) __asm__("FUN_L09_00303d10");
typedef struct Moby Moby;
extern Moby        *moby_list __asm__("D_L09_0015FFD8");
extern void         spawn_explosion(void *,
                                    void *,
                                    void *,
                                    float,
                                    float,
                                    int,
                                    int,
                                    int,
                                    float,
                                    float,
                                    float,
                                    float,
                                    int,
                                    float,
                                    int,
                                    int,
                                    int,
                                    int) __asm__("FUN_L00_0025e450");

extern s32 D_L09_0015F5CC NOT_SDA;

void handle_refueling_station_damage(struct Moby *moby, VendorVars *vars, f32 *health) {
    enum {
        DAMAGE_RESULT_UNAVAILABLE = 0
    };
    VendorHitSource *damage_hit_record;
    struct Moby     *link;
    s32              hit;

    if (moby->state != 3) {
        damage_hit_record = find_moby_hit(moby, 0x330000, 0);
        if (damage_hit_record != 0) {
            debug_print(D_L09_00208F90, moby->oclass, fptodp(damage_hit_record->damage));
        }
        switch (fun_00213928(moby, damage_hit_record, health, 0, &hit, 0, 0, 4)) {
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
        case 11:
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
                FUN_L00_00257470(moby, 0, -1);
                vars->anim[7] = 0x78;
                trooper_anim_update(moby, vars->anim);
                moby->state = 3;
            } else {
                *health -= damage_hit_record->damage;
                vars->anim[7] = 0xFA;
                vars->hurt_timer = scale_game_frames(60);
                trooper_anim_update(moby, vars->anim);
            }
        }
        moby->hit_slot = 0xFF;
    }
    trooper_anim_finish(moby, vars->anim);
}

void refueling_station_update(struct Moby *moby) {
    enum {
        REFUELING_STATION_STATE_INITIALIZE = 0,
        REFUELING_STATION_STATE_WAIT_COMMAND = 1,
        REFUELING_STATION_STATE_EXPLOSION_DELAY = 2,
        REFUELING_STATION_STATE_EXPLODE = 3
    };
    char          *station_vars = (char *)moby->pvars;
    float          explosion_offset[4];
    unsigned char *removal_check_base;
    handle_refueling_station_damage(moby, station_vars, station_vars + 0x20);
    switch (moby->state) {
    case REFUELING_STATION_STATE_INITIALIZE:
        if (*(short *)(station_vars + 0x76) != -1 &&
            (removal_check_base =
                 (unsigned char *)D_0013D388 + *(short *)(station_vars + 0x76))[0x39] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        *(int *)(station_vars + 0x7C) = -1;
        moby->state = 1;
        break;
    case REFUELING_STATION_STATE_WAIT_COMMAND:
        if ((D_L09_0015F5CC & 7) == (((int)moby >> 8) & 7)) {
            if (*(int *)(station_vars + 0x7C) == -1 ||
                (voice_is_playing(moby, *(int *)(station_vars + 0x7C)) == 0 &&
                 *(int *)(station_vars + 0x7C) == -1)) {
                *(int *)(station_vars + 0x7C) = allocate_voice_for_target_entry(1, 4, moby);
            }
        }
        if (moby->command == 1) {
            if (*(int *)(station_vars + 0x78) != -1) {
                unsigned char *o =
                    (unsigned char *)((char *)moby_list + (*(int *)(station_vars + 0x78) << 8));
                if (o[0x20] == 1)
                    o[0xBC] = o[0x20];
            }
            *(short *)(station_vars + 0x74) = scale_game_frames(0xA);
            if (*(int *)(station_vars + 0x7C) != -1) {
                char *voice_entry = (char *)&voice_pool + *(int *)(station_vars + 0x7C) * 0x70;
                if (*(char **)(voice_entry + 0x88) == moby &&
                    ((unsigned char *)voice_entry)[0x74] != 0) {
                    release_voice_slot(*(int *)(station_vars + 0x7C));
                }
            }
            *(int *)(station_vars + 0x7C) = -1;
            moby->update_dist = 0xFF;
            moby->state = 2;
        }
        break;
    case REFUELING_STATION_STATE_EXPLOSION_DELAY:
        if (fast_dec_timer_s16(station_vars + 0x74)) {
            if (*(int *)(station_vars + 0x70) != -1) {
                unsigned char *o =
                    (unsigned char *)((char *)moby_list + (*(int *)(station_vars + 0x70) << 8));
                if (o != 0 && o[0x20] != 0xFE && o[0x20] != 0xFD &&
                    (*(short *)(o + 0xA6) == 0x494 || *(short *)(o + 0xA6) == 0x49D ||
                     *(short *)(o + 0xA6) == 0x4A0)) {
                    o[0x30] = 0xFF;
                    o[0xBC] = 1;
                }
            }
            moby->state = 3;
        }
        break;
    case REFUELING_STATION_STATE_EXPLODE:
        fast_vec_sub(explosion_offset, D_L09_00166F40, &moby->pos);
        spawn_explosion(moby,
                        explosion_offset,
                        0,
                        0.0f,
                        0.0f,
                        0x14,
                        9,
                        0x20,
                        10.0f,
                        7.0f,
                        20.0f,
                        2.0f,
                        0,
                        40.0f,
                        0,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        break;
    }
}
