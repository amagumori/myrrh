#include "resource.h"
#include "../core/pod_hashmap.h"
#include "../core/allocator.h"

namespace Engine {

  class ResourceManager {
    typedef PODHashMap<uint32_t, Resource*> ResourceTable;

    public:
      void create(uint32_t id, ResourceDeity& owner);
      void destroy(void);

      Resource* get(const Path& path);
      Resource* load(const Path& path); 

      void add(Resource* resource);
      void remove(Resource* resource);
      void load(Resource& resource);

      void unload(const Path& path);
      void unload(Resource& resource);

      void reload(const Path& path);
      void reload(Resource& resource);

      ResourceManager(Allocator& allocator);      
      virtual ~ResourceManager(void); 

    protected:
      virtual Resource *createResource(const Path& path) = 0;
      virtual void destroyResource(Resource& resource) = 0;
      ResourceDeity& getOwner() const { return *_owner; }
    private:
      uint32_t _size;
      ResourceTable _resources;
      ResourceDeity* _owner;
  };

}
