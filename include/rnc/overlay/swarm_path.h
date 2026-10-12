#ifndef LOMBYTE_RNC_OVERLAY_SWARM_PATH_H
#define LOMBYTE_RNC_OVERLAY_SWARM_PATH_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;

/* A looping waypoint path with 16-byte points. */
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 points[1];
} SwarmPath;

/* Current point, direction and path for a path follower. */
typedef struct {
    s32 point;
    s8 direction;
    u8 pad5[0xB];
    SwarmPath *path;
    s32 path_id; /* index into D_L08_001B0CB0 */
    u8 pad18[0x18];
} SwarmPathFollower;

/* A child moby and its attachment joints. */
typedef struct {
    struct Moby *moby;
    struct Moby *parent;
    s32 joint;
    s32 parent_joint;
} SwarmPart;

/* Projectiles fired at a swarm target, held in its pvars. */
typedef struct {
    u8 pad0[0x60];
    struct Moby *shots[8];
} SwarmTargetVars;

#endif /* LOMBYTE_RNC_OVERLAY_SWARM_PATH_H */
