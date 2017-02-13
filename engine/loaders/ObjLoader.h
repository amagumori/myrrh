#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

void loadOBJ(const char *filename,
             std::vector<glm::vec3> vertices,
             std::vector<glm::vec2> uvs,
             std::vector<glm::vec3> normals);
