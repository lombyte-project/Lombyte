/* Level 11 moby class owner_linked_moby: hit timer and state of a moby tied to its owner. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

/* Runs the moby's hit timer: on a hit it either finishes the stun or counts it down and plays the hurt animation. */
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int   fptodp(float) __asm__("FUN_00120478");
extern void  print_fe50(char *, int, int) __asm__("FUN_001e93b0");
extern int   resolve_moby_hit_response(void *, void *, float *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void sound_fe50(void *, void *) __asm__("FUN_L00_0025d458");
extern void update_fe50(void *, void *) __asm__("FUN_L00_0025d538");
char        D_L11_0021B530[] = "Moby class %d Damage Received: %f\n";

void handle_owner_linked_moby_hits(struct Moby   *moby,
                                   unsigned char *private_data,
                                   float         *health) __asm__("FUN_L11_0030fe50");
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_003114F0), where it is exact; names translated to the US level program. */

void handle_owner_linked_moby_hits(struct Moby   *m,
                                   unsigned char *d,
                                   float         *t) __asm__("FUN_L11_0030fe50");

void handle_owner_linked_moby_hits(struct Moby *moby, unsigned char *private_data, float *health) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_DEATH = 1,
        HIT_REACTION_DEATH_KNOCKBACK = 2,
        HIT_REACTION_KNOCKBACK = 3,
        HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE = 4,
        HIT_REACTION_NO_RESPONSE = 11
    };
    int   out;
    char *info;
    if (moby->state != 4) {
        info = find_moby_hit(moby, 0x330000, 0);
        if (info != 0) {
            print_fe50(D_L11_0021B530, moby->oclass, fptodp(*(float *)(info + 0x2C)));
        }
        switch (resolve_moby_hit_response(moby, info, health, 0, &out, 0, 0, 4)) {
        case HIT_REACTION_DEATH:
        case HIT_REACTION_DEATH_KNOCKBACK:
            *health = 0.0f;
            break;
        case HIT_REACTION_NONE:
        case HIT_REACTION_KNOCKBACK:
        case HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE:
        case HIT_REACTION_NO_RESPONSE:
            break;
        }
        if (out >= 2) {
            if (*health <= *(float *)(info + 0x2C)) {
                unsigned char *owner_moby;
                *health = 0.0f;
                moby->flags &= 0xEFFF;
                private_data[0x67] = 0x78;
                sound_fe50(moby, private_data + 0x60);
                if (moby->prev_seq != 2) {
                    blend_moby_animation(moby, 2, 0, 3);
                }
                owner_moby = *(unsigned char **)(private_data + 0x70);
                if (owner_moby != 0 && owner_moby[0x20] != 0xFE && owner_moby[0x20] != 0xFD) {
                    owner_moby[0x20] = 3;
                }
                moby->state = 3;
            } else {
                *health -= *(float *)(info + 0x2C);
                private_data[0x67] = 0xFA;
                *(short *)(private_data + 0x26) = scale_game_frames(0x3C);
                sound_fe50(moby, private_data + 0x60);
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 0, 3);
                }
            }
        }
        moby->hit_slot = 0xFF;
    }
    update_fe50(moby, private_data + 0x60);
}

void update_owner_linked_moby_state(struct Moby *moby) __asm__("FUN_L11_00310028");

void update_owner_linked_moby_state(struct Moby *moby) {
    enum {
        LINKED_MOBY_STATE_START_ANIMATION = 0,
        LINKED_MOBY_STATE_WAIT_FOR_ANIMATION_FLAG = 1,
        LINKED_MOBY_STATE_HANDLE_HITS = 2,
        LINKED_MOBY_STATE_FADE_OUT = 3,
        LINKED_MOBY_STATE_REMOVE = 4
    };
    char        *data = (char *)moby->pvars;
    char        *health_ptr = data + 0x20;
    struct Moby *owner_moby = *(unsigned char **)(data + 0x70);
    if (owner_moby == 0 || owner_moby->state == 0xFE || owner_moby->state == 0xFD) {
        if (moby->state < 3) {
            moby->state = 4;
        }
    }
    switch (moby->state) {
    case LINKED_MOBY_STATE_START_ANIMATION:
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 0xF);
        }
        moby->state = 1;
        break;
    case LINKED_MOBY_STATE_WAIT_FOR_ANIMATION_FLAG:
        if (moby->anim_flags & 2) {
            moby->hit_slot = 0xFF;
            moby->state = 2;
        }
        break;
    case LINKED_MOBY_STATE_HANDLE_HITS:
        handle_owner_linked_moby_hits(moby, data, health_ptr);
        if ((moby->anim_flags & 2) && moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 0xA);
        }
        break;
    case LINKED_MOBY_STATE_FADE_OUT:
        if (moby->alpha != 0) {
            moby->alpha--;
        }
        if (compute_interpolated_record_value(moby) > 13.5f) {
            moby->state = 4;
        }
        break;
    case LINKED_MOBY_STATE_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}
