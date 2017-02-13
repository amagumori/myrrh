/*
 * just notes for now.
 *
 * what sort of data layout to use for skeletons and animations?
 *
 * first off, positions should all be vec4s, NOT vec3s.
 * this way they can be word-aligned for SIMD stuff.
 *
 * naive layout:
 * struct Joint {
 *   mat4x4 globalPose;
 *   mat4x4 localPose;
 *   vector<Joint*> children;
 * }
 *
 * instead of storing children for each joint, store a parent.
 * use indices into an array instead of pointers.
 * 
 * the index into array can be a uint16 which saves space compared to a
 * pointer type ( word-sized ).
 * this also makes copying way easier because no fucking with pointers.
 *
 * so now, traverse the hierarchical layout depth-first to flatten it into a 
 * array of joints.  now a joint i can only have a parent joint j if j < i.
 *
 * now change the structure layout to SoA instead of AoS.
 * huge cache locality bonus.
 *
 * how to optimize data layout / size of quaternion representation?
 * how to blend between quaternions?
 * how to transform to global (world-space) pose?
 * pinned joints, get rid of accumulating error?
 *
 * at what time do you sample the animation curve to get local pose?
 * blend between different local poses to smooth the animation?
 * transform local pose into global pose according to hierarchy.
 *
 * inverse / forward kinematics????? :O
 *
 *
 */ 

#include "animation.h"

using namespace Engine;

AnimationSystem::AnimationSystem() {
}

void AnimationSystem::compressAnimations() {
  // according to bitsquid ("the bitsquid low level animation system")
  // you can use a hermite spline.
}
