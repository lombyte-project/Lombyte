#ifndef LOMBYTE_RNC_OVERLAY_SWARM_PATH_H
#define LOMBYTE_RNC_OVERLAY_SWARM_PATH_H

#include "types.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/level_path.h"

struct Moby;

/* Current point, direction and path for a path follower. */
typedef struct {
    s32        point;
    s8         direction;
    u8         pad5[0xB];
    LevelPath *path;
    s32        path_id; /* index into l08_paths */
    u8         pad18[0x18];
} SwarmPathFollower;

/* A child moby and its attachment joints. */
typedef struct {
    struct Moby *moby;
    struct Moby *parent;
    s32          joint;
    s32          parent_joint;
} SwarmPart;

/* Projectiles fired at a swarm target, held in its pvars. */
typedef struct {
    u8           pad0[0x60];
    struct Moby *shots[8];
} SwarmTargetVars;

#endif /* LOMBYTE_RNC_OVERLAY_SWARM_PATH_H */
