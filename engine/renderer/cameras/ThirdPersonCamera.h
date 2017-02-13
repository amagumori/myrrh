#include "Camera.h"

namespace Engine {
  class ThirdPersonCamera : Camera {
    public:
      void setRadius ( float rad );
      void resetCamera();
    private: 
      float _radius;
  }
}
