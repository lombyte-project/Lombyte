#ifndef RNC_BROWSER_MOBY_JOINT_MATRICES_H
#define RNC_BROWSER_MOBY_JOINT_MATRICES_H

#include "types.h"

/* EE 0x2109b8: joint slots and matrices for the joints flagged in joint_flags (128 bytes;
   [0x7F] = highest joint + 1). Results stay in this module's scratchpad copy. */
void rnc_compute_moby_joint_slots(u8 *moby, const u8 *joint_flags);
/* EE 0x210850 get_moby_joint_matrices(moby, count, bones, out): 64-byte matrices to out. */
void rnc_get_moby_joint_matrices(u8 *moby, s32 count, const s32 *bones, u8 *out);
/* EE 0x2106f8: each point's joint translation (16 bytes) to out. */
void rnc_get_moby_point_positions(u8 *moby, s32 count, const s32 *points, u8 *out);
/* Every joint's 64-byte matrix to out; returns the joint count, 0 if it exceeds capacity. */
s32 rnc_moby_all_joint_matrices(u8 *moby, u8 *out, s32 capacity);

/* The local pose of every joint (no matrices); rnc_moby_pose_slot(j) is joint j's 64-byte slot:
   rotation, scale + flag word, translation + parent word, next translation. */
void rnc_moby_local_pose(u8 *moby);
const f32 *rnc_moby_pose_slot(s32 joint);
/* Level 0x252990: an animation layer's pose for its listed joints, as parked-pose entries. */
void rnc_animation_layer_pose(u8 *state);

#endif
