#include <glm/glm.hpp>

namespace Engine {
  struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float attenuation;
    float ambientCoefficient;
  };

  struct Spotlight : Light {
    float coneAngle;
    glm::vec3 coneDirection;
  };
}
