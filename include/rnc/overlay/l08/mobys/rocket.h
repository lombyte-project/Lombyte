#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_ROCKET_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_ROCKET_H

struct Moby;

/* Defined in src/overlays/l08/mobys/rocket.c. */
struct Moby *spawn_rocket(void        *pos,
                          struct Moby *target,
                          void        *velocity,
                          float        pitch,
                          float        yaw) __asm__("FUN_L08_002de3e0");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_ROCKET_H */
