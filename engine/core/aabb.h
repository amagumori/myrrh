#pragma once

#include "types.h"

namespace Engine {

  struct trivialAABB {
    vec3 points[8];
  };

  struct AABB {
    vec3 halfExtent;
    vec3 center;
  };

}
