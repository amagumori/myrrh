#include "skeleton.h"


namespace Engine {

  struct Animation {
    Skeleton *keyframes;  // naive, but: 
  };

  class AnimationSystem {
    public:
      AnimationSystem();
      ~AnimationSystem();
      void compressAnimations();
      void updateAnimations();
      
    private:
      //animationComponent* components;
  };
}
