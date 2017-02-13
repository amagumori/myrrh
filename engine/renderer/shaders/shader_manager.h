#include "../../core/resources/resource_manager.h"


namespace Engine {

  class ShaderManager : public ResourceManager {
    public:
      ShaderManager(Renderer& renderer, Allocator& allocator);
      ~ShaderManager();
      Renderer& getRenderer() { return _renderer; } 
      uint8_t* getBuffer(int32_t size);
  
    private:
      Allocator& _allocator;
      Renderer& _renderer;
      uint8_t* _buffer;
      int32_t _buffer_size;
  };


};
