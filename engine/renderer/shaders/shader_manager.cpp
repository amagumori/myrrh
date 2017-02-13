#include "shader_manager.h"

namespace Engine {

  ShaderManager::ShaderManager(Renderer& renderer, Allocator& allocator)
    : ResourceManager(allocator)
    , _allocator(allocator)
    , _renderer(renderer) 
  {
    _buffer = nullptr;
    _buffer_size = -1;
  }

  ShaderManager::~ShaderManager() {
    _allocator.deleteObject(_buffer);
  } 

  Resource* ShaderManager::createResource(const Path& path) {
    return _allocator.newObject<Shader>(path, getOwner(), _allocator);
  }

  void ShaderManager::destroyResource(Resource& resource) {
    _allocator.deleteObject(static_cast<Shader*>(&resource));
  }

  uint8_t* ShaderManager::getBuffer(int32_t size) {
    if ( _buffer_size < size ) {
      _allocator.deleteObject(_buffer);
      _buffer = nullptr;
      _buffer_size = -1;
    }
    if ( _buffer == nullptr ) {
      _buffer = (uint8_t*)_allocator.allocate(sizeof(uint8_t) * size);
      _buffer_size = size;
    }
    return _buffer;
  }

  

}
