
namespace Engine {

  class ResourceDeity {
    typedef PODHashMap<uint32_t, ResourceManager*> ResourceManagerTable;

    public:
      static const uint32_t MATERIAL = 0x11111111;
      static const uint32_t MODEL = 0x22222222;
      static const uint32_t SHADER = 0x33333333;
      static const uint32_t TEXTURE = 0x44444444;
      static const uint32_t ANIMATION = 0x55555555;  
      static const uint32_t PHYSICS = 0x66666666;

      ResourceDeity(Allocator& allocator);
      ~ResourceDeity();

      void create(FS::FileSystem& fs);
      void destroy(void);

      Allocator& getAllocator() { return _allocator; }
      ResourceManager* get(uint32_t id);

      void add(uint32_t id, ResourceManager* manager);
      void remove(uint32_t id);
      void reload(const char* path);
      bool isLoading() const;
      void incrementLoadingResources();
      void decrementLoadingResources();

      FS::FileSystem& getFileSystem() { return *_file_system; }

    private:
      Allocator& _allocator;
      ResourceManagerTable _resource_manager_table;
      FS::FileSystem& _file_system;
      int _loading_resources_count;
  };

}
