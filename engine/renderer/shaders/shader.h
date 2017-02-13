#pragma once

#include <GL/glew.h>
#include <string>

namespace Engine {

  class Shader {
    public:
      static Shader shaderFromFile(const std::string &filePath, GLenum shaderType);
      Shader(const std::string &shaderCode, GLenum shaderType);
      GLuint object() const;
      
      // shaders can be copied and assigned.
      Shader(const Shader &other);
      Shader& operator =(const Shader &other);
      ~Shader();
    private:
      GLuint _object;
      unsigned* _referenceCount;
      void _retain();
      void _release();
  };
}
