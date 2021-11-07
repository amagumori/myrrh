#include "Texture.h"
#include "../shaders/Shader.h"

namespace Engine {

  struct Uniform {
    Uniform() {}

    enum Type {
      INT,
      FLOAT,
      MATRIX,
      TIME
    };
    
    static const int MAX_NAME_LENGTH = 32;
    char name[MAX_NAME_LENGTH + 1];
    uint32_t name_hash;
    Type type;
    bgfx::UniformHandle handle;
    union {
      int32_t _int;
      float _float;
      float _matrix[16];
    };
  };

  class Material {
    public:
      void setShader(Shader* shader);
      Shader* getShader() const { return _shader; }
      int numTextures() const { return _texture_count; }

      Texture* getTexture(int i) const { return i < _texture_count ? _textures[i] : nullptr; }
      const char* getTextureUniform(int i);
      Texture* getTextureByUniform(const char* uniform) const;
      void setTexture(int i, Texture* texture);
      int numUniforms() const { return _uniforms.size(); }
      Uniform& getUniform(int index) { return _uniforms[index]; }
      const Uniform& getUniform(int index) const { return _uniforms[index]; }
      
    private: 
      static const int MAX_TEXTURE_COUNT = 16;
      Shader* _shader;
      Texture* _textures[MAX_TEXTURE_COUNT];
      int _texture_count;
      Allocator& _allocator;
      Array<Uniform> _uniforms;
      
  };
}
