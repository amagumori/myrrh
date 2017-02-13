#pragma once

#include <GL/glew.h>
#include "Bitmap.h"

namespace Engine {
  class Texture {
    public:
      // constructor:
      // creates a texture from a bitmap.
      // const Bitmap &
      // minMagFilter - GL_LINEAR or GL_NEAREST
      // wrapMode - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, or GL_CLAMP_TO_BORDER
      Texture( const Bitmap &bitmap,
               GLint minMagFilter = GL_LINEAR,
               GLint wrapMode = GL_CLAMP_TO_EDGE );
 
      ~Texture();

      GLuint object() const;

      GLfloat originalWidth() const;

      GLfloat originalHeight() const;

    private:
      Allocator& _allocator;

      GLuint _object;
      GLfloat _originalWidth;
      GLfloat _originalHeight;
      int _bpp;
      int _depth;

      // disable copying
      Texture(const Texture&);
      const Texture& operator=(const Texture&);
  };
}
