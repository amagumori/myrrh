#include "core/types.h"
#include "core/quaternion.h"

namespace Engine {

  // this should probably go in Quaternion.h
  // this should probably be half the size of a mat4 since the pos+scale
  // and quaternions are both vec4s.
  //
  // right now the orientation struct is 64 bytes because it uses 
  // double precision vec4s.
  //
  // with floats it would be 32 bytes.
  // cache line = 64 bytes.
  // either way, no packing necessary + this translates well to SIMD/SSE/AVX.
  
  typedef struct orientation {
    vec4 position;  // can also holds constant scale in 4th vector value?
    Quaternion quaternion;
  } Orientation;

  // or dis one
  typedef struct dq_orientation {
    Quaternion rotation;
    Quaternion translation;
  } DQOrientation;

  class Skeleton {

    public:

      // localPoses[i] * parentTransform * inverseBindPoses[i]
      Orientation localToFinalPose(void);
      void num_joints() const;


    private:
      // these should be allocated right next to each other in memory.
      // this can be done with a linear allocator, or allocating the entire memory
      // for the skeleton at once.
      // could make a constructor that takes an allocator reference or something.
      uint16_t* hierarchy;
      Orientation parentTransform;  // ???
      Orientation* inverseBindPoses;
      Orientation* localPoses;
      Orientation* globalPoses;
  };

  class SkeletonManager {
    public:
      void updateSkeletons();
      
    private:
      
  };

}
