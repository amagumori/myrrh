#include "renderer.h"

using namespace Engine;


void setState(RenderState state) {
  _render_state = state;
}

// putting all uniforms in a UBO.  GPUs optimize better when putting uniforms in UBOs
// vs putting them in an SSBO, where it's treated more like a texture read.
// SSBOs if you need to write to the buffer...

struct uniformBlock {

};

typedef uboIndex uint16_t;

uniformBufferHandle createUniformBuffer( uniformBlock& block ) {
  // init
  GLuint ubo = 0;
  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(block), block, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // update / assign
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
  memcpy(p, block, sizeof(block));
  glUnmapBuffer(GL_UNIFORM_BUFFER);
}

ssboHandle createSSBO( void ) {

}

uboIndex createUniform(const char* name, UniformType type) {
      
}

void destroyUniform(uniformHandle handle) {

}

void setUniform(uniformHandle handle, const void* data) {

}

// static index buf

IndexBufferHandle createIndexBuffer(const void* data);
void destroyIndexBuffer(IndexBufferHandle handle);
void setIndexBuffer(IndexBufferHandle handle, const void* value, u32 size);

// static vertex buf

VertexBufferHandle createVertexBuffer(const void* data, const VertexSpec& spec);
void destroyVertexBuffer(VertexBufferHandle handle);
void setVertexBuffer(VertexBufferHandle handle);
void setVertexBuffer(VertexBufferHandle handle, u32 firstVertex, u32 size);
void setVertexBuffer(DynamicVertexBufferHandle handle, u32 size);

// dynamic index buf - using MAP_PERSISTENT_BIT 

DynamicIndexBufferHandle createDynamicIndexBuffer(const void* data);
void updateDynamicIndexBuffer(DynamicIndexBufferHandle handle, const void* data, u32 index);
void destroyDynamicIndexBuffer(DynamicIndexBufferHandle handle);

// dynamic vertex buf

DynamicVertexBufferHandle createDynamicVertexBuffer(u32 size, VertexSpec& spec);
void updateDynamicVertexBuffer(DynamicVertexBufferHandle handle, const void* data, u32 index);
void destroyDynamicVertexBuffer(DynamicVertexBufferHandle handle);

// multi_draw_indirect buf

indirectBufferHandle createIndirectBufferHandle(const void* data);
void destroyIndirectBufferHandle(indirectBufferHandle handle);

// just passing the shader program handle for now..
u32 draw(programHandle program); 


