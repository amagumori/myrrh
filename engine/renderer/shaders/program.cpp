#include "../include/Shader.h"
#include "../include/Program.h"
#include "../include/Platform.h"
#include <stdexcept>

using namespace Engine;

Program::Program(const std::vector<Shader>& shaders) :
  _object(0)
{
  if (shaders.size() <= 0) {
    throw std::runtime_error("no shaders provided.");
  }

  _object = glCreateProgram();
  if (_object == 0) {
    throw std::runtime_error("glCreateProgram failed for some reason.");
  }

  // attach the shaders...
  for (unsigned i = 0; i < shaders.size(); i++)
    glAttachShader(_object, shaders[i].object());

  // link shaders
  glLinkProgram(_object);

  // detach shaders
  for (unsigned i = 0; i < shaders.size(); i++)
    glDetachShader(_object, shaders[i].object());

  // throw exception if linking failed..
  GLint status;
  
  glGetProgramiv(_object, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    std::string msg("failed to link program: ");
  
    GLint infoLogLength;
    glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
    char *strInfoLog = new char[infoLogLength + 1];
    glGetProgramInfoLog(_object, infoLogLength, NULL, strInfoLog);
    msg += strInfoLog;
    delete[] strInfoLog;

    glDeleteProgram(_object); _object = 0;
    throw std::runtime_error(msg);
  }
}

Program::~Program() {
  if (_object != 0) glDeleteProgram(_object);
}

GLuint Program::object() const {
  return _object;
}

void Program::use() const {
  glUseProgram(_object);
}

bool Program::isInUse() const {
  GLint currentProgram = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
  return (currentProgram == (GLint)_object);
}

void Program::stopUsing() const {
  assert(isInUse());
  glUseProgram(0);
}

GLint Program::attrib(const GLchar* attribName) const {
  if (!attribName)
    throw std::runtime_error("attrib name was null ya dingus");

  GLint attrib = glGetAttribLocation(_object, attribName);
  if (attrib == -1)
    throw std::runtime_error(std::string("program attribute not found: ") + attribName);
}

GLint Program::uniform(const GLchar* uniformName) const {
  if (!uniformName)
    throw std::runtime_error("uniform name was null ya dingus");

  GLint uniform = glGetUniformLocation(_object, uniformName);
  if (uniform == -1)
    throw std::runtime_error(std::string("program uniform not found: ") + uniformName);
}
