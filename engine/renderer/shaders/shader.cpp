#include "../include/Shader.h"
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

using namespace Engine;

Shader::Shader( const std::string &shaderCode, GLenum shaderType ) :
  _object(0),
  _referenceCount(NULL)
{
  _object = glCreateShader(shaderType);
  if (_object == 0) {

  }
  const char* code = shaderCode.c_str();
  glShaderSource(_object, 1, (const GLchar**)&code, NULL);

  // compile shader
  glCompileShader(_object);

  GLint status;
  glGetShaderiv(_object, GL_COMPILE_STATUS, &status);

  // if shader failed to compile:
  if (status == GL_FALSE) {
    std::string msg("shader failed to compile.\n");
    GLint infoLogLength;
    // returns a parameter from a shader.
    glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
    char *strInfoLog = new char[infoLogLength + 1]; // + 1 for the terminating null byte
    glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
    msg += strInfoLog;
    delete[] strInfoLog;

    glDeleteShader(_object);
    _object = 0;
    throw std::runtime_error(msg); 
  }

  _referenceCount = new unsigned;
  *_referenceCount = 1;
}

Shader::Shader(const Shader& other) : 
  _object(other._object),
  _referenceCount(other._referenceCount)
{
  _retain();
}

Shader::~Shader()
{
  if (_referenceCount) _release();
}

Shader& Shader::operator = (const Shader& other) {
  _release();
  _object = other._object;
  _referenceCount = other._referenceCount;
  _retain();
  return *this;
}

Shader Shader::shaderFromFile(const std::string& file, GLenum shaderType) {
  // open file
  std::ifstream f;
  f.open(file.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    throw std::runtime_error(std::string("failed to open file: ") + file);
  }

  std::stringstream buffer;
  buffer << f.rdbuf();
  Shader shader(buffer.str(), shaderType);
  return shader;
}

GLuint Shader::object() const {
  return _object;
}

void Shader::_retain() {
  assert(_referenceCount);
  *_referenceCount += 1;
}

void Shader::_release() {
  assert(_referenceCount && *_referenceCount > 0);
  *_referenceCount -= 1;
  if (*_referenceCount == 0) { 
    glDeleteShader(_object); _object = 0;
    delete _referenceCount; _referenceCount = NULL;
  } 
}
