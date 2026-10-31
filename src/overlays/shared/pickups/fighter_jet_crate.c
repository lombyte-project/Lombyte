/* Shared moby classes ammocrate and jet_fighter_health_crate: the ship pickups and their spawner. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"

extern int D_L13_00174158_lui __asm__("D_L13_00174158") NOT_SDA;

extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void
update_moby_vertical_oscillation(float, float, void *, void *, void *) __asm__("FUN_L00_00262b00");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B8FC0.c: func_L13_002E2F98), where it is exact; names translated to the US level program. */

extern char *create_moby(int);

unsigned char *spawn_ship_pickup(char *position, int moby_class) __asm__("FUN_L13_002e1c50");

unsigned char *spawn_ship_pickup(char *position, int moby_class) {
    unsigned char *moby = create_moby(moby_class);
    if (moby != 0) {
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        qcopy(moby + 0x10, position);
        update_entity(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}

void ammocrate_update(char *moby) __asm__("FUN_L13_002e1cc8");

/* Moby classes 224, 228 */
void ammocrate_update(char *moby) {
    enum {
        SHIP_PICKUP_INITIALIZE = 0,
        SHIP_PICKUP_SELECT_ACTIVATION_GATE = 1,
        SHIP_PICKUP_WAIT_FOR_TARGET_STATE = 2,
        SHIP_PICKUP_WAIT_FOR_ACTIVATION_TIMER = 3,
        SHIP_PICKUP_ACTIVE = 4,
        SHIP_PICKUP_COLLECTED_SHRINKING = 5,
        SHIP_PICKUP_REMOVE_OR_RESET = 6
    };
    int  *pickup_private_state = ((int *)MOBY(moby)->pvars);
    int   hero_moby = ((char *)hero.ship_moby);
    int   hit = 0;
    float distance = 100000.0f;
    float position[4];
    float direction[4];
    float start_size, activated_size;
    short moby_class;
    float speed;
    switch ((unsigned char)moby[0x20]) {
    case SHIP_PICKUP_INITIALIZE:
        if (*((unsigned char *)pickup_private_state + 0xc) != 0) {
            MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24);
            moby[0x20] = 4;
            break;
        }
        MOBY(moby)->collision = 0;
        moby[0x31] = 0;
        MOBY(moby)->flags |= 1;
        start_size = *(float *)(((char *)MOBY(moby)->pclass) + 0x24);
        moby[0x20] = 1;
        MOBY(moby)->scale = start_size * 0.1f;
        break;
    case SHIP_PICKUP_SELECT_ACTIVATION_GATE:
        if (pickup_private_state[0] != -1) {
            moby[0x20] = 2;
            break;
        }
        moby[0x20] = 3;
        break;
    case SHIP_PICKUP_WAIT_FOR_TARGET_STATE: {
        int          target_index = pickup_private_state[0];
        int          target_base = (int)moby_base;
        struct Moby *target = (struct Moby *)(target_base + target_index * 0x100);
        if (target != 0 && target->state != 0xfe && target->state != 0xfd &&
            pickup_private_state[1] != -1 && target->state != pickup_private_state[1]) {
            goto frame_tail;
        }
        moby[0x20] = 3;
        break;
    }
    case SHIP_PICKUP_WAIT_FOR_ACTIVATION_TIMER:
        if (pickup_private_state[2] == -1 || fast_dec_timer(pickup_private_state + 4)) {
            moby[0x20] = 4;
        }
        if ((unsigned char)moby[0x20] != 4)
            goto frame_tail;
        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        moby[0x31] = 1;
        MOBY(moby)->flags &= 0xfffe;
        activated_size = *(float *)(((char *)MOBY(moby)->pclass) + 0x24);
        MOBY(moby)->scale = activated_size * 0.1f;
        break;
    case SHIP_PICKUP_ACTIVE:
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, frame_time * 0.7853982f);
        if (MOBY(moby)->pos.x < 20.0f)
            MOBY(moby)->pos.x = 20.0f;
        if (1003.0f < MOBY(moby)->pos.x)
            MOBY(moby)->pos.x = 1003.0f;
        if (MOBY(moby)->pos.y < 20.0f)
            MOBY(moby)->pos.y = 20.0f;
        if (1003.0f < MOBY(moby)->pos.y)
            MOBY(moby)->pos.y = 1003.0f;
        if (MOBY(moby)->pos.z < 20.0f)
            MOBY(moby)->pos.z = 20.0f;
        if (1003.0f < MOBY(moby)->pos.z)
            MOBY(moby)->pos.z = 1003.0f;
        moby_point_position(moby, 0, position);
        MOBY(moby)->scale +=
            (*(float *)(((char *)MOBY(moby)->pclass) + 0x24) - MOBY(moby)->scale) * 0.1f;
        if (coll_sphere(2.0f, position, 1, moby) && D_L13_00174158_lui != 0) {
            short moby_class = *(short *)(D_L13_00174158_lui + 0xa6);
            if (moby_class == 0x4da || moby_class == 0x45 || moby_class == 0x563)
                hit = 1;
        }
        if (hero.state.current == 0x32 && hero_moby != 0 && MOBY(hero_moby)->state != 0xfe &&
            MOBY(hero_moby)->state != 0xfd) {
            short moby_class = MOBY(hero_moby)->oclass;
            if (moby_class == 0x4da || moby_class == 0x45 || moby_class == 0x563)
                distance = fast_vec_dist((void *)&MOBY(hero_moby)->pos, moby + 0x10);
        }
        if (hit || distance < 4.0f) {
            moby_class = MOBY(moby)->oclass;
            MOBY(moby)->collision = 0;
            if (moby_class == 0xe0)
                goto collect_ammo;
            if (moby_class == 0xe4)
                goto collect_health;
            goto collected;
        collect_ammo:
            hero.ship_ammo += 5;
            if (((char *)hero.ship_moby) != 0 &&
                *(unsigned char *)(((char *)hero.ship_moby) + 0x20) != 0xfe &&
                *(unsigned char *)(((char *)hero.ship_moby) + 0x20) != 0xfd)
                allocate_voice_for_target_entry(4, 0, hero.ship_moby);
            if (hero.ship_ammo_max < hero.ship_ammo)
                hero.ship_ammo = hero.ship_ammo_max;
            goto collected;
        collect_health:
            hero.ship_hp += 10.0f;
            if (((char *)hero.ship_moby) != 0 &&
                *(unsigned char *)(((char *)hero.ship_moby) + 0x20) != 0xfe &&
                *(unsigned char *)(((char *)hero.ship_moby) + 0x20) != 0xfd)
                allocate_voice_for_target_entry(5, 0, hero.ship_moby);
            if (hero.ship_hp_max < hero.ship_hp)
                hero.ship_hp = hero.ship_hp_max;
        collected:
            moby[0x20] = 5;
            break;
        } else if (distance < 40.0f && (MOBY(moby)->visible) != 0) {
            float step = 0.8f / square_root(distance);
            fast_vec_sub(direction, (void *)&MOBY(hero_moby)->pos, moby + 0x10);
            fast_vec_normalize(direction, direction, step);
            fast_vec_add(moby + 0x10, moby + 0x10, direction);
        }
        goto frame_tail;
    case SHIP_PICKUP_COLLECTED_SHRINKING: {
        float delta = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) - MOBY(moby)->scale;
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, delta + delta);
        MOBY(moby)->scale +=
            (*(float *)(((char *)MOBY(moby)->pclass) + 0x24) * 0.01f - MOBY(moby)->scale) * 0.3f;
        if (MOBY(moby)->scale < *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * 0.05f)
            moby[0x20] = 6;
        break;
    }
    case SHIP_PICKUP_REMOVE_OR_RESET:
        if (*((unsigned char *)pickup_private_state + 0xd) == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        MOBY(moby)->collision = 0;
        moby[0x31] = 0;
        MOBY(moby)->flags |= 1;
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * 0.1f;
        pickup_private_state[4] = pickup_private_state[2];
        moby[0x20] = 1;
        break;
    }
frame_tail:
    speed = frame_time;
    update_moby_vertical_oscillation(
        0.7f, speed * 1.2217305f, moby, pickup_private_state + 6, pickup_private_state + 5);
    if (MOBY(moby)->pos.x < 20.0f)
        MOBY(moby)->pos.x = 20.0f;
    if (1003.0f < MOBY(moby)->pos.x)
        MOBY(moby)->pos.x = 1003.0f;
    if (MOBY(moby)->pos.y < 20.0f)
        MOBY(moby)->pos.y = 20.0f;
    if (1003.0f < MOBY(moby)->pos.y)
        MOBY(moby)->pos.y = 1003.0f;
    if (MOBY(moby)->pos.z < 20.0f)
        MOBY(moby)->pos.z = 20.0f;
    if (1003.0f < MOBY(moby)->pos.z)
        MOBY(moby)->pos.z = 1003.0f;
}
