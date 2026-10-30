/* Compile-time EE layout checks; never linked into the game. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/overlay/swarm_path.h"

SIZE_CHECK(path, LevelPath, 0x20);
OFFSET_CHECK(count, LevelPath, count, 0);
OFFSET_CHECK(points, LevelPath, points, 0x10);
SIZE_CHECK(follower, SwarmPathFollower, 0x30);
OFFSET_CHECK(point, SwarmPathFollower, point, 0);
OFFSET_CHECK(direction, SwarmPathFollower, direction, 4);
OFFSET_CHECK(path, SwarmPathFollower, path, 0x10);
OFFSET_CHECK(path_id, SwarmPathFollower, path_id, 0x14);
SIZE_CHECK(part, SwarmPart, 0x10);
OFFSET_CHECK(moby, SwarmPart, moby, 0);
OFFSET_CHECK(parent, SwarmPart, parent, 4);
OFFSET_CHECK(joint, SwarmPart, joint, 8);
OFFSET_CHECK(parent_joint, SwarmPart, parent_joint, 0xC);
SIZE_CHECK(target_vars, SwarmTargetVars, 0x80);
OFFSET_CHECK(shots, SwarmTargetVars, shots, 0x60);
