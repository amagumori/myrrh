#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "skeleton.h"

using namespace Engine;

/* naive implementation */
/*
Skeleton::localToGlobalPose() {
  localToGlobalPose(&joints[0], glm::mat4());   // empty mat4 constructor = identity mat4.
}

Skeleton::localToGlobalPose(Joint *joint, Orientation parentTransform) {
  // this should do a quaternion multiply
  joint->globalPose.quaternion = parentTransform.quaternion * joint->localPose.quaternion;
  // this should do a vec4 multiply?? i think??
  joint->globalPose.position = parentTransform.position * joint->localPose.position;

  for (size_t i=0; i < joint->children.size(); ++i) {
    localToGlobalPose(joint->children[i], joint->globalPose);
  }
}

*/

Orientation Skeleton::localToFinalPose(void) {
  // the root of the hierarchy doesn't have a parent.
  globalPoses[0] = localPoses[0];

  unsigned numJoints = sizeof(hierarchy) / sizeof(uint16_t);
  
  for (unsigned int i=1; i < numJoints; ++i) {
    const uint16_t parentJoint = hierarchy[i];
    // this should be an overloaded quaternion multiply.
    globalPoses[i].quaternion = globalPoses[parentJoint].quaternion * localPoses[i].quaternion;
    // this should be an overloaded vector add.
    globalPoses[i].position = globalPoses[parentJoint].position + localPoses[i].position;
  }
}

  

#endif
