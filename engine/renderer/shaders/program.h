#pragma once

#include "shader.h"
#include <vector>
#include <glm/glm.hpp>

namespace Engine {
  class Program {
    public:
      Program(const std::vector<Shader>& shaders);
      ~Program();
      GLuint object() const;
      void use() const;
      bool isInUse() const;
      void stopUsing() const;
      GLint attrib(const GLchar *attribName) const;
      GLint uniform(const GLchar *uniformName) const;
      
    private:
      GLuint _object;

      Program(const Program&);
      const Program& operator =(const Program&);
  };
}
