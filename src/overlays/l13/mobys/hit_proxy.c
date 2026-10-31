/* Level 13 hit proxy moby class (0x660): passes the hits it takes on to its owner (the forcefield tower), and its spawn helper. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");

extern char *create_moby(int);

char *spawn_hit_proxy(char *owner, void *position, float scale) __asm__("FUN_L13_0030bfb0");

char *spawn_hit_proxy(char *owner_moby, void *position, float scale) {
    char *moby = create_moby(0x660);
    if (moby != 0) {
        char *spawned_moby_vars = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(char **)(spawned_moby_vars + 0x60) = owner_moby;
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * scale;
        qcopy(moby + 0x10, position);
        if (owner_moby != 0) {
            *(int *)(spawned_moby_vars + 0x64) = *(short *)(owner_moby + 0xA6);
        }
        update_entity(moby);
    }
    return moby;
}

void hit_proxy_update(struct Moby *moby) __asm__("FUN_L13_0030c060");

/* Moby class 1632 */
void hit_proxy_update(struct Moby *moby) {
    char *owner_tracking_vars = (char *)moby->pvars;
    char *hit_recipient_moby = *(char **)(owner_tracking_vars + 0x60);
    char *hit_record;
    char  forwarded_hit_descriptor[0x30];
    if (hit_recipient_moby == 0 ||
        *(short *)(hit_recipient_moby + 0xA6) != *(int *)(owner_tracking_vars + 0x64) ||
        (unsigned char)hit_recipient_moby[0x20] == 0xFE ||
        (unsigned char)hit_recipient_moby[0x20] == 0xFD) {
        mark_moby_for_removal(moby);
        return;
    }
    hit_record = find_moby_hit(moby, -1, 0);
    if (hit_record != 0) {
        qcopy(forwarded_hit_descriptor, hit_record + 0x10);
        *(int *)(forwarded_hit_descriptor + 0x10) = *(int *)(hit_record + 0x20);
        *(int *)(forwarded_hit_descriptor + 0x14) = *(int *)(hit_record + 0x24);
        forwarded_hit_descriptor[0x18] = hit_record[0x28];
        forwarded_hit_descriptor[0x19] = hit_record[0x29];
        *(short *)(forwarded_hit_descriptor + 0x1A) = *(short *)(hit_record + 0x2A);
        *(float *)(forwarded_hit_descriptor + 0x1C) = *(float *)(hit_record + 0x2C);
        *(int *)(forwarded_hit_descriptor + 0x20) = *(int *)(hit_record + 0x30);
        record_moby_hit(*(void **)(owner_tracking_vars + 0x60), forwarded_hit_descriptor);
    }
    moby->hit_slot = 0xFF;
}
