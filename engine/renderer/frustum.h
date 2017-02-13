#include "../core/aabb.h"
#include "vec4.h"   // wherever my vector math shit is

namespace Engine {

  // plane equation: ax + by + cz + d = 0;
  struct Plane {
    float a, b, c, d;
  };

  class Frustum {
    public:
      Frustum();
      ~Frustum();
      inline bool testAABB(trivialAABB& aabb) {
        bool inside;
        for (int i=0; i < 4; i++) {
          for (int j=0; j < 8; j++) {
            if (dot(aabb.vertices[j], _planes[i] > 0) {
              inside = true;
              break;
            }
          }
        }  
        if (!inside) {
          return false;
        }
        return true;
      }
      
    private:
      Plane _planes[4];
  };


}
