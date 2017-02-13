#include <GL/glew.h>
// including types.h should include <stdint.h>????
#include "../core/types.h"
#include "../core/array.h"
#include "../core/aabb.h"
#include "meshes/Mesh.h"
#include "textures/Material.h"
#include "camera.h"

namespace Engine {

  // could refactor these structs to inherit
  // as long as you don't use virtuals.
  // 
  // for now im not having Meshes contain Materials.

  static const int invalidHandle = UINT16_MAX;

  #define HANDLE(_foo) \
    struct _foo { u16 index; }; \

  HANDLE(uniformHandle);
  HANDLE(indexBufferHandle);
  HANDLE(dynamicIndexBufferHandle);
  HANDLE(vertexBufferHandle);
  HANDLE(dynamicVertexBufferHandle);
  HANDLE(indirectBufferHandle);
  HANDLE(shaderHandle);


  struct renderableMesh {
    u64 key;
    Mesh *mesh;
    AABB *boundingBox;
    Array<Material*> materials;
  };

  struct renderableSkinnedMesh {
    u64 key;
    Mesh *mesh;
    AABB *boundingBox;
    Skeleton *skel;
    Array<Material*> materials;
  };

  // static meshes can contain multiple Mesh instances 
  // so that i can render several meshes in one draw call.
  struct renderableStaticMesh {
    u64 key;
    Array<Mesh*> meshes;
    AABB *boundingBox;
    Array<Material*> materials;
  };

  class RendererSystem {
      typedef HashMap<int, uint64_t> RenderableCache;
  
    public:
      void setup();
      void update();
      void render();

    private:
      // "parent" references
      Engine& _engine;
      Allocator& _allocator;
      Renderer& _renderer;    // ?????
      // should this be a stack allocator or linear allocator?
      StackAllocator& _stack_allocator;
      LinearAllocator& _frame_allocator;
      Camera _camera;
      RenderState _render_state;
      // stuff to render
      RenderableCache _dynamic_renderable_cache; 
      RenderableCache _static_renderable_cache; 
      Array<Renderable*> _renderables;
      Array<PointLight> _point_lights;
      Array<GlobalLight> _global_lights;
      // hrmrmrmr
      Array<uniformHandle> _uniforms;
      
      // view frustum
      Frustum _camera_frustum;
      CullingSystem* _culling_system;
  };

  void renderSceneNaive(void) {
    for (renderable : _renderables) {
      if(_camera_frustum.testAABB(renderable)) {
        _dynamic_renderable_cache.push(renderable);
      }
    }

    for (renderable : _dynamic_renderable_cache) {
      glGenVertexArrays(1, &renderable.mesh.vao);
      glBindVertexArray(&renderable.mesh.vao);
      glGenBuffers(1, &renderable.mesh.vbo);

      glBindBuffer(GL_ARRAY_BUFFER, &renderable.mesh.vbo);
      glBufferData(GL_ARRAY_BUFFER, &renderable.mesh.count * 4 * sizeof(float), &renderable.mesh.vertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);

    }
  }

  void drawRigidMesh( renderableMesh& mesh ) {

  }

  void drawSkinnedMesh( renderableSkinnedMesh& mesh ) {
    
  }

  void drawMapUnsynchronized( renderableMesh& mesh ) {

  }


  /* don't even worry about this shit for now 

  void initMapPersistent( void ) {
    for (auto u : _persistently_mapped_uniforms ) {
      glGenBuffers(1, &u.uniform);
    }
  }

  */

  void setMaterial (Material* material) {
    for (int i=0; i < material.numUniforms(); ++i) {
      const Material::Uniform& uniform = material->getUniform(i);

      switch(uniform._type) {
        case Material::Uniform::FLOAT: {
          vec4 v(uniform._float, 0, 0, 0);
          bgfx::setUniform(uniform._handle, &v);
          break;
        }
        default: 
          ASSERT(false);
          break;
      }
      Shader* shader = material->getShader();
      for (int i=0; i < material->numTextures(); ++i) {
        Texture* texture = material->getTexture(i);
        if (texture) {
          bgfx::setTexture(i, shader->getTextureSlot(i)._uniform_handle, texture->getHandle());
        }
      }


    }
  }


  enum RenderState {
    DRAW_RGB,
    DRAW_ALPHA,
    DRAW_DEPTH,
    SCISSOR_TEST,
    DEPTH_TEST
  };

  void setState(RenderState state);

  uniformHandle createUniform(const char* name, UniformType type);
  void destroyUniform(uniformHandle handle);
  void setUniform(uniformHandle handle, const void* data);

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

}
